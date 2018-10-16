cmake_minimum_required(VERSION 3.12)
project(gns_observers_chain_back)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_STANDARD 14)



#
# Boost configuration
#
find_package(Boost COMPONENTS system filesystem date_time REQUIRED)

set(Boost_USE_SHARED_LIBS ON)
set(Boost_USE_MULTITHREADED ON)



#
# Platform build options
# (supported platforms are: LINUX | MAC_OS)
#
set(PLATFORM "LINUX")

IF (${PLATFORM} MATCHES "MAC_OS")
    add_definitions(-DMAC_OS)
ENDIF (${PLATFORM} MATCHES "MAC_OS")

IF (${PLATFORM} MATCHES "LINUX")
    add_definitions(-DLINUX)
ENDIF (${PLATFORM} MATCHES "LINUX")



#
# Debug options
# (available options are: DEBUG | RELEASE | TESTS)
#
set(BUILD_CONFIGURATION "DEBUG")

IF (${BUILD_CONFIGURATION} MATCHES "DEBUG")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")

    add_definitions(
            -DDEBUG
            -DINTERNAL_ARGUMENTS_VALIDATION
    )
ENDIF (${BUILD_CONFIGURATION} MATCHES "DEBUG")

IF (${BUILD_CONFIGURATION} MATCHES "RELEASE")
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Os -s -fdata-sections -fmerge-all-constants")
ENDIF (${BUILD_CONFIGURATION} MATCHES "RELEASE")

IF (${BUILD_CONFIGURATION} MATCHES "TESTS")
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

    add_definitions(
            -DTESTS
            -DINTERNAL_ARGUMENTS_VALIDATION
    )
ENDIF (${BUILD_CONFIGURATION} MATCHES "TESTS")



#
# Source files
#
add_subdirectory(core/exceptions/common)
add_subdirectory(core/logger)



#
# Libs
#
add_subdirectory(libs/lamport/)



set(SOURCE_FILES
        main.cpp

        core/Core.cpp
        core/Core.h
        )


add_executable(gns_observers_chain_back ${SOURCE_FILES})
target_link_libraries(gns_observers_chain_back
        ${Boost_SYSTEM_LIBRARY}

        -lsodium
        -lboost_date_time

        exceptions
        logger
        lamport)
