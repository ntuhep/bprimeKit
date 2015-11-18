/**
    Taken directly from TtTreeMakerTool.h and just renamed since it's a small file, just for ease
    @file bpkUtils.h

    @brief Tool for bprimeKit.

*/

#ifndef bpkUtils_h
#define bpkUtils_h

#include <iostream>
#include <TMath.h>

#include "MyAna/bprimeKit/interface/format.h"

namespace {

   double dPhi(double p1,double p2)
   {
      double dp = p1 - p2;
      if      (fabs(dp+TMath::Pi()*2.) < fabs(dp)) dp += TMath::Pi()*2.;
      else if (fabs(dp-TMath::Pi()*2.) < fabs(dp)) dp -= TMath::Pi()*2.;
      
      return fabs(dp);
   }
   
   double dR(double e1, double e2, double p1, double p2)
   {
      return sqrt(pow(e1-e2,2)+pow(dPhi(p1,p2),2));
   }

    template<class T>
    bool
    IndexedQuantityGreaterThan(const std::pair<size_t,T> a, const std::pair<size_t,T> b) {
        return a.second > b.second ;
    }

    template<class T>
    bool
    IndexedQuantityLessThan(const std::pair<size_t,T> a, const std::pair<size_t,T> b){
        return a.second < b.second ;
    }

    template<class T>
    bool
    IndexedQuantityAbsGreaterThan(const std::pair<size_t,T> a, const std::pair<size_t,T> b) {
        return fabs(a.second) > fabs(b.second) ;
    }

    template<class T>
    bool
    IndexedQuantityAbsLessThan(const std::pair<size_t,T> a, const std::pair<size_t,T> b) {
        return fabs(a.second) < fabs(b.second) ;
    }

    //Get index of GenInfo associated with LepInfo/JetInfo index
    template<class T>
    int getGenInfoIndex(const GenInfoBranches &gen, const T &a, const int indexT) {
      int index = -1;
      if(indexT >= a.Size) {
	printf("Error: index %i larger than size %i\n",indexT,a.Size);
	return index;
      }
      for(int igen=0; igen<gen.Size; igen++) {
	if(index > -1) break; //found index
	if(gen.PdgID[igen] != a.GenPdgID[indexT]) continue;
	if((fabs(gen.Pt[igen]-a.GenPt[indexT])/(gen.Pt[igen]+1E-32)) > 1E-3) continue;
	if((fabs(gen.Eta[igen]-a.GenEta[indexT])/(fabs(gen.Eta[igen])+1E-32)) > 1E-3) continue;
	index = igen;
      }
      return index;
    }

    template<class T>
    int originatorPdgID(const GenInfoBranches &gen, const T &a, const int indexT) {
      int igen = getGenInfoIndex<T>(gen,a,indexT);
      int i = igen;
      int id = 99;
      while(i>-1 && gen.nMo[i]==1) {
	i = gen.Mo1[i];
	id = gen.PdgID[i];
      }
      return id;
    }

    //determine whether selected jets originate from ttbar decay (or t'tbar' decay)
    int jetsFromTTbar(const GenInfoBranches &gen, const JetInfoBranches &jet, const std::vector<int> jetIndices) {
      //0 for not from ttbar, +/-1 for from ttbar, +/-2 for t'tbar'
      if(jetIndices.size()==0) {
	printf("empty jet list\n");
	return 0;
      }
      int nFromT = 0, nFromTbar = 0;
      int nFromTp = 0, nFromTbarp = 0;
      for(unsigned i=0; i<jetIndices.size(); i++) {
	int origID = originatorPdgID<JetInfoBranches>(gen,jet,jetIndices[i]);
	if(origID ==  6) nFromT++;
	if(origID == -6) nFromTbar++;
	if(origID ==  8) nFromTp++;
	if(origID == -8) nFromTbarp++;
      }
      if(nFromT  == 1 && nFromTbar  == 3) return -1;
      if(nFromT  == 3 && nFromTbar  == 1) return  1;
      if(nFromTp == 1 && nFromTbarp == 3) return -2;
      if(nFromTp == 3 && nFromTbarp == 1) return  2;
      return 0;
    }

   bool ptOrderJets(const JetInfoBranches &jets, std::vector<int> &jetIdx) {
      if(jetIdx.size()==0) {
         std::cout << "Empty jet list\n";
         return false;
      }
      std::pair<size_t,double> idxPt;
      std::vector< std::pair<size_t,double> > jetsIdxPt;
      for(unsigned j=0; j<jetIdx.size(); j++) {
         idxPt.first = jetIdx[j];
         idxPt.second = jets.Pt[jetIdx[j]];
         jetsIdxPt.push_back(idxPt);
      }
      std::stable_sort(jetsIdxPt.begin(),jetsIdxPt.end(),IndexedQuantityGreaterThan<double>);
      if(jetsIdxPt.size()!=jetIdx.size()) {//just a sanity check
         std::cout << "Don't know how, but we started with " << jetIdx.size() << " jets and ended with " << jetsIdxPt.size() << "jets.\n";
         return false;
      }
      for(unsigned j=0; j<jetsIdxPt.size(); j++) {
         jetIdx[j] = jetsIdxPt[j].first;
      }
      return true;
   }

}

#endif
