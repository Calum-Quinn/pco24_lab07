#ifndef MODELEXERCICE13_H
#define MODELEXERCICE13_H

#include <iostream>
#include <random>

#include "pcomodel.h"
#include "scenariobuilder.h"

// The shared variable

class BridgeManager
{
private:
    PcoSemaphore accesData;
    PcoSemaphore waitAcces;

    float maxWeight;
    float usedWeight;
    unsigned nbWaitter;

public:
    BridgeManager(float maxWeight) : nbWaitter(0), waitAcces(0), accesData(1), maxWeight(maxWeight), usedWeight(0.0f) {}

    ~BridgeManager() {}

    void access(float weight)
    {
        accesData.acquire();
        startSection(1);

        while ((usedWeight + weight) > maxWeight) {
            startSection(2);
            ++nbWaitter;
            accesData.release();
            waitAcces.acquire();
            accesData.acquire();
            --nbWaitter;
        }
        startSection(3);
        usedWeight += weight;

        accesData.release();
    }

    void leave(float weight)
    {
        accesData.acquire();
        startSection(4);

        usedWeight -= weight;

        if (nbWaitter > 0) {
            startSection(5);
            waitAcces.release();
        }

        startSection(6);

        accesData.release();
    }
};


class CarThread : public ObservableThread
{
public:
    explicit CarThread(BridgeManager& bridgeManager, std::string id = "") :
        ObservableThread(std::move(id)), bridgeManager(bridgeManager)
    {
        scenarioGraph = std::make_unique<ScenarioGraph>();
        auto scenario = scenarioGraph->createNode(this, -1);
        auto p1 = scenarioGraph->createNode(this, 1);
        auto p2 = scenarioGraph->createNode(this, 2);
        auto p3 = scenarioGraph->createNode(this, 3);
        auto p4 = scenarioGraph->createNode(this, 4);
        auto p5 = scenarioGraph->createNode(this, 5);
        auto p6 = scenarioGraph->createNode(this, 6);
        scenario->next.push_back(p1);
        p1->next.push_back(p2);
        p1->next.push_back(p3);
        p2->next.push_back(p2);
        p2->next.push_back(p3);
        p3->next.push_back(p4);
        p4->next.push_back(p5);
        p4->next.push_back(p6);
        p5->next.push_back(p6);
        scenarioGraph->setInitialNode(scenario);
    }

private:
    BridgeManager& bridgeManager;
    float WEIGHT = 3.0f;

    void run() override
    {
        bridgeManager.access(WEIGHT);
        bridgeManager.leave(WEIGHT);
        endScenario();
    }
};


class TruckThread : public ObservableThread
{
public:
    explicit TruckThread(BridgeManager& bridgeManager, std::string id = "") :
        ObservableThread(std::move(id)), bridgeManager(bridgeManager)
    {
        scenarioGraph = std::make_unique<ScenarioGraph>();
        auto scenario = scenarioGraph->createNode(this, -1);
        auto p1 = scenarioGraph->createNode(this, 1);
        auto p2 = scenarioGraph->createNode(this, 2);
        auto p3 = scenarioGraph->createNode(this, 3);
        auto p4 = scenarioGraph->createNode(this, 4);
        auto p5 = scenarioGraph->createNode(this, 5);
        auto p6 = scenarioGraph->createNode(this, 6);
        scenario->next.push_back(p1);
        p1->next.push_back(p2);
        p1->next.push_back(p3);
        p2->next.push_back(p2);
        p2->next.push_back(p3);
        p3->next.push_back(p4);
        p4->next.push_back(p5);
        p4->next.push_back(p6);
        p5->next.push_back(p6);
        scenarioGraph->setInitialNode(scenario);
    }

private:
    BridgeManager& bridgeManager;
    float WEIGHT = 10.0f;

    void run() override
    {
        bridgeManager.access(WEIGHT);
        bridgeManager.leave(WEIGHT);
        endScenario();
    }
};

class ModelExercice13: public PcoModel
{
private:
    BridgeManager bridgeManager;

public:

    ModelExercice13() : bridgeManager(30.0f) {}

    bool checkInvariants() override {
        return true;
    }

    void build() override {
        threads.emplace_back(std::make_unique<CarThread>(bridgeManager, "Car1"));
//        threads.emplace_back(std::make_unique<CarThread>(bridgeManager, "Car2"));
        threads.emplace_back(std::make_unique<TruckThread>(bridgeManager, "Truck1"));
//        threads.emplace_back(std::make_unique<TruckThread>(bridgeManager, "Truck2"));

        scenarioBuilder = std::make_unique<ScenarioBuilderBuffer>();
        scenarioBuilder->init(threads, 12);
    }

    void preRun(Scenario &/*scenario*/) override {

    }

    void postRun(Scenario &scenario) override {
//        std::cout << "---------------------------------------" << std::endl;
//        std::cout << "Scenario : ";
//        ScenarioPrint::printScenario(scenario);
    }

};

#endif // MODELEXERCICE13_H
