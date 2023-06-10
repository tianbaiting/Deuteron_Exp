#include "TDeutSimParameter.hh"
#include "G4SystemOfUnits.hh"//Geant4.10

#include <iostream>

ClassImp(TDeutSimParameter)
//____________________________________________________________________
TDeutSimParameter::TDeutSimParameter(TString name)
: TSimParameter(name), 
  fTargetThickness{0},
  fTargetPosition{0,0,0},
  fTargetAngle{10*deg},

  fPDCAngle{57*deg},
  fPDC1Position{0.4*m, 0, 4.1*m},
  fPDC2Position{0.4*m, 0, 5.1*m},

  fDumpAngle{39.3*deg},
  fDumpPosition{0, -25*cm, 10*m}
{;}
//____________________________________________________________________
TDeutSimParameter::~TDeutSimParameter()
{;}
//____________________________________________________________________
void TDeutSimParameter::Print(Option_t*) const
{
  std::cout << GetName() <<": "
	    <<" TGT="
	    <<fTargetPosition.x()<<"mm "
	    <<fTargetPosition.y()<<"mm "
	    <<fTargetPosition.z()<<"mm "
		<<fTargetAngle<<"deg "
	    <<" PDC1="
		<<fPDCAngle<<"deg "
	    <<fPDC1Position.x()<<"mm "
	    <<fPDC1Position.y()<<"mm "
	    <<fPDC1Position.z()<<"mm "
	    <<" PDC2="
		<<fPDCAngle<<"deg "
	    <<fPDC2Position.x()<<"mm "
	    <<fPDC2Position.y()<<"mm "
	    <<fPDC2Position.z()<<"mm "
	    <<" Dump="
		<<fDumpAngle<<"deg "
	    <<fDumpPosition.x()<<"mm "
	    <<fDumpPosition.y()<<"mm "
	    <<fDumpPosition.z()<<"mm "
	    
	    << std::endl;
}
//____________________________________________________________________
