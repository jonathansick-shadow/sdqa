// -*- lsst-c++ -*-

/** 
 * \file SdqaRatingFormatter.h
 *
 * \ingroup sdqa
 *
 * \brief Header file for SdqaRatingFormatter class.  
 *
 *        A formatter that supports persistence and retrieval with
 *
 *             - lsst::daf::persistence::DbStorage
 *             - lsst::daf::persistence::DbTsvStorage
 *             - lsst::daf::persistence::BoostStorage
 *             - lsst::daf::persistence::XmlStorage
 *
 *        for PersistableSdqaRatingVector instances.
 *
 * \author Russ Laher, IPAC
 */

#ifndef LSST_SDQA_SDQARATINGFORMATTER_H
#define LSST_SDQA_SDQARATINGFORMATTER_H

#include <string>
#include <vector>

#include "lsst/daf/base.h"
#include "lsst/daf/persistence.h"
#include "lsst/daf/base/PropertySet.h"
#include "lsst/daf/persistence/LogicalLocation.h"
#include "lsst/pex/policy/Policy.h"
#include "lsst/sdqa/SdqaRating.h"

namespace lsst {

    namespace sdqa {

class SdqaRatingVectorFormatter : public lsst::daf::persistence::Formatter {

public:

    virtual ~SdqaRatingVectorFormatter();

    virtual void write(
        lsst::daf::base::Persistable const *,
        lsst::daf::persistence::Storage::Ptr,
        lsst::daf::base::PropertySet::Ptr
    );
    virtual lsst::daf::base::Persistable* read(
        lsst::daf::persistence::Storage::Ptr,
        lsst::daf::base::PropertySet::Ptr
    );
    virtual void update(
        lsst::daf::base::Persistable*,
        lsst::daf::persistence::Storage::Ptr,
        lsst::daf::base::PropertySet::Ptr
    );

    template <class Archive>
    static void delegateSerialize(
        Archive &,
        unsigned int const,
        lsst::daf::base::Persistable *
    );

private:


    /*
     * ordered list of columns in appropriate Sdqa_Rating_xxx database table 
     * of the DC3b schema (omitting SDQA_RATING_ID, which is database-generated 
     * via auto-increment.
     */

    enum Columns {
        SDQA_METRIC_ID,
        SDQA_THRESHOLD_ID,
        AMP_EXPOSURE_ID,
        METRICVALUE,
        METRICERR,
    };

    lsst::pex::policy::Policy::Ptr _policy;

    explicit SdqaRatingVectorFormatter(lsst::pex::policy::Policy::Ptr const & policy);

    static lsst::daf::persistence::Formatter::Ptr createInstance(
        lsst::pex::policy::Policy::Ptr policy
    );

    static lsst::daf::persistence::FormatterRegistration registration;

    template <typename T>
    static void insertRow(
        T &,
        lsst::sdqa::SdqaRating const &,
        std::string columnNameOfImageId
    );

    static void setupFetch(
        lsst::daf::persistence::DbStorage &,
        lsst::sdqa::SdqaRating &,
        std::string columnNameOfImageId
    );

    int64_t extractExposureId(lsst::daf::base::PropertySet::Ptr const& properties);
    int64_t extractCcdExposureId(lsst::daf::base::PropertySet::Ptr const& properties);
    int64_t extractAmpExposureId(lsst::daf::base::PropertySet::Ptr const& properties);

    std::string const
        extractSdqaRatingScope(lsst::daf::base::PropertySet::Ptr const & properties);
};


    } // namespace sdqa

} // namespace lsst

#endif // LSST_SDQA_SDQARATINGFORMATTER_H



