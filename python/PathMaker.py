##**************************************************************************************************
##
##    FileName   :  pathMaker.py
##    Description:  Functions for making a path
##
##**************************************************************************************************

def JetBTagPath( process , jetname ):
   ans = (
         getattr( process, 'pfImpactParameterTagInfos'                    + jetname ) *
         getattr( process, 'pfJetProbabilityBJetTags'                     + jetname ) *
         getattr( process, 'pfInclusiveSecondaryVertexFinderTagInfos'     + jetname ) *
         getattr( process, 'pfCombinedInclusiveSecondaryVertexV2BJetTags' + jetname ) *
         getattr( process, 'pfSecondaryVertexTagInfos'                    + jetname ) *
         getattr( process, 'softPFMuonsTagInfos'                          + jetname ) *
         getattr( process, 'softPFElectronsTagInfos'                      + jetname ) *
         getattr( process, 'pfCombinedMVAV2BJetTags'                      + jetname ) *
         getattr( process, 'pfImpactParameterTagInfosAK8'                 + jetname ) *
         getattr( process, 'pfInclusiveSecondaryVertexFinderTagInfosAK8'  + jetname ) *
         getattr( process, 'pfBoostedDoubleSecondaryVertexAK8BJetTags'    + jetname ) *
         getattr( process, 'pfInclusiveSecondaryVertexFinderCvsLTagInfos' + jetname ) *
         getattr( process, 'pfCombinedCvsBJetTags'                        + jetname ) *
         getattr( process, 'pfCombinedCvsLJetTags'                        + jetname )
         )
   return ans

def JetCommon( process , jetname ):
   ans = (
         getattr( process , 'patJetCorrFactors' + jetname ) *
         JetBTagPath( process , jetname )*
         getattr( process , 'patJets'           + jetname ) *
         getattr( process , 'selectedPatJets'   + jetname )
         )
   return ans
