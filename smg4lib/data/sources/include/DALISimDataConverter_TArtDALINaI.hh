#ifndef _DALISIMDATACONVERTER_TARTDALINAI_HH_
#define _DALISIMDATACONVERTER_TARTDALINAI_HH_

#include "SimDataConverter.hh"
#include "SimDataInitializer.hh"

#include "TDALISimParameter.hh"
#include <map>

class TTree;
class TClonesArray;

class DALISimDataConverter_TArtDALINaI : public SimDataConverter
{
public:
  DALISimDataConverter_TArtDALINaI(TString name="DALISimDataConverter_TArtDALINaI");
  virtual ~DALISimDataConverter_TArtDALINaI();
  
  // called in RunActionBasic from SimDataManager
  virtual int Initialize();
  virtual int DefineBranch(TTree* tree);
  virtual int ConvertSimData();

  // called in EventActionBasic from SimDataManager
  virtual int ClearBuffer();

  void SetIncludeResolution(bool tf){fIncludeResolution = tf;}
  bool SetIncludeResolution(){return fIncludeResolution;}

  virtual double ConvoluteEnergyResolution(double Esim, TDALINaISimParameter* para);

protected:
  bool fIncludeResolution;

  TDALISimParameter *fDALISimParameter;
  TClonesArray *fDALISimDataArray;

  TClonesArray *fDALINaIArray;

  class tmp_data{
  public:
    tmp_data()
      : id(0), layer(0), 
	e(0), t(0),
	pos(0,0,0)
    {;}
    ~tmp_data(){;}
  public:
    Int_t id,layer;
    Double_t e,t;
    TVector3 pos;
  };
  std::map<int,tmp_data> fDataBufferMap;


};
#endif
