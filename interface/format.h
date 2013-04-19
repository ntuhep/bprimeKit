#ifndef BPRIMEKITFORMAT_H
#define BPRIMEKITFORMAT_H

#define MAX_LEPTONS 	256
#define MAX_TRACKS 	256
#define MAX_JETS 	128
#define MAX_PAIRS 	512
#define MAX_PHOTONS	128
#define MAX_GENS	128
#define MAX_Vertices    256
#define MAX_BX		128
#define N_TRIGGER_BOOKINGS	5842 // from 226 (CMSSW_3_6_1 : 8E29 & 1E31) to 644 (CMSSW_3_8_3 : 2E32) ; 904 (CMSSW_4_1_3 : 5E32); 1212 (CMSSW_4_1_4 : 1E33); 2014 (CMSSW_4_2_3)	; 2390 (CMSSW_4_2_3) ; 2722 (prompt_v5); 2996 (prompt_v6)

#include <TTree.h>

//For CMSSW_3_8_3

class EvtInfoBranches {
	public:
		int   RunNo;
		long long int   EvtNo;
		int   BxNo;
		int   LumiNo;
		int   Orbit;
		int   McFlag;	      	 // MC or not MC, that's the question
		int   McSigTag;     	 // MC Signature tag	 - 0: others, 1: 2L (opposite-sign), 2: 2L (same-sign), 3: 3L, 4: 4L
		int   McbprimeMode[2]; // b'(bar) decay mode   - 0: others, 1: tW, 2: cW, 3: bZ, 4: bH
		int   MctprimeMode[2]; // t'(bar) decay mode   - 0: others, 1: bW, 2: tZ, 3: tH, 4: tgamma
		int   McWMode[4];   // W- from b'(t'bar)/W+ from t/W+ from b'bar(t')/W- from tbar - 0: others, 1: enu, 2: munu, 3: taunu, 4: jj
		int 	McZMode[2];   	 // Z from b'(bar)/t'(t'bar)	 - 0: others, 1: ee, 2: mumu, 3: tautau, 4: nunu, 5: bb, 6: jj
		float McbprimeMass[2]; // mass: b'(bar)  
		float MctprimeMass[2]; // mass: t'(bar)  
		float MctopMass[2];	 // mass: top(bar)
		float McWMass[4];	 // mass: W- from b'(t'bar)/W+ from t/W+ from b'bar(t')/W- from tbar 
		float McZMass[2];	 // mass: Z from b'(bar) or t'(bar)
		float McDauPt[14];	 // Generic MC daughter information
		float McDauEta[14];	 // MC daughters: 0-1: hard jet from b'bar/t'bar, 2-9: W daughters, 10-13: Z daughters 
		float McDauPhi[14];	 
		float RhoPU[2];   // [electron,muon]	 
		float SigmaPU[2];	// [electron,muon] 
		int   McDauPdgID[14];	 
		int   PDFid1;
		int   PDFid2;
		float PDFx1;
		float PDFx2;
		float PDFscale;
		float PDFv1;
		float PDFv2;

		//BeamSpot
		float BeamSpotX;
		float BeamSpotY;
		float BeamSpotZ;

		// PU
		int nBX;
		int nPU[MAX_BX];
		int BXPU[MAX_BX];
		float TrueIT[MAX_BX];

		float PFMET;
		float PFMETPhi;
		float PFRawMET;
		float PFRawMETPhi;
		float PFSumEt;
		float PFMETSig;
		float PFGenMET;
		float PFGenMETPhi;

		float PFMETx; //Uly 2011-04-04
		float PFMETy; //Uly 2011-04-04

		int   TrgCount;	// No. of fired booking bits
		int   nTrgBook;
		char  TrgBook[N_TRIGGER_BOOKINGS];	// Trigger bits, reserved up to 120 entries
		int   nHLT;
		bool  HLTbits[N_TRIGGER_BOOKINGS];
		int L1[128]; // L1 trigger bits
		int TT[64]; 	// Techical trigger bits
		float HighPurityFraction; //Added by Dmitry to help filter out bad events
		int NofTracks; //Added by Dmitry to help filter out bad events


		void RegisterTree(TTree *root) {
			root->Branch("EvtInfo.RunNo"	    , &RunNo	       , "EvtInfo.RunNo/I"	    );
			root->Branch("EvtInfo.EvtNo"	    , &EvtNo	       , "EvtInfo.EvtNo/L"	    );
			root->Branch("EvtInfo.BxNo"	    , &BxNo	           , "EvtInfo.BxNo/I"	    );
			root->Branch("EvtInfo.LumiNo"	    , &LumiNo	       , "EvtInfo.LumiNo/I"	    );
			root->Branch("EvtInfo.Orbit"	    , &Orbit	       , "EvtInfo.Orbit/I"	    );
			root->Branch("EvtInfo.McFlag"	    , &McFlag	       , "EvtInfo.McFlag/I"	    );
			root->Branch("EvtInfo.McSigTag"	    , &McSigTag        , "EvtInfo.McSigTag/I"	    );
			root->Branch("EvtInfo.McbprimeMode" , &McbprimeMode[0] , "EvtInfo.McbprimeMode[2]/I");
			root->Branch("EvtInfo.MctprimeMode" , &MctprimeMode[0] , "EvtInfo.MctprimeMode[2]/I");
			root->Branch("EvtInfo.McWMode"      , &McWMode[0]      , "EvtInfo.McWMode[4]/I"     );
			root->Branch("EvtInfo.McZMode"      , &McZMode[0]      , "EvtInfo.McZMode[2]/I"     );
			root->Branch("EvtInfo.McbprimeMass" , &McbprimeMass[0] , "EvtInfo.McbprimeMass[2]/F");
			root->Branch("EvtInfo.MctprimeMass" , &MctprimeMass[0] , "EvtInfo.MctprimeMass[2]/F");
			root->Branch("EvtInfo.MctopMass"    , &MctopMass[0]    , "EvtInfo.MctopMass[2]/F"   );
			root->Branch("EvtInfo.McWMass"      , &McWMass[0]      , "EvtInfo.McWMass[4]/F"     );
			root->Branch("EvtInfo.McZMass"      , &McZMass[0]      , "EvtInfo.McZMass[2]/F"     );
			root->Branch("EvtInfo.McDauPt"      , &McDauPt[0]      , "EvtInfo.McDauPt[14]/F"    );
			root->Branch("EvtInfo.McDauEta"     , &McDauEta[0]     , "EvtInfo.McDauEta[14]/F"   );
			root->Branch("EvtInfo.McDauPhi"     , &McDauPhi[0]     , "EvtInfo.McDauPhi[14]/F"   );  	 
			root->Branch("EvtInfo.McDauPdgID"   , &McDauPdgID[0]   , "EvtInfo.McDauPdgID[14]/I" );  		 
			root->Branch("EvtInfo.PDFid1"	    , &PDFid1	       , "EvtInfo.PDFid1/I"	    );
			root->Branch("EvtInfo.PDFid2"	    , &PDFid2	       , "EvtInfo.PDFid2/I"	    );
			root->Branch("EvtInfo.PDFx1"	    , &PDFx1	       , "EvtInfo.PDFx1/F"	    );
			root->Branch("EvtInfo.RhoPU"	    , &RhoPU[0]	       , "EvtInfo.RhoPU[2]/F"	    );
			root->Branch("EvtInfo.SigmaPU"	    , &SigmaPU[0]	       , "EvtInfo.SigmaPU[2]/F"	    );
			root->Branch("EvtInfo.PDFx2"	    , &PDFx2	       , "EvtInfo.PDFx2/F"	    );
			root->Branch("EvtInfo.PDFscale"	    , &PDFscale	       , "EvtInfo.PDFscale/F"	    );
			root->Branch("EvtInfo.PDFv1"	    , &PDFv1	       , "EvtInfo.PDFv1/F"	    );
			root->Branch("EvtInfo.PDFv2"	    , &PDFv2	       , "EvtInfo.PDFv2/F"	    );		 

			root->Branch("EvtInfo.BeamSpotX"    , &BeamSpotX       , "EvtInfo.BeamSpotX/F"      );
			root->Branch("EvtInfo.BeamSpotY"    , &BeamSpotY       , "EvtInfo.BeamSpotY/F"      );
			root->Branch("EvtInfo.BeamSpotZ"    , &BeamSpotZ       , "EvtInfo.BeamSpotZ/F"      );
			root->Branch("EvtInfo.PFMET"          , &PFMET             , "EvtInfo.PFMET/F"            );
			root->Branch("EvtInfo.PFMETPhi"       , &PFMETPhi          , "EvtInfo.PFMETPhi/F"         );
			root->Branch("EvtInfo.PFRawMET"       , &PFRawMET          , "EvtInfo.PFRawMET/F"         );
			root->Branch("EvtInfo.PFRawMETPhi"    , &PFRawMETPhi       , "EvtInfo.PFRawMETPhi/F"      );
			root->Branch("EvtInfo.PFSumEt"        , &PFSumEt           , "EvtInfo.PFSumEt/F"          );
			root->Branch("EvtInfo.PFMETSig"       , &PFMETSig          , "EvtInfo.PFMETSig/F"         );
			root->Branch("EvtInfo.PFGenMET"       , &PFGenMET          , "EvtInfo.PFGenMET/F"         );
			root->Branch("EvtInfo.PFGenMETPhi"    , &PFGenMETPhi       , "EvtInfo.PFGenMETPhi/F"      );
			root->Branch("EvtInfo.PFMETx"         , &PFMETx            , "EvtInfo.PFMETx/F"            ); //Uly 2011-04-04
			root->Branch("EvtInfo.PFMETy"         , &PFMETy            , "EvtInfo.PFMETy/F"            ); //Uly 2011-04-04
			root->Branch("EvtInfo.TrgCount"     , &TrgCount        , "EvtInfo.TrgCount/I"	    );
			root->Branch("EvtInfo.nTrgBook"			, &nTrgBook	 		, "EvtInfo.nTrgBook/I");
			root->Branch("EvtInfo.TrgBook"      , &TrgBook[0]         , "EvtInfo.TrgBook[EvtInfo.nTrgBook]/B"    );
			root->Branch("EvtInfo.L1"	        , &L1[0]	       , "EvtInfo.L1[128]/I"	    );
			root->Branch("EvtInfo.TT"	        , &TT[0]	       , "EvtInfo.TT[64]/I"	    );
			root->Branch("EvtInfo.HighPurityFraction"    , &HighPurityFraction       , "EvtInfo.HighPurityFraction/F"      );
			root->Branch("EvtInfo.NofTracks"    , &NofTracks       , "EvtInfo.NofTracks/I"      );
			root->Branch("EvtInfo.nHLT"			, &nHLT	 		, "EvtInfo.nHLT/I");
			root->Branch("EvtInfo.HLTbits"			, HLTbits		, "EvtInfo.HLTbits[EvtInfo.nHLT]/O");
			root->Branch("EvtInfo.nBX"			, &nBX	 		, "EvtInfo.nBX/I");
			root->Branch("EvtInfo.nPU"			, &nPU[0]		, "EvtInfo.nPU[EvtInfo.nBX]/I");
			root->Branch("EvtInfo.BXPU"			, &BXPU[0]		, "EvtInfo.BXPU[EvtInfo.nBX]/I");
			root->Branch("EvtInfo.TrueIT"			, &TrueIT[0]		, "EvtInfo.TrueIT[EvtInfo.nBX]/F");
		}										    



		void Register(TTree *root) {
			root->SetBranchAddress("EvtInfo.RunNo"        , &RunNo  	 );
			root->SetBranchAddress("EvtInfo.EvtNo"        , &EvtNo  	 );
			root->SetBranchAddress("EvtInfo.BxNo"         , &BxNo   	 );
			root->SetBranchAddress("EvtInfo.LumiNo"       , &LumiNo  	 );
			root->SetBranchAddress("EvtInfo.Orbit"       , &Orbit  	 );
			root->SetBranchAddress("EvtInfo.McFlag"       , &McFlag 	 );
			root->SetBranchAddress("EvtInfo.McSigTag"     , &McSigTag	 );
			root->SetBranchAddress("EvtInfo.McbprimeMode" , &McbprimeMode[0] );
			root->SetBranchAddress("EvtInfo.MctprimeMode" , &MctprimeMode[0] );
			root->SetBranchAddress("EvtInfo.McWMode"      , &McWMode[0]	 );
			root->SetBranchAddress("EvtInfo.McZMode"      , &McZMode[0]	 );
			root->SetBranchAddress("EvtInfo.McbprimeMass" , &McbprimeMass[0] );
			root->SetBranchAddress("EvtInfo.MctprimeMass" , &MctprimeMass[0] );
			root->SetBranchAddress("EvtInfo.MctopMass"    , &MctopMass[0]    );
			root->SetBranchAddress("EvtInfo.McWMass"      , &McWMass[0]	 );
			root->SetBranchAddress("EvtInfo.McZMass"      , &McZMass[0]	 );
			root->SetBranchAddress("EvtInfo.McDauPt"      , &McDauPt[0]	 );
			root->SetBranchAddress("EvtInfo.McDauEta"     , &McDauEta[0]	 );
			root->SetBranchAddress("EvtInfo.McDauPhi"     , &McDauPhi[0]	 );	      
			root->SetBranchAddress("EvtInfo.McDauPdgID"   , &McDauPdgID[0]   );		      
			root->SetBranchAddress("EvtInfo.PDFid1"       , &PDFid1 	 );
			root->SetBranchAddress("EvtInfo.PDFid2"       , &PDFid2 	 );
			root->SetBranchAddress("EvtInfo.PDFx1"        , &PDFx1  	 );
			root->SetBranchAddress("EvtInfo.RhoPU"        , &RhoPU[0]  	 );
			root->SetBranchAddress("EvtInfo.SigmaPU"        , &SigmaPU[0]  	 );
			root->SetBranchAddress("EvtInfo.PDFx2"        , &PDFx2  	 );
			root->SetBranchAddress("EvtInfo.PDFscale"     , &PDFscale	 );
			root->SetBranchAddress("EvtInfo.PDFv1"        , &PDFv1  	 );
			root->SetBranchAddress("EvtInfo.PDFv2"        , &PDFv2  	 );	      

			root->SetBranchAddress("EvtInfo.BeamSpotX"    , &BeamSpotX	 );
			root->SetBranchAddress("EvtInfo.BeamSpotY"    , &BeamSpotY	 );
			root->SetBranchAddress("EvtInfo.BeamSpotZ"    , &BeamSpotZ	 );

			root->SetBranchAddress("EvtInfo.PFMET"          , &PFMET             );
			root->SetBranchAddress("EvtInfo.PFMETPhi"       , &PFMETPhi          );
			root->SetBranchAddress("EvtInfo.PFRawMET"       , &PFRawMET          );
			root->SetBranchAddress("EvtInfo.PFRawMETPhi"    , &PFRawMETPhi       );
			root->SetBranchAddress("EvtInfo.PFSumEt"        , &PFSumEt           );
			root->SetBranchAddress("EvtInfo.PFMETSig"       , &PFMETSig          );
			root->SetBranchAddress("EvtInfo.PFGenMET"       , &PFGenMET          );
			root->SetBranchAddress("EvtInfo.PFGenMETPhi"    , &PFGenMETPhi       );
			root->SetBranchAddress("EvtInfo.PFMETx"         , &PFMETx            ); //Uly 2011-04-04
			root->SetBranchAddress("EvtInfo.PFMETy"         , &PFMETy            ); //Uly 2011-04-04

			root->SetBranchAddress("EvtInfo.TrgCount"     , &TrgCount	 );
			root->SetBranchAddress("EvtInfo.nTrgBook"			, &nTrgBook	);
			root->SetBranchAddress("EvtInfo.TrgBook"      , TrgBook	 );
			root->SetBranchAddress("EvtInfo.L1"           , &L1[0]  	 );
			root->SetBranchAddress("EvtInfo.TT"           , &TT[0]  	 );
			root->SetBranchAddress("EvtInfo.HighPurityFraction"   , &HighPurityFraction	 );
			root->SetBranchAddress("EvtInfo.NofTracks"    , &NofTracks	 );
			root->SetBranchAddress("EvtInfo.nHLT"			, &nHLT	);
			root->SetBranchAddress("EvtInfo.HLTbits"			, HLTbits);
			root->SetBranchAddress("EvtInfo.nBX"			, &nBX);
			root->SetBranchAddress("EvtInfo.nPU"			, &nPU[0]);
			root->SetBranchAddress("EvtInfo.BXPU"			, &BXPU[0]);
			root->SetBranchAddress("EvtInfo.TrueIT"			, &TrueIT[0]);
		}  										    
};


class LepInfoBranches {
	public:
		int	Size; 
		int	Index[MAX_LEPTONS];
		int	isEcalDriven[MAX_LEPTONS];
		int	isTrackerDriven[MAX_LEPTONS];
		int	LeptonType[MAX_LEPTONS];
		int	Charge[MAX_LEPTONS];
		int	ChargeGsf[MAX_LEPTONS];
		int	ChargeCtf[MAX_LEPTONS];
		int	ChargeScPix[MAX_LEPTONS];
		float Pt[MAX_LEPTONS];
		float Et[MAX_LEPTONS];
		float Eta[MAX_LEPTONS];
		float caloEta[MAX_LEPTONS];
		float Phi[MAX_LEPTONS];
		float TrackIso[MAX_LEPTONS];
		float EcalIso[MAX_LEPTONS];
		float HcalIso[MAX_LEPTONS];
		float HcalDepth1Iso[MAX_LEPTONS];
		float HcalDepth2Iso[MAX_LEPTONS];
		float ChargedHadronIso[MAX_LEPTONS];
		float NeutralHadronIso[MAX_LEPTONS];
		float PhotonIso[MAX_LEPTONS];

		float ChargedHadronIsoR03[MAX_LEPTONS];
		float NeutralHadronIsoR03[MAX_LEPTONS];
		float PhotonIsoR03[MAX_LEPTONS];
		float sumPUPtR03[MAX_LEPTONS];
		float IsoRhoCorrR03[MAX_LEPTONS];
		float ChargedHadronIsoR04[MAX_LEPTONS];
		float NeutralHadronIsoR04[MAX_LEPTONS];
		float PhotonIsoR04[MAX_LEPTONS];
		float sumPUPtR04[MAX_LEPTONS];
		float IsoRhoCorrR04[MAX_LEPTONS];

		float Ip3dPV[MAX_LEPTONS];
		float Ip3dPVErr[MAX_LEPTONS];
		float Ip3dPVSignificance[MAX_LEPTONS];

		int   MuontimenDof[MAX_LEPTONS];
		float MuontimeAtIpInOut[MAX_LEPTONS];
		float MuontimeAtIpOutIn[MAX_LEPTONS];
		// enum Direction { OutsideIn = -1, Undefined = 0, InsideOut = 1 };
		int   Muondirection[MAX_LEPTONS];

		float CaloEnergy[MAX_LEPTONS];
		float e1x5[MAX_LEPTONS];
		float e2x5Max[MAX_LEPTONS];
		float e5x5[MAX_LEPTONS];

		float Px[MAX_LEPTONS]; //Uly 2011-04-04
		float Py[MAX_LEPTONS]; //Uly 2011-04-04
		float Pz[MAX_LEPTONS]; //Uly 2011-04-04
		float Energy[MAX_LEPTONS]; //Uly 2011-04-04
		bool isGoodMuonTMOneStationTight[MAX_LEPTONS]; // For Soft Muon
		float innerTracknormalizedChi2[MAX_LEPTONS];   // For Soft Muon

		float vertexZ[MAX_LEPTONS]; //Uly 2011-04-04

		bool  isPFMuon[MAX_LEPTONS];
		bool  MuIDGlobalMuonPromptTight[MAX_LEPTONS];

		float MuInnerPtError[MAX_LEPTONS];  
		float MuGlobalPtError[MAX_LEPTONS];  
		float MuInnerTrackDz[MAX_LEPTONS];  
		float MuInnerTrackD0[MAX_LEPTONS];  
		float MuInnerTrackDxy_BS[MAX_LEPTONS];  
		float MuInnerTrackDxy_PV[MAX_LEPTONS];  
		float MuInnerTrackDxy_PVBS[MAX_LEPTONS];  
		int   MuInnerTrackNHits[MAX_LEPTONS];
		int   MuNTrackerHits[MAX_LEPTONS];

		float MuGlobalNormalizedChi2[MAX_LEPTONS]; // Dmitry

		float MuCaloCompat[MAX_LEPTONS];
		int   MuNChambers[MAX_LEPTONS];
		int   MuNChambersMatchesSegment[MAX_LEPTONS];
		int   MuNMatchedStations[MAX_LEPTONS];
		int   MuNPixelLayers[MAX_LEPTONS];
		int   MuNPixelLayersWMeasurement[MAX_LEPTONS]; //Uly 2011-04-04
		int   MuNTrackLayersWMeasurement[MAX_LEPTONS];
		int   MuNLostInnerHits[MAX_LEPTONS];
		int   MuNLostOuterHits[MAX_LEPTONS];
		int   MuNMuonhits[MAX_LEPTONS];
		int   MuDThits[MAX_LEPTONS];
		int   MuCSChits[MAX_LEPTONS];
		int   MuRPChits[MAX_LEPTONS];
		int   MuType[MAX_LEPTONS];

		float EgammaMVANonTrig[MAX_LEPTONS]; // Add by Jacky
		float EgammaMVATrig[MAX_LEPTONS]; // Add by Jacky
		bool EgammaCutBasedEleIdTRIGGERTIGHT[MAX_LEPTONS]; // Add by Jacky
		bool EgammaCutBasedEleIdTRIGGERWP70[MAX_LEPTONS]; // Add by Jacky
		bool EgammaCutBasedEleIdVETO[MAX_LEPTONS]; // Add by Jacky
		bool EgammaCutBasedEleIdLOOSE[MAX_LEPTONS]; // Add by Jacky
		bool EgammaCutBasedEleIdMEDIUM[MAX_LEPTONS]; // Add by Jacky
		bool EgammaCutBasedEleIdTIGHT[MAX_LEPTONS]; // Add by Jacky

		float Eldr03HcalDepth1TowerSumEtBc[MAX_LEPTONS];
		float Eldr03HcalDepth2TowerSumEtBc[MAX_LEPTONS];
		float Eldr04HcalDepth1TowerSumEtBc[MAX_LEPTONS];
		float Eldr04HcalDepth2TowerSumEtBc[MAX_LEPTONS];
		float ElhcalOverEcalBc[MAX_LEPTONS];
		float ElEcalE[MAX_LEPTONS];
		float ElEoverP[MAX_LEPTONS];
		float EldeltaEta[MAX_LEPTONS];
		float EldeltaPhi[MAX_LEPTONS]; 
		float ElHadoverEm[MAX_LEPTONS];
		float ElsigmaIetaIeta[MAX_LEPTONS];	// Jacky
		float ElscSigmaIetaIeta[MAX_LEPTONS];	// Jacky
		float ElEnergyErr[MAX_LEPTONS];
		float ElMomentumErr[MAX_LEPTONS];
		int	ElTrackNHits[MAX_LEPTONS]; //Dmitry
		float ElSharedHitsFraction[MAX_LEPTONS]; //Dmitry
		float dR_gsf_ctfTrack[MAX_LEPTONS]; //Dmitry
		float dPt_gsf_ctfTrack[MAX_LEPTONS]; //Dmitry
		bool	ElhasConv[MAX_LEPTONS];

		float ElTrackNLostHits[MAX_LEPTONS];  //yjlei
		float ElTrackDz[MAX_LEPTONS];  
		float ElTrackDz_BS[MAX_LEPTONS];  
		float ElTrackD0[MAX_LEPTONS];  
		float ElTrackDxy_BS[MAX_LEPTONS];  
		float ElTrackDxy_PV[MAX_LEPTONS];  
		float ElTrackDxy_PVBS[MAX_LEPTONS]; //yjlei
		int	ElNClusters[MAX_LEPTONS];
		int	ElClassification[MAX_LEPTONS];
		float	ElFBrem[MAX_LEPTONS];
		int NumberOfExpectedInnerHits[MAX_LEPTONS]; // Add by Jacky
		float Eldist[MAX_LEPTONS]; // Add by Jacky
		float Eldcot[MAX_LEPTONS]; // Add by Jacky
		float Elconvradius[MAX_LEPTONS]; // Add by Jacky
		float ElConvPoint_x[MAX_LEPTONS]; // Add by Jacky
		float ElConvPoint_y[MAX_LEPTONS]; // Add by Jacky
		float ElConvPoint_z[MAX_LEPTONS]; // Add by Jacky

		// For CIC	
		float dcotdist[MAX_LEPTONS];
		float ElseedEoverP[MAX_LEPTONS];
		float ElEcalIso04[MAX_LEPTONS];
		float ElHcalIso04[MAX_LEPTONS];

		int	ElNumberOfBrems[MAX_LEPTONS];
		float GenPt[MAX_LEPTONS];
		float GenEta[MAX_LEPTONS];
		float GenPhi[MAX_LEPTONS];
		int	GenPdgID[MAX_LEPTONS];
		int	GenMCTag[MAX_LEPTONS]; 	// 0: unknown, 1: decay from W, 2: decay from Z, 
		// 3: from b, 4: from c, 5: match to a parton (q or g), 6: match to a photon
		// (+10) from b'
		// (+20) from t'
		float TrgPt[MAX_LEPTONS];
		float TrgEta[MAX_LEPTONS];
		float TrgPhi[MAX_LEPTONS];
		int TrgID[MAX_LEPTONS];

		//tau : hpsPFTau ID  
		int isPFTau         [MAX_LEPTONS];    // YoungKyu 2012-10-16
		float decayModeFinding[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float byVLooseCombinedIsolationDeltaBetaCorr[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float byLooseCombinedIsolationDeltaBetaCorr[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float byMediumCombinedIsolationDeltaBetaCorr[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float byTightCombinedIsolationDeltaBetaCorr[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float againstElectronLoose[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float againstElectronMedium[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float againstElectronTight[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float againstElectronMVA[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float againstMuonLoose[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float againstMuonMedium[MAX_LEPTONS]; // YoungKyu 2012-10-31
		float againstMuonTight[MAX_LEPTONS]; // YoungKyu 2012-10-31





#ifdef __BPRIMEKIT__
		reco::Candidate* CandRef[MAX_LEPTONS]; // backward pointer to the PAT objects
#endif

		void RegisterTree(TTree *root, std::string name="LepInfo") {
			root->Branch((name+".Size").c_str()	     		 , &Size	  		 , (name+".Size/I").c_str()			     	       );
			root->Branch((name+".Index").c_str()	     		 , &Index[0]	  		 , (name+".Index["+name+".Size]/I").c_str()	     	       );
			root->Branch((name+".isEcalDriven").c_str()	     	 , &isEcalDriven[0]  		 , (name+".isEcalDriven["+name+".Size]/I").c_str()	     	       );
			root->Branch((name+".isTrackerDriven").c_str()	     	 , &isTrackerDriven[0]  	 , (name+".isTrackerDriven["+name+".Size]/I").c_str()	     	       );
			root->Branch((name+".LeptonType").c_str()    		 , &LeptonType[0] 		 , (name+".LeptonType["+name+".Size]/I").c_str()    	       );
			root->Branch((name+".Charge").c_str()	     		 , &Charge[0]	  		 , (name+".Charge["+name+".Size]/I").c_str()	     	       );
			root->Branch((name+".ChargeGsf").c_str()	     		 , &ChargeGsf[0]	  		 , (name+".ChargeGsf["+name+".Size]/I").c_str()	     	       );
			root->Branch((name+".ChargeCtf").c_str()	     		 , &ChargeCtf[0]	  		 , (name+".ChargeCtf["+name+".Size]/I").c_str()	     	       );
			root->Branch((name+".ChargeScPix").c_str()	     		 , &ChargeScPix[0]	  		 , (name+".ChargeScPix["+name+".Size]/I").c_str()	     	       );
			root->Branch((name+".Pt").c_str()	     		 , &Pt[0]	  		 , (name+".Pt["+name+".Size]/F").c_str()      		       );
			root->Branch((name+".Et").c_str()	     		 , &Et[0]	  		 , (name+".Et["+name+".Size]/F").c_str()      		       );
			root->Branch((name+".Eta").c_str()	     		 , &Eta[0]	  		 , (name+".Eta["+name+".Size]/F").c_str()     		       );
			root->Branch((name+".caloEta").c_str()	     		 , &caloEta[0]	  		 , (name+".caloEta["+name+".Size]/F").c_str()     		       );
			root->Branch((name+".Phi").c_str()	     		 , &Phi[0]	  		 , (name+".Phi["+name+".Size]/F").c_str()     		       );
			root->Branch((name+".TrackIso").c_str()      		 , &TrackIso[0]   		 , (name+".TrackIso["+name+".Size]/F").c_str()	   	       );
			root->Branch((name+".EcalIso").c_str()       		 , &EcalIso[0]    		 , (name+".EcalIso["+name+".Size]/F").c_str()	     	       );   
			root->Branch((name+".HcalIso").c_str()       		 , &HcalIso[0]    		 , (name+".HcalIso["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".HcalDepth1Iso").c_str()       	 , &HcalDepth1Iso[0]   		 , (name+".HcalDepth1Iso["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".HcalDepth2Iso").c_str()       	 , &HcalDepth2Iso[0]   		 , (name+".HcalDepth2Iso["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".ChargedHadronIso").c_str()       	 , &ChargedHadronIso[0]   		 , (name+".ChargedHadronIso["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".NeutralHadronIso").c_str()       	 , &NeutralHadronIso[0]   		 , (name+".NeutralHadronIso["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".PhotonIso").c_str()       	 , &PhotonIso[0]   		 , (name+".PhotonIso["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".ChargedHadronIsoR03").c_str()       	 , &ChargedHadronIsoR03[0]   		 , (name+".ChargedHadronIsoR03["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".NeutralHadronIsoR03").c_str()       	 , &NeutralHadronIsoR03[0]   		 , (name+".NeutralHadronIsoR03["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".PhotonIsoR03").c_str()       	 , &PhotonIsoR03[0]   		 , (name+".PhotonIsoR03["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".sumPUPtR03").c_str()       	 , &sumPUPtR03[0]   		 , (name+".sumPUPtR03["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".IsoRhoCorrR03").c_str()       	 , &IsoRhoCorrR03[0]   		 , (name+".IsoRhoCorrR03["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".ChargedHadronIsoR04").c_str()       	 , &ChargedHadronIsoR04[0]   		 , (name+".ChargedHadronIsoR04["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".NeutralHadronIsoR04").c_str()       	 , &NeutralHadronIsoR04[0]   		 , (name+".NeutralHadronIsoR04["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".PhotonIsoR04").c_str()       	 , &PhotonIsoR04[0]   		 , (name+".PhotonIsoR04["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".sumPUPtR04").c_str()       	 , &sumPUPtR04[0]   		 , (name+".sumPUPtR04["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".IsoRhoCorrR04").c_str()       	 , &IsoRhoCorrR04[0]   		 , (name+".IsoRhoCorrR04["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".Ip3dPV").c_str()       	 , &Ip3dPV[0]   		 , (name+".Ip3dPV["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".Ip3dPVErr").c_str()       	 , &Ip3dPVErr[0]   		 , (name+".Ip3dPVErr["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".Ip3dPVSignificance").c_str()       	 , &Ip3dPVSignificance[0]   		 , (name+".Ip3dPVSignificance["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".MuontimenDof").c_str()       	 , &MuontimenDof[0]   		 , (name+".MuontimenDof["+name+".Size]/I").c_str()	     	       ); 
			root->Branch((name+".MuontimeAtIpInOut").c_str()       	 , &MuontimeAtIpInOut[0]   		 , (name+".MuontimeAtIpInOut["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".MuontimeAtIpOutIn").c_str()       	 , &MuontimeAtIpOutIn[0]   		 , (name+".MuontimeAtIpOutIn["+name+".Size]/F").c_str()	     	       ); 
			root->Branch((name+".Muondirection").c_str()       	 , &Muondirection[0]   		 , (name+".Muondirection["+name+".Size]/I").c_str()	     	       ); 

			root->Branch((name+".CaloEnergy").c_str()    	     	 , &CaloEnergy[0]	  	 , (name+".CaloEnergy["+name+".Size]/F").c_str()    	       );
			root->Branch((name+".e1x5").c_str()    	         , &e1x5[0]	  	         , (name+".e1x5["+name+".Size]/F").c_str()    	       );
			root->Branch((name+".e2x5Max").c_str()    	     	 , &e2x5Max[0]	  	         , (name+".e2x5Max["+name+".Size]/F").c_str()    	       );
			root->Branch((name+".e5x5").c_str()    	         , &e5x5[0]	  	         , (name+".e5x5["+name+".Size]/F").c_str()    	       );

			root->Branch((name+".Px").c_str()	     		 , &Px[0]	  		 , (name+".Px["+name+".Size]/F").c_str()      		       ); //Uly 2011-04-04
			root->Branch((name+".Py").c_str()	     		 , &Py[0]	  		 , (name+".Py["+name+".Size]/F").c_str()      		       ); //Uly 2011-04-04
			root->Branch((name+".Pz").c_str()	     		 , &Pz[0]	  		 , (name+".Pz["+name+".Size]/F").c_str()      		       ); //Uly 2011-04-04
			root->Branch((name+".Energy").c_str()	     		 , &Energy[0]	  		 , (name+".Energy["+name+".Size]/F").c_str()    		       ); //Uly 2011-04-04
			root->Branch((name+".innerTracknormalizedChi2").c_str()	     		 , &innerTracknormalizedChi2[0]	  		 , (name+".innerTracknormalizedChi2["+name+".Size]/F").c_str()    		       ); 

			root->Branch((name+".vertexZ").c_str()    	         , &vertexZ[0]	  	         , (name+".vertexZ["+name+".Size]/F").c_str()    	       ); //Uly 2011-04-04


			root->Branch((name+".MuIDGlobalMuonPromptTight").c_str()                   , &MuIDGlobalMuonPromptTight[0]                   , (name+".MuIDGlobalMuonPromptTight["+name+".Size]/O").c_str()                   );  // Add by Jacky
			root->Branch((name+".isPFMuon").c_str()                   , &isPFMuon[0]                   , (name+".isPFMuon["+name+".Size]/O").c_str()                   );  // Add by Jacky

			root->Branch((name+".MuInnerPtError").c_str()            , &MuInnerPtError[0]            , (name+".MuInnerPtError["+name+".Size]/F").c_str()            );
			root->Branch((name+".MuGlobalPtError").c_str()            , &MuGlobalPtError[0]            , (name+".MuGlobalPtError["+name+".Size]/F").c_str()            );
			root->Branch((name+".MuInnerTrackDz").c_str()            , &MuInnerTrackDz[0]            , (name+".MuInnerTrackDz["+name+".Size]/F").c_str()            );
			root->Branch((name+".MuInnerTrackD0").c_str()            , &MuInnerTrackD0[0]            , (name+".MuInnerTrackD0["+name+".Size]/F").c_str()            );
			root->Branch((name+".MuInnerTrackDxy_BS").c_str()        , &MuInnerTrackDxy_BS[0]        , (name+".MuInnerTrackDxy_BS["+name+".Size]/F").c_str()            );
			root->Branch((name+".MuInnerTrackDxy_PV").c_str()        , &MuInnerTrackDxy_PV[0]        , (name+".MuInnerTrackDxy_PV["+name+".Size]/F").c_str()            );
			root->Branch((name+".MuInnerTrackDxy_PVBS").c_str()      , &MuInnerTrackDxy_PVBS[0]      , (name+".MuInnerTrackDxy_PVBS["+name+".Size]/F").c_str()            );
			root->Branch((name+".MuInnerTrackNHits").c_str()         , &MuInnerTrackNHits[0]         , (name+".MuInnerTrackNHits["+name+".Size]/I").c_str()         );	
			root->Branch((name+".MuNTrackerHits").c_str()            , &MuNTrackerHits[0]            , (name+".MuNTrackerHits["+name+".Size]/I").c_str()         );	
			root->Branch((name+".MuCaloCompat").c_str()              , &MuCaloCompat[0]              , (name+".MuCaloCompat["+name+".Size]/F").c_str()              );
			root->Branch((name+".MuNChambers").c_str()               , &MuNChambers[0] 	         , (name+".MuNChambers["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuNChambersMatchesSegment").c_str() , &MuNChambersMatchesSegment[0] , (name+".MuNChambersMatchesSegment["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuNMatchedStations").c_str() , &MuNMatchedStations[0] , (name+".MuNMatchedStations["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuNPixelLayers").c_str()            , &MuNPixelLayers[0]            , (name+".MuNPixelLayers["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuNPixelLayersWMeasurement").c_str(), &MuNPixelLayersWMeasurement[0], (name+".MuNPixelLayersWMeasurement["+name+".Size]/I").c_str()       ); //Uly 2011-04-04
			root->Branch((name+".MuNTrackLayersWMeasurement").c_str(), &MuNTrackLayersWMeasurement[0], (name+".MuNTrackLayersWMeasurement["+name+".Size]/I").c_str()       ); 
			root->Branch((name+".MuNLostInnerHits").c_str()          , &MuNLostInnerHits[0]          , (name+".MuNLostInnerHits["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuNLostOuterHits").c_str()          , &MuNLostOuterHits[0]          , (name+".MuNLostOuterHits["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuNMuonhits").c_str()               , &MuNMuonhits[0]               , (name+".MuNMuonhits["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuDThits").c_str()               , &MuDThits[0]               , (name+".MuDThits["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuCSChits").c_str()               , &MuCSChits[0]               , (name+".MuCSChits["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuRPChits").c_str()               , &MuRPChits[0]               , (name+".MuRPChits["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".MuType").c_str()	                 , &MuType[0]	                 , (name+".MuType["+name+".Size]/I").c_str()        	       );

			root->Branch((name+".MuGlobalNormalizedChi2").c_str()    , &MuGlobalNormalizedChi2[0] 	 , (name+".MuGlobalNormalizedChi2["+name+".Size]/F").c_str()    ); //Dmitry

			root->Branch((name+".ElTrackNLostHits").c_str()           , &ElTrackNLostHits[0]	         , (name+".ElTrackNLostHits["+name+".Size]/F").c_str()	       );  //yjlei
			root->Branch((name+".ElTrackDz").c_str()            , &ElTrackDz[0]            , (name+".ElTrackDz["+name+".Size]/F").c_str()            );
			root->Branch((name+".ElTrackDz_BS").c_str()            , &ElTrackDz_BS[0]            , (name+".ElTrackDz_BS["+name+".Size]/F").c_str()            );
			root->Branch((name+".ElTrackD0").c_str()            , &ElTrackD0[0]            , (name+".ElTrackD0["+name+".Size]/F").c_str()            );
			root->Branch((name+".ElTrackDxy_BS").c_str()            , &ElTrackDxy_BS[0]            , (name+".ElTrackDxy_BS["+name+".Size]/F").c_str()            );
			root->Branch((name+".ElTrackDxy_PV").c_str()            , &ElTrackDxy_PV[0]            , (name+".ElTrackDxy_PV["+name+".Size]/F").c_str()            );
			root->Branch((name+".ElTrackDxy_PVBS").c_str()            , &ElTrackDxy_PVBS[0]            , (name+".ElTrackDxy_PVBS["+name+".Size]/F").c_str()            );

			root->Branch((name+".EgammaMVANonTrig").c_str()                   , &EgammaMVANonTrig[0]                   , (name+".EgammaMVANonTrig["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".EgammaMVATrig").c_str()                   , &EgammaMVATrig[0]                   , (name+".EgammaMVATrig["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".isGoodMuonTMOneStationTight").c_str()                   , &isGoodMuonTMOneStationTight[0]                   , (name+".isGoodMuonTMOneStationTight["+name+".Size]/O").c_str()                  );  // Add by Jacky
			root->Branch((name+".EgammaCutBasedEleIdTRIGGERTIGHT").c_str()                   , &EgammaCutBasedEleIdTRIGGERTIGHT[0]                   , (name+".EgammaCutBasedEleIdTRIGGERTIGHT["+name+".Size]/O").c_str()                  );  // Add by Jacky
			root->Branch((name+".EgammaCutBasedEleIdTRIGGERWP70").c_str()                   , &EgammaCutBasedEleIdTRIGGERWP70[0]                   , (name+".EgammaCutBasedEleIdTRIGGERWP70["+name+".Size]/O").c_str()                  );  // Add by Jacky
			root->Branch((name+".EgammaCutBasedEleIdVETO").c_str()                   , &EgammaCutBasedEleIdVETO[0]                   , (name+".EgammaCutBasedEleIdVETO["+name+".Size]/O").c_str()                  );  // Add by Jacky
			root->Branch((name+".EgammaCutBasedEleIdLOOSE").c_str()                   , &EgammaCutBasedEleIdLOOSE[0]                   , (name+".EgammaCutBasedEleIdLOOSE["+name+".Size]/O").c_str()                  );  // Add by Jacky
			root->Branch((name+".EgammaCutBasedEleIdMEDIUM").c_str()                   , &EgammaCutBasedEleIdMEDIUM[0]                   , (name+".EgammaCutBasedEleIdMEDIUM["+name+".Size]/O").c_str()                  );  // Add by Jacky
			root->Branch((name+".EgammaCutBasedEleIdTIGHT").c_str()                   , &EgammaCutBasedEleIdTIGHT[0]                   , (name+".EgammaCutBasedEleIdTIGHT["+name+".Size]/O").c_str()                  );  // Add by Jacky

			root->Branch((name+".Eldr03HcalDepth1TowerSumEtBc").c_str()         	 , &Eldr03HcalDepth1TowerSumEtBc[0]      		 , (name+".Eldr03HcalDepth1TowerSumEtBc["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".Eldr03HcalDepth2TowerSumEtBc").c_str()         	 , &Eldr03HcalDepth2TowerSumEtBc[0]      		 , (name+".Eldr03HcalDepth2TowerSumEtBc["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".Eldr04HcalDepth1TowerSumEtBc").c_str()         	 , &Eldr04HcalDepth1TowerSumEtBc[0]      		 , (name+".Eldr04HcalDepth1TowerSumEtBc["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".Eldr04HcalDepth2TowerSumEtBc").c_str()         	 , &Eldr04HcalDepth2TowerSumEtBc[0]      		 , (name+".Eldr04HcalDepth2TowerSumEtBc["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".ElhcalOverEcalBc").c_str()         	 , &ElhcalOverEcalBc[0]      		 , (name+".ElhcalOverEcalBc["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".ElEcalE").c_str()         	 , &ElEcalE[0]      		 , (name+".ElEcalE["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".ElEoverP").c_str()         	 , &ElEoverP[0]      		 , (name+".ElEoverP["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".EldeltaEta").c_str()       	 , &EldeltaEta[0]    		 , (name+".EldeltaEta["+name+".Size]/F").c_str()     	       );
			root->Branch((name+".EldeltaPhi").c_str()       	 , &EldeltaPhi[0]    		 , (name+".EldeltaPhi["+name+".Size]/F").c_str()     	       );
			root->Branch((name+".ElHadoverEm").c_str()      	 , &ElHadoverEm[0]   		 , (name+".ElHadoverEm["+name+".Size]/F").c_str()    	       );
			root->Branch((name+".ElsigmaIetaIeta").c_str()      	 , &ElsigmaIetaIeta[0] 		 , (name+".ElsigmaIetaIeta["+name+".Size]/F").c_str()   	       ); // Jacky
			root->Branch((name+".ElscSigmaIetaIeta").c_str()      	 , &ElscSigmaIetaIeta[0] 	 , (name+".ElscSigmaIetaIeta["+name+".Size]/F").c_str()   	       ); // Jacky
			root->Branch((name+".ElEnergyErr").c_str()      	 , &ElEnergyErr[0]   		 , (name+".ElEnergyErr["+name+".Size]/F").c_str()    	       );
			root->Branch((name+".ElMomentumErr").c_str()    	 , &ElMomentumErr[0] 		 , (name+".ElMomentumErr["+name+".Size]/F").c_str()  	       );
			root->Branch((name+".ElTrackNHits").c_str()     	 , &ElTrackNHits[0]  		 , (name+".ElTrackNHits["+name+".Size]/I").c_str()   	       );
			root->Branch((name+".ElSharedHitsFraction").c_str()      , &ElSharedHitsFraction[0]      , (name+".ElSharedHitsFraction["+name+".Size]/F").c_str()              ); //Dmitry
			root->Branch((name+".dR_gsf_ctfTrack").c_str()      , &dR_gsf_ctfTrack[0]      , (name+".dR_gsf_ctfTrack["+name+".Size]/F").c_str()              ); //Dmitry
			root->Branch((name+".dPt_gsf_ctfTrack").c_str()      , &dPt_gsf_ctfTrack[0]      , (name+".dPt_gsf_ctfTrack["+name+".Size]/F").c_str()              ); //Dmitry
			root->Branch((name+".ElNClusters").c_str()      	 , &ElNClusters[0]   		 , (name+".ElNClusters["+name+".Size]/I").c_str()    	       );
			root->Branch((name+".ElClassification").c_str()      	 , &ElClassification[0]   	 , (name+".ElClassification["+name+".Size]/I").c_str()          );

			root->Branch((name+".ElFBrem").c_str()  	    	 , &ElFBrem[0]  	 	 , (name+".ElFBrem["+name+".Size]/F").c_str()       	      ); //Jacky
			root->Branch((name+".ElNumberOfBrems").c_str()      	 , &ElNumberOfBrems[0]   	 , (name+".ElNumberOfBrems["+name+".Size]/I").c_str()          ); //Jacky
			root->Branch((name+".NumberOfExpectedInnerHits").c_str()                   , &NumberOfExpectedInnerHits[0]                   , (name+".NumberOfExpectedInnerHits["+name+".Size]/I").c_str()                  );  // Add by Jacky
			root->Branch((name+".Eldist").c_str()                   , &Eldist[0]                   , (name+".Eldist["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".Eldcot").c_str()                   , &Eldcot[0]                   , (name+".Eldcot["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".Elconvradius").c_str()                   , &Elconvradius[0]                   , (name+".Elconvradius["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".ElConvPoint_x").c_str()                   , &ElConvPoint_x[0]                   , (name+".ElConvPoint_x["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".ElConvPoint_y").c_str()                   , &ElConvPoint_y[0]                   , (name+".ElConvPoint_y["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".ElConvPoint_z").c_str()                   , &ElConvPoint_z[0]                   , (name+".ElConvPoint_z["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".ElhasConv").c_str()                   , &ElhasConv[0]                   , (name+".ElhasConv["+name+".Size]/O").c_str()                  );  // Add by Jacky
			root->Branch((name+".dcotdist").c_str()                   , &dcotdist[0]                   , (name+".dcotdist["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".ElseedEoverP").c_str()                   , &ElseedEoverP[0]                   , (name+".ElseedEoverP["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".ElEcalIso04").c_str()                   , &ElEcalIso04[0]                   , (name+".ElEcalIso04["+name+".Size]/F").c_str()                  );  // Add by Jacky
			root->Branch((name+".ElHcalIso04").c_str()                   , &ElHcalIso04[0]                   , (name+".ElHcalIso04["+name+".Size]/F").c_str()                  );  // Add by Jacky

			root->Branch((name+".GenPt").c_str()	        	 , &GenPt[0]	     		 , (name+".GenPt["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".GenEta").c_str()	        	 , &GenEta[0]	     		 , (name+".GenEta["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".GenPhi").c_str()	        	 , &GenPhi[0]	     		 , (name+".GenPhi["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".GenPdgID").c_str()         	 , &GenPdgID[0]      		 , (name+".GenPdgID["+name+".Size]/I").c_str()	    	       );
			root->Branch((name+".GenMCTag").c_str()         	 , &GenMCTag[0]      		 , (name+".GenMCTag["+name+".Size]/I").c_str()	    	       );
			root->Branch((name+".TrgPt").c_str()	        	 , &TrgPt[0]	     		 , (name+".TrgPt["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".TrgEta").c_str()	        	 , &TrgEta[0]	     		 , (name+".TrgEta["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".TrgPhi").c_str()	        	 , &TrgPhi[0]	     		 , (name+".TrgPhi["+name+".Size]/F").c_str()	    	       );
			root->Branch((name+".TrgID").c_str()         		 , &TrgID[0]      		 , (name+".TrgID["+name+".Size]/I").c_str()	    	       );
			root->Branch((name+".isPFTau").c_str()                   , &isPFTau[0]                   , (name+".isPFTau["+name+".Size]/I").c_str()                  );  // Add by YoungKyu
			root->Branch((name+".decayModeFinding").c_str()                  , &decayModeFinding[0]                  , (name+".decayModeFinding["+name+".Size]/F").c_str()                 );
			root->Branch((name+".byVLooseCombinedIsolationDeltaBetaCorr").c_str()                    , &byVLooseCombinedIsolationDeltaBetaCorr[0]                    , (name+".byVLooseCombinedIsolationDeltaBetaCorr["+name+".Size]/F").c_str()                   );  // Add by YoungKyu
			root->Branch((name+".byLooseCombinedIsolationDeltaBetaCorr").c_str()                     , &byLooseCombinedIsolationDeltaBetaCorr[0]                     , (name+".byLooseCombinedIsolationDeltaBetaCorr["+name+".Size]/F").c_str()                    );  // Add by YoungKyu
			root->Branch((name+".byMediumCombinedIsolationDeltaBetaCorr").c_str()                    , &byMediumCombinedIsolationDeltaBetaCorr[0]                    , (name+".byMediumCombinedIsolationDeltaBetaCorr["+name+".Size]/F").c_str()                   );  // Add by YoungKyu
			root->Branch((name+".byTightCombinedIsolationDeltaBetaCorr").c_str()                     , &byTightCombinedIsolationDeltaBetaCorr[0]                     , (name+".byTightCombinedIsolationDeltaBetaCorr["+name+".Size]/F").c_str()                    );  // Add by YoungKyu
			root->Branch((name+".againstElectronLoose").c_str()                      , &againstElectronLoose[0]                      , (name+".againstElectronLoose["+name+".Size]/F").c_str()                     );  // Add by YoungKyu
			root->Branch((name+".againstElectronMedium").c_str()                     , &againstElectronMedium[0]                     , (name+".againstElectronMedium["+name+".Size]/F").c_str()                    );  // Add by YoungKyu
			root->Branch((name+".againstElectronTight").c_str()                      , &againstElectronTight[0]                      , (name+".againstElectronTight["+name+".Size]/F").c_str()                     );  // Add by YoungKyu
			root->Branch((name+".againstElectronMVA").c_str()                        , &againstElectronMVA[0]                        , (name+".againstElectronMVA["+name+".Size]/F").c_str()                       );  // Add by YoungKyu
			root->Branch((name+".againstMuonLoose").c_str()                  , &againstMuonLoose[0]                  , (name+".againstMuonLoose["+name+".Size]/F").c_str()                 );  // Add by YoungKyu
			root->Branch((name+".againstMuonMedium").c_str()                         , &againstMuonMedium[0]                         , (name+".againstMuonMedium["+name+".Size]/F").c_str()                );  // Add by YoungKyu
			root->Branch((name+".againstMuonTight").c_str()                  , &againstMuonTight[0]                  , (name+".againstMuonTight["+name+".Size]/F").c_str()                 );  // Add by YoungKyu


		}


		void Register(TTree *root, std::string name="LepInfo") {
			root->SetBranchAddress((name+".Size").c_str()			   , &Size			   );
			root->SetBranchAddress((name+".Index").c_str()  		   , &Index[0]  		   );
			root->SetBranchAddress((name+".isEcalDriven").c_str()  		   , &isEcalDriven[0]  		   );
			root->SetBranchAddress((name+".isTrackerDriven").c_str()  	   , &isTrackerDriven[0]	   );
			root->SetBranchAddress((name+".LeptonType").c_str()		   , &LeptonType[0]		   );
			root->SetBranchAddress((name+".Charge").c_str() 		   , &Charge[0] 		   );
			root->SetBranchAddress((name+".ChargeGsf").c_str() 		   , &ChargeGsf[0] 		   );
			root->SetBranchAddress((name+".ChargeCtf").c_str() 		   , &ChargeCtf[0] 		   );
			root->SetBranchAddress((name+".ChargeScPix").c_str() 		   , &ChargeScPix[0] 		   );
			root->SetBranchAddress((name+".Pt").c_str()			   , &Pt[0]			   );
			root->SetBranchAddress((name+".Et").c_str()			   , &Et[0]			   );
			root->SetBranchAddress((name+".Eta").c_str()			   , &Eta[0]			   );
			root->SetBranchAddress((name+".caloEta").c_str()		   , &caloEta[0]		   );
			root->SetBranchAddress((name+".Phi").c_str()			   , &Phi[0]			   );
			root->SetBranchAddress((name+".TrackIso").c_str()		   , &TrackIso[0]		   );
			root->SetBranchAddress((name+".EcalIso").c_str()		   , &EcalIso[0]		   );	
			root->SetBranchAddress((name+".HcalIso").c_str()		   , &HcalIso[0]		   ); 
			root->SetBranchAddress((name+".HcalDepth1Iso").c_str()		   , &HcalDepth1Iso[0]		   ); 
			root->SetBranchAddress((name+".HcalDepth2Iso").c_str()		   , &HcalDepth2Iso[0]		   ); 
			root->SetBranchAddress((name+".ChargedHadronIso").c_str()		   , &ChargedHadronIso[0]		   ); 
			root->SetBranchAddress((name+".NeutralHadronIso").c_str()		   , &NeutralHadronIso[0]		   ); 
			root->SetBranchAddress((name+".PhotonIso").c_str()		   , &PhotonIso[0]		   ); 
			root->SetBranchAddress((name+".ChargedHadronIsoR03").c_str()		   , &ChargedHadronIsoR03[0]		   ); 
			root->SetBranchAddress((name+".NeutralHadronIsoR03").c_str()		   , &NeutralHadronIsoR03[0]		   ); 
			root->SetBranchAddress((name+".PhotonIsoR03").c_str()		   , &PhotonIsoR03[0]		   ); 
			root->SetBranchAddress((name+".sumPUPtR03").c_str()		   , &sumPUPtR03[0]		   ); 
			root->SetBranchAddress((name+".IsoRhoCorrR03").c_str()		   , &IsoRhoCorrR03[0]		   ); 
			root->SetBranchAddress((name+".ChargedHadronIsoR04").c_str()		   , &ChargedHadronIsoR04[0]		   ); 
			root->SetBranchAddress((name+".NeutralHadronIsoR04").c_str()		   , &NeutralHadronIsoR04[0]		   ); 
			root->SetBranchAddress((name+".PhotonIsoR04").c_str()		   , &PhotonIsoR04[0]		   ); 
			root->SetBranchAddress((name+".sumPUPtR04").c_str()		   , &sumPUPtR04[0]		   ); 
			root->SetBranchAddress((name+".IsoRhoCorrR04").c_str()		   , &IsoRhoCorrR04[0]		   ); 
			root->SetBranchAddress((name+".Ip3dPV").c_str()		   , &Ip3dPV[0]		   ); 
			root->SetBranchAddress((name+".Ip3dPVErr").c_str()		   , &Ip3dPVErr[0]		   ); 
			root->SetBranchAddress((name+".Ip3dPVSignificance").c_str()		   , &Ip3dPVSignificance[0]		   ); 
			root->SetBranchAddress((name+".MuontimenDof").c_str()		   , &MuontimenDof[0]		   ); 
			root->SetBranchAddress((name+".MuontimeAtIpInOut").c_str()		   , &MuontimeAtIpInOut[0]		   ); 
			root->SetBranchAddress((name+".MuontimeAtIpOutIn").c_str()		   , &MuontimeAtIpOutIn[0]		   ); 
			root->SetBranchAddress((name+".Muondirection").c_str()		   , &Muondirection[0]		   ); 
			root->SetBranchAddress((name+".CaloEnergy").c_str()		   , &CaloEnergy[0]		   );
			root->SetBranchAddress((name+".e1x5").c_str()		           , &e1x5[0]   		   );
			root->SetBranchAddress((name+".e2x5Max").c_str()		   , &e2x5Max[0]   		   );
			root->SetBranchAddress((name+".e5x5").c_str()		           , &e5x5[0]   		   );
			root->SetBranchAddress((name+".Px").c_str()			   , &Px[0]			   ); //Uly 2011-04-04
			root->SetBranchAddress((name+".Py").c_str()			   , &Py[0]			   ); //Uly 2011-04-04
			root->SetBranchAddress((name+".Pz").c_str()			   , &Pz[0]			   ); //Uly 2011-04-04
			root->SetBranchAddress((name+".Energy").c_str()		   , &Energy[0]			   ); //Uly 2011-04-04
			root->SetBranchAddress((name+".innerTracknormalizedChi2").c_str()		   , &innerTracknormalizedChi2[0]			   ); 
			root->SetBranchAddress((name+".vertexZ").c_str()	           , &vertexZ[0]   		   ); //Uly 2011-04-04

			root->SetBranchAddress((name+".MuIDGlobalMuonPromptTight").c_str()                   , &MuIDGlobalMuonPromptTight[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".isPFMuon").c_str()                   , &isPFMuon[0]                   ); //Add by Jacky

			root->SetBranchAddress((name+".MuInnerPtError").c_str() 	   , &MuInnerPtError[0] 	   );
			root->SetBranchAddress((name+".MuGlobalPtError").c_str() 	   , &MuGlobalPtError[0] 	   );
			root->SetBranchAddress((name+".MuInnerTrackDz").c_str() 	   , &MuInnerTrackDz[0] 	   );
			root->SetBranchAddress((name+".MuInnerTrackD0").c_str() 	   , &MuInnerTrackD0[0] 	   );
			root->SetBranchAddress((name+".MuInnerTrackDxy_BS").c_str() 	   , &MuInnerTrackDxy_BS[0] 	   );
			root->SetBranchAddress((name+".MuInnerTrackDxy_PV").c_str() 	   , &MuInnerTrackDxy_PV[0] 	   );
			root->SetBranchAddress((name+".MuInnerTrackDxy_PVBS").c_str() 	   , &MuInnerTrackDxy_PVBS[0] 	   );
			root->SetBranchAddress((name+".MuInnerTrackNHits").c_str()	   , &MuInnerTrackNHits[0]	   );	    
			root->SetBranchAddress((name+".MuNTrackerHits").c_str()	       , &MuNTrackerHits[0]	   );	    
			root->SetBranchAddress((name+".MuCaloCompat").c_str()		   , &MuCaloCompat[0]		   );
			root->SetBranchAddress((name+".MuNChambers").c_str()		   , &MuNChambers[0]		   );
			root->SetBranchAddress((name+".MuNChambersMatchesSegment").c_str() , &MuNChambersMatchesSegment[0] );
			root->SetBranchAddress((name+".MuNMatchedStations").c_str() , &MuNMatchedStations[0] );
			root->SetBranchAddress((name+".MuNPixelLayers").c_str()            , &MuNPixelLayers[0]	   );
			root->SetBranchAddress((name+".MuNPixelLayersWMeasurement").c_str(), &MuNPixelLayersWMeasurement[0]); //Uly 2011-04-04
			root->SetBranchAddress((name+".MuNTrackLayersWMeasurement").c_str(), &MuNTrackLayersWMeasurement[0]);
			root->SetBranchAddress((name+".MuNLostInnerHits").c_str()          , &MuNLostInnerHits[0]	   );
			root->SetBranchAddress((name+".MuNLostOuterHits").c_str()          , &MuNLostOuterHits[0]	   );
			root->SetBranchAddress((name+".MuNMuonhits").c_str()               , &MuNMuonhits[0] 		   );
			root->SetBranchAddress((name+".MuDThits").c_str()               , &MuDThits[0] 		   );
			root->SetBranchAddress((name+".MuCSChits").c_str()               , &MuCSChits[0] 		   );
			root->SetBranchAddress((name+".MuRPChits").c_str()               , &MuRPChits[0] 		   );
			root->SetBranchAddress((name+".MuType").c_str() 		   , &MuType[0] 		   );

			root->SetBranchAddress((name+".MuGlobalNormalizedChi2").c_str()    , &MuGlobalNormalizedChi2[0]	   ); //Dmitry

			root->SetBranchAddress((name+".ElTrackNLostHits").c_str()	   , &ElTrackNLostHits[0]	   );   //yjlei
			root->SetBranchAddress((name+".ElTrackDz").c_str() 	   	   , &ElTrackDz[0] 	   	   );
			root->SetBranchAddress((name+".ElTrackDz_BS").c_str() 	   	   , &ElTrackDz_BS[0] 	   	   );
			root->SetBranchAddress((name+".ElTrackD0").c_str() 	   	   , &ElTrackD0[0] 	   	   );
			root->SetBranchAddress((name+".ElTrackDxy_BS").c_str() 	   	   , &ElTrackDxy_BS[0] 	   	   );
			root->SetBranchAddress((name+".ElTrackDxy_PV").c_str() 	   	   , &ElTrackDxy_PV[0] 	   	   );
			root->SetBranchAddress((name+".ElTrackDxy_PVBS").c_str() 	   , &ElTrackDxy_PVBS[0] 	   );

			root->SetBranchAddress((name+".EgammaMVANonTrig").c_str()                   , &EgammaMVANonTrig[0]); //Add by Jacky
			root->SetBranchAddress((name+".EgammaMVATrig").c_str()                   , &EgammaMVATrig[0]); //Add by Jacky
			root->SetBranchAddress((name+".isGoodMuonTMOneStationTight").c_str()                   , &isGoodMuonTMOneStationTight[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".EgammaCutBasedEleIdTRIGGERTIGHT").c_str()                   , &EgammaCutBasedEleIdTRIGGERTIGHT[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".EgammaCutBasedEleIdTRIGGERWP70").c_str()                   , &EgammaCutBasedEleIdTRIGGERWP70[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".EgammaCutBasedEleIdVETO").c_str()                   , &EgammaCutBasedEleIdVETO[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".EgammaCutBasedEleIdLOOSE").c_str()                   , &EgammaCutBasedEleIdLOOSE[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".EgammaCutBasedEleIdMEDIUM").c_str()                   , &EgammaCutBasedEleIdMEDIUM[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".EgammaCutBasedEleIdTIGHT").c_str()                   , &EgammaCutBasedEleIdTIGHT[0]                   ); //Add by Jacky

			root->SetBranchAddress((name+".Eldr03HcalDepth1TowerSumEtBc").c_str()		   , &Eldr03HcalDepth1TowerSumEtBc[0]		   );
			root->SetBranchAddress((name+".Eldr03HcalDepth2TowerSumEtBc").c_str()		   , &Eldr03HcalDepth2TowerSumEtBc[0]		   );
			root->SetBranchAddress((name+".Eldr04HcalDepth1TowerSumEtBc").c_str()		   , &Eldr04HcalDepth1TowerSumEtBc[0]		   );
			root->SetBranchAddress((name+".Eldr04HcalDepth2TowerSumEtBc").c_str()		   , &Eldr04HcalDepth2TowerSumEtBc[0]		   );
			root->SetBranchAddress((name+".ElhcalOverEcalBc").c_str()		   , &ElhcalOverEcalBc[0]		   );
			root->SetBranchAddress((name+".ElEcalE").c_str()		   , &ElEcalE[0]		   );
			root->SetBranchAddress((name+".ElEoverP").c_str()		   , &ElEoverP[0]		   );
			root->SetBranchAddress((name+".EldeltaEta").c_str()		   , &EldeltaEta[0]		   );
			root->SetBranchAddress((name+".EldeltaPhi").c_str()		   , &EldeltaPhi[0]		   );
			root->SetBranchAddress((name+".ElHadoverEm").c_str()		   , &ElHadoverEm[0]		   );
			root->SetBranchAddress((name+".ElsigmaIetaIeta").c_str()	   , &ElsigmaIetaIeta[0]	   );// Jacky
			root->SetBranchAddress((name+".ElscSigmaIetaIeta").c_str()	   , &ElscSigmaIetaIeta[0]	   );// Jacky
			root->SetBranchAddress((name+".ElEnergyErr").c_str()		   , &ElEnergyErr[0]		   );
			root->SetBranchAddress((name+".ElMomentumErr").c_str()  	   , &ElMomentumErr[0]  	   );
			root->SetBranchAddress((name+".ElTrackNHits").c_str()		   , &ElTrackNHits[0]		   );
			root->SetBranchAddress((name+".ElSharedHitsFraction").c_str()      , &ElSharedHitsFraction[0]      ); //Dmitry
			root->SetBranchAddress((name+".dR_gsf_ctfTrack").c_str()      , &dR_gsf_ctfTrack[0]      ); //Dmitry
			root->SetBranchAddress((name+".dPt_gsf_ctfTrack").c_str()      , &dPt_gsf_ctfTrack[0]      ); //Dmitry
			root->SetBranchAddress((name+".ElNClusters").c_str()		   , &ElNClusters[0]		   );
			root->SetBranchAddress((name+".ElClassification").c_str()	   , &ElClassification[0]	   );

			root->SetBranchAddress((name+".ElFBrem").c_str()	 	   , &ElFBrem[0]		   ); //Jacky
			root->SetBranchAddress((name+".ElNumberOfBrems").c_str()	   , &ElNumberOfBrems[0]	   ); //Jacky
			root->SetBranchAddress((name+".NumberOfExpectedInnerHits").c_str() , &NumberOfExpectedInnerHits[0] ); //Add by Jacky
			root->SetBranchAddress((name+".Eldist").c_str()                   , &Eldist[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".Eldcot").c_str()                   , &Eldcot[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".Elconvradius").c_str()             , &Elconvradius[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".ElConvPoint_x").c_str()            , &ElConvPoint_x[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".ElConvPoint_y").c_str()            , &ElConvPoint_y[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".ElConvPoint_z").c_str()            , &ElConvPoint_z[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".ElhasConv").c_str()             , &ElhasConv[0]                   ); //Add by Jacky

			root->SetBranchAddress((name+".dcotdist").c_str()                 , &dcotdist[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".ElseedEoverP").c_str()             , &ElseedEoverP[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".ElEcalIso04").c_str()              , &ElEcalIso04[0]                   ); //Add by Jacky
			root->SetBranchAddress((name+".ElHcalIso04").c_str()              , &ElHcalIso04[0]                   ); //Add by Jacky

			root->SetBranchAddress((name+".GenPt").c_str()  		   , &GenPt[0]  		   );
			root->SetBranchAddress((name+".GenEta").c_str() 		   , &GenEta[0] 		   );
			root->SetBranchAddress((name+".GenPhi").c_str() 		   , &GenPhi[0] 		   );
			root->SetBranchAddress((name+".GenPdgID").c_str()		   , &GenPdgID[0]		   );
			root->SetBranchAddress((name+".GenMCTag").c_str()		   , &GenMCTag[0]		   );
			root->SetBranchAddress((name+".TrgPt").c_str()  		   , &TrgPt[0]  		   );
			root->SetBranchAddress((name+".TrgEta").c_str() 		   , &TrgEta[0] 		   );
			root->SetBranchAddress((name+".TrgPhi").c_str() 		   , &TrgPhi[0] 		   );
			root->SetBranchAddress((name+".TrgID").c_str() 	       	   , &TrgID[0] 		   	   );
			root->SetBranchAddress((name+".isPFTau").c_str()                   , &isPFTau[0]                           ); // Add by YoungKyu
			root->SetBranchAddress((name+".decayModeFinding").c_str()                  , &decayModeFinding[0]                          ); // Add by YoungKyu
			root->SetBranchAddress((name+".byVLooseCombinedIsolationDeltaBetaCorr").c_str()                    , &byVLooseCombinedIsolationDeltaBetaCorr[0]                            ); // Add by YoungKyu
			root->SetBranchAddress((name+".byLooseCombinedIsolationDeltaBetaCorr").c_str()             , &byLooseCombinedIsolationDeltaBetaCorr[0]                     ); // Add by YoungKyu
			root->SetBranchAddress((name+".byMediumCombinedIsolationDeltaBetaCorr").c_str()                    , &byMediumCombinedIsolationDeltaBetaCorr[0]                            ); // Add by YoungKyu
			root->SetBranchAddress((name+".byTightCombinedIsolationDeltaBetaCorr").c_str()             , &byTightCombinedIsolationDeltaBetaCorr[0]                     ); // Add by YoungKyu
			root->SetBranchAddress((name+".againstElectronLoose").c_str()              , &againstElectronLoose[0]                      ); // Add by YoungKyu
			root->SetBranchAddress((name+".againstElectronMedium").c_str()             , &againstElectronMedium[0]                     ); // Add by YoungKyu
			root->SetBranchAddress((name+".againstElectronTight").c_str()              , &againstElectronTight[0]                      ); // Add by YoungKyu
			root->SetBranchAddress((name+".againstElectronMVA").c_str()                , &againstElectronMVA[0]                        ); // Add by YoungKyu
			root->SetBranchAddress((name+".againstMuonLoose").c_str()                  , &againstMuonLoose[0]                          ); // Add by YoungKyu
			root->SetBranchAddress((name+".againstMuonMedium").c_str()                 , &againstMuonMedium[0]                         ); // Add by YoungKyu
			root->SetBranchAddress((name+".againstMuonTight").c_str()                  , &againstMuonTight[0]                          ); // Add by YoungKyu


		}    
};


class JetInfoBranches {
	public:
		int	Size; 
		int   Index[MAX_JETS];
		int   NTracks[MAX_JETS];
		float Et[MAX_JETS];
		float Pt[MAX_JETS];
		float Unc[MAX_JETS];
		float Eta[MAX_JETS];
		float Phi[MAX_JETS];
		int   JetIDLOOSE[MAX_JETS]; //Add by Chiyi
		float JetCharge[MAX_JETS];
		int   NConstituents[MAX_JETS];
		int   NCH[MAX_JETS];
		float CEF[MAX_JETS];
		float NHF[MAX_JETS];
		float NEF[MAX_JETS];
		float CHF[MAX_JETS];
		float JVAlpha[MAX_JETS];
		float JVBeta[MAX_JETS];
		float PtCorrRaw[MAX_JETS];  
		float PtCorrL2[MAX_JETS];  
		float PtCorrL3[MAX_JETS];  
		float PtCorrL7g[MAX_JETS];
		float PtCorrL7uds[MAX_JETS];
		float PtCorrL7c[MAX_JETS];  
		float PtCorrL7b[MAX_JETS];  
		float JetBProbBJetTags[MAX_JETS];
		float JetProbBJetTags[MAX_JETS];
		float TrackCountHiPurBJetTags[MAX_JETS];  
		float TrackCountHiEffBJetTags[MAX_JETS]; 
		//float ImpactParaMVABJetTags[MAX_JETS]; //NONE //remove by Chiyi
		float SimpleSVBJetTags[MAX_JETS];  //for 35X sample //Add by Chiyi
		float SimpleSVHEBJetTags[MAX_JETS];  //for 36X sample //Add by Chiyi
		float SimpleSVHPBJetTags[MAX_JETS];  //for 36X sample //Add by Chiyi
		float CombinedSVBJetTags[MAX_JETS];
		float CombinedSVMVABJetTags[MAX_JETS];
		float SoftElecByIP3dBJetTags[MAX_JETS];
		float SoftElecByPtBJetTags[MAX_JETS];  
		float SoftMuonBJetTags[MAX_JETS];      
		float SoftMuonByIP3dBJetTags[MAX_JETS];
		float SoftMuonByPtBJetTags[MAX_JETS];  
		float DoubleSVHighEffBJetTags[MAX_JETS]; //// Added by DM 
		//float JetLRval[MAX_JETS]; //NONE //remove by Chiyi
		//float JetProb[MAX_JETS]; //NONE //remove by Chiyi
		float GenJetPt[MAX_JETS];
		float GenJetEta[MAX_JETS];
		float GenJetPhi[MAX_JETS];
		float GenPt[MAX_JETS];
		float GenEta[MAX_JETS];
		float GenPhi[MAX_JETS];
		int   GenPdgID[MAX_JETS];
		int   GenFlavor[MAX_JETS];
		int	GenMCTag[MAX_JETS]; // 0: unknown, 1: decay from W, 2: decay from Z, (+10) from b', (+20) from t'

		float Px[MAX_JETS]; //Uly 2011-04-04
		float Py[MAX_JETS]; //Uly 2011-04-04
		float Pz[MAX_JETS]; //Uly 2011-04-04
		float Energy[MAX_JETS]; //Uly 2011-04-04

		float Mass[MAX_JETS];
		float Area[MAX_JETS];

		float MassD1[MAX_JETS];
		float MassD2[MAX_JETS];
		float PtD1[MAX_JETS];
		float PtD2[MAX_JETS];
		float EtD1[MAX_JETS];
		float EtD2[MAX_JETS];
		float EtaD1[MAX_JETS];
		float EtaD2[MAX_JETS];
		float PhiD1[MAX_JETS];
		float PhiD2[MAX_JETS];

#ifdef __BPRIMEKIT__
		reco::Candidate* CandRef[MAX_JETS]; // backward pointer to the PAT objects
#endif  

		void RegisterTree(TTree *root, std::string name="JetInfo") {
			root->Branch((name+".Size").c_str()		       , &Size  		     , (name+".Size/I").c_str() 				);
			root->Branch((name+".Index").c_str()		       , &Index[0]		     , (name+".Index["+name+".Size]/I").c_str()  		);
			root->Branch((name+".NTracks").c_str()  	       , &NTracks[0]		     , (name+".NTracks["+name+".Size]/I").c_str()		);
			root->Branch((name+".Et").c_str()		       , &Et[0] 		     , (name+".Et["+name+".Size]/F").c_str()			);
			root->Branch((name+".Pt").c_str()		       , &Pt[0] 		     , (name+".Pt["+name+".Size]/F").c_str()			);
			root->Branch((name+".Unc").c_str()		       , &Unc[0] 		     , (name+".Unc["+name+".Size]/F").c_str()			);
			root->Branch((name+".Eta").c_str()		       , &Eta[0]		     , (name+".Eta["+name+".Size]/F").c_str()			);
			root->Branch((name+".Phi").c_str()		       , &Phi[0]		     , (name+".Phi["+name+".Size]/F").c_str()			);
			root->Branch((name+".JetIDLOOSE").c_str()	       ,&JetIDLOOSE[0]	     , (name+".JetIDLOOSE["+name+".Size]/I").c_str()		); //Add by Chiyi
			root->Branch((name+".JetCharge").c_str()	       , &JetCharge[0]  	     , (name+".JetCharge["+name+".Size]/F").c_str()		);
			root->Branch((name+".NConstituents").c_str()	       , &NConstituents[0]	     , (name+".NConstituents["+name+".Size]/I").c_str()		);
			root->Branch((name+".NCH").c_str()	       , &NCH[0]	     , (name+".NCH["+name+".Size]/I").c_str()		);
			root->Branch((name+".CEF").c_str() 	       	       , &CEF[0]		     , (name+".CEF["+name+".Size]/F").c_str()		);
			root->Branch((name+".NHF").c_str() 	       	       , &NHF[0]		     , (name+".NHF["+name+".Size]/F").c_str()		);
			root->Branch((name+".NEF").c_str() 	       	       , &NEF[0]		     , (name+".NEF["+name+".Size]/F").c_str()		);
			root->Branch((name+".CHF").c_str() 	       	       , &CHF[0]		     , (name+".CHF["+name+".Size]/F").c_str()		);
			root->Branch((name+".JVAlpha").c_str() 	       	       , &JVAlpha[0]		     , (name+".JVAlpha["+name+".Size]/F").c_str()		);
			root->Branch((name+".JVBeta").c_str()	       	       , &JVBeta[0]	     	     , (name+".JVBeta["+name+".Size]/F").c_str()  		); 
			root->Branch((name+".PtCorrRaw").c_str() 	       , &PtCorrRaw[0]		     , (name+".PtCorrRaw["+name+".Size]/F").c_str()		);
			root->Branch((name+".PtCorrL2").c_str() 	       , &PtCorrL2[0]		     , (name+".PtCorrL2["+name+".Size]/F").c_str()		);
			root->Branch((name+".PtCorrL3").c_str() 	       , &PtCorrL3[0]		     , (name+".PtCorrL3["+name+".Size]/F").c_str()		);
			root->Branch((name+".PtCorrL7g").c_str() 	       , &PtCorrL7g[0]	  	     , (name+".PtCorrL7g["+name+".Size]/F").c_str()		);
			root->Branch((name+".PtCorrL7uds").c_str() 	       , &PtCorrL7uds[0]	     , (name+".PtCorrL7uds["+name+".Size]/F").c_str()		);	
			root->Branch((name+".PtCorrL7c").c_str() 	       , &PtCorrL7c[0]	  	     , (name+".PtCorrL7c["+name+".Size]/F").c_str()		);	
			root->Branch((name+".PtCorrL7b").c_str() 	       , &PtCorrL7b[0]	  	     , (name+".PtCorrL7b["+name+".Size]/F").c_str()		);	
			root->Branch((name+".JetBProbBJetTags").c_str()        , &JetBProbBJetTags[0]	     , (name+".JetBProbBJetTags["+name+".Size]/F").c_str()	);
			root->Branch((name+".JetProbBJetTags").c_str()	       , &JetProbBJetTags[0]	     , (name+".JetProbBJetTags["+name+".Size]/F").c_str()	);
			root->Branch((name+".TrackCountHiPurBJetTags").c_str() , &TrackCountHiPurBJetTags[0] , (name+".TrackCountHiPurBJetTags["+name+".Size]/F").c_str());	    
			root->Branch((name+".TrackCountHiEffBJetTags").c_str() , &TrackCountHiEffBJetTags[0] , (name+".TrackCountHiEffBJetTags["+name+".Size]/F").c_str());
			//	root->Branch((name+".ImpactParaMVABJetTags").c_str()   , &ImpactParaMVABJetTags[0]   , (name+".ImpactParaMVABJetTags["+name+".Size]/F").c_str()  ); //NONE //remove by Chiyi
			root->Branch((name+".SimpleSVBJetTags").c_str()        , &SimpleSVBJetTags[0]	     , (name+".SimpleSVBJetTags["+name+".Size]/F").c_str()	); //for 35X sample //Add by Chiyi
			root->Branch((name+".SimpleSVHEBJetTags").c_str()        , &SimpleSVHEBJetTags[0]	     , (name+".SimpleSVHEBJetTags["+name+".Size]/F").c_str()	); //for 36X sample //Add by Chiyi
			root->Branch((name+".SimpleSVHPBJetTags").c_str()        , &SimpleSVHPBJetTags[0]	     , (name+".SimpleSVHPBJetTags["+name+".Size]/F").c_str()	); //for 36X sample //Add by Chiyi
			root->Branch((name+".CombinedSVBJetTags").c_str()      , &CombinedSVBJetTags[0]      , (name+".CombinedSVBJetTags["+name+".Size]/F").c_str()	);
			root->Branch((name+".CombinedSVMVABJetTags").c_str()   , &CombinedSVMVABJetTags[0]   , (name+".CombinedSVMVABJetTags["+name+".Size]/F").c_str()  );

			root->Branch((name+".SoftElecByIP3dBJetTags").c_str()  , &SoftElecByIP3dBJetTags[0]  , (name+".SoftElecByIP3dBJetTags["+name+".Size]/F").c_str()	);
			root->Branch((name+".SoftElecByPtBJetTags").c_str()    , &SoftElecByPtBJetTags[0]    , (name+".SoftElecByPtBJetTags["+name+".Size]/F").c_str()	);
			root->Branch((name+".SoftMuonBJetTags").c_str()        , &SoftMuonBJetTags[0]	     , (name+".SoftMuonBJetTags["+name+".Size]/F").c_str()	);
			root->Branch((name+".SoftMuonByIP3dBJetTags").c_str()  , &SoftMuonByIP3dBJetTags[0]  , (name+".SoftMuonByIP3dBJetTags["+name+".Size]/F").c_str()	);	
			root->Branch((name+".SoftMuonByPtBJetTags").c_str()    , &SoftMuonByPtBJetTags[0]    , (name+".SoftMuonByPtBJetTags["+name+".Size]/F").c_str()	);	
			root->Branch((name+".DoubleSVHighEffBJetTags").c_str() , &DoubleSVHighEffBJetTags[0] , (name+".DoubleSVHighEffBJetTags["+name+".Size]/F").c_str()); //// Added by DM	

			//	root->Branch((name+".JetLRval").c_str() 	       , &JetLRval[0]		     , (name+".JetLRval["+name+".Size]/F").c_str()		); //NONE //remove by Chiyi
			//	root->Branch((name+".JetProb").c_str()  	       , &JetProb[0]		     , (name+".JetProb["+name+".Size]/F").c_str()		); //NONE //remove by Chiyi
			root->Branch((name+".GenJetPt").c_str() 	       , &GenJetPt[0]		     , (name+".GenJetPt["+name+".Size]/F").c_str()		);
			root->Branch((name+".GenJetEta").c_str()	       , &GenJetEta[0]  	     , (name+".GenJetEta["+name+".Size]/F").c_str()		);
			root->Branch((name+".GenJetPhi").c_str()	       , &GenJetPhi[0]  	     , (name+".GenJetPhi["+name+".Size]/F").c_str()		);
			root->Branch((name+".GenPt").c_str()		       , &GenPt[0]		     , (name+".GenPt["+name+".Size]/F").c_str()  		);
			root->Branch((name+".GenEta").c_str()		       , &GenEta[0]		     , (name+".GenEta["+name+".Size]/F").c_str() 		);
			root->Branch((name+".GenPhi").c_str()		       , &GenPhi[0]		     , (name+".GenPhi["+name+".Size]/F").c_str() 		);
			root->Branch((name+".GenPdgID").c_str() 	       , &GenPdgID[0]		     , (name+".GenPdgID["+name+".Size]/I").c_str()		);
			root->Branch((name+".GenFlavor").c_str()	       , &GenFlavor[0]  	     , (name+".GenFlavor["+name+".Size]/I").c_str()		);
			root->Branch((name+".GenMCTag").c_str()	       	       , &GenMCTag[0]  	             , (name+".GenMCTag["+name+".Size]/I").c_str()		);

			root->Branch((name+".Px").c_str()		       , &Px[0] 		     , (name+".Px["+name+".Size]/F").c_str()			); //Uly 2011-04-04
			root->Branch((name+".Py").c_str()		       , &Py[0] 		     , (name+".Py["+name+".Size]/F").c_str()			); //Uly 2011-04-04
			root->Branch((name+".Pz").c_str()		       , &Pz[0] 		     , (name+".Pz["+name+".Size]/F").c_str()			); //Uly 2011-04-04
			root->Branch((name+".Energy").c_str()	       , &Energy[0] 		     , (name+".Energy["+name+".Size]/F").c_str()			); //Uly 2011-04-04
			root->Branch((name+".Mass").c_str()	       , &Mass[0] 		     , (name+".Mass["+name+".Size]/F").c_str()			); 
			root->Branch((name+".Area").c_str()	       , &Area[0] 		     , (name+".Area["+name+".Size]/F").c_str()			); 
			root->Branch((name+".MassD1").c_str()	     , &MassD1[0] 		   , (name+".MassD1["+name+".Size]/F").c_str()		); 
			root->Branch((name+".MassD2").c_str()	     , &MassD2[0] 		   , (name+".MassD2["+name+".Size]/F").c_str()		); 
			root->Branch((name+".PtD1").c_str()	     , &PtD1[0] 		   , (name+".PtD1["+name+".Size]/F").c_str()		); 
			root->Branch((name+".PtD2").c_str()	     , &PtD2[0] 		   , (name+".PtD2["+name+".Size]/F").c_str()		); 
			root->Branch((name+".EtD1").c_str()	     , &EtD1[0] 		   , (name+".EtD1["+name+".Size]/F").c_str()		); 
			root->Branch((name+".EtD2").c_str()	     , &EtD2[0] 		   , (name+".EtD2["+name+".Size]/F").c_str()		); 
			root->Branch((name+".EtaD1").c_str()	     , &EtaD1[0] 		   , (name+".EtaD1["+name+".Size]/F").c_str()		); 
			root->Branch((name+".EtaD2").c_str()	     , &EtaD2[0] 		   , (name+".EtaD2["+name+".Size]/F").c_str()		); 
			root->Branch((name+".PhiD1").c_str()	     , &PhiD1[0] 		   , (name+".PhiD1["+name+".Size]/F").c_str()		); 
			root->Branch((name+".PhiD2").c_str()	     , &PhiD2[0] 		   , (name+".PhiD2["+name+".Size]/F").c_str()		); 
		}  



		void Register(TTree *root, std::string name="JetInfo") {
			root->SetBranchAddress((name+".Size").c_str()			 , &Size		       );
			root->SetBranchAddress((name+".Index").c_str()  		 , &Index[0]		       );
			root->SetBranchAddress((name+".NTracks").c_str()		 , &NTracks[0]  	       );
			root->SetBranchAddress((name+".Et").c_str()			 , &Et[0]		       );
			root->SetBranchAddress((name+".Pt").c_str()			 , &Pt[0]		       );
			root->SetBranchAddress((name+".Unc").c_str()			 , &Unc[0]		       );
			root->SetBranchAddress((name+".Eta").c_str()			 , &Eta[0]		       );
			root->SetBranchAddress((name+".Phi").c_str()			 , &Phi[0]		       );
			root->SetBranchAddress((name+".JetIDLOOSE").c_str()		 , &JetIDLOOSE[0]	       ); //Add by Chiyi
			root->SetBranchAddress((name+".JetCharge").c_str()		 , &JetCharge[0]	       );
			root->SetBranchAddress((name+".NConstituents").c_str()		 , &NConstituents[0]	       );
			root->SetBranchAddress((name+".NCH").c_str()		 , &NCH[0]	       );
			root->SetBranchAddress((name+".CEF").c_str()		 , &CEF[0]  	       );
			root->SetBranchAddress((name+".NHF").c_str()		 , &NHF[0]  	       );
			root->SetBranchAddress((name+".NEF").c_str()		 , &NEF[0]  	       );
			root->SetBranchAddress((name+".CHF").c_str()		 , &CHF[0]  	       );
			root->SetBranchAddress((name+".JVAlpha").c_str()		 , &JVAlpha[0]  	       );
			root->SetBranchAddress((name+".JVBeta").c_str() 		 , &JVBeta[0]		       ); 
			root->SetBranchAddress((name+".PtCorrRaw").c_str()		 , &PtCorrRaw[0]	       );
			root->SetBranchAddress((name+".PtCorrL2").c_str()		 , &PtCorrL2[0] 	       );
			root->SetBranchAddress((name+".PtCorrL3").c_str()		 , &PtCorrL3[0] 	       );
			root->SetBranchAddress((name+".PtCorrL7g").c_str()		 , &PtCorrL7g[0]	       );
			root->SetBranchAddress((name+".PtCorrL7uds").c_str()		 , &PtCorrL7uds[0]	       );      
			root->SetBranchAddress((name+".PtCorrL7c").c_str()		 , &PtCorrL7c[0]	       );      
			root->SetBranchAddress((name+".PtCorrL7b").c_str()		 , &PtCorrL7b[0]	       );      
			root->SetBranchAddress((name+".JetBProbBJetTags").c_str()	 , &JetBProbBJetTags[0]        );
			root->SetBranchAddress((name+".JetProbBJetTags").c_str()	 , &JetProbBJetTags[0]         );
			root->SetBranchAddress((name+".TrackCountHiPurBJetTags").c_str() , &TrackCountHiPurBJetTags[0] );	   
			root->SetBranchAddress((name+".TrackCountHiEffBJetTags").c_str() , &TrackCountHiEffBJetTags[0] );
			//	root->SetBranchAddress((name+".ImpactParaMVABJetTags").c_str()   , &ImpactParaMVABJetTags[0]   ); //NONE //remove by Chiyi
			root->SetBranchAddress((name+".SimpleSVBJetTags").c_str()	 , &SimpleSVBJetTags[0]        ); //for 36X sample //Add by Chiyi
			root->SetBranchAddress((name+".SimpleSVHEBJetTags").c_str()	 , &SimpleSVHEBJetTags[0]        ); //for 36X sample //Add by Chiyi
			root->SetBranchAddress((name+".SimpleSVHPBJetTags").c_str()	 , &SimpleSVHPBJetTags[0]        ); //for 36X sample //Add by Chiyi
			root->SetBranchAddress((name+".CombinedSVBJetTags").c_str()	 , &CombinedSVBJetTags[0]      );
			root->SetBranchAddress((name+".CombinedSVMVABJetTags").c_str()   , &CombinedSVMVABJetTags[0]   );

			root->SetBranchAddress((name+".SoftElecByIP3dBJetTags").c_str()  , &SoftElecByIP3dBJetTags[0]	);
			root->SetBranchAddress((name+".SoftElecByPtBJetTags").c_str()    , &SoftElecByPtBJetTags[0]  	);
			root->SetBranchAddress((name+".SoftMuonBJetTags").c_str()        , &SoftMuonBJetTags[0]	     	);
			root->SetBranchAddress((name+".SoftMuonByIP3dBJetTags").c_str()  , &SoftMuonByIP3dBJetTags[0]   );
			root->SetBranchAddress((name+".SoftMuonByPtBJetTags").c_str()    , &SoftMuonByPtBJetTags[0]   	);	
			root->SetBranchAddress((name+".DoubleSVHighEffBJetTags").c_str() , &DoubleSVHighEffBJetTags[0]  ); //// Added by DM 

			//	root->SetBranchAddress((name+".JetLRval").c_str()		 , &JetLRval[0] 	       ); //NONE //remove by Chiyi
			//	root->SetBranchAddress((name+".JetProb").c_str()		 , &JetProb[0]  	       ); //NONE //remove by Chiyi
			root->SetBranchAddress((name+".GenJetPt").c_str()		 , &GenJetPt[0] 	       );
			root->SetBranchAddress((name+".GenJetEta").c_str()		 , &GenJetEta[0]	       );
			root->SetBranchAddress((name+".GenJetPhi").c_str()		 , &GenJetPhi[0]	       );
			root->SetBranchAddress((name+".GenPt").c_str()  		 , &GenPt[0]		       );
			root->SetBranchAddress((name+".GenEta").c_str() 		 , &GenEta[0]		       );
			root->SetBranchAddress((name+".GenPhi").c_str() 		 , &GenPhi[0]		       );
			root->SetBranchAddress((name+".GenPdgID").c_str()		 , &GenPdgID[0] 	       );
			root->SetBranchAddress((name+".GenFlavor").c_str()		 , &GenFlavor[0]	       );
			root->SetBranchAddress((name+".GenMCTag").c_str()		 , &GenMCTag[0] 	       );

			root->SetBranchAddress((name+".Px").c_str()			 , &Px[0]		       ); //Uly 2011-04-04
			root->SetBranchAddress((name+".Py").c_str()			 , &Py[0]		       ); //Uly 2011-04-04
			root->SetBranchAddress((name+".Pz").c_str()			 , &Pz[0]		       ); //Uly 2011-04-04
			root->SetBranchAddress((name+".Energy").c_str()		 , &Energy[0]		       ); //Uly 2011-04-04

			root->SetBranchAddress((name+".Mass").c_str()		 , &Mass[0]		       );
			root->SetBranchAddress((name+".Area").c_str()		 , &Area[0]		       );
			root->SetBranchAddress((name+".MassD1").c_str()		 , &MassD1[0]		       );
			root->SetBranchAddress((name+".MassD2").c_str()		 , &MassD2[0]		       );
			root->SetBranchAddress((name+".PtD1").c_str()		 , &PtD1[0]		       );
			root->SetBranchAddress((name+".PtD2").c_str()		 , &PtD2[0]		       );
			root->SetBranchAddress((name+".EtD1").c_str()		 , &EtD1[0]		       );
			root->SetBranchAddress((name+".EtD2").c_str()		 , &EtD2[0]		       );
			root->SetBranchAddress((name+".EtaD1").c_str()		 , &EtaD1[0]		       );
			root->SetBranchAddress((name+".EtaD2").c_str()		 , &EtaD2[0]		       );
			root->SetBranchAddress((name+".PhiD1").c_str()		 , &PhiD1[0]		       );
			root->SetBranchAddress((name+".PhiD2").c_str()		 , &PhiD2[0]		       );
		}   
};


class PairInfoBranches {
	public:
		int	Size; 
		int	Index[MAX_PAIRS];
		int	Type[MAX_PAIRS]; // type of pairing - 1: ll (regardless of charge and flavor!), 2: jj
		int   Obj1Index[MAX_PAIRS];
		int   Obj2Index[MAX_PAIRS];
		float Mass[MAX_PAIRS];
		float Pt[MAX_PAIRS];
		float Eta[MAX_PAIRS];
		float Phi[MAX_PAIRS];  
		float GenMass[MAX_PAIRS];
		float GenPt[MAX_PAIRS];
		float GenEta[MAX_PAIRS];
		float GenPhi[MAX_PAIRS];
		int	GenPdgID[MAX_PAIRS];

		void RegisterTree(TTree *root) {
			root->Branch("PairInfo.Size" 	  , &Size	  , "PairInfo.Size/I"			 );
			root->Branch("PairInfo.Index"	  , &Index[0]	  , "PairInfo.Index[PairInfo.Size]/I"	 );
			root->Branch("PairInfo.Type"      , &Type[0]	  , "PairInfo.Type[PairInfo.Size]/I"	 );
			root->Branch("PairInfo.Obj1Index" , &Obj1Index[0] , "PairInfo.Obj1Index[PairInfo.Size]/I"); 
			root->Branch("PairInfo.Obj2Index" , &Obj2Index[0] , "PairInfo.Obj2Index[PairInfo.Size]/I"); 
			root->Branch("PairInfo.Mass"	  , &Mass[0]	  , "PairInfo.Mass[PairInfo.Size]/F"	 );
			root->Branch("PairInfo.Pt"	  , &Pt[0]	  , "PairInfo.Pt[PairInfo.Size]/F"	 );
			root->Branch("PairInfo.Eta"	  , &Eta[0]	  , "PairInfo.Eta[PairInfo.Size]/F"	 ); 
			root->Branch("PairInfo.Phi"  	  , &Phi[0]	  , "PairInfo.Phi[PairInfo.Size]/F"	 ); 
			root->Branch("PairInfo.GenMass"   , &GenMass[0]   , "PairInfo.GenMass[PairInfo.Size]/F"  ); 
			root->Branch("PairInfo.GenPt"	  , &GenPt[0]	  , "PairInfo.GenPt[PairInfo.Size]/F"	 );
			root->Branch("PairInfo.GenEta"	  , &GenEta[0]    , "PairInfo.GenEta[PairInfo.Size]/F"   );
			root->Branch("PairInfo.GenPhi"	  , &GenPhi[0]    , "PairInfo.GenPhi[PairInfo.Size]/F"   );
			root->Branch("PairInfo.GenPdgID"  , &GenPdgID[0]  , "PairInfo.GenPdgID[PairInfo.Size]/I" ); 
		}  	 



		void Register(TTree *root) {
			root->SetBranchAddress("PairInfo.Size"      , &Size	    );
			root->SetBranchAddress("PairInfo.Index"     , &Index[0]     );
			root->SetBranchAddress("PairInfo.Type"      , &Type[0]      );
			root->SetBranchAddress("PairInfo.Obj1Index" , &Obj1Index[0] ); 
			root->SetBranchAddress("PairInfo.Obj2Index" , &Obj2Index[0] ); 
			root->SetBranchAddress("PairInfo.Mass"      , &Mass[0]      );
			root->SetBranchAddress("PairInfo.Pt"	    , &Pt[0]	    );
			root->SetBranchAddress("PairInfo.Eta"	    , &Eta[0]	    ); 
			root->SetBranchAddress("PairInfo.Phi"	    , &Phi[0]	    ); 
			root->SetBranchAddress("PairInfo.GenMass"   , &GenMass[0]   ); 
			root->SetBranchAddress("PairInfo.GenPt"     , &GenPt[0]     );
			root->SetBranchAddress("PairInfo.GenEta"    , &GenEta[0]    );
			root->SetBranchAddress("PairInfo.GenPhi"    , &GenPhi[0]    );
			root->SetBranchAddress("PairInfo.GenPdgID"  , &GenPdgID[0]  ); 
		}  	 
};

class PhotonInfoBranches {
	public:
		int	Size; 
		float Pt[MAX_PHOTONS];
		float Eta[MAX_PHOTONS];
		float Phi[MAX_PHOTONS];
		float HoverE[MAX_PHOTONS];
		float SigmaIetaIeta[MAX_PHOTONS];
		float hadTowOverEm[MAX_PHOTONS];
		float hcalIsoConeDR04_2012[MAX_PHOTONS];

		float phoPFChIsoDR03[MAX_PHOTONS];
		float phoPFNeuIsoDR03[MAX_PHOTONS];
		float phoPFPhoIsoDR03[MAX_PHOTONS];

		float phoPFChIsoDR04[MAX_PHOTONS];
		float phoPFNeuIsoDR04[MAX_PHOTONS];
		float phoPFPhoIsoDR04[MAX_PHOTONS];

		float r9[MAX_PHOTONS];

		bool  passelectronveto[MAX_PHOTONS];
		float  EcalIso[MAX_PHOTONS];
		float  HcalIso[MAX_PHOTONS];
		float  TrackIso[MAX_PHOTONS];
		float GenPt[MAX_PHOTONS];
		float GenEta[MAX_PHOTONS];
		float GenPhi[MAX_PHOTONS];
		int   GenPdgID[MAX_PHOTONS];

		void RegisterTree(TTree *root, std::string name="PhotonInfo") {
			root->Branch((name+".Size").c_str()	    , &Size		      ,(name+"Size/I").c_str()				       );
			root->Branch((name+"Pt").c_str()		     	, &Pt[0]	      ,(name+"Pt["+name+".Size]/F"	).c_str()	       );
			root->Branch((name+"Eta").c_str()	     	, &Eta[0]	      ,(name+"Eta["+name+".Size]/F"	).c_str()	       );
			root->Branch((name+"Phi").c_str()	     	, &Phi[0]	      ,(name+"Phi["+name+".Size]/F"	).c_str()	       );
			root->Branch((name+"HoverE").c_str()	     	, &HoverE[0]	      ,(name+"HoverE["+name+".Size]/F"	).c_str()	       );
			root->Branch((name+"SigmaIetaIeta").c_str()	, &SigmaIetaIeta[0]	      ,(name+"SigmaIetaIeta["+name+".Size]/F"	).c_str()	       );
			root->Branch((name+"hadTowOverEm").c_str()	, &hadTowOverEm[0]	      ,(name+"hadTowOverEm["+name+".Size]/F"	).c_str()	       );
			root->Branch((name+"hcalIsoConeDR04_2012").c_str()	, &hcalIsoConeDR04_2012[0]	      ,(name+"hcalIsoConeDR04_2012["+name+".Size]/F"	).c_str()	       );
			root->Branch((name+"passelectronveto").c_str()    , &passelectronveto[0]		,(name+"passelectronveto["+name+".Size]/O" ).c_str()      );
			root->Branch((name+"phoPFChIsoDR03").c_str()    , &phoPFChIsoDR03[0]		,(name+"phoPFChIsoDR03["+name+".Size]/F" ).c_str()      );
			root->Branch((name+"phoPFNeuIsoDR03").c_str()    , &phoPFNeuIsoDR03[0]		,(name+"phoPFNeuIsoDR03["+name+".Size]/F" ).c_str()      );
			root->Branch((name+"phoPFPhoIsoDR03").c_str()    , &phoPFPhoIsoDR03[0]		,(name+"phoPFPhoIsoDR03["+name+".Size]/F" ).c_str()      );
			root->Branch((name+"phoPFChIsoDR04").c_str()    , &phoPFChIsoDR04[0]		,(name+"phoPFChIsoDR04["+name+".Size]/F" ).c_str()      );
			root->Branch((name+"phoPFNeuIsoDR04").c_str()    , &phoPFNeuIsoDR04[0]		,(name+"phoPFNeuIsoDR04["+name+".Size]/F" ).c_str()      );
			root->Branch((name+"phoPFPhoIsoDR04").c_str()    , &phoPFPhoIsoDR04[0]		,(name+"phoPFPhoIsoDR04["+name+".Size]/F" ).c_str()      );
			root->Branch((name+"r9").c_str()    , &r9[0]		,(name+"r9["+name+".Size]/F" ).c_str()      );
			root->Branch((name+"EcalIso").c_str()    , &EcalIso[0]		,(name+"EcalIso["+name+".Size]/F" ).c_str()      );
			root->Branch((name+"HcalIso").c_str()   	, &HcalIso[0]		,(name+"HcalIso["+name+".Size]/F"  ).c_str()     );
			root->Branch((name+"TrackIso").c_str()   , &TrackIso[0]	,(name+"TrackIso["+name+".Size]/F"  ).c_str()     );
			root->Branch((name+"GenPt").c_str()		  , &GenPt[0]		  ,(name+"GenPt["+name+".Size]/F"  ).c_str()     );
			root->Branch((name+"GenEta").c_str()    	, &GenEta[0]		,(name+"GenEta["+name+".Size]/F"  ).c_str()     );
			root->Branch((name+"GenPhi").c_str()    	, &GenPhi[0]		,(name+"GenPhi["+name+".Size]/F"  ).c_str()     );
			root->Branch((name+"GenPdgID").c_str()   , &GenPdgID[0]  ,(name+"GenPdgID["+name+".Size]/I" ).c_str()      );

		}  

		void Register(TTree *root, std::string name="PhotonInfo") {
			root->SetBranchAddress((name+".Size").c_str()	      , &Size			       );
			root->SetBranchAddress((name+"Pt").c_str()		     	  , &Pt[0]		       );
			root->SetBranchAddress((name+"Eta").c_str()	       	, &Eta[0]		       );
			root->SetBranchAddress((name+"Phi").c_str()	       	, &Phi[0]		       );
			root->SetBranchAddress((name+"HoverE").c_str()	       	, &HoverE[0]		       );
			root->SetBranchAddress((name+"SigmaIetaIeta").c_str()	       	, &SigmaIetaIeta[0]		       );
			root->SetBranchAddress((name+"hadTowOverEm").c_str()	       	, &hadTowOverEm[0]		       );
			root->SetBranchAddress((name+"hcalIsoConeDR04_2012").c_str()	       	, &hcalIsoConeDR04_2012[0]		       );
			root->SetBranchAddress((name+"passelectronveto").c_str()      	, &passelectronveto[0]		       );
			root->SetBranchAddress((name+"phoPFChIsoDR03").c_str()      	, &phoPFChIsoDR03[0]		       );
			root->SetBranchAddress((name+"phoPFNeuIsoDR03").c_str()      	, &phoPFNeuIsoDR03[0]		       );
			root->SetBranchAddress((name+"phoPFPhoIsoDR03").c_str()      	, &phoPFPhoIsoDR03[0]		       );
			root->SetBranchAddress((name+"phoPFChIsoDR04").c_str()      	, &phoPFChIsoDR04[0]		       );
			root->SetBranchAddress((name+"phoPFNeuIsoDR04").c_str()      	, &phoPFNeuIsoDR04[0]		       );
			root->SetBranchAddress((name+"phoPFPhoIsoDR04").c_str()      	, &phoPFPhoIsoDR04[0]		       );
			root->SetBranchAddress((name+"r9").c_str()      	, &r9[0]		       );
			root->SetBranchAddress((name+"EcalIso").c_str()      	, &EcalIso[0]		       );
			root->SetBranchAddress((name+"HcalIso").c_str()      	, &HcalIso[0]		       );
			root->SetBranchAddress((name+"TrackIso").c_str()	    	, &TrackIso[0]		       );
			root->SetBranchAddress((name+"GenPt").c_str()	      , &GenPt[0]		       );
			root->SetBranchAddress((name+"GenEta").c_str()     	, &GenEta[0]		       );
			root->SetBranchAddress((name+"GenPhi").c_str()     	, &GenPhi[0]		       );
			root->SetBranchAddress((name+"GenPdgID").c_str()	    	, &GenPdgID[0]		       );
		}  
};

class VertexInfoBranches {
	public:
		int     Size;
		int     isValid[MAX_Vertices];
		bool    isFake[MAX_Vertices]; //Uly 2011-04-04
		int     Type[MAX_Vertices];   //0 - Offline Primary Vertices, 1 - Offline Primary Vertices with beam spot constraint, 2 - Pixel Vertices
		float   Ndof[MAX_Vertices];
		float   NormalizedChi2[MAX_Vertices];
		float   Pt_Sum[MAX_Vertices];
		float   Pt_Sum2[MAX_Vertices];
		float   x[MAX_Vertices];
		float   y[MAX_Vertices];
		float   z[MAX_Vertices];
		float   Rho[MAX_Vertices];

		void RegisterTree(TTree *root) {
			root->Branch("VertexInfo.Size"	    , &Size	       , "VertexInfo.Size/I"	    );
			root->Branch("VertexInfo.isValid"  , &isValid[0]      , "VertexInfo.isValid[VertexInfo.Size]/I"	    );
			root->Branch("VertexInfo.isFake"   , &isFake[0]       , "VertexInfo.isFake[VertexInfo.Size]/O"	    ); //Uly 2011-04-04
			root->Branch("VertexInfo.Type"	    , &Type[0]	       , "VertexInfo.Type[VertexInfo.Size]/I"	    );
			root->Branch("VertexInfo.Ndof"	    , &Ndof[0]	       , "VertexInfo.Ndof[VertexInfo.Size]/F"	    );
			root->Branch("VertexInfo.NormalizedChi2"	    , &NormalizedChi2[0]	       , "VertexInfo.NormalizedChi2[VertexInfo.Size]/F"	    );
			root->Branch("VertexInfo.Pt_Sum"	    , &Pt_Sum[0]	       , "VertexInfo.Pt_Sum[VertexInfo.Size]/F"	    );
			root->Branch("VertexInfo.Pt_Sum2"	    , &Pt_Sum2[0]	       , "VertexInfo.Pt_Sum2[VertexInfo.Size]/F"	    );
			root->Branch("VertexInfo.x"	    , &x[0]	       , "VertexInfo.x[VertexInfo.Size]/F"	    );
			root->Branch("VertexInfo.y"	    , &y[0]	       , "VertexInfo.y[VertexInfo.Size]/F"	    );
			root->Branch("VertexInfo.z"	    , &z[0]	       , "VertexInfo.z[VertexInfo.Size]/F"	    );
			root->Branch("VertexInfo.Rho"	    , &Rho[0]	       , "VertexInfo.Rho[VertexInfo.Size]/F"	    );
		}										    
		void Register(TTree *root) {
			root->SetBranchAddress("VertexInfo.Size"        , &Size  	 );
			root->SetBranchAddress("VertexInfo.isValid"     , &isValid[0]  	 );
			root->SetBranchAddress("VertexInfo.isFake"      , &isFake[0]  	 ); //Uly 2011-04-04
			root->SetBranchAddress("VertexInfo.Type"        , &Type[0]  	 );
			root->SetBranchAddress("VertexInfo.Ndof"        , &Ndof[0]  	 );
			root->SetBranchAddress("VertexInfo.NormalizedChi2"        , &NormalizedChi2[0]  	 );
			root->SetBranchAddress("VertexInfo.Pt_Sum"        , &Pt_Sum[0]  	 );
			root->SetBranchAddress("VertexInfo.Pt_Sum2"        , &Pt_Sum2[0]  	 );
			root->SetBranchAddress("VertexInfo.x"        , &x[0]  	 );
			root->SetBranchAddress("VertexInfo.y"        , &y[0]  	 );
			root->SetBranchAddress("VertexInfo.z"        , &z[0]  	 );
			root->SetBranchAddress("VertexInfo.Rho"        , &Rho[0]  	 );
		}  										    
};



class GenInfoBranches {
	public:
		int Size;
		float Weight;
		float Pt[MAX_GENS];
		float Eta[MAX_GENS];
		float Phi[MAX_GENS];
		float Mass[MAX_GENS];
		int PdgID[MAX_GENS];
		int Status[MAX_GENS];
		int nMo[MAX_GENS];
		int nDa[MAX_GENS];
		int Mo1[MAX_GENS];
		int Mo2[MAX_GENS];
		int Da1[MAX_GENS];
		int Da2[MAX_GENS];

		void RegisterTree(TTree *root) {
			root->Branch("GenInfo.Size"	, &Size		, "GenInfo.Size/I"			);
			root->Branch("GenInfo.Weight"	, &Weight		, "GenInfo.Weight/F"			);
			root->Branch("GenInfo.Pt"	, &Pt[0]	, "GenInfo.Pt[GenInfo.Size]/F"		);
			root->Branch("GenInfo.Eta"	, &Eta[0]	, "GenInfo.Eta[GenInfo.Size]/F"		);
			root->Branch("GenInfo.Phi"	, &Phi[0]	, "GenInfo.Phi[GenInfo.Size]/F"		);
			root->Branch("GenInfo.Mass"	, &Mass[0]	, "GenInfo.Mass[GenInfo.Size]/F"	);
			root->Branch("GenInfo.PdgID"	, &PdgID[0]	, "GenInfo.PdgID[GenInfo.Size]/I"	);
			root->Branch("GenInfo.Status"	, &Status[0]	, "GenInfo.Status[GenInfo.Size]/I"	);
			root->Branch("GenInfo.nMo"	, &nMo[0]	, "GenInfo.nMo[GenInfo.Size]/I"		);
			root->Branch("GenInfo.nDa"	, &nDa[0]	, "GenInfo.nDa[GenInfo.Size]/I"		);
			root->Branch("GenInfo.Mo1"	, &Mo1[0]	, "GenInfo.Mo1[GenInfo.Size]/I"		);
			root->Branch("GenInfo.Mo2"	, &Mo2[0]	, "GenInfo.Mo2[GenInfo.Size]/I"		);
			root->Branch("GenInfo.Da1"	, &Da1[0]	, "GenInfo.Da1[GenInfo.Size]/I"		);
			root->Branch("GenInfo.Da2"	, &Da2[0]	, "GenInfo.Da2[GenInfo.Size]/I"		);
		}

		void Register(TTree *root) {
			root->SetBranchAddress("GenInfo.Size"	, &Size		);
			root->SetBranchAddress("GenInfo.Weight"	, &Weight		);
			root->SetBranchAddress("GenInfo.Pt"	, &Pt[0]	);
			root->SetBranchAddress("GenInfo.Eta"	, &Eta[0]	);
			root->SetBranchAddress("GenInfo.Phi"	, &Phi[0]	);
			root->SetBranchAddress("GenInfo.Mass"	, &Mass[0]	);
			root->SetBranchAddress("GenInfo.PdgID"	, &PdgID[0]	);
			root->SetBranchAddress("GenInfo.Status"	, &Status[0]	);
			root->SetBranchAddress("GenInfo.nMo"	, &nMo[0]	);
			root->SetBranchAddress("GenInfo.nDa"	, &nDa[0]	);
			root->SetBranchAddress("GenInfo.Mo1"	, &Mo1[0]	);
			root->SetBranchAddress("GenInfo.Mo2"	, &Mo2[0]	);
			root->SetBranchAddress("GenInfo.Da1"	, &Da1[0]	);
			root->SetBranchAddress("GenInfo.Da2"	, &Da2[0]	);
		}

};

#endif
