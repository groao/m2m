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

#ifndef __M2MIntegrator_h__
#define __M2MIntegrator_h__

// #include "M2MContext.h"
#include "M2MState.h"

namespace M2M_PSM_Controller
{
	class M2MContext;
	// class M2MState;
	class M2MIntegrator;
}

namespace M2M_PSM_Controller
{
	/**
	 * \ingroup M2Mapp
	 * \defgroup integrator M2MIntegrator
	 * 
	 * When the PSM defines that an Integrator Node is needed this class is called.
	 * An integrating node would give the system a greater ability to "learn" since
	 * having the computing capacity could be able to extract patterns from
	 * different sub-nets, suggest reorganizations to the propagators and
	 * extrapolate information of value on the Behavior of devices with similar features.
	 * 
	 * \ingroup integrator
	 * 
	 * \brief Describe the behavior of an Integrator node.
	 */
	class M2MIntegrator: public M2M_PSM_Controller::M2MState
	{
		public:

			M2MIntegrator(M2M_PSM_Controller::M2MContext* context);
			void Handle();
		private:
		/**
		 * //!< Pointer to current state.
		 */
			M2M_PSM_Controller::M2MContext* _context;

	};
}

#endif
