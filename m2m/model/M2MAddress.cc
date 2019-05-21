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
#include <math.h>
#include "ns3/random-variable-stream.h"
#include "ns3/stats-module.h"
using namespace std;
using namespace ns3;

#include "M2MTerminal.h"
#include "M2MPropagator.h"
#include "M2MIntegrator.h"
#include "M2MAddress.h"

M2M_PSM_Controller::M2MAddress::M2MAddress() {
	fill_n(Appl_Id, 4, 0);
	fill_n(Devi_Id, 4, 0);
        fill_n(Role_Id, 2, 0);
	fill_n(Redu_Id, 3, 0);
	fill_n(SelfAdd, 11, 0);
	fill_n(PropAdd, 8, 0);
}

M2M_PSM_Controller::M2MAddress::~M2MAddress() {
	//throw "Not yet implemented";
}

void M2M_PSM_Controller::M2MAddress::getAddress() {
	  unsigned int prov_Add[32] = {0};
	    int i=0,j=0,k=0,l=0,m=0,n=0;
	    
            for (i=0; i<2; i++){
	    	prov_Add[i] = Role_Id[k];
                k++;
	    }
	    
	    for (i=2; i<5; i++){
	    	prov_Add[i] = Redu_Id[l];
	    	l++;
	    }
            
            for (i=5; i<9; i++){
	    	prov_Add[i] = Appl_Id[i];
	    }
	        
	    for (i=9; i<13; i++){
	    	prov_Add[i] = Devi_Id[j];
	    	j++;
	    }
            
	    for (i=13; i<24; i++){
	    	prov_Add[i] = SelfAdd[m];
	    	m++;
	    }
	    
	    for (i=24; i<32; i++){
	    	prov_Add[i] = PropAdd[n];
	    	n++;
	    }
	    
	    cout << "m2mAddress:R|Ri|-Ai|-Di|--11 Self-|-8 Prop|" <<endl;
            cout << "ID_Address=";
	    for (i=0; i<32; i++){
	         cout << prov_Add[i] ;
	    }
	    cout << endl;
	    //return prov_Add;
}

unsigned int* M2M_PSM_Controller::M2MAddress::get_ID(int sID) {
	  static unsigned int ID0[2];
	  static unsigned int ID1[3];
	  static unsigned int ID2[4];
	  static unsigned int ID3[8];
	  static unsigned int ID4[11];
	    
	    switch ( sID ) {
	    case 1:
	      copy(std::begin(Role_Id), std::end(Role_Id), std::begin(ID0));  
	      break;
	    case 2:
	      copy(std::begin(Appl_Id), std::end(Appl_Id), std::begin(ID1));    
	      break;
	    case 3:
	      copy(std::begin(Redu_Id), std::end(Redu_Id), std::begin(ID2));  
	      break;
	    case 4:
	      copy(std::begin(Devi_Id), std::end(Devi_Id), std::begin(ID2));  
	      break;
	    case 5:
	      copy(std::begin(PropAdd), std::end(PropAdd), std::begin(ID3));  
	      break;
	     case 6:
	      copy(std::begin(SelfAdd), std::end(SelfAdd), std::begin(ID4));  
	      break;
	    default:
	      cout << "Wrong ID string!" <<endl;
	      break;
	    }
	    
	    if (sID==1){
	        return ID0;
	    }
	    else if (sID==2){
	        return ID1;
	    }
	   else if (sID==5){
	        return ID3;
	    }
	    else if (sID==6){
	        return ID4;
	    }
	    else  
	        return ID2;
}

void M2M_PSM_Controller::M2MAddress::self_Ass_Pro() {
	  Ptr<UniformRandomVariable> z = CreateObject<UniformRandomVariable> ();
	  z->SetAttribute ("Min", DoubleValue (1));
	  z->SetAttribute ("Max", DoubleValue (256));
          unsigned int zVal = z->GetValue ();
	  std::bitset<8> bin(zVal);
	  int i=0,j=0;
	    for (i=7; i>=0; i--){
	    	PropAdd[i] = bin[j];
	      j++;
	    }
}

void M2M_PSM_Controller::M2MAddress::self_Ass_Ter() {
	  Ptr<UniformRandomVariable> z = CreateObject<UniformRandomVariable> ();
	  z->SetAttribute ("Min", DoubleValue (1));
	  z->SetAttribute ("Max", DoubleValue (2049));
          unsigned int zVal = z->GetValue ();
	  std::bitset<11> bin(zVal);
	  int i=0,j=0;
	    for (i=10; i>=0; i--){
	    	SelfAdd[i] = bin[j];
	      j++;
	    }
}

void M2M_PSM_Controller::M2MAddress::set_Role_Id(unsigned int Rol[2]) {
	    int i=0,j=0;
	    for (i=0; i<2; i++){
	     	Role_Id[i] = Rol[j];
	      j++;
	    }
}

void M2M_PSM_Controller::M2MAddress::set_Redu_Id(unsigned int Red[3]) {
	    int i=0,j=0;
	    for (i=0; i<3; i++){
	     	Redu_Id[i] = Red[j];
	      j++;
	    }
}

void M2M_PSM_Controller::M2MAddress::set_Appl_Id(unsigned int App[4]) {
	    int i=0,j=0;
	    for (i=0; i<4; i++){
	     	Appl_Id[i] = App[j];
	      j++;
	    }
}

void M2M_PSM_Controller::M2MAddress::set_Devi_Id(unsigned int Dev[4]) {
	    int i=0,j=0;
	    for (i=0; i<4; i++){
	     	Devi_Id[i] = Dev[j];
	      j++;
	    }
}

void M2M_PSM_Controller::M2MAddress::set_PropAdd(unsigned int Prop[8]) {
	    int i=0,j=0;
	    for (i=0; i<8; i++){
	     	PropAdd[i] = Prop[j];
	      j++;
	    }
}

uint8_t M2M_PSM_Controller::M2MAddress::get_Byte1(){
    uint8_t Byte1= 0x00;
    
    for (uint8_t i=0; i<4; i++){
        if (Appl_Id[3-i] == 1){
        Byte1 = Byte1 + pow(2,i+4);
	}
    }
    
    for (uint8_t i=0; i<4; i++){
        if (Devi_Id[3-i] == 1){
        Byte1 = Byte1 + pow(2,i);
	}
    }
    
    return Byte1;
}

uint16_t M2M_PSM_Controller::M2MAddress::get_Byte2(){
    uint16_t Byte2= 0x0000;
    
    for (uint8_t i=0; i<11; i++){
        if (SelfAdd[10-i] == 1){
        Byte2 = Byte2 + pow(2,i);
	}
    }
    
    for (uint8_t i=0; i<3; i++){
        if (Redu_Id[2-i] == 1){
        Byte2 = Byte2 + pow(2,i+11);
	}
    }
    
        for (uint8_t i=0; i<2; i++){
        if (Role_Id[1-i] == 1){
        Byte2 = Byte2 + pow(2,i+14);
	}
    }
    
    return Byte2;
}

uint8_t M2M_PSM_Controller::M2MAddress::get_Byte3(){
    uint8_t Byte3= 0x00;
    
    for (uint8_t i=0; i<8; i++){
        if (PropAdd[7-i] == 1){
        Byte3 = Byte3 + pow(2,i);
	}
    }
    
    return Byte3;
}
