#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TCanvas.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

void AddIncludePath(std::string dir);
void AddLinkedLibs();
void LoadModule(std::string dir);

void rootlogon()
{

  // for ANAROOT
  LoadModule("libXMLParser.so");
  char* c = getenv("TARTSYS");
  if(c){
    std::string install_dir(c);
  }else{
    std::cout << "set environment variable \"TARTSYS\"" << std::endl;
    std::cout << "quit ROOT" << std::endl;      
    gROOT->ProcessLine(".q");
  } 

  AddIncludePath(install_dir+"/include");
  LoadModule(install_dir+"/lib/libanaroot.so");

  // for SMSIMULATOR
  char* c2 = getenv("G4SMDATADIR");
  if(c){
    std::string install_dir2(c2);
  }else{
    std::cout << "set environment variable \"G4SMDATADIR\"" << std::endl;
    std::cout << "quit ROOT" << std::endl;      
    gROOT->ProcessLine(".q");
  } 

  AddIncludePath(install_dir2+"/include");
  LoadModule(install_dir2+"/lib/libsmdata.so");
  //LoadModule(install_dir2+"/lib/libsmana.so");

  //Base Style
  //  gROOT->SetStyle("Plain");
  gROOT->SetStyle("Modern");
  //  gROOT->SetStyle("Classic");

  //Force Style
  gStyle->SetHistFillColor(7);
  gStyle->SetHistFillStyle(3002);
  gStyle->SetHistLineColor(kBlue);
  gStyle->SetFuncColor(kRed);
  gStyle->SetFrameLineWidth(2);
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetStatColor(0);
  gStyle->SetStatStyle(0);
  gStyle->SetStatX(0.9);  
  gStyle->SetStatY(0.9);  
  gStyle->SetPalette(1);
  gStyle->SetOptLogz(1);
  //  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(1);
  gStyle->SetOptStat(1111111);
  //gStyle->SetOptStat(0);
  gStyle->SetPadBorderMode(1);
  gStyle->SetPaperSize(20,19);
  //  gStyle->SetOptDate(1);

  gStyle->SetLabelFont(132,"XYZ");
  gStyle->SetTitleFont(132,"XYZ");
  gStyle->SetTitleFont(132,"");
  gStyle->SetTextFont(132);
  gStyle->SetStatFont(132);
  
}

void AddIncludePath(std::string install_dir)
{
  std::vector<std::string> include;
  include.push_back("-I"+install_dir);

  std::vector<std::string>::iterator it = include.begin();
  while(it != include.end()){
    gSystem->AddIncludePath((*it).c_str());
    std::cout << "add include path : " << *it << std::endl;
    ++it;
  }
}

void AddLinkedLibs()
{
}

void LoadModule(std::string install_libfile)
{
  std::vector<std::string> modules;
  modules.push_back(install_libfile); // load at once
  
  std::vector<std::string>::iterator it = modules.begin();
  while(it != modules.end()){
    std::cout << "reading " << *it << std::endl;
    if(gSystem->Load((*it).c_str()) < 0){
      std::cout << "cannnot read in " << *it << std::endl;      
    }
    ++it;
  }
}

