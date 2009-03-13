// -*- lsst-c++ -*-

/** 
 * \file SdqaRating.h
 *
 * \ingroup sdqa
 *
 * \brief Header file for SdqaRating class.  
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

#include "lsst/daf/base/Citizen.h"
#include "lsst/daf/base/Persistable.h"

#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"

namespace boost {
    namespace serialization {
        class access;
    }
}


namespace lsst {

    namespace sdqa {

        class SdqaRatingVectorFormatter;


/**
 * Class for containing an SDQA rating. 
 */

class SdqaRating { 

public:
    typedef boost::shared_ptr<SdqaRating> Ptr;

    enum RatingScope {INVALID = -1, AMP, CCD, FPA, FOOTPRINT};

    SdqaRating();

    SdqaRating(SdqaRating const & other);  

    SdqaRating(std::string metricName, double metricValue, 
	       double metricErr, RatingScope ratingScope);

    ~SdqaRating(); 

    void set(std::string metricName, double metricValue, 
	     double metricErr, RatingScope ratingScope);

    std::string    getName() const;
    double         getValue() const;
    double         getErr() const;
    RatingScope    getRatingScope() const;

    SdqaRating & operator = (SdqaRating const & other);

    bool operator == (SdqaRating const & other) const;

private:

    std::string    _metricName;
    double         _metricValue;
    double         _metricErr;
    RatingScope    _ratingScope;    // 0=Amp, 1=CCD, 2=FPA (see enum RatingScope).
    boost::int64_t _parentDbId;     // ampExposureId, ccdExposureId, exposureId, etc.
    int            _sdqa_metricId;
    int            _sdqa_thresholdId;

    void setParentDbId(boost::int64_t parentDbId);
    void setsetSdqaMetricId(int sdqa_metricId);
    void setSdqaThresholdId(int sdqa_thresholdId);

    boost::int64_t getParentDbId() const;
    int getsetSdqaMetricId() const;
    int getsetSdqaThresholdId() const;

    template <typename Archive> 
    void serialize(Archive & ar, unsigned int const version);

    friend class boost::serialization::access;
    friend class SdqaRatingVectorFormatter;   

};


/**
 * Define an inline overloaded operator for testing inequality between
 *  two SdqaRating objects. 
 */

inline bool operator != (const SdqaRating & lhs, const SdqaRating & rhs) {
    return ! (lhs == rhs);
}


/**
 * Define an alias for a vector of SdqaRating objects. 
 */

typedef std::vector<SdqaRating::Ptr> SdqaRatingSet;


/**
 * Class for persisting a vector of SdqaRating objects. 
 */

class PersistableSdqaRatingVector : public lsst::daf::base::Persistable {

public:
    typedef boost::shared_ptr<PersistableSdqaRatingVector> Ptr;

    PersistableSdqaRatingVector();

    PersistableSdqaRatingVector(SdqaRatingSet const & sdqaRatings);

    ~PersistableSdqaRatingVector();
        
    void setSdqaRatings(SdqaRatingSet const & sdqaRatings);
    
    SdqaRatingSet getSdqaRatings() const;

    //bool operator == (SdqaRatingSet const & other) const;

bool operator == 
    (SdqaRatingSet const & other) const {
    if (_sdqaRatings.size() != other.size())
        return false;
                    
    SdqaRatingSet::size_type i;
    for (i = 0; i < _sdqaRatings.size(); ++i) {
        if (*_sdqaRatings[i] != *other[i])
            return false;            
    }
        
    return true;
}



    bool operator == (PersistableSdqaRatingVector const & other) const;

private:

    LSST_PERSIST_FORMATTER(lsst::sdqa::SdqaRatingVectorFormatter);
    SdqaRatingSet _sdqaRatings;
}; 

    } // namespace sdqa

} // namespace lsst

#endif // LSST_SDQARATING_H

