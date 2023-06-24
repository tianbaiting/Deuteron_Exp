#ifndef _FRAGSIMDATACONVERTER_BASIC_HH_
#define _FRAGSIMDATACONVERTER_BASIC_HH_

#include "TObject.h"
#include "SimDataConverter.hh"
#include "TFragSimParameter.hh"

class TString;
class TVector3;
class TTree;
class TClonesArray;

class FragSimDataConverter_Basic : public SimDataConverter
{
public:
  FragSimDataConverter_Basic(TString name="FragSimDataConverter_Basic");
  ~FragSimDataConverter_Basic();

  int Initialize();
  int DefineBranch(TTree *tree);
  int ConvertSimData();
  int ClearBuffer();

private:
  TFragSimParameter *fFragSimParameter;
  TClonesArray *fFragSimDataArray;

  Double_t fTargetX, fTargetY, fTargetTheta, fTargetPhi, fTargetEnergy;
  Double_t fPDC1X, fPDC1Y, fPDC2X, fPDC2Y;
  Bool_t   fOK_Target, fOK_PDC1, fOK_PDC2;

};
#endif
