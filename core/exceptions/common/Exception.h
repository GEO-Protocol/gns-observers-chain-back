/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#ifndef GNS_OBSERVERS_CHAIN_BACK_EXCEPTIONS_H
#define GNS_OBSERVERS_CHAIN_BACK_EXCEPTIONS_H

#include <string>


using namespace std;

// todo: add stacktrace (source code line, subsystem, etc)
class Exception : public std::exception {
public:
    virtual ~Exception() throw(){}

    explicit Exception(const std::string &message);
    explicit Exception(const char *message) :
        mMessage(message) {};

    const std::string message() const;

    virtual const char* what() const throw(){
        return mMessage.c_str();
    }

private:
    std::string mMessage;
};


#endif //GNS_OBSERVERS_CHAIN_BACK_EXCEPTIONS_H
