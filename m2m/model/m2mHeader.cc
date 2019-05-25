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

#include "m2mHeader.h"
#include "ns3/address-utils.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (M2MHeader);

M2MHeader::M2MHeader ()
 :m_selfids(0x00),
  m_selfadd (0x0000), 
  m_propadd (0x00)
{
}
M2MHeader::~M2MHeader ()
{
  m_selfids = 0xff;
  m_selfadd = 0xffff; 
  m_propadd = 0xff;
}
void
M2MHeader::SetSelfIds (uint8_t selfids)
{
  m_selfids = selfids;
}
uint8_t 
M2MHeader::GetSelfIds (void) const
{
  return m_selfids;
}
void
M2MHeader::SetSelfAdd (uint16_t selfadd)
{
  m_selfadd = selfadd;
}
uint8_t 
M2MHeader::GetSelfAdd (void) const
{
  return m_selfadd;
}
void
M2MHeader::SetPropAdd (uint8_t propadd)
{
  m_propadd = propadd;
}
uint8_t 
M2MHeader::GetPropAdd (void) const
{
  return m_propadd;
}

TypeId 
M2MHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::M2MHeader")
    .SetParent<Header> ()
    .SetGroupName ("Internet")
    .AddConstructor<M2MHeader> ()
  ;
  return tid;
}
TypeId 
M2MHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void 
M2MHeader::Print (std::ostream &os) const
{
  os << "Source: " << m_selfids << " | "<< m_selfadd << " > " << m_propadd;
}

uint32_t 
M2MHeader::GetSerializedSize (void) const
{
  return 4;
}

void
M2MHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_selfids);
  i.WriteHtonU16 (m_selfadd);
  i.WriteU8 (m_propadd);
}
uint32_t
M2MHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_selfids = i.ReadU8 ();
  m_selfadd = i.ReadNtohU16 ();
  m_propadd = i.ReadU8 ();

  return GetSerializedSize ();
}

} // namespace ns3
