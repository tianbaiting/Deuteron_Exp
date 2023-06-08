#ifndef _CATANASIMDATACONVERTER_TARTCATANACSI_
#define _CATANASIMDATACONVERTER_TARTCATANACSI_

#include "SimDataConverter.hh"
#include "SimDataInitializer.hh"

#include "TCATANASimParameter.hh"
#include <map>

class TTree;
class TClonesArray;

class CATANASimDataConverter_TArtCATANACsI : public SimDataConverter
{
public:
  CATANASimDataConverter_TArtCATANACsI(TString name = "CATANASimDataConverter_TArtCATANACsI");
  virtual ~CATANASimDataConverter_TArtCATANACsI();

  // called in RuActionBasic from SimDataManager
  virtual int Initialize();
  virtual int DefineBranch(TTree *tree);
  virtual int ConvertSimData();

  // called in EventActionBasic from SimDataManager
  virtual int ClearBuffer();

  void SetIncludeResolution(bool val){fIncludeResolution = val;}
  bool GetIncludeResolution() const {return fIncludeResolution;}

  virtual Double_t ConvoluteEnergyResolution(Double_t Esim,
					   TCATANACsISimParameter* para);

  virtual Double_t RandomizeEnergyThreshold(TCATANACsISimParameter* para);
  
protected:
  bool fIncludeResolution;

  TCATANASimParameter *fCATANASimParameter;
  TClonesArray *fCATANASimDataArray;

  TClonesArray *fCATANACsIArray;

  class tmp_data{
  public:
    tmp_data(): id(-1), layer(0), e(0), t(0), pos(0,0,0)
    {;}
    ~tmp_data(){;}
  public:
    Int_t id, layer;
    Double_t e,t;
    TVector3 pos;
  };
  std::map<int,tmp_data> fDataBufferMap;

};
#endif
