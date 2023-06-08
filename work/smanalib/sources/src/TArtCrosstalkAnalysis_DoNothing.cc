#include "TArtCrosstalkAnalysis_DoNothing.hh"

//_____________________________________________________________________________
TArtCrosstalkAnalysis_DoNothing::TArtCrosstalkAnalysis_DoNothing()
  : TArtCrosstalkAnalysis()
{}
//_____________________________________________________________________________
TArtCrosstalkAnalysis_DoNothing::~TArtCrosstalkAnalysis_DoNothing()
{
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis_DoNothing::Analyze(){
  Clear();                      // clear field variables
  Threshold();                  // cut by software threshold
  if (fDoUseVeto) Veto();       // charged particel VETO and remove VETO hit

  RemoveVeto();

  if (fNEBULAPlaArray->GetEntries()>1){
    fNEBULAPlaArray->Sort();    // sort by time

    if (fDoKillCrosstalk) KillCrosstalk();
    if (!fSkipCalculateDifference) CalculateDifference();  // calculate dpos, beta12, etc.
  }
  return;
}
//_____________________________________________________________________________
void TArtCrosstalkAnalysis_DoNothing::KillCrosstalk()
{
  // Do DoNothing
}
//_____________________________________________________________________________
