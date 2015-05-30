// -----------------------------------------------------
// bprimeKit.cc -- b' analysis kit
// -----------------------------------------------------
// For CMSSW_3_6_1
// Lastest updates:
// Jul 05, 2010 - Add Electron ID by Jacky, Add PFMet by Chiyi
// May 19, 2010 - Add Muon Iso variables, MCTag, and PF Jet  - by Dmitry Hits & Yeong-jyi Kei
// Apr 15, 2010 - Update to CMSSW_3_5_6, fix bTag bug  - by Yeong-jyi Lei
// Sep 24, 2009 - Updates label name, and triggerbook. Add PhotonInfo - by Yeong-jyi Lei
// Sep 11, 2009 - Updates with CMSSW_3_1_2 - by Kai-Feng Chen
// May 29, 2009 - Include GenInfo, electron classification, bug fix on vtx - by Yeong-jyi Lei
// Mar 10, 2009 - Change the trigger table
// Feb 20, 2009 - Protection added for missing InnerTrack() from muons, adding MC top mass, bug fix on JetInfo.
// Jan 03, 2009 - Updates according to CMSSW_2_2_3 release (clean 2_2_3 + PhysicsTools/PatAlgos V04-14-15)

#include "MyAna/bprimeKit/interface/bprimeKit.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <map>

#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"

#include "DataFormats/Math/interface/angle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

//For electron convertion flag
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/TrackReco/interface/Track.h"
//#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCoreFwd.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PileUpPFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PileUpPFCandidateFwd.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"


// For JEC
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
//#include "JetMETCorrections/Objects/interface/JetCorrector.h"

// PileupSummaryInfo

#include <string>

//
// DM: Addiign header for IVF and double b tagging
//
#include "RecoBTag/SecondaryVertex/interface/SecondaryVertex.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"

#include "QuarkGluonTagger/EightTeV/interface/QGTagger.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

// Including tool from ggNTuplizer for isolation calculation 


#define W_MASS          80.403
#define Z_MASS          91.1876
#define MUON_MASS       0.105658
#define ELECTRON_MASS   0.0005109989
// uncomment the following line for filling the di-jet pairs
//#define FILL_DIJET_PAIRS 1

using namespace edm;
using namespace reco;
using namespace std;
using namespace pat;
using namespace math;
using namespace ROOT;


bprimeKit::bprimeKit( const edm::ParameterSet& iConfig )
{
   edm::Service<TFileService> fs;
   TFileDirectory results = TFileDirectory( fs->mkdir( "results" ) );

   //MCtag                = iConfig.getUntrackedParameter<bool>("MCtag",true);
   muonlabel_           = iConfig.getParameter<std::vector<InputTag> >( "muonlabel" ); //"cleanPatMuons"
   eleclabel_           = iConfig.getParameter<std::vector<InputTag> >( "eleclabel" ); // "cleanPatElectrons"
   taulabel_            = iConfig.getParameter<std::vector<InputTag> >( "taulabel" ); // "selectedPatTausPFlow"
   pholabel_            = iConfig.getParameter<std::vector<InputTag> >( "pholabel" );
   // jetlabel_            = iConfig.getParameter<std::vector<InputTag> >( "jetlabel" ); // "cleanPatJets"
   metlabel_            = iConfig.getParameter<std::vector<InputTag> >( "metlabel" ); //"patMETs"
   pfmetlabel_          = iConfig.getParameter<std::vector<InputTag> >( "pfmetlabel" ); //"pfpatMETs"
   genlabel_            = iConfig.getParameter<std::vector<InputTag> >( "genlabel" ); // "genParticles"
   hltlabel_            = iConfig.getParameter<std::vector<InputTag> >( "hltlabel" ); // "TriggerResults::HLT"
   pathltlabel_         = iConfig.getParameter<std::vector<InputTag> >( "pathltlabel" ); // patTriggerEvent
   offlinePVlabel_      = iConfig.getParameter<std::vector<InputTag> >( "offlinePVlabel" ); //offlinePrimaryVertices
   offlinePVBSlabel_    = iConfig.getParameter<std::vector<InputTag> >( "offlinePVBSlabel" ); //offlinePrimaryVerticesWithBS
   offlineBSlabel_      = iConfig.getParameter<std::vector<InputTag> >( "offlineBSlabel" ); //offlineBeamSpot
   tracklabel_          = iConfig.getParameter<std::vector<InputTag> >( "tracklabel" ); //generalTracks
   dcslabel_            = iConfig.getParameter<std::vector<InputTag> >( "dcslabel" ); //scalersRawToDigi
   genevtlabel_         = iConfig.getParameter<std::vector<InputTag> >( "genevtlabel" ); //generator
   gtdigilabel_         = iConfig.getParameter<std::vector<InputTag> >( "gtdigilabel" ); //gtDigis
   rhocorrectionlabel_  = iConfig.getParameter<std::vector<InputTag> >( "rhocorrectionlabel" ); // For PU correction
   sigmaLabel_          = iConfig.getParameter<std::vector<InputTag> >( "sigmaLabel" ); // For PU correction
   puInfoLabel_         = iConfig.getParameter<std::vector<InputTag> >( "puInfoLabel" );

   pfToken_             = ( consumes<pat::PackedCandidateCollection>( iConfig.getParameter<edm::InputTag>( "pfCands" ) ) );

   // Add 2012 EID simple-cut-based
   conversionsInputTag_    = iConfig.getParameter<edm::InputTag>( "conversionsInputTag" );
   rhoIsoInputTag          = iConfig.getParameter<edm::InputTag>( "rhoIsoInputTag" );
   isoValInputTags_        = iConfig.getParameter<std::vector<edm::InputTag> >( "isoValInputTags" );
   EIDMVAInputTags_        = iConfig.getParameter<std::vector<std::string> >( "EIDMVAInputTags" );

   lepcollections_      = iConfig.getParameter<std::vector<std::string> >( "LepCollections" ); //branch names
   phocollections_      = iConfig.getParameter<std::vector<std::string> >( "PhoCollections" ); //branch names
   jetcollections_      = iConfig.getParameter<std::vector<std::string> >( "JetCollections" ); //branch names
   jettype_             = iConfig.getParameter<std::vector<int> >( "JetType" );

   pairColl_            = iConfig.getUntrackedParameter<int>( "PairCollection", 0 );
   getElectronID_       = iConfig.getUntrackedParameter<bool>( "ElectronID", true );
   skipGenInfo_         = iConfig.getUntrackedParameter<bool>( "SkipGenInfo", false );
   includeL7_           = iConfig.getUntrackedParameter<bool>( "IncludeL7", true );

   SelectionParameters_ = iConfig.getParameter<edm::ParameterSet>( "SelectionParameters" );

   // update for CMSSW_7_2_0
   reducedEBRecHitCollectionToken_ = consumes<EcalRecHitCollection>( iConfig.getParameter<edm::InputTag>( "reducedEBRecHitCollection" ) );
   reducedEERecHitCollectionToken_ = consumes<EcalRecHitCollection>( iConfig.getParameter<edm::InputTag>( "reducedEERecHitCollection" ) );
   // update for CMSSW_7_3_1
   // reducedEBRecHitCollectionToken_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedEgamma","reducedEBRecHits"));
   // reducedEERecHitCollectionToken_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedEgamma","reducedEERecHits"));
   
   debug_ = iConfig.getUntrackedParameter<int>( "Debug", 0 );

   isolatorR03.initializeElectronIsolation( kTRUE ); //NOTE: this automatically set all the correct defaul veto values
   isolatorR04.initializeElectronIsolation( kTRUE ); //NOTE: this automatically set all the correct defaul veto values
   isolatorR03.setConeSize( 0.3 );
   isolatorR04.setConeSize( 0.4 );

   PhotonisolatorR03.initializePhotonIsolation( kTRUE ); //NOTE: this automatically set all the correct defaul veto values
   PhotonisolatorR04.initializePhotonIsolation( kTRUE ); //NOTE: this automatically set all the correct defaul veto values
   PhotonisolatorR03.setConeSize( 0.3 );
   PhotonisolatorR04.setConeSize( 0.4 );
   // NOTE: It is safer and crab-compliant to get the files locally, i.e in EgammaAnalysis/ElectronTools/data
   // (see the downloard.url file in that directory)
   // Alternatively (for tests), they can be read from AFS:
   //*
   myMVANonTrig = new EGammaMvaEleEstimator();
   std::vector<std::string> myManualCatWeigths;
   if( EIDMVAInputTags_.size() != 12 ) { cout << "EIDMVAInputTags array size (12) is not correct" << endl; }
   for( int ie = 0; ie < 6; ie++ ) { myManualCatWeigths.push_back( EIDMVAInputTags_[ie].c_str() ); }

   Bool_t manualCat = true;

   myMVANonTrig->initialize( "BDT",
                             EGammaMvaEleEstimator::kNonTrig,
                             manualCat,
                             myManualCatWeigths );

   // NOTE: it is better if you copy the MVA weight files locally. See the previous remark
   std::vector<std::string> myManualCatWeigthsTrig;
   for( int ie = 0; ie < 6; ie++ ) { myManualCatWeigthsTrig.push_back( EIDMVAInputTags_[ie + 6].c_str() ); }

   myMVATrig = new EGammaMvaEleEstimator();
   myMVATrig->initialize( "BDT",
                          EGammaMvaEleEstimator::kTrig,
                          manualCat,
                          myManualCatWeigthsTrig );

   for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ) { HLTmaplist.insert( pair< std::string, int > ( TriggerBooking[i], i ) ); }

}


bprimeKit::~bprimeKit()
{
}

void bprimeKit::beginJob()
{
   root = new TTree( "root", "root" );
   EvtInfo.RegisterTree( root );
   if( !skipGenInfo_ ) { GenInfo.RegisterTree( root ); }
   if( lepcollections_.size() > MAX_LEPCOLLECTIONS ) { cout << "WARNING: Too many lep collections, using first " << MAX_LEPCOLLECTIONS << endl; }
   for( unsigned i = 0; i < lepcollections_.size(); i++ ) {
      if( i >= MAX_LEPCOLLECTIONS ) { break; }
      LepInfo[i].RegisterTree( root, lepcollections_[i] );
   }
   for( unsigned i = 0; i < phocollections_.size(); i++ ) {
      if( i >= MAX_PHOCOLLECTIONS ) { break; }
      PhotonInfo[i].RegisterTree( root, phocollections_[i] );
   }
   VertexInfo.RegisterTree( root );
   if( jetcollections_.size() > MAX_JETCOLLECTIONS ) { cout << "WARNING: Too many jet collections, using first " << MAX_JETCOLLECTIONS << endl; }
   for( unsigned i = 0; i < jetcollections_.size(); i++ ) {
      if( i >= MAX_JETCOLLECTIONS ) { break; }
      JetInfo[i].RegisterTree( root, jetcollections_[i] );
   }
   if( pairColl_ >= 0 ) { PairInfo.RegisterTree( root ); }

}

void bprimeKit::endJob()
{
}


//-----------------  Method called when starting to process a run  ------------------
void bprimeKit::beginRun
( edm::Run const& iRun, edm::EventSetup const& iSetup )
{
   std::string processName_ = "HLT";
   bool changed( false );
   hltConfig_.init( iRun, iSetup, processName_, changed );
}

//-----------------------------  When ending a process  -----------------------------
void bprimeKit::endRun
( edm::Run const&, edm::EventSetup const& )
{
}


void bprimeKit::analyze
( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   edm::ESHandle<ParticleDataTable> pdt_;
   iSetup.getData( pdt_ );
   if( debug_ > 0 ) { cout << "Begin Analyze" << endl; }
   isData = iEvent.isRealData();   // Add by Jacky

   edm::Handle<std::vector<pat::MET> >          METHandle;
   edm::Handle<std::vector<pat::MET> >          pfMETHandle;


   //edm::Handle<reco::VertexCollection>          VertexHandlePixel; //Dmitry
   edm::Handle<edm::View<reco::Track> >         TrackHandle; //Dmitry

   std::vector<edm::Handle<double> > sigmaHandle;

   if( !TurnOffInCMSSW73x )
      for( unsigned il = 0; il < rhocorrectionlabel_.size(); il++ ) {
         rhoH.push_back( edm::Handle<double> () );
         iEvent.getByLabel( rhocorrectionlabel_[il], rhoH[il] );
         sigmaHandle.push_back( edm::Handle<double> () );
         iEvent.getByLabel( sigmaLabel_[il], sigmaHandle[il] );
      }



   //  if(metlabel_.size() > 0) iEvent.getByLabel( metlabel_[0],    METHandle);
   if( pfmetlabel_.size() > 0 ) { iEvent.getByLabel( pfmetlabel_[0],  pfMETHandle ); }

   if( !isData && genlabel_.size() > 0 ) { iEvent.getByLabel( genlabel_[0], GenHandle ); }

   if( offlinePVlabel_.size() > 0 ) { iEvent.getByLabel( offlinePVlabel_[0], VertexHandle ); }
   //const reco::VertexCollection* pvCol = VertexHandle.product();

   if( offlinePVBSlabel_.size() > 0 ) { iEvent.getByLabel( offlinePVBSlabel_[0], VertexHandleBS ); } //Offline primary vertices with Beam Spot constraint //Dmitry
   if( !TurnOffInCMSSW73x )
      if( tracklabel_.size() > 0 ) { iEvent.getByLabel( tracklabel_[0], TrackHandle ); }         //get tracks for calculating dRmin (Dmitry)

   printf( "[Test] tracklabel is ok?\n" );

   edm::Handle< pat::TriggerEvent > triggerEvent;
   if( pathltlabel_.size() > 0 ) { iEvent.getByLabel( pathltlabel_[0], triggerEvent ); }
   const pat::helper::TriggerMatchHelper matchHelper;

   const edm::View<reco::Track>& tracks = *TrackHandle;


   // All PF Candidate for alternate isolation
   edm::Handle<reco::PFCandidateCollection> pfCandidatesH;
   if( !TurnOffInCMSSW73x )
   { iEvent.getByLabel( "particleFlow", pfCandidatesH ); }
   if( !TurnOffInCMSSW73x )
   { thePfColl = *( pfCandidatesH.product() ); }

   // development for CMSSW_73X
   //edm::Handle<pat::PackedCandidateCollection> thePfColl;
   //iEvent.getByToken(pfToken_, thePfColl);
   //const  PackedCandidateCollection thePfColl = pfCandidatesH;


   // update for CMSSW_7_2_0
   // reference to (https://cmssdt.cern.ch/SDT/lxr/source//EgammaAnalysis/ElectronTools/plugins/ElectronIdMVAProducer.cc)
   EcalClusterLazyTools lazyTools( iEvent, iSetup, reducedEBRecHitCollectionToken_, reducedEERecHitCollectionToken_ );

   if( TurnOnInCMSSW_7_4_1 ){
   //    edm::ESHandle<TransientTrackBuilder> builder;
   //    iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", builder );
   //    TransientTrackBuilder thebuilder = *( builder.product() );
   //    const TransientTrackBuilder* transientTrackBuilder = builder.product();
   }

   //  get ConditionsInRunBlock (Only for RECO)
   /*
   edm::Handle<edm::ConditionsInRunBlock> condInRunBlock;
   iEvent.getByLabel("conditionsInEdm", condInRunBlock);

   const boost::uint16_t beamMomentumVal = condInRunBlock->beamMomentum;
   const boost::uint32_t lhcFillNumberVal = condInRunBlock->lhcFillNumber;

   std::cout <<"  Beam Momentum = " << beamMomentumVal << " GeV" << std::endl;
   std::cout <<"  LHC Fill Number = " << lhcFillNumberVal<< std::endl;
   */

   //  iEvent.getByLabel("electronGsfTracks",      gsfTrackHandle);  //get gsftracks (Dmitry)
   //  const edm::View<reco::GsfTrack> & gsfTracks = *gsfTrackHandle;

   // Start to fill the main root branches


   //------------------------------------------------------------------------------ 
   //   VertexInfo added by Dmitry to help filter out pile up and bad events
   //------------------------------------------------------------------------------ 
   fillVertex( iEvent , iSetup ) ;

   //=================================================================================
   // Leptons
   //=================================================================================
   for( unsigned icoll = 0; icoll < lepcollections_.size(); icoll++ ) { //loop over collections
      if( icoll >= MAX_LEPCOLLECTIONS ) { break; }
      if( debug_ > 5 ) { cout << "Fill lepton info, collection " << icoll << " with name " << lepcollections_[icoll] << endl; }
      memset( &LepInfo[icoll], 0x00, sizeof( LepInfo[icoll] ) );
      //Muons
      fillMuon( iEvent , iSetup , icoll ) ; 
      //Electrons
      fillElectron( iEvent, iSetup , icoll ) ;
      //Taus
      fillTau( iEvent , iSetup , icoll ) ;

   }//loop over collections

   //=================================================================================
   // Photons 
   //=================================================================================
   fillPhoton( iEvent , iSetup );

   //=================================================================================
   // Jets
   //=================================================================================
   fillJet( iEvent , iSetup ) ;

   //=================================================================================
   // Pairs
   //=================================================================================
   if( pairColl_ >= 0 ) { fillLepPair( iEvent , iSetup ) ; } 
#ifdef FILL_DIJET_PAIRS
   fillJetPair( iEvent, iSetup ) ;
#endif


   if( debug_ > 5 ) { cout << "\tFill Gen and Event Info.\n"; }
   memset( &GenInfo, 0x00, sizeof( GenInfo ) );
   memset( &EvtInfo, 0x00, sizeof( EvtInfo ) );
   fillGenInfo( iEvent , iSetup );

   EvtInfo.RunNo  = iEvent.id().run();
   EvtInfo.EvtNo  = iEvent.id().event();
   EvtInfo.McFlag = iEvent.isRealData() ? 0 : 1;
   EvtInfo.BxNo   = iEvent.bunchCrossing();
   EvtInfo.LumiNo = iEvent.luminosityBlock();
   EvtInfo.Orbit  = iEvent.orbitNumber();
   EvtInfo.nTrgBook = N_TRIGGER_BOOKINGS;

   EvtInfo.ptHat = -1.;
   /*
      // only for RECO, thus used GenProducer instead of HepMC with AOD sample
      // https://cmssdt.cern.ch/SDT/lxr/source/SimDataFormats/GeneratorProducts/src/GenEventInfoProduct.cc
   if(!isData){
       edm::Handle<HepMCProduct> evtpthat;
       iEvent.getByLabel("generator", evtpthat);
       std::cout<<"not data : evtpthat.isValid() : "<<evtpthat.isValid()<<std::endl;
       if(evtpthat.isValid()){
           HepMC::GenEvent * myGenEvent = new HepMC::GenEvent(*(evtpthat->GetEvent()));
           EvtInfo.ptHat = myGenEvent->event_scale();
       }
   }
   */


   if( !TurnOffInCMSSW73x )
      for( unsigned int ri_ = 0; ri_ < 2; ri_++ ) {
         if( rhoH[ri_].isValid() ) { EvtInfo.RhoPU[ri_] = *( rhoH[ri_].product() ); }
         if( sigmaHandle[ri_].isValid() ) { EvtInfo.SigmaPU[ri_] = *( sigmaHandle[ri_].product() ); }
      }

   // MC daughters: 0-1: hard jet from b'bar/t'bar, 2-9: W daughters, 10-13: Z daughters

   int mclep_count[2] = {0, 0};
   if ( EvtInfo.McWMode[0] == 1 || EvtInfo.McWMode[0] == 2 ) { mclep_count[1]++; }
   if ( EvtInfo.McWMode[1] == 1 || EvtInfo.McWMode[1] == 2 ) { mclep_count[0]++; }
   if ( EvtInfo.McWMode[2] == 1 || EvtInfo.McWMode[2] == 2 ) { mclep_count[0]++; }
   if ( EvtInfo.McWMode[3] == 1 || EvtInfo.McWMode[3] == 2 ) { mclep_count[1]++; }
   if ( EvtInfo.McZMode[0] == 1 || EvtInfo.McZMode[0] == 2 ) {
      mclep_count[0]++;
      mclep_count[1]++;
   }
   if ( EvtInfo.McZMode[1] == 1 || EvtInfo.McZMode[1] == 2 ) {
      mclep_count[0]++;
      mclep_count[1]++;
   }
   // McSigTag - 0: others, 1: dilepton (opposite-sign), 2: dilepton (same-sign), 3: trilepton, 4: 4-leptons
   if ( mclep_count[0] == 1 && mclep_count[1] == 1 ) { EvtInfo.McSigTag = 1; }
   if ( mclep_count[0] == 2 && mclep_count[1] == 0 ) { EvtInfo.McSigTag = 2; }
   if ( mclep_count[0] == 0 && mclep_count[1] == 2 ) { EvtInfo.McSigTag = 2; }
   if ( mclep_count[0] == 2 && mclep_count[1] == 1 ) { EvtInfo.McSigTag = 3; }
   if ( mclep_count[0] == 1 && mclep_count[1] == 2 ) { EvtInfo.McSigTag = 3; }
   if ( mclep_count[0] == 2 && mclep_count[1] == 2 ) { EvtInfo.McSigTag = 4; }


   //   if(METHandle.isValid()) {
   //     if(debug_>15) cout << "Get MET info\n";
   //     for( std::vector<pat::MET>::const_iterator it_met = METHandle->begin();
   //    it_met != METHandle->end(); it_met++ ) {

   //       EvtInfo.MET        = it_met->pt();
   //       EvtInfo.METPhi        = it_met->phi();
   //       EvtInfo.RawMET        = it_met->uncorrectedPt();
   //       EvtInfo.RawMETPhi     = it_met->uncorrectedPhi();

   //       EvtInfo.SumEt         = it_met->sumEt();
   //       EvtInfo.METSig        = it_met->mEtSig();
   //       EvtInfo.eLong         = it_met->e_longitudinal();
   //       EvtInfo.MaxHadTower   = it_met->maxEtInHadTowers();
   //       EvtInfo.MaxEmTower    = it_met->maxEtInEmTowers();
   //       EvtInfo.FracHad       = it_met->etFractionHadronic();
   //       EvtInfo.FracEm        = it_met->emEtFraction();

   //       const reco::GenMET * genmet = it_met->genMET();
   //       if (genmet!=NULL) {
   //         EvtInfo.GenMET        = genmet->pt();
   //         EvtInfo.GenMETPhi     = genmet->phi();
   //       }
   //     }
   //   }

   if( pfMETHandle.isValid() ) {
      if( debug_ > 15 ) { cout << "Get pfMET info\n"; }
      for( std::vector<pat::MET>::const_iterator it_pfmet = pfMETHandle->begin();
           it_pfmet != pfMETHandle->end(); it_pfmet++ ) {

         EvtInfo.PFMET           = it_pfmet->pt();
         EvtInfo.PFMETPhi        = it_pfmet->phi();
         EvtInfo.PFRawMET        = it_pfmet->uncorrectedPt();
         EvtInfo.PFRawMETPhi     = it_pfmet->uncorrectedPhi();

         EvtInfo.PFMETx          = it_pfmet->px(); //Uly 2011-04-04
         EvtInfo.PFMETy          = it_pfmet->py(); //Uly 2011-04-04

         EvtInfo.PFSumEt         = it_pfmet->sumEt();
         EvtInfo.PFMETSig        = it_pfmet->mEtSig();  //MET Significance = MET / std::sqrt(SumET)
         EvtInfo.PFMETRealSig    = it_pfmet->significance();  //real MET significance
         EvtInfo.PFMETlongitudinal    = it_pfmet->e_longitudinal();  //longitudinal component of the vector sum of energy over all object

         const reco::GenMET* genmet = it_pfmet->genMET();
         if ( !skipGenInfo_ && genmet != NULL ) {
            EvtInfo.PFGenMET        = genmet->pt();
            EvtInfo.PFGenMETPhi     = genmet->phi();
         }
      }
   }

   edm::Handle<std::vector<pat::MET> >          pfMETHandle_TempPlus;
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnUp",  pfMETHandle_TempPlus );
   if( pfMETHandle_TempPlus.isValid() )
      for( std::vector<pat::MET>::const_iterator it_pfmet = pfMETHandle_TempPlus->begin();
           it_pfmet != pfMETHandle_TempPlus->end(); it_pfmet++ ) {
         EvtInfo.PFMETType1CorrectedPFMetUnclusteredEnUp           = it_pfmet->pt();
      }
   edm::Handle<std::vector<pat::MET> >          pfMETHandle_TempDown;
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnDown",  pfMETHandle_TempDown );
   if( pfMETHandle_TempDown.isValid() )
      for( std::vector<pat::MET>::const_iterator it_pfmet = pfMETHandle_TempDown->begin();
           it_pfmet != pfMETHandle_TempDown->end(); it_pfmet++ ) {
         EvtInfo.PFMETType1CorrectedPFMetUnclusteredEnDown           = it_pfmet->pt();
      }




   edm::Handle<GenEventInfoProduct> GenEventInfoHandle;
   bool with_GenEventInfo = ( genevtlabel_.size() > 0 ) ? iEvent.getByLabel( genevtlabel_[0], GenEventInfoHandle ) : false;

   if ( with_GenEventInfo && GenEventInfoHandle->hasPDF() ) {
      EvtInfo.PDFid1   = GenEventInfoHandle->pdf()->id.first;
      EvtInfo.PDFid2   = GenEventInfoHandle->pdf()->id.second;
      EvtInfo.PDFx1    = GenEventInfoHandle->pdf()->x.first;
      EvtInfo.PDFx2    = GenEventInfoHandle->pdf()->x.second;
      EvtInfo.PDFscale = GenEventInfoHandle->pdf()->scalePDF;
      EvtInfo.PDFv1    = GenEventInfoHandle->pdf()->xPDF.first;
      EvtInfo.PDFv2    = GenEventInfoHandle->pdf()->xPDF.second;
   }

   // HLT: Booking trigger bits
   edm::Handle<TriggerResults> TrgResultsHandle;
   bool with_TriggerResults = ( hltlabel_.size() > 0 ) ? iEvent.getByLabel( hltlabel_[0], TrgResultsHandle ) : false;
   if ( with_TriggerResults ) {

      const edm::TriggerNames& TrgNames = iEvent.triggerNames( *TrgResultsHandle );
      EvtInfo.TrgCount = 0;
      for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ) {

         unsigned int TrgIndex = TrgNames.triggerIndex( TriggerBooking[i] );

         if ( TrgIndex == TrgNames.size() ) {
            EvtInfo.TrgBook[i] = -4; // The trigger path is not known in this event.
         } else if ( !TrgResultsHandle->wasrun( TrgIndex ) ) {
            EvtInfo.TrgBook[i] = -3; // The trigger path was not included in this event.
         } else if ( !TrgResultsHandle->accept( TrgIndex ) ) {
            EvtInfo.TrgBook[i] = -2; // The trigger path was not accepted in this event.
         } else if (  TrgResultsHandle->error ( TrgIndex ) ) {
            EvtInfo.TrgBook[i] = -1; // The trigger path has an error in this event.
         } else {
            EvtInfo.TrgBook[i] = +1; // It's triggered.
            EvtInfo.TrgCount++;
         }
      }
      EvtInfo.nHLT = TrgNames.size();
      std::pair<int, int>  psValueCombo;
      for( unsigned int i = 0; i < TrgNames.size(); i++ ) {
         EvtInfo.HLTbits[i] = ( TrgResultsHandle->accept( i ) == true ) ? 1 : 0;
         const std::string triggerName_ = TrgNames.triggerName( i );
         psValueCombo = hltConfig_.prescaleValues( iEvent, iSetup, triggerName_ );
         // 02/13/2015 : new function for prescale of L1/HLT (http://cmslxr.fnal.gov/source/HLTrigger/HLTcore/plugins/HLTEventAnalyzerAOD.cc)
         EvtInfo.HLTPrescaleFactor[i] = ( int )psValueCombo.second;

         //std::cout << "TriggerPath= " << TrgNames.triggerName(i) <<" Prescale= "<< psValueCombo.first*psValueCombo.second
         //    <<" "<< (int)psValueCombo.first<<" "<<(int)psValueCombo.second
         //    << std::endl;


         HLTmaplist_pr = HLTmaplist.find( TrgNames.triggerName( i ) );
         if( HLTmaplist_pr != HLTmaplist.end() ) {
            EvtInfo.HLTName2enum[i] = HLTmaplist_pr->second ;
         } else {
            EvtInfo.HLTName2enum[i] = -1;
         }
         // Print out Trigger table
         //std::cout << "trigger path= " << TrgNames.triggerName(i) << std::endl;
      }

   }
   ///**********************************************************
   //   L1 trigger and techincal trigger bits
   //***********************************************************
   edm::Handle< L1GlobalTriggerReadoutRecord > gtRecord;
   if( gtdigilabel_.size() > 0 ) { iEvent.getByLabel( gtdigilabel_[0], gtRecord ); }

   if( gtRecord.isValid() ) {
      DecisionWord dWord = gtRecord->decisionWord();
      if ( ! dWord.empty() ) { // if board not there this is zero
         // loop over dec. bit to get total rate (no overlap)
         for ( int i = 0; i < 128; ++i ) {
            //  if(dWord[i]!=0 && debug)cout << i << " " << dWord[i] << ": ";
            EvtInfo.L1[i] = dWord[i];
         }
      }
      TechnicalTriggerWord tw = gtRecord->technicalTriggerWord();
      if ( ! tw.empty() ) {
         // loop over dec. bit to get total rate (no overlap)
         for ( int i = 0; i < 64; ++i ) {
            //  if(tw[i]!=0 && debug) cout << i << "  " << tw[i] << ": ";
            EvtInfo.TT[i] = tw[i];
         }
      }
   }

   //by Dmitry Hits for filtering the real data
   //*******************************************************************
   //Add the number of tracks and fraction of high purity tracks in the track collection to the EvtInfo
   //*******************************************************************
   //
   //The desciption of tracks quality selection is here:
   //https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFinalTrackSelectors
   //
   //Cuts which determine the high purity depend on the CMSSW release:
   //http://cmslxr.fnal.gov/lxr/source/RecoTracker/FinalTrackSelectors/python/selectHighPurity_cfi.py
   //
   //For details see CMS IN-2008/017
   //Currently, if the number of tracks > 10,
   //then 25% of HighPurity tracks have to be present in the event for it to be called good event

   int numhighpurity = 0;
   if( !TurnOffInCMSSW73x )
      if( TrackHandle.isValid() && !TrackHandle.failedToGet() && TrackHandle->size() > 0 ) {

         reco::TrackBase::TrackQuality _trackQuality = reco::TrackBase::qualityByName( "highPurity" );

         edm::View<reco::Track>::const_iterator itk = tracks.begin();
         edm::View<reco::Track>::const_iterator itk_e = tracks.end();
         for( ; itk != itk_e; ++itk ) {
            // std::cout << "HighPurity?  " << itk->quality(_trackQuality) << std::endl;
            if( itk->quality( _trackQuality ) ) { numhighpurity++; }
         }
         EvtInfo.NofTracks = TrackHandle->size();
         EvtInfo.HighPurityFraction = ( float )numhighpurity / ( float )TrackHandle->size();
      }

   if( debug_ > 11 ) {
      for( unsigned i = 0; i < lepcollections_.size(); i++ ) {
         cout << "Lepton Collection " << i << "(" << lepcollections_[i] << "): size " << LepInfo[i].Size << endl;
         for( int j = 0; j < LepInfo[i].Size; j++ )
         { cout << "  Lep " << j << " type,pt,eta,phi " << LepInfo[i].LeptonType[j] << "," << LepInfo[i].Pt[j] << "," << LepInfo[i].Eta[j] << "," << LepInfo[i].Phi[j] << endl; }
      }
   }

   if( debug_ > 5 ) { cout << "Fill tree with all information\n"; }
   root->Fill();
   if( debug_ > 10 ) { cout << "Filled event information: Run " << EvtInfo.RunNo << " Event " << EvtInfo.EvtNo << endl; }

   if( debug_ > 11 ) {
      root->Show( -1, 999 );
      for( unsigned i = 0; i < lepcollections_.size(); i++ ) {
         cout << "After fill, Lepton Collection " << i << "(" << lepcollections_[i] << "): size " << LepInfo[i].Size << endl;
         for( int j = 0; j < LepInfo[i].Size; j++ )
         { cout << "  Lep " << j << " type,pt,eta,phi " << LepInfo[i].LeptonType[j] << "," << LepInfo[i].Pt[j] << "," << LepInfo[i].Eta[j] << "," << LepInfo[i].Phi[j] << endl; }
      }
   }

}


//define this as a plug-in
DEFINE_FWK_MODULE( bprimeKit );

