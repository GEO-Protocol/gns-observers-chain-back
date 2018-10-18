#ifndef GNS_OBSERVERS_CHAIN_BACK_NEWCOMPLAINTCOMMAND_H
#define GNS_OBSERVERS_CHAIN_BACK_NEWCOMPLAINTCOMMAND_H

#include "../BaseUserCommand.h"
#include "../../../../exceptions/common/ValueError.h"


class NewComplaintCommand :
    public BaseUserCommand {

public:
    typedef shared_ptr<NewComplaintCommand> Shared;

public:
    NewComplaintCommand(
        const CommandUUID &uuid,
        const string &commandBuffer);

    static const string &identifier();

    const vector<NodeUUID> &gateways() const;

    const SerializedEquivalent equivalent() const;

    CommandResult::SharedConst resultOk(
        string &totalBalancesStr) const;

private:
    size_t mGatewaysCount;
    vector<NodeUUID> mGateways;
    SerializedEquivalent mEquivalent;
};


#endif //GNS_OBSERVERS_CHAIN_BACK_NEWCOMPLAINTCOMMAND_H
