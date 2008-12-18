import unittest

import lsst.sdqa as sdqa
from lsst.pex.exceptions import LsstCppException

class SdqaRatingTestCase(unittest.TestCase):
    """A test case for SdqaRating."""

    def testDefaultConstructor(self):
        sr = sdqa.SdqaRating()
        self.assert_(sr is not None)

    def testLoadConstructor(self):
        sr = sdqa.SdqaRating("testlc", 17.5, 12.3, sdqa.SdqaRating.FPA)
        self.assert_(sr is not None)

    def testInt(self):
        sr = sdqa.SdqaRating()
        n = "testInt"
        t = sdqa.SdqaRating.AMP
        v = 19
        e = 1
        sr.set(n, v, e, t)

        self.assertEqual(sr.getName(), "testInt")
        self.assertEqual(sr.getValue(), 19)
        self.assertEqual(sr.getErr(), 1)
        self.assertEqual(sr.getRatingScope(), sdqa.SdqaRating.AMP)

    def testInt64(self):
        n = "testInt64"
        t = sdqa.SdqaRating.CCD
        v = 0xfeeddeadbeefL
        e = 0xdeadbeeffeedL
        sr = sdqa.SdqaRating(n, v, e, t)

        self.assertEqual(sr.getName(), "testInt64")
        self.assertEqual(sr.getValue(), 0xfeeddeadbeefL)
        self.assertEqual(sr.getErr(), 0xdeadbeeffeedL)
        self.assertEqual(sr.getRatingScope(), sdqa.SdqaRating.CCD)

    def testDouble(self):
        n = "testDouble"
        t = sdqa.SdqaRating.FPA
        v = 2.718281828459045
        e = 0.111111111111111
        sr = sdqa.SdqaRating(n, v, e, t)

        self.assertEqual(sr.getName(), "testDouble")
        self.assertEqual(sr.getValue(), 2.718281828459045)
        self.assertEqual(sr.getErr(), 0.111111111111111)
        self.assertEqual(sr.getRatingScope(), sdqa.SdqaRating.FPA)

    def testExceptionHandling1(self):
        n = "testExceptionHandling1"
        t = sdqa.SdqaRating.INVALID
        v = 33.33
        e = 0.22

        self.assertRaises(LsstCppException, sdqa.SdqaRating, n, v, e, t)
        
    def testExceptionHandling2(self):
        n = "testExceptionHandling2"
        t = sdqa.SdqaRating.INVALID
        v = 55.55
        e = 0.44
        sr = sdqa.SdqaRating()

        self.assertRaises(LsstCppException, sr.set, n, v, e, t)

if __name__ == '__main__':
    unittest.main()


