#if defined(TANA)
#include "SAMURAI21BeamSimDataConverter.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "TanaBeamInfo.hh"

#include "SimDataManager.hh"
#include "TBeamSimData.hh"
#include "TFragSimParameter.hh"

#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"

#include <iostream>

//____________________________________________________________________
SAMURAI21BeamSimDataConverter::SAMURAI21BeamSimDataConverter(TString name)
  : SimDataConverter(name)
{;}
//____________________________________________________________________
SAMURAI21BeamSimDataConverter::~SAMURAI21BeamSimDataConverter()
{;}
//____________________________________________________________________
int SAMURAI21BeamSimDataConverter::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fFragParameter = (TFragSimParameter*)sman->FindParameter("FragParameter");
  if (fFragParameter==0){
    std::cout<<"SAMURAI21BeamSimDataConveter : FragParameter is not found."
	     <<std::endl;
    return 1;
  }

  fBeamInfoArray = new TClonesArray("TanaBeamInfo",1);
  fBeamInfoArray->SetOwner();
  fBeamInfoArray->SetName("BeamInfoArray");

  ClearBuffer();

  return 0;
}
//____________________________________________________________________
int SAMURAI21BeamSimDataConverter::DefineBranch(TTree *tree)
{
  if (fDataStore) {
    tree->Branch(fBeamInfoArray->GetName(),&fBeamInfoArray);
    tree->Branch("evenum",&fEventID,"evenum/i");
    tree->Branch("tgtz",&fTGTZ,"tgtz/D");
    tree->Branch("beam_z",&fBeamZ,"beam_z/D");
    tree->Branch("beam_aoz",&fBeamAoZ,"beam_aoz/D");
  }
  return 0;
}
//____________________________________________________________________
int SAMURAI21BeamSimDataConverter::ConvertSimData()
{
  const G4Event *event = G4RunManager::GetRunManager()->GetCurrentEvent();
  fEventID = event->GetEventID();

  Int_t ndata = gBeamSimDataArray->size();
  for (int i=0;i<ndata;++i){
    TBeamSimData* data = &((*gBeamSimDataArray)[i]);

    //std::cout<<*data<<std::endl;

    if (data->fZ>0){// heavy ion
      Int_t nbeam = fBeamInfoArray->GetEntries();
      TanaBeamInfo *binfo = new ((*fBeamInfoArray)[nbeam]) TanaBeamInfo();
      binfo->SetBeamPosX(data->fPosition.x());
      binfo->SetBeamPosY(data->fPosition.y());
      binfo->SetBeamAngX(data->fMomentum.Px()/data->fMomentum.Pz()*1000.);// mrad
      binfo->SetBeamAngY(data->fMomentum.Py()/data->fMomentum.Pz()*1000.);
      // z=0 is front face of minos target cell
      fTGTZ = data->fPosition.z() 
	- fFragParameter->fTargetPosition.z() + 0.5*fFragParameter->fTargetThickness;

      //fBeamZ = 0;// not used at this moment...
      //fBeamAoZ = 0;

    }
  }

  return 0;
}
//____________________________________________________________________
int SAMURAI21BeamSimDataConverter::ClearBuffer()
{
  fBeamInfoArray->Delete();
  fTGTZ = -9999;
  fBeamZ = -9999;
  fBeamAoZ = -9999;
  return 0;
}
//____________________________________________________________________

#endif
