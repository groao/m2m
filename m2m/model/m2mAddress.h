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
#include <stdint.h>
#include <string>
using namespace std;

#ifndef __M2MAddress_h__
#define __M2MAddress_h__

/*
namespace M2M_PSM_Controller
{
	class M2MAddress;
}
*/

namespace M2M_PSM_Controller
{
	/**
	 * \ingroup M2Mapp
	 * \defgroup m2maddress M2MAddress
	 * 
	 * \ingroup m2maddress
	 * 
	 * \brief Describe the features of a M2MAddress.
	 */
	class M2MAddress
	{
		public:

			M2MAddress();
			~M2MAddress();
		/// <summary>
		/// \brief Get Node Address
		/// </summary>
			void getAddress();
		/// <summary>
		/// \brief Get any Identifier
		/// \param sID desired identifier.
		/// \return The address identifier desired.
		/// </summary>
			unsigned int* get_ID(int sID);
		/// <summary>
		/// \brief Self-assign Propagator Address
		/// </summary>
			void self_Ass_Pro();
		/// <summary>
		/// \brief Self-assign Terminal Address
		/// </summary>
			void self_Ass_Ter();
		/// <summary>
		/// \brief Set the Role Id
		/// \param Rol Role ID to be set.
		/// </summary>
			void set_Role_Id(unsigned int Rol[2]);
		/// <summary>
		/// \brief Set the Redundancy Id
		/// \param Red Redundancy ID to be set.
		/// </summary>
			void set_Redu_Id(unsigned int Red[3]);
		/// <summary>
		/// \brief Set the Application Address
		/// \param App Application ID to be set.
		/// </summary>
			void set_Appl_Id(unsigned int App[4]);
		/// <summary>
		/// \brief Set the Device Id
		/// \param Dev Kind of Device ID to be set.
		/// </summary>
			void set_Devi_Id(unsigned int Dev[4]);
                /// \brief Set the Propagator Address
		/// \param Prop Propagator address to be set.
		/// </summary>
			void set_PropAdd(unsigned int Prop[8]);   
		/// <summary>
		/// \brief Set the Propagator Address
		/// \param Prop Propagator address to be set.
		/// </summary>
			uint8_t get_Byte1();
                /// <summary>
		/// \brief Set the Propagator Address
		/// \param Prop Propagator address to be set.
		/// </summary>
			uint16_t get_Byte2();
                /// <summary>
		/// \brief Set the Propagator Address
		/// \param Prop Propagator address to be set.
		/// </summary>
			uint8_t get_Byte3();
     
		private:
      		/**
		 * //!< 4 bits Application Id
		 */
			unsigned int Appl_Id[4];
		/**
		 * //!< 4 bits Device Id
		 */
			unsigned int Devi_Id[4];
		/**
		 * //!< 2 bits Role Id
		 */
			unsigned int Role_Id[2];
		/**
		 * //!< 3 bits Redundancy Id
		 */
			unsigned int Redu_Id[3];
		/**
		 * //!< 11 bits self-generated address
		 */
			unsigned int SelfAdd[11];
		/**
		 * //!< 1 byte Propagator Address
		 */
			unsigned int PropAdd[8];

	};
}

#endif
