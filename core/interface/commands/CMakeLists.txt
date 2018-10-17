cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
        ../BaseFIFOInterface.hpp

        interface/CommandsInterface.h
        interface/CommandsInterface.cpp

        commands/BaseUserCommand.h
        commands/BaseUserCommand.cpp
        )

add_library(interface__commands ${SOURCE_FILES})
target_link_libraries(interface__commands
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        common)
