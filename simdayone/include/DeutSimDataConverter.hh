#ifndef _DEUTSIMDATACONVERTER_H_
#define _DEUTSIMDATACONVERTER_H_

#include "TObject.h"
#include "SimDataConverter.hh"
#include "TDeutSimParameter.hh"

class TString;
class TTree;
class TClonesArray;

class DeutSimDataConverter : public SimDataConverter
{
public:
  DeutSimDataConverter(TString name="DeutSimDataConverter");
  ~DeutSimDataConverter();

  int Initialize();
  int DefineBranch(TTree *tree);
  int ConvertSimData();
  int ClearBuffer();

private:
  TDeutSimParameter *fDeutSimParameter;
  TClonesArray *fFragSimDataArray;
  TClonesArray *fNEBULASimDataArray;

  Double_t fTargetX, fTargetY, fTargetZ, fTargetAngle;
  Double_t fPDC1X, fPDC1Y, fPDC2X, fPDC2Y, fEnergy;
  Bool_t   fOK_Target, fOK_PDC1, fOK_PDC2;
  
};
#endif
