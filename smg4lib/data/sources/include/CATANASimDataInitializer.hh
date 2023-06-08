#ifndef _CATANASIMDATAINITIALIZER_
#define _CATANASIMDATAINITIALIZER_

#include "SimDataInitializer.hh"

class TFile;
class TTree;

class CATANASimDataInitializer : public SimDataInitializer
{
public:
  CATANASimDataInitializer(TString name="CATANASimDataInitializer");
  virtual ~CATANASimDataInitializer();

  // called in RunActionBasic
  virtual int Initialize();
  virtual int DefineBranch(TTree* tree);
  virtual int AddParameters(TFile*){return 0;}
  virtual int RemoveParameters(TFile*){return 0;}
  virtual int PrintParameters(TFile*){return 0;}

  // called in EventActionBasic
  virtual int ClearBuffer();
};

#endif

