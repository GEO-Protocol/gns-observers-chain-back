#ifndef GNS_OBSERVERS_CHAIN_BACK_MEMORYERROR_H
#define GNS_OBSERVERS_CHAIN_BACK_MEMORYERROR_H

#include "Exception.h"


class MemoryError: public Exception {
    using Exception::Exception;
};

#endif //GNS_OBSERVERS_CHAIN_BACK_MEMORYERROR_H
