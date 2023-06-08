#ifndef NeuLANDSIMPARAMETERREADER_HH
#define NeuLANDSIMPARAMETERREADER_HH

#include "SimDataManager.hh"
#include "TNeuLANDSimParameter.hh"
#include "TDetectorSimParameter.hh"
#include "TSimParameter.hh"

#include "TString.h"

#include <vector>

class NeuLANDSimParameterReader
{
public:
  NeuLANDSimParameterReader();
  ~NeuLANDSimParameterReader();

  void ReadNeuLANDParameters(const char* PrmFileName);
  void ReadNeuLANDDetectorParameters(const char* PrmFileName);
  void CSVToVector(const char* PrmFileName,
		   std::vector<std::vector<TString> >* PrmArray);

public:
  SimDataManager *fSimDataManager;
  TNeuLANDSimParameter* fNeuLANDSimParameter;
};
#endif
