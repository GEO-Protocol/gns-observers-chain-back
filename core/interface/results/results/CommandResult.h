#ifndef GNS_OBSERVERS_CHAIN_BACK_COMMANDRESULT_H
#define GNS_OBSERVERS_CHAIN_BACK_COMMANDRESULT_H

#include "../../commands/interface/Types.h"
#include "../../commands/interface/CommandUUID.h"
#include "../../../common/Types.h"
#include "../../../common/time/utils.hpp"

#include <string>
#include <memory>


using namespace std;


class CommandResult {
public:
    using Shared = shared_ptr<CommandResult>;
    using SharedConst = shared_ptr<const CommandResult>;
    using ResultCode = uint16_t;

public:
    enum CommandResultCode {
        OK = 200,

        // todo: add codes from specification
    };

public:
    CommandResult(
        const string &commandIdentifier,
        const CommandUUID &commandUUID,
        const ResultCode resultCode);

    CommandResult(
        const string &commandIdentifier,
        const CommandUUID &commandUUID,
        const ResultCode resultCode,
        const string &payload);

    const string identifier()
        const;

    const CommandUUID &commandUUID()
        const;

    const ResultCode resultCode()
        const;

    const DateTime &timestampCompleted()
        const;

    /**
     * Serializes the result to the string representation usable for exporting.
     *
     * @param includePayload - if "true" the representation will contain payload (in case if present),
     * otherwise - payload would be dropped.
     */
    const string serialize(
        const bool includePayload=true)
        const;

private:
    CommandUUID mCommandUUID;
    ResultCode mResultCode;
    DateTime mTimestampCompleted;
    string mPayload;
    string mCommandIdentifier;
};


#endif //GNS_OBSERVERS_CHAIN_BACK_COMMANDRESULT_H
