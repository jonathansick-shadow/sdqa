// -*- lsst-c++ -*-

/** 
 * \file SdqaRatingFormatter.h
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
 *
 *        for PersistableSdqaRatingVector instances.
 *
 * \author Russ Laher, IPAC
 */

#include <memory>

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


/*
 *Lookup a metricName in the sdqa_Metric database table to find a sdqa_metricId.
 */

static int lookupSdqaMetricId(per::DbStorage* db, std::string metricName) {
    db->setTableForQuery("sdqa_Metric");
    db->outColumn("sdqa_metricId");
    db->condParam<std::string>("name", metricName);
    db->setQueryWhere("metricName = :name");
    db->query();
    if (!db->next() || db->columnIsNull(0)) {
        throw LSST_EXCEPT(ex::RuntimeErrorException,
                          "Unable to get sdqa_metricId for metricName: " + metricName);
    }
    int sdqa_metricId = db->getColumnByPos<int>(0);
    db->finishQuery();
    return sdqa_metricId;
}


/*
 *Lookup an sdqa_metricId in the database to find an sdqa_metricId.
 */

static int lookupSdqaThresholdId(per::DbStorage* db, int sdqa_metricId) {
    db->setTableForQuery("sdqa_Threshold");
    db->outColumn("sdqa_thresholdId");
    db->condParam<int>("id", sdqa_metricId);
    db->setQueryWhere("sdqa_metricId = :id");
    db->query();
    if (!db->next() || db->columnIsNull(0)) {
        throw LSST_EXCEPT(ex::RuntimeErrorException,
            "Unable to get sdqa_metricId for sdqa_metricId: " + sdqa_metricId);
    }
    int sdqa_thresholdId = db->getColumnByPos<int>(0);
    db->finishQuery();
    return sdqa_thresholdId;
}


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
    
    qa::SdqaRatingSet::size_type sz;

    if (Archive::is_loading::value) {        
        qa::SdqaRating data;        
        archive & sz;
        p->_sdqaRatings.clear();
        p->_sdqaRatings.reserve(sz);
        for (; sz > 0; --sz) {
            archive & data;
            qa::SdqaRating::Ptr sdqaRatingPtr(new qa::SdqaRating(data));
            p->_sdqaRatings.push_back(sdqaRatingPtr);
        }
    } else {
        sz = p->_sdqaRatings.size();
        archive & sz;
        qa::SdqaRatingSet::iterator i = p->_sdqaRatings.begin();
        qa::SdqaRatingSet::iterator const end(p->_sdqaRatings.end());
        for ( ; i != end; ++i) {
            archive &  **i;
        }
    }
}


template 
void qa::SdqaRatingVectorFormatter::delegateSerialize<boost::archive::text_oarchive>(
    boost::archive::text_oarchive &, unsigned int const, bas::Persistable *
);

template 
void qa::SdqaRatingVectorFormatter::delegateSerialize<boost::archive::text_iarchive>(
    boost::archive::text_iarchive &, unsigned int const, bas::Persistable *
);


/** 
 * Persist a set of SdqaRating objects to BoostStorage, DbStorage or DbTsvStorage.
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


    qa::SdqaRatingSet::iterator i = sdqaRatingVector.begin();


    /*
       Assume all SDQA Ratings in the set have the same scope and 
       parent DB ID.
    */

    int64_t parentDbId;
    std::string tableName;
    std::string columnNameOfImageId;

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

    unsigned short seq    = 1;
    per::DbStorage* db_sdqa = dynamic_cast<per::DbStorage*>(storage.get());

    for ( ; i != sdqaRatingVector.end(); ++i) {

        // Look up the sdqa_metricId given the metricName.
	std::string metricName = (*i)->qa::SdqaRating::getName();
        int sdqa_metricId = lookupSdqaMetricId( db_sdqa, metricName);
        (*i)->qa::SdqaRating::setSdqaMetricId(sdqa_metricId);

        // Look up the sdqa_thresholdId given the metricName.
        int sdqa_thresholdId = lookupSdqaThresholdId(db_sdqa, sdqa_metricId);
        (*i)->qa::SdqaRating::setSdqaThresholdId(sdqa_thresholdId);

        (*i)->qa::SdqaRating::setParentDbId(parentDbId);
        ++seq;
        if (seq == 0) { // Overflowed
            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Too many SdqaRatings");
        }
    }

    if (typeid(*storage) == typeid(per::BoostStorage)) {
        per::BoostStorage * bs = 
            dynamic_cast<per::BoostStorage *>(storage.get());
        if (bs == 0) {
            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Didn't get BoostStorage");
        }
        bs->getOArchive() & *p;

    } else if (typeid(*storage) == typeid(per::DbStorage) || 
               typeid(*storage) == typeid(per::DbTsvStorage)) {
        if (typeid(*storage) == typeid(per::DbStorage)) {

            per::DbStorage * db = 
                dynamic_cast<per::DbStorage *>(storage.get());
            if (db == 0) {
                throw LSST_EXCEPT(ex::RuntimeErrorException, 
                    "Didn't get DbStorage");
            }

            db->setTableForInsert(tableName);
            
            qa::SdqaRatingSet::const_iterator i(sdqaRatingVector.begin());
            qa::SdqaRatingSet::const_iterator const end(sdqaRatingVector.end());
            for ( ; i != end; ++i) {
	      insertRow<per::DbStorage>(*db, **i, columnNameOfImageId);
            }

        } else {

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
 * Retrieve a set of SdqaRating objects from BoostStorage, DbStorage or DbTsvStorage.
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

    if (sdqaRatingScope == "AMP") {
        parentDbId = extractAmpExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceAmpExposure";
        columnNameOfImageId = "ampExposureId";
    } else if  (sdqaRatingScope == "CCD") {
        parentDbId = extractCcdExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceCCDExposure";
        columnNameOfImageId = "ccdExposureId";
    } else if  (sdqaRatingScope == "FPA") {
        parentDbId = extractExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceFPAExposure";
        columnNameOfImageId = "exposureId";
    } else if  (sdqaRatingScope == "FOOTPRINT") {
        parentDbId = extractAmpExposureId(additionalData);
        tableName = "sdqa_Rating_ForScienceAmpExposure";
        columnNameOfImageId = "ampExposureId";
    }


    if (typeid(*storage) == typeid(per::BoostStorage)) {
    	//handle retrieval from BoostStorage
        per::BoostStorage* bs = 
            dynamic_cast<per::BoostStorage *>(storage.get());
        if (bs == 0) { 
            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Didn't get BoostStorage");
        }
        bs->getIArchive() & *p;

    } else if (typeid(*storage) == typeid(per::DbStorage) || 
               typeid(*storage) == typeid(per::DbTsvStorage)) {
        per::DbStorage * db = 
            dynamic_cast<per::DbStorage *>(storage.get());
        if (db == 0) {  
            throw LSST_EXCEPT(ex::RuntimeErrorException, 
                "Didn't get DbStorage");
        }

        db->setTableForQuery(tableName);

        qa::SdqaRatingSet sdqaRatingVector;
        qa::SdqaRating data;
            
        setupFetch(*db, data, columnNameOfImageId);
        db->query();
            
        while (db->next()) {
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


