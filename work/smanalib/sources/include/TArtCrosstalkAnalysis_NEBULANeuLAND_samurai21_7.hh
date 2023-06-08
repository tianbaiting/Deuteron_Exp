#ifndef _TARTCROSSTALKANALYSIS_NEBULANeuLAND_SAMURAI21_7_H_
#define _TARTCROSSTALKANALYSIS_NEBULANeuLAND_SAMURAI21_7_H_

#include "TArtCrosstalkAnalysis.hh"

class TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_7 : public TArtCrosstalkAnalysis
{
public:
  TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_7();
  ~TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_7();
  void Analyze();
  void KillInvalidHit();
  void KillCrosstalk();
  void CheckCrosstalk();
  Bool_t IsCrosstalk(TArtNEBULAPla *pla1, TArtNEBULAPla *pla2);

  void SetBetaCut_Upper(Double_t val){fBeta_cut_upper = val;}
  Double_t GetBetaCut_Upper(){return fBeta_cut_upper;}

private:
  std::vector<int> *fKillflag;// 0:OK 1:crsostalk and kill

  Double_t fBeta_cut_upper;

  ClassDef(TArtCrosstalkAnalysis_NEBULANeuLAND_samurai21_7,1)
};
#endif
