/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#ifndef GNS_OBSERVERS_CHAIN_BACK_CORE_H
#define GNS_OBSERVERS_CHAIN_BACK_CORE_H


#include "interface/commands/interface/CommandsInterface.h"
#include "interface/results/interface/ResultsInterface.h"

#include "logger/Logger.h"
#include "logger/LoggerMixin.hpp"
#include "common/memory/utils.hpp"


class Core{
public:
    enum ErrorCodes {
        LoggerInitialisationFailed = 1,
        CommandsInterfaceInitialisationFailed,
        ResultsInterfaceInitialisationFailed,

        UnhandledException = 254,
        Default = 255
    };

public:
    Core()
        noexcept
    {};

    int run();

protected:
    int initSubsystems();

    int initLogger();

    int initCommandsInterface();

protected:
    static string logHeader()
        noexcept;

    /*
     * Core implements its own log methods, and does not uses LoggerMixin,
     * because core itself is created before the logger instance,
     * and LoggerMixin instance can't be initialised properly.
     */
    LoggerStream warning()
        const
        noexcept;

    LoggerStream error()
        const
        noexcept;

    LoggerStream info()
        const
        noexcept;

    LoggerStream debug()
        const
        noexcept;

    /**
     * warning(), error(), info(), and debug()
     * are using the same logic for checking if logger was initialised.
     * This method unifies this logic.
     *
     * @param group - WARNING / ERROR / etc
     * @returns logger stream with "group" set.
     */
    LoggerStream constructLoggerStream(
        const string &group)
        const
        noexcept;

protected:
    as::io_service mIOService;

    unique_ptr<Logger> mLog;
    unique_ptr<CommandsInterface> mCommandsInterface;
    unique_ptr<ResultsInterface> mResultsInterface;
};


#endif //GNS_OBSERVERS_CHAIN_BACK_CORE_H
