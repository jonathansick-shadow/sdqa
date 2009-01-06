// -*- lsst-c++ -*-

/**
 * \file SdqaImageStatus_1.cc
 *
 * \ingroup sdqa
 *
 * \brief Test suite for the SdqaImageStatus class. 
 *
 * \author Russ Laher, IPAC
 */

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

#include "lsst/sdqa/SdqaImageStatus.h"

using namespace lsst::sdqa;

#define BOOST_TEST_MODULE SdqaImageStatus_1
#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"

#include "boost/format.hpp"

namespace testSdqaImageStatus = boost::test_tools;

BOOST_AUTO_TEST_SUITE(SdqaImageStatusSuite)

BOOST_AUTO_TEST_CASE(test1) {
    int i1 = 1234;
    string n1 = "marginallyPassedAuto";
    string d1 = "Image marginally passed by automated SDQA.";
    SdqaImageStatus st1(i1, n1, d1);
    int ri1 = st1.getImageStatusId();
    string rn1 = st1.getName();
    string rd1 = st1.getDefinition();

    BOOST_CHECK_EQUAL(i1, ri1);
    BOOST_CHECK_EQUAL(n1, rn1);
    BOOST_CHECK_EQUAL(d1, rd1);
}

BOOST_AUTO_TEST_CASE(test2) {
    int i2 = 32767;
    string n2 = "marginallyFailedManual";
    string d2 = "Image marginally failed by manual SDQA.";
    SdqaImageStatus sis;
    sis.set(i2, n2, d2);
    int ri2 = sis.getImageStatusId();
    string rn2 = sis.getName();
    string rd2 = sis.getDefinition();

    BOOST_CHECK_EQUAL(i2, ri2);
    BOOST_CHECK_EQUAL(n2, rn2);
    BOOST_CHECK_EQUAL(d2, rd2);
}

BOOST_AUTO_TEST_SUITE_END()

