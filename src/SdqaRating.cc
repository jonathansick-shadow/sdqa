// -*- lsst-c++ -*-

/** 
 * \file SdqaRating.cc
 *
 * \ingroup sdqa
 *
 * \brief Source file for class SdqaRating.  
 *
 *        SdqaRating provides a container for an SDQA rating, which consists
 *        of a metricName, metricValue, metricErr, and a database-table flag.
 *
 *        The database-table flag is of data type RatingScope (defined in the
 *        SdqaRating header file), and allowed values are 0, 1, and 2, 
 *        corresponding to amplifier, CCD, or FPA for the image level of the 
 *        SDQA rating, respectively.  An exception is thrown if the flag is 
 *        not an allowed value.
 *
 * \author Russ Laher, IPAC
 */

#include "lsst/sdqa/SdqaRating.h"
#include "lsst/pex/exceptions.h"

namespace sdqa = lsst::sdqa;

/**
 * Basic constructor.
 *
 * Initializes class attribute _ratingScope to -1, an invalid value.
 * The set function must be subsequently called to set the class
 * attributes, as well as reset _ratingScope to a valid value.  
 * Valid _ratingScope values are 0=Amp, 1=CCD, 2=FPA.
 */

sdqa::SdqaRating::SdqaRating() : _ratingScope(INVALID) {}


/**
 *  Constructor to explicitly initialize class attributes.
 *
 * \param metricName is the name of the SDQA metric.
 * \param metricValue is the value of the SDQA metric.
 * \param metricErr is the uncertainty associated with metricValue.
 * \param ratingScope is a flag that tells the formatter in which 
 *        database table the SDQA-Rating record is to be stored.
 *
 */

sdqa::SdqaRating::SdqaRating(std::string metricName, double metricValue, 
		       double metricErr, RatingScope ratingScope) {
    set(metricName, metricValue, metricErr, ratingScope);
}

sdqa::SdqaRating::~SdqaRating() {
}


/**
 * Set class attributes, and raise exception for invalid ratingScope.
 *
 * \param metricName is the name of the SDQA metric.
 * \param metricValue is the value of the SDQA metric.
 * \param metricErr is the uncertainty associated with metricValue.
 * \param ratingScope is a flag that tells the formatter in which 
 *        database table the SDQA-Rating record is to be stored.
 *
 * \throws Exception if ratingScope is an invalid value (not 0, 1, or 2).
 */

void sdqa::SdqaRating::set(std::string metricName, double metricValue, 
		     double metricErr, RatingScope ratingScope) {
    if (! ((ratingScope == AMP) || 
           (ratingScope == CCD) ||
           (ratingScope == FPA))) {
        throw LSST_EXCEPT(lsst::pex::exceptions::InvalidParameterException, 
      	    "Error: Input ratingScope has invalid value.");
    } else {
        _ratingScope = ratingScope;
        _metricName  = metricName;
        _metricValue = metricValue;  
        _metricErr   = metricErr;    
    }
}


std::string sdqa::SdqaRating::getName() const {
    return _metricName;
}


double sdqa::SdqaRating::getValue() const {
    return _metricValue;
}


double sdqa::SdqaRating::getErr() const {
    return _metricErr;
}


sdqa::SdqaRating::RatingScope sdqa::SdqaRating::getRatingScope() const {
    return _ratingScope;
}
