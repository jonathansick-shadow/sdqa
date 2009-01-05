// -*- lsst-c++ -*-

/**
 * \file SdqaMetric_1.cc
 *
 * \ingroup sdqa
 *
 * \brief Test suite for the SdqaMetric class. 
 *
 * \author Russ Laher, IPAC
 */

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

#include "lsst/sdqa/SdqaMetric.h"
#include "lsst/pex/exceptions.h"

using namespace lsst::sdqa;

#define BOOST_TEST_MODULE SdqaMetric_1
#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"

#include "boost/format.hpp"

namespace testSdqaMetric = boost::test_tools;

BOOST_AUTO_TEST_SUITE(SdqaMetricSuite)

BOOST_AUTO_TEST_CASE(test1) {
    int i1 = 1234;
    string n1 = "img.stat.mean";
    string p1 = "MJy/sr";
    SdqaMetric::DataType t1 = SdqaMetric::FLOAT;
    string d1 = "Mean of good pixel values";
    SdqaMetric sm1;
    sm1.set(i1, n1, p1, t1, d1);
    int ri1 = sm1.getId();
    string rn1 = sm1.getName();
    string rp1 = sm1.getPhysicalUnits();
    SdqaMetric::DataType rt1 = sm1.getDataType();
    string rd1 = sm1.getDefinition();

    BOOST_CHECK_EQUAL(i1, ri1);
    BOOST_CHECK_EQUAL(n1, rn1);
    BOOST_CHECK_EQUAL(p1, rp1);
    BOOST_CHECK_EQUAL(t1, rt1);
    BOOST_CHECK_EQUAL(d1, rd1);
}

BOOST_AUTO_TEST_CASE(test2) {
    SdqaMetric s;
    BOOST_CHECK_THROW(s.set(4321,
                            string("img.stat.stdDev"), 
                            string("MJy/sr"), 
			    SdqaMetric::INVALID,
                            string("Standard deviation of good pixels")), 
		      lsst::pex::exceptions::InvalidParameterException);
    BOOST_CHECK_THROW(new SdqaMetric(567, 
                                     string("img.stat.badCalibPixN"), 
                                     string("counts"), 
				     SdqaMetric::INVALID, 
		                     string("Number of bad pixels")),
		      lsst::pex::exceptions::InvalidParameterException);
}

BOOST_AUTO_TEST_SUITE_END()

