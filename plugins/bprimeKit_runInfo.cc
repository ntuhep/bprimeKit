/*******************************************************************************
*
*  Filename    : bprimeKit_runInfo.cc
*  Description : Extracting the Run Informations
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"
#include <iostream>

using namespace std;

void
bprimeKit::FillRunInfo()
{
   if( fLHERunInfo_H.isValid() ){
      fRunInfo.PdfID = fLHERunInfo_H->heprup().PDFSUP.first;
   } else {
      cout << "Invalid handle!" << endl;
   }
}

void
bprimeKit::GetRunObjects( const edm::Run& iRun, const edm::EventSetup& iSetup )
{
   iRun.getByToken<LHERunInfoProduct>( fLHERunToken, fLHERunInfo_H );
}
