#! /usr/bin/env python

import unittest

import lsst.sdqa.sdqaLib as sdqa

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

