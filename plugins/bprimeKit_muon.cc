/*******************************************************************************
 *
 *  Filename    : bprimeKit_muon.cc
 *  Description : Filling in the muon informations
 *
*******************************************************************************/


#include "MyAna/bprimeKit/interface/bprimeKit.h"
#include "MyAna/bprimeKit/interface/bprimeKit_util.h"

//-------------------------  Muon specific CMSSW libraries  -------------------------
#include "UserCode/sixie/Muon/MuonAnalysisTools/interface/MuonEffectiveArea.h"


//------------------------------------------------------------------------------ 
//   Helper static variables and functions
//------------------------------------------------------------------------------ 
extern MuonHandlerList MuonHandle;
static MuonIterator    it_mu;
static GenIterator     it_gen;

//------------------------------------------------------------------------------ 
//   Begin bprimeKit muon method implementaion
//------------------------------------------------------------------------------ 
bool bprimeKit::fillMuon( const edm::Event& iEvent , const edm::EventSetup& iSetup , const size_t icoll )
{

   if( MuonHandle.size() <= icoll ) { return false; } 
   
   if( debug_ > 10 ) { cout << " Muon collection size " << MuonHandle[icoll]->size() << endl; }

   for( it_mu = MuonHandle[icoll]->begin(); it_mu != MuonHandle[icoll]->end(); it_mu++ ) { //loop over muons in collection
      if ( LepInfo[icoll].Size >= MAX_LEPTONS ) {
         cerr << "ERROR: number of leptons exceeds the size of array." << endl;
         break;//exit(0);
      }

      if( debug_ > 11 ) { cout << "  Size " << LepInfo[icoll].Size << " mu pt,eta,phi " << it_mu->pt() << "," << it_mu->eta() << "," << it_mu->phi() << endl; }

      LepInfo[icoll].Index      [LepInfo[icoll].Size] = LepInfo[icoll].Size;
      LepInfo[icoll].LeptonType [LepInfo[icoll].Size] = 13;
      LepInfo[icoll].Charge     [LepInfo[icoll].Size] = it_mu->charge();
      LepInfo[icoll].Pt         [LepInfo[icoll].Size] = it_mu->pt();
      LepInfo[icoll].Eta        [LepInfo[icoll].Size] = it_mu->eta();
      LepInfo[icoll].Phi        [LepInfo[icoll].Size] = it_mu->phi();
      LepInfo[icoll].TrackIso   [LepInfo[icoll].Size] = it_mu->trackIso();
      LepInfo[icoll].EcalIso    [LepInfo[icoll].Size] = it_mu->ecalIso();
      LepInfo[icoll].HcalIso    [LepInfo[icoll].Size] = it_mu->hcalIso();
      LepInfo[icoll].Px         [LepInfo[icoll].Size] = it_mu->px(); //Uly 2011-04-04
      LepInfo[icoll].Py         [LepInfo[icoll].Size] = it_mu->py(); //Uly 2011-04-04
      LepInfo[icoll].Pz         [LepInfo[icoll].Size] = it_mu->pz(); //Uly 2011-04-04
      LepInfo[icoll].Energy     [LepInfo[icoll].Size] = it_mu->energy(); //Uly 2011-04-04

      // For Soft Muon selection (https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Soft_Muon)
      if( muon::isGoodMuon( *it_mu, muon::TMOneStationTight ) ) {
         LepInfo[icoll].isGoodMuonTMOneStationTight    [LepInfo[icoll].Size] = true;
      } else {
         LepInfo[icoll].isGoodMuonTMOneStationTight    [LepInfo[icoll].Size] = false;
      }

      //std::cout<<"Muon IP : "<<fabs(it_mu->dB(pat::Muon::PV3D))<<std::endl;
      //std::cout<<"Muon IPError : "<<fabs(it_mu->edB(pat::Muon::PV3D))<<std::endl;
      //std::cout<<"Muon Ip3d : "<<fabs(it_mu->dB(pat::Muon::PV3D))/fabs(it_mu->edB(pat::Muon::PV3D))<<std::endl;
      // Initialize these three variables in case the muon has no track.
      LepInfo[icoll].Ip3dPV[LepInfo[icoll].Size]             = -10000;
      LepInfo[icoll].Ip3dPVErr[LepInfo[icoll].Size]          = -10000;
      LepInfo[icoll].Ip3dPVSignificance[LepInfo[icoll].Size] = -10000;

      //PFIso
      LepInfo[icoll].isPFMuon          [LepInfo[icoll].Size] = it_mu->isPFMuon();
      LepInfo[icoll].ChargedHadronIso  [LepInfo[icoll].Size] = it_mu->chargedHadronIso();
      LepInfo[icoll].NeutralHadronIso  [LepInfo[icoll].Size] = it_mu->neutralHadronIso();
      LepInfo[icoll].PhotonIso         [LepInfo[icoll].Size] = it_mu->photonIso();
      /*
         There are two ways to correct PFIsol by using DeltaBeta or rho correction.
         1). DeltaBeta : I = [sumChargedHadronPt+ max(0.,sumNeutralHadronPt+sumPhotonPt-0.5sumPUPt]/pt
         2). Rho correction in page9 of
             https://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=188494
        Effective area :
                http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/sixie/Muon/MuonAnalysisTools/interface/MuonEffectiveArea.h?revision=1.7&view=markup
         */
      if ( it_mu->isPFMuon() && it_mu->isPFIsolationValid() ) {
         LepInfo[icoll].ChargedHadronIsoR03  [LepInfo[icoll].Size] = it_mu->pfIsolationR03().sumChargedHadronPt;
         LepInfo[icoll].NeutralHadronIsoR03  [LepInfo[icoll].Size] = it_mu->pfIsolationR03().sumNeutralHadronEt;
         LepInfo[icoll].PhotonIsoR03         [LepInfo[icoll].Size] = it_mu->pfIsolationR03().sumPhotonEt;
         LepInfo[icoll].sumPUPtR03           [LepInfo[icoll].Size] = it_mu->pfIsolationR03().sumPUPt;
         MuonEffectiveArea::MuonEffectiveAreaTarget EATarget = MuonEffectiveArea::kMuEAFall11MC;
         if( isData ) { EATarget = MuonEffectiveArea::kMuEAData2012; }
         float AEffR03 = MuonEffectiveArea::GetMuonEffectiveArea( MuonEffectiveArea::kMuGammaAndNeutralHadronIso03, LepInfo[icoll].Eta[LepInfo[icoll].Size], EATarget );
         double rhoPrime = 0;

         if( !TurnOffInCMSSW73x )
         rhoPrime = max( ( double ) * ( rhoH[1].product() ), 0.0 );
         
         LepInfo[icoll].IsoRhoCorrR03             [LepInfo[icoll].Size] =
            LepInfo[icoll].ChargedHadronIsoR03[LepInfo[icoll].Size] +
            max( LepInfo[icoll].NeutralHadronIsoR03[LepInfo[icoll].Size]
                 + LepInfo[icoll].PhotonIsoR03[LepInfo[icoll].Size] - rhoPrime * AEffR03, 0.0 );
         LepInfo[icoll].ChargedHadronIsoR04  [LepInfo[icoll].Size] = it_mu->pfIsolationR04().sumChargedHadronPt;
         LepInfo[icoll].NeutralHadronIsoR04  [LepInfo[icoll].Size] = it_mu->pfIsolationR04().sumNeutralHadronEt;
         LepInfo[icoll].PhotonIsoR04         [LepInfo[icoll].Size] = it_mu->pfIsolationR04().sumPhotonEt;
         LepInfo[icoll].sumPUPtR04           [LepInfo[icoll].Size] = it_mu->pfIsolationR04().sumPUPt;
         float AEffR04 = MuonEffectiveArea::GetMuonEffectiveArea( MuonEffectiveArea::kMuGammaAndNeutralHadronIso04, LepInfo[icoll].Eta[LepInfo[icoll].Size], EATarget );
         LepInfo[icoll].IsoRhoCorrR04             [LepInfo[icoll].Size] =
            LepInfo[icoll].ChargedHadronIsoR04[LepInfo[icoll].Size] +
            max( LepInfo[icoll].NeutralHadronIsoR04[LepInfo[icoll].Size]
                 + LepInfo[icoll].PhotonIsoR04[LepInfo[icoll].Size] - rhoPrime * AEffR04, 0.0 );
      }

      // Timing information for distinguishing cosmic ray and prompt muon
      if( it_mu->isTimeValid() ) {
         // number of muon stations used
         LepInfo[icoll].MuontimenDof             [LepInfo[icoll].Size] = it_mu->time().nDof;
         // time of arrival at the IP for the Beta=1 hypothesis
         // a) particle is moving from inside out
         LepInfo[icoll].MuontimeAtIpInOut        [LepInfo[icoll].Size] = it_mu->time().timeAtIpInOut;
         // b) particle is moving from outside in
         LepInfo[icoll].MuontimeAtIpOutIn        [LepInfo[icoll].Size] = it_mu->time().timeAtIpOutIn;
         // enum Direction { OutsideIn = -1, Undefined = 0, InsideOut = 1 };
         LepInfo[icoll].Muondirection            [LepInfo[icoll].Size] = it_mu->time().direction();
      }

      reco::MuonEnergy muEnergy = it_mu->calEnergy();
      LepInfo[icoll].CaloEnergy [LepInfo[icoll].Size] = muEnergy.em + muEnergy.had + muEnergy.ho;

      LepInfo[icoll].MuIDGlobalMuonPromptTight         [LepInfo[icoll].Size] = it_mu->muonID( "GlobalMuonPromptTight" );

      // InnerTrack() is only valid for GlobalMuon and TrackerMuon
      // See LepInfo[icoll].MuType for it_mu->type() bits
      if ( ( it_mu->type() & 0x02 ) || ( it_mu->type() & 0x04 ) ) {
         LepInfo[icoll].innerTracknormalizedChi2 [LepInfo[icoll].Size] = it_mu->innerTrack()->normalizedChi2();
         LepInfo[icoll].MuInnerPtError         [LepInfo[icoll].Size] = it_mu->innerTrack()->ptError();
         LepInfo[icoll].MuInnerTrackDz         [LepInfo[icoll].Size] = it_mu->innerTrack()->dz( PrimVtx.position() );
         LepInfo[icoll].MuInnerTrackD0         [LepInfo[icoll].Size] = it_mu->innerTrack()->d0();
         LepInfo[icoll].MuInnerTrackDxy_BS     [LepInfo[icoll].Size] = it_mu->innerTrack()->dxy( beamSpot.position() );
         LepInfo[icoll].MuInnerTrackDxy_PV     [LepInfo[icoll].Size] = it_mu->innerTrack()->dxy( PrimVtx.position() );
         LepInfo[icoll].MuInnerTrackDxy_PVBS   [LepInfo[icoll].Size] = it_mu->innerTrack()->dxy( PrimVtx_BS.position() );
         LepInfo[icoll].MuInnerTrackNHits      [LepInfo[icoll].Size] = it_mu->innerTrack()->numberOfValidHits();
         LepInfo[icoll].MuNTrackerHits         [LepInfo[icoll].Size] = it_mu->innerTrack()->hitPattern().numberOfValidTrackerHits();
         LepInfo[icoll].MuNPixelLayers         [LepInfo[icoll].Size] = it_mu->innerTrack()->hitPattern().numberOfValidPixelHits();
         //LepInfo[icoll].MuNLostInnerHits       [LepInfo[icoll].Size] = it_mu->innerTrack()->hitPattern().numberOfLostHits();
         // not valid (https://cmssdt.cern.ch/SDT/lxr/source//DataFormats/TrackReco/interface/HitPattern.h)
         LepInfo[icoll].MuNLostInnerHits       [LepInfo[icoll].Size] = -1;
         LepInfo[icoll].vertexZ                [LepInfo[icoll].Size] = it_mu->vertex().z(); //Uly 2011-04-04
         LepInfo[icoll].MuNPixelLayersWMeasurement[LepInfo[icoll].Size] = it_mu->innerTrack()->hitPattern().pixelLayersWithMeasurement(); //Uly 2011-04-04
         LepInfo[icoll].MuNTrackLayersWMeasurement[LepInfo[icoll].Size] = it_mu->innerTrack()->hitPattern().trackerLayersWithMeasurement();

         // Reference from UserCode/MitProd/TreeFiller/src/FillerMuons.cc
         if( TurnOnInCMSSW_7_4_1 ) {
         //   const reco::TransientTrack& tt_mu = transientTrackBuilder->build( it_mu->track() );
         //   reco::Vertex thevtx = pvCol->at( 0 );
         //   const std::pair<bool, Measurement1D>& ip3dpv =  IPTools::absoluteImpactParameter3D( tt_mu, thevtx );
         //   const double thesign   = ( ( -it_mu->track()->dxy( thevtx.position() ) )   >= 0 ) ? 1. : -1.;
         //   //std::cout<<"Muon Ip3dPVSignificance : "<<thesign*ip3dpv.second.value()/ip3dpv.second.error() <<std::endl;
         //   LepInfo[icoll].Ip3dPV[LepInfo[icoll].Size] = thesign * ip3dpv.second.value();
         //   LepInfo[icoll].Ip3dPVErr[LepInfo[icoll].Size] = ip3dpv.second.error();
         //   LepInfo[icoll].Ip3dPVSignificance[LepInfo[icoll].Size] = thesign * ip3dpv.second.value() / ip3dpv.second.error();
         }
      }

      if ( it_mu->type() & 0x02 ) {
         LepInfo[icoll].MuGlobalPtError        [LepInfo[icoll].Size] = it_mu->globalTrack()->ptError();
         LepInfo[icoll].MuGlobalNormalizedChi2 [LepInfo[icoll].Size] = it_mu->globalTrack()->normalizedChi2();
         LepInfo[icoll].MuNMuonhits            [LepInfo[icoll].Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonHits();
         LepInfo[icoll].MuDThits               [LepInfo[icoll].Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonDTHits();
         LepInfo[icoll].MuCSChits              [LepInfo[icoll].Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonCSCHits();
         LepInfo[icoll].MuRPChits              [LepInfo[icoll].Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonRPCHits();
      }
      if ( ( it_mu->type() & 0x02 ) || ( it_mu->type() & 0x08 ) ) {
         //LepInfo[icoll].MuNLostOuterHits       [LepInfo[icoll].Size] = it_mu->outerTrack()->hitPattern().numberOfLostHits();
         // not valid (https://cmssdt.cern.ch/SDT/lxr/source//DataFormats/TrackReco/interface/HitPattern.h)
         LepInfo[icoll].MuNLostOuterHits       [LepInfo[icoll].Size] = -1;
      }

      LepInfo[icoll].MuCaloCompat              [LepInfo[icoll].Size] = it_mu->caloCompatibility();
      LepInfo[icoll].MuNChambers               [LepInfo[icoll].Size] = it_mu->numberOfChambers();
      LepInfo[icoll].MuNChambersMatchesSegment [LepInfo[icoll].Size] = it_mu->numberOfMatches();  // At least 2 Chambers matched with segments
      LepInfo[icoll].MuNMatchedStations        [LepInfo[icoll].Size] = it_mu->numberOfMatchedStations();

      // Muon type bits:
      // static const unsigned int GlobalMuon     =  1<<1 (0x02);
      // static const unsigned int TrackerMuon    =  1<<2 (0x04);
      // static const unsigned int StandAloneMuon =  1<<3 (0x08);
      // static const unsigned int CaloMuon       =  1<<4 (0x10);
      // static const unsigned int PFMuon         =  1<<5 (0x20);
      LepInfo[icoll].MuType[LepInfo[icoll].Size] = it_mu->type();

      if ( !isData && !skipGenInfo_ ) {
         const reco::GenParticle* gen = it_mu->genLepton();
         if ( gen != NULL ) {
            LepInfo[icoll].GenPt    [LepInfo[icoll].Size] = gen->pt();
            LepInfo[icoll].GenEta   [LepInfo[icoll].Size] = gen->eta();
            LepInfo[icoll].GenPhi   [LepInfo[icoll].Size] = gen->phi();
            LepInfo[icoll].GenPdgID [LepInfo[icoll].Size] = gen->pdgId();

            LepInfo[icoll].GenMCTag[LepInfo[icoll].Size] = getGenMCTag( gen ) ;
         }
      }

      if ( LepInfo[icoll].GenMCTag[LepInfo[icoll].Size] == 0 && !isData && !skipGenInfo_ ) {
         for( it_gen = GenHandle->begin(); it_gen != GenHandle->end(); it_gen++ ) {
            if( LepInfo[icoll].GenMCTag[LepInfo[icoll].Size] != 0 ) break;
            LepInfo[icoll].GenMCTag[LepInfo[icoll].Size] = getGenMCTag( it_gen , it_mu )  ; 
         }
      }
      LepInfo[icoll].CandRef [LepInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_mu );
      LepInfo[icoll].Size++;
   }
   return true;
}


int getGenMCTag( int pdgId ) 
{
   if     ( abs( pdgId )                    == 23 ) { return 2; }
   else if( abs( pdgId )                    == 24 ) { return 1; }
   else if( abs( pdgId )                    == 5  ) { return 3; } // from a b quark
   else if( ( abs( pdgId ) % 1000 ) / 100   == 5  ) { return 3; } // from a B meson
   else if( ( abs( pdgId ) % 10000 ) / 1000 == 5  ) { return 3; } // from a bottom baryon
   else if( abs( pdgId )                    == 4  ) { return 4; } // from a c quark
   else if( ( abs( pdgId ) % 1000 ) / 100   == 4  ) { return 4; } // from a D meson
   else if( ( abs( pdgId ) % 10000 ) / 1000 == 4  ) { return 4; } // from a charm baryon
   else if( abs( pdgId )                    == 15 ) { return 7; } // from tau
   return 0;
}
