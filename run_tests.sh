#!/bin/bash
g++ -std=c++17 ascii85.cpp test.cpp \
    -I${GTEST_INCLUDE_DIR} \
    -L${GTEST_LIBRARY_DIR} \
    -lgtest -lgtest_main -pthread \
    -o tests