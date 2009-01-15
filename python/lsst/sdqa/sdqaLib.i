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
#include "lsst/sdqa/SdqaMetric.h"
#include "lsst/sdqa/SdqaThreshold.h"
#include "lsst/sdqa/SdqaImageStatus.h"
%}

%include "lsst/p_lsstSwig.i"

%lsst_exceptions()

%include "lsst/sdqa/SdqaRating.h"
%include "lsst/sdqa/SdqaMetric.h"
%include "lsst/sdqa/SdqaThreshold.h"
%include "lsst/sdqa/SdqaImageStatus.h"



