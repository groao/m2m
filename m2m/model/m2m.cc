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

#include "ns3/log.h"
#include "ns3/address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/packet-socket-address.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/random-variable-stream.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/string.h"
#include "ns3/pointer.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"

#include "m2m.h"
#include "ns3/m2mHeader.h"
#include "ns3/M2MAddress.h"
#include "ns3/M2MContext.h"
#include "ns3/M2MIntegrator.h"
#include "ns3/M2MPropagator.h"
#include "ns3/M2MState.h"
#include "ns3/M2MTerminal.h"

using namespace M2M_PSM_Controller;

M2MContext m2mNode;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("M2MApplication");

NS_OBJECT_ENSURE_REGISTERED (M2MApplication);

TypeId
M2MApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::M2MApplication")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<M2MApplication> ()
    .AddAttribute ("DataRate", "The data rate in on state.",
                   DataRateValue (DataRate ("500kb/s")),
                   MakeDataRateAccessor (&M2MApplication::m_cbrRate),
                   MakeDataRateChecker ())
    .AddAttribute ("PacketSize", "The size of packets sent in on state",
                   UintegerValue (512),
                   MakeUintegerAccessor (&M2MApplication::m_pktSize),
                   MakeUintegerChecker<uint32_t> (1))
    .AddAttribute ("Remote", "The address of the destination",
                   AddressValue (),
                   MakeAddressAccessor (&M2MApplication::m_peer),
                   MakeAddressChecker ())
    .AddAttribute ("OnTime", "A RandomVariableStream used to pick the duration of the 'On' state.",
                   StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"),
                   MakePointerAccessor (&M2MApplication::m_onTime),
                   MakePointerChecker <RandomVariableStream>())
    .AddAttribute ("OffTime", "A RandomVariableStream used to pick the duration of the 'Off' state.",
                   StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"),
                   MakePointerAccessor (&M2MApplication::m_offTime),
                   MakePointerChecker <RandomVariableStream>())
    .AddAttribute ("MaxBytes", 
                   "The total number of bytes to send. Once these bytes are sent, "
                   "no packet is sent again, even in on state. The value zero means "
                   "that there is no limit.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&M2MApplication::m_maxBytes),
                   MakeUintegerChecker<uint64_t> ())
    .AddAttribute ("Protocol", "The type of protocol to use.",
                   TypeIdValue (UdpSocketFactory::GetTypeId ()),
                   MakeTypeIdAccessor (&M2MApplication::m_tid),
                   MakeTypeIdChecker ())
    .AddTraceSource ("Tx", "A new packet is created and is sent",
                     MakeTraceSourceAccessor (&M2MApplication::m_txTrace),
                     "ns3::Packet::TracedCallback")
  ;
  return tid;
}


M2MApplication::M2MApplication ()
  : m_socket (0),
    m_connected (false),
    m_residualBits (0),
    m_lastStartTime (Seconds (0)),
    m_totBytes (0)
{
  NS_LOG_FUNCTION (this);
}

M2MApplication::~M2MApplication()
{
  NS_LOG_FUNCTION (this);
}

void 
M2MApplication::SetMaxBytes (uint64_t maxBytes)
{
  NS_LOG_FUNCTION (this << maxBytes);
  m_maxBytes = maxBytes;
}

Ptr<Socket>
M2MApplication::GetSocket (void) const
{
  NS_LOG_FUNCTION (this);
  return m_socket;
}

int64_t 
M2MApplication::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_onTime->SetStream (stream);
  m_offTime->SetStream (stream + 1);
  return 2;
}

void
M2MApplication::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  m_socket = 0;
  // chain up
  Application::DoDispose ();
}

// Application Methods
void M2MApplication::StartApplication () // Called at time specified by Start
{
  NS_LOG_FUNCTION (this);
  
  // Uniform Random Distribution -------------------------------------------------
  int dis = 3;
  unsigned int Prob = 0;
  unsigned int array[3] = {0,0,0};
  unsigned int matrix[8][3] = {
    0,0,0,
    0,0,1,
    0,1,0,
    0,1,1,
    1,0,0,
    1,0,1,
    1,1,0,
    1,1,1 
    };
  
  unsigned int array1[4] = {0,0,0,0};
  unsigned int array2[4] = {0,0,0,0};
  unsigned int matrix1[16][4] = {
    0,0,0,0,
    0,0,0,1,
    0,0,1,0,
    0,0,1,1,
    0,1,0,0,
    0,1,0,1,
    0,1,1,0,
    0,1,1,1,
    1,0,0,0,
    1,0,0,1,
    1,0,1,0,
    1,0,1,1,
    1,1,0,0,
    1,1,0,1,
    1,1,1,0,
    1,1,1,1   
    };
    
  if (dis == 1)  {
// Uniform Random Resources Distribution ---------------------------------------    
  Ptr<UniformRandomVariable> z = CreateObject<UniformRandomVariable> ();
  z->SetAttribute ("Min", DoubleValue (0));
  z->SetAttribute ("Max", DoubleValue (3));
  Prob = z->GetValue () + 1;
  m2mNode.setResources(Prob);

// Uniform Random Redundancy Distribution --------------------------------------    
  Ptr<UniformRandomVariable> y = CreateObject<UniformRandomVariable> ();
  y->SetAttribute ("Min", DoubleValue (0));
  y->SetAttribute ("Max", DoubleValue (8));
  Prob = y->GetValue ();
  for(int i=0; i < 3; i++) {
    array[i] = matrix[Prob][i]; 
  }
  m2mNode.set_Redu_Id(array);
  
// Uniform Random Application Distribution -------------------------------------
  Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
  x->SetAttribute ("Min", DoubleValue (0));
  x->SetAttribute ("Max", DoubleValue (16));
  Prob = x->GetValue ();
  for(int i=0; i < 4; i++) {
    array1[i] = matrix1[Prob][i]; 
  }
  m2mNode.set_Appl_Id(array1);
   
// Uniform Random Kind of Device Distribution ----------------------------------
  Ptr<UniformRandomVariable> w = CreateObject<UniformRandomVariable> ();
  w->SetAttribute ("Min", DoubleValue (0));
  w->SetAttribute ("Max", DoubleValue (16));
  Prob = w->GetValue ();
  for(int i=0; i < 4; i++) {
    array2[i] = matrix1[Prob][i]; 
  }
  m2mNode.set_Devi_Id(array2); 
  
  }
  else if (dis == 2) {
// Exponential Random Resources Distribution -----------------------------------   
  Ptr<ExponentialRandomVariable> z = CreateObject<ExponentialRandomVariable> ();
  z->SetAttribute ("Mean", DoubleValue (1));
  z->SetAttribute ("Bound", DoubleValue (3));
  Prob = z->GetValue ()+1;
  m2mNode.setResources(Prob); 
  
// Exponential Random Redundancy Distribution ----------------------------------  
  Ptr<ExponentialRandomVariable> y = CreateObject<ExponentialRandomVariable> ();
  y->SetAttribute ("Mean", DoubleValue (5));
  y->SetAttribute ("Bound", DoubleValue (8));
  Prob = y->GetValue ();
  if (Prob > 7){Prob = 7;}
  for(int i=0; i < 3; i++) {
    array[i] = matrix[Prob][i]; 
  }
  m2mNode.set_Redu_Id(array);
  
// Exponential Random Application Distribution ---------------------------------
  Ptr<ExponentialRandomVariable> x = CreateObject<ExponentialRandomVariable> ();
  x->SetAttribute ("Mean", DoubleValue (7));
  x->SetAttribute ("Bound", DoubleValue (16));
  Prob = x->GetValue ();
  if (Prob > 15){Prob = 15;}
  for(int i=0; i < 4; i++) {
    array1[i] = matrix1[Prob][i]; 
  }
  m2mNode.set_Appl_Id(array1);
   
// Exponential Random Kind of Device Distribution ------------------------------
  Ptr<ExponentialRandomVariable> w = CreateObject<ExponentialRandomVariable> ();
  w->SetAttribute ("Mean", DoubleValue (7));
  w->SetAttribute ("Bound", DoubleValue (16));
  Prob = w->GetValue ();
  if (Prob > 15){Prob = 15;}
  for(int i=0; i < 4; i++) {
    array2[i] = matrix1[Prob][i]; 
  }
  m2mNode.set_Devi_Id(array2); 
  
  }
  else {
  Ptr<GammaRandomVariable> z = CreateObject<GammaRandomVariable> ();
  z->SetAttribute ("Alpha", DoubleValue (1.7));
  z->SetAttribute ("Beta", DoubleValue (0.5));
  Prob = z->GetValue ()+1;
  m2mNode.setResources(Prob); 
  
// Gamma Random Redundancy Distribution --------------------------------------       
  Ptr<GammaRandomVariable> y = CreateObject<GammaRandomVariable> ();
  y->SetAttribute ("Alpha", DoubleValue (4));
  y->SetAttribute ("Beta", DoubleValue (0.8));
  Prob = y->GetValue ();
  if (Prob > 7){Prob = 7;}
  for(int i=0; i < 3; i++) {
    array[i] = matrix[Prob][i]; 
  }
  m2mNode.set_Redu_Id(array);
  
// Gamma Random Application Distribution -------------------------------------
  Ptr<GammaRandomVariable> x = CreateObject<GammaRandomVariable> ();
  x->SetAttribute ("Alpha", DoubleValue (10));
  x->SetAttribute ("Beta", DoubleValue (0.8));
  Prob = x->GetValue ();
  if (Prob > 15){Prob = 15;}
  for(int i=0; i < 4; i++) {
    array1[i] = matrix1[Prob][i]; 
  }
  m2mNode.set_Appl_Id(array1);
   
// Gamma Random Kind of Device Distribution ----------------------------------
  Ptr<GammaRandomVariable> w = CreateObject<GammaRandomVariable> ();
  w->SetAttribute ("Alpha", DoubleValue (10));
  w->SetAttribute ("Beta", DoubleValue (0.8));
  Prob = w->GetValue ();
  if (Prob > 15){Prob = 15;}
  for(int i=0; i < 4; i++) {
    array2[i] = matrix1[Prob][i]; 
  }
  m2mNode.set_Devi_Id(array2); 
  
  }
  
  //cout <<"Resources node: " <<m2mNode.getResources () << endl;
  m2mNode.Handle();
           
  // Create the socket if not already
  if (!m_socket)
    {
      m_socket = Socket::CreateSocket (GetNode (), m_tid);
      if (Inet6SocketAddress::IsMatchingType (m_peer))
        {
          if (m_socket->Bind6 () == -1)
            {
              NS_FATAL_ERROR ("Failed to bind socket");
            }
        }
      else if (InetSocketAddress::IsMatchingType (m_peer) ||
               PacketSocketAddress::IsMatchingType (m_peer))
        {
          if (m_socket->Bind () == -1)
            {
              NS_FATAL_ERROR ("Failed to bind socket");
            }
        }
      m_socket->Connect (m_peer);
      m_socket->SetAllowBroadcast (true);
      m_socket->ShutdownRecv ();

      m_socket->SetConnectCallback (
        MakeCallback (&M2MApplication::ConnectionSucceeded, this),
        MakeCallback (&M2MApplication::ConnectionFailed, this));
    }
  m_cbrRateFailSafe = m_cbrRate;

  // Insure no pending event
  CancelEvents ();
  // If we are not yet connected, there is nothing to do here
  // The ConnectionComplete upcall will start timers at that time
  //if (!m_connected) return;
  
  ScheduleStartEvent ();
}

void M2MApplication::StopApplication () // Called at time specified by Stop
{
  NS_LOG_FUNCTION (this);

  CancelEvents ();
  if(m_socket != 0)
    {
      m_socket->Close ();
    }
  else
    {
      NS_LOG_WARN ("M2MApplication found null socket to close in StopApplication");
    }
}

void M2MApplication::CancelEvents ()
{
  NS_LOG_FUNCTION (this);

  if (m_sendEvent.IsRunning () && m_cbrRateFailSafe == m_cbrRate )
    { // Cancel the pending send packet event
      // Calculate residual bits since last packet sent
      Time delta (Simulator::Now () - m_lastStartTime);
      int64x64_t bits = delta.To (Time::S) * m_cbrRate.GetBitRate ();
      m_residualBits += bits.GetHigh ();
    }
  m_cbrRateFailSafe = m_cbrRate;
  Simulator::Cancel (m_sendEvent);
  Simulator::Cancel (m_startStopEvent);
}

// Event handlers
void M2MApplication::StartSending ()
{
  NS_LOG_FUNCTION (this);
  m_lastStartTime = Simulator::Now ();
  ScheduleNextTx ();  // Schedule the send packet event
  ScheduleStopEvent ();
}

void M2MApplication::StopSending ()
{
  NS_LOG_FUNCTION (this);
  CancelEvents ();

  //ScheduleStartEvent ();
}

// Private helpers
void M2MApplication::ScheduleNextTx ()
{
  NS_LOG_FUNCTION (this);

  if (m_maxBytes == 0 || m_totBytes < m_maxBytes)
    {
      uint32_t bits = m_pktSize * 8 - m_residualBits;
      NS_LOG_LOGIC ("bits = " << bits);
      Time nextTime (Seconds (bits /
                              static_cast<double>(m_cbrRate.GetBitRate ()))); // Time till next packet
      NS_LOG_LOGIC ("nextTime = " << nextTime);
      m_sendEvent = Simulator::Schedule (nextTime,
                                         &M2MApplication::SendPacket, this);
    }
  else
    { // All done, cancel any pending events
      StopApplication ();
    }
}

void M2MApplication::ScheduleStartEvent ()
{  // Schedules the event to start sending data (switch to the "On" state)
  NS_LOG_FUNCTION (this);

  Time offInterval = Seconds (m_offTime->GetValue ());
  NS_LOG_LOGIC ("start at " << offInterval);
  m_startStopEvent = Simulator::Schedule (offInterval, &M2MApplication::StartSending, this);
}

void M2MApplication::ScheduleStopEvent ()
{  // Schedules the event to stop sending data (switch to "Off" state)
  NS_LOG_FUNCTION (this);

  Time onInterval = Seconds (m_onTime->GetValue ());
  NS_LOG_LOGIC ("stop at " << onInterval);
  m_startStopEvent = Simulator::Schedule (onInterval, &M2MApplication::StopSending, this);
}

void M2MApplication::SendPacket ()
{
  NS_LOG_FUNCTION (this);
 
  //m2mNode.getAddress();
  NS_ASSERT (m_sendEvent.IsExpired ());
    
  Ptr<Packet> packet1 = Create<Packet> (4);
  M2MHeader m2mHeader;
  m2mHeader.SetSelfIds(m2mNode.get_Byte1());
  m2mHeader.SetSelfAdd(m2mNode.get_Byte2());
  m2mHeader.SetPropAdd(m2mNode.get_Byte3());
  NS_LOG_LOGIC ("Byte1: " << unsigned(m2mNode.get_Byte1()) << " Byte2: " << unsigned(m2mNode.get_Byte2()) << " Byte3: " << unsigned(m2mNode.get_Byte3()));
  packet1->AddHeader (m2mHeader);
  m_txTrace (packet1);
  m_socket->Send (packet1);
  m_totBytes += m_pktSize;
    
  Ptr<Packet> packet = Create<Packet> (m_pktSize);
  m_txTrace (packet);
  m_socket->Send (packet);
  m_totBytes += m_pktSize;
    
  if (InetSocketAddress::IsMatchingType (m_peer))
    {
      NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds ()
                   << "s m2m application sent "
                   <<  packet->GetSize () << " bytes to "
                   << InetSocketAddress::ConvertFrom(m_peer).GetIpv4 ()
                   << " port " << InetSocketAddress::ConvertFrom (m_peer).GetPort ()
                   << " total Tx " << m_totBytes << " bytes");
    }
  else if (Inet6SocketAddress::IsMatchingType (m_peer))
    {
      NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds ()
                   << "s m2m application sent "
                   <<  packet->GetSize () << " bytes to "
                   << Inet6SocketAddress::ConvertFrom(m_peer).GetIpv6 ()
                   << " port " << Inet6SocketAddress::ConvertFrom (m_peer).GetPort ()
                   << " total Tx " << m_totBytes << " bytes" << packet1);
    }
  m_lastStartTime = Simulator::Now ();
  m_residualBits = 0;
  //ScheduleNextTx ();
}

void M2MApplication::ConnectionSucceeded (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  m_connected = true;
}

void M2MApplication::ConnectionFailed (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
}

} // Namespace ns3
