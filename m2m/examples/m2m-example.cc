/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// A module for create by Virtualization TLON Distributed System
// Copycenter (C) 2019
// Giuseppe Roa Osorio <groao@unal.edu.co>
// Grupo de investigación TLÖN
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser Public License for more details.
//
// You should have received a copy of the GNU Lesser Public License
// along with this program.  If not, see [http://www.gnu.org/licenses/].

//__author__ = "Giuseppe Roa Osorio"
//__copyright__ = "Copyright 2019, TLON group"
//__license__ = "LGPL"
//__version__ = "1"
//__email__ = "groao@unal.edu.co"
//__status__ = "Development"

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
  uint32_t packetSize = 64; // Packet size in bytes
  uint32_t flows=100; // number of data flows in the network
  uint32_t traffic = 0;
  string animFile = "iteration.xml";
  string pcapFile = "iteration_trace.pcap";
  string netanimFile = "iterationx.xml";
   
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
  cmd.AddValue ("traffic", "Type of traffic", traffic);
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
cout << "----------------MOBILITY MODEL--------------------" <<endl;
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
cout << "GaussMarkov Mobility Model installed on: " << round(numNodes*PerMobil) << " nodes" <<endl;
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
cout << "RandomWaypoint Mobility Model installed on: " << round(numNodes*PerMobil) << " nodes" <<endl;
}

NodeContainer allNodes; //Collect all nodes in a single container
allNodes.Add(nodes); 
allNodes.Add(nodes1); 
cout << "---------------------------------------------------" <<endl;
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
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); 

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
//*/
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
//%                        TRAFFIC MODEL
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
cout << "---------------------TRAFFIC MODEL------------------" <<endl;
ostringstream stringRate;
stringRate << packetSize;

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

unsigned int Prob = 0; 
Ptr<NormalRandomVariable> m = CreateObject<NormalRandomVariable> ();
m->SetAttribute ("Mean", DoubleValue (3.5));
m->SetAttribute ("Variance", DoubleValue (1.0));
 
Ptr<ExponentialRandomVariable> n = CreateObject<ExponentialRandomVariable> ();
n->SetAttribute ("Mean", DoubleValue (3.5));
n->SetAttribute ("Bound", DoubleValue (7.0));
 
Ptr<GammaRandomVariable> o = CreateObject<GammaRandomVariable> ();
o->SetAttribute ("Alpha", DoubleValue (1.8));
o->SetAttribute ("Beta", DoubleValue (1.8));
 
Ptr<UniformRandomVariable> p = CreateObject<UniformRandomVariable> ();
p->SetAttribute ("Min", DoubleValue (0));
p->SetAttribute ("Max", DoubleValue (7));
 
///*
switch ( traffic ) {
case 0:
  Prob = m->GetValue ();
  cout << "----------NORMAL DISTRIBUTED----------------------" <<endl;
  break;
case 1:
  Prob = n->GetValue ();
  cout << "-------------POISSON TRAFFIC----------------------" <<endl;
  break;
case 2:  
  Prob = o->GetValue ();
  cout << "--------------SELF-SIMILAR------------------------" <<endl;
  break;
default:
  Prob = p->GetValue ();
  cout << "--------------UNIFORM-----------------------------" <<endl;
  break;
}//*/
 
 uint32_t psize[8] = {32,64,128,256,512,1024,2048,4096};
 packetSize = psize[Prob];
 stringRate.str("");
 stringRate << packetSize;
 cout << "PacketSize="<< packetSize << endl;
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
cout << "---------------------------------------------------" <<endl;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        MANET DYNAMICS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
cout << "---------------------M2M DYNAMICS------------------" <<endl;
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
///*
for (uint32_t i = 0; i < numNodes; ++i)
 {
      Ptr<Node> n = allNodes.Get (i);
      Ptr<Ipv6> ipv6 = n->GetObject<Ipv6> ();
      Ipv6InterfaceAddress iaddr = ipv6->GetAddress (1,IPv6Interface);
      cout << "IP_="<<iaddr <<endl;
 }//*/
  
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
cout << "---------------------------------------------------" <<endl;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        TRACING
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//wifiPhy.EnablePcap (pcapFile, adhoc); // Tracing
//wifiPhy.EnablePcapAll (pcapFile); // Tracing
//AsciiTraceHelper ascii;
//Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream (pcapFile+".tr");
//wifiPhy.EnableAsciiAll (stream);
  
// Flow monitor
Ptr<FlowMonitor> flowMonitor;
FlowMonitorHelper flowHelper;
flowMonitor = flowHelper.InstallAll();
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        CUSTOM VISUALIZATION
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
AnimationInterface anim (netanimFile);   // XML animation file generation
anim.SetBackgroundImage ("Images/background.jpg", 0, 0, 0.5, 0.5, 0.9); //(PWD,X,Y,ESX,ESY,OPAC)
anim.EnablePacketMetadata (); // Optional

uint32_t resourceId[5];
resourceId[0] = anim.AddResource ("Images/integrator.png");
resourceId[1] = anim.AddResource ("Images/propagator.png");
resourceId[2] = anim.AddResource ("Images/terminal_voltage.png");
resourceId[3] = anim.AddResource ("Images/terminal_temperature.png");
resourceId[4] = anim.AddResource ("Images/terminal_speed.png");
resourceId[5] = anim.AddResource ("Images/terminal_humid.png");
resourceId[6] = anim.AddResource ("Images/terminal_eolic.png");
  
for (uint32_t i = 0; i < allNodes.GetN(); ++i)
{
    uint32_t IdDevice = allNodes.Get(i)->GetId();
    anim.UpdateNodeImage (IdDevice, resourceId[0]);
    anim.UpdateNodeImage (IdDevice, resourceId[1]); 
    anim.UpdateNodeImage (IdDevice, resourceId[(rand() % 5) + 2]);
    anim.UpdateNodeSize (IdDevice, 15, 15);
    //anim.UpdateNodeColor (Id, 255, 0, 0);
}//*/  
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                        RUN SIMULATION
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
  Simulator::Stop(Seconds(totalTime));  // Simulation time
  Simulator::Run ();
  
  flowMonitor->SerializeToXmlFile(animFile, false, false);

  Simulator::Destroy ();
  return 0;
}