import FWCore.ParameterSet.Config as cms

from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import *
# s. https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1196.html
HBHENoiseFilter.minIsolatedNoiseSumE        = 999999.
HBHENoiseFilter.minNumIsolatedNoiseChannels = 999999
HBHENoiseFilter.minIsolatedNoiseSumEt       = 999999.


scrapingFilter = cms.EDFilter(
  "FilterOutScraping"
, applyfilter = cms.untracked.bool( True )
, debugOn     = cms.untracked.bool( False )
, numtrack    = cms.untracked.uint32( 10 )
, thresh      = cms.untracked.double( 0.2 )
)

eventCleaning = cms.Sequence(
  HBHENoiseFilter
+ scrapingFilter
)
