#ifndef _FRAGMENTSIMDATACONVERTER_SIMTRACE_HH_
#define _FRAGMENTSIMDATACONVERTER_SIMTRACE_HH_

#include "TObject.h"
#include "SimDataConverter.hh"
#include "TFragSimParameter.hh"

class TString;
class TTree;
class TClonesArray;

class FragmentSimDataConverter_simtrace : public SimDataConverter
{
public:
  FragmentSimDataConverter_simtrace(TString name="FragmentSimDataConverter_simtrace");
  ~FragmentSimDataConverter_simtrace();

  int Initialize();
  int DefineBranch(TTree *tree);
  int ConvertSimData();
  int ClearBuffer();

private:
  TFragSimParameter *fFragSimParameter;
  TClonesArray *fFragSimDataArray;

  Double_t fFDC1X;
  Double_t fFDC1A;
  Double_t fFDC1Y;
  Double_t fFDC1B;
  Double_t fFDC2X;
  Double_t fFDC2A;
  Double_t fFDC2Y;
  Double_t fFDC2B;

  Bool_t   fOK_FDC1;
  Bool_t   fOK_Window;
  Bool_t   fOK_FDC2;
  Bool_t   fOK_HOD;

};
#endif
