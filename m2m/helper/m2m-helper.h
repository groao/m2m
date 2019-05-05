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

#ifndef M2M_HELPER_H
#define M2M_HELPER_H

#include <stdint.h>
#include <string>
#include "ns3/object-factory.h"
#include "ns3/address.h"
#include "ns3/attribute.h"
#include "ns3/net-device.h"
#include "ns3/node-container.h"
#include "ns3/application-container.h"
#include "ns3/m2m.h"

namespace ns3 {

class DataRate;

/**
 * \ingroup m2m
 * \brief A helper to make it easier to instantiate an ns3::M2MApplication 
 * on a set of nodes.
 */
class M2MHelper
{
public:
  /**
   * Create an M2MHelper to make it easier to work with M2MApplication
   *
   * \param protocol the name of the protocol to use to send traffic
   *        by the applications. This string identifies the socket
   *        factory type used to create sockets for the applications.
   *        A typical value would be ns3::UdpSocketFactory.
   * \param address the address of the remote node to send traffic
   *        to.
   */
  M2MHelper (std::string protocol, Address address);

  /**
   * Helper function used to set the underlying application attributes.
   *
   * \param name the name of the application attribute to set
   * \param value the value of the application attribute to set
   */
  void SetAttribute (std::string name, const AttributeValue &value);

  /**
   * Helper function to set a constant rate source.  Equivalent to
   * setting the attributes OnTime to constant 1000 seconds, OffTime to 
   * constant 0 seconds, and the DataRate and PacketSize set accordingly
   *
   * \param dataRate DataRate object for the sending rate
   * \param packetSize size in bytes of the packet payloads generated
   */
  void SetConstantRate (DataRate dataRate, uint32_t packetSize = 512);

  /**
   * Install an ns3::M2MApplication on each node of the input container
   * configured with all the attributes set with SetAttribute.
   *
   * \param c NodeContainer of the set of nodes on which an M2MApplication 
   * will be installed.
   * \returns Container of Ptr to the applications installed.
   */
  ApplicationContainer Install (NodeContainer c) const;

  /**
   * Install an ns3::M2MApplication on the node configured with all the 
   * attributes set with SetAttribute.
   *
   * \param node The node on which an M2MApplication will be installed.
   * \returns Container of Ptr to the applications installed.
   */
  ApplicationContainer Install (Ptr<Node> node) const;

  /**
   * Install an ns3::M2MApplication on the node configured with all the 
   * attributes set with SetAttribute.
   *
   * \param nodeName The node on which an M2MApplication will be installed.
   * \returns Container of Ptr to the applications installed.
   */
  ApplicationContainer Install (std::string nodeName) const;

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.  The Install() method should have previously been
  * called by the user.
  *
  * \param stream first stream index to use
  * \param c NodeContainer of the set of nodes for which the M2MApplication
  *          should be modified to use a fixed stream
  * \return the number of stream indices assigned by this helper
  */
  int64_t AssignStreams (NodeContainer c, int64_t stream);

private:
  /**
   * Install an ns3::M2MApplication on the node configured with all the 
   * attributes set with SetAttribute.
   *
   * \param node The node on which an M2MApplication will be installed.
   * \returns Ptr to the application installed.
   */
  Ptr<Application> InstallPriv (Ptr<Node> node) const;

  ObjectFactory m_factory; //!< Object factory.
};

} // namespace ns3

#endif /* ON_OFF_HELPER_H */