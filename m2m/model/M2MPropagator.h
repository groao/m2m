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

#include <exception>
using namespace std;

#ifndef __M2MPropagator_h__
#define __M2MPropagator_h__

// #include "M2MContext.h"
#include "M2MState.h"

namespace M2M_PSM_Controller
{
	class M2MContext;
	// class M2MState;
	class M2MPropagator;
}

namespace M2M_PSM_Controller
{
	/**
	 * \ingroup M2Mapp
	 * \defgroup propagator M2MPropagator
	 * 
	 * In case there are no Propagators, the node assumes this role if it
	 * has enough computational resources.
	 * It self-allocates a one-byte address and begins to be the axis of the
	 * organization of the nodes that wish to disseminate its information.
	 * 
	 * \ingroup propagator
	 * 
	 * \brief Describe the behavior of a Propagator node.
	 */
	class M2MPropagator: public M2M_PSM_Controller::M2MState
	{
		public:

			M2MPropagator(M2M_PSM_Controller::M2MContext* context);
			void Handle();
		private:
		/**
		 * //!< Pointer to current state.
		 */
			M2M_PSM_Controller::M2MContext* _context;

	};
}

#endif
