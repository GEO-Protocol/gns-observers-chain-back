/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#ifndef GNS_OBSERVERS_CHAIN_BACK_NOTFOUNDERROR_H
#define GNS_OBSERVERS_CHAIN_BACK_NOTFOUNDERROR_H

#include "Exception.h"


class NotFoundError: public Exception {
    using Exception::Exception;
};

#endif //GNS_OBSERVERS_CHAIN_BACK_NOTFOUNDERROR_H
