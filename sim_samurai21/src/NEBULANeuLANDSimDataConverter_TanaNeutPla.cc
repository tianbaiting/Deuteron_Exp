#if defined(TANA)

#include "NEBULANeuLANDSimDataConverter_TanaNeutPla.hh"

#include "G4RunManager.hh"
#include "TrackingActionBasic.hh"

#include "TSimData.hh"
#include "SimDataManager.hh"
#include "TanaNeutPla.hh"

#include "TTree.h"
#include "TClonesArray.h"
#include "TRandom.h"

#include <iostream>
//____________________________________________________________________
NEBULANeuLANDSimDataConverter_TanaNeutPla::NEBULANeuLANDSimDataConverter_TanaNeutPla(TString name)
  : SimDataConverter(name),
    fIncludeResolution(true)
{
  G4RunManager *RunManager = G4RunManager::GetRunManager();
  fTrackingAction = (TrackingActionBasic*)RunManager->GetUserTrackingAction();
  if (fTrackingAction==0){
    std::cout<<"\x1b[31m"
	     <<__FILE__
	     <<": TrackingActionBasic must be defined before this converter"
	     <<"\x1b[0m"
	     <<std::endl;
  }
}
//____________________________________________________________________
NEBULANeuLANDSimDataConverter_TanaNeutPla::~NEBULANeuLANDSimDataConverter_TanaNeutPla()
{
  delete fNeutPlaArray;
  delete fVetoPlaArray;
}
//____________________________________________________________________
int NEBULANeuLANDSimDataConverter_TanaNeutPla::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();

  fNEBULASimParameter = (TNEBULASimParameter*)sman->FindParameter("NEBULAParameter");
  if (fNEBULASimParameter==0){
    std::cout<<__FILE__<<" : NEBULAParameter is not found."
	     <<std::endl;
    return 1;
  }

  fNeuLANDSimParameter = (TNeuLANDSimParameter*)sman->FindParameter("NeuLANDParameter");
  if (fNeuLANDSimParameter==0){
    std::cout<<__FILE__<<" : NeuLANDParameter is not found."
	     <<std::endl;
    return 1;
  }

  Int_t NumOfNeut = fNEBULASimParameter->fNeutNum + fNeuLANDSimParameter->fNeutNum;
  Int_t NumOfVeto = fNEBULASimParameter->fVetoNum + fNeuLANDSimParameter->fVetoNum;

  fNEBULASimDataArray = sman->FindSimDataArray("NEBULASimData");
  if (fNEBULASimDataArray==0){
    std::cout<<"NEBULANeuLANDSimDataConveter_TArtNEBULAPla : NEBULASimDataArray is not found."
	     <<std::endl;
    return 1;
  }
  fNeuLANDSimDataArray = sman->FindSimDataArray("NeuLANDSimData");
  if (fNeuLANDSimDataArray==0){
    std::cout<<"NEBULANeuLANDSimDataConveter_TArtNEBULAPla : NeuLANDSimDataArray is not found."
	     <<std::endl;
    return 1;
  }

  fNeutPlaArray = new TClonesArray("TanaNeutPla",NumOfNeut);
  fNeutPlaArray->SetOwner();
  fNeutPlaArray->SetName("NeutPla");

  fVetoPlaArray = new TClonesArray("TanaNeutPla",NumOfVeto);
  fVetoPlaArray->SetOwner();
  fVetoPlaArray->SetName("VetoPla");

  return 0;
}
//____________________________________________________________________
int NEBULANeuLANDSimDataConverter_TanaNeutPla::DefineBranch(TTree* tree)
{
  if (fDataStore) tree->Branch("NeutPla",&fNeutPlaArray);
  if (fDataStore) tree->Branch("VetoPla",&fVetoPlaArray);
  return 0;
}
//____________________________________________________________________
int NEBULANeuLANDSimDataConverter_TanaNeutPla::ConvertSimData()
{
  fDataBufferMap.clear();
  fDataBufferMap_TrackID.clear();

  std::map<int,tmp_data>::iterator it;
  std::map<int,tmp_data*>::iterator itp;

  Int_t nhit_nebula = fNEBULASimDataArray->GetEntries();
  for (int ihit=0;ihit<nhit_nebula;++ihit){
    TSimData* simdata = (TSimData*)fNEBULASimDataArray->At(ihit);
    it = fDataBufferMap.find(simdata->fID);
    if (it==fDataBufferMap.end()) {
      tmp_data data;
      data.id       = simdata->fID;
      data.q        = simdata->fEnergyDeposit;
      data.light    = MeVtoMeVee(simdata->fPreKineticEnergy,
				 simdata->fPreKineticEnergy -
				 simdata->fEnergyDeposit,
				 simdata->fParticleName.Data(),
				 simdata->fPDGCode);
      data.t        = simdata->fPreTime;
      data.pos      = simdata->fPrePosition;
      data.track_id = simdata->fTrackID;
      fDataBufferMap.insert(std::pair<int,tmp_data>(data.id,data));

      tmp_data* datap = &(fDataBufferMap[data.id]);
      ///
      itp = fDataBufferMap_TrackID.find(datap->track_id);
      if (itp == fDataBufferMap_TrackID.end()){
	datap->isprimary = true;
	fDataBufferMap_TrackID.insert(std::pair<int,tmp_data*>(datap->track_id,datap));
      }else {
	tmp_data *ddd = itp->second;
	if (ddd->t > datap->t){
	  ddd->isprimary = false;
	  datap->isprimary = true;
	  fDataBufferMap_TrackID[data.track_id] = datap;
	}
      }
      ///

    }else{
      tmp_data* data = &(it->second);
      data->q  += simdata->fEnergyDeposit;
      data->light += MeVtoMeVee(simdata->fPreKineticEnergy,
				 simdata->fPreKineticEnergy -
				 simdata->fEnergyDeposit,
				 simdata->fParticleName.Data(),
				 simdata->fPDGCode);
      if (simdata->fPreTime<data->t ) { // take earliest timing
	data->t   = simdata->fPreTime;
	data->pos = simdata->fPrePosition;
	if (simdata->fTrackID != data->track_id)
	  data->track_id = simdata->fTrackID;

	itp = fDataBufferMap_TrackID.find(data->track_id);
	if (itp == fDataBufferMap_TrackID.end()){
	  data->isprimary = true;
	  fDataBufferMap_TrackID.insert(std::pair<int,tmp_data*>(data->track_id,data));
	}else {
	  tmp_data *ddd = itp->second;
	  if (ddd->t > data->t){
	    ddd->isprimary = false;
	    data->isprimary = true;
	    fDataBufferMap_TrackID[data->track_id] = data;
	  }
	}
	///
      }

    }
  }

  Int_t nhit_neuland = fNeuLANDSimDataArray->GetEntries();
  for (int ihit=0;ihit<nhit_neuland;++ihit){
    TSimData* simdata = (TSimData*)fNeuLANDSimDataArray->At(ihit);
    it = fDataBufferMap.find(simdata->fID);
    if (it==fDataBufferMap.end()) {
      tmp_data data;
      data.id       = simdata->fID;
      data.q        = simdata->fEnergyDeposit;
      data.light    = MeVtoMeVee(simdata->fPreKineticEnergy,
				 simdata->fPreKineticEnergy -
				 simdata->fEnergyDeposit,
				 simdata->fParticleName.Data(),
				 simdata->fPDGCode);
      data.t        = simdata->fPreTime;
      data.pos      = simdata->fPrePosition;
      data.track_id = simdata->fTrackID;

      fDataBufferMap.insert(std::pair<int,tmp_data>(data.id,data));

      tmp_data* datap = &(fDataBufferMap[data.id]);
      itp = fDataBufferMap_TrackID.find(datap->track_id);
      if (itp == fDataBufferMap_TrackID.end()){
	datap->isprimary = true;
	fDataBufferMap_TrackID.insert(std::pair<int,tmp_data*>(datap->track_id,datap));
      }else {
	tmp_data *ddd = itp->second;
	if (ddd->t > datap->t){
	  ddd->isprimary = false;
	  datap->isprimary = true;
	  fDataBufferMap_TrackID[data.track_id] = datap;
	}
      }

    }else{
      tmp_data* data = &(it->second);
      data->q  += simdata->fEnergyDeposit;
      data->light += MeVtoMeVee(simdata->fPreKineticEnergy,
				 simdata->fPreKineticEnergy -
				 simdata->fEnergyDeposit,
				 simdata->fParticleName.Data(),
				 simdata->fPDGCode);
      if (simdata->fPreTime<data->t ) { // take earliest timing
	data->t   = simdata->fPreTime;
	data->pos = simdata->fPrePosition;
	if (simdata->fTrackID != data->track_id)
	  data->track_id = simdata->fTrackID;

	itp = fDataBufferMap_TrackID.find(data->track_id);
	if (itp == fDataBufferMap_TrackID.end()){
	  data->isprimary = true;
	  fDataBufferMap_TrackID.insert(std::pair<int,tmp_data*>(data->track_id,data));
	}else {
	  tmp_data *ddd = itp->second;
	  if (ddd->t > data->t){
	    ddd->isprimary = false;
	    data->isprimary = true;
	    fDataBufferMap_TrackID[data->track_id] = data;
	  }
	}
      }

    }
  }

  it = fDataBufferMap.begin();
  while (it!=fDataBufferMap.end()){
    tmp_data* data = &(it->second);
    TanaNeutPla *pla = 0;

    Int_t ID = data->id;
    TDetectorSimParameter *prm_nebula = fNEBULASimParameter->FindDetectorSimParameter(ID);
    if (prm_nebula!=0) { // NEBULA
      TDetectorSimParameter *prm = prm_nebula;

      if (prm->fSubLayer!=0){// Neut
	Int_t npla = fNeutPlaArray->GetEntries();
	pla = new ((*fNeutPlaArray)[npla]) TanaNeutPla();
      }else{                 // Veto
	Int_t npla = fVetoPlaArray->GetEntries();
	pla = new ((*fVetoPlaArray)[npla]) TanaNeutPla();
      }

      // store converted data
      pla->SetID(data->id);
      pla->SetLayer(prm->fLayer);
      pla->SetSubLayer(prm->fSubLayer);
      pla->SetDetectorName(prm->GetName());
      pla->SetTrackID(data->track_id);
      pla->SetPrimaryTrackID(FindPrimaryTrackID(data->track_id));
      pla->SetIsPrimarySim(data->isprimary);
      //pla->SetDetPos(prm->fPosition.x()+fNEBULASimParameter->fPosition.x(),0);
      //pla->SetDetPos(prm->fPosition.y()+fNEBULASimParameter->fPosition.y(),1);
      //pla->SetDetPos(prm->fPosition.z()+fNEBULASimParameter->fPosition.z(),2);

      if (fIncludeResolution){
	Double_t AttLen=9999, Xsiz=9999, Ysiz=9999, Zsiz=9999;
	if (prm->fDetectorType=="Neut"){
	  Xsiz = fNEBULASimParameter->fNeutSize.x();
	  Ysiz = fNEBULASimParameter->fNeutSize.y();
	  Zsiz = fNEBULASimParameter->fNeutSize.z();
	  AttLen=fNEBULASimParameter->fAttLen_Neut;
	}else if (prm->fDetectorType=="Veto"){
	  Xsiz = fNEBULASimParameter->fVetoSize.x();
	  Ysiz = fNEBULASimParameter->fVetoSize.y();
	  Zsiz = fNEBULASimParameter->fVetoSize.z();
	  AttLen=fNEBULASimParameter->fAttLen_Veto;
	}
	Double_t V_scinti = fNEBULASimParameter->fV_scinti;

	Double_t y_at_detec = data->pos.y() 
	  - ( prm->fPosition.y() + fNEBULASimParameter->fPosition.y() );
	Double_t dy_u = 0.5*Ysiz - y_at_detec;
	Double_t dy_d = 0.5*Ysiz + y_at_detec;

	Double_t tu = data->t + dy_u/V_scinti;
	Double_t td = data->t + dy_d/V_scinti;
	tu += gRandom->Gaus(0,fNEBULASimParameter->fTimeReso);
	td += gRandom->Gaus(0,fNEBULASimParameter->fTimeReso);
	Double_t t = 0.5*(tu+td) - 0.5*Ysiz/V_scinti;
	Double_t dt = td - tu;

	Double_t qu = data->light * exp(-dy_u/AttLen) * fNEBULASimParameter->fQ_factor;
	Double_t qd = data->light * exp(-dy_d/AttLen) * fNEBULASimParameter->fQ_factor;
	
	//Double_t q = sqrt(qu*qd);
	Double_t q = sqrt(qu*qd)*exp(0.5*Ysiz/AttLen);

	Double_t x = prm->fPosition.x() + fNEBULASimParameter->fPosition.x();
	Double_t z = prm->fPosition.z() + fNEBULASimParameter->fPosition.z();

	Double_t y = 0.5*dt*V_scinti + prm->fPosition.y() + fNEBULASimParameter->fPosition.y();

	pla->SetRandX(x + gRandom->Uniform(-0.5*Xsiz,0.5*Xsiz));
	pla->SetRandY(y);
	pla->SetRandZ(z + gRandom->Uniform(-0.5*Zsiz,0.5*Zsiz));
	pla->SetX(x);
	pla->SetY(y);
	pla->SetZ(z);
	pla->SetTOF(t);
	//pla->SetQUCal(qu);
	//pla->SetQDCal(qd);
	pla->SetQ(q);

      }else{// without resolutions
	pla->SetRandX(data->pos.x());
	pla->SetRandY(data->pos.y());
	pla->SetRandZ(data->pos.z());
	pla->SetX(data->pos.x());
	pla->SetY(data->pos.y());
	pla->SetZ(data->pos.z());
	pla->SetTOF(data->t);
	//pla->SetQUCal(data->q);
	//pla->SetQDCal(data->q);
	pla->SetQ(data->q);
      }
    }//if (prm_nebula!=0) { // NEBULA


    TDetectorSimParameter *prm_neuland = fNeuLANDSimParameter->FindDetectorSimParameter(ID);
    if (prm_neuland!=0) { // NeuLAND
      TDetectorSimParameter *prm = prm_neuland;

      if (prm->fSubLayer!=0){// Neut
	Int_t npla = fNeutPlaArray->GetEntries();
	pla = new ((*fNeutPlaArray)[npla]) TanaNeutPla();
      }else{                 // Veto
	Int_t npla = fVetoPlaArray->GetEntries();
	pla = new ((*fVetoPlaArray)[npla]) TanaNeutPla();
      }

      // store converted data
      pla->SetID(data->id);
      pla->SetLayer(prm->fLayer);
      pla->SetSubLayer(prm->fSubLayer);
      pla->SetDetectorName(prm->GetName());
      pla->SetTrackID(data->track_id);
      pla->SetPrimaryTrackID(FindPrimaryTrackID(data->track_id));
      pla->SetIsPrimarySim(data->isprimary);
      //pla->SetDetPos(prm->fPosition.x()+fNeuLANDSimParameter->fPosition.x(),0);
      //pla->SetDetPos(prm->fPosition.y()+fNeuLANDSimParameter->fPosition.y(),1);
      //pla->SetDetPos(prm->fPosition.z()+fNeuLANDSimParameter->fPosition.z(),2);

      if (fIncludeResolution){
	Double_t AttLen=9999, Xsiz=9999, Ysiz=9999, Zsiz=9999;
	if (prm->fDetectorType=="NeuLAND"){
	  Xsiz = fNeuLANDSimParameter->fNeutSize.x();
	  Ysiz = fNeuLANDSimParameter->fNeutSize.y();
	  Zsiz = fNeuLANDSimParameter->fNeutSize.z();
	  AttLen=fNeuLANDSimParameter->fAttLen_Neut;
	}else if (prm->fDetectorType=="NeuLANDVeto"){
	  Xsiz = fNeuLANDSimParameter->fVetoSize.x();
	  Ysiz = fNeuLANDSimParameter->fVetoSize.y();
	  Zsiz = fNeuLANDSimParameter->fVetoSize.z();
	  AttLen=fNeuLANDSimParameter->fAttLen_Veto;
	}
	Double_t V_scinti = fNeuLANDSimParameter->fV_scinti;

	Double_t tu=-9999,td=-9999,x=-9999,y=-9999,dl_u=-9999,dl_d=-9999;
	if (fabs(prm->fAngle.z())<1) {// 0 degree
	  Double_t y_at_detec = data->pos.y() 
	    - ( prm->fPosition.y() + fNeuLANDSimParameter->fPosition.y() );
	  dl_u = 0.5*Ysiz - y_at_detec;
	  dl_d = 0.5*Ysiz + y_at_detec;
	  tu = data->t + dl_u/V_scinti;
	  td = data->t + dl_d/V_scinti;
	  tu += gRandom->Gaus(0,fNeuLANDSimParameter->fTimeReso);
	  td += gRandom->Gaus(0,fNeuLANDSimParameter->fTimeReso);
	  Double_t dt = td - tu;
	  y = 0.5*dt*V_scinti + prm->fPosition.y() + fNeuLANDSimParameter->fPosition.y();
	  x = prm->fPosition.x() + fNeuLANDSimParameter->fPosition.x();
	}else if (fabs(prm->fAngle.z()-90)<1) {// 90 degrees
	  Double_t x_at_detec = data->pos.x() 
	    - ( prm->fPosition.x() + fNeuLANDSimParameter->fPosition.x() );
	  dl_u = 0.5*Ysiz - x_at_detec;
	  dl_d = 0.5*Ysiz + x_at_detec;
	  tu = data->t + dl_u/V_scinti;
	  td = data->t + dl_d/V_scinti;
	  tu += gRandom->Gaus(0,fNeuLANDSimParameter->fTimeReso);
	  td += gRandom->Gaus(0,fNeuLANDSimParameter->fTimeReso);
	  Double_t dt = td - tu;
	  x = 0.5*dt*V_scinti + prm->fPosition.x() + fNeuLANDSimParameter->fPosition.x();
	  y = prm->fPosition.y() + fNeuLANDSimParameter->fPosition.y();
	}
	Double_t t = 0.5*(tu+td) - 0.5*Ysiz/V_scinti;

	Double_t qu = data->light * exp(-dl_u/AttLen) * fNeuLANDSimParameter->fQ_factor;
	Double_t qd = data->light * exp(-dl_d/AttLen) * fNeuLANDSimParameter->fQ_factor;
	//Double_t q = sqrt(qu*qd);
	Double_t q = sqrt(qu*qd)*exp(0.5*Ysiz/AttLen);

	Double_t z = prm->fPosition.z() + fNeuLANDSimParameter->fPosition.z();


        if (fabs(prm->fAngle.z())<1) {// 0 degree
	  pla->SetRandX(x + gRandom->Uniform(-0.5*Xsiz,0.5*Xsiz) );
	  pla->SetRandY(y);
	  pla->SetRandZ(z + gRandom->Uniform(-0.5*Zsiz,0.5*Zsiz) );
	}else if (fabs(prm->fAngle.z()-90)<1) {// 90 degrees
	  pla->SetRandX(x);
	  pla->SetRandY(y + gRandom->Uniform(-0.5*Xsiz,0.5*Xsiz) );
	  pla->SetRandZ(z + gRandom->Uniform(-0.5*Zsiz,0.5*Zsiz) );
	}
	pla->SetX(x);
	pla->SetY(y);
	pla->SetZ(z);
	pla->SetTOF(t);
	//pla->SetQUCal(qu);
	//pla->SetQDCal(qd);
	pla->SetQ(q);

      }else{// without resolutions
	pla->SetRandX(data->pos.x());
	pla->SetRandY(data->pos.y());
	pla->SetRandZ(data->pos.z());
	pla->SetX(data->pos.x());
	pla->SetY(data->pos.y());
	pla->SetZ(data->pos.z());
	pla->SetTOF(data->t);
	pla->SetQ(data->q);
	//pla->SetQUCal(data->q);
	//pla->SetQDCal(data->q);
      }
    }//if (prm_neuland!=0) { // NeuLAND

    it++;
  }

  return 0;
}
//____________________________________________________________________
int NEBULANeuLANDSimDataConverter_TanaNeutPla::ClearBuffer()
{
  fNeutPlaArray->Delete();
  fVetoPlaArray->Delete();
  return 0;
}
//____________________________________________________________________
//____________________________________________________________________
//fit by
// D.Fox et al.    Nucl. Inst. and Meth. A374(1996)63 
Double_t NEBULANeuLANDSimDataConverter_TanaNeutPla::MeVeeRelation_FOX(Double_t T, std::string name, Int_t pdgcode)
{
  Double_t a1, a2, a3, a4;

  //    std::cout << pdgcode << std::endl;
  //  std::cout << name << " " << pdgcode << std::endl;
  if(name == "e-" || name == "e+" || name == "mu-" || name == "mu+"){//leptons
    a1 = 1;
    a2 = 0;
    a3 = 0;
    a4 = 0;
  }else if(name == "proton"){
    a1 = 0.902713 ;
    a2 = 7.55009  ;
    a3 = 0.0990013;
    a4 = 0.736281 ;
  }else if(name == "deuteron"){
    a1 = 0.891575 ;
    a2 = 12.2122  ;
    a3 = 0.0702262;
    a4 = 0.782977 ;
  }else if(name == "triton"){
    a1 = 0.881489 ;
    a2 = 15.9064  ;
    a3 = 0.0564987;
    a4 = 0.811916 ;
  }else if(name == "He3"){
    a1 = 0.803919 ;
    a2 = 34.4153  ;
    a3 = 0.0254322;
    a4 = 0.894859 ;
  }else if(name == "alpha"){
    a1 = 0.781501 ;
    a2 = 39.3133  ;
    a3 = 0.0217115;
    a4 = 0.910333 ;
  }else{
    pdgcode -= 1000000000;
    double z = pdgcode / 10000;
    //    std::cout << z << std::endl;
    //    double a = pdgcode % 10000;
    if(z == 3){ // Li
      // Li7
      a1 = 0.613491 ;
      a2 = 57.1372  ;
      a3 = 0.0115948;
      a4 = 0.951875 ;
    }else if(z == 4){ // be
      // Be9
      a1 = 0.435772 ;
      a2 = 45.538   ;
      a3 = 0.0104221;
      a4 = 0.916373 ;
    }else if(z == 5){ // B
      // B11
      a1 = 0.350273 ;
      a2 = 34.4664  ;
      a3 = 0.0112395;
      a4 = 0.912711 ;
    }else if(z == 6){ // C
      // C12
      a1 = 0.298394 ;
      a2 = 25.5679  ;
      a3 = 0.0130345; 
      a4 = 0.908512 ;
    }else{
      // C12
      a1 = 0.298394 ;
      a2 = 25.5679  ;
      a3 = 0.0130345; 
      a4 = 0.908512 ;
    }
  }
  return a1*T-a2*(1-exp(-a3*pow(T, a4)));
}
//____________________________________________________________________
Double_t NEBULANeuLANDSimDataConverter_TanaNeutPla::MeVtoMeVee(Double_t Tin, Double_t Tout, std::string name, Int_t pdgcode)
{
  Double_t Tein, Teout, dTe;
  Double_t Tout_chk=Tout;
  if (Tout_chk<0) Tout_chk=0;

  Tein  = MeVeeRelation_FOX(Tin, name, pdgcode);
  Teout = MeVeeRelation_FOX(Tout_chk, name, pdgcode);
  dTe = Tein - Teout;

  if(dTe < 0) dTe = 0;

  return dTe;
}
//____________________________________________________________________
int NEBULANeuLANDSimDataConverter_TanaNeutPla::FindPrimaryTrackID(int TrackID)
{
  int parentid = fTrackingAction->GetParentID(TrackID);
  if (parentid<0) {
    std::cout<<__FILE__<<": something wrong with trackid/parentid"<<std::endl;
  }

  if (parentid==0) return TrackID;

  int id;
  while (parentid!=0){
    id = parentid;
    parentid = fTrackingAction->GetParentID(id);
  }
  return id;
}
//____________________________________________________________________
#endif
