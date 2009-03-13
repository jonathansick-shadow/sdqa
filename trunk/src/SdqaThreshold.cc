// -*- lsst-c++ -*-

/** 
 * \file SdqaThreshold.cc
 *
 * \ingroup sdqa
 *
 * \brief Source file for class SdqaThreshold.  
 *
 *        SdqaThreshold provides a container for a record from the 
 *        SDQA_Threshold database table.
 *
 * \author Russ Laher, IPAC
 */

#include "lsst/sdqa/SdqaThreshold.h"

namespace sdqa = lsst::sdqa;


/**
 * Basic constructor.
 *
 * Initializes class attributes to zero or null, as appropriate. 
 * The set function must be subsequently called to set the class 
 * attributes.
 */

sdqa::SdqaThreshold::SdqaThreshold() : _sdqaThresholdId(0), 
                                 _sdqaMetricId(0),  
                                 _upperThreshold(0.0), 
                                 _lowerThreshold(0.0),  
                                 _createdDate("") {}  


/**
 *  Constructor to explicitly initialize class attributes.
 *
 * \param sdqaThresholdId is the sdqa_Threshold database table ID.
 * \param sdqaMetricId is the associated sdqa_Metric database table ID.
 * \param upperThreshold is the SDQA metric's upper threshold.
 * \param lowerThreshold is the SDQA metric's lower threshold.
 * \param createdDate is the sdqa_Threshold database record's creation date.
 *
 * Either upperThreshold or lowerThreshold may be set to NaN if the value 
 * in the corresponding database record is null.
 */

sdqa::SdqaThreshold::SdqaThreshold(int         sdqaThresholdId, 
                             int         sdqaMetricId,
                             double      upperThreshold,
                             double      lowerThreshold,
                             std::string createdDate) : 
    _sdqaThresholdId(sdqaThresholdId), 
    _sdqaMetricId(sdqaMetricId),  
    _upperThreshold(upperThreshold), 
    _lowerThreshold(lowerThreshold),  
    _createdDate(createdDate) {}  


sdqa::SdqaThreshold::~SdqaThreshold() {
}


/**
 * Set class attributes.
 *
 * \param sdqaThresholdId is the sdqa_Threshold database table ID.
 * \param sdqaMetricId is the associated sdqa_Metric database table ID.
 * \param upperThreshold is the SDQA metric's upper threshold.
 * \param lowerThreshold is the SDQA metric's lower threshold.
 * \param createdDate is the sdqa_Threshold database record's creation date.
 *
 * Either upperThreshold or lowerThreshold may be set to NaN if the value 
 * in the corresponding database record is null.
 */

void sdqa::SdqaThreshold::set(int         sdqaThresholdId, 
                        int         sdqaMetricId,
                        double      upperThreshold,
                        double      lowerThreshold,
                        std::string createdDate) {
        _sdqaThresholdId  = sdqaThresholdId;
        _sdqaMetricId     = sdqaMetricId;
        _upperThreshold   = upperThreshold;
        _lowerThreshold   = lowerThreshold;
        _createdDate      = createdDate;
}


int sdqa::SdqaThreshold::getThresholdId() const {
    return _sdqaThresholdId;
}

int sdqa::SdqaThreshold::getMetricId() const {
    return _sdqaMetricId;
}

double sdqa::SdqaThreshold::getUpperThreshold() const {
    return _upperThreshold;
}

double sdqa::SdqaThreshold::getLowerThreshold() const {
    return _lowerThreshold;
}

std::string sdqa::SdqaThreshold::getCreatedDate() const {
    return _createdDate;
}
