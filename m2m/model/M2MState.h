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
