/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#include "Core.h"


int Core::run()
{
    try {
        auto initCode = initSubsystems();
        if (initCode != 0) {
            error() << "Can't initialize internal subsystems. "
                       "Process will now be stopped.";

            return initCode;
        }

        info() << "Processing started.";

        mCommandsInterface->beginCommandsAccepting();
        mIOService.run();
        return 0;

    } catch (Exception &e) {
        error() << e.what();
        return ErrorCodes::UnhandledException;
    }
}

int Core::initSubsystems()
{
    // note: there is no need to catch any exceptions here.
    // all exceptions would be catch on a run() level.

    int initCode = initLogger();
    if (initCode != 0) {
        return initCode;
    }

    initCode = initCommandsInterface();
    if (initCode != 0) {
        return initCode;
    }

    return initCode;
}

int Core::initLogger()
{
    mLog = make_unique<Logger>();
    return 0;
}

int Core::initCommandsInterface()
{
    mCommandsInterface = make_unique<CommandsInterface>(
        mIOService,
        *mLog);

    info() << "Commands interface is successfully initialised";
    return 0;
}

string Core::logHeader()
    noexcept
{
    return "[CORE]";
}

LoggerStream Core::error()
    const
    noexcept
{
    return constructLoggerStream("ERROR");
}

LoggerStream Core::warning()
    const
    noexcept
{
    return constructLoggerStream("WARNING");
}

LoggerStream Core::info()
    const
    noexcept
{
    return constructLoggerStream("INFO");
}

LoggerStream Core::debug()
    const
    noexcept
{
    return constructLoggerStream("DEBUG");
}

LoggerStream Core::constructLoggerStream(
    const string &group)
    const
    noexcept
{
    // In case if logger was not initialised - error must be reported to standard errors stream.
    if (mLog == nullptr)
        return LoggerStream::defaultErrorsStream(group, logHeader());

    if (group == "ERROR")
        return mLog->error(logHeader());

    if (group == "WARNING")
        return mLog->warning(logHeader());

    if (group == "INFO")
        return mLog->info(logHeader());

    if (group == "DEBUG")
        return mLog->debug(logHeader());

    return LoggerStream::dummy();
}