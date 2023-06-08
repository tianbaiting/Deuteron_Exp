#include "DALISimParameterReader.hh"

#include "SimDataManager.hh"
#include "TDALISimParameter.hh"
#include "TDALINaISimParameter.hh"

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <math.h>
//____________________________________________________________________
DALISimParameterReader::DALISimParameterReader()
{
  fSimDataManager = SimDataManager::GetSimDataManager();
  fDALISimParameter = (TDALISimParameter*)fSimDataManager->FindParameter("DALIParameter");
  if (fDALISimParameter==0){
    fDALISimParameter = new TDALISimParameter("DALIParameter");
    fSimDataManager->AddParameter(fDALISimParameter);
  }
}
//____________________________________________________________________
DALISimParameterReader::~DALISimParameterReader()
{;}
//____________________________________________________________________
void DALISimParameterReader::ReadDALIParameters(const char* PrmFileName)
{
  std::cout<<"DALISimParameterReader: reading "<<PrmFileName<<std::endl;

  std::vector<std::vector<TString> > PrmArray;
  CSVToVector(PrmFileName, &PrmArray);
  if (PrmArray.size()==0) {
    std::cout<< "\x1b[33m" // yellow colored text
	     << __FILE__ << ": No Parameter is Loaded from "<< PrmFileName 
	     << "\x1b[0m" // red colored text
	     <<std::endl;
    return;
  }

  for (int iline=0;iline<(int)PrmArray.size();++iline){
    std::vector<TString> str_line = PrmArray[iline];
    
    if ("Position"==str_line[0]) fDALISimParameter->fPosition.SetXYZ(str_line[1].Atof(),
								     str_line[2].Atof(),
								     str_line[3].Atof());

    if ("Angle"==str_line[0]) fDALISimParameter->fAngle.SetXYZ(str_line[1].Atof(),
							       str_line[2].Atof(),
							       str_line[3].Atof());
  }

  //std::cout<< "ReadParamDALI : "<<(*fDALISimParameter) <<std::endl;
  //G4RunManager::GetRunManager()->GeometryHasBeenModified();
  
  fDALISimParameter->fIsLoaded = true;

}
//____________________________________________________________________
void DALISimParameterReader::ReadDALINaIParameters(const char* PrmFileName)
{
  std::cout<<"DALISimParameterReader: reading "<<PrmFileName<<std::endl;

  std::vector<std::vector<TString> > PrmArray;
  CSVToVector(PrmFileName, &PrmArray);
  if (PrmArray.size()==0) {
    std::cout<< "\x1b[33m" // yellow colored text
  	     << __FILE__ << ": No Parameter is Loaded from "<< PrmFileName 
  	     << "\x1b[0m" // reset color
  	     <<std::endl;
    return;
  }

  std::map<int,TDALINaISimParameter*> *Map = &(fDALISimParameter->fDALINaISimParameterMap);
  std::map<int,TDALINaISimParameter*>::iterator it;
  
  std::vector<TString> str_line0 = PrmArray[0];// 1st line = list of Name of PRM
  for (int iline=1;iline<(int)PrmArray.size();++iline){
    std::vector<TString> str_line = PrmArray[iline];
    int id=0;

    TDALINaISimParameter* para = 0;
   
    for (int i=0;i<(int)str_line.size();++i){
      if      ("ID"       ==str_line0[i]) {
	id = str_line[i].Atoi();

	it = Map->find(id);
	if (it==Map->end()){
	  para = new TDALINaISimParameter;
	  para->fID     = id;
	  para->SetName(Form("DALINaISimPara%d",id));
	  Map->insert(std::pair<int,TDALINaISimParameter*>(id,para));
	}else{
	  para = it->second;
	}

      }//if      ("ID"       ==str_line0[i]) {

      if      ("Layer"    ==str_line0[i]) para->fLayer  = str_line[i].Atoi();
      else if ("PositionX"==str_line0[i]) para->fPosition.SetX(str_line[i].Atof());
      else if ("PositionY"==str_line0[i]) para->fPosition.SetY(str_line[i].Atof());
      else if ("PositionZ"==str_line0[i]) para->fPosition.SetZ(str_line[i].Atof());	
      else if ("AngleX"==str_line0[i]) para->fAngle.SetX(str_line[i].Atof());
      else if ("AngleY"==str_line0[i]) para->fAngle.SetY(str_line[i].Atof());
      else if ("AngleZ"==str_line0[i]) para->fAngle.SetZ(str_line[i].Atof());
      else if ("DetectorType"==str_line0[i])para->fDetectorType = str_line[i];	
      else if ("rotSign"==str_line0[i])para->frotSign = str_line[i].Atof();
      else if ("CrystalType"==str_line0[i])para->fCrystalType = str_line[i].Atof();

      else if (str_line0[i].Contains("Ereso")){
	TString num_str(str_line0[i].Data());
	num_str.ReplaceAll("Ereso","");
	UInt_t ipara = num_str.Atoi();
	if (ipara+1 > para->fEnergyResolutionPrm.size()){
	  std::vector<Double_t> vtmp(para->fEnergyResolutionPrm.size());
	  copy(para->fEnergyResolutionPrm.begin(),para->fEnergyResolutionPrm.end(),vtmp.begin());
	  para->fEnergyResolutionPrm.resize(ipara+1);
	  copy(vtmp.begin(), vtmp.end(),para->fEnergyResolutionPrm.begin());
	}
	Double_t val = str_line[i].Atof();
	para->fEnergyResolutionPrm[ipara] = val;
      }
      else if (str_line0[i].Contains("Treso"))para->fTimeResolution = str_line[i].Atof();

	
    }//for (int i=0;i<(int)str_line.size();++i){
    //std::cout<<*para<<std::endl;

  }//for (int iline=1;iline<(int)PrmArray.size();++iline){
  if (fDALISimParameter->fDetectorNum==0){
    fDALISimParameter->fDetectorNum = Map->size();
  }else{
    Int_t nread = PrmArray.size()-1;
    if (fDALISimParameter->fDetectorNum != nread)
      std::cout<< "\x1b[33m" // yellow colored text
	       <<"Number of DALINaIParameters="
	       <<PrmArray.size()-1
	       <<" is differen from existing="
	       <<fDALISimParameter->fDetectorNum
	       << "\x1b[0m" // reset color
	       <<std::endl;
  }

  //std::cout<<"Loading DALI detector Parameters -->DONE"<<std::endl;
}
//____________________________________________________________________
void DALISimParameterReader::CSVToVector(const char* PrmFileName, 
					   std::vector<std::vector<TString> >* PrmArray)
{
  //std::cout<<"Convert Parameter Parameters CSV to Vector"<<std::endl;
  std::ifstream ifs(PrmFileName);
  if (ifs.fail()) {
    std::cout << "\x1b[31m" // red colored text
	   <<__FILE__<<": File open error \""<<PrmFileName<<"\""
	   << "\x1b[0m" // reset color
	   <<std::endl;
    return;
  }

  std::string line;
  int iline=0;
  while(getline(ifs,line,'\n')){

    std::vector<TString>* str_line = new std::vector<TString>;

    /* Skip space line */
    TString tsline=line;
    tsline=tsline.Strip(TString::kBoth);
    if (tsline.IsNull()) {iline++;continue;}

    // skip comment line
    if (tsline.First("//")==0) {iline++;continue;}

    //--------------------------------------
    /* Scan one line */
    std::istringstream ss(line);
    std::string s;
    while (getline(ss,s,',')){
      TString ts=s;
      ts=ts.Strip(TString::kBoth);
      if (ts.IsNull()) continue;// skip space column
      str_line->push_back(ts);
    }

    PrmArray->push_back(*str_line);
    ++iline;//increment of line number
  }
  ifs.close();
  //std::cout<<"Convert Parameter Parameters CSV to Vector -->DONE"<<std::endl;
}
//____________________________________________________________________
