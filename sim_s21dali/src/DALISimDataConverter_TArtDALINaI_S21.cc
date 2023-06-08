#include "DALISimDataConverter_TArtDALINaI_S21.hh"

#include "TSimData.hh"
#include "TDALISimParameter.hh"
#include "TDALINaISimParameter.hh"
#include "SimDataManager.hh"
#include "SimDataInitializer.hh"
#include "SimDataConverter.hh"

#include "TArtDALINaI.hh"

#include "TTree.h"
#include "TClonesArray.h"
#include "TRandom.h"

#include <iostream>
//____________________________________________________________________
DALISimDataConverter_TArtDALINaI_S21::DALISimDataConverter_TArtDALINaI_S21(TString name)
  : DALISimDataConverter_TArtDALINaI(name)
{;}
//____________________________________________________________________
DALISimDataConverter_TArtDALINaI_S21::~DALISimDataConverter_TArtDALINaI_S21()
{;}
//____________________________________________________________________
double  DALISimDataConverter_TArtDALINaI_S21::ConvoluteEnergyResolution(double Esim, TDALINaISimParameter* para)
{
  double esim_kev = Esim * 1000.;//MeV -> keV
  double e_kev = 0;
  if ((para->fEnergyResolutionPrm).size()<3){
    std::cout<<__FILE__
	     <<" DALI Resolution file is not loaded, skip resolution convolution"
	     <<std::endl;
    e_kev = esim_kev;
  }else{
    Double_t sigma2fwhm = 2.0*sqrt(2.0*log(2));
    Double_t de = para->fEnergyResolutionPrm[0]*sqrt(esim_kev)
      * para->fEnergyResolutionPrm[1] * exp( para->fEnergyResolutionPrm[2] * esim_kev )
      /sigma2fwhm;

    e_kev = gRandom->Gaus(esim_kev,de);
  }
  return e_kev;
}
//____________________________________________________________________
