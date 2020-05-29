#!/bin/bash

ILCSOFT=/cvmfs/clicdp.cern.ch/iLCSoft/builds/current/CI_${COMPILER}
source $ILCSOFT/init_ilcsoft.sh

cd /Package
mkdir build
cd build
cmake -GNinja -DSIO_BUILTIN_ZLIB=${BUILTIN_ZLIB-OFF} -DSIO_EXAMPLES=ON -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" .. && \
ninja  -k 0 && \
ninja install && \
ctest --output-on-failure
