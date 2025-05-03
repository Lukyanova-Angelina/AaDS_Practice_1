#!/bin/bash
g++ -std=c++17 ascii85.cpp test.cpp -lgtest -lgtest_main -pthread -o tests
