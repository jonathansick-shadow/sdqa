// -*- lsst-c++ -*-

/** 
 * \file SdqaRating.h
 *
 * \ingroup sdqa
 *
 * \brief Header file for class SdqaRating.  
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

#ifndef LSST_SDQARATING_H
#define LSST_SDQARATING_H

#include <string>
#include <vector>

#include "boost/shared_ptr.hpp"

namespace lsst {

    namespace sdqa {


/**
* Class for containing an SDQA rating. 
*/

class SdqaRating { 


public:
    typedef boost::shared_ptr<SdqaRating> Ptr;

    enum RatingScope {INVALID = -1, AMP, CCD, FPA};

    SdqaRating();

    SdqaRating(std::string metricName, double metricValue, 
	       double metricErr, RatingScope ratingScope);

    ~SdqaRating(); 

    void set(std::string metricName, double metricValue, 
	     double metricErr, RatingScope ratingScope);
 
    std::string getName() const;
    double      getValue() const;
    double      getErr() const;
    RatingScope getRatingScope() const;

private:

    std::string  _metricName;
    double       _metricValue;
    double       _metricErr;
    RatingScope  _ratingScope;    // 0=Amp, 1=CCD, 2=FPA (see enum RatingScope).
};

typedef std::vector<SdqaRating::Ptr> SdqaRatingSet;

    } // namespace sdqa

} // namespace lsst

#endif // LSST_SDQARATING_H
