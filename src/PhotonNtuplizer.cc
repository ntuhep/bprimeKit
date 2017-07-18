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
  _photonLooseIDToken( GetToken<edm::ValueMap<bool> >( "phoLooseIdMap"             ) ),
  _photonMediumIDToken( GetToken<edm::ValueMap<bool> >( "phoMediumIdMap"            ) ),
  _photonTightIDToken( GetToken<edm::ValueMap<bool> >( "phoTightIdMap"             ) ),
  _photonIsolation_Charged_Token( GetToken<edm::ValueMap<float> >( "phoChargedIsolation"       ) ),
  _photonIsolation_Neutral_Token( GetToken<edm::ValueMap<float> >( "phoNeutralHadronIsolation" ) ),
  _photonIsolation_Photon_Token( GetToken<edm::ValueMap<float> >( "phoPhotonIsolation"        ) ),
  _photonSignaIEtaIEtaToken( GetToken<edm::ValueMap<float> >( "full5x5SigmaIEtaIEtaMap"   ) ),
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
  iEvent.getByToken( _photonLooseIDToken,            _photonIDLoose             );
  iEvent.getByToken( _photonMediumIDToken,           _photonIDMedium            );
  iEvent.getByToken( _photonTightIDToken,            _photonIDTight             );
  iEvent.getByToken( _photonIsolation_Charged_Token, _photonIsolation_Charged_H );
  iEvent.getByToken( _photonIsolation_Neutral_Token, _photonIsolation_Neutral_H );
  iEvent.getByToken( _photonIsolation_Photon_Token,  _photonIsolation_Photon_H  );
  iEvent.getByToken( _photonSignaIEtaIEtaToken,      _photonSigmaIEtaIEta_H     );

  memset( &PhotonInfo, 0x00, sizeof( PhotonInfo ) );

  for( auto it_pho = _photonhandle->begin(); it_pho != _photonhandle->end(); it_pho++ ){
    if( PhotonInfo.Size >= MAX_PHOTONS ){
      cerr <<  "ERROR: number of photons exceeds the size of array." << endl;
      break;// exit(0);
    }

    if( it_pho->pt() < 10.  ){ continue; }
    if( fabs( it_pho->eta() ) > 2.4 ){ continue; }

    // ------------------------  Filling in generic information  -------------------------
    PhotonInfo.Pt                   [PhotonInfo.Size] = it_pho->pt();
    PhotonInfo.Eta                  [PhotonInfo.Size] = it_pho->eta();
    PhotonInfo.Phi                  [PhotonInfo.Size] = it_pho->phi();
    PhotonInfo.TrackIso             [PhotonInfo.Size] = it_pho->trackIso();
    PhotonInfo.EcalIso              [PhotonInfo.Size] = it_pho->ecalIso();
    PhotonInfo.HcalIso              [PhotonInfo.Size] = it_pho->hcalIso();
    PhotonInfo.HoverE               [PhotonInfo.Size] = it_pho->hadronicOverEm();
    PhotonInfo.SigmaIetaIeta        [PhotonInfo.Size] = it_pho->sigmaIetaIeta();
    PhotonInfo.hadTowOverEm         [PhotonInfo.Size] = it_pho->hadTowOverEm();
    PhotonInfo.hcalIsoConeDR04_2012 [PhotonInfo.Size]
      = it_pho->hcalTowerSumEtConeDR04() +
        ( it_pho->hadronicOverEm() - it_pho->hadTowOverEm() )
        * it_pho->superCluster()->energy()
        / cosh( it_pho->superCluster()->eta() );
    PhotonInfo.passelectronveto     [PhotonInfo.Size] = (int)it_pho->passElectronVeto();
    PhotonInfo.hasPixelSeed         [PhotonInfo.Size] = it_pho->hasPixelSeed();
    PhotonInfo.r9                   [PhotonInfo.Size] = it_pho->r9();

    // -----------------------  Filling in isolation information  ------------------------
    const edm::Ptr<pat::Photon> pho( _photonhandle, it_pho - _photonhandle->begin() );
    try {
      PhotonInfo.phoPFChIso    [PhotonInfo.Size] = ( *_photonIsolation_Charged_H )[pho];
      PhotonInfo.phoPFPhoIso   [PhotonInfo.Size] = ( *_photonIsolation_Photon_H )[pho];
      PhotonInfo.phoPFNeuIso   [PhotonInfo.Size] = ( *_photonIsolation_Neutral_H )[pho];
      PhotonInfo.sigmaIetaIeta [PhotonInfo.Size] = ( *_photonSigmaIEtaIEta_H )[pho];
      PhotonInfo.phoPassLoose  [PhotonInfo.Size] = ( *_photonIDLoose )[pho];
      PhotonInfo.phoPassMedium [PhotonInfo.Size] = ( *_photonIDMedium )[pho];
      PhotonInfo.phoPassTight  [PhotonInfo.Size] = ( *_photonIDTight )[pho];

      const double rho       = *_rhohandle;
      const double isochhad  = ( *_photonIsolation_Charged_H )[pho];
      const double isopho    = ( *_photonIsolation_Photon_H )[pho];
      const double isonuhad  = ( *_photonIsolation_Neutral_H )[pho];
      const double areachhad = _photonEffectiveArea_ChargeHadron.getEffectiveArea( abs( it_pho->eta() ) );
      const double areapho   = _photonEffectiveArea_Photons.getEffectiveArea( abs( it_pho->eta() ) );
      const double areanuhad = _photonEffectiveArea_NeutralHadron.getEffectiveArea( abs( it_pho->eta() ) );

      PhotonInfo.isoChEffArea  [PhotonInfo.Size] = std::max( 0.0, isochhad - rho* areachhad );
      PhotonInfo.isoPhoEffArea [PhotonInfo.Size] = std::max( 0.0,  isopho - rho * areapho );
      PhotonInfo.isoNeuEffArea [PhotonInfo.Size] = std::max( 0.0, isonuhad - rho*areanuhad );
    } catch( std::exception& e ){
      // cout << "Weird photon found!!" << endl;
    }

    // ----- Generation MC information  ---------------------------------------------
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
