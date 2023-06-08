#ifndef TDALINAISIMPARAMETER_HH
#define TDALINAISIMPARAMETER_HH

#include "TSimParameter.hh"
#include "TNamed.h"
#include "TVector3.h"
#include "TString.h"
#include <vector>

class TDALINaISimParameter : public TSimParameter
{
public:
  TDALINaISimParameter(TString name="");
  virtual ~TDALINaISimParameter();
  virtual void Print(Option_t* option="") const;// interited from TNamed
  friend ostream& operator<<(ostream& out,  const TDALINaISimParameter& prm);

public:
  Int_t    fID;
  Int_t    fLayer;
  TString  fDetectorType;
  TString  fDetectorName;
  TVector3 fPosition;// position 
  TVector3 fSize;// size of Crystal
  TVector3 fAngle;// 0: rot angle around X-axis (deg), 1:Y, 2:Z
  Double_t fThreshold;
  Int_t    frotSign;
  Int_t    fCrystalType;
  std::vector<Double_t> fEnergyResolutionPrm;
  Double_t fTimeResolution;

  ClassDef(TDALINaISimParameter, 1)
};

#endif
