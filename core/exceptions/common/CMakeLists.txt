cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
        Exception.cpp
        Exception.h

        CommandParsingError.h
        IndexError.h
        MemoryError.h
        NotFoundError.h
        OverflowError.h
        PreconditionFailedError.h
        RuntimeError.h
        IOError.h
        ConflictError.h
        ValueError.h)

add_library(exceptions ${SOURCE_FILES})