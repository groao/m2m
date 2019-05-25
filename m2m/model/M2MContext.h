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

#include <exception>
using namespace std;

#ifndef __M2MContext_h__
#define __M2MContext_h__

#include "M2MAddress.h"
#include "M2MState.h"

namespace M2M_PSM_Controller
{
	// class M2MAddress;
	// class M2MState;
	class M2MContext;
}

namespace M2M_PSM_Controller
{
	/**
	 * \addtogroup M2Mapp M2MProtocol
	 * 
	 * Class ns3::M2MContext is the base class for describing the M2M Protocol State Machine.
	 * It defines the interface between the PSM and outside world.
	 * 
	 * Every M2MContext is designed as an Application to each node in ns3.
	 * It doesn't implement state-specific behavior, it delegates its function to class M2MState.
	 * 
	 * \brief The base class for M2M Protocol
	 */
	class M2MContext: public M2M_PSM_Controller::M2MState, public M2M_PSM_Controller::M2MAddress
	{
		public:

			M2MContext();
			~M2MContext();
		/// <summary>
		/// \brief Set the current state for M2M Protocol
		/// 
		/// \param state current M2MState
		/// </summary>
			void setState(M2M_PSM_Controller::M2MState* state);
		/// <summary>
		/// \brief Delegates state-specific behavior to M2MStates objects.
		/// </summary>
			void Handle();
			void setResources(int Resources);
			int getResources();
		private:
		/**
		 * //!< The M2MAddress defined for this node application.
		 */
			M2M_PSM_Controller::M2MAddress* M2MAddress;
		/**
		 * //!< The current state of M2M PSM.
		 */
			M2M_PSM_Controller::M2MState* _state;
		/**
		 * //!< The pondered available resources in the node.
		 */
			int Resources;

	};
}

#endif
