#include "Logger.h"


LoggerStream::LoggerStream(
    Logger *logger,
    const string &group,
    const string &subsystem,
    const StreamType type)
    noexcept :

    mLogger(logger),
    mGroup(group),
    mSubsystem(subsystem),
    mType(type)
{}

LoggerStream::LoggerStream(
    const LoggerStream &other)
    noexcept :

    mLogger(other.mLogger),
    mGroup(other.mGroup),
    mSubsystem(other.mSubsystem),
    mType(other.mType)
{}

LoggerStream::~LoggerStream()
{
    // Standard logger type is expected to be most usable,
    // so it should be placed first to prevent redundant comparisons.
    if (mType == Standard) {
        auto message = this->str();
        mLogger->logRecord(
            mGroup,
            mSubsystem,
            message);
        return;

    } else if (mType == ErrorsStream) {
        cerr << currentUTC() << ":\t"
             << mGroup << "\t"
             << mSubsystem << "\t"
             << this->str()
             << endl;
        return;


    /*
     * Other log types hould go here.
     */


    } else if (mType == Dummy) {
        // No output must be generated.
        return;

    } else {
        // Unexpected logger type.
        // Report it along with the original error.
        // Default system errors strem is sed here because other loggers might be not initialised.
        LoggerStream::defaultErrorsStream(mGroup, mSubsystem)
            << "Unexpected log type occurred: "
            << mType;

        LoggerStream::defaultErrorsStream(mGroup, mSubsystem)
            << this->str();
    }
}

LoggerStream LoggerStream::dummy()
{
    return LoggerStream(nullptr, "", "", Dummy);
}

LoggerStream LoggerStream::defaultErrorsStream(
    const string &group,
    const string &subsystem)
{
    return LoggerStream(nullptr, group, subsystem, ErrorsStream);
}

const boost::posix_time::ptime LoggerStream::currentUTC() noexcept
{
    return boost::posix_time::microsec_clock::universal_time();
}


Logger::Logger():
    mOperationsLogFileLinesNumber(0)
{
    calculateCurrentLogFileLinesCount();

#ifdef DEBUG
    // It is mush more useful to truncate log file on each application start when in debug mode.
    mOperationsLogFile.open(kFileName, std::fstream::out | std::fstream::trunc);
#endif

#ifndef DEBUG
    // In production mode logs must be appended.
    mOperationsLogFile.open(kFileName, std::fstream::out | std::fstream::app);
#endif

}

void Logger::logException(
    const string &subsystem,
    const exception &e)
{
    auto m = string(e.what());
    logRecord("EXCEPT", subsystem, m);
}

LoggerStream Logger::info(
    const string &subsystem)
{
    return LoggerStream(this, "INFO", subsystem);
}

LoggerStream Logger::warning(
        const string &subsystem)
{
    return LoggerStream(this, "WARNING", subsystem);
}

LoggerStream Logger::debug(
    const string &subsystem)
{
    return LoggerStream(this, "DEBUG", subsystem);
}

LoggerStream Logger::error(
    const string &subsystem)
{
    return LoggerStream(this, "ERROR", subsystem);
}

const string Logger::formatMessage(
    const string &message) const
{
    if (message.empty()) {
        return message;
    }

    auto m = message;

    // Cut the '\n' symbol in the edn of line (if present).
    if (m.at(m.size()-1) == '\n') {
        m = m.substr(0, m.size()-1);
    }

    // Add "." to the end if last symbol is not a punctuation.
    if (m.at(m.size()-1) != '.' && m.at(m.size()-1) != '\n' && m.at(m.size()-1) != ':') {
        m += ".";
    }

    return m;
}

const string Logger::recordPrefix(
    const string &group)
{
    stringstream s;
    s << LoggerStream::currentUTC() << " : " << group << "\t";
    return s.str();
}

void Logger::logRecord(
    const string &group,
    const string &subsystem,
    const string &message)
{
    stringstream recordStream;
    recordStream << recordPrefix(group)
                 << subsystem << "\t"
                 << formatMessage(message) << endl;

    // Logging to the console
    cout << recordStream.str();

    // Logging to the file
    mOperationsLogFile << recordStream.str();
    mOperationsLogFile.flush();

    mOperationsLogFileLinesNumber++;
    if(mOperationsLogFileLinesNumber >= kMaxLinesPerFile){
        rotate();
        mOperationsLogFileLinesNumber = 0;
    }
}

void Logger::rotate()
{
    mOperationsLogFile.close();

    stringstream rotateFileName;
    rotateFileName << kArchivedNamePrefix
                   << LoggerStream::currentUTC()
                   << ".log";

    auto archivedLogFileName = rotateFileName.str();
    formatLogFileName(archivedLogFileName, " ", "_");

    // Move original log file to the new one.
    rename(kFileName, archivedLogFileName.c_str());

    // New operations log file must be open in trunc mode to clear all previosly written records.
    mOperationsLogFile.open(kFileName, std::fstream::out | std::fstream::trunc);
}

void Logger::calculateCurrentLogFileLinesCount()
{
    ifstream kOperationLogFile(kFileName);
    std::string line;
    while (std::getline(kOperationLogFile , line)){
        ++mOperationsLogFileLinesNumber;
    }
    if (kOperationLogFile.is_open()){
        kOperationLogFile.close();
    }
}

bool Logger::formatLogFileName(
    string& str,
    const string& from,
    const string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
