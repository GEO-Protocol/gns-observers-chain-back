#include "BaseUserCommand.h"

BaseUserCommand::BaseUserCommand(
    const string& identifier) :

    mCommandIdentifier(identifier)
{}

BaseUserCommand::BaseUserCommand(
    const CommandUUID &commandUUID,
    const string &identifier) :

    mCommandUUID(commandUUID),
    mCommandIdentifier(identifier)
{}


const CommandUUID &BaseUserCommand::UUID() const
{
    return mCommandUUID;
}

const string &BaseUserCommand::identifier() const
{
    return mCommandIdentifier;
}

/**
 * Shortcut for creating results in derived commands classes.
 *
 * @param code - result code that would be transferred out of the engine.
 */
CommandResult::SharedConst BaseUserCommand::makeResult(
    const uint16_t code) const
{
    return make_shared<const CommandResult>(
        identifier(),
        UUID(),
        code);
}

CommandResult::SharedConst BaseUserCommand::responseOK() const
{
    return makeResult(200);
}

CommandResult::SharedConst BaseUserCommand::responseCreated() const
{
    return makeResult(201);
}

