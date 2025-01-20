#ifndef OUR_MODEL_H
#define OUR_MODEL_H

#include <iostream>
#include <random>
#include <memory>
#include <set>
#include <pcosynchro/pcologger.h>

#include "pcomodel.h"
#include "scenariobuilder.h"
#include "abstractreaderwriter.h"
#include "ConcreteReaderWriter.h"
#include "ReaderWriterChecker.h"

std::unique_ptr<ConcreteReaderWriter> resource = std::make_unique<ConcreteReaderWriter>();
std::shared_ptr<ReaderWriterChecker> checker = std::make_unique<ReaderWriterChecker>();

// Nombre d'itérations configurable
static int nbIterations = 10;

// Accesseurs pour nbIterations
int getNbIterations() {
    return nbIterations;
}

// Classes ThreadA et ThreadB (écrivain et lecteur)
class ThreadA : public ObservableThread {
public:
    explicit ThreadA(std::shared_ptr<ConcreteReaderWriter> resource,
                     std::shared_ptr<ReaderWriterChecker> checker,
                     const std::string &id = "") : 
        ObservableThread(id),
        resource(std::move(resource)),
        checker(std::move(checker))
    {
        // Création du graphe de scénario
        auto scenario = scenarioGraph->createNode(this, -1);
        auto p1 = scenarioGraph->createNode(this, 1);
        auto p2 = scenarioGraph->createNode(this, 2);
        auto p3 = scenarioGraph->createNode(this, 3);
        auto p4 = scenarioGraph->createNode(this, 4);

        scenario->next.push_back(p1);
        p1->next.push_back(p2);
        p2->next.push_back(p3);
        p2->next.push_back(p4);
        p3->next.push_back(p4);
        scenarioGraph->setInitialNode(scenario);
    }

private:
    void run() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 1000);

        for (int iter = 0; iter < getNbIterations(); ++iter) {
            startSection(1);
            resource->lockWriting();
            checker->writerIn();
            logger() << "ThreadA: Writing iteration " << iter << std::endl;
            PcoThread::usleep(dis(gen));
            checker->writerOut();
            resource->unlockWriting();
            PcoThread::usleep(dis(gen));
            endSection();
        }
        endScenario();
    }

    std::shared_ptr<ConcreteReaderWriter> resource;
    std::shared_ptr<ReaderWriterChecker> checker;
};

class ThreadB : public ObservableThread {
public:
    explicit ThreadB(std::shared_ptr<ConcreteReaderWriter> resource,
                     std::shared_ptr<ReaderWriterChecker> checker,
                     const std::string &id = "") : 
        ObservableThread(id),
        resource(std::move(resource)),
        checker(std::move(checker))
    {
        // Création du graphe de scénario
        auto scenario = scenarioGraph->createNode(this, -1);
        auto p1 = scenarioGraph->createNode(this, 1);
        auto p2 = scenarioGraph->createNode(this, 2);
        auto p3 = scenarioGraph->createNode(this, 3);
        auto p4 = scenarioGraph->createNode(this, 4);

        scenario->next.push_back(p1);
        p1->next.push_back(p2);
        p2->next.push_back(p3);
        p2->next.push_back(p4);
        p3->next.push_back(p4);
        scenarioGraph->setInitialNode(scenario);
    }

private:
    void run() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 1000);

        for (int iter = 0; iter < getNbIterations(); ++iter) {
            startSection(1);
            resource->lockReading();
            checker->readerIn();
            logger() << "ThreadB: Reading iteration " << iter << std::endl;
            PcoThread::usleep(dis(gen));
            checker->readerOut();
            resource->unlockReading();
            PcoThread::usleep(dis(gen));
            endSection();
        }
        endScenario();
    }

    std::shared_ptr<ConcreteReaderWriter> resource;
    std::shared_ptr<ReaderWriterChecker> checker;
};

// Classe OurModel
class OurModel : public PcoModel {
public:
    bool checkInvariants() override {
        return true; // Toujours vrai dans ce cas
    }

    void build() override {
        auto sharedResource = std::make_shared<ConcreteReaderWriter>();
        auto sharedChecker = std::make_shared<ReaderWriterChecker>();
#ifdef PREDEFINED_SCENARIOS

        auto t1 = std::make_unique<ThreadA>(sharedResource, sharedChecker, "A");
        auto t2 = std::make_unique<ThreadB>(sharedResource, sharedChecker, "B");

        std::vector<Scenario> scenarios = {
            {{t1.get(), 1}, {t1.get(), 2}, {t2.get(), 1}, {t2.get(), 2}},
            {{t2.get(), 1}, {t2.get(), 2}, {t1.get(), 1}, {t1.get(), 2}}
        };

        auto builder = std::make_unique<PredefinedScenarioBuilderIter>();
        builder->setScenarios(scenarios);
        scenarioBuilder = std::move(builder);
#else
        threads.emplace_back(std::make_unique<ThreadA>(sharedResource, sharedChecker, "A"));
        threads.emplace_back(std::make_unique<ThreadB>(sharedResource, sharedChecker, "B"));

        scenarioBuilder = std::make_unique<ScenarioBuilderBuffer>();
        scenarioBuilder->init(threads, 30);
#endif
    }

    void postRun(Scenario &scenario) override {
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Scenario: ";
        ScenarioPrint::printScenario(scenario);
    }

    void finalReport() override {
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Possible Numbers: ";
    }

private:
};

#endif // OUR_MODEL_H
