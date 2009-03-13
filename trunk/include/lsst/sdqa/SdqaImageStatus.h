// -*- lsst-c++ -*-

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

