// -*- lsst-c++ -*-

/** 
 * \file SdqaRatingFormatter.cc
 *
 * \ingroup sdqa
 *
 * \brief Implementation file for SdqaRatingFormatter class.  
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

#include <memory>
#include <map>

#include "boost/format.hpp"
#include "lsst/daf/base.h"
#include "lsst/daf/persistence.h"
#include "lsst/pex/exceptions.h"
#include "lsst/pex/policy/Policy.h"
#include "lsst/sdqa/SdqaRating.h"
#include "lsst/sdqa/SdqaRatingFormatter.h"

namespace ex = lsst::pex::exceptions;
namespace pol = lsst::pex::policy;
namespace bas = lsst::daf::base;
namespace per = lsst::daf::persistence;
namespace qa = lsst::sdqa;


/*******************
 * SdqaRatingFormatter class.
 */

/**
 * Basic constructor for SdqaRatingFormatter class.
 */

qa::SdqaRatingVectorFormatter::SdqaRatingVectorFormatter(
    pol::Policy::Ptr const & policy) : per::Formatter(typeid(*this)),
    _policy(policy) {}


/**
 * Destructor for SdqaRatingFormatter class.
 */

qa::SdqaRatingVectorFormatter::~SdqaRatingVectorFormatter() {}


/**
 *  Member functions for SdqaRatingFormatter class.
 */

per::Formatter::Ptr qa::SdqaRatingVectorFormatter::createInstance(
    pol::Policy::Ptr policy) {
    return per::Formatter::Ptr(new qa::SdqaRatingVectorFormatter(policy));
}

per::FormatterRegistration qa::SdqaRatingVectorFormatter::registration(
    "PersistableSdqaRatingVector",
    typeid(qa::PersistableSdqaRatingVector),
    createInstance
);


/*!
    Puts a single SdqaRating into an instance of lsst::daf::persistence::DbStorage or 
    subclass thereof.
 */
template <typename T>
void qa::SdqaRatingVectorFormatter::insertRow(T & db, 
                                                qa::SdqaRating const & d,
                                                std::string columnNameOfImageId) {
    db.template setColumn<boost::int32_t>("sdqa_metricId", d._sdqa_metricId);
    db.template setColumn<boost::int32_t>("sdqa_thresholdId", 
        d._sdqa_thresholdId);
    db.template setColumn<boost::int64_t>(columnNameOfImageId, d._parentDbId);
    db.template setColumn<double>("metricValue", d._metricValue);
    db.template setColumn<double>("metricErr", d._metricErr);
    db.insertRow();
}


//! \cond
template void qa::SdqaRatingVectorFormatter::insertRow<per::DbStorage>(
    per::DbStorage & db,    
    qa::SdqaRating const &d,
    std::string columnNameOfImageId);
template void qa::SdqaRatingVectorFormatter::insertRow<per::DbTsvStorage>(
    per::DbTsvStorage & db, 
    qa::SdqaRating const &d,
    std::string columnNameOfImageId);
//! \endcond


/*!     
    Gets a single SdqaRating from an instance of lsst::daf::persistence::DbStorage or 
    subclass thereof.
 */
void qa::SdqaRatingVectorFormatter::setupFetch(per::DbStorage & db, 
                                                 qa::SdqaRating & d,
                                                 std::string columnNameOfImageId) {
    db.outParam("sdqa_metricId",     &(d._sdqa_metricId));
    db.outParam("sdqa_thresholdId",  &(d._sdqa_thresholdId));
    db.outParam(columnNameOfImageId, &(d._parentDbId));
    db.outParam("metricValue",       &(d._metricValue));
    db.outParam("metricErr",         &(d._metricErr));
}


/**
 * Private delegate-serialize function for SdqaRatingVectorFormatter class.
 */

template <class Archive>
void qa::SdqaRatingVectorFormatter::delegateSerialize(
    Archive & archive,
    unsigned int const version,
    bas::Persistable * persistable) {  

    qa::PersistableSdqaRatingVector * p = 
        dynamic_cast<qa::PersistableSdqaRatingVector*>(persistable);

    archive & boost::serialization::base_object<bas::Persistable>(*p);
    archive & p->_sdqaRatings;

}


/** 
 * Persist set of SdqaRating objects to BoostStorage, XmlStorage, DbStorage or DbTsvStorage.
 */

void qa::SdqaRatingVectorFormatter::write(
    bas::Persistable const * persistable,
    per::Storage::Ptr storage,
    bas::PropertySet::Ptr additionalData
) {
    if (persistable == 0) {
        throw LSST_EXCEPT(ex::InvalidParameterException, 
            "No lsst::base::Persistable provided");
    }
    if (!storage) {
        throw LSST_EXCEPT(ex::InvalidParameterException, "No Storage provided");
    }

    qa::PersistableSdqaRatingVector const * p = 
        dynamic_cast<qa::PersistableSdqaRatingVector const *>(persistable);
    if (p == 0) {
        throw LSST_EXCEPT(ex::RuntimeErrorException, 
            "Persistable was not of concrete type SdqaRatingVector");
    }
    qa::SdqaRatingSet sdqaRatingVector = p->getSdqaRatings();   


    /*
       Assume all SDQA Ratings in the set have the same scope and 
       parent DB ID.
    */

    int64_t parentDbId;
    std::string tableName;
    std::string columnNameOfImageId;
    qa::SdqaRatingSet::iterator i = sdqaRatingVector.begin();

    if ((*i)->qa::SdqaRating::getRatingScope() == 
        qa::SdqaRating::AMP) {
        parentDbId = extractAmpExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceAmpExposure";
        columnNameOfImageId = "ampExposureId";
    } else if  ((*i)->qa::SdqaRating::getRatingScope() == 
        qa::SdqaRating::CCD) {
        parentDbId = extractCcdExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceCCDExposure";
        columnNameOfImageId = "ccdExposureId";
    } else if  ((*i)->qa::SdqaRating::getRatingScope() == 
        qa::SdqaRating::FPA) {
        parentDbId = extractExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceFPAExposure";
        columnNameOfImageId = "exposureId";
    } else if  ((*i)->qa::SdqaRating::getRatingScope() == 
        qa::SdqaRating::FOOTPRINT) {
        parentDbId = extractAmpExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceAmpExposure";
        columnNameOfImageId = "ampExposureId";
    }


    if (typeid(*storage) == typeid(per::BoostStorage)) {


        throw LSST_EXCEPT(ex::RuntimeErrorException, 
            "Need to add code here to assign parentDbId, etc.");


        per::BoostStorage * bs = 
            dynamic_cast<per::BoostStorage *>(storage.get());
        if (bs == 0) {
            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Didn't get BoostStorage");
        }
        bs->getOArchive() & *p;

    /* This part needs more work because of differences between XmlStorage and BoostStorage,
       to be fixed for DC3b.
    } else if (typeid(*storage) == typeid(per::XmlStorage)) {


        throw LSST_EXCEPT(ex::RuntimeErrorException,
            "Need to add code here to assign parentDbId, etc.");


        per::XmlStorage * xs = 
            dynamic_cast<per::XmlStorage *>(storage.get());
        if (xs == 0) {
            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Didn't get XmlStorage");
        }
        xs->getOArchive() & *p;
   */

    } else if (typeid(*storage) == typeid(per::DbStorage) || 
               typeid(*storage) == typeid(per::DbTsvStorage)) {
        if (typeid(*storage) == typeid(per::DbStorage)) {

            per::DbStorage * db = 
                dynamic_cast<per::DbStorage *>(storage.get());
            if (db == 0) {
                throw LSST_EXCEPT(ex::RuntimeErrorException, 
                    "Didn't get DbStorage");
            }


            /*
	     * Query the sdqa_Metric database table for all sdqa_metricIds vs. metricName
             * and store it in a map.
	     */

	    std::map<std::string, int, std::less< std::string > > sdqa_metricIds;

            db->setTableForQuery("sdqa_Metric");
            db->outColumn("sdqa_metricId");
            db->outColumn("metricName");
            db->query();
            while (db->next()) {
                int sdqa_metricId = db->getColumnByPos<int>(0);
                std::string metricName = db->getColumnByPos<std::string>(1);
                sdqa_metricIds.insert(std::map<std::string, int, 
                    std::less< std::string > >::value_type(metricName, sdqa_metricId));
	    }
            db->finishQuery();


            /*
	     * Query the sdqa_Threshold database table for all sdqa_thresholdIds vs. 
	     * sdqa_metricId and store it in a map.
	     */

	    std::map<int, int, std::less< int > > sdqa_thresholdIds;

            db->setTableForQuery("sdqa_Threshold");
            db->outColumn("sdqa_metricId");
            db->outColumn("sdqa_thresholdId");
            db->query();
            while (db->next()) {
                int sdqa_metricId = db->getColumnByPos<int>(0);
                int sdqa_thresholdId = db->getColumnByPos<int>(1);
                sdqa_thresholdIds.insert(std::map<int, int, 
                   std::less< int > >::value_type(sdqa_metricId, sdqa_thresholdId));
	    }
            db->finishQuery();


            unsigned short seq    = 1;
            qa::SdqaRatingSet::iterator i = sdqaRatingVector.begin();

            for ( ; i != sdqaRatingVector.end(); ++i) {

                // Look up the sdqa_metricId given the metricName.
	        std::string metricName = (*i)->qa::SdqaRating::getName();
                int sdqa_metricId = sdqa_metricIds[metricName];
                (*i)->qa::SdqaRating::setSdqaMetricId(sdqa_metricId);

                // Look up the sdqa_thresholdId given the sdqa_metricId.
                int sdqa_thresholdId = sdqa_thresholdIds[sdqa_metricId];
                (*i)->qa::SdqaRating::setSdqaThresholdId(sdqa_thresholdId);

                (*i)->qa::SdqaRating::setParentDbId(parentDbId);
                ++seq;
                if (seq == 0) { // Overflowed
                    throw LSST_EXCEPT(ex::RuntimeErrorException, 
                        "Too many SdqaRatings");
                }
            }


            /*
	     * Insert rows into the appropriate sdqa_Rating_xxx database table.
	     */

            db->setTableForInsert(tableName);

            qa::SdqaRatingSet::const_iterator j(sdqaRatingVector.begin());
            qa::SdqaRatingSet::const_iterator const end(sdqaRatingVector.end());
            for ( ; j != end; ++j) {
	      insertRow<per::DbStorage>(*db, **j, columnNameOfImageId);
            }

        } else {


            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Need to add code here to assign parentDbId, etc.");


            per::DbTsvStorage * db = 
                dynamic_cast<per::DbTsvStorage *>(storage.get());
            if (db == 0) {
                throw LSST_EXCEPT(ex::RuntimeErrorException, 
                    "Didn't get DbTsvStorage");
            }
            db->setTableForInsert(tableName);

            qa::SdqaRatingSet::const_iterator i(sdqaRatingVector.begin());
            qa::SdqaRatingSet::const_iterator const end(sdqaRatingVector.end());
            for (; i != end; ++i) {
                insertRow<per::DbTsvStorage>(*db, **i, columnNameOfImageId);
            }
        }

    } else {


        throw LSST_EXCEPT(ex::InvalidParameterException, 
            "Storage type is not supported"); 

    }
}


/** 
 * Retrieve SdqaRating objects from BoostStorage, XmlStorage, DbStorage or DbTsvStorage.
 */

bas::Persistable* qa::SdqaRatingVectorFormatter::read(
    per::Storage::Ptr storage,
    bas::PropertySet::Ptr additionalData) {   

    std::auto_ptr<qa::PersistableSdqaRatingVector> p 
        (new qa::PersistableSdqaRatingVector);
    
    int64_t parentDbId;
    std::string tableName;
    std::string sdqaRatingScope = extractSdqaRatingScope(additionalData);
    std::string columnNameOfImageId;
    qa::SdqaRating::RatingScope ratingScope;

    if (sdqaRatingScope == "AMP") {
        parentDbId = extractAmpExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceAmpExposure";
        columnNameOfImageId = "ampExposureId";
        ratingScope = sdqa::SdqaRating::AMP;
    } else if  (sdqaRatingScope == "CCD") {
        parentDbId = extractCcdExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceCCDExposure";
        columnNameOfImageId = "ccdExposureId";
        ratingScope = sdqa::SdqaRating::CCD;
    } else if  (sdqaRatingScope == "FPA") {
        parentDbId = extractExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceFPAExposure";
        columnNameOfImageId = "exposureId";
        ratingScope = sdqa::SdqaRating::FPA;
    } else if  (sdqaRatingScope == "FOOTPRINT") {
        parentDbId = extractAmpExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceAmpExposure";
        columnNameOfImageId = "ampExposureId";
        ratingScope = sdqa::SdqaRating::FOOTPRINT;
    }


    if (typeid(*storage) == typeid(per::BoostStorage)) {
        per::BoostStorage* bs = 
            dynamic_cast<per::BoostStorage *>(storage.get());
        if (bs == 0) { 
            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Didn't get BoostStorage");
        }
        bs->getIArchive() & *p;

    /* This part needs more work because of differences between XmlStorage and BoostStorage,
       to be fixed for DC3b.
    } else if (typeid(*storage) == typeid(per::XmlStorage)) {
        per::XmlStorage* xs = 
            dynamic_cast<per::XmlStorage *>(storage.get());
        if (xs == 0) { 
            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Didn't get XmlStorage");
        }
        xs->getIArchive() & *p;
    */

    } else if (typeid(*storage) == typeid(per::DbStorage) || 
               typeid(*storage) == typeid(per::DbTsvStorage)) {
        per::DbStorage * db = 
            dynamic_cast<per::DbStorage *>(storage.get());
        if (db == 0) {  
            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Didn't get DbStorage");
        }


        /*
	 * Query the sdqa_Metric database table for all metricNames vs. sdqa_metricId
         * and store it in a map.
	 */

	std::map<int, std::string, std::less< int > > metricNames;

        db->setTableForQuery("sdqa_Metric");
        db->outColumn("sdqa_metricId");
        db->outColumn("metricName");
        db->query();
        while (db->next()) {
            int sdqa_metricId = db->getColumnByPos<int>(0);
            std::string metricName = db->getColumnByPos<std::string>(1);
            metricNames.insert(std::map<int, std::string, std::less< int > >::value_type(sdqa_metricId, metricName));
	}
        db->finishQuery();


        /*
	 * Query the appropriate sdqa_Rating_xxx database table.
	 */

        db->setTableForQuery(tableName);

        qa::SdqaRatingSet sdqaRatingVector;
        qa::SdqaRating data;

        data._ratingScope = ratingScope;

        setupFetch(*db, data, columnNameOfImageId);
        db->query();

        while (db->next()) {
	    data._metricName = metricNames[data._sdqa_metricId];
            qa::SdqaRating::Ptr sdqaRatingPtr(new qa::SdqaRating(data));
            sdqaRatingVector.push_back(sdqaRatingPtr);
        }
        db->finishQuery();

        p->setSdqaRatings(sdqaRatingVector);

    } else {
        throw LSST_EXCEPT(ex::InvalidParameterException, 
            "Storage type is not supported");
    }
    
    return p.release();
}


void qa::SdqaRatingVectorFormatter::update(bas::Persistable*, 
    per::Storage::Ptr, bas::PropertySet::Ptr
) {
    throw LSST_EXCEPT(ex::RuntimeErrorException, 
        "SdqaRatingVectorFormatter: updates not supported");
}


int64_t qa::SdqaRatingVectorFormatter::extractExposureId(
    bas::PropertySet::Ptr const & properties) {
    int64_t exposureId = properties->getAsInt64("exposureId");
    return exposureId;
}


int64_t qa::SdqaRatingVectorFormatter::extractCcdExposureId(
    bas::PropertySet::Ptr const & properties) {
    int64_t ccdExposureId = properties->getAsInt64("ccdExposureId");
    return ccdExposureId;
}


int64_t qa::SdqaRatingVectorFormatter::extractAmpExposureId(
    bas::PropertySet::Ptr const & properties) {
    int64_t ampExposureId = properties->getAsInt64("ampExposureId");
    return ampExposureId;
}


/**
 * Extracts and returns the string-valued @c "sdqaRatingScope" property from the 
 * given data property object.
 */

std::string const qa::SdqaRatingVectorFormatter::extractSdqaRatingScope(
    bas::PropertySet::Ptr const & properties) {
    std::string sdqaRatingScope = properties->getAsString("sdqaRatingScope");
    return sdqaRatingScope;
}



