
#include "m2m.h"

void Integrator::Mining() {
}

void Integrator::Load_Bal() {
}

void Integrator::Ipv6_Int() {
}

Integrator::Integrator() {
}

Integrator::~Integrator() {
}

//4 bits Version Id
unsigned int m2mAddress::Version[4]= {0,0,0,1};

//4 bits Application Id
unsigned int m2mAddress::Appl_Id[4]= { 0 };

//4 bits Redundancy Id
unsigned int m2mAddress::Redu_Id[4]= { 0 };

//4 bits Device Id
unsigned int m2mAddress::Devi_Id[4]= { 0 };

//2 bytes self-generated address
unsigned int m2mAddress::SelfAdd[16]= { 0 };

//1 bytes Propagator Address
unsigned int m2mAddress::PropAdd[8]= { 0 };

//Constructor
m2mAddress::m2mAddress(){
}

//Destructor
m2mAddress::~m2mAddress(){
}

//Get Node Address
void m2mAddress::GetAddress() {
  
  unsigned int prov_Add[40] = {0};
    int i=0,j=0,k=0,l=0,m=0,n=0;
    
    for (i=0; i<4; i++){
    	prov_Add[i] = Version[i];
    }
    
    for (i=4; i<8; i++){
    	prov_Add[i] = Appl_Id[j];
    	j++;
    }
    
    for (i=8; i<12; i++){
    	prov_Add[i] = Redu_Id[k];
    	k++;
    }
    
    for (i=12; i<16; i++){
    	prov_Add[i] = Devi_Id[l];
    	l++;
    }
    
    for (i=16; i<32; i++){
    	prov_Add[i] = SelfAdd[m];
    	m++;
    }
    
    for (i=32; i<40; i++){
    	prov_Add[i] = PropAdd[n];
    	n++;
    }
    
    cout << "m2m Address: " <<endl;
    cout << "-Vi|-Ai|-Ri|-Di|--16bits Self--|-8 Prop|" <<endl;
    for (i=0; i<40; i++){
         cout << prov_Add[i] ;
    }
    cout << endl;
    //return prov_Add;

}

//Get any Identifier
unsigned int * m2mAddress::Get_Id(int sID) {
  static unsigned int ID1[4];
  static unsigned int ID2[8];
  static unsigned int ID3[16];
    
    switch ( sID ) {
    case 1:
      copy(std::begin(Version), std::end(Version), std::begin(ID1));  
      break;
    case 2:
      copy(std::begin(Appl_Id), std::end(Appl_Id), std::begin(ID1));    
      break;
    case 3:
      copy(std::begin(Redu_Id), std::end(Redu_Id), std::begin(ID1));  
      break;
    case 4:
      copy(std::begin(Devi_Id), std::end(Devi_Id), std::begin(ID1));  
      break;
    case 5:
      copy(std::begin(PropAdd), std::end(PropAdd), std::begin(ID2));  
      break;
     case 6:
      copy(std::begin(SelfAdd), std::end(SelfAdd), std::begin(ID3));  
      break;
    default:
      cout << "Wrong ID string!" <<endl;
      break;
    }
    
    if (sID==5){
        return ID2;
    }
    else if (sID==6){
        return ID3;
    }
    else  
        return ID1;
}

//Self-assign Propagator Address
void m2mAddress::Self_Ass_Pro() {
  
  Ptr<UniformRandomVariable> z = CreateObject<UniformRandomVariable> ();
  z->SetAttribute ("Min", DoubleValue (0));
  z->SetAttribute ("Max", DoubleValue (255));
  unsigned int zVal = z->GetValue ();
  std::bitset<8> bin(zVal);
  //std::cout << zVal << " in binary is " <<  bin << std::endl;
   
  int i=0,j=0;
    for (i=7; i>=0; i--){
    	PropAdd[i] = bin[j];
      j++;
    }
}

//Self-assign Terminal Address
void m2mAddress::Self_Ass_Ter() {
  
  Ptr<UniformRandomVariable> z = CreateObject<UniformRandomVariable> ();
  z->SetAttribute ("Min", DoubleValue (0));
  z->SetAttribute ("Max", DoubleValue (65535));
  unsigned int zVal = z->GetValue ();
  std::bitset<16> bin(zVal);
  //std::cout << zVal << " in binary is " <<  bin << std::endl;
   
  int i=0,j=0;
    for (i=15; i>=0; i--){
    	SelfAdd[i] = bin[j];
      j++;
    }
}

//Set the Version of the address
void m2mAddress::Set_Version(unsigned int Ver[4]) {
  
    int i=0,j=0;
    for (i=3; i>=0; i--){
     	Version[i] = Ver[j];
      j++;
    }
}

//Set the Version of the address
void m2mAddress::Set_Appl_Id(unsigned int App[4]) {
  
    int i=0,j=0;
    for (i=3; i>=0; i--){
     	Appl_Id[i] = App[j];
      j++;
    }
}

//Set the RedundancyId
void m2mAddress::Set_Redu_Id(unsigned int Red[4]) {
  
    int i=0,j=0;
    for (i=3; i>=0; i--){
     	Redu_Id[i] = Red[j];
      j++;
    }
}

//Set the DeviceId
void m2mAddress::Set_Devi_Id(unsigned int Dev[4]) {
  
    int i=0,j=0;
    for (i=3; i>=0; i--){
     	Devi_Id[i] = Dev[j];
      j++;
    }
}

//Set the Propagator address
void m2mAddress::Set_PropAdd(unsigned int Prop[8]) {
  
    int i=0,j=0;
    for (i=7; i>=0; i--){
     	PropAdd[i] = Prop[j];
      j++;
    }
}

unsigned int m2mNode::Application[4];

unsigned int m2mNode::Redundancy[4];

unsigned int m2mNode::Device[4];

m2mNode::m2mNode(){
}

m2mNode::~m2mNode(){
}

//Assign the resources value
void m2mNode::SetResources(int resour) {
  Resources=resour;
}

void m2mNode::Set_m2m_Role() {
}

void Propagator::Disco_Inte() {
}

void Propagator::Ask_Dupl() {
}

void Propagator::Propagate() {
}

void Propagator::Load_Bal() {
}

void Propagator::Reorder() {
}

Propagator::Propagator() {
}

Propagator::~Propagator() {
}

void Terminal::Disco_Prop() {
}

void Terminal::Ask_Dupl() {
}

void Terminal::Send_Dat() {
}

Terminal::Terminal() {
}

Terminal::~Terminal() {
}

