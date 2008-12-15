// -*- lsst-c++ -*-
%define sdqa_DOCSTRING
"
Access to the classes from the sdqa library
"
%enddef

%feature("autodoc", "1");
%module(package="lsst.sdqa", docstring=sdqa_DOCSTRING) sdqaLib

%{
#include "lsst/sdqa/SdqaRating.h"
%}

%include "lsst/p_lsstSwig.i"

%lsst_exceptions()

%import "lsst/pex/exceptions/exceptionsLib.i"

%include "lsst/sdqa/SdqaRating.h"



