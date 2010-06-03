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
import lsst.sdqa as sdqa
import lsst.sdqa.pipeline as sdqaPipe
import lsst.afw.image as afwImage

class IsrSdqaStageTestCase(unittest.TestCase):
    """A test case for IsrSdqaStage.py"""

    def setUp(self):
        self.v = [x**1.1 for x in range(40)]

    def tearDown(self):
        pass

    def testPipeline(self):
        policyFile = pexPolicy.DefaultPolicyFile("sdqa", 
                                                 "isrSdqaStagePolicy.paf", "tests")
        policy = pexPolicy.Policy.createPolicy(policyFile)

        tester = SimpleStageTester(sdqaPipe.IsrSdqaStage(policy.get("IsrSdqaStage")))

        clipboard = pexClipboard.Clipboard()

        exposureKey = policy.get("IsrSdqaStage.inputKeys.exposureKey")

        sdqaMetricNames = policy.getArray("IsrSdqaStage.parameters.sdqaMetricNames")

        exposure = afwImage.ExposureF()
        metadata = exposure.getMetadata()

        # Load metadata.  Assume FITS round-tripping converts some metadata names
        # to all upper case.

        for i in xrange(len(sdqaMetricNames)):
            if i % 2 == 0:
                metadata.setDouble(sdqaMetricNames[i], self.v[i])
            else:
                metadata.setDouble(sdqaMetricNames[i].upper(), self.v[i])

        exposure.setMetadata( metadata )

        clipboard.put(exposureKey, exposure)

        tester.runWorker(clipboard)

        res = clipboard.get(policy.get("IsrSdqaStage.outputKeys.isrPersistableSdqaRatingVectorKey"))

        sdqaRatingScope = policy.get("IsrSdqaStage.parameters.sdqaRatingScope")

        print "get ====="
        for i in xrange(len(sdqaMetricNames)):
            s2 = res.getSdqaRatings()[i]
            print i, "[", s2.getName(), "]", s2.getValue(), s2.getErr(), s2.getRatingScope()
        assert(s2.getName().upper()  == sdqaMetricNames[i].upper())
        assert(s2.getValue() == self.v[i])
        assert(s2.getErr()   == 0.0)
        assert(s2.getRatingScope() == sdqaRatingScope)
 
def suite():
    """Returns a suite containing all the test cases in this module."""

    utilsTests.init()

    suites = []

    if not eups.productDir("sdqa"):
        print >> sys.stderr, "sdqa is not setting up; skipping test"
    else:        
        suites += unittest.makeSuite(IsrSdqaStageTestCase)

    suites += unittest.makeSuite(utilsTests.MemoryTestCase)
    return unittest.TestSuite(suites)

def run(exit=False):
    """Run the tests"""
    utilsTests.run(suite(), exit)

if __name__ == "__main__":
    run(True)


