/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#ifndef GEO_NETWORK_CLIENT_LOGGER_H
#define GEO_NETWORK_CLIENT_LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/date_time.hpp>


using namespace std;


class Logger;

/**
 * Logger stream is used as interface for the logger.
 */
class LoggerStream:
    public stringstream {

public:
    enum StreamType {
        Standard = 0,

        //...
        // Other logs types must be located here
        //  ...

        // Dummy logger is used in configurations,
        // where no log records should be issued,
        // but the logger structure is required by some type of signature(s).
        Dummy,
    };

public:
    explicit LoggerStream(
        Logger *logger,
        const string &group,
        const string &subsystem,
        StreamType type = Standard);

    LoggerStream(
        const LoggerStream &other);

    ~LoggerStream() override;

    static LoggerStream dummy();

private:
    Logger *mLogger;
    const string mGroup;
    const string mSubsystem;
    const StreamType mType;
};


class Logger {
    friend class LoggerStream;

public:
    Logger();

    void logException(
        const string &subsystem,
        const exception &e);

    LoggerStream info(
        const string &subsystem);

    LoggerStream error(
        const string &subsystem);

    LoggerStream debug(
        const string &subsystem);

    LoggerStream warning(
        const string &subsystem);

protected:
    const uint32_t maxRotateLimit = 500000;

protected:
    const string formatMessage(
        const string &message) const;

    const string recordPrefix(
        const string &group);

    void logRecord(
        const string &group,
        const string &subsystem,
        const string &message);

    void rotate();

    bool formatLogFileName(
        string& str,
        const string& from,
        const string& to);

    void calculateOperationsLogFileLinesNumber();

private:
    std::ofstream mOperationsLogFile;
    uint32_t mOperationsLogFileLinesNumber;
    string mOperationLogFileName;
};
#endif //GEO_NETWORK_CLIENT_LOGGER_H