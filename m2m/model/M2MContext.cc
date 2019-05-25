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

