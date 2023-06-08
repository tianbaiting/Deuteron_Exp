#include "TNeuLANDSimData.hh"

#include <iostream>
#include <algorithm>
#include <iterator>

ClassImp(TNeuLANDSimData)
TNeuLANDSimDataArray* gNeuLANDSimDataArray = 0;
//____________________________________________________________________
std::ostream& operator<<(std::ostream& out, const TNeuLANDSimData& data)
{
  out << data.fPrimaryParticleID << " ";
  out << "\""<<data.fParticleName.Data() << "\" ";
  out << "\""<<data.fDetectorName.Data() << "\" ";
  out << data.fZ << " ";
  out << data.fA << " ";
  out << data.fPDGCode << " ";
  out << data.fCharge << " ";
  out << data.fPreMomentum.M() << " ";
  out << data.fPreMomentum.Px() << " ";
  out << data.fPreMomentum.Py() << " ";
  out << data.fPreMomentum.Pz() << " ";
  out << data.fPrePosition.X() << " ";
  out << data.fPrePosition.Y() << " ";
  out << data.fPrePosition.Z() << " ";
  out << data.fPreTime << " ";
  out << data.fIsAccepted;
  if(! data.fUserDouble.empty()){
    for(int i=0; i<(int)data.fUserDouble.size(); ++i) out << " " << data.fUserDouble[i];
  }
  if(!data.fUserInt.empty()){
    for(int i=0; i<(int)data.fUserInt.size(); ++i) out << " " << data.fUserInt[i];
  }

  return out;
}
//____________________________________________________________________
TNeuLANDSimData::TNeuLANDSimData() 
  : TObject(),
    fPrimaryParticleID(0),
    fZ(-9999),
    fA(-9999),
    fPDGCode(0),
    fParticleName(""),
    fDetectorName(""),
    fCharge(-9999),
    fMass(-9999),
    fPreMomentum(0,0,0,0),
    fPostMomentum(0,0,0,0),
    fPrePosition(0,0,0),
    fPostPosition(0,0,0),
    fPreTime(0),
    fPostTime(0),
    fIsAccepted(true)
{;}
//____________________________________________________________________
TNeuLANDSimData::~TNeuLANDSimData()
{;}
//____________________________________________________________________
