#include "SpawningPedestriansRunnable.h"
#include "Simulator.h"
#include "Constants.h"
#include "Pedestrian.h"

#include <QThread>

SpawningPedestriansRunnable::SpawningPedestriansRunnable(Scene* scene_, Simulator* simulator_)
    : scene(scene_), simulator(simulator_), go(true) {}

void SpawningPedestriansRunnable::run() {
    for(int i = 0; i < static_cast<int>(NUMBER_OF_PEDESTRIANS/4) && go == true; i++){
        for(int j = 0; j < 4; j++) { // startujemy 4 pieszych na raz
            PPedestrian p(new Pedestrian());
            simulator->addPedestrian(p);
        }

        QThread::sleep(3 * VEHICLE_SPAWN_INTERVAL);
    }
}

void SpawningPedestriansRunnable::stopThread() {
    go = false;
}
