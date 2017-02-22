/*******************************************************************************
*
*  Filename    : bprimeKit_runInfo.cc
*  Description : Extracting the Run Informations
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.hpp"

#include <iostream>

using namespace std;

void
bprimeKit::FillRunInfo()
{
  if( _runinfohandle.isValid() ){
    RunInfo.PdfID = _runinfohandle->heprup().PDFSUP.first;
  } else {
    cerr << "Invalid handle!" << endl;
  }
}

void
bprimeKit::GetRunObjects( const edm::Run& iRun, const edm::EventSetup& iSetup )
{
  iRun.getByToken<LHERunInfoProduct>( _lheruntoken, _runinfohandle );
}
