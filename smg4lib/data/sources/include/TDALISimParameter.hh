#ifndef TDALISIMPARAMETER_HH
#define TDALISIMPARAMETER_HH

#include "TSimParameter.hh"
#include "TNamed.h"
#include "TVector3.h"
#include <map>

class TDALINaISimParameter;

class TDALISimParameter : public TSimParameter
{
public:
  TDALISimParameter(TString name="DALIParameter");
  virtual ~TDALISimParameter();

  TDALINaISimParameter *FindDALINaISimParameter(Int_t id);

  virtual void Print(Option_t* option="") const;// interited from TNamed
  friend ostream& operator<<(ostream& out,  const TDALISimParameter& prm);


public:

  bool     fIsLoaded;
  Int_t    fDetectorNum;// Total number of crystals
  Int_t    fLayerNum;// Total number of Layers

  TVector3 fPosition;// position of whole system
  TVector3 fDetectorSize;// size of crystal
  TVector3 fSize;// size of whole system (for Mother volume def.)
  TVector3 fAngle;// 0: rot angle around X-axis (deg), 1:Y, 2:Z

  // for Converter
  Double_t fV_scinti;// velocity of light in scintillator (mm/ns)

  //       ID  parameter
  std::map<int,TDALINaISimParameter*> fDALINaISimParameterMap;

  ClassDef(TDALISimParameter, 1)
};

#endif
