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

#ifndef __M2MTerminal_h__
#define __M2MTerminal_h__

// #include "M2MContext.h"
#include "M2MState.h"

namespace M2M_PSM_Controller
{
	class M2MContext;
	// class M2MState;
	class M2MTerminal;
}

namespace M2M_PSM_Controller
{
	/**
	 * \ingroup M2Mapp
	 * \defgroup terminal M2MTerminal
	 * 
	 * Every node starts being a Terminal.
	 * When there is, at least, one proximal Propagator it remains as a Terminal.
	 * 
	 * \ingroup terminal
	 * 
	 * \brief Describe the behavior of a Terminal node.
	 */
	class M2MTerminal: public M2M_PSM_Controller::M2MState
	{
		public:

			M2MTerminal(M2M_PSM_Controller::M2MContext* context);
			void Handle();
		private:
		/**
		 * //!< Pointer to current state.
		 */
			M2M_PSM_Controller::M2MContext* _context;

	};
}

#endif
