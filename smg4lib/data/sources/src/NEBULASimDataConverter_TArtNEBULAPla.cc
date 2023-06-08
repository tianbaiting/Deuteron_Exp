#include "NEBULASimDataConverter_TArtNEBULAPla.hh"

//#include "TNEBULASimData.hh"
#include "TSimData.hh"
#include "TNEBULASimParameter.hh"
#include "SimDataManager.hh"
#include "SimDataInitializer.hh"
#include "SimDataConverter.hh"

#include "TArtNEBULAPla.hh"

#include "TTree.h"
#include "TClonesArray.h"
#include "TRandom.h"

#include <iostream>
//____________________________________________________________________
NEBULASimDataConverter_TArtNEBULAPla::NEBULASimDataConverter_TArtNEBULAPla(TString name)
  : SimDataConverter(name),
    fIncludeResolution(true)
{}
//____________________________________________________________________
NEBULASimDataConverter_TArtNEBULAPla::~NEBULASimDataConverter_TArtNEBULAPla()
{
  delete fNEBULAPlaArray;
}
//____________________________________________________________________
int NEBULASimDataConverter_TArtNEBULAPla::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fNEBULASimParameter = (TNEBULASimParameter*)sman->FindParameter("NEBULAParameter");
  if (fNEBULASimParameter==0){
    std::cout<<"NEBULASimDataConveter_TArtNEBULAPla : NEBULAParameter is not found."
	     <<std::endl;
    return 1;
  }

  Int_t NumOfDetector = fNEBULASimParameter->fNeutNum + fNEBULASimParameter->fVetoNum;

  fNEBULASimDataArray = sman->FindSimDataArray("NEBULASimData");
  if (fNEBULASimDataArray==0){
    std::cout<<"NEBULANeuLANDSimDataConveter_TArtNEBULAPla : NEBULASimDataArray is not found."
	     <<std::endl;
    return 1;
  }

  fNEBULAPlaArray = new TClonesArray("TArtNEBULAPla",NumOfDetector);
  fNEBULAPlaArray->SetOwner();
  fNEBULAPlaArray->SetName("NEBULAPla");
  return 0;
}
//____________________________________________________________________
int NEBULASimDataConverter_TArtNEBULAPla::DefineBranch(TTree* tree)
{
  if (fDataStore) tree->Branch("NEBULAPla",&fNEBULAPlaArray);
  return 0;
}
//____________________________________________________________________
int NEBULASimDataConverter_TArtNEBULAPla::ConvertSimData()
{
  fDataBufferMap.clear();

  //Int_t nhit = gNEBULASimDataArray->size();
  Int_t nhit = fNEBULASimDataArray->GetEntries();

  std::map<int,tmp_data>::iterator it;
  for (int ihit=0;ihit<nhit;++ihit){
    //TSimData simdata = (*gNEBULASimDataArray)[ihit];
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
      fDataBufferMap.insert(std::pair<int,tmp_data>(data.id,data));

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
      }

    }
  }

  SimDataManager *sman = SimDataManager::GetSimDataManager();
  TNEBULASimParameter *NEBULAParameter = (TNEBULASimParameter*)sman->FindParameter("NEBULAParameter");

  it = fDataBufferMap.begin();
  while (it!=fDataBufferMap.end()){
    tmp_data* data = &(it->second);
    Int_t npla = fNEBULAPlaArray->GetEntries();
    TArtNEBULAPla *pla = new ((*fNEBULAPlaArray)[npla]) TArtNEBULAPla();

    Int_t ID = data->id;
    TDetectorSimParameter *prm = NEBULAParameter->FindDetectorSimParameter(ID);

    // store converted data
    pla->SetID(data->id);
    pla->SetLayer(prm->fLayer);
    pla->SetSubLayer(prm->fSubLayer);
    pla->SetDetectorName(prm->GetName());
    pla->SetDetPos(prm->fPosition.x()+NEBULAParameter->fPosition.x(),0);
    pla->SetDetPos(prm->fPosition.y()+NEBULAParameter->fPosition.y(),1);
    pla->SetDetPos(prm->fPosition.z()+NEBULAParameter->fPosition.z(),2);

    if (fIncludeResolution){
      Double_t AttLen, Xsiz, Ysiz, Zsiz;
      if (prm->fDetectorType=="Neut"){
	Xsiz = NEBULAParameter->fNeutSize.x();
	Ysiz = NEBULAParameter->fNeutSize.y();
	Zsiz = NEBULAParameter->fNeutSize.z();
	AttLen=NEBULAParameter->fAttLen_Neut;
      }else if (prm->fDetectorType=="Veto"){
	Xsiz = NEBULAParameter->fVetoSize.x();
	Ysiz = NEBULAParameter->fVetoSize.y();
	Zsiz = NEBULAParameter->fVetoSize.z();
	AttLen=NEBULAParameter->fAttLen_Veto;
      }
      Double_t V_scinti = NEBULAParameter->fV_scinti;

      Double_t y_at_detec = data->pos.y() 
	- ( prm->fPosition.y() + NEBULAParameter->fPosition.y() );
      Double_t dy_u = 0.5*Ysiz - y_at_detec;
      Double_t dy_d = 0.5*Ysiz + y_at_detec;

      Double_t tu = data->t + dy_u/V_scinti;
      Double_t td = data->t + dy_d/V_scinti;
      tu += gRandom->Gaus(0,NEBULAParameter->fTimeReso);
      td += gRandom->Gaus(0,NEBULAParameter->fTimeReso);
      Double_t t = 0.5*(tu+td) - 0.5*Ysiz/V_scinti;
      Double_t dt = td - tu;

      Double_t qu = data->light * exp(-dy_u/AttLen);
      Double_t qd = data->light * exp(-dy_d/AttLen);
      //Double_t q = sqrt(qu*qd);
      Double_t q = sqrt(qu*qd)*exp(0.5*Ysiz/AttLen);

      Double_t x = prm->fPosition.x() + NEBULAParameter->fPosition.x() + gRandom->Uniform(-0.5*Xsiz,0.5*Xsiz);
      Double_t z = prm->fPosition.z() + NEBULAParameter->fPosition.z() + gRandom->Uniform(-0.5*Zsiz,0.5*Zsiz);

      Double_t y = 0.5*dt*V_scinti + prm->fPosition.y() + NEBULAParameter->fPosition.y();

      pla->SetPos(x,0);
      pla->SetPos(y,1);
      pla->SetPos(z,2);
      pla->SetTAveSlw(t);
      pla->SetQUCal(qu);
      pla->SetQDCal(qd);
      pla->SetQAveCal(q);

    }else{// without resolutions
      pla->SetPos(data->pos.x(),0);
      pla->SetPos(data->pos.y(),1);
      pla->SetPos(data->pos.z(),2);
      pla->SetTAveSlw(data->t);
      pla->SetQAveCal(data->q);
      pla->SetQUCal(data->q);
      pla->SetQDCal(data->q);
    }

    it++;
  }

  return 0;
}
//____________________________________________________________________
int NEBULASimDataConverter_TArtNEBULAPla::ClearBuffer()
{
  fNEBULAPlaArray->Delete();
  return 0;
}
//____________________________________________________________________
//fit by
// D.Fox et al.    Nucl. Inst. and Meth. A374(1996)63 
Double_t NEBULASimDataConverter_TArtNEBULAPla::MeVeeRelation_FOX(Double_t T, std::string name, Int_t pdgcode)
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
Double_t NEBULASimDataConverter_TArtNEBULAPla::MeVtoMeVee(Double_t Tin, Double_t Tout, std::string name, Int_t pdgcode)
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
