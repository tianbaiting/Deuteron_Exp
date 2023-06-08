#include <iostream>

#include "TSimData.hh"
#include "TCATANASimParameter.hh"
#include "TCATANACsISimParameter.hh"
#include "SimDataManager.hh"
#include "SimDataInitializer.hh"
#include "SimDataConverter.hh"

#include "TArtCATANACsI.hh"

#include <TTree.h>
#include <TClonesArray.h>
#include <TRandom.h>

#include "CATANASimDataConverter_TArtCATANACsI.hh"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
CATANASimDataConverter_TArtCATANACsI::CATANASimDataConverter_TArtCATANACsI(TString name)
  : SimDataConverter(name), fIncludeResolution(true)
{;}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
CATANASimDataConverter_TArtCATANACsI::~CATANASimDataConverter_TArtCATANACsI()
{
  delete fCATANACsIArray;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int CATANASimDataConverter_TArtCATANACsI::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fCATANASimDataArray = sman->FindSimDataArray("CATANASimData");
  if(fCATANASimDataArray == 0){
    std::cout << __FILE__ << "CATANASimDataArray is not found. " << std::endl;
    return 1;
  }

  fCATANACsIArray = new TClonesArray("TArtCATANACsI",100);
  fCATANACsIArray->SetOwner();
  fCATANACsIArray->SetName("CATANACsI");
  return 0;
}
  
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int CATANASimDataConverter_TArtCATANACsI::DefineBranch(TTree* tree)
{
  if(fDataStore) tree->Branch(fCATANACsIArray->GetName(), &fCATANACsIArray);
  return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int CATANASimDataConverter_TArtCATANACsI::ConvertSimData()
{
  fDataBufferMap.clear();

  Int_t nhit = fCATANASimDataArray->GetEntries();
  std::map<int,tmp_data>::iterator it;
  for(Int_t ihit=0;ihit<nhit;++ihit){
    TSimData *simdata = (TSimData*)fCATANASimDataArray->At(ihit);
    it = fDataBufferMap.find(simdata->fID);
    if(it == fDataBufferMap.end()){ // not found
      tmp_data data;
      data.id  = simdata->fID;
      data.e   = simdata->fEnergyDeposit;
      data.t   = simdata->fPreTime;
      data.pos = simdata->fPrePosition;
      fDataBufferMap.insert(std::pair<int,tmp_data>(data.id,data));
    }
    else{
      tmp_data *data = &(it->second);
      data->e += simdata->fEnergyDeposit;
      if(simdata->fPreTime < data->t){ // take earliest timing
	data->t   = simdata->fPreTime;
	data->pos = simdata->fPrePosition;
      }
    }
  }

  SimDataManager *sman = SimDataManager::GetSimDataManager();
  TCATANASimParameter *CATANAParameter = (TCATANASimParameter*)sman->
    FindParameter("CATANAParameter");

  std::map<int, TCATANACsISimParameter*> ParaMap =
    CATANAParameter->fCATANACsISimParameterMap;

  it = fDataBufferMap.begin();
  while(it != fDataBufferMap.end()){
    tmp_data*data = &(it->second);
    Int_t ID = data->id - 20000; // data is offset is 20000
    TCATANACsISimParameter *para = ParaMap[ID];

    Double_t tmpe = ConvoluteEnergyResolution(data->e, para);

   if(tmpe < RandomizeEnergyThreshold(para)){ 
      ++it;
      continue;
    }
    
    Int_t ncsi = fCATANACsIArray->GetEntries();
    TArtCATANACsI *csi = new ((*fCATANACsIArray)[ncsi]) TArtCATANACsI();

    //store converted data
    csi->SetID(ID);
    csi->SetDetectorName(para->GetName());

    if(fIncludeResolution){
      // Position
      csi->SetPositionX(para->fPosition.X());
      csi->SetPositionY(para->fPosition.Y());
      csi->SetPositionZ(para->fPosition.Z());
      // Time
      csi->SetTime(gRandom->Gaus(data->t,para->fTimeResolution));
      // Energy
      csi->SetEnergy(tmpe);
    }
    else{
      csi->SetPositionX(data->pos.X());
      csi->SetPositionY(data->pos.Y());
      csi->SetPositionZ(data->pos.Z());
      csi->SetTime(data->t);
      csi->SetEnergy(data->e);
    }

    ++it;
  }

  return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int CATANASimDataConverter_TArtCATANACsI::ClearBuffer()
{
  fCATANACsIArray->Delete();
  return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Double_t CATANASimDataConverter_TArtCATANACsI::ConvoluteEnergyResolution(Double_t Esim, TCATANACsISimParameter *para){

  Double_t esim_kev = Esim *1000.; //MeV -> keV
  Double_t eout = 0;
  if((para->fEnergyResolutionPrm).size() == 0){ // no parameter file
    // Apply Global parameter
    Double_t a = 0.686569;
    Double_t b = 0.564352;
    Double_t sigma = a * pow(esim_kev,b); // keV
    eout = gRandom->Gaus(esim_kev,sigma);
  }
  else{ // parameter file found -> individual parameter
    Double_t sigma = esim_kev *
      (para->fEnergyResolutionPrm[0] *
       TMath::Power(esim_kev,para->fEnergyResolutionPrm[1]));
    eout = gRandom->Gaus(esim_kev,sigma);
  }

  return eout;
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Since the threshold have width, the threshold is randomized by Gaus
Double_t CATANASimDataConverter_TArtCATANACsI::RandomizeEnergyThreshold(TCATANACsISimParameter* para)
{
  Double_t threshold = para->fEThreshold;
  if( para->fEThreshold>0 && para->fEThresholdSigma>0){
    threshold = gRandom->Gaus(para->fEThreshold, para->fEThresholdSigma);
  }

  return threshold;
}
