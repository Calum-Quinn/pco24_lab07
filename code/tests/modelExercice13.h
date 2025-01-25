#ifndef MODELEXERCICE13_H
#define MODELEXERCICE13_H

#include <iostream>
#include <random>

#include "pcomodel.h"
#include "scenariobuilder.h"

// The shared variable
static int number = 0;

int getNumber()
{
    return number;
}

class CarThread : public ObservableThread
{
public:
    explicit CarThread(std::string id = "") :
        ObservableThread(std::move(id))
    {
        scenarioGraph = std::make_unique<ScenarioGraph>();
        auto scenario = scenarioGraph->createNode(this, -1);
        auto p1 = scenarioGraph->createNode(this, 1);
        auto p2 = scenarioGraph->createNode(this, 2);
        auto p3 = scenarioGraph->createNode(this, 3);
        scenario->next.push_back(p1);
        p1->next.push_back(p2);
        p2->next.push_back(p3);
        scenarioGraph->setInitialNode(scenario);
    }

private:
    void run() override
    {
        endScenario();
    }
};


class TruckThread : public ObservableThread
{
public:
    explicit TruckThread(std::string id = "") :
        ObservableThread(std::move(id))
    {
        scenarioGraph = std::make_unique<ScenarioGraph>();
        auto scenario = scenarioGraph->createNode(this, -1);
        auto p1 = scenarioGraph->createNode(this, 4);
        auto p2 = scenarioGraph->createNode(this, 5);
        auto p3 = scenarioGraph->createNode(this, 6);
        scenario->next.push_back(p1);
        p1->next.push_back(p2);
        p2->next.push_back(p3);
        scenarioGraph->setInitialNode(scenario);
    }

private:
    void run() override
    {
        endScenario();
    }
};

class ModelExercice13: public PcoModel
{
public:

    bool checkInvariants() override {
        return true;
    }

    void build() override {
        threads.emplace_back(std::make_unique<CarThread>("Car"));
        threads.emplace_back(std::make_unique<TruckThread>("Truck"));

        scenarioBuilder = std::make_unique<ScenarioBuilderBuffer>();
        scenarioBuilder->init(threads, 9);
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

#endif // MODELEXERCICE13_H
