/*******************************************************************************
*
*  Filename    : bprimeKit_photon.cc
*  Description : Fill the ntuple with photon information
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/PhotonNtuplizer.hpp"

using namespace std;

/*******************************************************************************
*   PhotonNtuplizer constructor and destructor
*******************************************************************************/
PhotonNtuplizer::PhotonNtuplizer( const edm::ParameterSet& iConfig, bprimeKit* bpk ) :
  NtuplizerBase( iConfig, bpk ),
  _photonname( iConfig.getParameter<string>( "photonname" ) ),
  _rhotoken( GetToken<double>( "rhosrc" ) ),
  _photontoken( GetToken<vector<pat::Photon> >( "photonsrc"  ) ),
  _photonID_loosemap( iConfig.getParameter<string>( "phoLooseIdMap" ) ),
  _photonID_mediummap( iConfig.getParameter<string>( "phoMediumIdMap" ) ),
  _photonID_tightmap( iConfig.getParameter<string>( "phoTightIdMap" ) ),
  _photonID_mva( iConfig.getParameter<string>( "phoIdMvaMap" ) ),
  _photonEffectiveArea_ChargeHadron( iConfig.getParameter<edm::FileInPath>( "effAreaChHadFile" ).fullPath() ),
  _photonEffectiveArea_NeutralHadron( iConfig.getParameter<edm::FileInPath>( "effAreaNeuHadFile" ).fullPath() ),
  _photonEffectiveArea_Photons( iConfig.getParameter<edm::FileInPath>( "effAreaPhoFile" ).fullPath()  )
{
}

/******************************************************************************/

PhotonNtuplizer::~PhotonNtuplizer()
{

}

/******************************************************************************/

void
PhotonNtuplizer::RegisterTree( TTree* tree )
{
  PhotonInfo.RegisterTree( tree, _photonname );
}


/*******************************************************************************
*   Main loop
*******************************************************************************/
void
PhotonNtuplizer::Analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  iEvent.getByToken( _rhotoken,                      _rhohandle                 );
  iEvent.getByToken( _photontoken,                   _photonhandle              );

  memset( &PhotonInfo, 0x00, sizeof( PhotonInfo ) );

  for( auto it_pho = _photonhandle->begin(); it_pho != _photonhandle->end(); it_pho++ ){
    if( PhotonInfo.Size >= MAX_PHOTONS ){
      cerr <<  "ERROR: number of photons exceeds the size of array." << endl;
      break;// exit(0);
    }

    // ----- Very very loose pre-selection -----------------------------------------------------------------
    if( it_pho->pt() < 10. ){ continue; }

    // ----- Basic kinematics ------------------------------------------------------------------------------
    PhotonInfo.Pt            [PhotonInfo.Size] = it_pho->pt();
    PhotonInfo.Eta           [PhotonInfo.Size] = it_pho->eta();
    PhotonInfo.Phi           [PhotonInfo.Size] = it_pho->phi();
    PhotonInfo.Px            [PhotonInfo.Size] = it_pho->px();
    PhotonInfo.Py            [PhotonInfo.Size] = it_pho->py();
    PhotonInfo.Pz            [PhotonInfo.Size] = it_pho->pz();
    PhotonInfo.Energy        [PhotonInfo.Size] = it_pho->energy();
    PhotonInfo.isPFPhoton    [PhotonInfo.Size] = it_pho->isPFlowPhoton();

    // ----- Energy scale and smear correction and associated uncertainties --------------------------------
    // Twiki : https://twiki.cern.ch/twiki/bin/view/CMS/EgammaMiniAODV2
    // Internal Code : https://github.com/cms-sw/cmssw/blob/CMSSW_9_4_X/RecoEgamma/EgammaTools/src/PhotonEnergyCalibrator.cc#L93
    PhotonInfo.PhoEnergyCorrFactor         [PhotonInfo.Size] = it_pho->userFloat("ecalEnergyPostCorr") / it_pho->energy(); 
    PhotonInfo.PhoEnergyPreCorrErr         [PhotonInfo.Size] = it_pho->getCorrectedEnergyError(reco::Photon::P4type::regression2); 
    PhotonInfo.PhoEnergyPostCorrErr        [PhotonInfo.Size] = it_pho->userFloat("ecalEnergyErrPostCorr"); 
    PhotonInfo.PhoEnergyPostCorrScaleUp    [PhotonInfo.Size] = it_pho->userFloat("energyScaleUp"); 
    PhotonInfo.PhoEnergyPostCorrScaleDown  [PhotonInfo.Size] = it_pho->userFloat("energyScaleDown"); 
    PhotonInfo.PhoEnergyPostCorrSmearUp    [PhotonInfo.Size] = it_pho->userFloat("energySigmaUp"); 
    PhotonInfo.PhoEnergyPostCorrSmearDown  [PhotonInfo.Size] = it_pho->userFloat("energySigmaDown"); 

    // ----- Cut based photon ID and ID MVA (V2) ----------------------------------------------------------------
    // ----- Cut based photon ID -----
    // Twiki : https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2#Offline_selection_criteria
    PhotonInfo.EgammaCutBasedPhoIdLOOSE  [PhotonInfo.Size] = it_pho->photonID( _photonID_loosemap );
    PhotonInfo.EgammaCutBasedPhoIdMEDIUM [PhotonInfo.Size] = it_pho->photonID( _photonID_mediummap );
    PhotonInfo.EgammaCutBasedPhoIdTIGHT  [PhotonInfo.Size] = it_pho->photonID( _photonID_tightmap );

    // ----- Photon ID MVA -----
    PhotonInfo.EgammaPhoIdMVA            [PhotonInfo.Size] = it_pho->userFloat( _photonID_mva );

    // ----- Photon isolation variables --------------------------------------------------------------------
    PhotonInfo.ChargedHadronIsoR03      [PhotonInfo.Size] = it_pho->userFloat("phoChargedIsolation"); 
    PhotonInfo.NeutralHadronIsoR03      [PhotonInfo.Size] = it_pho->userFloat("phoNeutralHadronIsolation");
    PhotonInfo.PhotonIsoR03             [PhotonInfo.Size] = it_pho->userFloat("phoPhotonIsolation");
    PhotonInfo.WorstChargedIsolationR03 [PhotonInfo.Size] = it_pho->userFloat("phoWorstChargedIsolation"); 

    const double rho       = _rhohandle.isValid() ? *_rhohandle : 0.;
    const double areachhad = _photonEffectiveArea_ChargeHadron.getEffectiveArea( fabs( it_pho->superCluster()->eta() ) );
    const double areanuhad = _photonEffectiveArea_NeutralHadron.getEffectiveArea( fabs( it_pho->superCluster()->eta() ) );
    const double areapho   = _photonEffectiveArea_Photons.getEffectiveArea( fabs( it_pho->superCluster()->eta() ) );

    PhotonInfo.PFChIsoRhoCorr           [PhotonInfo.Size] = std::max( 0.0, PhotonInfo.ChargedHadronIsoR03  [PhotonInfo.Size] - rho * areachhad );
    PhotonInfo.PFNeuIsoRhoCorr          [PhotonInfo.Size] = std::max( 0.0, PhotonInfo.NeutralHadronIsoR03  [PhotonInfo.Size] - rho * areanuhad );
    PhotonInfo.PFPhoIsoRhoCorr          [PhotonInfo.Size] = std::max( 0.0, PhotonInfo.PhotonIsoR03         [PhotonInfo.Size] - rho * areapho   );

    // ----- Other information associated with ID and Iso --------------------------------------------------
    PhotonInfo.passelectronveto     [PhotonInfo.Size] = (int)it_pho->passElectronVeto();
    PhotonInfo.hasPixelSeed         [PhotonInfo.Size] = it_pho->hasPixelSeed();

    // ----- Photon superCluster and shrower shape (Full 5x5, no ZS version) variables ---------------------
    // ----- superCluster variables -----
    PhotonInfo.EtaSC         [PhotonInfo.Size] = it_pho->superCluster()->eta();
    PhotonInfo.PhiSC         [PhotonInfo.Size] = it_pho->superCluster()->phi();
    PhotonInfo.rawEnergySC   [PhotonInfo.Size] = it_pho->superCluster()->rawEnergy();
    PhotonInfo.ESEnergySC    [PhotonInfo.Size] = it_pho->superCluster()->preshowerEnergy();
    PhotonInfo.EnergySC      [PhotonInfo.Size] = it_pho->superCluster()->energy();

    // ----- shrower shape (Full 5x5, no ZS version) variables -----
    PhotonInfo.R9            [PhotonInfo.Size] = it_pho->full5x5_r9();
    PhotonInfo.E5x5          [PhotonInfo.Size] = it_pho->full5x5_e5x5();
    PhotonInfo.SigmaIetaIeta [PhotonInfo.Size] = it_pho->full5x5_sigmaIetaIeta();
    PhotonInfo.SigmaIetaIphi [PhotonInfo.Size] = it_pho->full5x5_showerShapeVariables().sigmaIetaIphi;
    PhotonInfo.SigmaIphiIphi [PhotonInfo.Size] = it_pho->full5x5_showerShapeVariables().sigmaIphiIphi;
    PhotonInfo.S4            [PhotonInfo.Size] = it_pho->full5x5_showerShapeVariables().e2x2 / it_pho->full5x5_e5x5();
    PhotonInfo.ESEffSigmaRR  [PhotonInfo.Size] = it_pho->full5x5_showerShapeVariables().effSigmaRR;
    PhotonInfo.EtaWidth      [PhotonInfo.Size] = it_pho->superCluster()->etaWidth();
    PhotonInfo.PhiWidth      [PhotonInfo.Size] = it_pho->superCluster()->phiWidth();
    PhotonInfo.HoverE        [PhotonInfo.Size] = it_pho->hadronicOverEm();
    PhotonInfo.hadTowOverEm  [PhotonInfo.Size] = it_pho->hadTowOverEm();

    // ----- Generation MC information ---------------------------------------------------------------------
    if( !iEvent.isRealData() ){
      const reco::Candidate* gen = it_pho->genPhoton();
      if( gen != NULL ){
        PhotonInfo.GenPt        [PhotonInfo.Size] = gen->pt();
        PhotonInfo.GenEta       [PhotonInfo.Size] = gen->eta();
        PhotonInfo.GenPhi       [PhotonInfo.Size] = gen->phi();
        PhotonInfo.GenPdgID     [PhotonInfo.Size] = gen->pdgId();
      }
    }
    PhotonInfo.Size++;
  }
}
