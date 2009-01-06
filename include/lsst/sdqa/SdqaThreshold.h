// -*- lsst-c++ -*-

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

