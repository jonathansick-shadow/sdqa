#! /usr/bin/env python
"""Unit test of isrSdqaStage.py.
Run with:
   python isrSdqaStageTest.py
or
   python
   >>> isrSdqaStageTest
   >>> isrSdqaStageTest.run()
"""
from __future__ import with_statement
import sys, os
import unittest
import eups
import lsst.utils.tests as utilsTests
import lsst.pex.harness.Clipboard as pexClipboard
import lsst.pex.policy as pexPolicy
import lsst.pex.harness as pexHarness
import lsst.pex.harness.stage as harnessStage
from lsst.pex.harness.simpleStageTester import SimpleStageTester
import lsst.sdqa.sdqaLib as sdqa
import lsst.sdqa.pipeline as sdqaPipe
import lsst.afw.image as afwImage

class IrsSdqaStageTestCase(unittest.TestCase):
    """A test case for IrsSdqaStage.py"""

    def setUp(self):
        self.nBadCalibPix = 123456789.0
        self.nSaturatePix = 987654321.0
        self.overscanMean = 123.4
        self.overscanMeanUnc = 234.5
        self.overscanStdDev = 345.6
        self.overscanMedian = 456.7
        self.imageMedian = 567.8
        self.imageMin = 678.9
        self.imageMax = 789.0
        self.xImageGradient = 890.1
        self.yImageGradient = 901.2

    def tearDown(self):
        pass

    def testPipeline(self):
        policyFile = pexPolicy.DefaultPolicyFile("sdqa", 
                                                 "isrSdqaStagePolicy.paf", "tests")
        policy = pexPolicy.Policy.createPolicy(policyFile)

        tester = SimpleStageTester(sdqaPipe.IrsSdqaStage(policy.get("IrsSdqaStage")))

        clipboard = pexClipboard.Clipboard()

        exposureKey = policy.get("IrsSdqaStage.inputKeys.exposureKey")
        nBadCalibPixKey = policy.get("IrsSdqaStage.inputKeys.nBadCalibPixKey")
        nSaturatePixKey = policy.get("IrsSdqaStage.inputKeys.nSaturatePixKey")
        overscanMeanKey = policy.get("IrsSdqaStage.inputKeys.overscanMeanKey")
        overscanMeanUncKey = policy.get("IrsSdqaStage.inputKeys.overscanMeanUncKey")
        overscanStdDevKey = policy.get("IrsSdqaStage.inputKeys.overscanStdDevKey")
        overscanMedianKey = policy.get("IrsSdqaStage.inputKeys.overscanMedianKey")
        imageMedianKey = policy.get("IrsSdqaStage.inputKeys.imageMedianKey")
        imageMinKey = policy.get("IrsSdqaStage.inputKeys.imageMinKey")
        imageMaxKey = policy.get("IrsSdqaStage.inputKeys.imageMaxKey")
        xImageGradientKey = policy.get("IrsSdqaStage.inputKeys.xImageGradientKey")
        yImageGradientKey = policy.get("IrsSdqaStage.inputKeys.yImageGradientKey")

        exposure = afwImage.ExposureF()
        metadata = afwImage.Metadata()
        metadata.setDouble(nBadCalibPixKey, self.nBadCalibPix)
        metadata.setDouble(nSaturatePixKey, self.nSaturatePix)
        metadata.setDouble(overscanMeanKey, self.overscanMean)
        metadata.setDouble(overscanMeanUncKey, self.overscanMeanUnc)
        metadata.setDouble(overscanStdDevKey, self.overscanStdDev)
        metadata.setDouble(overscanMedianKey, self.overscanMedian)
        metadata.setDouble(imageMedianKey, self.imageMedian)
        metadata.setDouble(imageMinKey, self.imageMin)
        metadata.setDouble(imageMaxKey, self.imageMax)
        metadata.setDouble(xImageGradientKey, self.xImageGradient)
        metadata.setDouble(yImageGradientKey, self.yImageGradient)

        exposure.setMetadata( metadata )

        clipboard.put(exposureKey, exposure)

        tester.runWorker(clipboard)

        res = clipboard.get(policy.get("IrsSdqaStage.outputKeys.isrPersistableSdqaRatingVectorKey"))

        sdqaRatingScope = policy.get("parameters.sdqaRatingScope")

        containerSlice2 = res.getSdqaRatings()[0:10]
        print "get ====="
        j = 0
        for s2 in containerSlice2:
            print j, "[", s2.getName(), "]", s2.getValue(), s2.getErr(), s2.getRatingScope()
            j += 1

        i = 0
        assert(res.getSdqaRatings()[i].getName()  == nBadCalibPixKey)
        assert(res.getSdqaRatings()[i].getValue() == self.nBadCalibPix)
        assert(res.getSdqaRatings()[i].getErr()   == 0.0)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

        i = 1
        assert(res.getSdqaRatings()[i].getName()  == nSaturatePixKey)
        assert(res.getSdqaRatings()[i].getValue() == self.nSaturatePix)
        assert(res.getSdqaRatings()[i].getErr()   == 0.0)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

        i = 2
        assert(res.getSdqaRatings()[i].getName()  == overscanMeanKey)
        assert(res.getSdqaRatings()[i].getValue() == self.overscanMean)
        assert(res.getSdqaRatings()[i].getErr()   == self.overscanMeanUnc)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

        i = 3
        assert(res.getSdqaRatings()[i].getName()  == overscanStdDevKey)
        assert(res.getSdqaRatings()[i].getValue() == self.overscanStdDev)
        assert(res.getSdqaRatings()[i].getErr()   == 0.0)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

        i = 4
        assert(res.getSdqaRatings()[i].getName()  == overscanMedianKey)
        assert(res.getSdqaRatings()[i].getValue() == self.noverscanMedian)
        assert(res.getSdqaRatings()[i].getErr()   == 0.0)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

        i = 5
        assert(res.getSdqaRatings()[i].getName()  == imageMedianKey)
        assert(res.getSdqaRatings()[i].getValue() == self.imageMedian)
        assert(res.getSdqaRatings()[i].getErr()   == 0.0)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

        i = 6
        assert(res.getSdqaRatings()[i].getName()  == imageMinKey)
        assert(res.getSdqaRatings()[i].getValue() == self.imageMin)
        assert(res.getSdqaRatings()[i].getErr()   == 0.0)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

        i = 7
        assert(res.getSdqaRatings()[i].getName()  == imageMaxKey)
        assert(res.getSdqaRatings()[i].getValue() == self.imageMax)
        assert(res.getSdqaRatings()[i].getErr()   == 0.0)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

        i = 8
        assert(res.getSdqaRatings()[i].getName()  == xImageGradientKey)
        assert(res.getSdqaRatings()[i].getValue() == self.xImageGradient)
        assert(res.getSdqaRatings()[i].getErr()   == 0.0)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

        i = 9
        assert(res.getSdqaRatings()[i].getName()  == yImageGradientKey)
        assert(res.getSdqaRatings()[i].getValue() == self.yImageGradient)
        assert(res.getSdqaRatings()[i].getErr()   == 0.0)
        assert(res.getSdqaRatings()[i].getRatingScope() == sdqaRatingScope)

 
def suite():
    """Returns a suite containing all the test cases in this module."""

    utilsTests.init()

    suites = []

    if not eups.productDir("sdqa"):
        print >> sys.stderr, "sdqa is not setting up; skipping test"
    else:        
        suites += unittest.makeSuite(IrsSdqaStageTestCase)

    suites += unittest.makeSuite(utilsTests.MemoryTestCase)
    return unittest.TestSuite(suites)

def run(exit=False):
    """Run the tests"""
    utilsTests.run(suite(), exit)

if __name__ == "__main__":
    run(True)

