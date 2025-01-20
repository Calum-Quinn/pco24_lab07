#include "abstractreaderwriter.h"

#include <mutex>
#include <shared_mutex>

class ReaderWriterLock : public AbstractReaderWriter {
    std::mutex writeMutex;
    std::shared_mutex readWriteMutex;

public:
    void lockReading() override {
        readWriteMutex.lock_shared();
    }

    void unlockReading() override {
        readWriteMutex.unlock_shared();
    }

    void lockWriting() override {
        writeMutex.lock();
        readWriteMutex.lock();
    }

    void unlockWriting() override {
        readWriteMutex.unlock();
        writeMutex.unlock();
    }
};
