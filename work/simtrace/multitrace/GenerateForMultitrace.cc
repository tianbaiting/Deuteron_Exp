#include "TBeamSimData.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TGenPhaseSpace.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <iostream>
#include <sstream>
#include <fstream>

void GenerateForMultitrace(TString filename="multitrace/multitrace.inp"){
  const double AMU = 931.494043;//MeV
  const double c_light=299.792458;//mm/ns

  std::vector<string> vec_str;
  std::vector<int> vec_A;
  std::vector<int> vec_Z;
  std::vector<double> vec_M;//mass
  std::vector<double> vec_E;// kinetic energy in MeV/u

  //-----------------------------------------
  string str_tmp;
  int A_tmp,Z_tmp;
  double M_tmp,E_tmp;

  ifstream ifs;
  ifs.open(filename.Data(), ios::in);
  cout<<"file: "<<filename.Data()<<" open"<<endl;
  string line;
  while(!ifs.eof()){
    getline(ifs,line);

    if (line.find("#")==0) continue;
    if (line.empty()) continue;

    stringstream ss(line);
    ss>>str_tmp
      >>A_tmp
      >>Z_tmp
      >>M_tmp
      >>E_tmp;

    vec_str.push_back(str_tmp);
    vec_A.push_back(A_tmp);
    vec_Z.push_back(Z_tmp);
    vec_M.push_back(M_tmp);//MeV
    vec_E.push_back(E_tmp);

    cout<<ss.str().c_str()<<endl;
  }

  stringstream ss;
  ss<<"multitrace/input.root";
  TFile *file = new TFile(ss.str().c_str(),"recreate");
  cout<<file->GetName()<<endl;

  TTree *tree = new TTree("tree_input","Input tree for trajectories");
  gBeamSimDataArray = new TBeamSimDataArray();
  tree->Branch("TBeamSimData", gBeamSimDataArray);

  TNamed *header = new TNamed("header","");
  file->Add(header);

  stringstream sheader;
  sheader<<endl;
  TVector3 vec_pos(0,0,-4500);//target position

  for (int i=0;i<(int)vec_A.size();++i){
    double Etot = vec_M[i] + vec_E[i]*vec_M[i]/AMU;
    double Ptot = sqrt(Etot*Etot - vec_M[i]*vec_M[i]);
    double Brho = Ptot/(double)vec_Z[i]/c_light;
    sheader<<"i="<<i
	   <<" "<<vec_str[i].c_str()
	   <<" A="<<vec_A[i]
	   <<" Z="<<vec_Z[i]
	   <<" E="<<vec_E[i]<<"MeV/u"
	   <<" Brho="<<Brho<<"Tm"
	   <<endl;

    gBeamSimDataArray->clear();
    TLorentzVector P(0, 0, Ptot, Etot);
    TBeamSimData particle(vec_Z[i],vec_A[i],P,vec_pos);
    gBeamSimDataArray->push_back(particle);
    tree->Fill();
  }

  header->SetTitle(sheader.str().c_str());

  file->Write();
}
