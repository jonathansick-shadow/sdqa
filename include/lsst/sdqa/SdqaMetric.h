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
 * \file SdqaMetric.h
 *
 * \ingroup sdqa
 *
 * \brief Header file for class SdqaMetric.  
 *
 *        SdqaMetric provides a container for a record from the 
 *        SDQA_Metric database table.
 *
 *        An exception is thrown if the metric's data type is not
 *        an allowed value.
 *
 * \author Russ Laher, IPAC
 */

#ifndef LSST_SDQAMETRIC_H
#define LSST_SDQAMETRIC_H

#include <string>

namespace lsst {

    namespace sdqa {


/**
* Class for containing an SDQA metric. 
*/

class SdqaMetric { 

public:
    enum DataType {INVALID = -1, FLOAT, INT};

    SdqaMetric();

    SdqaMetric(int         sdqaMetricId, 
               std::string metricName, 
               std::string physicalUnits, 
               DataType    dataType,
               std::string definition);

    ~SdqaMetric(); 

    void set(int         sdqaMetricId, 
             std::string metricName, 
             std::string physicalUnits, 
             DataType    dataType,
             std::string definition);
 
    int         getId() const;
    std::string getName() const; 
    std::string getPhysicalUnits() const; 
    DataType    getDataType() const;
    std::string getDefinition() const;

private:
    int         _sdqaMetricId;
    std::string _metricName;
    std::string _physicalUnits;
    DataType    _dataType;   // 0=FLOAT, 1=INT (see enum declaration).
    std::string _definition;
};

    } // namespace sdqa

} // namespace lsst

#endif // LSST_SDQAMETRIC_H

