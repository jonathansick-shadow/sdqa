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

namespace sdqa = lsst::sdqa;


/**
 * Basic constructor.
 *
 * Initializes class attributes to zero or null, as appropriate. 
 * The set function must be subsequently called to set the class 
 * attributes.
 */

sdqa::SdqaImageStatus::SdqaImageStatus() : _sdqaImageStatusId(0), 
                                     _name(""),  
                                     _definition("") {}  


/**
 *  Constructor to explicitly initialize class attributes.
 *
 * \param sdqaImageStatusId is the sdqa_ImageStatus database table ID.
 * \param name is the moniker of a particular SDQA image status.
 * \param definition gives a description of the SDQA image status.
 */

sdqa::SdqaImageStatus::SdqaImageStatus(int         sdqaImageStatusId, 
                                 std::string name,
                                 std::string definition) : 
    _sdqaImageStatusId(sdqaImageStatusId), 
    _name(name),  
    _definition(definition) {}  


sdqa::SdqaImageStatus::~SdqaImageStatus() {
}


/**
 * Set class attributes.
 *
 * \param sdqaImageStatusId is the sdqa_ImageStatus database table ID.
 * \param name is the moniker of a particular SDQA image status.
 * \param definition gives a description of the SDQA image status.
 */

void sdqa::SdqaImageStatus::set(int         sdqaImageStatusId, 
                          std::string name,
                          std::string definition) {
        _sdqaImageStatusId  = sdqaImageStatusId;
        _name     = name;
        _definition   = definition;
}


int sdqa::SdqaImageStatus::getImageStatusId() const {
    return _sdqaImageStatusId;
}

std::string sdqa::SdqaImageStatus::getName() const {
    return _name;
}

std::string sdqa::SdqaImageStatus::getDefinition() const {
    return _definition;
}
