/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#include "CommandUUID.h"


CommandUUID::CommandUUID():
    uuid(boost::uuids::random_generator()())
{}

CommandUUID::CommandUUID(
    uuid const &u):
    boost::uuids::uuid(u)
{}

CommandUUID::CommandUUID(
    const string &hex):
    uuid(boost::lexical_cast<uuid>(hex))
{}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
CommandUUID::CommandUUID(
    const uint8_t *bytes)
{
    memcpy(data, bytes, kBytesSize);
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
CommandUUID::CommandUUID(
    CommandUUID const &u)
    = default;
#pragma clang diagnostic pop

const string CommandUUID::toString()
    const
{
    return boost::lexical_cast<string>(this);
}

const CommandUUID& CommandUUID::empty ()
{
    static const CommandUUID kEmpty("00000000-0000-0000-0000-000000000000");
    return kEmpty;
}


