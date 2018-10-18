cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
        Types.h
        time/utils.hpp
        memory/utils.hpp
        )

add_library(common ${SOURCE_FILES})
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)