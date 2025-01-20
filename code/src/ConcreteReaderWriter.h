class ConcreteReaderWriter : public AbstractReaderWriter {
public:
    void lockWriting() {
        writeMutex.lock();
    }

    void unlockWriting() {
        writeMutex.unlock();
    }

    void lockReading() {
        readMutex.lock();
    }

    void unlockReading() {
        readMutex.unlock();
    }

private:
    std::mutex writeMutex;
    std::mutex readMutex;
};
