#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TCanvas.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

void AddIncludePath(std::string inc_path);
void LoadModule(std::string module);

void rootlogon()
{

  //----------------------------------------------
  // for load of ANAROOT libraries
  char* c_tart = getenv("TARTSYS");
  if(c_tart){
    std::string install_dir(c_tart);
    AddIncludePath("-I"+install_dir+"/include");
    AddIncludePath("`xml2-config --cflags`");
    LoadModule("libXMLParser.so");
    LoadModule("libMinuit.so");
    LoadModule(install_dir+"/lib/libanaroot.so"); // load at once
  }else{
    std::cout << "set environment variable \"TARTSYS\"" << std::endl;
    std::cout << "quit ROOT" << std::endl;      
    gROOT->ProcessLine(".q");
  }
  //----------------------------------------------
  // for load of simtrace lib
  char* c = getenv("G4SMLIBDIR");
  if(c){
    std::string install_dir(c);
    AddIncludePath("-I"+install_dir+"/include");
    LoadModule(install_dir+"/lib/libsmdata.so");
  }else{
    std::cout << "set environment variable \"G4SMLIBDIR\"" << std::endl;
    std::cout << "quit ROOT" << std::endl;      
    gROOT->ProcessLine(".q");
  }



  //----------------------------------------------
  //Base Style
  gROOT->SetStyle("Modern");

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
  gStyle->SetOptFit(1);
  gStyle->SetOptStat(1111111);
  gStyle->SetPadBorderMode(1);

  Int_t FontNum=132;
  gStyle->SetLabelFont(FontNum,"XYZ");
  gStyle->SetTitleFont(FontNum,"XYZ");
  gStyle->SetTitleFont(FontNum,"");
  gStyle->SetTextFont(FontNum);
  gStyle->SetStatFont(FontNum);

  gStyle->SetCanvasDefW(600);
  gStyle->SetCanvasDefH(600);

  gStyle->SetPaperSize(15,15);

  
}
//____________________________________________________________
void AddIncludePath(std::string inc_path)
{
  gSystem->AddIncludePath(inc_path.c_str());
  std::cout << "add include path : " << inc_path << std::endl;
}
//____________________________________________________________
void LoadModule(std::string module)
{
  std::cout << "reading " << module << std::endl;
  if(gSystem->Load(module.c_str()) < 0){
    std::cout << "cannnot read in " << module << std::endl;      
  }
}
//____________________________________________________________
