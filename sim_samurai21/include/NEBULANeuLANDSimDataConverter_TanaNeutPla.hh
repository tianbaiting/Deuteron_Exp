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
class TrackingActionBasic;

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

  int FindPrimaryTrackID(int TrackID);// find trackID for generated particle

protected:
  TrackingActionBasic* fTrackingAction;
  bool fIncludeResolution;

  TNEBULASimParameter *fNEBULASimParameter;
  TNeuLANDSimParameter *fNeuLANDSimParameter;

  TClonesArray *fNEBULASimDataArray;
  TClonesArray *fNeuLANDSimDataArray;

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
	t_sim(0), q_sim(0), pos_sim(0,0,0),
	track_id(-1), isprimary(false)
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
    Int_t track_id;
    Bool_t isprimary;
  };
  //--------------------------------
  std::map<int,tmp_data> fDataBufferMap;
  std::map<int,tmp_data*> fDataBufferMap_TrackID;

};

#endif

#endif // defined(TANA)
