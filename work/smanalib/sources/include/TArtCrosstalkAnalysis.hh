// interface class for crosstalk analysis
#ifndef _TARTCROSSTALKANALYSIS_H_
#define _TARTCROSSTALKANALYSIS_H_

#include <TROOT.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>

#include "TArtNEBULAPla.hh"

class TArtCrosstalkAnalysis : public TObject
{
public:
  TArtCrosstalkAnalysis();
  virtual ~TArtCrosstalkAnalysis();
  
  // user interface
  virtual void SetNEBULAPlaArray(TClonesArray *array){fNEBULAPlaArray = array;}
  virtual void SetNeutThreshold(Double_t val){fNeutThreshold = val;}
  virtual void SetVetoThreshold(Double_t val){fVetoThreshold = val;}
  virtual void Analyze() = 0;// must be inplemented!

  virtual void SetDoKillCrosstalk(Bool_t tf){fDoKillCrosstalk = tf;}
  virtual void SetDoUseVeto(Bool_t tf){fDoUseVeto = tf;}
  virtual void SetSkipCalculateDifference(Bool_t tf){fSkipCalculateDifference = tf;}

  // subroutines in Analyze
  virtual void Clear();
  virtual void Threshold();
  virtual void Veto();
  virtual void RemoveVeto();// included in Veto()
  virtual void KillCrosstalk() = 0; // must be inplemented!
  virtual void CalculateDifference();
  virtual TLorentzVector GetLorentzVector(Int_t in=0);

  // user interface for two hit correlations
  virtual Bool_t GetDoKillCrosstalk(){return fDoKillCrosstalk;}
  virtual Bool_t GetDoUseVeto(){return fDoUseVeto;}
  virtual Bool_t GetSkipCalculateDifference(){return fSkipCalculateDifference;}

  virtual Double_t GetBeta01(){return fBeta01;}
  virtual Double_t GetBeta02(){return fBeta02;}
  virtual Double_t GetBeta12(){return fBeta12;}
  virtual Double_t GetBinv01(){return fBinv01;}
  virtual Double_t GetBinv02(){return fBinv02;}
  virtual Double_t GetBinv12(){return fBinv12;}
  virtual Double_t GetDPos(){return fDPos[3];}
  virtual Double_t GetDPos(Int_t i){return fDPos[i];}
  virtual Double_t GetPos(Int_t in, Int_t i){
    if (in==1) return fPos1[i];
    else if (in==2) return fPos2[i]; 
    else return -9999;}
  virtual Double_t GetDt(){return fDt;}
  virtual Double_t GetT1(){return fT1;}
  virtual Double_t GetT2(){return fT2;}
  virtual Double_t GetQ1(){return fQ1;}
  virtual Double_t GetQ2(){return fQ2;}
  virtual Int_t    GetDLayer(){return fDLayer;}
  virtual Int_t    GetLayer1(){return fLayer1;}
  virtual Int_t    GetLayer2(){return fLayer2;}
  virtual Int_t    GetID1(){return fID1;}
  virtual Int_t    GetID2(){return fID2;}

  TClonesArray *GetNEBULAPlaArray(){return fNEBULAPlaArray;}

protected:
  static const Int_t fMaxNeut = 50;
  static const Int_t fMaxLayer = 4;

  static const Double_t Mn = 939.565;         // MeV
  static const Double_t c_light = 299.792458; // mm/ns

  TClonesArray *fNEBULAPlaArray;

  Double_t fNeutThreshold;
  Double_t fVetoThreshold;

  Bool_t fDoUseVeto;
  Bool_t fDoKillCrosstalk;
  Bool_t fSkipCalculateDifference;

  // variables for crosstalk analysis
  Int_t    fDLayer;
  Int_t    fLayer1, fLayer2;
  Int_t    fID1, fID2;
  Bool_t   fHitVETO[fMaxLayer];
  Double_t fPos1[4],fPos2[4];
  Double_t fDPos[4];
  Double_t fQ1,fQ2;
  Double_t fT1,fT2;
  Double_t fDt;
  Double_t fBeta01,fBeta02,fBeta12;
  Double_t fBinv01,fBinv02,fBinv12;

  ClassDef(TArtCrosstalkAnalysis,1)
};
#endif
