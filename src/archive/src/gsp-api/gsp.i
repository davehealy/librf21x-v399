%module gsp

%{
#include "gsp-wrap.h"
%}

%include "std_string.i"
%include "std_vector.i"
namespace std {
    %template(GspData) vector<unsigned char>;
    %template(GspHidSerialNumbers) vector<string>;
}

%include "gsp-wrap.h"
