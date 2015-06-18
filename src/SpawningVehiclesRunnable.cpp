#include "SpawningVehiclesRunnable.h"
#include "Simulator.h"
#include "Spawn.h"

#include <QThread>
#include <algorithm>

SpawningVehiclesRunnable::SpawningVehiclesRunnable(std::vector<PMapElement> spawns_, Simulator* simulator_)
    : spawns(spawns_), simulator(simulator_), go(true) {}

void SpawningVehiclesRunnable::run() {
    while(spawns.size() > 0 && go == true) {
        for(std::vector<PMapElement>::iterator it = spawns.begin(); it != spawns.end(); ++it) {
            PVehicle v = (dynamic_cast<Spawn*>( it->get() ))->startVehicle();

            simulator->addVehicle(v);

            QThread::sleep(VEHICLE_SPAWN_INTERVAL);
        }

        spawns.erase(std::remove_if(spawns.begin(), spawns.end(), Spawn::spawnHasVehiclesToStart), spawns.end());
    }
}

void SpawningVehiclesRunnable::stopThread() {
    go = false;
}
