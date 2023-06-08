#ifndef TFragSIMPARAMETER_HH
#define TFragSIMPARAMETER_HH

#include "TSimParameter.hh"
#include "TNamed.h"
#include "TVector3.h"

class TFragSimParameter : public TSimParameter
{
public:
  TFragSimParameter(TString name="FragParameter");
  virtual ~TFragSimParameter();
  virtual void Print(Option_t* option="") const;// interited from TNamed
  friend ostream& operator<<(ostream& out,  const TFragSimParameter& prm);

public:
  TVector3 fTargetPosition;// position at Laboratory coordinate in mm
  Double_t fTargetThickness;// target thickness in mm
  TVector3 fFDC1Position;// position at Laboratory coordinate in mm
  TVector3 fFDC2Position;
  TVector3 fWindowHolePosition;
  TVector3 fICFPosition;
  TVector3 fHODPosition;

  Double_t fFDC1Angle;
  Double_t fFDC2Angle;
  Double_t fWindowHoleAngle;
  Double_t fICFAngle;
  Double_t fHODAngle;

  ClassDef(TFragSimParameter, 1)
};

#endif
