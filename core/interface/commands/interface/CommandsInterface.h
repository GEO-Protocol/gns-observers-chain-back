/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#ifndef GNS_OBSERVERS_CHAIN_BACK_COMMANDSINTERFACE_H
#define GNS_OBSERVERS_CHAIN_BACK_COMMANDSINTERFACE_H


#include "../../../exceptions/common/MemoryError.h"
#include "../../../logger/LoggerMixin.hpp"
#include "../../../logger/Logger.h"

#include "../../BaseFIFOInterface.hpp"
#include "CommandsParser.h"

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include <string>
#include <memory>


using namespace std;
namespace signals = boost::signals2;


/**
 * User commands are transmitted via named pipe (FIFO on Linux).
 * This class is used to asynchronously receive them, parse,
 * and transfer for the further execution.
 */
class CommandsInterface:
    public BaseFIFOInterface,
    public LoggerMixin {

public:
    signals::signal<void(BaseUserCommand::Shared)> commandReceivedSignal;

public:
    static const constexpr char *kFIFOName = "commands.fifo";
    static const constexpr unsigned int kPermissionsMask = 0755;

public:
    explicit CommandsInterface(
        as::io_service &ioService,
        Logger &logger);

    ~CommandsInterface();

    void beginCommandsAccepting();

protected:
    void asyncReceiveNextCommand();

    void handleReceivedInfo(
        const boost::system::error_code &error,
        size_t bytesTransferred);

    void handleTimeout(
        const boost::system::error_code &error);

    const char* FIFOName()
        const
        override;

    const string& logHeader()
        const
        noexcept
        override;

protected:
    static const constexpr size_t kCommandBufferSize = 1024;

protected:
    as::io_service &mIOService;
    as::streambuf mCommandBuffer;

    unique_ptr<as::posix::stream_descriptor> mFIFOStreamDescriptor;
    unique_ptr<as::deadline_timer> mReadTimeoutTimer;
    unique_ptr<CommandsParser> mCommandsParser;
};

#endif //GNS_OBSERVERS_CHAIN_BACK_COMMANDSINTERFACE_H
