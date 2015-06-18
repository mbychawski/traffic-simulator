#ifndef SPAWNINGVEHICLESRUNNABLE_H
#define SPAWNINGVEHICLESRUNNABLE_H

class Simulator;

#include "Types.h"

#include <QtCore>
#include <QRunnable>
#include <vector>

/**
 * @brief
 * Klasa obsługujaca powstawanie pojazdów na planszy w odpowiednich punktach Spawn
 */
class SpawningVehiclesRunnable : public QRunnable {
    std::vector<PMapElement> spawns;
    Simulator* simulator;
    volatile bool go;

public:
    /// Konstruktor.
    SpawningVehiclesRunnable(std::vector<PMapElement>, Simulator*);
    /// @overload przeciążona metoda run
    void run();
    /// Metoda służąca do zatrzymywania wątku.
    void stopThread();
};

#endif // SPAWNINGVEHICLESRUNNABLE_H
