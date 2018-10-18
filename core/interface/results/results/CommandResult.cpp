#include "CommandResult.h"


CommandResult::CommandResult(
    const string &commandIdentifier,
    const CommandUUID &commandUUID,
    const ResultCode resultCode) :

    mCommandIdentifier(commandIdentifier),
    mCommandUUID(commandUUID),
    mTimestampCompleted(nowUTC()),
    mResultCode(resultCode)
{}

CommandResult::CommandResult(
    const string &commandIdentifier,
    const CommandUUID &commandUUID,
    const ResultCode resultCode,
    const string &payload) :

    CommandResult(
        commandIdentifier,
        commandUUID,
        resultCode)
{
    mPayload = payload;
}

const string CommandResult::identifier() const
{
    return mCommandIdentifier;
}

const CommandUUID &CommandResult::commandUUID() const
{
    return mCommandUUID;
}

const uint16_t CommandResult::resultCode() const
{
    return mResultCode;
}

const DateTime &CommandResult::timestampCompleted() const
{
    return mTimestampCompleted;
}

const string CommandResult::serialize(
    const bool includePayload)
    const
{
    stringstream result;

    if (mPayload.empty() or includePayload == false) {
        result << mCommandUUID.toString() << kTokensSeparator
               << mResultCode << kCommandsSeparator;

    } else {
        result << mCommandUUID.toString() << kTokensSeparator
               << mResultCode << kTokensSeparator
               << mPayload << kCommandsSeparator;
    }

    return result.str();
}
