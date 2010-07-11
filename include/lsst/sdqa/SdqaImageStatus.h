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
 * \file SdqaImageStatus.h
 *
 * \ingroup sdqa
 *
 * \brief Header file for class SdqaImageStatus.  
 *
 *        SdqaImageStatus provides a container for a record from the 
 *        SDQA_ImageStatus database table.
 *
 * \author Russ Laher, IPAC
 */

#ifndef LSST_SDQAIMAGESTATUS_H
#define LSST_SDQAIMAGESTATUS_H

#include <string>

namespace lsst {

    namespace sdqa {


/**
* Class for containing an SDQA image status. 
*/

class SdqaImageStatus { 

public:
    SdqaImageStatus();

    SdqaImageStatus(int         sdqaImageStatusId, 
                    std::string name,
                    std::string definition);

    ~SdqaImageStatus(); 

    void set(int         sdqaImageStatusId, 
             std::string name,
             std::string definition);
 
    int         getImageStatusId() const;
    std::string getName() const;
    std::string getDefinition() const;

private:
    int         _sdqaImageStatusId;
    std::string _name;
    std::string _definition;
};

    } // namespace sdqa

} // namespace lsst

#endif // LSST_SDQAIMAGESTATUS_H

