/*******************************************************************************
*
*  Filename    : bprimeKit_jets.cc
*  Description : Filling in jet information
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/JetNtuplizer.hpp"

// ----- Jet Specific CMSSW packages  ---------------------------------------------------------------
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/BTauReco/interface/CATopJetTagInfo.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "TLorentzVector.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

/*******************************************************************************
*   Static helper variables
*******************************************************************************/
static const string prefix = "bpkFrameWork/bprimeKit/data/";

/*******************************************************************************
*   Jet Ntuplization constructor
*******************************************************************************/
JetNtuplizer::JetNtuplizer( const edm::ParameterSet& iConfig, bprimeKit* bpk ) :
  NtuplizerBase( iConfig, bpk ),
  _jetname( iConfig.getParameter<std::string>( "jetname" ) ),
  _jettype( iConfig.getParameter<std::string>( "jettype" ) ),
  _jecversion( iConfig.getParameter<string>( "jecversion" ) ),
  _rhotoken( GetToken<double>( "rhosrc" ) ),
  _vtxtoken( GetToken<std::vector<reco::Vertex> >( "vtxsrc" ) ),
  _jettoken( GetToken<std::vector<pat::Jet> >( "jetsrc" ) ),
  _muontoken( GetToken<std::vector<pat::Muon> >( "muonsrc" ) ),
  _subjettoken( GetToken<std::vector<pat::Jet> >( "subjetsrc" ) )
{
  if( _jecversion != "" ){
    _jetcorrector = new FactorizedJetCorrector( {
      JetCorrectorParameters( edm::FileInPath( prefix + _jecversion + "_L1FastJet_" +    _jettype + ".txt" ).fullPath() ),
      JetCorrectorParameters( edm::FileInPath( prefix + _jecversion + "_L2Relative_" +   _jettype + ".txt" ).fullPath() ),
      JetCorrectorParameters( edm::FileInPath( prefix + _jecversion + "_L3Absolute_" +   _jettype + ".txt" ).fullPath() ),
      JetCorrectorParameters( edm::FileInPath( prefix + _jecversion + "_L2L3Residual_" + _jettype + ".txt" ).fullPath() )
    } );
    _jetunc = new JetCorrectionUncertainty(
      edm::FileInPath( prefix + _jecversion + "_Uncertainty_" + _jettype + ".txt" ).fullPath()
      );
  } else {
    _jetcorrector = nullptr;
    _jetunc       = nullptr;
  }
}

/******************************************************************************/

JetNtuplizer::~JetNtuplizer()
{
  if( _jetcorrector ){ delete _jetcorrector; }
  if( _jetunc       ){ delete _jetunc; }
}

/*******************************************************************************
*   Implementaion of Virtual functions
*******************************************************************************/
void
JetNtuplizer::RegisterTree( TTree* tree )
{
  JetInfo.RegisterTree( tree, _jetname );
}

/******************************************************************************/

void
JetNtuplizer::Analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  edm::ESHandle<JetCorrectorParametersCollection> jetCorParColl;
  iEvent.getByToken( _rhotoken,    _rhohandle );
  iEvent.getByToken( _jettoken,    _jethandle );
  iEvent.getByToken( _vtxtoken,    _vtxhandle );
  iEvent.getByToken( _muontoken,   _muonhandle );
  iEvent.getByToken( _subjettoken, _subjethandle );

  memset( &JetInfo, 0x00, sizeof( JetInfo ) );

  const double pt_cut = IsAK4() ? 15. : 100;

  iSetup.get<JetCorrectionsRecord>().get( _jettype.c_str(), jetCorParColl );
  const JetCorrectorParameters& jetCorPar = ( *jetCorParColl )["Uncertainty"];
  JetCorrectionUncertainty jecUnc( jetCorPar );

  const JME::JetResolution jetptres( JME::JetResolution::get( iSetup, _jettype + "_pt" ) );
  const JME::JetResolution jetphires( JME::JetResolution::get( iSetup, _jettype + "_phi" ) );
  const JME::JetResolutionScaleFactor jetressf( JME::JetResolutionScaleFactor::get( iSetup, _jettype ) );

  // Beginning maing jet loop
  for( auto it_jet = _jethandle->begin(); it_jet != _jethandle->end(); it_jet++ ){
    if( JetInfo.Size >= MAX_JETS ){
      cerr << "ERROR: number of jets exceeds the size of array." << endl;
      break;
    }
    if( it_jet->pt() <= pt_cut ){ continue; }// IMPORTANT: Only book jet with pt>15 GeV.

    // ----- Generic Jet Information  -------------------------------------
    JetInfo.Index         [JetInfo.Size] = JetInfo.Size;
    JetInfo.NTracks       [JetInfo.Size] = it_jet->associatedTracks().size();
    JetInfo.JetCharge     [JetInfo.Size] = it_jet->jetCharge();
    JetInfo.NConstituents [JetInfo.Size] = it_jet->numberOfDaughters();
    JetInfo.Mass          [JetInfo.Size] = it_jet->mass();
    JetInfo.Area          [JetInfo.Size] = it_jet->jetArea();
    JetInfo.Pt            [JetInfo.Size] = it_jet->pt();
    JetInfo.Eta           [JetInfo.Size] = it_jet->eta();
    JetInfo.Phi           [JetInfo.Size] = it_jet->phi();
    JetInfo.Energy        [JetInfo.Size] = it_jet->energy();
    JetInfo.Px            [JetInfo.Size] = it_jet->px();
    JetInfo.Py            [JetInfo.Size] = it_jet->py();
    JetInfo.Pz            [JetInfo.Size] = it_jet->pz();
    JetInfo.Et            [JetInfo.Size] = it_jet->et();

    // ----- Jet Correction Information  ----------------------------------------------------------------
    JetInfo.PtCorrRaw   [JetInfo.Size] = it_jet->correctedJet( "Uncorrected" ).pt();
    JetInfo.PtCorrL2    [JetInfo.Size] = it_jet->correctedJet( "L2Relative" ).pt();// L2(rel)
    JetInfo.PtCorrL3    [JetInfo.Size] = it_jet->correctedJet( "L3Absolute" ).pt();// L3(abs)
    JetInfo.NCH         [JetInfo.Size] = it_jet->chargedMultiplicity();
    JetInfo.CEF         [JetInfo.Size] = it_jet->chargedEmEnergyFraction();
    JetInfo.NHF         [JetInfo.Size] = it_jet->neutralHadronEnergyFraction();
    JetInfo.NEF         [JetInfo.Size] = it_jet->neutralEmEnergyFraction();
    JetInfo.CHF         [JetInfo.Size] = it_jet->chargedHadronEnergyFraction();

    // Legacy corrections?
    // JetInfo.PtCorrL7g   [JetInfo.Size] = it_jet->correctedJet( "L7Parton", "gluon"  ).pt();// L7(gluon)
    // JetInfo.PtCorrL7uds [JetInfo.Size] = it_jet->correctedJet( "L7Parton", "uds"    ).pt();// L7(uds-jet)
    // JetInfo.PtCorrL7c   [JetInfo.Size] = it_jet->correctedJet( "L7Parton", "charm"  ).pt();// L7(c-jet)
    // JetInfo.PtCorrL7b   [JetInfo.Size] = it_jet->correctedJet( "L7Parton", "bottom" ).pt();// L7(b-jet)

    // ----- B Tagging discriminators  ------------------------------------------------------------------
    JetInfo.pfJetBProbabilityBJetTags[JetInfo.Size]
      = it_jet->bDiscriminator( "pfBoostedDoubleSecondaryVertexAK8BJetTags"    );
    JetInfo.combinedSecondaryVertexBJetTags[JetInfo.Size]
      = it_jet->bDiscriminator( "combinedSecondaryVertexBJetTags"              );
    JetInfo.pfJetProbabilityBJetTags[JetInfo.Size]
      = it_jet->bDiscriminator( "pfJetProbabilityBJetTags"                     );
    JetInfo.pfTrackCountingHighPurBJetTags[JetInfo.Size]
      = it_jet->bDiscriminator( "pfTrackCountingHighPurBJetTags"               );
    JetInfo.pfTrackCountingHighEffBJetTags[JetInfo.Size]
      = it_jet->bDiscriminator( "pfTrackCountingHighEffBJetTags"               );
    JetInfo.pfSimpleSecondaryVertexHighEffBJetTags[JetInfo.Size]
      = it_jet->bDiscriminator( "pfSimpleSecondaryVertexHighEffBJetTags"       );
    JetInfo.pfSimpleSecondaryVertexHighPurBJetTags[JetInfo.Size]
      = it_jet->bDiscriminator( "pfSimpleSecondaryVertexHighPurBJetTags"       );
    JetInfo.pfCombinedSecondaryVertexV2BJetTags [JetInfo.Size]
      = it_jet->bDiscriminator( "pfCombinedSecondaryVertexV2BJetTags"          );
    JetInfo.pfCombinedMVABJetTags[JetInfo.Size]
      = it_jet->bDiscriminator( "pfCombinedMVABJetTags"                        );
    JetInfo.pfCombinedInclusiveSecondaryVertexV2BJetTags[JetInfo.Size]
      = it_jet->bDiscriminator( "pfCombinedInclusiveSecondaryVertexV2BJetTags" );
    JetInfo.pfCombinedSecondaryVertexSoftLeptonBJetTags [JetInfo.Size]
      = it_jet->bDiscriminator( "pfCombinedSecondaryVertexSoftLeptonBJetTags"  );

    // ----- Cleaned Jet four momentum  -----------------------------------------
    const TLorentzVector cleanedJet =
      IsAK4() ? CleanAK4Jet( it_jet ) :
      CleanAK8Jet( it_jet  );
    JetInfo.Pt_MuonCleaned[JetInfo.Size]     = cleanedJet.Pt();
    JetInfo.Eta_MuonCleaned[JetInfo.Size]    = cleanedJet.Eta();
    JetInfo.Phi_MuonCleaned[JetInfo.Size]    = cleanedJet.Phi();
    JetInfo.Energy_MuonCleaned[JetInfo.Size] = cleanedJet.Energy();

    // ----- Jet ID string insertions  -----------------------------------------
    bool jetID = true;
    if( IsAK4() ){
      edm::ParameterSet jetidParam;
      jetidParam.addParameter<std::string>( "version", "FIRSTDATA" );
      jetidParam.addParameter<std::string>( "quality", "LOOSE" );
      PFJetIDSelectionFunctor pfjetIDLOOSE( jetidParam );
      pat::strbitset ret = pfjetIDLOOSE.getBitTemplate();
      ret.set( false );
      jetID = pfjetIDLOOSE( *it_jet, ret );
    } else {
      jetID = true;// Apply jetID in PAT level
    }
    JetInfo.JetIDLOOSE[JetInfo.Size] = ( jetID ) ?  1 : 0;

    // ----- Jet Uncertainty  ----------------------------------------------------
    if( fabs( it_jet->eta() ) <= 5.0 ){
      if( _jetcorrector && _jetunc ){
        _jetcorrector->setJetEta( it_jet->eta() );
        _jetcorrector->setJetPt( it_jet->pt() );
        _jetcorrector->setJetA( it_jet->jetArea() );
        _jetcorrector->setRho( *_rhohandle );
        const double unc = _jetcorrector->getCorrection();
        JetInfo.Unc[JetInfo.Size] = unc;

        _jetunc->setJetEta( it_jet->eta() );
        _jetunc->setJetPt( it_jet->pt() * unc );
        JetInfo.JesUnc[JetInfo.Size] = _jetunc->getUncertainty( true );
      } else {
        jecUnc.setJetEta( it_jet->eta() );
        jecUnc.setJetPt( it_jet->pt() );// here you must use the CORRECTED jet pt
        const double tmp = jecUnc.getUncertainty( true ); // must only be evaluated once.
        JetInfo.Unc[JetInfo.Size]    = tmp;
        JetInfo.JesUnc[JetInfo.Size] = tmp;
      }
    }

    // Jet Resolution information --------------------------------------------------
    JME::JetParameters jetparm;
    jetparm.setJetPt( it_jet->pt() ).setJetEta( it_jet->eta() ).setRho( *_rhohandle );
    JetInfo.JERPt [JetInfo.Size]   = jetptres.getResolution( jetparm );
    JetInfo.JERPhi[JetInfo.Size]   = jetphires.getResolution( jetparm );
    JetInfo.JERScale[JetInfo.Size] = jetressf.getScaleFactor( jetparm );


    // ------------------------------------------------------------------------------
    //   AK4 Jet Specific variables
    // ------------------------------------------------------------------------------
    if( IsAK4() ){
      if( _jetname == "JetInfo" ){
        JetInfo.QGTagsLikelihood [JetInfo.Size] = it_jet->userFloat( "QGTaggerAK4PFCHS:qgLikelihood" );
      } else if( _jetname == "JetInfoPuppi" ){
      }
      // ----- Particle flow information  -----------------------------------------------------------------
    }

    // ------------------------------------------------------------------------------
    //   AK8 Jet Specific variables
    // ------------------------------------------------------------------------------
    if( IsWideJet() ){
      JetInfo.pfBoostedDoubleSecondaryVertexAK8BJetTags[JetInfo.Size]
                                                      = it_jet->bDiscriminator( "pfBoostedDoubleSecondaryVertexAK8BJetTags" );
      JetInfo.NjettinessAK8tau1        [JetInfo.Size] = it_jet->userFloat( "Njettiness" + UserFloatName() + ":tau1"       );
      JetInfo.NjettinessAK8tau2        [JetInfo.Size] = it_jet->userFloat( "Njettiness" + UserFloatName() + ":tau2"       );
      JetInfo.NjettinessAK8tau3        [JetInfo.Size] = it_jet->userFloat( "Njettiness" + UserFloatName() + ":tau3"       );
      JetInfo.ak8PFJetsCHSSoftDropMass [JetInfo.Size] = it_jet->userFloat( UserFloatPrefix() + "SoftDropMass" );
      JetInfo.ak8PFJetsCHSPrunedMass   [JetInfo.Size] = it_jet->userFloat( UserFloatPrefix() + "PrunedMass"   );

      JetInfo.NSubjets        [JetInfo.Size] = 0;
      JetInfo.SubjetsIdxStart [JetInfo.Size] = 0;

      for( int idx_pre = 0; idx_pre < JetInfo.Size; ++idx_pre ){
        JetInfo.SubjetsIdxStart[JetInfo.Size] += JetInfo.NSubjets[idx_pre];
      }

      auto subjet_bunch = GetSubjetBunch( it_jet  );
      if( subjet_bunch != _subjethandle->end() ){
        JetInfo.NSubjets[JetInfo.Size] = subjet_bunch->numberOfDaughters();

        for( unsigned i = 0; i < subjet_bunch->numberOfDaughters(); ++i ){
          const pat::Jet* subjet = (pat::Jet*)subjet_bunch->daughter( i );
          JetInfo.SubjetMass_w.push_back( subjet->mass() );
          JetInfo.SubjetPt_w.push_back( subjet->pt() );
          JetInfo.SubjetEt_w.push_back( subjet->et() );
          JetInfo.SubjetEta_w.push_back( subjet->eta() );
          JetInfo.SubjetPhi_w.push_back( subjet->phi() );
          JetInfo.SubjetArea_w.push_back( subjet->jetArea() );
          JetInfo.SubjetPtUncorr_w.push_back( subjet->pt()*subjet->jecFactor( "Uncorrected" ) );
          JetInfo.SubjetCombinedSVBJetTags_w.push_back( subjet->bDiscriminator( "pfCombinedInclusiveSecondaryVertexV2BJetTags" ) );
          if( !iEvent.isRealData() ){
            JetInfo.SubjetHadronFlavour_w.push_back( subjet->hadronFlavour() );
            JetInfo.SubjetGenFlavour_w.push_back( subjet->hadronFlavour() );
            JetInfo.SubjetGenPdgId_w.push_back( subjet->pdgId() );
          }
        }
      }
    }

    // ----- Generation MC Data  --------------------------------------------------
    if( !iEvent.isRealData() ){
      const reco::GenJet* genjet = it_jet->genJet();
      if( genjet != NULL ){
        JetInfo.GenJetPt  [JetInfo.Size] = genjet->pt();
        JetInfo.GenJetEta [JetInfo.Size] = genjet->eta();
        JetInfo.GenJetPhi [JetInfo.Size] = genjet->phi();
      }
      const reco::GenParticle* parton = it_jet->genParton();
      if( parton != NULL ){
        JetInfo.GenPt     [JetInfo.Size]      = parton->pt();
        JetInfo.GenEta    [JetInfo.Size]      = parton->eta();
        JetInfo.GenPhi    [JetInfo.Size]      = parton->phi();
        JetInfo.GenPdgID  [JetInfo.Size]      = parton->pdgId();
        JetInfo.GenFlavor [JetInfo.Size]      = it_jet->partonFlavour();
        JetInfo.GenHadronFlavor[JetInfo.Size] = it_jet->hadronFlavour();
        JetInfo.GenMCTag  [JetInfo.Size]      = bprimeKit::GetGenMCTag( parton );
      }
    }
    JetInfo.Size++;
  }
}

/*******************************************************************************
*   Helper private functions
*******************************************************************************/

vector<pat::Jet>::const_iterator
JetNtuplizer::GetSubjetBunch( const vector<pat::Jet>::const_iterator& mainjet )
{
  for( auto ans = _subjethandle->begin(); ans != _subjethandle->end(); ++ans ){
    if( reco::deltaR2( mainjet->eta(), mainjet->phi(), ans->eta(), ans->phi() ) < 0.8 ){
      return ans;
    }
  }

  return _subjethandle->end();
}


/*******************************************************************************
*   Jet type parsing functions
*******************************************************************************/
bool
JetNtuplizer::IsAK4() const
{
  return _jettype.find( "8" ) == std::string::npos;
}

/******************************************************************************/

bool
JetNtuplizer::IsWideJet() const
{
  return !IsAK4();
}

/******************************************************************************/

string
JetNtuplizer::UserFloatName() const
{
  return _jetname.find( "Puppi" )   != std::string::npos ? "AK8Puppi" :
         _jetname.find( "MiniAOD" ) != std::string::npos ? "AK8" :
         "AK8CHS";
}

/******************************************************************************/

string
JetNtuplizer::UserFloatPrefix() const
{
  return _jetname.find( "Puppi"   ) != std::string::npos ? "ak8PFJetsPuppi" :
         _jetname.find( "MiniAOD" ) != std::string::npos ? "ak8PFJetsCHS" :
         "ak8PFJetsCHS";
}
