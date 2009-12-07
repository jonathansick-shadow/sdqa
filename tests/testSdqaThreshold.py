#! /usr/bin/env python

import unittest

import lsst.sdqa as sdqa

class SdqaThresholdTestCase(unittest.TestCase):
    """A test case for SdqaThreshold."""

    def testDefaultConstructor(self):
        st = sdqa.SdqaThreshold()
        self.assert_(st is not None)

    def testLoadConstructor(self):
        st = sdqa.SdqaThreshold(12, 34, 12.3, 4.56, "2009-01-15 11:58:00.678")
        self.assert_(st is not None)

    def test1(self):
        st = sdqa.SdqaThreshold()
        a = 123
        b = 345
        c = 1.234e30
        d = -87.342515
        e = "2009-01-16 11:58:00.678"
        st.set(a, b, c, d, e)

        self.assertEqual(st.getThresholdId(), 123)
        self.assertEqual(st.getMetricId(), 345)
        self.assertEqual(st.getUpperThreshold(), 1.234e30)
        self.assertEqual(st.getLowerThreshold(), -87.342515)
        self.assertEqual(st.getCreatedDate(), "2009-01-16 11:58:00.678")

if __name__ == '__main__':
    unittest.main()

