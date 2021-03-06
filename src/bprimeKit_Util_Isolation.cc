/*******************************************************************************
*
*  Filename    : Isolations.cc
*  Description : Implementations of more complex isolation variables
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  GetMiniPFIsolation: Reimplementation as public static function
*  https://github.com/manuelfs/CfANtupler/blob/master/minicfa/interface/miniAdHocNTupler.h#L54
*
*******************************************************************************/

#include "bpkFrameWork/bprimeKit/interface/bprimeKit.hpp"

#include "DataFormats/Math/interface/deltaR.h"
#include <algorithm>

using namespace std;

double
bprimeKit::GetMiniPFIso(
   const edm::Handle<pat::PackedCandidateCollection>& pfcands,
   const reco::Candidate*                             ptcl,
   const double&                                      r_iso_min,
   const double&                                      r_iso_max,
   const double&                                      kt_scale,
   const bool&                                        charged_only )
{
   if( ptcl->pt() < 5. ){ return 99999.; }
   double deadcone_nh = 0.;
   double deadcone_ch = 0.;
   double deadcone_ph = 0.;
   double deadcone_pu = 0.;
   if( ptcl->isElectron() ){
      if( fabs( ptcl->eta() ) > 1.479 ){
         deadcone_ch = 0.015;
         deadcone_pu = 0.015;
         deadcone_ph = 0.08;
      }
   } else if( ptcl->isMuon() ){
      deadcone_ch = 0.0001;
      deadcone_pu = 0.01;
      deadcone_ph = 0.01;
      deadcone_nh = 0.01;
   } else {
      // deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01; // maybe use muon cones??
   }

   double iso_nh   = 0.;
   double iso_ch   = 0.;
   double iso_ph   = 0.;
   double iso_pu   = 0.;
   double ptThresh = 0.5;
   if( ptcl->isElectron() ){ptThresh = 0; }
   double r_iso = max( r_iso_min, min( r_iso_max, kt_scale/ptcl->pt() ) );

   for( const pat::PackedCandidate& pfc : * pfcands ){
      if( abs( pfc.pdgId() ) < 7 ){ continue; }

      double dr = deltaR( pfc, *ptcl );
      if( dr > r_iso ){ continue; }

      //////////////////  NEUTRALS  /////////////////////////
      if( pfc.charge() == 0 ){
         if( pfc.pt() > ptThresh ){
            /////////// PHOTONS ////////////
            if( abs( pfc.pdgId() ) == 22 ){
               if( dr < deadcone_ph ){ continue; }
               iso_ph += pfc.pt();
               /////////// NEUTRAL HADRONS ////////////
            } else if( abs( pfc.pdgId() ) == 130 ){
               if( dr < deadcone_nh ){ continue; }
               iso_nh += pfc.pt();
            }
         }
         //////////////////  CHARGED from PV  /////////////////////////
      } else if( pfc.fromPV() > 1 ){
         if( abs( pfc.pdgId() ) == 211 ){
            if( dr < deadcone_ch ){ continue; }
            iso_ch += pfc.pt();
         }

         //////////////////  CHARGED from PU  /////////////////////////
      } else {
         if( pfc.pt() > ptThresh ){
            if( dr < deadcone_pu ){ continue; }
            iso_pu += pfc.pt();
         }
      }
   }

   double iso = 0.;
   if( charged_only ){
      iso = iso_ch;
   } else {
      iso  = iso_ph + iso_nh;
      iso -= 0.5*iso_pu;
      if( iso > 0 ){
         iso += iso_ch;
      } else {
         iso = iso_ch;
      }
   }

   return iso;
}

double 
bprimeKit::GetMiniPFIsoRhoCorr(
   const pat::PFIsolation& iso,
   const double&           pt,
   const double&           rho,
   const double&           effarea )
{
   const double mindr = 0.05;
   const double maxdr = 0.2;
   const double kt_scale = 10.;
   double dr = max( mindr, min( maxdr, kt_scale/pt ) );
   double correction = rho * effarea * (dr/0.3) * (dr/0.3);
   double correctedIso = iso.chargedHadronIso() + max( 0.0, iso.neutralHadronIso() + iso.photonIso() - correction );

   return correctedIso;
}
