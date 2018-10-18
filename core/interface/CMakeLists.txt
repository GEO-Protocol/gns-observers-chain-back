cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
        BaseFIFOInterface.hpp

        commands/interface/CommandUUID.h
        commands/interface/CommandUUID.cpp

        commands/interface/CommandsInterface.h
        commands/interface/CommandsInterface.cpp
        commands/interface/CommandsParser.h
        commands/interface/CommandsParser.cpp
        commands/commands/BaseUserCommand.h
        commands/commands/BaseUserCommand.cpp

        results/interface/ResultsInterface.h
        results/interface/ResultsInterface.cpp
        results/results/CommandResult.h
        results/results/CommandResult.cpp
        )

add_library(interface ${SOURCE_FILES})
target_link_libraries(interface
        common
        exceptions)
