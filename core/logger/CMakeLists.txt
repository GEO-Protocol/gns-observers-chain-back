cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
        Logger.h
        Logger.cpp
        LoggerMixin.hpp)

add_library(logger ${SOURCE_FILES})
target_link_libraries(logger
        -lboost_date_time
        )