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

#ifndef __M2MState_h__
#define __M2MState_h__

namespace M2M_PSM_Controller
{
	class M2MState;
}

namespace M2M_PSM_Controller
{
	/**
	 * \ingroup M2Mapp
	 * 
	 * \brief Interface between M2M Roles and M2MContext
	 * 
	 * Class ns3::M2MState is used as interface between state-specific behavior and PSM Context.
	 */
	class M2MState
	{
		public:

		/// <summary>
		/// \brief Delegates state-specific behavior to M2MStates objects.
		/// </summary>
			virtual void Handle() = 0;
	};
}

#endif
