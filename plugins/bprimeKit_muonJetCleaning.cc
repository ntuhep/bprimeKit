/*******************************************************************************
 *
 *  Filename    : bprimeKit_muonJetCleaning.cc
 *  Description : Helper function for muonJet Cleaning
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
 *  Reference file:
 *  https://github.com/cms-ljmet/Ljmet-Com/blob/CMSSW_7_4_X/src/singleLepEventSelector.cc#L929-L1041
 *
 *  Only the Muon part will be implemented 
 *  
*******************************************************************************/
#include "MyAna/bprimeKit/interface/bprimeKit"

bool bprimeKit::passMuonJetClean( const pat::Jet* )
{
TLorentzVector jetP4;

	    pat::Jet tmpJet = _ijet->correctedJet(0);

	    if ( mbPar["doLepJetCleaning"] ){
		if (mbPar["debug"]) std::cout << "Checking Overlap" << std::endl;
                if (mvSelMuons.size()>0){
	            if ( deltaR(mvSelMuons[0]->p4(),_ijet->p4()) < 0.6 ){
                        std::vector<reco::CandidatePtr> muDaughters;
                        if (mbPar["debug"]) std::cout<<"Mu origin ref = "<<mvSelMuons[0]->originalObjectRef().key()<<std::endl;
                        for ( unsigned int isrc = 0; isrc < mvSelMuons[0]->numberOfSourceCandidatePtrs(); ++isrc ){
                            if (mvSelMuons[0]->sourceCandidatePtr(isrc).isAvailable()) {
                                muDaughters.push_back( mvSelMuons[0]->sourceCandidatePtr(isrc) );
                                if (mbPar["debug"]) std::cout<<"Mu daughter ref = "<<mvSelMuons[0]->sourceCandidatePtr(isrc).key()<<std::endl;
                            }
                        }
            	        if (mbPar["debug"]) {
			    std::cout << "Jet Overlaps with the Muon... Cleaning jet..." << std::endl;
            	            std::cout << "Lepton : pT = " << mvSelMuons[0]->pt() << " eta = " << mvSelMuons[0]->eta() << " phi = " << mvSelMuons[0]->phi() << std::endl;
            	            std::cout << "      Raw Jet : pT = " << _ijet->pt() << " eta = " << _ijet->eta() << " phi = " << _ijet->phi() << std::endl;
			}
			const std::vector<edm::Ptr<reco::Candidate> > _ijet_consts = _ijet->daughterPtrVector();
        		for ( std::vector<edm::Ptr<reco::Candidate> >::const_iterator _i_const = _ijet_consts.begin(); _i_const != _ijet_consts.end(); ++_i_const){
                            if (mbPar["debug"]) std::cout<<"Jet constituent ref = "<<(*_i_const).key()<<std::endl;
			    /*if ( (*_i_const).key() == mvSelMuons[0]->originalObjectRef().key() ) {
				tmpJet.setP4( _ijet->p4() - mvSelMuons[0]->p4() );
				jetP4 = correctJet(tmpJet, event);
				if (mbPar["debug"]) std::cout << "Corrected Jet : pT = " << jetP4.Pt() << " eta = " << jetP4.Eta() << " phi = " << jetP4.Phi() << std::endl;
			        _cleaned = true;
			    }*////old ref mathcing method, appears to be depreciated in CMSSW_7_4_X(?) 
                            for (unsigned int muI = 0; muI < muDaughters.size(); muI++) {
			        if ( (*_i_const).key() == muDaughters[muI].key() ) {
				    tmpJet.setP4( tmpJet.p4() - muDaughters[muI]->p4() );
				    if (mbPar["debug"]) std::cout << "  Cleaned Jet : pT = " << tmpJet.pt() << " eta = " << tmpJet.eta() << " phi = " << tmpJet.phi() << std::endl;
				    if (mbPar["debug"]) std::cout << "Clean Raw Jet : pT = " << tmpJet.correctedJet(0).pt() << " eta = " << tmpJet.correctedJet(0).eta() << " phi = " << tmpJet.correctedJet(0).phi() << std::endl;
				    jetP4 = correctJet(tmpJet, event, false, true);
				    if (mbPar["debug"]) std::cout << "Corrected Jet : pT = " << jetP4.Pt() << " eta = " << jetP4.Eta() << " phi = " << jetP4.Phi() << std::endl;
			            _cleaned = true;
                                    muDaughters.erase( muDaughters.begin()+muI );
                                    break;
			        }
			    }
			}
			// zprime method (gives same results as far as i can tell)
			/*if (_ijet->muonMultiplicity() > 0) {
			    double muEchk = (_ijet->correctedJet(0).energy()*_ijet->muonEnergyFraction()-mvSelMuons[0]->energy())/mvSelMuons[0]->energy();
			    if ( !(muEchk < -0.1 || (muEchk > 0.1 && _ijet->muonMultiplicity()==1)) ) {
 			        tmpJet.setP4( _ijet->correctedJet(0).p4()-mvSelMuons[0]->p4() );
			        if (tmpJet.pt() > 5 && deltaR(_ijet->correctedJet(0).p4(),tmpJet.p4()) > 1.57) std::cout << "Lepton-Jet cleaning flipped direction, not cleaning!" << std::endl;
			        else {
 			            jetP4 = correctJet(tmpJet, event);
			            if (mbPar["debug"]) std::cout << "Corrected Jet : pT = " << jetP4.Pt() << " eta = " << jetP4.Eta() << " phi = " << jetP4.Phi() << std::endl;
			            _cleaned = true;
			        }
                            }
                        }*/
			//old deltaR matching method
			/*for (unsigned int id = 0, nd = (*_ijet).numberOfDaughters(); id < nd; ++id) {
            		    const pat::PackedCandidate &_ijet_const = dynamic_cast<const pat::PackedCandidate &>(*(*_ijet).daughter(id));
			    if ( deltaR(mvSelMuons[0]->p4(),_ijet_const.p4()) < 0.001 ) {
 				tmpJet.setP4( _ijet->p4()-mvSelMuons[0]->p4() );
 				jetP4 = correctJet(tmpJet, event);
				if (mbPar["debug"]) std::cout << "Corrected Jet : pT = " << jetP4.Pt() << " eta = " << jetP4.Eta() << " phi = " << jetP4.Phi() << std::endl;
			        _cleaned = true;
 			    }
                        }*/
		    }
            	}
            
                if (mvSelElectrons.size()>0){
	            if ( deltaR(mvSelElectrons[0]->p4(),_ijet->p4()) < 0.6 ){
                        std::vector<reco::CandidatePtr> elDaughters;
                        if (mbPar["debug"]) std::cout<<"El origin ref = "<<mvSelElectrons[0]->originalObjectRef().key()<<std::endl;
                        for ( unsigned int isrc = 0; isrc < mvSelElectrons[0]->numberOfSourceCandidatePtrs(); ++isrc ){
                            if (mvSelElectrons[0]->sourceCandidatePtr(isrc).isAvailable()) {
                                elDaughters.push_back( mvSelElectrons[0]->sourceCandidatePtr(isrc) );
                                if (mbPar["debug"]) std::cout<<"El daughter ref = "<<mvSelElectrons[0]->sourceCandidatePtr(isrc).key()<<" , id = "<<mvSelElectrons[0]->sourceCandidatePtr(isrc)->pdgId()<<" , pt = "<<mvSelElectrons[0]->sourceCandidatePtr(isrc)->pt()<<" , "<<mvSelElectrons[0]->sourceCandidatePtr(isrc)->eta()<<" , "<<mvSelElectrons[0]->sourceCandidatePtr(isrc)->phi()<<std::endl;
                            }
                        }
            	        if (mbPar["debug"]) {
			    std::cout << "Jet Overlaps with the Electron... Cleaning jet..." << std::endl;
            	            std::cout << "Lepton : pT = " << mvSelElectrons[0]->pt() << " eta = " << mvSelElectrons[0]->eta() << " phi = " << mvSelElectrons[0]->phi() << std::endl;
            	            std::cout << "      Raw Jet : pT = " << _ijet->pt() << " eta = " << _ijet->eta() << " phi = " << _ijet->phi() << std::endl;
                            std::cout << "_ijet->electronMultiplicity() = "<<_ijet->electronMultiplicity()<<std::endl;
			}
			const std::vector<edm::Ptr<reco::Candidate> > _ijet_consts = _ijet->daughterPtrVector();
        		for ( std::vector<edm::Ptr<reco::Candidate> >::const_iterator _i_const = _ijet_consts.begin(); _i_const != _ijet_consts.end(); ++_i_const){
                            if (mbPar["debug"]) std::cout<<"Jet constituent ref = "<<(*_i_const).key()<<std::endl;
                            for (unsigned int elI = 0; elI < elDaughters.size(); elI++) {
			        if ( (*_i_const).key() == elDaughters[elI].key() ) {
				    tmpJet.setP4( tmpJet.p4() - elDaughters[elI]->p4() );
				    jetP4 = correctJet(tmpJet, event, false, true);
				    if (mbPar["debug"]) std::cout << "Corrected Jet : pT = " << jetP4.Pt() << " eta = " << jetP4.Eta() << " phi = " << jetP4.Phi() << std::endl;
			            _cleaned = true;
                                    elDaughters.erase( elDaughters.begin()+elI );
                                    break;
			        }
			    }
			}
			/*if (_ijet->electronMultiplicity() > 0) {
			    double elEchk = (_ijet->correctedJet(0).energy()*_ijet->chargedEmEnergyFraction()-mvSelElectrons[0]->energy())/mvSelElectrons[0]->energy();
                            if (mbPar["debug"]) std::cout<<"Non-zero electron multiplicity in jet, jet_chEm_Energy = "<<_ijet->correctedJet(0).energy()*_ijet->chargedEmEnergyFraction()<<std::endl;
			    if ( !(elEchk < -0.1 || (elEchk > 0.1 && _ijet->electronMultiplicity()==1)) ) {
 			        //tmpJet.setP4( _ijet->correctedJet(0).p4()-mvSelElectrons[0]->p4() );
			        if (tmpJet.pt() > 5 && deltaR(_ijet->correctedJet(0).p4(),tmpJet.p4()) > 1.57) std::cout << "Lepton-Jet cleaning flipped direction, not cleaning!" << std::endl;
			        else {
 			            //jetP4 = correctJet(tmpJet, event);
			            if (mbPar["debug"]) std::cout << "Corrected Jet : pT = " << jetP4.Pt() << " eta = " << jetP4.Eta() << " phi = " << jetP4.Phi() << std::endl;
			            //_cleaned = true;
			        }
                            }
                        }*/
		    }   
}
