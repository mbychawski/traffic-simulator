#ifndef SPAWNINGPEDESTRIANSRUNNABLE_H
#define SPAWNINGPEDESTRIANSRUNNABLE_H

class Scene;
class Simulator;

#include <QtCore>
#include <QRunnable>

/**
 * @brief
 * Klasa obsługująca powstawanie pieszych na planszy.
 */
class SpawningPedestriansRunnable : public QRunnable {
    Scene* scene;
    Simulator* simulator;

    volatile bool go;

public:
    /// Konstruktor
    SpawningPedestriansRunnable(Scene*, Simulator*);
    /// @overload przeciążona metoda run wątku
    void run();
    /// Metoda służąca do zatrzymywania działania wątku
    void stopThread();
};

#endif // SPAWNINGPEDESTRIANSRUNNABLE_H
