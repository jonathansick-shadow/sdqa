// -*- lsst-c++ -*-

/* 
 * LSST Data Management System
 * Copyright 2008, 2009, 2010 LSST Corporation.
 * 
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the LSST License Statement and 
 * the GNU General Public License along with this program.  If not, 
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */
 

/**
 * \file SdqaImageStatus_1.cc
 *
 * \ingroup sdqa
 *
 * \brief Test suite for the SdqaImageStatus class. 
 *
 * \author Russ Laher, IPAC
 */

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SdqaImageStatus_1
#endif // BOOST_TEST_MODULE

#include <iostream>
#include <string>

#include "lsst/sdqa/SdqaImageStatus.h"

#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"
#include "boost/format.hpp"

using std::cout;
using std::endl;
using std::string;

namespace sdqa = lsst::sdqa;
namespace testSdqaImageStatus = boost::test_tools;

BOOST_AUTO_TEST_SUITE(SdqaImageStatusSuite) /* parasoft-suppress LsstDm-3-6a  LsstDm-3-2c LsstDm-4-6 "Macro expands to non-LSST code" */

BOOST_AUTO_TEST_CASE(test1) { /* parasoft-suppress LsstDm-3-1 LsstDm-5-25 LsstDm-4-6 "Macro expands to non-LSST code" */
    int i1 = 1234;
    string n1 = "marginallyPassedAuto";
    string d1 = "Image marginally passed by automated SDQA.";
    sdqa::SdqaImageStatus st1(i1, n1, d1);
    int ri1 = st1.getImageStatusId();
    string rn1 = st1.getName();
    string rd1 = st1.getDefinition();

    BOOST_CHECK_EQUAL(i1, ri1);
    BOOST_CHECK_EQUAL(n1, rn1);
    BOOST_CHECK_EQUAL(d1, rd1);
}

BOOST_AUTO_TEST_CASE(test2) { /* parasoft-suppress LsstDm-3-1 LsstDm-5-25 LsstDm-4-6 "Macro expands to non-LSST code" */
    int i2 = 32767;
    string n2 = "marginallyFailedManual";
    string d2 = "Image marginally failed by manual SDQA.";
    sdqa::SdqaImageStatus sis;
    sis.set(i2, n2, d2);
    int ri2 = sis.getImageStatusId();
    string rn2 = sis.getName();
    string rd2 = sis.getDefinition();

    BOOST_CHECK_EQUAL(i2, ri2);
    BOOST_CHECK_EQUAL(n2, rn2);
    BOOST_CHECK_EQUAL(d2, rd2);
}

BOOST_AUTO_TEST_SUITE_END()
