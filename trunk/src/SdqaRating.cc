// -*- lsst-c++ -*-

/** 
 * \file SdqaRating.cc
 *
 * \ingroup sdqa
 *
 * \brief Implementation file for SdqaRating class.  
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


/*******************
 * SdqaRating class.
 */

/**
 * Basic constructor for SdqaRating class.
 *
 * Initializes class attribute _ratingScope to -1, an invalid value.
 * The set function must be subsequently called to set the class
 * attributes, as well as reset _ratingScope to a valid value.  
 * Valid _ratingScope values are 0=Amp, 1=CCD, 2=FPA.
 */

sdqa::SdqaRating::SdqaRating() : _ratingScope(INVALID), _parentDbId(0) {}


/**
 * Copy constructor for SdqaRating class.
 */

sdqa::SdqaRating::SdqaRating(SdqaRating const & other) {
    set(other._metricName, other._metricValue, other._metricErr, other._ratingScope);
}


/**
 *  Constructor for SdqaRating class to explicitly initialize class attributes.
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


/**
 *  Destructor for SdqaRating class.
 */

sdqa::SdqaRating::~SdqaRating() {}


/**
 *  Member functions for SdqaRating class.
 */

/**
 * Set SdqaRating class attributes, and raise exception for invalid ratingScope.
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
           (ratingScope == FPA) ||
           (ratingScope == FOOTPRINT))) {
        throw LSST_EXCEPT(lsst::pex::exceptions::InvalidParameterException, 
      	    "Error: Input ratingScope has invalid value.");
    } else {
        _ratingScope = ratingScope;
        _metricName  = metricName;
        _metricValue = metricValue;  
        _metricErr   = metricErr;    
    }
}

void sdqa::SdqaRating::setParentDbId(boost::int64_t parentDbId) {
    if (parentDbId < 1) {
        throw LSST_EXCEPT(lsst::pex::exceptions::InvalidParameterException, 
      	    "Error: Input parentDbId must be > zero.");
    } else {
        _parentDbId = parentDbId;
    }
}

void sdqa::SdqaRating::setsetSdqaMetricId(int sdqa_metricId) {
    _sdqa_metricId = sdqa_metricId;
}

void sdqa::SdqaRating::setSdqaThresholdId(int sdqa_thresholdId) {
    _sdqa_thresholdId = sdqa_thresholdId;
}


/**
 * Get SdqaRating class attributes.
 */

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

boost::int64_t sdqa::SdqaRating::getParentDbId() const {
    return _parentDbId;
}

int sdqa::SdqaRating::getsetSdqaMetricId() const {
    return _sdqa_metricId;
}

int sdqa::SdqaRating::getsetSdqaThresholdId() const {
    return _sdqa_thresholdId;
}


/**
 * Overloaded assignment operator for SdqaRating class.
 */

sdqa::SdqaRating & sdqa::SdqaRating::operator = (sdqa::SdqaRating const & other) {
    if (&other != this) {
            set(other._metricName, 
		other._metricValue, 
		other._metricErr, 
		other._ratingScope);
    }
    return *this;
}


/**
 * Overloaded equality operator for SdqaRating class.
 */

bool sdqa::SdqaRating::operator == (sdqa::SdqaRating const & other) const {
    if (_metricName != other._metricName) {
        return false;
    } else if (_metricValue != other._metricValue) {
        return false;
    } else if (_metricErr != other._metricErr) {
        return false;
    } else if (_ratingScope != other._ratingScope) {
        return false;
    } else if (_parentDbId != other._parentDbId) {
        return false;
    } 
    return true;
}


/**
 * Private serialize function for SdqaRating class.
 */

template <typename Archive> 
void sdqa::SdqaRating::serialize(Archive & ar, unsigned int const version) {
    ar & _parentDbId;
    ar & _metricValue;
    ar & _metricErr;
}



/************************************
 * PersistableSdqaRatingVector class.
 */

/**
 * Basic constructor for PersistableSdqaRatingVector class.
 */

sdqa::PersistableSdqaRatingVector::PersistableSdqaRatingVector() {}


/**
 *  Constructor for PersistableSdqaRatingVector class to explicitly initialize 
 *  class attributes.
 */

sdqa::PersistableSdqaRatingVector::PersistableSdqaRatingVector(
    sdqa::SdqaRatingSet const & sdqaRatings) : _sdqaRatings(sdqaRatings) {}


/**
 * Destructor for PersistableSdqaRatingVector class.
 */

sdqa::PersistableSdqaRatingVector::~PersistableSdqaRatingVector() { 
    _sdqaRatings.clear(); 
}


/**
 *  Member functions for PersistableSdqaRatingVector class.
 */

/**
 * Set PersistableSdqaRatingVector class attributes.
 */

void sdqa::PersistableSdqaRatingVector::setSdqaRatings(
    SdqaRatingSet const & sdqaRatings) {
        _sdqaRatings = sdqaRatings; 
}

  
/**
 * Get PersistableSdqaRatingVector class attributes.
 */

sdqa::SdqaRatingSet sdqa::PersistableSdqaRatingVector::getSdqaRatings() const { 
    return _sdqaRatings; 
}


/**
 * Overloaded equality operator for PersistableSdqaRatingVector class.
 */

bool sdqa::PersistableSdqaRatingVector::operator == 
    (sdqa::PersistableSdqaRatingVector const & other) 
    const {
    return other == _sdqaRatings;
}



