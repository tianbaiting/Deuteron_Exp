#include "TDALINaISimParameter.hh"

#include <iostream>

ClassImp(TDALINaISimParameter)
//____________________________________________________________________
TDALINaISimParameter::TDALINaISimParameter(TString name)
: TSimParameter(name), 
  fID(-1), fLayer(-1), fDetectorType(""), fDetectorName(""),
  fPosition(0,0,0), fSize(0,0,0), fAngle(0,0,0),
  fThreshold(0), frotSign(0), fCrystalType(0), fTimeResolution(3)
{
  fEnergyResolutionPrm.clear();
}
//____________________________________________________________________
TDALINaISimParameter::~TDALINaISimParameter()
{;}
//____________________________________________________________________
std::ostream& operator<<(ostream& out,  const TDALINaISimParameter& prm)
{
  out << prm.GetName() <<": "
      << "fLayer=" <<prm.fLayer<<" "
      << "fPosition(mm)=" <<prm.fPosition.x()<<" "<<prm.fPosition.y()<<" "<<prm.fPosition.z()<<" "
      << "fSize="     <<prm.fSize.x() <<" "<<prm.fSize.y() <<" "<<prm.fSize.z() <<" "
      << "fAngle(deg)="    <<prm.fAngle.x()<<" "<<prm.fAngle.y()<<" "<<prm.fAngle.z()<<" "
      << "fThreshold="  <<prm.fThreshold<<" "
      << "frotSign="  <<prm.frotSign<<" "
      << "fCrystalType="  <<prm.fCrystalType<<" "
    ;

  Int_t nprm = prm.fEnergyResolutionPrm.size();
  if (nprm>0){
    out<<  "fEnergyResolutionPrm=";
    for (Int_t i=0;i<nprm;++i)
      out<<prm.fEnergyResolutionPrm[i]<<", ";
  }
  out << "fTimeResolution(ns)="  <<prm.fTimeResolution<<" ";

  return out;
}
//____________________________________________________________________
void TDALINaISimParameter::Print(Option_t*) const
{
  std::cout << GetName() <<": "
	    << "fLayer=" <<fLayer<<" "
	    << "fPosition(mm)=" <<fPosition.x()<<" "<<fPosition.y()<<" "<<fPosition.z()<<" "
	    << "fSize="     <<fSize.x() <<" "<<fSize.y() <<" "<<fSize.z() <<" "
	    << "fAngle(deg)="    <<fAngle.x()<<" "<<fAngle.y()<<" "<<fAngle.z()<<" "
	    << "fThreshold="  <<fThreshold<<" "
	    << "frotSign="    <<frotSign<<" "
	    << "fCrystalType=" <<fCrystalType<<" "
    ;       

  Int_t nprm = fEnergyResolutionPrm.size();
  if (nprm>0){
    std::cout<<  "fEnergyResolutionPrm=";
    for (Int_t i=0;i<nprm;++i)
      std::cout<<fEnergyResolutionPrm[i]<<", ";
  }
  std::cout << "fTimeResolution(ns)=" <<fTimeResolution<<" ";

  std::cout<< std::endl;

}
//____________________________________________________________________
