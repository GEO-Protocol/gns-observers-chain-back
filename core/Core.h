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

#include "logger/Logger.h"
#include "logger/LoggerMixin.hpp"

#include <memory>


class Core{
public:
    Core()
        noexcept
        = default;

    int run();

protected:
    int initSubsystems();

    int initLogger();

protected:
    static string logHeader()
        noexcept;

    LoggerStream warning() const
        noexcept;

    LoggerStream error() const
        noexcept;

    LoggerStream info() const
        noexcept;

    LoggerStream debug() const
        noexcept;

protected:
    unique_ptr<Logger> mLog;
};


#endif //GNS_OBSERVERS_CHAIN_BACK_CORE_H
