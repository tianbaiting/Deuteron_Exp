#ifndef _TARTCROSSTALKANALYSIS_DONOTHING_H_
#define _TARTCROSSTALKANALYSIS_DONOTHING_H_

#include "TArtCrosstalkAnalysis.hh"

class TArtCrosstalkAnalysis_DoNothing : public TArtCrosstalkAnalysis
{
public:
  TArtCrosstalkAnalysis_DoNothing();
  ~TArtCrosstalkAnalysis_DoNothing();
  void Analyze();
  void KillCrosstalk();

  ClassDef(TArtCrosstalkAnalysis_DoNothing,1)
};
#endif
