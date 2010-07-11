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
 * \file SdqaThreshold_1.cc
 *
 * \ingroup sdqa
 *
 * \brief Test suite for the SdqaThreshold class. 
 *
 * \author Russ Laher, IPAC
 */

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SdqaThreshold_1 
#endif // BOOST_TEST_MODULE

#include <iostream>
#include <string>

#include "lsst/sdqa/SdqaThreshold.h"

#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"
#include "boost/format.hpp"

using std::cout;
using std::endl;
using std::string;

namespace sdqa = lsst::sdqa;
namespace testSdqaThreshold = boost::test_tools;

BOOST_AUTO_TEST_SUITE(SdqaThresholdSuite) /* parasoft-suppress LsstDm-3-6a  LsstDm-3-2c LsstDm-4-6 "Macro expands to non-LSST code" */

BOOST_AUTO_TEST_CASE(test1) { /* parasoft-suppress LsstDm-3-1 LsstDm-5-25 LsstDm-4-6 "Macro expands to non-LSST code" */
    int i1 = 1234;
    int j1 = 5678;
    double u1 = 1.234;
    double l1 = 5.678;
    string c1 = "2008-12-11 10:22:29.601242";
    sdqa::SdqaThreshold st1(i1, j1, u1, l1, c1);
    int ri1 = st1.getThresholdId();
    int rj1 = st1.getMetricId();
    double ru1 = st1.getUpperThreshold();
    double rl1 = st1.getLowerThreshold();
    string rc1 = st1.getCreatedDate();

    BOOST_CHECK_EQUAL(i1, ri1);
    BOOST_CHECK_EQUAL(j1, rj1);
    BOOST_CHECK_EQUAL(u1, ru1);
    BOOST_CHECK_EQUAL(l1, rl1);
    BOOST_CHECK_EQUAL(c1, rc1);
}

BOOST_AUTO_TEST_CASE(test2) { /* parasoft-suppress LsstDm-3-1 LsstDm-5-25 LsstDm-4-6 "Macro expands to non-LSST code" */
    int i2 = 5555;
    int j2 = 7777;
    double u2 = 5.555;
    double l2 = 7.777;
    string c2 = "2009-12-11 10:22:29.601242";
    sdqa::SdqaThreshold st2;
    st2.set(i2, j2, u2, l2, c2);
    int ri2 = st2.getThresholdId();
    int rj2 = st2.getMetricId();
    double ru2 = st2.getUpperThreshold();
    double rl2 = st2.getLowerThreshold();
    string rc2 = st2.getCreatedDate();

    BOOST_CHECK_EQUAL(i2, ri2);
    BOOST_CHECK_EQUAL(j2, rj2);
    BOOST_CHECK_EQUAL(u2, ru2);
    BOOST_CHECK_EQUAL(l2, rl2);
    BOOST_CHECK_EQUAL(c2, rc2);
}

BOOST_AUTO_TEST_SUITE_END()
