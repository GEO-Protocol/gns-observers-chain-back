/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#include "ResultsInterface.h"


ResultsInterface::ResultsInterface(
    Logger &logger) :

    mLog(logger)
{
    if (!isFIFOExists()) {
        createFIFO(kPermissionsMask);
    }
    mFIFODescriptor = 0;
    signal(SIGPIPE, SIG_IGN);
}

ResultsInterface::~ResultsInterface()
{
    if (mFIFODescriptor != 0) {
        close(mFIFODescriptor);
    }
}

void ResultsInterface::writeResult(
    const char *bytes,
    const size_t bytesCount)
{
    if (mFIFODescriptor == 0){

#ifdef MAC_OS
        mFIFODescriptor = open(
            FIFOFilePath().c_str(),
            O_WRONLY | O_DSYNC);
#endif

#ifdef LINUX
        mFIFODescriptor = open(
            FIFOFilePath().c_str(),
            O_WRONLY | O_RSYNC | O_DSYNC);
#endif

        if (mFIFODescriptor == -1) {
            throw IOError(
                "ResultsInterface::ResultsInterface: "
                    "Can't open FIFO file.");
        }
    }
    if (write(mFIFODescriptor, bytes, bytesCount) != bytesCount) {
        close(mFIFODescriptor);

#ifdef MAC_OS
        mFIFODescriptor = open(
            FIFOFilePath().c_str(),
            O_WRONLY | O_DSYNC);
#endif

#ifdef LINUX
        mFIFODescriptor = open(
            FIFOFilePath().c_str(),
            O_WRONLY | O_RSYNC | O_DSYNC);
#endif

        if (mFIFODescriptor == -1) {
            throw IOError(
                "ResultsInterface::ResultsInterface: "
                    "Can't open FIFO file.");
        }

        if (write(mFIFODescriptor, bytes, bytesCount) != bytesCount) {
            throw IOError(
                "ResultsInterface::writeResult: "
                    "Can't write results to the disk.");
        }
    }
    #ifdef TESTS__TRUSTLINES
    Timestamp start_time_s = posix::microsec_clock::universal_time();
    MicrosecondsTimestamp starttime_m = microsecondsTimestamp(start_time_s);

    auto debug = mLog.debug("ResultsInterface");
    debug << starttime_m;
    #endif

}

const char *ResultsInterface::FIFOName() const
{
    return kFIFOName;
}

