%module rf21x

%{
#include "rf21x.h"
%}

%include std_string.i
%include std_map.i
namespace std {
  %template(QuizParameters) map<std::string,std::string>;
}

%include "rf21x.h"
