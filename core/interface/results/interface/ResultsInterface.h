/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#ifndef GNS_OBSERVERS_CHAIN_BACK_RESULTSINTERFACE_H
#define GNS_OBSERVERS_CHAIN_BACK_RESULTSINTERFACE_H

#include "../../BaseFIFOInterface.hpp"
#include "../../../logger/Logger.h"
#include "../../../exceptions/common/IOError.h"
#include "../../../exceptions/common/MemoryError.h"

#include <boost/bind.hpp>

#include <string>


using namespace std;


class ResultsInterface:
    public BaseFIFOInterface {

public:
    static const constexpr char *kFIFOName = "results.fifo";
    static const constexpr unsigned int kPermissionsMask = 0755;

public:
    explicit ResultsInterface(
        Logger &logger);

    ~ResultsInterface();

    void writeResult(
        const char *bytes,
        const size_t bytesCount);

private:
    virtual const char* FIFOName()
        const;

private:
    Logger &mLog;
};

#endif //GNS_OBSERVERS_CHAIN_BACK_RESULTSINTERFACE_H
