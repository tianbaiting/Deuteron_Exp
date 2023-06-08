#include "TCATANACsISimParameter.hh"

#include <iostream>
ClassImp(TCATANACsISimParameter)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
TCATANACsISimParameter::TCATANACsISimParameter(TString name)
: TSimParameter(name),
  fID(-1), fLayer(-1), fDetectorType(-1), fDetectorName(""),
  fPosition(0,0,0), fTheta(0), fPhi(0), fTimeResolution(2.),
  fEThreshold(100.), fEThresholdSigma(0.)
{
  fEnergyResolutionPrm.clear();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
TCATANACsISimParameter::~TCATANACsISimParameter(){;}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
std::ostream& operator<<(ostream& out, const TCATANACsISimParameter& prm)
{
  out << prm.GetName() << ": "
      << " Layer=" << prm.fLayer
      << " detType=" << prm.fDetectorType
      << " Pos=" << prm.fPosition.X() << ", " << prm.fPosition.Y() << ", "
      << prm.fPosition.Z()
      << " Theta=" << prm.fTheta
      << " Phi=" << prm.fPhi << " ";

  Int_t nprm = prm.fEnergyResolutionPrm.size();
  if(nprm>0){
    out << "EResoPrm= ";
    for(Int_t i=0;i<nprm;++i) out << prm.fEnergyResolutionPrm.at(i) << ", ";
  }
  out << " TReso(ns)=" << prm.fTimeResolution << ", " ;
  out << " EThreshold(keV)=" << prm.fEThreshold << ", " ;
  out << " EThSigma(keV)=" << prm.fEThresholdSigma << " " ;
  
  return out;
}
    
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void TCATANACsISimParameter::Print(Option_t*) const
{
  std::cout << GetName() << ": "
	    << " Layer=" << fLayer
	    << " detType=" << fDetectorType
	    << " Pos=" << fPosition.X() << ", " << fPosition.Y() << ", "
	    << fPosition.Z()
	    << " Theta=" << fTheta
	    << " Phi=" << fPhi << std::endl;
  
  Int_t nprm = fEnergyResolutionPrm.size();
  if(nprm>0){
    std::cout << " EResoPrm=";
    for(Int_t i=0;i<nprm;++i) std::cout << fEnergyResolutionPrm.at(i) << ", ";
  }
  std::cout << " TReso(ns)=" << fTimeResolution<< ", " ;
  std::cout << " EThreshold(MeV)=" << fEThreshold<< ", " ;
  std::cout << " EThSigma(MeV)=" << fEThresholdSigma<< " " ;
}
    
