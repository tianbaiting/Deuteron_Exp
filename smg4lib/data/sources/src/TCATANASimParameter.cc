#include "TCATANASimParameter.hh"
#include "TCATANACsISimParameter.hh"

#include <iostream>

#include <TMath.h>


ClassImp(TCATANASimParameter)

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
TCATANASimParameter::TCATANASimParameter(TString name)
: TSimParameter(name), fIsLoaded(false), fDetectorNum(0), 
  fPosition(0,0,0), fAngle(0,0,0), fV_scinti(150)
{;}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
TCATANASimParameter::~TCATANASimParameter(){;}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
TCATANACsISimParameter* TCATANASimParameter::FindCATANACsISimParameter(Int_t id)
{
  std::map<int, TCATANACsISimParameter*>::iterator it =
    fCATANACsISimParameterMap.find(id);
  if(it == fCATANACsISimParameterMap.end()) return 0; // not found
  return it->second;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
std::ostream& operator<<(ostream& out, const TCATANASimParameter& prm)
{
  out << prm.GetName() <<": "
      << "fIsLoaded=" << prm.fIsLoaded << " "
      << "fPosition=" << prm.fPosition.X() << ", " << prm.fPosition.Y() << ", "
      << prm.fPosition.Z() << " "
      << "fAngle=" << prm.fAngle.X() << ", " << prm.fAngle.Y() << ", "
      <<  prm.fAngle.Z() << " "
      << "fV_scinti=" << prm.fV_scinti << " ";
  return out;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void TCATANASimParameter::Print(Option_t*) const
{
  std::cout << GetName() <<": "
	    << "fIsLoaded=" << fIsLoaded << " "
	    << "fPosition=" << fPosition.X() << ", " << fPosition.Y() << ", "
	    << fPosition.Z()
	    << "fAngle=" << fAngle.X() << ", " << fAngle.Y() << ", "
	    <<  fAngle.Z() << " "
	    << "fV_scinti=" << fV_scinti << " " << std::endl;
}

