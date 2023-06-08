#if defined(TANA)

#ifndef NEBULANEULANDSIMDATACONVERTER_TANANEUTPLA_HH
#define NEBULANEULANDSIMDATACONVERTER_TANANEUTPLA_HH


#include "SimDataConverter.hh"
#include "SimDataInitializer.hh"

#include "TNEBULASimParameter.hh"
#include "TNeuLANDSimParameter.hh"

#include "TString.h"

#include <map>

class TanaNeutPla;
class TTree;
class TClonesArray;

class NEBULANeuLANDSimDataConverter_TanaNeutPla : public SimDataConverter
{
public:
  NEBULANeuLANDSimDataConverter_TanaNeutPla(TString name="NEBULANeuLANDSimDataConverter_TanaNeutPla");
  virtual ~NEBULANeuLANDSimDataConverter_TanaNeutPla();

  // called in RunActionBasic from SimDataManager
  virtual int Initialize();
  virtual int DefineBranch(TTree* tree);
  virtual int ConvertSimData();

  // called in EventActionBasic from SimDataManager
  virtual int ClearBuffer();

  Double_t MeVeeRelation_FOX(Double_t T, std::string name, Int_t pdgcode);
  Double_t MeVtoMeVee(Double_t Tin, Double_t Tout, std::string name, Int_t pdgcode);

  void SetIncludeResolution(bool tf){fIncludeResolution = tf;}
  bool GetIncludeResolution(){return fIncludeResolution;}

protected:
  bool fIncludeResolution;

  TNEBULASimParameter *fNEBULASimParameter;
  TNeuLANDSimParameter *fNeuLANDSimParameter;

  TClonesArray *fNeutPlaArray;
  TClonesArray *fVetoPlaArray;

  //--------------------------------
  class tmp_data{
  public:
    tmp_data()
      : id(0), layer(0), sublayer(0),
	qu(0), qd(0), q(0), light(0),
	tu(0), td(0), t(0),
	pos(0,0,0), beta(0,0,0), p(0,0,0),
	t_sim(0), q_sim(0), pos_sim(0,0,0)
    {;}
    ~tmp_data(){;}
  public:
    Int_t id,layer,sublayer;
    Double_t qu,qd,q, light;
    Double_t tu,td,t;
    TVector3 pos;
    TVector3 beta,p;
    Double_t t_sim;
    Double_t q_sim;
    TVector3 pos_sim;
  };
  //--------------------------------
  std::map<int,tmp_data> fDataBufferMap;

};

#endif

#endif // defined(TANA)
