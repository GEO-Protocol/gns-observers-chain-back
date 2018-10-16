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
    auto initCode = initSubsystems();
    if (initCode != 0) {
        error() << "Can't initialize internal subsystems. "
                   "Process will now be stopped.";

        return initCode;
    }

    return 0;
    // todo: commands interface reading should go here
}

int Core::initSubsystems()
{
    int initCode = initLogger();
    if (initCode != 0) {
        return initCode;
    }

    return 0;
}

int Core::initLogger()
{
    try {
        mLog = make_unique<Logger>();
        return 0;

    } catch (exception &e) {
        cerr << boost::posix_time::microsec_clock::universal_time() << ": FATAL\tCORE\tLogger cannot be initialized." << endl;
        return -1;
    }
}

string Core::logHeader()
noexcept
{
    return "[CORE]";
}

LoggerStream Core::error() const
noexcept
{
    return mLog->error(logHeader());
}

LoggerStream Core::warning() const
noexcept
{
    return mLog->warning(logHeader());
}

LoggerStream Core::info() const
noexcept
{
    return mLog->info(logHeader());
}

LoggerStream Core::debug() const
noexcept
{
    return mLog->debug(logHeader());
}
