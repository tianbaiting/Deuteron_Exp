#if defined(TANA)

#ifndef _SAMURAI21BEAMSIMDATACONVERTER_HH_
#define _SAMURAI21BEAMSIMDATACONVERTER_HH_

#include "TObject.h"
#include "SimDataConverter.hh"

class TString;
class TTree;
class TClonesArray;
class TFragSimParameter;

class SAMURAI21BeamSimDataConverter : public SimDataConverter
{
public:
  SAMURAI21BeamSimDataConverter(TString name="SAMURAI21BeamSimDataConverter");
  ~SAMURAI21BeamSimDataConverter();

  int Initialize();
  int DefineBranch(TTree *tree);
  int ConvertSimData();
  int ClearBuffer();

private:
  TFragSimParameter *fFragParameter;

  UInt_t fEventID;
  TClonesArray* fBeamInfoArray;
  Double_t fTGTX;
  Double_t fTGTY;
  Double_t fTGTZ;
  Double_t fTGTA;
  Double_t fTGTB;
  Double_t fBeamZ;
  Double_t fBeamAoZ;

};
#endif

#endif
