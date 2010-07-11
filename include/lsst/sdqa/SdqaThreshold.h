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
 * \file SdqaThreshold.h
 *
 * \ingroup sdqa
 *
 * \brief Header file for class SdqaThreshold.  
 *
 *        SdqaThreshold provides a container for a record from the 
 *        SDQA_Threshold database table.
 *
 * \author Russ Laher, IPAC
 */

#ifndef LSST_SDQATHRESHOLD_H
#define LSST_SDQATHRESHOLD_H

#include <string>

namespace lsst {

    namespace sdqa {


/**
* Class for containing an SDQA threshold. 
*/

class SdqaThreshold { 

public:
    SdqaThreshold();

    SdqaThreshold(int         sdqaThresholdId, 
                  int         sdqaMetricId,
                  double      upperThreshold,
                  double      lowerThreshold,
                  std::string createdDate);

    ~SdqaThreshold(); 

    void set(int         sdqaThresholdId, 
             int         sdqaMetricId,
             double      upperThreshold,
             double      lowerThreshold,
             std::string createdDate);
 
    int         getThresholdId() const;
    int         getMetricId() const;
    double      getUpperThreshold() const;
    double      getLowerThreshold() const;
    std::string getCreatedDate() const; 

private:
    int         _sdqaThresholdId;
    int         _sdqaMetricId;
    double      _upperThreshold;
    double      _lowerThreshold;
    std::string _createdDate;
};

    } // namespace sdqa

} // namespace lsst

#endif // LSST_SDQATHRESHOLD_H

