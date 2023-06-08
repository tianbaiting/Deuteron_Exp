#ifndef _DALISIMDATACONVERTER_TARTDALINAI_S21_HH_
#define _DALISIMDATACONVERTER_TARTDALINAI_S21_HH_

#include "DALISimDataConverter_TArtDALINaI.hh"
#include "TDALISimParameter.hh"

class DALISimDataConverter_TArtDALINaI_S21 : public DALISimDataConverter_TArtDALINaI
{
public:
  DALISimDataConverter_TArtDALINaI_S21(TString name="DALISimDataConverter_TArtDALINaI_S21");
  virtual ~DALISimDataConverter_TArtDALINaI_S21();

  virtual double ConvoluteEnergyResolution(double Esim, TDALINaISimParameter* para);

};
#endif
