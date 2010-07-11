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
#include <lsst/tr1/unordered_map.h>

#include "boost/cstdint.hpp"
#include "boost/serialization/shared_ptr.hpp"
#include "boost/serialization/vector.hpp"

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

    typedef std::tr1::unordered_map<std::string, int> NamesIdsMap;
    typedef std::tr1::unordered_map<int, int> IdsIdsMap;
    typedef std::tr1::unordered_map<int, std::string> IdsNamesMap;

    NamesIdsMap _sdqaMetricIds;
    IdsIdsMap   _sdqaThresholdIds;
    IdsNamesMap _sdqaMetricNames;

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

    void querySdqaTables(
        lsst::daf::persistence::DbStorage &
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



