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
 * \file SdqaMetric_1.cc
 *
 * \ingroup sdqa
 *
 * \brief Test suite for the SdqaMetric class. 
 *
 * \author Russ Laher, IPAC
 */

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SdqaMetric_1 
#endif // BOOST_TEST_MODULE

#include <iostream>
#include <string>

#include "lsst/sdqa/SdqaMetric.h"
#include "lsst/pex/exceptions.h"

#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"
#include "boost/format.hpp"

using std::cout;
using std::endl;
using std::string;

namespace sdqa = lsst::sdqa;
namespace testSdqaMetric = boost::test_tools;

BOOST_AUTO_TEST_SUITE(SdqaMetricSuite) /* parasoft-suppress LsstDm-3-6a  LsstDm-3-2c LsstDm-4-6 "Macro expands to non-LSST code" */

BOOST_AUTO_TEST_CASE(test1) { /* parasoft-suppress LsstDm-3-1 LsstDm-5-25 LsstDm-4-6 "Macro expands to non-LSST code" */
    int i1 = 1234;
    string n1 = "img.stat.mean";
    string p1 = "MJy/sr";
    sdqa::SdqaMetric::DataType t1 = sdqa::SdqaMetric::FLOAT;
    string d1 = "Mean of good pixel values";
    sdqa::SdqaMetric sm1;
    sm1.set(i1, n1, p1, t1, d1);
    int ri1 = sm1.getId();
    string rn1 = sm1.getName();
    string rp1 = sm1.getPhysicalUnits();
    sdqa::SdqaMetric::DataType rt1 = sm1.getDataType();
    string rd1 = sm1.getDefinition();

    BOOST_CHECK_EQUAL(i1, ri1);
    BOOST_CHECK_EQUAL(n1, rn1);
    BOOST_CHECK_EQUAL(p1, rp1);
    BOOST_CHECK_EQUAL(t1, rt1);
    BOOST_CHECK_EQUAL(d1, rd1);
}

BOOST_AUTO_TEST_CASE(test2) { /* parasoft-suppress LsstDm-3-1 LsstDm-5-25 LsstDm-4-6 "Macro expands to non-LSST code" */
    sdqa::SdqaMetric s;
    BOOST_CHECK_THROW(s.set(4321,
                            string("img.stat.stdDev"), 
                            string("MJy/sr"), 
                            sdqa::SdqaMetric::INVALID,
                            string("Standard deviation of good pixels")), 
                      lsst::pex::exceptions::InvalidParameterException);
    BOOST_CHECK_THROW(new sdqa::SdqaMetric(567, 
                                           string("img.stat.badCalibPixN"), 
                                           string("counts"), 
                                           sdqa::SdqaMetric::INVALID, 
                                           string("Number of bad pixels")),
                      lsst::pex::exceptions::InvalidParameterException);
}

BOOST_AUTO_TEST_SUITE_END()
