#if defined(TANA)

#ifndef _SAMURAI21FRAGSIMDATACONVERTER_HH_
#define _SAMURAI21FRAGSIMDATACONVERTER_HH_

#include "TObject.h"
#include "SimDataConverter.hh"

class TString;
class TTree;
class TClonesArray;
class TFragSimParameter;

class SAMURAI21FragSimDataConverter : public SimDataConverter
{
public:
  SAMURAI21FragSimDataConverter(TString name="SAMURAI21FragSimDataConverter");
  ~SAMURAI21FragSimDataConverter();

  int Initialize();
  int DefineBranch(TTree *tree);
  int ConvertSimData();
  int ClearBuffer();

private:
  TFragSimParameter *fFragSimParameter;
  TClonesArray *fFragSimDataArray;
  Int_t fOK_Window;

  Int_t fOK_FDC1;
  Int_t fOK_FDC2;
  Double_t fFDC1X;
  Double_t fFDC1Y;
  Double_t fFDC1A;
  Double_t fFDC1B;
  Double_t fFDC2X;
  Double_t fFDC2Y;
  Double_t fFDC2A;
  Double_t fFDC2B;
  Double_t fFragZ;
  Double_t fFragAoZ;
  Double_t fFragBrho;

};
#endif

#endif
