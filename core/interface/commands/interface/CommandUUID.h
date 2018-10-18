#ifndef GNS_OBSERVERS_CHAIN_BACK_COMMANDUUID_H
#define GNS_OBSERVERS_CHAIN_BACK_COMMANDUUID_H


#include "../../../common/memory/utils.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>


using namespace boost::uuids;
using namespace std;


/**
 * todo: add comment
 */
class CommandUUID:
    public uuid {

public:
    using Shared = shared_ptr<CommandUUID>;

public:
    static const size_t kHexSize = 36;
    static const size_t kBytesSize = 16;

    static const CommandUUID& empty();

public:
    explicit CommandUUID();

    explicit CommandUUID(
        uuid const &u);

    explicit CommandUUID(
        const string &hex);

    explicit CommandUUID(
        const uint8_t *bytes);

    CommandUUID(
        CommandUUID const &u);

    const string toString() const;

    using boost::uuids::uuid::operator=;
};

/**
 * todo: add comment
 */
namespace std {
    template <>
    class hash<CommandUUID>{
    public :
        size_t operator()(const CommandUUID &nodeUUID) const {
            const auto kPtr = reinterpret_cast<const size_t* >(nodeUUID.data);
            return *kPtr;
        }
    };
}

#endif //GNS_OBSERVERS_CHAIN_BACK_COMMANDUUID_H
