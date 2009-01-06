// -*- lsst-c++ -*-

/** 
 * \file SdqaImageStatus.cc
 *
 * \ingroup sdqa
 *
 * \brief Source file for class SdqaImageStatus.  
 *
 *        SdqaImageStatus provides a container for a record from the 
 *        SDQA_ImageStatus database table.
 *
 * \author Russ Laher, IPAC
 */

#include "lsst/sdqa/SdqaImageStatus.h"

using namespace lsst::sdqa;


/**
 * Basic constructor.
 *
 * Initializes class attributes to zero or null, as appropriate. 
 * The set function must be subsequently called to set the class 
 * attributes.
 */

SdqaImageStatus::SdqaImageStatus() : _sdqaImageStatusId(0), 
                                     _name(""),  
                                     _definition("") {}  


/**
 *  Constructor to explicitly initialize class attributes.
 *
 * \param sdqaImageStatusId is the sdqa_ImageStatus database table ID.
 * \param name is the moniker of a particular SDQA image status.
 * \param definition gives a description of the SDQA image status.
 */

SdqaImageStatus::SdqaImageStatus(int         sdqaImageStatusId, 
                                 std::string name,
                                 std::string definition) : 
    _sdqaImageStatusId(sdqaImageStatusId), 
    _name(name),  
    _definition(definition) {}  


SdqaImageStatus::~SdqaImageStatus() {
}


/**
 * Set class attributes.
 *
 * \param sdqaImageStatusId is the sdqa_ImageStatus database table ID.
 * \param name is the moniker of a particular SDQA image status.
 * \param definition gives a description of the SDQA image status.
 */

void SdqaImageStatus::set(int         sdqaImageStatusId, 
                          std::string name,
                          std::string definition) {
        _sdqaImageStatusId  = sdqaImageStatusId;
        _name     = name;
        _definition   = definition;
}


int SdqaImageStatus::getImageStatusId() const {
    return _sdqaImageStatusId;
}

std::string SdqaImageStatus::getName() const {
    return _name;
}

std::string SdqaImageStatus::getDefinition() const {
    return _definition;
}



