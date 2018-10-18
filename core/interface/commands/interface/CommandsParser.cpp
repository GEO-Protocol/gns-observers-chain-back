#include "CommandsParser.h"


CommandsParser::CommandsParser(
    Logger &log):

    LoggerMixin(log)
{}

/**
 * Copies data, received from the commands FIFO, into the internal buffer.
 * There is a non-zero probability, that buffer will contains some part of previous command,
 * that was not parsed on previous steps. It may happen, when async read from FIFO returned
 * one and a half of the command. In such a case - newly received data will be appended to the
 * previously received.
 *
 * @param buffer - pointer to the buffer, that collects raw input from the user (ASIO buffer).
 * @param receivedBytesCount - specifies how many bytes was received by the last FIFO read.
 *      (how many bytes are in the "buffer" at the moment)
 */
void CommandsParser::appendReadData(
    as::streambuf *buffer,
    const size_t receivedBytesCount)
{
    if (receivedBytesCount == 0) {
        return;
    }

    // TODO: check if using buffer->data() doesn't leads to the buffer overflow.
    const char *data = as::buffer_cast<const char*>(buffer->data());
    for (size_t i = 0; i < receivedBytesCount; ++i) {
        mBuffer.push_back(*data++);
    }
}

/**
 * Parses internal buffer for user commands.
 * Returns <true, command> in case, when command was parsed well.
 * Otherwise returns <false, nullptr>.
 *
 * This method should be called several times to parse all received commands.
 *
 * See: tryDeserializeCommand() for the details.
 */
pair<bool, BaseUserCommand::Shared> CommandsParser::processReceivedCommands()
{
    return tryDeserializeCommand();
}

/*!
 * Tries to deserialize received command.
 * In case when buffer is too short to contains even one command -
 * will return immediately.
 *
 * First value of returned pair indicates if command was parsed succesfully,
 * and, if so, - the second one will contains shared pointer to the command instance itself.
 * Otherwise - the second value of the pair will contains "nullptr".
 */
pair<bool, BaseUserCommand::Shared> CommandsParser::tryDeserializeCommand()
{
    if (mBuffer.size() < kMinCommandSize) {
        return commandIsInvalidOrIncomplete();
    }

    size_t nextCommandSeparatorIndex = mBuffer.find(kCommandsSeparator);
    if (nextCommandSeparatorIndex == string::npos) {
        return commandIsInvalidOrIncomplete();
    }

    CommandUUID commandUUID;
    try {
        string hexUUID = mBuffer.substr(0, kUUIDHexRepresentationSize);
        commandUUID = boost::lexical_cast<uuid>(hexUUID);

    } catch (...) {
        cutBufferUpToNextCommand();
        return commandIsInvalidOrIncomplete();
    }


    string commandIdentifier;
    const size_t identifierOffset = kUUIDHexRepresentationSize + 1;

    commandIdentifier.reserve(kAverageCommandIdentifierLength);
    size_t nextTokenOffset = identifierOffset;
    for (size_t i = identifierOffset; i < mBuffer.size(); ++i) {
        char symbol = mBuffer.at(i);
        if (symbol == kTokensSeparator || symbol == kCommandsSeparator) {
            break;
        }
        nextTokenOffset += 1;
        commandIdentifier.push_back(symbol);
    }
    nextTokenOffset += 1;

    if (commandIdentifier.size() == 0) {
        cutBufferUpToNextCommand();
        return commandIsInvalidOrIncomplete();
    }

    try {
        auto nextCommandBegin = mBuffer.find(kCommandsSeparator);
        if (nextCommandBegin == string::npos) {
            nextCommandBegin = mBuffer.size();
        }

        auto command = tryParseCommand(
            commandUUID,
            commandIdentifier,
            mBuffer.substr(
                nextTokenOffset,
                nextCommandBegin - nextTokenOffset + 1));

        cutBufferUpToNextCommand();
        return command;

    } catch (std::exception &e) {
        cutBufferUpToNextCommand();
        return commandIsInvalidOrIncomplete();
    }
}

/*!
 * Checks identifier and tries to build relevant command object.
 * @returns <true, command object> in case of success, otherwise returns <false, nullptr>.
 *
 * @param uuid - uuid of the received command (parsed on previous step).
 * @param identifier - identifier of the received command (parsed on the previous step).
 * @param buffer - content of the command without it's identifier.
 *
 * @throws ValueError in case if @param identifier would be unexpected.
 */
pair<bool, BaseUserCommand::Shared> CommandsParser::tryParseCommand(
    const CommandUUID &uuid,
    const string &identifier,
    const string &buffer)
{
    BaseUserCommand *command = nullptr;
    try {

        // if command == some command - parse it
        if (false) {

        } else {
            throw RuntimeError(
                "CommandsParser::tryParseCommand: "
                "unexpected command identifier received. " + identifier);
        }

    } catch (bad_alloc &) {
        error() << "tryParseCommand: Memory allocation error occurred on command instance creation. "
                << "Command was dropped. ";

        return commandIsInvalidOrIncomplete();

    } catch (exception &e){
        mLog.logException("CommandsParser", e);
        return commandIsInvalidOrIncomplete();
    }

    return make_pair(
        true,
        BaseUserCommand::Shared(command));
}

/*!
 * Removes last command (valid, or invalid) from the buffer.
 * Stops on commands separator symbol.
 * If no commands separator symbol is present - clears buffer at all.
 */
void CommandsParser::cutBufferUpToNextCommand()
{
    size_t nextCommandSeparatorIndex = mBuffer.find(kCommandsSeparator);
    if (mBuffer.size() > (nextCommandSeparatorIndex + 1)) {
        // Buffer contains other commands (or their parts), and them should be keept;
        mBuffer = mBuffer.substr(nextCommandSeparatorIndex + 1, mBuffer.size() - 1);

    } else {
        // Buffer doesn't contains any other commands (even parts).
        mBuffer.clear();
    }

    // Resize the capacity to free unused memory.
    mBuffer.shrink_to_fit();
}

pair<bool, BaseUserCommand::Shared> CommandsParser::commandIsInvalidOrIncomplete()
{
    return make_pair(
        false,
        nullptr);
}
