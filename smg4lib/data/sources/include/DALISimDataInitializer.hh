#ifndef DALISIMDATAINITIALIZER_HH
#define DALISIMDATAINITIALIZER_HH

#include "SimDataInitializer.hh"

class TFile;
class TTree;

class DALISimDataInitializer : public SimDataInitializer
{
public:
  DALISimDataInitializer(TString name="DALISimDataInitializer");
  virtual ~DALISimDataInitializer();
  
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
