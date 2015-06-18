#ifndef ANIMATION_H
#define ANIMATION_H

class Scene;
class AnimationWaitingThread;
class AnimationTimerThread;

#include "Types.h"

#include <QWaitCondition>
#include <vector>

using std::vector;


/**
 * @brief
 * Klasa odpowiadająca za trzy podstawowe wątki animacji: aktualizacja położenia pojazdów i pieszych, oraz wątek timera.
 */
class Animation {

    Scene* scene;
    QMutex* vMutex;
    QMutex* pMutex;
    QWaitCondition viewUpdated;

    vector<PVehicle> vehicles;
    vector<PPedestrian> pedestrians;

    AnimationWaitingThread * vThread;
    AnimationWaitingThread * pThread;
    AnimationTimerThread* timerThread;

public:
    /// Konstruktor. Przyjmuje dwa mutexy: jeden dla synchronizacji pojazdów drugi pieszych
    Animation(Scene* scene_, QMutex* vMutex_, QMutex* pMutex_);
    ~Animation();

    /// Wywołuje dla każdego pojazdu obliczanie nowej pozycji oraz usuwa już nie aktywne pojazdy
    void vNextPosition();
    /// Metoda analogiczna do vNextPosition(), tylko że dla pieszych
    void pNextPosition();

    /// Uruchamia wątki animacji
    void startAnimation();
    /// Wylącza wszystkie wątki
    void stopAnimation();

    /// Dodaje pojazdy do kontenera pojazdów
    void addVehicle(PVehicle);
    /// Metoda analogiczna do addVehicle(PVehicle)
    void addPedestrian(PPedestrian);

    /// Wywołuje dla każdego pojazdu aktualizację pozycji na Scene oraz sprawdzenie kolizji
    void updateVehiclesOnScene();
    /// Metoda analogiczna do updateVehiclesOnScene()
    void updatePedestriansOnScene();
};

#endif // ANIMATION_H
