#ifndef SIMULATOR_H
#define SIMULATOR_H

class Scene;
class Animation;
class SpawningVehiclesRunnable;
class SpawningPedestriansRunnable;

#include "Types.h"

#include <QWidget>
#include <QtCore>
#include <QThread>

/**
 * @brief
 * Jedna z głównych klas aplikacji, odpowiada za zarządzanie wątkami oraz sterowanie animacją.
 */
class Simulator : public QObject {
    Q_OBJECT

    Scene* scene;
    SpawningVehiclesRunnable*    vRunnable;
    SpawningPedestriansRunnable* pRunnable;

    QMutex vMutex;
    QMutex pMutex;

 public:
    Animation* animation;

    /// Konstruktor
    Simulator(Scene*, QObject* = 0 );
    ~Simulator();

    /// Rozpoczynanie symulacji
    void startSimulation();
    /// Zatrzymywanie symulacji
    void stopSimulation();
    /// Dodawanie pojazdu do symulacji
    void addVehicle(PVehicle);
    /// Dodawanie pieszego do symulacji
    void addPedestrian(PPedestrian);

Q_SIGNALS:
    /// Sygnał potrzebny do dodawania pojazdu do animacji (ponieważ jest to oddzielny wątek)
    void addVehicleToAnimate(PVehicle, Animation*, QMutex*);
    /// Analogicznie jak addVehicleToAnimate, ale dla pieszego
    void addPedestrianToAnimate(PPedestrian, Animation*, QMutex*);
};

#endif // SIMULATOR_H
