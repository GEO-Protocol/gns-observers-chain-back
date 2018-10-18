#ifndef GNS_OBSERVERS_CHAIN_BACK_BASEUSERCOMMAND_H
#define GNS_OBSERVERS_CHAIN_BACK_BASEUSERCOMMAND_H


#include "../../results/results/CommandResult.h"


class CommandResult;


class BaseUserCommand {
public:
    using Shared = shared_ptr<BaseUserCommand>;

public:
    explicit BaseUserCommand(
        const string& identifier);

    BaseUserCommand(
        const CommandUUID &commandUUID,
        const string& identifier);

    virtual ~BaseUserCommand() = default;

    const CommandUUID &UUID() const;

    const string &identifier() const;

    /*
     * All commands results codes must be listed here.
     * The order is from common usage codes to the command-specific codes.
     */

    CommandResult::SharedConst responseOK() const;

    CommandResult::SharedConst responseCreated() const;

    /*
     * ...
     *
     * Other commands results must go here.
     *
     * ...
     */

protected:
    CommandResult::SharedConst makeResult(
        const uint16_t code) const;

private:
    const CommandUUID mCommandUUID;
    const string mCommandIdentifier;
};
#endif //GNS_OBSERVERS_CHAIN_BACK_BASEUSERCOMMAND_H
