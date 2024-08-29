#!/bin/bash
mkdir -p com/enjoyars/rf21x
swig -c++ -java -package com.enjoyars.rf21x -outdir com/enjoyars/rf21x -o jni/rf21x_wrap_wrap.cpp ../src/rf21x-wrap/rf21x.i
/opt/android-ndk-r8-crystax-1/ndk-build
