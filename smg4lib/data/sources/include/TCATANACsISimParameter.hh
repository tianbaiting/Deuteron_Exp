#ifndef _TCATANACSISIMPARAMETER_
#define _TCATANACSISIMPARAMETER_

#include <vector>

#include <TNamed.h>
#include <TVector3.h>
#include <TString.h>

#include "TSimParameter.hh"

class TCATANACsISimParameter : public TSimParameter
{
public:
  TCATANACsISimParameter(TString name="CATANACsI");
  virtual ~TCATANACsISimParameter();
  virtual void Print(Option_t* option="") const;
  friend ostream& operator<<(ostream&out, const TCATANACsISimParameter& prm);
public:
  Int_t fID;
  Int_t fLayer;
  Int_t fDetectorType;
  TString fDetectorName;
  TVector3 fPosition;
  Double_t fTheta;
  Double_t fPhi;
  std::vector<Double_t> fEnergyResolutionPrm;
  Double_t fEThreshold;
  Double_t fEThresholdSigma;
  Double_t fTimeResolution;

  ClassDef(TCATANACsISimParameter, 1)
};
#endif

