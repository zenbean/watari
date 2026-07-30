#!/bin/bash
# configurations
# cmake -B build

cmake --build build
#cd build && ctest --output-on-failure

# verbose test output
cd build && ctest --output-on-failure -V