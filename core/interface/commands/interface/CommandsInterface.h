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
//using namespace boost::uuids;
namespace signals = boost::signals2;



// todo: refactor, use signals

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

    const char* FIFOName() const override;

    const string& logHeader() const
        noexcept override;

public:
    static const constexpr char *kFIFOName = "commands.fifo";
    static const constexpr unsigned int kPermissionsMask = 0755;

protected:
    as::io_service &mIOService;
    as::streambuf mCommandBuffer;
    static const constexpr size_t kCommandBufferSize = 1024;

    unique_ptr<as::posix::stream_descriptor> mFIFOStreamDescriptor;
    unique_ptr<as::deadline_timer> mReadTimeoutTimer;
    unique_ptr<CommandsParser> mCommandsParser;
};

#endif //GNS_OBSERVERS_CHAIN_BACK_COMMANDSINTERFACE_H
