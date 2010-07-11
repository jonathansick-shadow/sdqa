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
 * \file SdqaMetric.cc
 *
 * \ingroup sdqa
 *
 * \brief Source file for class SdqaMetric.  
 *
 *        SdqaMetric provides a container for a record from the 
 *        SDQA_Metric database table.
 *
 *        An exception is thrown if the metric's data type is not
 *        an allowed value.
 *
 * \author Russ Laher, IPAC
 */

#include "lsst/sdqa/SdqaMetric.h"
#include "lsst/pex/exceptions.h"

namespace sdqa = lsst::sdqa;


/**
 * Basic constructor.
 *
 * Initializes class attribute _dataType to -1, an invalid value.
 * The set function must be subsequently called to set the class
 * attributes, as well as reset _dataType to a valid value.  
 * Valid _dataType values are 0=FLOAT and 1=INT.
 */

sdqa::SdqaMetric::SdqaMetric() : _dataType(INVALID) {}


/**
 *  Constructor to explicitly initialize class attributes.
 *
 * \param sdqaMetricId is the sdqa_Metric database table ID.
 * \param metricName is the name of the SDQA metric.
 * \param physicalUnits is a string defining the metric's data units.
 * \param dataType is a data-type flag for the metric's values.
 * \param definition is a descriptive definition string for the metric.
 *
 */

sdqa::SdqaMetric::SdqaMetric(int         sdqaMetricId, 
                       std::string metricName, 
                       std::string physicalUnits, 
                       DataType    dataType,
                       std::string definition) {
    set(sdqaMetricId, 
        metricName, 
        physicalUnits, 
        dataType,
        definition);
}

sdqa::SdqaMetric::~SdqaMetric() {
}


/**
 * Set class attributes, and raise exception for invalid dataType.
 *
 * \param sdqaMetricId is the sdqa_Metric database table ID.
 * \param metricName is the name of the SDQA metric.
 * \param physicalUnits is a string defining the metric's data units.
 * \param dataType is a data-type flag for the metric's values.
 * \param definition is a descriptive definition string for the metric.
 *
 * \throws Exception if dataType is an invalid value (not 0 or 1).
 */

void sdqa::SdqaMetric::set(int         sdqaMetricId, 
                     std::string metricName, 
                     std::string physicalUnits, 
                     DataType    dataType,
                     std::string definition) {
    if (! ((dataType == FLOAT) || 
           (dataType == INT))) {
        throw LSST_EXCEPT(lsst::pex::exceptions::InvalidParameterException, 
            "Error: Input dataType has invalid value.");
    } else {
        _sdqaMetricId  = sdqaMetricId;
        _metricName    = metricName;
        _physicalUnits = physicalUnits;
        _dataType      = dataType;
        _definition    = definition;    
    }
}


int sdqa::SdqaMetric::getId() const {
    return _sdqaMetricId;
}

std::string sdqa::SdqaMetric::getName() const {
    return _metricName;
}

std::string sdqa::SdqaMetric::getPhysicalUnits() const {
    return _physicalUnits;
}

sdqa::SdqaMetric::DataType sdqa::SdqaMetric::getDataType() const {
    return _dataType;
}

std::string sdqa::SdqaMetric::getDefinition() const {
    return _definition;
}
