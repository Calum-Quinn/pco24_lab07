#include <gtest/gtest.h>

class ReaderWriterChecker {
public:
    void writerIn()
    {
        checkerLock.lock();
        nbWriters++;
        // Check after getting in
        check();
        checkerLock.unlock();

    }

    void writerOut()
    {
        checkerLock.lock();
        // Check before leaving
        check();
        nbWriters--;
        checkerLock.unlock();
    }

    void readerIn()
    {
        checkerLock.lock();
        nbReaders++;
        // Check after getting in
        check();
        checkerLock.unlock();
    }

    void readerOut()
    {
        checkerLock.lock();
        // Check before leaving
        check();
        nbReaders--;
        checkerLock.unlock();
    }

protected:

    void check()
    {
        ASSERT_EQ((nbReaders == 0) || (nbWriters == 0), true);
        ASSERT_LE(nbWriters, 1);
        ASSERT_GE(nbWriters, 0);
        ASSERT_GE(nbReaders, 0);
    }

    std::mutex checkerLock;

    int nbReaders{0};
    int nbWriters{0};
};
