/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#ifndef GNS_OBSERVERS_CHAIN_BACK_LOGGER_H
#define GNS_OBSERVERS_CHAIN_BACK_LOGGER_H

#include "../exceptions/common/RuntimeError.h"

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

    friend class Logger;

public:
    enum StreamType {
        Standard = 0,
        ErrorsStream,

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
        StreamType type = Standard)
        noexcept;

    LoggerStream(
        const LoggerStream &other)
        noexcept;

    /**
     * Outputs collected log information.
     */
    ~LoggerStream() override;

    /**
     * @returns logger stream, that would collect information, but would never outputs it.
     */
    static LoggerStream dummy();

    /**
     * @returns logger stream, that would print errors to the default errors strem (cerr),
     * and would not use internal logger structure.
     *
     * This stream type is useful for the cases, when logger is not initialised yet,
     * but it is necessary to output some log information.
     */
    static LoggerStream defaultErrorsStream(
        const string &group,
        const string &subsystem);

protected:
    /**
     * @returns current time point in UTC.
     *
     * This method is used as a shortcut for the cases, when logger instance is null.
     * For example, it is used to print errors to the cerr via streams of type ErrorsStream.
     */
    static const boost::posix_time::ptime currentUTC()
        noexcept;

private:
    Logger *mLogger;
    const string &mGroup;
    const string &mSubsystem;
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
    static constexpr const char* kFileName = "operations.log";
    static constexpr const char* kArchivedNamePrefix = "archived_operations_";
    static const uint32_t kMaxLinesPerFile = 500000;

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

    void calculateCurrentLogFileLinesCount();

private:
    std::ofstream mOperationsLogFile;
    uint32_t mOperationsLogFileLinesNumber;
};
#endif //GNS_OBSERVERS_CHAIN_BACK_LOGGER_H