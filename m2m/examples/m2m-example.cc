/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2018 National University of Colombia
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Giuseppe Roa Osorio<groao@unal.edu.co>

// ns3 - M2M Protocol
// Giuseppe Roa Osorio, National University of Colombia, 2018

#include "ns3/m2m.h"
#include "ns3/m2m-helper.h"
#include "ns3/M2MAddress.h"
#include "ns3/M2MContext.h"
#include "ns3/M2MIntegrator.h"
#include "ns3/M2MPropagator.h"
#include "ns3/M2MState.h"
#include "ns3/M2MTerminal.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/position-allocator.h"
#include "ns3/netanim-module.h"
#include "ns3/trace-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/olsr-helper.h"
//#include "ns3/aodv-helper.h"
#include "ns3/dsdv-module.h"
#include "ns3/dsr-module.h"
#include "ns3/ipv4.h"
#include "ns3/ipv6.h"
#include "ns3/ipv4-global-routing.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-interface-container.h"
#include "ns3/ipv6-extension.h"
#include "ns3/ipv6-extension-demux.h"
#include "ns3/ipv6-extension-header.h"
#include "ns3/icmpv6-l4-protocol.h"
#include "ns3/global-router-interface.h"
#include "ns3/random-variable-stream.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/stats-module.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>
#include <vector>
#include <cstdlib>
#include <unistd.h>
//#include <stdlib.h> 
//#include <stdio.h>
//#include <math.h> 
//#include <tgmath.h>
//#include <cmath>

using namespace ns3;
using namespace std;
using namespace M2M_PSM_Controller;

NS_LOG_COMPONENT_DEFINE ("AdHocSimulation");
   
int main (int argc, char *argv[])
{ 
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        Basic simulation parameters
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  bool mobModel = true; // Mobility model (GaussMarkov or Randomwaypoint)
  bool IPv = true; // IP version
  double totalTime = 60.0; // simulation time seconds
  double FieldSize = 500.0; // Field size in m2
  double interval = 0.001; // seconds
  double PerMobil = 0.7; // percentage of nodes with mobility
  uint32_t Seed = 1;
  uint32_t Run = 1;
  uint32_t numNodes = 10; // number of nodes in the network
  uint32_t NumFailureNodes = 0;
  uint32_t packetSize = 64; // bytes
  uint32_t flows=4; // number of data flows in the network
  string animFile = "iterationx.xml";
  string pcapFile = "iterationx";
   
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        Command line arguments
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  CommandLine cmd;
  cmd.AddValue ("Seed", "Set seed to RNG", Seed);
  cmd.AddValue ("Run", "Set run simulation to RNG", Run);
  cmd.AddValue ("numNodes", "Number of nodes", numNodes);
  cmd.AddValue ("animFile", "Name of xml file", animFile);
  cmd.AddValue ("pcapFile", "Name of pcap file", pcapFile);
  cmd.AddValue ("totalTime", "Simulation time (s)", totalTime);
  cmd.AddValue ("FieldSize", "Field size (m2)", FieldSize);
  cmd.AddValue ("NumFailureNodes", "Number of failure nodes", NumFailureNodes);
  cmd.AddValue ("PerMobil", "Percentage of nodes with mobility", PerMobil);
  cmd.AddValue ("mobModel", "Mobility model", mobModel);
  cmd.AddValue ("IPv", "IP version", IPv);
  cmd.AddValue ("PacketSize", "Packet size in bytes", packetSize);
  cmd.AddValue ("flows", "Number of traffic flows", flows);
  cmd.Parse (argc, argv);
  
  SeedManager::SetSeed (Seed); 
  SeedManager::SetRun (Run); 
  string FS_String = to_string(FieldSize);
  
  Time interPacketInterval = Seconds (interval);// Convert to time object
  NodeContainer nodes;
  nodes.Create (round(numNodes*PerMobil));
  NodeContainer nodes1;
  nodes1.Create (round(numNodes*(1-PerMobil)));
  
  M2MContext m2mNodes[numNodes];
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                       MOBILITY MODELS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
MobilityHelper mobility1;
mobility1.SetPositionAllocator ("ns3::RandomRectanglePositionAllocator",
                                    "X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=" + FS_String + "]"),
                                    "Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=" + FS_String + "]"));
mobility1.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
mobility1.Install (nodes1);
if (mobModel == true)
{
MobilityHelper mobility;
mobility.SetPositionAllocator ("ns3::RandomRectanglePositionAllocator",
                                    "X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=" + FS_String + "]"),
                                    "Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=" + FS_String + "]"));
mobility.SetMobilityModel ("ns3::GaussMarkovMobilityModel",
  "Bounds", BoxValue (Box (0, FieldSize, 0, FieldSize, 0, FieldSize)),
  "TimeStep", TimeValue (Seconds (0.5)),
  "Alpha", DoubleValue (0.85),
  "MeanVelocity", StringValue ("ns3::UniformRandomVariable[Min=10|Max=90]"),
  "MeanDirection", StringValue ("ns3::UniformRandomVariable[Min=0|Max=7]"),
  "MeanPitch", StringValue ("ns3::UniformRandomVariable[Min=0.05|Max=0.05]"),
  "NormalVelocity", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.0|Bound=0.0]"),
  "NormalDirection", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.2|Bound=0.4]"),
  "NormalPitch", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.02|Bound=0.04]"));
mobility.Install (nodes);
} 
else {
MobilityHelper mobility;
ObjectFactory position;
position.SetTypeId ("ns3::RandomRectanglePositionAllocator");
position.Set ("X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=" + FS_String + "]"));
position.Set ("Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=" + FS_String + "]"));
Ptr<PositionAllocator> PositionAlloc = position.Create ()->GetObject<PositionAllocator> ();
mobility.SetMobilityModel ("ns3::RandomWaypointMobilityModel",
                                "Speed", StringValue ("ns3::ExponentialRandomVariable[Mean=10]"),
                                "Pause",StringValue ("ns3::ExponentialRandomVariable[Mean=5]"),
                                "PositionAllocator", PointerValue (PositionAlloc));
mobility.SetPositionAllocator (PositionAlloc);
mobility.Install (nodes);
}

NodeContainer allNodes; //Collect all nodes in a single container
allNodes.Add(nodes); 
allNodes.Add(nodes1); 
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                 Channel and network configuration
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  string phyMode ("DsssRate1Mbps");
  string rtslimit = "2200";
  //Defines the higher size of the PSDU Physical Layer Service Data Unite before fragmentation (bytes)
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  //If PSDU is bigger than this value, use RTS/CTS handshake before sending data(bytes)
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue (rtslimit));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue (phyMode));
 
  WifiHelper wifi;  // The below set of helpers will help us to put together the wifi NICs we want
  //Definition of physical layer (sending-receiving frames)
  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  wifiPhy.Set ("RxGain", DoubleValue (-20) ); // Reception gain (dB).
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); // ns-3 supports RadioTap and Prism tracing extensions for 802.11b

  //Definition of channel layer (getting signal to all connected physical layer)
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper mac, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));
  wifiMac.SetType ("ns3::AdhocWifiMac");// Set it to adhoc mode
  
  NetDeviceContainer adhoc;
  adhoc = wifi.Install (wifiPhy, wifiMac, allNodes);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                  Routing Algorithm - Address Allocation
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
 Ipv6InterfaceContainer allInterfacesIPv6;
 Ipv4InterfaceContainer allInterfacesIPv4;
 uint32_t IPv6Interface = 1;
 uint32_t IPv4Interface = 0; 
 if (IPv == true)
 {
 // Add IPv6
 InternetStackHelper internetv6Nodes;
 internetv6Nodes.SetIpv4StackInstall (false);
 
 // Add Routing Protocol
 //Ipv6ListRoutingHelper list;
 internetv6Nodes.Install (allNodes);
 
 // Add the addresses
 Ipv6AddressHelper ipv6;
 NS_LOG_INFO ("Assign IP Addresses.");
 ipv6.SetBase (Ipv6Address ("2001:db8::"), Ipv6Prefix (64));
 Ipv6InterfaceContainer ifcont = ipv6.Assign (adhoc);
 allInterfacesIPv6.Add(ifcont);

 
 ifcont.SetForwarding (IPv6Interface, true);
 ifcont.SetDefaultRouteInAllNodes (0);
/*
NS_LOG_INFO ("Specifically Assign an IPv6 Address.");
//Ipv6AddressHelper ipv6;
Ptr<NetDevice> device = adhoc.Get (1);
Ptr<Node> node = device->GetNode ();
Ptr<Ipv6> ipv6proto = node->GetObject<Ipv6> ();
int32_t ifIndex = 0;
ifIndex = ipv6proto->GetInterfaceForDevice (device);
Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:db8:f00d:cafe::42"), Ipv6Prefix (64));
ipv6proto->AddAddress (ifIndex, ipv6Addr);
  */
} 
else {
  
  // Add IPv4
  InternetStackHelper internet;
  
  // Add Routing Protocol
  Ipv4ListRoutingHelper list;
  NS_LOG_INFO ("OLSR protocol");
  OlsrHelper olsr; 
  list.Add (olsr, 100);
  internet.SetRoutingHelper (list); 
  internet.Install (allNodes);
 
 // Add the addresses   
 Ipv4AddressHelper ipv4;
 NS_LOG_INFO ("Assign IP Addresses.");
 ipv4.SetBase ("10.1.1.0", "255.255.255.0");
 Ipv4InterfaceContainer ifcont = ipv4.Assign (adhoc);
 allInterfacesIPv4.Add(ifcont);
 
 // Set up the actual simulation
 //Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        Random Traffic- (Poisson Model)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ostringstream stringRate;
stringRate <<packetSize;

/*
int min = 0;
int max = allNodes.GetN(); //-1;
Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
x->SetAttribute ("Min", DoubleValue (min));
x->SetAttribute ("Max", DoubleValue (max));

for ( uint32_t i = 0; i <flows ; i++)
   {
int source = x->GetValue ();
int sink =source;
while(sink==source){
    sink=x->GetValue ();
    }

  if (IPv == true){
    
  OnOffHelper onoff("ns3::UdpSocketFactory",Inet6SocketAddress (allInterfacesIPv6.GetAddress (source, IPv6Interface), i+9));
  //cout << "SourAddress="<<allInterfacesIPv6.GetAddress (source, IPv6Interface) << " Port="<<i+9 <<endl;
  //cout << "SinkAddress="<<allInterfacesIPv6.GetAddress (sink, IPv6Interface) << " Port="<<i+9 <<endl;
  onoff.SetAttribute ("OnTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1]"));
  onoff.SetAttribute ("OffTime",   StringValue ("ns3::ExponentialRandomVariable[Mean=1]"));
  onoff.SetAttribute ("PacketSize", StringValue(stringRate.str()));
  onoff.SetAttribute ("DataRate", StringValue("2048bps"));
  AddressValue remoteAddress (Inet6SocketAddress (allInterfacesIPv6.GetAddress (sink, IPv6Interface), i+9));
  onoff.SetAttribute ("Remote", remoteAddress);
  ApplicationContainer temp = onoff.Install (allNodes.Get (source));
  temp.Start (Seconds (5.0));    
  temp.Stop (Seconds (totalTime));
  }
  else{
    
  OnOffHelper onoff("ns3::UdpSocketFactory",InetSocketAddress (allInterfacesIPv4.GetAddress (source), i+9));
  //cout << "SourAddress="<<allInterfacesIPv4.GetAddress (source) << " Port="<<i+9 <<endl;
  //cout << "SinkAddress="<<allInterfacesIPv4.GetAddress (sink) << " Port="<<i+9 <<endl;
  onoff.SetAttribute ("OnTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1]"));
  onoff.SetAttribute ("OffTime",   StringValue ("ns3::ExponentialRandomVariable[Mean=1]"));
  onoff.SetAttribute ("PacketSize", StringValue(stringRate.str()));
  onoff.SetAttribute ("DataRate", StringValue("2048bps"));
  AddressValue remoteAddress (InetSocketAddress (allInterfacesIPv4.GetAddress (sink), i+9));
  onoff.SetAttribute ("Remote", remoteAddress);
  ApplicationContainer temp = onoff.Install (allNodes.Get (source));
  temp.Start (Seconds (5.0));    
  temp.Stop (Seconds (totalTime));
  }
}
*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        MANET DYNAMICS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
cout << "---------M2M PSM DYNAMICS------------------" <<endl;
// Uniform Random Failure Distributions ----------------------------------------
  uint32_t FailureNode = 0;
  uint32_t FailureTime = 0;
  
for (uint32_t i = 0; i < NumFailureNodes; ++i)
 { 
    Ptr<UniformRandomVariable> k = CreateObject<UniformRandomVariable> (); 
    k->SetAttribute ("Min", DoubleValue (0));
    k->SetAttribute ("Max", DoubleValue (numNodes));
    FailureNode = k->GetValue ();
 
    Ptr<UniformRandomVariable> j = CreateObject<UniformRandomVariable> (); 
    j->SetAttribute ("Min", DoubleValue (10));
    j->SetAttribute ("Max", DoubleValue (totalTime));
    FailureTime = j->GetValue ();

    if (IPv == true){
 
    Ptr<Node> n = allNodes.Get (FailureNode);
    Ptr<Ipv6> ipv6 = n->GetObject<Ipv6> ();
    Ipv6InterfaceAddress iaddr = ipv6->GetAddress (1,IPv6Interface);
    cout << "FailureNode="<<iaddr <<endl;
    cout << "It will fail at second="<<FailureTime <<endl;
    uint32_t ipv6ifIndex = 1;
    Simulator::Schedule (Seconds (FailureTime),&Ipv6::SetDown,ipv6, ipv6ifIndex);
    Simulator::Schedule (Seconds (FailureTime+30),&Ipv6::SetUp,ipv6, ipv6ifIndex);    
    }
    else {
     
    Ptr<Node> n = allNodes.Get (FailureNode);
    Ptr<Ipv4> ipv4 = n->GetObject<Ipv4> ();
    Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,IPv4Interface);
    cout << "FailureNode="<<iaddr <<endl;
    cout << "It will fail at second="<<FailureTime <<endl;
    uint32_t ipv4ifIndex = 1;
    Simulator::Schedule (Seconds (FailureTime),&Ipv4::SetDown,ipv4, ipv4ifIndex);
    Simulator::Schedule (Seconds (FailureTime+30),&Ipv4::SetUp,ipv4, ipv4ifIndex);
    } 
 }
// Uniform Random Resources Distribution ---------------------------------------
  
  /*
  NodeContainer Propagator;
  NodeContainer Integrator;
  NodeContainer Terminal;
  uint32_t ProbTerminal = 0;
  uint32_t ProbPropagator = 0;
  uint32_t ProbIntegrator = 0;
  
  for (uint32_t i = 0; i < numNodes; ++i)
    {
      Ptr<UniformRandomVariable> z = CreateObject<UniformRandomVariable> ();
      z->SetAttribute ("Min", DoubleValue (0));
      z->SetAttribute ("Max", DoubleValue (3));
      uint32_t Prob = z->GetValue ();
      switch ( Prob )  
      {  
         case 0:  
            Terminal.Add (allNodes.Get(i));
            m2mNodes[i].setResources(1);
            ProbTerminal++;
            break;  
         case 1:  
            Propagator.Add (allNodes.Get(i));
            m2mNodes[i].setResources(2);
            ProbPropagator++;
            break;  
         default:  
            Integrator.Add (allNodes.Get(i));
            m2mNodes[i].setResources(3);
            ProbIntegrator++;
      }  
    }
   */ 
/*  
// Uniform Random REDUNDANCY Distribution --------------------------------------
  NodeContainer RED2;
  NodeContainer RED4;
  NodeContainer RED6;
  NodeContainer RED10;
  NodeContainer RED18;
  NodeContainer RED31;
  NodeContainer RED55;
  NodeContainer RED98;
  
  uint32_t ProbRED2 = 0;
  uint32_t ProbRED4 = 0;
  uint32_t ProbRED6 = 0;
  uint32_t ProbRED10 = 0;
  uint32_t ProbRED18 = 0;
  uint32_t ProbRED31 = 0;
  uint32_t ProbRED55 = 0;
  uint32_t ProbRED98 = 0;

  unsigned int Red2[3] = {0,0,0};
  unsigned int Red4[3] = {0,0,1};
  unsigned int Red6[3] = {0,1,0};
  unsigned int Red10[3] = {0,1,1};
  unsigned int Red18[3] = {1,0,0};
  unsigned int Red31[3] = {1,0,1};
  unsigned int Red55[3] = {1,1,0};
  unsigned int Red98[3] = {1,1,1};
  
  for (uint32_t i = 0; i < numNodes; ++i)
    {
      Ptr<UniformRandomVariable> z = CreateObject<UniformRandomVariable> ();
      z->SetAttribute ("Min", DoubleValue (0));
      z->SetAttribute ("Max", DoubleValue (8));
      uint32_t Prob = z->GetValue ();
      switch ( Prob )  
      {  
         case 0:  
            RED2.Add (allNodes.Get(i));
            m2mNodes[i].set_Redu_Id(Red2);
            ProbRED2++;
            break;  
         case 1:  
            RED4.Add (allNodes.Get(i));
            m2mNodes[i].set_Redu_Id(Red4);
            ProbRED4++;
            break;  
         case 2:  
            RED6.Add (allNodes.Get(i));
            m2mNodes[i].set_Redu_Id(Red6);
            ProbRED6++;
            break;
         case 3:  
            RED10.Add (allNodes.Get(i));
            m2mNodes[i].set_Redu_Id(Red10);
            ProbRED10++;
            break;
         case 4:  
            RED18.Add (allNodes.Get(i));
            m2mNodes[i].set_Redu_Id(Red18);
            ProbRED18++;
            break;
         case 5:  
            RED31.Add (allNodes.Get(i));
            m2mNodes[i].set_Redu_Id(Red31);
            ProbRED31++;
            break;
         case 6:  
            RED55.Add (allNodes.Get(i));
            m2mNodes[i].set_Redu_Id(Red55);
            ProbRED55++;
            break;
         default:  
            RED98.Add (allNodes.Get(i));
            m2mNodes[i].set_Redu_Id(Red98);
            ProbRED98++;
      }  
    }
  // Uniform Random Application Distribution -----------------------------------
  NodeContainer OTHERS;
  NodeContainer IT_NET;
  NodeContainer SURVEI;
  NodeContainer EMERGE;
  NodeContainer TRACKI;
  NodeContainer RET_STO;
  NodeContainer RET_HOS;
  NodeContainer TRA_VEH;
  NodeContainer TRA_NNV;
  NodeContainer IND_DIS;
  NodeContainer RES_AUT;
  NodeContainer AGRICU;
  NodeContainer HEALTH;
  NodeContainer ENERGY;
  NodeContainer HOME_AU;
  NodeContainer ENTERT;
  
  uint32_t ProbOTHERS = 0;
  uint32_t ProbIT_NET = 0;
  uint32_t ProbSURVEI = 0;
  uint32_t ProbEMERGE = 0;
  uint32_t ProbTRACKI = 0;
  uint32_t ProbRET_STO = 0;
  uint32_t ProbRET_HOS = 0;
  uint32_t ProbTRA_VEH = 0;
  uint32_t ProbTRA_NNV = 0;
  uint32_t ProbIND_DIS = 0;
  uint32_t ProbRES_AUT = 0;
  uint32_t ProbAGRICU = 0;
  uint32_t ProbHEALTH = 0;
  uint32_t ProbENERGY = 0;
  uint32_t ProbHOME_AU = 0;
  uint32_t ProbENTERT = 0;
  
  unsigned int ID_others[4] = {0,0,0,0};
  unsigned int ID_it_net[4] = {0,0,0,1};
  unsigned int ID_survei[4] = {0,0,1,0};
  unsigned int ID_emerge[4] = {0,0,1,1};
  unsigned int ID_tracki[4] = {0,1,0,0};
  unsigned int ID_retsto[4] = {0,1,0,1};
  unsigned int ID_rethos[4] = {0,1,1,0};
  unsigned int ID_traveh[4] = {0,1,1,1};
  unsigned int ID_trannv[4] = {1,0,0,0};
  unsigned int ID_inddis[4] = {1,0,0,1};
  unsigned int ID_resaut[4] = {1,0,1,0};
  unsigned int ID_agricu[4] = {1,0,1,1};
  unsigned int ID_health[4] = {1,1,0,0};
  unsigned int ID_energy[4] = {1,1,0,1};
  unsigned int ID_homeau[4] = {1,1,1,0};
  unsigned int ID_entert[4] = {1,1,1,1};
    
  for (uint32_t i = 0; i < numNodes; ++i)
    {
      Ptr<UniformRandomVariable> z = CreateObject<UniformRandomVariable> ();
      z->SetAttribute ("Min", DoubleValue (0));
      z->SetAttribute ("Max", DoubleValue (16));
      uint32_t Prob = z->GetValue ();
      switch ( Prob )  
      {   
         case 0:  
            OTHERS.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_others);
            ProbOTHERS++;
            break;  
         case 1:  
            IT_NET.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_it_net);
            ProbIT_NET++;
            break;  
         case 2:  
            SURVEI.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_survei);
            ProbSURVEI++;
            break;
         case 3:  
            EMERGE.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_emerge);
            ProbEMERGE++;
            break;
         case 4:  
            TRACKI.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_tracki);
            ProbTRACKI++;
            break;
         case 5:  
            RET_STO.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_retsto);
            ProbRET_STO++;
            break;
         case 6:  
            RET_HOS.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_rethos);
            ProbRET_HOS++;
            break;
         case 7:  
            TRA_VEH.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_traveh);
            ProbTRA_VEH++;
            break;
         case 8:  
            TRA_NNV.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_trannv);
            ProbTRA_NNV++;
            break;
         case 9:  
            IND_DIS.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_inddis);
            ProbIND_DIS++;
            break;
         case 10:  
            RES_AUT.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_resaut);
            ProbRES_AUT++;
            break;
         case 11:  
            AGRICU.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_agricu);
            ProbAGRICU++;
            break;
         case 12:  
            HEALTH.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_health);
            ProbHEALTH++;
            break;
         case 13:  
            ENERGY.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_energy);
            ProbENERGY++;
            break;
         case 14:  
            HOME_AU.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_homeau);
            ProbHOME_AU++;
            break;
         default:  
            ENTERT.Add (allNodes.Get(i));
            m2mNodes[i].set_Appl_Id(ID_entert);
            ProbENTERT++;
      }  
    }
// Uniform Random KIND OF DEVICE Distribution ----------------------------------
  NodeContainer OTHER;
  NodeContainer TAG;
  NodeContainer PUMP;
  NodeContainer MOTOR;
  NodeContainer VALVE;
  NodeContainer ALARM;
  NodeContainer SEN_IM;
  NodeContainer SEN_EN;
  NodeContainer SEN_SE;
  NodeContainer SEN_WE;
  NodeContainer GENER;
  NodeContainer ACTUA;
  NodeContainer VEHIC;
  NodeContainer LIGHT;
  NodeContainer BATTE;
  NodeContainer FUELC;
  
  uint32_t ProbOTHER = 0;
  uint32_t ProbTAG = 0;
  uint32_t ProbPUMP = 0;
  uint32_t ProbMOTOR = 0;
  uint32_t ProbVALVE = 0;
  uint32_t ProbALARM = 0;
  uint32_t ProbSEN_IM = 0;
  uint32_t ProbSEN_EN = 0;
  uint32_t ProbSEN_SE = 0;
  uint32_t ProbSEN_WE = 0;
  uint32_t ProbGENER = 0;
  uint32_t ProbACTUA = 0;
  uint32_t ProbVEHIC = 0;
  uint32_t ProbLIGHT = 0;
  uint32_t ProbBATTE = 0;
  uint32_t ProbFUELC = 0;
  
  unsigned int ID_other[4] = {0,0,0,0};
  unsigned int ID_tag__[4] = {0,0,0,1};
  unsigned int ID_pump_[4] = {0,0,1,0};
  unsigned int ID_motor[4] = {0,0,1,1};
  unsigned int ID_valve[4] = {0,1,0,0};
  unsigned int ID_alarm[4] = {0,1,0,1};
  unsigned int ID_senim[4] = {0,1,1,0};
  unsigned int ID_senen[4] = {0,1,1,1};
  unsigned int ID_sense[4] = {1,0,0,0};
  unsigned int ID_senwe[4] = {1,0,0,1};
  unsigned int ID_gener[4] = {1,0,1,0};
  unsigned int ID_actua[4] = {1,0,1,1};
  unsigned int ID_vehic[4] = {1,1,0,0};
  unsigned int ID_light[4] = {1,1,0,1};
  unsigned int ID_batte[4] = {1,1,1,0};
  unsigned int ID_fuelc[4] = {1,1,1,1};
  
  for (uint32_t i = 0; i < numNodes; ++i)
    {
      Ptr<UniformRandomVariable> z = CreateObject<UniformRandomVariable> ();
      z->SetAttribute ("Min", DoubleValue (0));
      z->SetAttribute ("Max", DoubleValue (16));
      uint32_t Prob = z->GetValue ();
      switch ( Prob )  
      {  
         case 0:  
            OTHER.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_other);
            ProbOTHER++;
            break;  
         case 1:  
            TAG.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_tag__);
            ProbTAG++;
            break;  
         case 2:  
            PUMP.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_pump_);
            ProbPUMP++;
            break;
         case 3:  
            MOTOR.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_motor);
            ProbMOTOR++;
            break;
         case 4:  
            VALVE.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_valve);
            ProbVALVE++;
            break;
         case 5:  
            ALARM.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_alarm);
            ProbALARM++;
            break;
         case 6:  
            SEN_IM.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_senim);
            ProbSEN_IM++;
            break;
         case 7:  
            SEN_EN.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_senen);
            ProbSEN_EN++;
            break;
         case 8:  
            SEN_SE.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_sense);
            ProbSEN_SE++;
            break;
         case 9:  
            SEN_WE.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_senwe);
            ProbSEN_WE++;
            break;
         case 10:  
            GENER.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_gener);
            ProbGENER++;
            break;
         case 11:  
            ACTUA.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_actua);
            ProbACTUA++;
            break;
         case 12:  
            VEHIC.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_vehic);
            ProbVEHIC++;
            break;
         case 13:  
            LIGHT.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_light);
            ProbLIGHT++;
            break;
         case 14:  
            BATTE.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_batte);
            ProbBATTE++;
            break;
         default:  
            FUELC.Add (allNodes.Get(i));
            m2mNodes[i].set_Devi_Id(ID_fuelc);
            ProbFUELC++;
      }
    }
*/
  
  if (IPv == true){
    for (uint32_t i = 0; i < numNodes; ++i)
    {    
    M2MHelper m2mtest("ns3::UdpSocketFactory",Inet6SocketAddress (allInterfacesIPv6.GetAddress (i, IPv6Interface), i+9));
    m2mtest.SetAttribute ("OnTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1]"));
    m2mtest.SetAttribute ("OffTime",   StringValue ("ns3::ExponentialRandomVariable[Mean=1]"));
    m2mtest.SetAttribute ("PacketSize", StringValue(stringRate.str()));
    m2mtest.SetAttribute ("DataRate", StringValue("2048bps"));
    AddressValue remoteNode (Inet6SocketAddress (allInterfacesIPv6.GetAddress (1, IPv6Interface), i+9));
    m2mtest.SetAttribute ("Remote", remoteNode);
    ApplicationContainer m2mtemp = m2mtest.Install (allNodes.Get (i));
    m2mtemp.Start (Seconds (5.0));    
    m2mtemp.Stop (Seconds (totalTime));
    }
  }
    else{
    for (uint32_t i = 0; i < numNodes; ++i)
    { 
    M2MHelper m2mtest("ns3::UdpSocketFactory",InetSocketAddress (allInterfacesIPv4.GetAddress (i), i+9));
    m2mtest.SetAttribute ("OnTime", StringValue ("ns3::ExponentialRandomVariable[Mean=1]"));
    m2mtest.SetAttribute ("OffTime",   StringValue ("ns3::ExponentialRandomVariable[Mean=1]"));
    m2mtest.SetAttribute ("PacketSize", StringValue(stringRate.str()));
    m2mtest.SetAttribute ("DataRate", StringValue("2048bps"));
    AddressValue remoteNode (InetSocketAddress (allInterfacesIPv4.GetAddress (1), i+9));
    m2mtest.SetAttribute ("Remote", remoteNode);
    ApplicationContainer m2mtemp = m2mtest.Install (allNodes.Get (i));
    m2mtemp.Start (Seconds (5.0));    
    m2mtemp.Stop (Seconds (totalTime)); 
    }    
  }

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        Tracing and Visualization
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  wifiPhy.EnablePcap (pcapFile, adhoc); // Tracing
  //wifiPhy.EnablePcapAll (pcapFile); // Tracing
  AsciiTraceHelper ascii;
  Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream (pcapFile+".tr");
  wifiPhy.EnableAsciiAll (stream);
   
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        CUSTOM VISUALIZATION
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  AnimationInterface anim (animFile);   // XML animation file generation
  /*
  uint32_t resourceId[5];
  resourceId[0] = anim.AddResource ("/home/groao/Desktop/NS-3/Pruebas/Images/integrator.png");
  resourceId[1] = anim.AddResource ("/home/groao/Desktop/NS-3/Pruebas/Images/propagator.png");
  resourceId[2] = anim.AddResource ("/home/groao/Desktop/NS-3/Pruebas/Images/terminal_voltage.png");
  resourceId[3] = anim.AddResource ("/home/groao/Desktop/NS-3/Pruebas/Images/terminal_temperature.png");
  resourceId[4] = anim.AddResource ("/home/groao/Desktop/NS-3/Pruebas/Images/terminal_speed.png");
  */
  /*
    for (uint32_t i = 0; i < Terminal.GetN(); ++i)
    {
        uint32_t IdTerminal = Terminal.Get(i)->GetId();
        //anim.UpdateNodeImage (IdTerminal, resourceId[(rand() % 3) + 2]);
        anim.UpdateNodeSize (IdTerminal, 20, 20);
        anim.UpdateNodeColor (IdTerminal, 255, 0, 0);
    }  
  
    for (uint32_t i = 0; i < Propagator.GetN(); ++i)
    {  
        uint32_t IdPropagator = Propagator.Get(i)->GetId();
        //anim.UpdateNodeImage (IdPropagator, resourceId[1]); 
        anim.UpdateNodeSize (IdPropagator, 20, 20);
        anim.UpdateNodeColor (IdPropagator, 0, 0, 255);
    }
    
    for (uint32_t i = 0; i < Integrator.GetN(); ++i)
    {
        uint32_t IdIntegrator = Integrator.Get(i)->GetId();
        //anim.UpdateNodeImage (IdIntegrator, resourceId[0]);
        anim.UpdateNodeSize (IdIntegrator, 20, 20);
        anim.UpdateNodeColor (IdIntegrator, 0, 255, 0);
    }     
  */
    for (uint32_t i = 0; i < numNodes; ++i)
    {
        uint32_t Id = allNodes.Get(i)->GetId();
        anim.UpdateNodeSize (Id, 20, 20);
        anim.UpdateNodeColor (Id, 255, 0, 0);
    } 
  
  anim.EnablePacketMetadata (); // Optional
  //(PWD,X,Y,ESX,ESY,OPAC)
  anim.SetBackgroundImage ("/home/groao/Desktop/NS-3/Pruebas/Images/background.jpg", 0, 0, 0.5, 0.5, 0.5); 
  
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        RUN SIMULATION
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
  Simulator::Stop(Seconds(totalTime));  // Simulation time
  Simulator::Run ();
  /*
  cout << "---------ROLE DISTRIBUTION-----------------" <<endl;
  cout << "Termi="<<ProbTerminal << "\t Propa="<<ProbPropagator << "\t Integ="<<ProbIntegrator <<endl;
  cout << "---------REDUNDANCY DISTRIBUTION-----------" <<endl;
  cout << "RED2="<<ProbRED2 << " \t RED4="<<ProbRED4 << "  \t RED6="<<ProbRED6 <<endl;
  cout << "RED10="<<ProbRED10 << "\t RED18="<<ProbRED18 << "\t RED31="<<ProbRED31 <<endl;
  cout << "RED55="<<ProbRED55 << "\t RED98="<<ProbRED98 <<endl; 
  cout << "---------APPLICATION DISTRIBUTION----------" <<endl;
  cout << "OTHERS="<<ProbOTHERS << "\t IT_NET="<<ProbIT_NET << "\t SURVEI="<<ProbSURVEI <<endl;
  cout << "EMERGE="<<ProbEMERGE << "\t TRACKI="<<ProbTRACKI << "\t RET_STO="<<ProbRET_STO <<endl;
  cout << "RET_HOS="<<ProbRET_HOS << "\t TRA_VEH="<<ProbTRA_VEH << "\t TRA_NNV="<<ProbTRA_NNV <<endl;
  cout << "IND_DIS="<<ProbIND_DIS << "\t REA_AUT="<<ProbRES_AUT << "\t AGRICU="<<ProbAGRICU <<endl;
  cout << "HEALTH="<<ProbHEALTH << "\t ENERGY="<<ProbENERGY << "\t HOME_AU="<<ProbHOME_AU <<endl;
  cout << "ENTER="<<ProbENTERT <<endl;
  cout << "---------KIND OF DEVICE DISTRIBUTION-------" <<endl;
  cout << "OTHER="<<ProbOTHER << "\t TAG="<<ProbTAG << "\t\t PUMP="<<ProbPUMP <<endl;
  cout << "MOTOR="<<ProbMOTOR << "\t VALVE="<<ProbVALVE << "\t ALARM="<<ProbALARM <<endl;
  cout << "SEN_IM="<<ProbSEN_IM << " SEN_EN="<<ProbSEN_EN << "\t SEN_SE="<<ProbSEN_SE <<endl;
  cout << "SEN_WE="<<ProbSEN_WE << " GENER="<<ProbGENER << "\t ACTUA="<<ProbACTUA <<endl;
  cout << "VEHIC="<<ProbVEHIC << "\t LIGHT="<<ProbLIGHT << "\t BATTE="<<ProbBATTE <<endl;
  cout << "FUELC="<<ProbFUELC <<endl;
  */
  Simulator::Destroy ();
  return 0;
}