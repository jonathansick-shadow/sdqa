#! /usr/bin/env python 
"""Harvest metadata for SDQA.
"""
from __future__ import with_statement
import lsst.pex.harness.stage as harnessStage
import lsst.pex.policy as pexPolicy
from lsst.pex.logging import Log, Debug, LogRec, Prop
import lsst.pex.exceptions.exceptionsLib as pexExcept
import lsst.sdqa.sdqaLib as sdqa


class IsrSdqaStageParallel(harnessStage.ParallelProcessing):
    """ IsrSdqaStage:

        Inputs:
            Policy:
                Clipboard key of exposure
                Clipboard keys of SDQA ratings
            Clipboard:
                Exposure
                SDQA ratings associated with exposure

        Outputs:
            Clipboard (outputKeys.isrPersistableSdqaRatingVectorKey):
                SDQA ratings to be persisted in database, contained in persistable SDQA rating vector 
    """
    def setup(self):
        """Configure this stage with a policy and then log the set-up information.
        """
        defaults = pexPolicy.DefaultPolicyFile("sdqa",  
                                               "isrSdqaStageDictionary.paf", 
                                               "policy"
                                              )
        defPolicy = pexPolicy.Policy.createPolicy(defaults, defaults.getRepositoryPath())
        if self.policy is None:
            self.policy = defPolicy
        else:
            self.policy.mergeDefaults(defPolicy)

        self.exposureKey = self.policy.get("inputKeys.exposureKey")

        self.sdqaRatingScope = self.policy.get("parameters.sdqaRatingScope")

        self.sdqaMetricNames = self.policy.getStringArray("parameters.sdqaMetricNames")

        self.isrPersistableSdqaRatingVectorKey = self.policy.get("outputKeys.isrPersistableSdqaRatingVectorKey")

        self.log = Debug(self.log, "IsrSdqaStage")
        self.log.log(Log.INFO, "Clipboard name of exposure: %s"% self.exposureKey)
        self.log.log(Log.INFO, \
                     "Clipboard name of isrPersistableSdqaRatingVector: %s"% self.isrPersistableSdqaRatingVectorKey)
        self.log.log(Log.INFO, "Number of sdqaMetricNames: %d"% len(self.sdqaMetricNames))
        for k in self.sdqaMetricNames:
            self.log.log(Log.INFO, "SDQA metric name: %s"% k)

    def process(self, clipboard):

        if clipboard is not None:
            if not clipboard.contains(self.exposureKey):
                raise RuntimeError("Missing exposure on clipboard")

        exposure = clipboard.get(self.exposureKey)
        propertySet = exposure.getMetadata()

        v = {}
        for m in self.sdqaMetricNames:
           v[m] = propertySet.getAsDouble(m)

        # Put SDQA ratings on clipboard.
        sdqaRatings = sdqa.SdqaRatingSet()
        for m in self.sdqaMetricNames:
            sdqaRatings.append(sdqa.SdqaRating(m, v[m], 0, self.sdqaRatingScope))
        
        persistableSdqaRatingVector = sdqa.PersistableSdqaRatingVector(sdqaRatings)
        clipboard.put(self.isrPersistableSdqaRatingVectorKey, persistableSdqaRatingVector)

# Required with SimpleStageTester.
class IsrSdqaStage(harnessStage.Stage):
    parallelClass = IsrSdqaStageParallel

if __name__ == "__main__":
    main()

