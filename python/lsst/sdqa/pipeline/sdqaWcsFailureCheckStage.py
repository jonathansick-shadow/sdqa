#! /usr/bin/env python 
"""SDQA independent check for WCS failure.
"""
from __future__ import with_statement
import lsst.pex.harness.stage as harnessStage
import lsst.pex.policy as pexPolicy
from lsst.pex.logging import Log, Debug, LogRec, Prop
import lsst.pex.exceptions.exceptionsLib as pexExcept
import lsst.afw.image.imageLib as afwImage
import lsst.afw.detection as afwDetect
import lsst.sdqa.sdqaLib as sdqa


class SdqaWcsFailureCheckStageParallel(harnessStage.ParallelProcessing):
    """ SdqaWcsFailureCheckStage:

        Inputs:
            Policy (parameters.*):
                minMatches (mininum required number of matches between                                       
                            extracted and astrometric-reference sources)
                maxRmsRadDist (maximum allowed RMS radial distance between
                               extracted and astrometric-reference sources,
                               in arcseconds)
                matchRadius (maximum allowed radius for matches between 
                             extracted sources and astrometric-reference 
                             sources, in arcseconds)
                minStellarity (minimum stellarity for selection of extracted sources; 
                               dimensionless)
                nLocalBkgSigma (number of standard deviations above background required 
                                of peak pixel in source extraction, for selection of 
                                extracted sources; dimensionless)
            Clipboard:
                ccdExposureId (ccdMetadataKey.ccdExposureId)
                Set of extracted sources from CCD image (inputKeys.extractedSourceSetKey)
                Set of astrometric-reference sources (inputKeys.refCatSourceSetKey)

        Outputs:
            Clipboard (outputKeys.astromVerifSdqaRatingsKey):
                SDQA ratings to be persisted in database:
                    Number of matches (nAstromVerifMatches)
                    RMS radial distance (astromVerifRmsRadDist)   

        Assumptions:
            1. Reference sources have been pre-filtered to have the following properties:
               a. Bright (range of magnitudes must cover a variety of backgrounds)
               b. Isolated
               c. High stellarity
               d. Non-confused
               e. Very low proper motion
               f. Non-diffraction-spike origin (in case of USNOB1 catalog)
               g. Optical band appropriate for image data
            2. List of reference sources sorted by R.A. is preferable (but not required).
            3. Reference sources are packaged in lsst.afw.detection.SourceSet object.
            4. List of extracted sources sorted by R.A. is preferable (but not required).
            5. Extracted sources are packaged in lsst.afw.detection.SourceSet object.

        Attributes of extracted sources:
            R.A. (degrees)
            Dec. (degrees)
            Peak pixel (D.N.; e.g., SExtractor MU_MAX divided by pixel area in arcseconds^2)
            Stellarity (dimensionless value between 0 and 1)
            Local background (D.N.)
            Local background standard deviation (D.N.; e.g., derivable from SExtractor MU_THRESHOLD etc.)
            Confusion flag (e.g., SExtractor bits 0 and 1)
            Saturated flag (e.g., SExtractor bit 2)
            Image-edge flag (e.g., SExtractor bit 3)
            Corruption flag (e.g., SExtractor bit 4)

        Attributes of reference sources:
            R.A. (degrees)
            Dec. (degrees)

        Selection criteria for extracted sources:
            Stellarity > minStellarity
            Flags = 0 (i.e., no confusion, saturation, edge effects, or corruption)
            Peak > threshold (= local background plus nLocalBkgSigma times local background std. dev.)

        Processing flow:
            Read ccdExposureId from clipboard.
            Read extracted sources from clipboard.
            Apply selection criteria to extracted sources.
            Sort extracted sources by R.A.
            Query database for astrometric-reference sources.
            Sort reference sources by R.A.
            Match extracted sources with reference sources.
            Compute radial root-mean-squared (RMS) radial distance of the matches, in arcseconds.
            Put SDQA ratings on clipboard for database storage: nAstromVerifMatches and astromVerifRmsRadDist
            Raise exception if nAstromVerifMatches < minMatches.
            Raise exception if astromVerifRmsRadDist > maxRmsRadDist.
    """
    def setup(self):
        """Configure this stage with a policy and then log the set-up information.
        """
        defaults = pexPolicy.DefaultPolicyFile("sdqa",  
                                               "sdqaWcsFailureCheckStagePolicyDict.paf", 
                                               "policy"
                                              )
        defPolicy = pexPolicy.Policy.createPolicy(defaults, defaults.getRepositoryPath())
        if self.policy is None:
            self.policy = defPolicy
        else:
            self.policy.mergeDefaults(defPolicy)

        self.minMatches = self.policy.get("parameters.minMatches")
        self.maxRmsRadDist = self.policy.get("parameters.maxRmsRadDist")
        self.matchRadius = self.policy.get("parameters.matchRadius")
        self.minStellarity = self.policy.get("parameters.minStellarity")
        self.nLocalBkgSigma = self.policy.get("parameters.nLocalBkgSigma")
        self.ccdMetadataKey = self.policy.get("inputKeys.ccdMetadataKey")
        self.extractedSourceSetKey = self.policy.get("inputKeys.extractedSourceSetKey")
        self.refCatSourceSetKey = self.policy.get("inputKeys.refCatSourceSetKey")
        self.astromVerifSdqaRatingsKey = self.policy.get("outputKeys.astromVerifSdqaRatingsKey")

        self.log = Debug(self.log, "SdqaWcsFailureCheckStage")
        self.log.log(Log.INFO, "Minimum number of matches required: %i"% self.minMatches)
        self.log.log(Log.INFO, "Maximum RMS radial distance allowed: %f"% self.maxRmsRadDist)
        self.log.log(Log.INFO, "Match radius: %f"% self.matchRadius)
        self.log.log(Log.INFO, "Required minimum stellarity for extracted sources: %f"% self.minStellarity)
        self.log.log(Log.INFO, "N local background sigma: %f"% self.nLocalBkgSigma)
        self.log.log(Log.INFO, "Clipboard name of ccdMetadata: %s"% self.ccdMetadataKey)
        self.log.log(Log.INFO, "Clipboard name of extractedSourceSet: %s"% self.extractedSourceSetKey)
        self.log.log(Log.INFO, "Clipboard name of refCatSourceSet: %s"% self.refCatSourceSetKey)
        self.log.log(Log.INFO, \
                     "Clipboard name of astromVerifSdqaRatings: %s"% self.astromVerifSdqaRatingsKey)

        self.log.debug(3, "Running with sliceID = %s" % self.getRank())

    def process(self, clipboard):

        if clipboard is not None:
            if not clipboard.contains(self.extractedSourceSetKey):
                raise RuntimeError("Missing extractedSourceSet on clipboard")
            if not clipboard.contains(self.refCatSourceSetKey):
                raise RuntimeError("Missing refCatSourceSet on clipboard")
            if not clipboard.contains(self.ccdMetadataKey):
                raise RuntimeError("Missing ccdMetadata on clipboard")

        propertySet = clipboard.get(self.ccdMetadataKey)
        ccdExposureId = propertySet.getInt("ccdExposureId")

        extractedSourceSet = clipboard.get(self.extractedSourceSetKey)

        refCatSourceSet = clipboard.get(self.refCatSourceSetKey)

        self.log.log(Log.INFO, "ccdExposureId = %s"% ccdExposureId)

        self.log.debug(3, "ccdExposureId = %s"% ccdExposureId)

        if self.log.sends(Log.DEBUG):
            LogRec(self.log, Log.DEBUG) << "All input data found."           \
                             << Prop("Number of extracted-source positions", len(extractedSourceSet))   \
                             << LogRec.endr

        # Do the work.

        # Apply selection critera to extracted sources.
        # Sort extracted sources by R.A.
        # Query database for astrometric-reference sources.
        # Sort reference sources by R.A.
        # Match extracted sources with reference sources.
        # Compute radial root-mean-squared (RMS) radial distance of the matches, in arcseconds.
        # E.g., the results are
        # nMatches = 456789
        rmsRadDist = 0.456789

        sourceMatches = afwDetect.matchRaDec(extractedSourceSet, refCatSourceSet, self.matchRadius)
        nMatches = len(sourceMatches)

        for i in range(nMatches):
            print "Matches: ", \
                  sourceMatches[i][0].getRa(), \
                  sourceMatches[i][0].getDec(), \
                  sourceMatches[i][1].getRa(), \
                  sourceMatches[i][1].getDec()
  
        self.log.log(Log.INFO, "Number of matches: %i"% nMatches)
        self.log.log(Log.INFO, "RMS radial distance: %f"% rmsRadDist)

        # Put SDQA ratings on clipboard.
        sdqaRatings = sdqa.SdqaRatingSet()
        sdqaRatings.append(sdqa.SdqaRating("nAstromVerifMatches", \
                                            nMatches,  0, sdqa.SdqaRating.CCD))
        sdqaRatings.append(sdqa.SdqaRating("astromVerifRmsRadDist", \
                                            rmsRadDist,  0.0, sdqa.SdqaRating.CCD))

        persistableSdqaRatingVector = sdqa.PersistableSdqaRatingVector(sdqaRatings)
        clipboard.put(self.astromVerifSdqaRatingsKey, persistableSdqaRatingVector)

        if self.log.sends(Log.DEBUG):
            LogRec(self.log, Log.DEBUG) << "Astrometric-verification results:" \
                                        << Prop("ccdExposureId", ccdExposureId) \
                                        << Prop("nAstromVerifMatches", nMatches) \
                                        << Prop("astromVerifRmsRadDist", rmsRadDist) \
                                        << LogRec.endr

        # Raise exceptions if tests fail.
        if nMatches < self.minMatches:
            raise pexExcept.LsstCppException("asdf")
        if rmsRadDist > self.maxRmsRadDist:
            raise pexExcept.LsstCppException


# Required with SimpleStageTester.
class SdqaWcsFailureCheckStage(harnessStage.Stage):
    parallelClass = SdqaWcsFailureCheckStageParallel


if __name__ == "__main__":
    main()
