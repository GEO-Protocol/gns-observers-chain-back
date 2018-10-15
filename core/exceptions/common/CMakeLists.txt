cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
        Exception.cpp
        Exception.h
        IOError.h)

add_library(exceptions ${SOURCE_FILES})