#!/usr/bin/env python
"""
Tests for C++ SdqaRating and SdqaRatingVector Python wrappers (including persistence)

Run with:
   python SdqaRatingFormatter_1.py
or
   python
   >>> import unittest; T=load("SdqaRatingFormatter_1"); unittest.TextTestRunner(verbosity=1).run(T.suite())
"""
import pdb
import unittest
import random
import time

import lsst.daf.base as dafBase
import lsst.pex.policy as dafPolicy
import lsst.daf.persistence as dafPers
import lsst.utils.tests as utilsTests
import lsst.sdqa as SDQA

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

class SdqaRatingTestCase(unittest.TestCase):
    """A test case for SdqaRating and PersistableSdqaRatingVector"""

    metricNames = [ 'cMedian1',
                    'cMedian2', 
                    'cMedian3', 
                    'cMedian4', 
                    'fwhmSex', 
                    'gKurtos', 
                    'gMAbsDev', 
                    'gMaxVal', 
                    'gMean', 
                    'gMedian', 
                    'gMinVal', 
                    'gMode', 
                    'gSkewns', 
                    'gStdDev', 
                    'MmFlag', 
                    'nDeadPix'] 

    def setUp(self):
        self.container1 = SDQA.SdqaRatingSet(16)
        self.container2 = SDQA.SdqaRatingSet()
        
        for m in xrange(16):
            ds = SDQA.SdqaRating()
            ds.set(self.metricNames[m], m * 10 + 1234, m * 0.1 + 0.1234, ds.AMP)
            self.container1[m] = ds
            
            ds = SDQA.SdqaRating()
            ds.set("nObjPix", 1234, 0.6789, ds.AMP)
            self.container2.push_back(ds)

        self.dsv1 = SDQA.PersistableSdqaRatingVector(self.container1)
        self.dsv2 = SDQA.PersistableSdqaRatingVector(self.container2)

    def tearDown(self):
        del self.dsv1
        del self.dsv2

    def testIterable(self):
        """Check that we can iterate over a SdqaRatingSet"""
        j = 0
        container = self.container1[:]
        for s in container:
            assert s.getName() == self.metricNames[j]
            assert s.getValue() == j * 10 + 1234
            assert s.getErr() == j * 0.1 + 0.1234
            assert s.getRatingScope() == s.AMP
            j += 1

    def testCopyAndCompare(self):
        dsv1Copy = self.dsv1.getSdqaRatings()
        dsv2Copy = self.dsv2.getSdqaRatings()
        
        assert dsv1Copy.size() == self.container1.size()
        for i in xrange(dsv1Copy.size()):
            assert dsv1Copy[i] == self.container1[i]        
        assert dsv2Copy.size() == self.container2.size()
        for i in xrange(dsv2Copy.size()):
            assert dsv2Copy[i] == self.container2[i]

        dsv1Copy.swap(dsv2Copy)
        assert dsv2Copy.size() == self.container1.size()
        for i in xrange(dsv2Copy.size()):
            assert dsv2Copy[i] == self.container1[i]           
        assert dsv1Copy.size() == self.container2.size()
        for i in xrange(dsv1Copy.size()):
            assert dsv1Copy[i] == self.container2[i]
            
        dsv1Copy.swap(dsv2Copy)
        
        if dsv1Copy.size() == 0:
            ds = SDQA.SdqaRating()
            dsv1Copy.append(ds)
        else:
            dsv1Copy.pop()
        ds = SDQA.SdqaRating()
        dsv2Copy.append(ds)
        
        assert dsv1Copy.size() != self.container1.size()
        assert dsv2Copy.size() != self.container2.size()

    def testInsertErase(self):
        container =  self.dsv1.getSdqaRatings()
        
        front = container[:8]
        back = container[8:]

        copy = SDQA.SdqaRatingSet()
        
        for i in xrange(front.size()):
            copy.append(front[i])
            
        ds = SDQA.SdqaRating()
        for i in xrange(4):
            copy.append(ds)
        
        for i in xrange(back.size()):
            copy.append(back[i])
                    
        del copy[8]
        del copy[8:11]
        assert copy.size() == self.container1.size()
        for i in xrange(copy.size()):
            assert copy[i] == self.container1[i]       

    def testSlice(self):
        containerSlice = self.dsv1.getSdqaRatings()[0:3]
        
        j = 0
        for s in containerSlice:
            assert s.getName() == self.metricNames[j]
            j += 1

    def testPersistence(self):
        if dafPers.DbAuth.available():
            pol  = dafPolicy.Policy()
            pol.set("Formatter.PersistableSdqaRatingVector.SdqaRating.templateTableName", "SdqaRating")
            pol.set("Formatter.PersistableSdqaRatingVector.SdqaRating.perVisitTableNamePattern", "_tmp_visit%1%_SdqaRating")
            pers = dafPers.Persistence.getPersistence(pol)
            loc  = dafPers.LogicalLocation("mysql://lsst10.ncsa.uiuc.edu:3306/russ")
            dp = dafBase.PropertySet()
            dp.addInt("visitId", int(time.clock())*16384 + random.randint(0,16383))
            dp.addInt("sliceId", 0)
            dp.addInt("ampExposureId", 1234)
            dp.addInt("numSlices", 1)
            dp.addString("sdqaRatingScope", "AMP")

            stl = dafPers.StorageList()
            stl.append(pers.getPersistStorage("DbStorage", loc))
            pers.persist(self.dsv1, stl, dp)
            stl = dafPers.StorageList()
            stl.append(pers.getRetrieveStorage("DbStorage", loc))
            persistable = pers.unsafeRetrieve("PersistableSdqaRatingVector", stl, dp)
            res = SDQA.PersistableSdqaRatingVector.swigConvert(persistable)


            containerSlice = self.dsv1.getSdqaRatings()[0:16]
            print "put ===="        
            j = 0
            for s in containerSlice:
                print j, "[", s.getName(), "]", s.getValue(), s.getErr(), s.getRatingScope()
                j += 1

            containerSlice2 = res.getSdqaRatings()[0:16]
            print "get ===="
            j = 0
            for s2 in containerSlice2:
                print j, "[", s2.getName(), "]", s2.getValue(), s2.getErr(), s2.getRatingScope()
                j += 1

            print "end ===="

            for m in xrange(16):
                print "Comparing metricName for m = ", m, " [", self.dsv1.getSdqaRatings()[m].getName(), "] [", res.getSdqaRatings()[m].getName(), "]" 
                a = self.dsv1.getSdqaRatings()[m].getName()
                b = res.getSdqaRatings()[m].getName()
                if a == b:
                    print "The metricNames are equal.\n"
                else:
                    print "The metricNames are NOT EQUAL.\n"
                print "Comparing metricValue for m = ", m, " [", self.dsv1.getSdqaRatings()[m].getValue(), "] [", res.getSdqaRatings()[m].getValue(), "]"
                a = self.dsv1.getSdqaRatings()[m].getValue()
                b = res.getSdqaRatings()[m].getValue()
                if a == b:
                    print "The metricValues are equal.\n"
                else:
                    print "The metricValues are NOT EQUAL.\n"
                assert(self.dsv1.getSdqaRatings()[m].getName()  == res.getSdqaRatings()[m].getName())
                assert(self.dsv1.getSdqaRatings()[m].getValue() == res.getSdqaRatings()[m].getValue())
                assert(self.dsv1.getSdqaRatings()[m].getErr()   == res.getSdqaRatings()[m].getErr())

            """The following statement will fail because the returned SDQA Ratings have extra info."""
            #assert(res == self.dsv1)

        else:

            print "skipping database tests"


#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

def suite():
    """Returns a suite containing all the test cases in this module."""

    utilsTests.init()

    suites = []
    suites += unittest.makeSuite(SdqaRatingTestCase)
    suites += unittest.makeSuite(utilsTests.MemoryTestCase)
    return unittest.TestSuite(suites)

if __name__ == "__main__":
    utilsTests.run(suite())

