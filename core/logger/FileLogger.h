/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#ifndef GNS_OBSERVERS_CHAIN_BACK_FILELOGGER_H
#define GNS_OBSERVERS_CHAIN_BACK_FILELOGGER_H

#include <cstdio>
#include <string>

#include <boost/filesystem.hpp>

#include "../common/exceptions/IOError.h"

using namespace std;

namespace fs = boost::filesystem;

class FileLogger {

public:
    FileLogger(){
        obtainFileDescriptor();
    }

    ~FileLogger(){
        if (mFileDescriptor != NULL) {
            fclose(mFileDescriptor);
        }
    }

    void addLine(const char *sentence){
        fseek(mFileDescriptor, 0, SEEK_END);
        fputs(sentence, mFileDescriptor);
        fputs("\n", mFileDescriptor);
        fflush(mFileDescriptor);
    }

    void writeEmptyLine(size_t count){
        for (size_t i=0; i<count; i++) {
            fputs("\n", mFileDescriptor);
        }
        fflush(mFileDescriptor);
    }

private:
    void obtainFileDescriptor() {
        if (isFileExist()) {
            mFileDescriptor = fopen(kFileName.c_str(), kModeUpdate.c_str());

        } else {
            mFileDescriptor = fopen(kFileName.c_str(), kModeCreate.c_str());
        }
        checkFileDescriptor();
    }

    const bool isFileExist(){
        return fs::exists(fs::path(kFileName.c_str()));
    }

    void checkFileDescriptor() {
        if (mFileDescriptor == NULL) {
            throw IOError(string("Unable to obtain file descriptor.").c_str());
        }
    }

private:
    const string kModeCreate = "w+";
    const string kModeUpdate = "r+";

private:
    FILE *mFileDescriptor;
    string kFileName = "routing_tables_log.txt";

};

#endif //GNS_OBSERVERS_CHAIN_BACK_FILELOGGER_H
