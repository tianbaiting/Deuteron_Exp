#if defined(TANA)

#include "SAMURAI21FragSimDataConverter.hh"

#include "TanaBeamInfo.hh"
#include "TanaKinematics.hh"

#include "SimDataManager.hh"
#include "TBeamSimData.hh"
#include "TSimData.hh"
#include "TFragSimParameter.hh"

#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"

#include <iostream>

//____________________________________________________________________
SAMURAI21FragSimDataConverter::SAMURAI21FragSimDataConverter(TString name)
  : SimDataConverter(name)
{;}
//____________________________________________________________________
SAMURAI21FragSimDataConverter::~SAMURAI21FragSimDataConverter()
{;}
//____________________________________________________________________
int SAMURAI21FragSimDataConverter::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fFragSimParameter = (TFragSimParameter*)sman->FindParameter("FragParameter");
  if (fFragSimParameter==0){
    std::cout<<"SAMURAI21FragSimDataConveter : FragSimParameter is not found."
	     <<std::endl;
    return 1;
  }


  fFragSimDataArray = sman->FindSimDataArray("FragSimData");
  if (fFragSimDataArray==0){
    std::cout<<"FragSimDataConveter_Basic : FragSimDataArray is not found."
	     <<std::endl;
    return 1;
  }

  ClearBuffer();

  return 0;
}
//____________________________________________________________________
int SAMURAI21FragSimDataConverter::DefineBranch(TTree *tree)
{
  if (fDataStore) {
    tree->Branch("OK_FDC1",&fOK_FDC1,"OK_FDC1/I");
    tree->Branch("fdc1x",&fFDC1X,"fdc1x/D");
    tree->Branch("fdc1y",&fFDC1Y,"fdc1y/D");
    tree->Branch("OK_FDC2",&fOK_FDC2,"OK_FDC2/I");
    tree->Branch("fdc2x",&fFDC2X,"fdc2x/D");
    tree->Branch("fdc2y",&fFDC2Y,"fdc2y/D");
    tree->Branch("fdc2a",&fFDC2A,"fdc2a/D");
    tree->Branch("fdc2b",&fFDC2B,"fdc2b/D");
    tree->Branch("frag_z",&fFragZ,"frag_z/D");
    tree->Branch("frag_aoz",&fFragAoZ,"frag_aoz/D");
    tree->Branch("frag_brho",&fFragBrho,"frag_brho/D");
  }
  return 0;
}
//____________________________________________________________________
int SAMURAI21FragSimDataConverter::ConvertSimData()
{
  Int_t ndata = fFragSimDataArray->GetEntries();


  for (int i=0;i<ndata;++i){
    TSimData* data = (TSimData*)fFragSimDataArray->At(i);

    //-----------------------------------------
    if (data->fDetectorName=="FDC1"){

      TVector3 pos_fdc1 = fFragSimParameter->fFDC1Position;
      TVector3 pos_pre = data->fPrePosition - pos_fdc1;
      TVector3 pos_post = data->fPostPosition - pos_fdc1;
      fFDC1X = 0.5*(pos_pre.x() + pos_post.x());
      fFDC1Y = 0.5*(pos_pre.y() + pos_post.y()); 
      //fFDC1A = (pos_post.x() - pos_pre.x())/(pos_post.z()-pos_pre.z());
      //fFDC1B = (pos_post.y() - pos_pre.y())/(pos_post.z()-pos_pre.z());
      Double_t dz = pos_post.z() - pos_pre.z();
      if (dz>139) fOK_FDC1 = 1; // to avoid aksuri event (dz(FDC1)=140)
    }
    //-----------------------------------------
    else if (data->fDetectorName=="WinC2_Hole"){
      fOK_Window = 1;
    }
    //-----------------------------------------
    else if (data->fDetectorName=="FDC2"){

      TVector3 pos_fdc2 = fFragSimParameter->fFDC2Position;
      Double_t ang_fdc2 = fFragSimParameter->fFDC2Angle/180.*TMath::Pi();//deg

      TVector3 pos_pre = data->fPrePosition;
      TVector3 pos_post = data->fPostPosition;
      pos_fdc2.RotateY(-ang_fdc2);
      pos_pre.RotateY(-ang_fdc2);
      pos_post.RotateY(-ang_fdc2);
      pos_pre -= pos_fdc2;
      pos_post -= pos_fdc2;
      fFDC2X = 0.5*(pos_pre.x() + pos_post.x());
      fFDC2Y = 0.5*(pos_pre.y() + pos_post.y()); 
      fFDC2A = (pos_post.x() - pos_pre.x())/(pos_post.z()-pos_pre.z())*1000.;// mrad
      fFDC2B = (pos_post.y() - pos_pre.y())/(pos_post.z()-pos_pre.z())*1000.;
      fFragZ = data->fZ;
      fFragAoZ = (double)data->fA/data->fZ;

      fFragBrho = TanaKinematics::Mome2Brho(data->fPostMomentum.P(), data->fZ);// temptemptemp

      Double_t dz = pos_post.z() - pos_pre.z();
      if (dz>634) fOK_FDC2 = 1;// to avoid kasuri event (dz(FDC2)=634.64)

//      std::cout<<"fdc2 "<<pos_post.z() - pos_pre.z()<<" "
//      	       <<pos_pre.z()<<" "
//      	       <<pos_post.z()<<" "
//      	       <<fOK_FDC2<<" "
//      	       <<fFragBrho<<" "
//      	       <<std::endl;

    }
    //-----------------------------------------

  }//for (int i=0;i<ndata;++i){

  fOK_FDC2 *= fOK_Window;// AND of WIN,FDC2

  return 0;
}
//____________________________________________________________________
int SAMURAI21FragSimDataConverter::ClearBuffer()
{
  fOK_Window = 0;
  fOK_FDC1 = 0;
  fOK_FDC2 = 0;
  fFDC1X = -9999;
  fFDC1Y = -9999;
  fFDC1A = -9999;
  fFDC1B = -9999;
  fFDC2X = -9999;
  fFDC2Y = -9999;
  fFDC2A = -9999;
  fFDC2B = -9999;
  fFragZ = -9999;
  fFragAoZ = -9999;
  fFragBrho = -9999;
  return 0;
}
//____________________________________________________________________

#endif
