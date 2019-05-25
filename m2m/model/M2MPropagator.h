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
