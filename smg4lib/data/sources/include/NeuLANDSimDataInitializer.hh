#ifndef NeuLANDSIMDATAINITIALIZER_HH
#define NeuLANDSIMDATAINITIALIZER_HH

#include "SimDataInitializer.hh"

class TFile;
class TTree;

class NeuLANDSimDataInitializer : public SimDataInitializer
{
public:
  NeuLANDSimDataInitializer(TString name="NeuLANDSimDataInitializer");
  virtual ~NeuLANDSimDataInitializer();
  
  // called in RunActionBasic
  virtual int Initialize();
  virtual int DefineBranch(TTree* tree);
  virtual int AddParameters(TFile* file);
  virtual int RemoveParameters(TFile* file);
  virtual int PrintParameters(TFile* file);

  // called in EventActionBasic
  virtual int ClearBuffer();
};

#endif
