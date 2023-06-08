#include "DALISimDataConverter_TArtDALINaI.hh"

#include "TSimData.hh"
#include "TDALISimParameter.hh"
#include "TDALINaISimParameter.hh"
#include "SimDataManager.hh"
#include "SimDataInitializer.hh"
#include "SimDataConverter.hh"

#include "TArtDALINaI.hh"

#include "TTree.h"
#include "TClonesArray.h"
#include "TRandom.h"

#include <iostream>
//____________________________________________________________________
DALISimDataConverter_TArtDALINaI::DALISimDataConverter_TArtDALINaI(TString name)
  : SimDataConverter(name),
    fIncludeResolution(true)
{}
//____________________________________________________________________
DALISimDataConverter_TArtDALINaI::~DALISimDataConverter_TArtDALINaI()
{
  delete fDALINaIArray;
}
//____________________________________________________________________
int DALISimDataConverter_TArtDALINaI::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();

  fDALISimDataArray = sman->FindSimDataArray("DALISimData");
  if (fDALISimDataArray==0){
    std::cout<<"DALINeuLANDSimDataConveter_TArtDALINaI : DALISimDataArray is not found."
	     <<std::endl;
    return 1;
  }

  fDALINaIArray = new TClonesArray("TArtDALINaI",128);
  fDALINaIArray->SetOwner();
  fDALINaIArray->SetName("DALINaI");
  return 0;
}
//____________________________________________________________________
int DALISimDataConverter_TArtDALINaI::DefineBranch(TTree* tree)
{
  if (fDataStore) tree->Branch("DALINaI",&fDALINaIArray);
  return 0;
}
//____________________________________________________________________
int DALISimDataConverter_TArtDALINaI::ConvertSimData()
{
  fDataBufferMap.clear();

  Int_t nhit = fDALISimDataArray->GetEntries();

  std::map<int,tmp_data>::iterator it;
  for (int ihit=0;ihit<nhit;++ihit){
    TSimData* simdata = (TSimData*)fDALISimDataArray->At(ihit);
    it = fDataBufferMap.find(simdata->fID);
    if (it==fDataBufferMap.end()) {
      tmp_data data;
      data.id       = simdata->fID;
      data.e        = simdata->fEnergyDeposit;
      data.t        = simdata->fPreTime;
      data.pos      = simdata->fPrePosition;
      fDataBufferMap.insert(std::pair<int,tmp_data>(data.id,data));

    }else{
      tmp_data* data = &(it->second);
      data->e  += simdata->fEnergyDeposit;
      if (simdata->fPreTime<data->t ) { // take earliest timing
	data->t   = simdata->fPreTime;
	data->pos = simdata->fPrePosition;
      }

    }
  }

  SimDataManager *sman = SimDataManager::GetSimDataManager();
  TDALISimParameter *DALIParameter = (TDALISimParameter*)sman->FindParameter("DALIParameter");
  std::map<int, TDALINaISimParameter*> ParaMap 
    = DALIParameter->fDALINaISimParameterMap;

  it = fDataBufferMap.begin();
  while (it!=fDataBufferMap.end()){
    tmp_data* data = &(it->second);
    Int_t ID = data->id;

    TDALINaISimParameter* para = ParaMap[ID];

    Int_t nnai = fDALINaIArray->GetEntries();
    TArtDALINaI *nai = new ((*fDALINaIArray)[nnai]) TArtDALINaI();

    // store converted data
    nai->SetID(data->id);
    nai->SetLayer(para->fLayer);
    nai->SetDetectorName(para->GetName());
    if (fIncludeResolution){
      Double_t costht = para->fPosition.z()/para->fPosition.Mag();
      nai->SetCosTheta(costht);
      Double_t t = data->t;
      Double_t dt = para->fTimeResolution;
      nai->SetTime(gRandom->Gaus(t,dt));
      Double_t e = ConvoluteEnergyResolution(data->e, para);
      nai->SetEnergy(e);
    }else{// without resolutions
      Double_t costht = data->pos.z()/data->pos.Mag();
      nai->SetCosTheta(costht);
      nai->SetTime(data->t);
      nai->SetEnergy(data->e * 1000.);// MeV -> keV
    }

    it++;
  }

  return 0;
}
//____________________________________________________________________
int DALISimDataConverter_TArtDALINaI::ClearBuffer()
{
  fDALINaIArray->Delete();
  return 0;
}
//____________________________________________________________________
double  DALISimDataConverter_TArtDALINaI::ConvoluteEnergyResolution(double Esim, TDALINaISimParameter* para)
{
  double esim_kev = Esim * 1000.;//MeV -> keV
  double e_kev = 0;
  if ((para->fEnergyResolutionPrm).size()==0){
    std::cout<<__FILE__
	     <<" DALI Resolution file is not loaded, skip resolution convolution"
	     <<std::endl;
    e_kev = esim_kev;
  }else{
    Double_t sigma2fwhm = 2.0*sqrt(2.0*log(2));
    Double_t de = para->fEnergyResolutionPrm[0]*sqrt(esim_kev)/sigma2fwhm;
    e_kev = gRandom->Gaus(esim_kev,de);
  }
  return e_kev;
}
//____________________________________________________________________
