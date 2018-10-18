/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#include "CommandsInterface.h"


CommandsInterface::CommandsInterface(
    as::io_service &ioService,
    Logger &logger) :

    mIOService(ioService),
    LoggerMixin(mLog)
{
    // Try to open FIFO file in non-blocking manner.
    // In case if this file will be opened in standard blocking manner -
    // it will freeze whole the process,
    // until writer would connect to the FIFO from the other side.
    //
    // For the server realisation this makes the process unusable,
    // because it can't be demonized, until the commands writer will
    // open the commands file for writing.
    if (!isFIFOExists()) {
        createFIFO(kPermissionsMask);
    }

    mFIFODescriptor = open(
        FIFOFilePath().c_str(),
        O_RDONLY | O_NONBLOCK);

    if (mFIFODescriptor == -1) {
        throw IOError("CommandsInterface::CommandsInterface: "
                          "Can't open FIFO file.");
    }

    try {
        mFIFOStreamDescriptor = unique_ptr<as::posix::stream_descriptor> (new as::posix::stream_descriptor(
            mIOService,
            mFIFODescriptor));
        mFIFOStreamDescriptor->non_blocking(true);

    } catch (std::bad_alloc &) {
        throw MemoryError("CommandsInterface::CommandsInterface: "
                              "Can not allocate enough memory for fifo stream descriptor.");
    }

    try {
        mReadTimeoutTimer = unique_ptr<as::deadline_timer> (new as::deadline_timer(
            mIOService,
            boost::posix_time::seconds(2)));

    } catch (std::bad_alloc &) {
        throw MemoryError(
            "CommandsInterface::CommandsInterface: "
                "Can not allocate enough memory for fifo read timer.");
    }

    try {
        mCommandsParser = unique_ptr<CommandsParser> (new CommandsParser(mLog));

    } catch (std::bad_alloc &) {
        throw MemoryError(
            "CommandsInterface::CommandsInterface: "
                "Can not allocate enough memory for commands parser.");
    }

    mCommandBuffer.prepare(kCommandBufferSize);
}

CommandsInterface::~CommandsInterface()
{
    mFIFOStreamDescriptor->cancel();
    mReadTimeoutTimer->cancel();
    close(mFIFODescriptor);
}

void CommandsInterface::beginCommandsAccepting()
{
    asyncReceiveNextCommand();
}

void CommandsInterface::asyncReceiveNextCommand()
{
    as::async_read_until(
        *mFIFOStreamDescriptor,
        mCommandBuffer,
        kCommandsSeparator,
        boost::bind(
            &CommandsInterface::handleReceivedInfo,
            this,
            as::placeholders::error,
            as::placeholders::bytes_transferred));
}

/** Parse received commands data until parsing is successfully.
 *  (several commands may be read at once, so the parsing should be repeated
 *  until all the received data would be processed)
 *
 *  It is possible, that FIFO file is not opened by the other side.
 *  Next read attempt should be performed with a Timeout,
 *  to prevent heavy processor usage.
 */
void CommandsInterface::handleReceivedInfo(
    const boost::system::error_code &error,
    const size_t bytesTransferred)
{
    if (!error || error == as::error::message_size) {
        mCommandsParser->appendReadData(
            &mCommandBuffer,
            bytesTransferred);
        mCommandBuffer.consume(bytesTransferred);

        while (true) {
            auto flagAndCommand = mCommandsParser->processReceivedCommands();
            if (flagAndCommand.first){
                commandReceivedSignal(flagAndCommand.second);
            } else {
                break;
            }
        }
        asyncReceiveNextCommand();

    } else {
        if (error == as::error::eof) {
            mReadTimeoutTimer->expires_from_now(boost::posix_time::seconds(1));
            mReadTimeoutTimer->async_wait(
                boost::bind(
                    &CommandsInterface::handleTimeout,
                    this,
                    as::placeholders::error));

        } else {
            // Looks like FIFO file is corrupted or unreachable.
            // Next read attempt should be performed with a long Timeout.
            mReadTimeoutTimer->expires_from_now(boost::posix_time::minutes(10));
            mReadTimeoutTimer->async_wait(
                boost::bind(
                &CommandsInterface::handleTimeout,
                this,
                as::placeholders::error));
        }
    }
}

void CommandsInterface::handleTimeout(
    const boost::system::error_code &errorMessage)
{
    if (errorMessage) {
        error() << "handleTimeout error: " << errorMessage.message();
    }

    asyncReceiveNextCommand();
}

const char *CommandsInterface::FIFOName() const {

    return kFIFOName;
}

const string& CommandsInterface::logHeader() const
    noexcept
{
    static string header = "[CommandsInterface]";
    return header;
}
