#include "TDALISimParameter.hh"
#include "TDALINaISimParameter.hh"

#include <iostream>
#include <math.h>

ClassImp(TDALISimParameter)
//____________________________________________________________________
TDALISimParameter::TDALISimParameter(TString name)
: TSimParameter(name),
  fIsLoaded(false),
  fDetectorNum(0), fLayerNum(0),
  fPosition(0,0,0),
  fSize(200,200,200), fAngle(0,0,0),
  fV_scinti(150)   //mm/ns
{
  std::cout<<"TDALISimParameter"<<std::endl;
}
//____________________________________________________________________
TDALISimParameter::~TDALISimParameter()
{;}
//____________________________________________________________________
TDALINaISimParameter*  TDALISimParameter::FindDALINaISimParameter(Int_t id)
{
  std::map<int, TDALINaISimParameter*>::iterator it
    = fDALINaISimParameterMap.find(id);
  if (it==fDALINaISimParameterMap.end()) return 0;
  return it->second;
}
//____________________________________________________________________
std::ostream& operator<<(ostream& out,  const TDALISimParameter& prm)
{
  out << prm.GetName() <<": "
      << "fIsLoaded="       <<prm.fIsLoaded <<" "
      << "fDetectorNum="    <<prm.fDetectorNum <<" "
      << "fLayerNum="       <<prm.fLayerNum      <<" "
      << "fPosition="       <<prm.fPosition.x() <<" "<<prm.fPosition.y() <<" "<<prm.fPosition.z() <<" "
      << "fV_scinti="       <<prm.fV_scinti<<" ";
  return out;
}
//____________________________________________________________________
void TDALISimParameter::Print(Option_t*) const
{
  std::cout << GetName() <<": "
	    << "fIsLoaded="       <<fIsLoaded <<": "
	    << "fDetectorNum="    <<fDetectorNum <<" "
	    << "fLayerNum="       <<fLayerNum <<" "
	    << "fPosition="       <<fPosition.x() <<" "<<fPosition.y() <<" "<<fPosition.z() <<" "
	    << "fAngle="          <<fAngle.x()<<" "<<fAngle.y()<<" "<<fAngle.z()<<" "
            << "fV_scinti="       <<fV_scinti<<" "
	    << std::endl;
}
//____________________________________________________________________
