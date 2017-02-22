/*******************************************************************************
*
*  Filename    : VertexNtuplizer.hpp
*  Description : Class for handling Vertex ntuplizing
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef BPKFRAMEWORK_BPRIMEKIT_VERTEXNTUPLIZER_HPP
#define BPKFRAMEWORK_BPRIMEKIT_VERTEXNTUPLIZER_HPP

#include "bpkFrameWork/bprimeKit/interface/NtuplizerBase.hpp"
#include "format.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

class VertexNtuplizer : public NtuplizerBase
{
public:
  VertexNtuplizer ( const edm::ParameterSet&, bprimeKit* );
  virtual
  ~VertexNtuplizer ();

  void RegisterTree( TTree* );
  void Analyze( const edm::Event&, const edm::EventSetup& );

private:
  VertexInfoBranches VertexInfo;

  const edm::EDGetToken _vtxtoken;
  const edm::EDGetToken _vtxBStoken;

  edm::Handle<std::vector<reco::Vertex>> _vtxhandle;
  edm::Handle<std::vector<reco::Vertex>> _vtxBShandle;

};

#endif/* end of include guard: BPKFRAMEWORK_BPRIMEKIT_VERTEXNTUPLIZER_HPP */
