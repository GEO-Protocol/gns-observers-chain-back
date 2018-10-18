#ifndef GNS_OBSERVERS_CHAIN_BACK_COMMANDSPARSER_H
#define GNS_OBSERVERS_CHAIN_BACK_COMMANDSPARSER_H

#include "Types.h"
#include "../../commands/commands/BaseUserCommand.h"
#include "../../../logger/LoggerMixin.hpp"
#include "../../../exceptions/common/RuntimeError.h"

#include <boost/asio.hpp>


namespace as = boost::asio;


/**
 * User commands are transmitted via text protocol.
 * CommandsParser is used for parsing received user input
 * and deserializing them into commands instances.
 */
class CommandsParser:
    public LoggerMixin {

public:
    static const size_t kUUIDHexRepresentationSize = 36;
    static const size_t kMinCommandSize = kUUIDHexRepresentationSize + 2;
    static const size_t kAverageCommandIdentifierLength = 15;

public:
    explicit CommandsParser(
        Logger &log);

    void appendReadData(
        as::streambuf *buffer,
        size_t receivedBytesCount);

    pair<bool, BaseUserCommand::Shared> processReceivedCommands();

protected:
    inline pair<bool, BaseUserCommand::Shared> tryDeserializeCommand();

    inline pair<bool, BaseUserCommand::Shared> tryParseCommand(
        const CommandUUID &uuid,
        const string &identifier,
        const string &buffer);

    inline pair<bool, BaseUserCommand::Shared> commandIsInvalidOrIncomplete();

    void cutBufferUpToNextCommand();

    const string& logHeader() const noexcept override;

    template <typename CommandType, typename... Args>
    inline pair<bool, BaseUserCommand::Shared> newCommand(
        const CommandUUID &uuid,
        const string &buffer) const
    {
        return make_pair(
            true,
            static_pointer_cast<BaseUserCommand>(
                make_shared<CommandType>(
                    uuid,
                    buffer)));
    }

private:
    string mBuffer;
};

#endif //GNS_OBSERVERS_CHAIN_BACK_COMMANDSPARSER_H
