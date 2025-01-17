#ifndef OurModel_H
#define OurModel_H

#include <iostream>

#include "pcomodel.h"
#include "scenariobuilder.h"

int getNumber()
{
    return number;
}

// TaskWriter
class ThreadA : public ObservableThread
{
public:
    explicit ThreadA(std::string id = "") :
        ObservableThread(std::move(id))
    {
        scenarioGraph = std::make_unique<ScenarioGraph>();
        auto scenario = scenarioGraph->createNode(this, -1);
        auto p1 = scenarioGraph->createNode(this, 1);
        auto p2 = scenarioGraph->createNode(this, 2);
        auto p3 = scenarioGraph->createNode(this, 3);
        auto p4 = scenarioGraph->createNode(this, 4);
        scenario->next.push_back(p1);
        p1->next.push_back(p2);
        p2->next.push_back(p3);
        p2->next.push_back(p4);
        p3->next.push_back(p3);
        p3->next.push_back(p4);
        scenarioGraph->setInitialNode(scenario);
    }

private:
    void run() override
    {
        startSection(1);
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> dis(1, 1000);

        startSection(2);
        for(int iter = 0; iter < nbIterations; iter ++) {
            startSection(3);
            resource->lockWriting();
            checker->writerIn();
            logger() << "Task W" << tid << ": accessing " << iter << '\n';
            PcoThread::usleep(static_cast<uint64_t>(dis(gen)));
            checker->writerOut();
            resource->unlockWriting();
            logger() << "Task W" << tid << ": leaving" << '\n';
            PcoThread::usleep(static_cast<uint64_t>(dis(gen)));
        }
        startSection(4);
        endScenario();
    }
};

// TaskReader
class ThreadB : public ObservableThread
{
public:
    explicit ThreadB(std::string id = "") :
        ObservableThread(std::move(id))
    {
        scenarioGraph = std::make_unique<ScenarioGraph>();
        auto scenario = scenarioGraph->createNode(this, -1);
        auto p1 = scenarioGraph->createNode(this, 1);
        auto p2 = scenarioGraph->createNode(this, 2);
        auto p3 = scenarioGraph->createNode(this, 3);
        auto p4 = scenarioGraph->createNode(this, 4);
        scenario->next.push_back(p1);
        p1->next.push_back(p2);
        p2->next.push_back(p3);
        p2->next.push_back(p4);
        p3->next.push_back(p3);
        p3->next.push_back(p4);
        scenarioGraph->setInitialNode(scenario);
    }

private:
    void run() override
    {
        startSection(1);
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> dis(1, 1000);

        startSection(2);
        for(int iter = 0; iter < nbIterations; iter ++) {
            startSection(3);
            resource->lockReading();
            checker->readerIn();
            logger() << "Task R" << tid << ": accessing " << iter << '\n';
            PcoThread::usleep(static_cast<uint64_t>(dis(gen)));
            checker->readerOut();
            resource->unlockReading();
            logger() << "Task R" << tid << ": leaving" << '\n';
            PcoThread::usleep(static_cast<uint64_t>(dis(gen)));
        }
        startSection(4);
        endScenario();
    }
};

class OurModel: public PcoModel
{
public:

    bool checkInvariants() override {
        // For testing purpose :
        //std::cout << "Checking invariant" << std::endl;
        return true;
    }

    void build() override {
#ifdef PREDEFINED_SCENARIOS

        // threads.emplace_back(std::make_unique<ThreadA>("1"));
        // threads.emplace_back(std::make_unique<ThreadB>("2"));

        // auto t1 = threads[0].get();
        // auto t2 = threads[1].get();
        // auto builder = std::make_unique<PredefinedScenarioBuilderIter>();
        // std::vector<Scenario> scenarios = {
        //     {{t1, 1},{t1, 2},{t1, 3},{t2, 4},{t2, 5},{t2, 6}},
        //     {{t2, 4},{t2, 5},{t2, 6},{t1, 1},{t1, 2},{t1, 3}}
        // };
        // builder->setScenarios(scenarios);
        // scenarioBuilder = std::move(builder);

#else // PREDEFINED_SCENARIOS

        threads.emplace_back(std::make_unique<ThreadA>("1"));
        threads.emplace_back(std::make_unique<ThreadB>("2"));

        scenarioBuilder = std::make_unique<ScenarioBuilderBuffer>();
        scenarioBuilder->init(threads, 30);

#endif // PREDEFINED_SCENARIOS
    }

    void preRun(Scenario &/*scenario*/) override {

    }

    void postRun(Scenario &scenario) override {
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Scenario : ";
        ScenarioPrint::printScenario(scenario);
        std::cout << "Number = " << getNumber() << std::endl;
        possibleNumber.insert(getNumber());
    }

    std::set<int> possibleNumber;

    void finalReport() override {
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Possible output number : ";
        for (const int &value : possibleNumber)
            std::cout << value << ", ";
        std::cout << std::endl;
        std::cout << std::flush;
    }

};

#endif // OurModel_H
