#include "TArtCrosstalkAnalysis.hh"
#include "TArtCore.hh"
#include "TIterator.h"
#include <iostream>

//_____________________________________________________________________________
TArtCrosstalkAnalysis::TArtCrosstalkAnalysis()
  : fNeutThreshold(-1), fVetoThreshold(-1),
    fDoUseVeto(true), fDoKillCrosstalk(true),
    fSkipCalculateDifference(false)
{
  Clear();
}
//_____________________________________________________________________________
TArtCrosstalkAnalysis::~TArtCrosstalkAnalysis(){
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis::Clear(){
  for (int i=0;i<fMaxLayer;i++) fHitVETO[i] = false;

  fLayer1=-1;
  fLayer2=-1;
  fDLayer=-1;
  fID1=-1;
  fID2=-1;
  for (Int_t i=0;i<4;++i) {
    fPos1[i]=-9999;
    fPos2[i]=-9999;
    fDPos[i]=-9999;
  }
  fQ1=-9999;
  fQ2=-9999;
  fT1=-9999;
  fT2=-9999;
  fDt=-99999;
  fBeta01=-9999;
  fBeta02=-9999;
  fBeta12=-9999;
  fBinv01=-9999;
  fBinv02=-9999;
  fBinv12=-9999;

}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis::Threshold(){

  Int_t npla = fNEBULAPlaArray->GetEntries();
  for (int i=0;i<npla;++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    Double_t thre=-1;
    if ( pla->GetSubLayer()!=0 ) {
      thre = fNeutThreshold;
      if (thre<0){
	TArtCore::Warning(__FILE__,"Threshold is not set. SetNeutThreshold must be done.");
      }

    } else {
      thre = fVetoThreshold;
      if (thre<0){
	TArtCore::Warning(__FILE__,"Threshold is not set. SetVetoThreshold must be done.");
      }
    }

    if ( pla->GetQAveCal()<thre ) fNEBULAPlaArray->Remove(pla);

  }
  fNEBULAPlaArray->Compress();
  return;
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis::Veto(){
  // simple VETO algorithm
  // remove only hit of Neut at 1wall when hit of VETO at 1wall exists

  int npla = fNEBULAPlaArray->GetEntries();
  for (int i=0;i<npla;i++){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if ( pla->GetSubLayer() == 0 ) // VETO
      fHitVETO[pla->GetLayer()-1]=true;
  }

  for (int i=0; i<npla; ++i) {
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if ( pla->GetLayer() == 1 && fHitVETO[pla->GetLayer()-1] )  
      fNEBULAPlaArray->RemoveAt(i); // remove Neut & Veto at 1st Wall
  }
  fNEBULAPlaArray->Compress();

  return;
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis::RemoveVeto(){
  int npla = fNEBULAPlaArray->GetEntries();
  for (int i=0;i<npla;++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if ( pla->GetSubLayer()==0 ) // remove VETO
      fNEBULAPlaArray->RemoveAt(i);

  }
  fNEBULAPlaArray->Compress();
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis::CalculateDifference(){
  // should be called after fNEBULAPlaArray->Sort()
  Int_t Multi = fNEBULAPlaArray->GetEntries();
  if (Multi<2) return;
  TArtNEBULAPla *pla1 = (TArtNEBULAPla*)fNEBULAPlaArray->At(0);
  TArtNEBULAPla *pla2 = (TArtNEBULAPla*)fNEBULAPlaArray->At(1);

  fLayer1 = pla1->GetLayer();
  fLayer2 = pla2->GetLayer();
  fDLayer = pla2->GetLayer() - pla1->GetLayer();
  fID1 = pla1->GetID();
  fID2 = pla2->GetID();
  
  for (Int_t i=0;i<3;++i){
    fPos1[i] = pla1->GetPos(i);
    fPos2[i] = pla2->GetPos(i);
    fDPos[i] = pla2->GetPos(i) - pla1->GetPos(i);
  }
  fPos1[3] = sqrt( pow(fPos1[0],2) + pow(fPos1[1],2) + pow(fPos1[2],2) );
  fPos2[3] = sqrt( pow(fPos2[0],2) + pow(fPos2[1],2) + pow(fPos2[2],2) );
  fDPos[3] = sqrt( pow(fDPos[0],2) + pow(fDPos[1],2) + pow(fDPos[2],2) );

  fDt = pla2->GetTAveSlw() - pla1->GetTAveSlw();
  fT1 = pla1->GetTAveSlw();
  fT2 = pla2->GetTAveSlw();
  fQ1 = pla1->GetQAveCal();
  fQ2 = pla2->GetQAveCal();

  Double_t FL;
  FL = sqrt( pow(pla1->GetPos(0),2) + pow(pla1->GetPos(1),2) + pow(pla1->GetPos(2),2) );
  fBeta01 = FL/pla1->GetTAveSlw()/c_light;
  FL = sqrt( pow(pla2->GetPos(0),2) + pow(pla2->GetPos(1),2) + pow(pla2->GetPos(2),2) );
  fBeta02 = FL/pla2->GetTAveSlw()/c_light;
  fBinv01 = 1./fBeta01;
  fBinv02 = 1./fBeta02;
  fBinv12 = fDt*c_light/fDPos[3];
  if (fDPos[2]<0) fBinv12 *= -1.0;
  fBeta12 = 1./fBinv12;

  return;
}
//_____________________________________________________________________________
TLorentzVector TArtCrosstalkAnalysis::GetLorentzVector(Int_t in){
  if (in>=fMaxNeut){
    TArtCore::Warning(__FILE__,"Too much hits of neutron, ingored.");
  }

  TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(in);

  Double_t FL = sqrt( pow(pla->GetPos(0),2) + pow(pla->GetPos(1),2) + pow(pla->GetPos(2),2) );
  Double_t beta = FL/pla->GetTAveSlw()/c_light;

  // Momentum Vector of Fragment
  Double_t gamma = 1./sqrt((1.0+beta)*(1.0-beta));
  Double_t Pabs = Mn*beta*gamma;
  Double_t Etot = Mn*gamma;

  Double_t dxdz = pla->GetPos(0)/pla->GetPos(2);
  Double_t dydz = pla->GetPos(1)/pla->GetPos(2);
  Double_t vec_u = sqrt( 1.0 + pow(dxdz,2) + pow(dydz,2) );


  TLorentzVector P(dxdz/vec_u * Pabs,
		   dydz/vec_u * Pabs,
		   1.0/vec_u  * Pabs,
		   Etot);
  return P;
}
//_____________________________________________________________________________
