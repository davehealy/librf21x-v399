%module rf21x

%{
#include "rf21x-wrap.h"
%}

%include "std_string.i"
%include "std_vector.i"
namespace std {
    %template(RF21xMessageRawData) vector<unsigned char>;
    %template(RF21xHidSerialNumbers) vector<string>;
}

%include "rf21x-wrap.h"
