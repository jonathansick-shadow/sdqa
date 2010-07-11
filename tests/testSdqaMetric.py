#! /usr/bin/env python

# 
# LSST Data Management System
# Copyright 2008, 2009, 2010 LSST Corporation.
# 
# This product includes software developed by the
# LSST Project (http://www.lsst.org/).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the LSST License Statement and 
# the GNU General Public License along with this program.  If not, 
# see <http://www.lsstcorp.org/LegalNotices/>.
#


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
        self.assertEqual(sm.getDefinition(), 
                         "This is the time for all good men to come to the aid of their country.")

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

