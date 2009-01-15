#! /usr/bin/env python

import unittest

import lsst.sdqa as sdqa
from lsst.pex.exceptions import LsstCppException

class SdqaMetricTestCase(unittest.TestCase):
    """A test case for SdqaMetric."""

    def testDefaultConstructor(self):
        sm = sdqa.SdqaMetric()
        self.assert_(sm is not None)

    def testLoadConstructor(self):
        sm = sdqa.SdqaMetric(123, "img.stat.goodPixels", "counts", sdqa.SdqaMetric.INT, "definition")
        self.assert_(sm is not None)

    def testFloat(self):
        sm = sdqa.SdqaMetric()
        n = 456
        v = "img.stat.stdDev"
        e = "MJy/sr"
        t = sdqa.SdqaMetric.FLOAT
        d = "This is the time for all good men to come to the aid of their country."
        sm.set(n, v, e, t, d)

        self.assertEqual(sm.getId(), 456)
        self.assertEqual(sm.getName(), "img.stat.stdDev")
        self.assertEqual(sm.getPhysicalUnits(), "MJy/sr")
        self.assertEqual(sm.getDataType(), sdqa.SdqaMetric.FLOAT)
        self.assertEqual(sm.getDefinition(), "This is the time for all good men to come to the aid of their country.")

    def testInt(self):
        n = 111
        v = "img.stat.badPixels"
        e = "number"
        t = sdqa.SdqaMetric.INT
        d = "asdf" 
        sm = sdqa.SdqaMetric(n, v, e, t, d)

        self.assertEqual(sm.getId(), 111)
        self.assertEqual(sm.getName(), "img.stat.badPixels")
        self.assertEqual(sm.getPhysicalUnits(), "number")
        self.assertEqual(sm.getDataType(), sdqa.SdqaMetric.INT)
        self.assertEqual(sm.getDefinition(), "asdf")

    def testExceptionHandling1(self):
        n = 222
        v = "speed"
        e = "m/sec"
        t = sdqa.SdqaMetric.INVALID
        d = "xyz"

        self.assertRaises(LsstCppException, sdqa.SdqaMetric, n, v, e, t, d)
        
    def testExceptionHandling2(self):
        n = 333
        v = "acceleration"
        e = "m/sec2"
        t = sdqa.SdqaMetric.INVALID
        d = "domo arigato"
        sm = sdqa.SdqaMetric()

        self.assertRaises(LsstCppException, sm.set, n, v, e, t, d)

if __name__ == '__main__':
    unittest.main()

