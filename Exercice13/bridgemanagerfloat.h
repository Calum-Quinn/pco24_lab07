#include <pcosynchro/pcosemaphore.h>

class BridgeManagerFloat
{
private:
    PcoSemaphore accesData;
    PcoSemaphore waitAcces;

    float maxWeight;
    float usedWeight;
    unsigned nbWaitter;

public:
    BridgeManagerFloat(float maxWeight) : nbWaitter(0), waitAcces(0), accesData(1), maxWeight(maxWeight), usedWeight(0.0f) {}

    ~BridgeManagerFloat() {}

    void access(float weight)
    {
        accesData.acquire();

        while ((usedWeight + weight) > maxWeight) {
            ++nbWaitter;
            accesData.release(); // Relâche le verrou pour permettre aux autres de libérer le poids
            waitAcces.acquire(); // Attend la libération du poids
            accesData.acquire(); // Réacquiert le verrou pour vérifier les conditions
            --nbWaitter;
        }
        usedWeight += weight;

        accesData.release();
    }

    void leave(float weight)
    {
        accesData.acquire();

        usedWeight -= weight;

        if (nbWaitter > 0) {
            waitAcces.release(); // Libère un seul thread en attente
        }

        accesData.release();
    }
};
