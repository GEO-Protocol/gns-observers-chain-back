#include "CommandUUID.h"


CommandUUID::CommandUUID():
    uuid(boost::uuids::random_generator()()){
}

CommandUUID::CommandUUID(
    uuid const &u):
    boost::uuids::uuid(u){
}

CommandUUID::CommandUUID(
    const string &hex):
    uuid(boost::lexical_cast<uuid>(hex)){
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
CommandUUID::CommandUUID(
    const uint8_t *bytes){

    memcpy(data, bytes, kBytesSize);
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
CommandUUID::CommandUUID(
    CommandUUID const &u)
    = default;
#pragma clang diagnostic pop

const string CommandUUID::toString() const{
    return boost::lexical_cast<string>(this);
}

const CommandUUID& CommandUUID::empty () {
    static const CommandUUID kEmpty("00000000-0000-0000-0000-000000000000");
    return kEmpty;
}


