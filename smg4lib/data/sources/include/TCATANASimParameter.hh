#ifndef _TCATANASIMPARAMETER_
#define _TCATANASIMPARAMETER_

#include "TNamed.h"
#include "TVector3.h"
#include <map>

#include "TSimParameter.hh"

class TCATANACsISimParameter;

class TCATANASimParameter : public TSimParameter {

public:
  TCATANASimParameter(TString name="CATANAParameter");
  virtual ~TCATANASimParameter();

  TCATANACsISimParameter *FindCATANACsISimParameter(Int_t id);

  virtual void Print(Option_t *option="") const;
  friend ostream& operator<<(ostream& out, const TCATANASimParameter& prm);

public:
  Bool_t fIsLoaded;
  Int_t fDetectorNum;
  TVector3 fPosition;
  TVector3 fAngle;

  Double_t fV_scinti; // velocity of light in scinti. (mm/ns)

  std::map<int,TCATANACsISimParameter*> fCATANACsISimParameterMap;

  ClassDef(TCATANASimParameter, 1)
};

#endif

