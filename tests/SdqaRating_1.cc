// -*- lsst-c++ -*-

/**
 * \file SdqaRating_1.cc
 *
 * \ingroup sdqa
 *
 * \brief Test suite for the SdqaRating class. 
 *
 * \author Russ Laher, IPAC
 */

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SdqaRating_1 
#endif // BOOST_TEST_MODULE

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

#include "lsst/sdqa/SdqaRating.h"
#include "lsst/pex/exceptions.h"

#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"
#include "boost/format.hpp"

namespace sdqa = lsst::sdqa;
namespace test = boost::test_tools;

BOOST_AUTO_TEST_SUITE(SdqaRatingSuite) /* parasoft-suppress LsstDm-3-6a  LsstDm-3-2c LsstDm-4-6 "Macro expands to non-LSST code" */

BOOST_AUTO_TEST_CASE(test1) { /* parasoft-suppress LsstDm-3-1 LsstDm-5-25 LsstDm-4-6 "Macro expands to non-LSST code" */
    string n1 = "test1";
    sdqa::SdqaRating::RatingScope t1 = sdqa::SdqaRating::AMP;
    int v1 = 19;
    int e1 = 1;
    sdqa::SdqaRating sr1;
    sr1.set(n1, static_cast<double>(v1), static_cast<double>(e1), t1);
    string rn1 = sr1.getName();
    double rv1 = sr1.getValue();
    double re1 = sr1.getErr();
    sdqa::SdqaRating::RatingScope rt1 = sr1.getRatingScope();

    BOOST_CHECK_EQUAL(n1, rn1);
    BOOST_CHECK_EQUAL(v1, rv1);
    BOOST_CHECK_EQUAL(e1, re1);
    BOOST_CHECK_EQUAL(t1, rt1);
}

BOOST_AUTO_TEST_CASE(test2) { /* parasoft-suppress LsstDm-3-1 LsstDm-5-25 LsstDm-4-6 "Macro expands to non-LSST code" */
    sdqa::SdqaRating sr;
    BOOST_CHECK_THROW(sr.set(string("test2a"), 
                             2.2, 
                             3.3, 
                             sdqa::SdqaRating::INVALID), 
                      lsst::pex::exceptions::InvalidParameterException);
    BOOST_CHECK_THROW(new sdqa::SdqaRating(string("test2b"), 
                                           4.9, 
                                           5.8, 
                                           sdqa::SdqaRating::INVALID), 
                      lsst::pex::exceptions::InvalidParameterException);
}

BOOST_AUTO_TEST_SUITE_END()
