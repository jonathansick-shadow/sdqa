// -*- lsst-c++ -*-

/**
 * \file SdqaThreshold_1.cc
 *
 * \ingroup sdqa
 *
 * \brief Test suite for the SdqaThreshold class. 
 *
 * \author Russ Laher, IPAC
 */

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

#include "lsst/sdqa/SdqaThreshold.h"

namespace sdqa = lsst::sdqa;

#define BOOST_TEST_MODULE SdqaThreshold_1
#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"

#include "boost/format.hpp"

namespace testSdqaThreshold = boost::test_tools;

BOOST_AUTO_TEST_SUITE(SdqaThresholdSuite)

BOOST_AUTO_TEST_CASE(test1) {
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

BOOST_AUTO_TEST_CASE(test2) {
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
