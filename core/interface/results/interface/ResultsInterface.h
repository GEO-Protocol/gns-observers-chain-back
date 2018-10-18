#ifndef GNS_OBSERVERS_CHAIN_BACK_RESULTSINTERFACE_H
#define GNS_OBSERVERS_CHAIN_BACK_RESULTSINTERFACE_H

#include "../../BaseFIFOInterface.hpp"
#include "../../../logger/Logger.h"
#include "../../../exceptions/common/IOError.h"
#include "../../../exceptions/common/MemoryError.h"

#include <boost/bind.hpp>

#include <string>


using namespace std;


class ResultsInterface:
    public BaseFIFOInterface {

public:
    explicit ResultsInterface(
        Logger &logger);

    ~ResultsInterface();

    void writeResult(
        const char *bytes,
        const size_t bytesCount);

private:
    virtual const char* FIFOName() const;

public:
    static const constexpr char *kFIFOName = "results.fifo";
    static const constexpr unsigned int kPermissionsMask = 0755;

private:
    Logger &mLog;
};

#endif //GNS_OBSERVERS_CHAIN_BACK_RESULTSINTERFACE_H
