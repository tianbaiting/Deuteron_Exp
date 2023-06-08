#ifndef _CATANASIMPARAMETERREADER_
#define _CATANASIMPARAMETERREADER_

#include <vector>

#include <TString.h>

class SimDataManager;
class TCATANASimParameter;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class CATANASimParameterReader
{
public:
  CATANASimParameterReader();
  ~CATANASimParameterReader();

  void ReadCATANAParameters(const char* PrmFileName);
  void ReadCATANACsIParameters(const char* PrmFileName);
  void CSVToVector(const char *PrmFileName,
		   std::vector<std::vector<TString> >* PrmArray);

public:
  SimDataManager *fSimDataManager;
  TCATANASimParameter* fCATANASimParameter;
};
#endif



