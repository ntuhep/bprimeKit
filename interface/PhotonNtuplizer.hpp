/*******************************************************************************
*
*  Filename    : PhotonNtuplizer.hpp
*  Description : Class for handling objects required to ntuplize the photon
*                collection
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef BPKFRAMEWORK_BPRIMEKIT_PHOTONNTUPLIZER_HPP
#define BPKFRAMEWORK_BPRIMEKIT_PHOTONNTUPLIZER_HPP
#include "bpkFrameWork/bprimeKit/interface/NtuplizerBase.hpp"

class PhotonNtuplizer : public NtuplizerBase
{

public:
  PhotonNtuplizer ( const edm::ParameterSet&, bprimeKit );
  virtual
  ~PhotonNtuplizer ();

private:

};

#endif/* end of include guard: BPKFRAMEWORK_BPRIMEKIT_PHOTONNTUPLIZER_HPP */
