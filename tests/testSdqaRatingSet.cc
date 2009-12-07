// -*- lsst-c++ -*-

/**
 * \file SdqaRatingSet_1.cc
 *
 * \ingroup sdqa
 *
 * \brief Test suite for SdqaRatingSet of the SdqaRating class. 
 *
 * \author Russ Laher, IPAC
 */

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE SdqaRatingSet_1 
#endif // BOOST_TEST_MODULE

#include <iostream>
#include <string>

#include "lsst/sdqa/SdqaRating.h"
#include "lsst/pex/exceptions.h"

#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"
#include "boost/format.hpp"

using std::cout;
using std::endl;
using std::string;

namespace sdqa = lsst::sdqa;
namespace test = boost::test_tools;

BOOST_AUTO_TEST_SUITE(SdqaRatingSetSuite) /* parasoft-suppress LsstDm-3-6a  LsstDm-3-2c LsstDm-4-6 "Macro expands to non-LSST code" */

BOOST_AUTO_TEST_CASE(test1) { /* parasoft-suppress LsstDm-3-1 LsstDm-5-25 LsstDm-4-6 "Macro expands to non-LSST code" */

    sdqa::SdqaRatingSet srs;
    int nVec = 3;

    srs.clear();
    srs.reserve(nVec);
    string n1 = "test1";
    sdqa::SdqaRating::RatingScope t1 = sdqa::SdqaRating::AMP;

    // Initialize vector of SdqaRating objects.

    for (int i = 0; i < nVec; ++i) {
        int v1 = i;
        double e1 = 0.1 * static_cast<double>(i);
        sdqa::SdqaRating::Ptr ptr(
            new sdqa::SdqaRating(n1, static_cast<double>(v1), e1, t1)
                                 );
        srs.push_back(ptr);
    }

    // Perform tests.

    BOOST_CHECK_EQUAL(static_cast<double>(srs.size()), nVec);

    sdqa::SdqaRatingSet::size_type i;
    for (i = 0; i < srs.size(); ++i) {
        double v1 = static_cast<double>(i);
        double e1 = 0.1 * static_cast<double>(i);
        sdqa::SdqaRating rsr = *srs[i];
        string rn1 = rsr.getName();
        sdqa::SdqaRating::RatingScope rt1 = rsr.getRatingScope();
        double rv1 = rsr.getValue();
        double re1 = rsr.getErr();
        BOOST_CHECK_EQUAL(n1, rn1);
        BOOST_CHECK_EQUAL(t1, rt1);
        BOOST_CHECK_EQUAL(v1, rv1);
        BOOST_CHECK_EQUAL(e1, re1);
    }
}

BOOST_AUTO_TEST_SUITE_END()


