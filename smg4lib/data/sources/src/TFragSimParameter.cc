#include "TFragSimParameter.hh"

#include <iostream>

ClassImp(TFragSimParameter)
//____________________________________________________________________
TFragSimParameter::TFragSimParameter(TString name)
: TSimParameter(name), 
  fTargetPosition(0,0,0), fTargetThickness(0),
  fFDC1Position(0,0,0),
  fFDC2Position(0,0,0), fWindowHolePosition(0,0,0),
  fICFPosition(0,0,0), fHODPosition(0,0,0), 
  fFDC1Angle(0), fFDC2Angle(0), 
  fICFAngle(0), fHODAngle(0)
{;}
//____________________________________________________________________
TFragSimParameter::~TFragSimParameter()
{;}
//____________________________________________________________________
void TFragSimParameter::Print(Option_t*) const
{
  std::cout << GetName() <<": "
	    <<" TGT="
	    <<fTargetPosition.x()<<"mm "
	    <<fTargetPosition.y()<<"mm "
	    <<fTargetPosition.z()<<"mm "
	    <<fTargetThickness<<"mm "
	    <<" FDC1="
	    <<fFDC1Position.x()<<"mm "
	    <<fFDC1Position.y()<<"mm "
	    <<fFDC1Position.z()<<"mm "
	    <<fFDC1Angle<<"deg "
	    <<" FDC2="
	    <<fFDC2Position.x()<<"mm "
	    <<fFDC2Position.y()<<"mm "
	    <<fFDC2Position.z()<<"mm "
	    <<fFDC1Angle<<"deg "
	    <<" ICF="
	    <<fICFPosition.x()<<"mm "
	    <<fICFPosition.y()<<"mm "
	    <<fICFPosition.z()<<"mm "
	    <<fICFAngle<<"deg "
	    <<" HOD="
	    <<fHODPosition.x()<<"mm "
	    <<fHODPosition.y()<<"mm "
	    <<fHODPosition.z()<<"mm "
	    <<fHODAngle<<"deg "

	    << std::endl;
}
//____________________________________________________________________
