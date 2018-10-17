#ifndef GEO_NETWORK_CLIENT_TYPES_H
#define GEO_NETWORK_CLIENT_TYPES_H


#include <memory>
#include <cstdint>


using namespace std;


/*
 * Byte
 */
typedef uint8_t byte;
typedef std::shared_ptr<byte> BytesShared;
typedef std::shared_ptr<const byte> ConstBytesShared;


#endif //GEO_NETWORK_CLIENT_TYPES_H
