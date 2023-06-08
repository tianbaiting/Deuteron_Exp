#include <iostream>
#include <map>
#include <sstream>
#include <fstream>

#include <TMath.h>

#include "SimDataManager.hh"
#include "TCATANASimParameter.hh"
#include "TCATANACsISimParameter.hh"
#include "CATANASimParameterReader.hh"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CATANASimParameterReader::CATANASimParameterReader(){
  fSimDataManager = SimDataManager::GetSimDataManager();
  fCATANASimParameter = (TCATANASimParameter*)fSimDataManager->
    FindParameter("CATANAParameter");
  if(fCATANASimParameter == 0){
    fCATANASimParameter = new TCATANASimParameter("CATANAParameter");
    fSimDataManager->AddParameter(fCATANASimParameter);
  }
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CATANASimParameterReader::~CATANASimParameterReader(){;}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void CATANASimParameterReader::ReadCATANAParameters(const char* PrmFileName)
{
  std::cout << "CATANASimParameterReader: reading " << PrmFileName <<std::endl;

  std::vector<std::vector<TString> > PrmArray;
  CSVToVector(PrmFileName, &PrmArray);
  if(PrmArray.size() == 0){
    std::cout << "\xb[33m"
	      << __FILE__ << ": No Parameter is Loaded from " << PrmFileName
	      << "\x1b[0m" << std::endl;
    return;
  }

  for(size_t iline=0;iline<PrmArray.size();++iline){
    std::vector<TString> str_line = PrmArray[iline];

    if("Position"==str_line[0])
      fCATANASimParameter->fPosition.SetXYZ(str_line[1].Atof(),
					    str_line[2].Atof(),
					    str_line[3].Atof());
    if("Angle"==str_line[0])
      fCATANASimParameter->fAngle.SetXYZ(str_line[1].Atof(),	
					 str_line[2].Atof(),	
					 str_line[3].Atof());
  }

  fCATANASimParameter->fIsLoaded = true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void CATANASimParameterReader::ReadCATANACsIParameters(const char* PrmFileName)
{
  std::vector<std::vector<TString> > PrmArray;
  CSVToVector(PrmFileName, &PrmArray);
  if(PrmArray.size() == 0){
    std::cout << "\xb[33m"
	      << __FILE__ << ": No Parameter is Loaded from " << PrmFileName
	      << "\x1b[0m" << std::endl;
    return;
  }

  std::map<int,TCATANACsISimParameter*> *Map =
    &(fCATANASimParameter->fCATANACsISimParameterMap);

  std::map<int,TCATANACsISimParameter*>::iterator it;

  std::vector<TString> str_line0 = PrmArray[0]; // Prm Name
  for(size_t iline=1;iline<PrmArray.size();++iline){
    std::vector<TString> str_line = PrmArray[iline];
    Int_t id = 0;

    TCATANACsISimParameter* para = 0;

    for(size_t isize=0;isize<str_line.size();++isize){
      if("ID" == str_line0[isize]){
	id = str_line[isize].Atoi();
	it = Map->find(id);
	if(it == Map->end()){ // not found
	  para = new TCATANACsISimParameter;
	  para->fID = id;
	  para->SetName(Form("CATANACsISimPara%d",id));
	  Map->insert(std::pair<int,TCATANACsISimParameter*>(id,para));
	}
	else{
	  para = it->second;
	}

      }
      if("DetectorType" == str_line0[isize])
	para->fDetectorType = str_line[isize].Atoi();
      else if("Layer" == str_line0[isize])
	para->fLayer = str_line[isize].Atoi();
      else if("X" == str_line0[isize])
	para->fPosition.SetX(str_line[isize].Atof());
      else if("Y" == str_line0[isize])
	para->fPosition.SetY(str_line[isize].Atof());
      else if("Z" == str_line0[isize])
	para->fPosition.SetZ(str_line[isize].Atof());
      else if("Theta" == str_line0[isize])
	para->fTheta = str_line[isize].Atof();
      else if("Phi" == str_line0[isize])
	para->fPhi = str_line[isize].Atof();
      
      else if(str_line0[isize].Contains("Ereso")){
	TString num_str(str_line0[isize].Data());
	num_str.ReplaceAll("Ereso","");
	UInt_t ipara = num_str.Atoi();
	if(ipara+1 > para->fEnergyResolutionPrm.size()){
	  std::vector<Double_t> vtmp(para->fEnergyResolutionPrm.size());
	  copy(para->fEnergyResolutionPrm.begin(),
	       para->fEnergyResolutionPrm.end(), vtmp.begin());
	  para->fEnergyResolutionPrm.resize(ipara+1);
	  copy(vtmp.begin(), vtmp.end(), para->fEnergyResolutionPrm.begin());
	}
	
	Double_t val = str_line[isize].Atof();
	para->fEnergyResolutionPrm[ipara] = val;
      }
      
      else if(str_line0[isize].Contains("Treso"))
	para->fTimeResolution = str_line[isize].Atof();
      else if(str_line0[isize].Contains("Ethreshold"))
	para->fEThreshold = str_line[isize].Atof();
      else if(str_line0[isize].Contains("EthSigma"))
	para->fEThresholdSigma = str_line[isize].Atof();
      
      
    }
  }

  if(fCATANASimParameter->fDetectorNum == 0){
    fCATANASimParameter->fDetectorNum = Map->size();
  }
  else{
    Int_t nread = PrmArray.size() - 1;
    if(fCATANASimParameter->fDetectorNum != nread){
      std::cout<< "\x1b[33m" // yellow colored text
	       <<"Number of CATANACsIParameters="
	       <<PrmArray.size()-1
	       <<" is differen from existing="
	       <<fCATANASimParameter->fDetectorNum
	       << "\x1b[0m" // reset color
	       <<std::endl;
    }
  }

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void CATANASimParameterReader::CSVToVector(const char* PrmFileName,
					   std::vector<std::vector<TString> >* PrmArray)
{
  std::ifstream ifs(PrmFileName);
  if(ifs.fail()){
    std::cout << "\x1b[31m" // red colored text
	      <<__FILE__<<": File open error \""<<PrmFileName<<"\""
	      << "\x1b[0m" // reset color
	      <<std::endl;
    return;
  }

  std::string line;
  Int_t iline = 0;
  while(getline(ifs,line, '\n')){
    std::vector<TString>* str_line = new std::vector<TString>;

    /* Skip space line */
    TString tsline = line;
    tsline = tsline.Strip(TString::kBoth);
    if(tsline.IsNull()){
      ++iline;
      continue;
    }

    /* Skip comment line */
    if(tsline.First("//") == 0 || tsline.First("#") == 0){
      ++iline;
      continue;
    }

    /* Scan one line */
    std::istringstream ss(line);
    std::string s;
    while(getline(ss,s,',')){
      TString ts = s;
      ts = ts.Strip(TString::kBoth);
      if(ts.IsNull()) continue; // skip space
      str_line->push_back(ts);
    }

    PrmArray->push_back(*str_line);
    ++iline;
  }
  ifs.close();

}
