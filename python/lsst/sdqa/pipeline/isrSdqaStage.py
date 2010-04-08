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
        self.nBadCalibPixKey = self.policy.get("inputKeys.nBadCalibPixKey")
        self.nSaturatePixKey = self.policy.get("inputKeys.nSaturatePixKey")
        self.overscanMeanKey = self.policy.get("inputKeys.overscanMeanKey")
        self.overscanMeanUncKey = self.policy.get("inputKeys.overscanMeanUncKey")
        self.overscanStdDevKey = self.policy.get("inputKeys.overscanStdDevKey")
        self.overscanMedianKey = self.policy.get("inputKeys.overscanMedianKey")
        self.imageMedianKey = self.policy.get("inputKeys.imageMedianKey")
        self.imageMinKey = self.policy.get("inputKeys.imageMinKey")
        self.imageMaxKey = self.policy.get("inputKeys.imageMaxKey")
        self.xImageGradientKey = self.policy.get("inputKeys.xImageGradientKey")
        self.yImageGradientKey = self.policy.get("inputKeys.yImageGradientKey")

        self.sdqaRatingScope = self.policy.get("parameters.sdqaRatingScope")

        self.isrPersistableSdqaRatingVectorKey = self.policy.get("outputKeys.isrPersistableSdqaRatingVectorKey")

        self.log = Debug(self.log, "IsrSdqaStage")
        self.log.log(Log.INFO, "Clipboard name of exposure: %s"% self.exposureKey)
        self.log.log(Log.INFO, \
                     "Clipboard name of isrPersistableSdqaRatingVector: %s"% self.isrPersistableSdqaRatingVectorKey)

    def process(self, clipboard):

        if clipboard is not None:
            if not clipboard.contains(self.exposureKey):
                raise RuntimeError("Missing exposure on clipboard")
            if not clipboard.contains(self.nBadCalibPixKey):
                raise RuntimeError("Missing nBadCalibPix on clipboard")
            if not clipboard.contains(self.nSaturatePixKey):
                raise RuntimeError("Missing nSaturatePix on clipboard")
            if not clipboard.contains(self.overscanMeanKey):
                raise RuntimeError("Missing overscanMean on clipboard")
            if not clipboard.contains(self.overscanMeanUncKey):
                raise RuntimeError("Missing overscanMeanUnc on clipboard")
            if not clipboard.contains(self.overscanStdDevKey):
                raise RuntimeError("Missing overscanStdDev on clipboard")
            if not clipboard.contains(self.overscanMedianKey):
                raise RuntimeError("Missing overscanMedian on clipboard")
            if not clipboard.contains(self.imageMedianKey):
                raise RuntimeError("Missing imageMedian on clipboard")
            if not clipboard.contains(self.imageMinKey):
                raise RuntimeError("Missing imageMin on clipboard")
            if not clipboard.contains(self.imageMaxKey):
                raise RuntimeError("Missing imageMax on clipboard")
            if not clipboard.contains(self.xImageGradientKey):
                raise RuntimeError("Missing xImageGradient on clipboard")
            if not clipboard.contains(self. yImageGradientKey):
                raise RuntimeError("Missing  yImageGradient on clipboard")
            if not clipboard.contains(self.isrPersistableSdqaRatingVectorKey):
                raise RuntimeError("Missing isrPersistableSdqaRatingVector on clipboard")

        exposure = clipboard.get(self.exposureKey)
        propertySet = exposure.getMetadata()
        nBadCalibPix = propertySet.getAsDouble(self.nBadCalibPixKey)
        nSaturatePix = propertySet.getAsDouble(self.nSaturatePixKey)
        overscanMean = propertySet.getAsDouble(self.overscanMeanKey)
        overscanMeanUnc = propertySet.getAsDouble(self.overscanMeanUncKey)
        overscanStdDev = propertySet.getAsDouble(self.overscanStdDevKey)
        overscanMedian = propertySet.getAsDouble(self.overscanMedianKey)
        imageMedian = propertySet.getAsDouble(self.imageMedianKey)
        imageMin = propertySet.getAsDouble(self.imageMinKey)
        imageMax = propertySet.getAsDouble(self.imageMaxKey)
        xImageGradient = propertySet.getAsDouble(self.xImageGradientKey)
        yImageGradient = propertySet.getAsDouble(self.yImageGradientKey)

        # Put SDQA ratings on clipboard.
        sdqaRatings = sdqa.SdqaRatingSet()
        sdqaRatings.append(sdqa.SdqaRating(self.nBadCalibPixKey, \
                                           nBadCalibPix,  0, self.sdqaRatingScope))
        sdqaRatings.append(sdqa.SdqaRating(self.nSaturatePixKey, \
                                           nSaturatePix,  0, self.sdqaRatingScope))
        sdqaRatings.append(sdqa.SdqaRating(self.overscanMeanKey, \
                                           overscanMean,  overscanMeanUnc, self.sdqaRatingScope))
        sdqaRatings.append(sdqa.SdqaRating(self.overscanStdDevKey, \
                                           overscanStdDev,  0.0, self.sdqaRatingScope))
        sdqaRatings.append(sdqa.SdqaRating(self.overscanMedianKey, \
                                           overscanMedian,  0.0, self.sdqaRatingScope))
        sdqaRatings.append(sdqa.SdqaRating(self.imageMedianKey, \
                                           imageMedian,  0.0, self.sdqaRatingScope))
        sdqaRatings.append(sdqa.SdqaRating(self.imageMinKey, \
                                           imageMin,  0.0, self.sdqaRatingScope))
        sdqaRatings.append(sdqa.SdqaRating(self.imageMaxKey, \
                                           imageMax,  0.0, self.sdqaRatingScope))
        sdqaRatings.append(sdqa.SdqaRating(self.xImageGradientKey, \
                                           xImageGradient,  0.0, self.sdqaRatingScope))
        sdqaRatings.append(sdqa.SdqaRating(self.yImageGradientKey, \
                                           yImageGradient,  0.0, self.sdqaRatingScope))

        persistableSdqaRatingVector = sdqa.PersistableSdqaRatingVector(sdqaRatings)
        clipboard.put(self.isrPersistableSdqaRatingVectorKey, persistableSdqaRatingVector)

# Required with SimpleStageTester.
class IsrSdqaStage(harnessStage.Stage):
    parallelClass = IsrSdqaStageParallel

if __name__ == "__main__":
    main()

