#include "Simulator.h"

#include "Scene.h"
#include "VehicleCreator.h"
#include "Vehicle.h"
#include "Pedestrian.h"
#include "Spawn.h"
#include "Constants.h"
#include "Animation.h"
#include "SpawningVehiclesRunnable.h"
#include "SpawningPedestriansRunnable.h"
#include "Exceptions.h"

#include <QMutex>
#include <QThreadPool>

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

Simulator::Simulator(Scene* scene_, QObject* parent_) : QObject(parent_), scene(scene_) {
    string mapJSON = "settings/map.json";
    string vehiclesJSON = "settings/vehicles.json";

    scene->init(mapJSON);
    VehicleCreator vehicleCreator(scene, vehiclesJSON);

    animation = new Animation(scene, &vMutex, &pMutex);

    //pobranie elementów mapy ze sceny do lokalnego vetora i usinięcie z niego wszystkiego co nie jest Spawnem posiadającym pojazdy do startu
    vector<PMapElement> spawns_ = scene->getMapElements();
    spawns_.erase(std::remove_if(spawns_.begin(), spawns_.end(), Spawn::isNotSpawn                ), spawns_.end() );
    spawns_.erase(std::remove_if(spawns_.begin(), spawns_.end(), Spawn::spawnHasVehiclesToStart), spawns_.end() );

    vRunnable = new SpawningVehiclesRunnable(spawns_, this);
    vRunnable->setAutoDelete(false);
    pRunnable = new SpawningPedestriansRunnable(scene  , this);
    pRunnable->setAutoDelete(false);
    qRegisterMetaType<PVehicle>("PVehicle");
    qRegisterMetaType<PPedestrian>("PPedestrian");

    connect(this, SIGNAL( addVehicleToAnimate(PVehicle, Animation*, QMutex*)     ), scene, SLOT( addVehicleToAnimationSlot   (PVehicle, Animation*, QMutex *) ), Qt::QueuedConnection);
    connect(this, SIGNAL( addPedestrianToAnimate(PPedestrian,Animation*,QMutex*) ), scene, SLOT( addPedestrianToAnimationSlot(PPedestrian,Animation*,QMutex*) ), Qt::QueuedConnection);
}

Simulator::~Simulator() {
    delete animation;
    delete pRunnable;
    delete vRunnable;
}

void Simulator::startSimulation() {
    QThreadPool::globalInstance()->start( vRunnable );
    QThreadPool::globalInstance()->start( pRunnable );
    animation->startAnimation();
}

void Simulator::stopSimulation() {
    if(vRunnable)
        vRunnable->stopThread();
    if(pRunnable)
        pRunnable->stopThread();
    QThreadPool::globalInstance()->waitForDone();

    animation->stopAnimation();
}

void Simulator::addVehicle(PVehicle v_) {
    Q_EMIT addVehicleToAnimate(v_, animation, &vMutex);
}

void Simulator::addPedestrian(PPedestrian p_) {
    Q_EMIT addPedestrianToAnimate(p_, animation, &pMutex);
}
