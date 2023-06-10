#ifndef TDeutSIMPARAMETER_HH
#define TDeutSIMPARAMETER_HH

#include "TSimParameter.hh"
#include "TNamed.h"
#include "TVector3.h"

class TDeutSimParameter : public TSimParameter
{
public:
  TDeutSimParameter(TString name="DeutParameter");
  virtual ~TDeutSimParameter() override;
  virtual void Print(Option_t* option="") const override;// interited from TNamed
  friend std::ostream& operator<<(std::ostream& out,  const TDeutSimParameter& prm);

public:
  Double_t fTargetThickness;  // target thickness in mm
  TVector3 fTargetPosition;   // position at laboratory coordinate in mm
  Double_t fTargetAngle;      // angle (counterclockwise) in rad

  Double_t fPDCAngle;         // PDC angle (counterclockwise) in rad
  TVector3 fPDC1Position;     // position at rotated coordinate in mm
  TVector3 fPDC2Position;     // position at rotated coordinate in mm

  Double_t fDumpAngle;        // dump angle (counterclockwise) in rad
  TVector3 fDumpPosition;     // position at rotated coordinate in mm

  ClassDefOverride(TDeutSimParameter, 1)
};

#endif
