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

#ifndef M2M_HEADER_H
#define M2M_HEADER_H

#include <stdint.h>
#include <string>
#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"

namespace ns3 {
/**
 * \ingroup M2M
 * \brief Packet header for M2M packets
 *
 * This class has fields corresponding to those in a network M2M header
 * as well as methods for serialization
 * to and deserialization from a byte buffer.
 */
class M2MHeader : public Header 
{
public:

  /**
   * \brief Constructor
   *
   * Creates a null header
   */
  M2MHeader ();
  ~M2MHeader ();

  /**
   * \param port the destination port for this M2MHeader
   */
  void SetSelfIds (uint8_t selfids);
    /**
   * \return the destination port for this M2MHeader
   */
  uint8_t GetSelfIds (void) const;
  /**
   * \param port the destination port for this M2MHeader
   */
  void SetSelfAdd (uint16_t selfadd);
    /**
   * \return the destination port for this M2MHeader
   */
  uint8_t GetSelfAdd (void) const;
  /**
   * \param port the destination port for this M2MHeader
   */
  void SetPropAdd (uint8_t propadd);
    /**
   * \return the destination port for this M2MHeader
   */
  uint8_t GetPropAdd (void) const;
  
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  
  uint8_t m_selfids;
  uint16_t m_selfadd; 
  uint8_t m_propadd;

};

} // namespace ns3

#endif /* M2M_HEADER */
