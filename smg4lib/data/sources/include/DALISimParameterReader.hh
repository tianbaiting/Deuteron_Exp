#ifndef DALISIMPARAMETERREADER_HH
#define DALISIMPARAMETERREADER_HH

#include "TString.h"
#include <vector>

class SimDataManager;
class TDALISimParameter;
class DALISimParameterReader
{
public:
  DALISimParameterReader();
  ~DALISimParameterReader();

  void ReadDALIParameters(const char* PrmFileName);
  void ReadDALINaIParameters(const char* PrmFileName);
  void CSVToVector(const char* PrmFileName,
		   std::vector<std::vector<TString> >* PrmArray);

  void Print();

public:
  SimDataManager *fSimDataManager;
  TDALISimParameter* fDALISimParameter;
};
#endif
