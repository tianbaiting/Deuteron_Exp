#include "TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8.hh"
#include "TArtCore.hh"

//_____________________________________________________________________________
TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8::TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8()
  : TArtCrosstalkAnalysis(),
    fBeta_cut_upper(0.8)
{
  fKillflag = new std::vector<int>(20); // 0:OK 1:crsostalk and kill
}
//_____________________________________________________________________________
TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8::~TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8()
{
  delete fKillflag;
  fKillflag=0;
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8::Analyze(){
  Clear();                      // clear field variables
  Threshold();                  // cut by software threshold
  if (fDoUseVeto) Veto();       // charged particel VETO and remove VETO hit
  RemoveVeto();
  
  KillInvalidHit();

  if (fNEBULAPlaArray->GetEntries()>1){
    fNEBULAPlaArray->Sort();    // sort by time

    if (fDoKillCrosstalk) {
      CheckCrosstalk();
      KillCrosstalk();
    }

    if (!fSkipCalculateDifference) CalculateDifference();  // calculate dpos, beta12, etc.
  }
  return;
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8::KillInvalidHit(){

  int npla = fNEBULAPlaArray->GetEntries();
  for (int i=0; i<npla; ++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    // remove very fast hit
    Double_t FL = sqrt( pow(pla->GetPos(0),2) + pow(pla->GetPos(1),2) + pow(pla->GetPos(2),2) );
    Double_t beta01 = FL/pla->GetTAveSlw()/c_light;
    if (beta01>fBeta_cut_upper) fNEBULAPlaArray->RemoveAt(i);
  }
  fNEBULAPlaArray->Compress();
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8::KillCrosstalk(){
  int npla = fNEBULAPlaArray->GetEntries();

  if ( npla < 2 ) return;// no need to kill

  for (int i=0; i<npla; ++i){
    if ( (*fKillflag)[i]==1 ) fNEBULAPlaArray->RemoveAt(i);
  }
  fNEBULAPlaArray->Compress();
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8::CheckCrosstalk(){
  int npla = fNEBULAPlaArray->GetEntries();

  if ( npla < 2 ) return;// not need to check

  // Initialize
  if ((int)fKillflag->size() < npla ){
    delete fKillflag;
    fKillflag = new std::vector<int>(npla);
  }
  for (int i=0;i<npla;++i) (*fKillflag)[i] = 0;

  // Check crosstalk for all the combinations
  for (int i1=0;i1<npla-1;++i1){
    for (int i2=i1+1;i2<npla;++i2){
      TArtNEBULAPla *pla1 = (TArtNEBULAPla*)fNEBULAPlaArray->At(i1);
      TArtNEBULAPla *pla2 = (TArtNEBULAPla*)fNEBULAPlaArray->At(i2);
      if (IsCrosstalk(pla1,pla2)) (*fKillflag)[i2]=1; // eliminate pla2 (later hit)
    }
  }

}
//_____________________________________________________________________________
Bool_t TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8::IsCrosstalk(TArtNEBULAPla *pla1, TArtNEBULAPla *pla2)
{
  Double_t t1 = pla1->GetTAveSlw();
  Double_t t2 = pla2->GetTAveSlw();
  Double_t q1 = pla1->GetQAveCal();
  Double_t q2 = pla2->GetQAveCal();
  Double_t dt = t2-t1;
  Double_t dx = pla2->GetPos(0) - pla1->GetPos(0);
  Double_t dy = pla2->GetPos(1) - pla1->GetPos(1);
  Double_t dz = pla2->GetPos(2) - pla1->GetPos(2);
  Double_t dr = sqrt( dx*dx + dy*dy + dz*dz );
  

  //------------------------------------------
  // same wall event
  if (pla1->GetLayer() == pla2->GetLayer() ){

    // NEBULA Detectors
    if (pla1->GetDetectorName()->Contains("Neut")){
      Double_t dl = pow((dr-158)/(3.*50.10),2) + pow((dt-0.4956)/(3.*0.6124),2);
      if (dl<1) return true;

      Double_t FL = sqrt( pow(pla1->GetPos(0),2) + pow(pla1->GetPos(1),2) + pow(pla1->GetPos(2),2) );
      Double_t beta01 = FL/pla1->GetTAveSlw()/c_light;
      Double_t beta12inv = dt*c_light/dr;// use inversed value for dt=0 case
      Double_t br = beta01*beta12inv;
      if ( (50.-6.0)/(1.5-1.0)*(fabs(br)-0.5)+6.0 > q2 &&
	   (6.0-50.)/(4.0-1.5)*(fabs(br)-4.0)+6.0 > q2      ) return true;

//      // kill gamma events
//      if ( fabs(fabs(beta12inv)-1.0)<3.0*0.2393 && q2<15 ) return true;

      // NeuLAND Detectors
    }else if  (pla1->GetDetectorName()->Contains("NeuLAND")){
      Double_t dl = pow((dr-73.8871)/(3.*22.7037),2) + pow(dt/(3.*0.396189),2);
      if (dl<1) return true;

      Double_t FL = sqrt( pow(pla1->GetPos(0),2) + pow(pla1->GetPos(1),2) + pow(pla1->GetPos(2),2) );
      Double_t beta01 = FL/pla1->GetTAveSlw()/c_light;
      Double_t beta12inv = dt*c_light/dr;// use inversed value for dt=0 case
      Double_t br = beta01*beta12inv;
      if ( (50.-6.0)/(1.5-1.0)*(fabs(br)-0.5)+6.0 > q2 &&
           (6.0-50.)/(4.0-1.5)*(fabs(br)-4.0)+6.0 > q2    ) return true;

//      // kill gamma events
//      if ( fabs(fabs(beta12inv)-1.0)<3.0*0.2393 && q2<15 ) return true;

    }else {
      std::cout<<"TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8: Strange DetectorName"<<std::endl;
    }


    //----------------------------------------
    // different wall event
  }else{ 
    // kill neutron events
    Double_t FL = sqrt( pow(pla1->GetPos(0),2) + pow(pla1->GetPos(1),2) + pow(pla1->GetPos(2),2) );
    Double_t beta01 = FL/pla1->GetTAveSlw()/c_light;
    Double_t beta12inv = dt*c_light/dr;// use inversed value for dt=0 case
    if (dz<0) beta12inv *= -1.0;// beta12>0 --> 1st hit is upstream
    Double_t br = beta01*beta12inv;

    if ( (pla1->GetLayer()==1 && pla2->GetLayer()==2) || 
	 (pla1->GetLayer()==2 && pla2->GetLayer()==1) ){

      if (fHitVETO[1]) return true;// kill proton events
      if (br>1.036) return true;
      if (fabs(fabs(beta12inv)-1)< 3*0.05204 && q2<15 ) return true;// kill gamma events

    }else if  ( (pla1->GetLayer()==1 && pla2->GetLayer()==3) || 
		(pla1->GetLayer()==3 && pla2->GetLayer()==1) ){

      if (br>1.032) return true;
      if (fabs(fabs(beta12inv)-1)< 3*0.01532 && q2<15 ) return true;// kill gamma events

    }else if  ( (pla1->GetLayer()==2 && pla2->GetLayer()==3) || 
		(pla1->GetLayer()==3 && pla2->GetLayer()==2) ){

      if (fHitVETO[2]) return true;// kill proton events
      if (br>1.034) return true;
      if (fabs(fabs(beta12inv)-1)< 3*0.0543 && q2<15 ) return true;// kill gamma events

    }else{
      std::cout<<"TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_8: unknown Layer combination "
	       <<pla1->GetLayer()<<" "<<pla2->GetLayer()<<std::endl;
      return true;
    }

  }
  //------------------------------------------
  
  return false;
}
//_____________________________________________________________________________
