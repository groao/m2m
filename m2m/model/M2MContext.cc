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
#include <string>
#include <iostream>
#include <bitset>
#include "ns3/random-variable-stream.h"
#include "ns3/stats-module.h"
using namespace std;
using namespace ns3;

#include "M2MTerminal.h"
#include "M2MPropagator.h"
#include "M2MIntegrator.h"

#include "M2MContext.h"
#include "M2MAddress.h"
#include "M2MState.h"

M2M_PSM_Controller::M2MContext::M2MContext() {
	Resources = 0;
	_state = new M2MTerminal(this);
}

M2M_PSM_Controller::M2MContext::~M2MContext() {
	//throw "Not yet implemented";
}

void M2M_PSM_Controller::M2MContext::setState(M2M_PSM_Controller::M2MState* state) {
	_state=state;
}

void M2M_PSM_Controller::M2MContext::Handle() {
	_state->Handle();
}

void M2M_PSM_Controller::M2MContext::setResources(int Resources) {
	this->Resources = Resources;
}

int M2M_PSM_Controller::M2MContext::getResources() {
	return this->Resources;
}
