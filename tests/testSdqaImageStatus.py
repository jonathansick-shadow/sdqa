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

class SdqaImageStatusTestCase(unittest.TestCase):
    """A test case for SdqaImageStatus."""

    def testDefaultConstructor(self):
        sis = sdqa.SdqaImageStatus()
        self.assert_(sis is not None)

    def testLoadConstructor(self):
        sis = sdqa.SdqaImageStatus(456, "Failed", "The image has failed SDQA.")
        self.assert_(sis is not None)

    def test1(self):
        sis = sdqa.SdqaImageStatus()
        a = 123
        b = "Passed"
        c = "The image has passed SDQA."
        sis.set(a, b, c)

        self.assertEqual(sis.getImageStatusId(), 123)
        self.assertEqual(sis.getName(), "Passed")
        self.assertEqual(sis.getDefinition(), "The image has passed SDQA.")

if __name__ == '__main__':
    unittest.main()

