#include "Animation.h"

#include "Vehicle.h"
#include "Scene.h"
#include "Constants.h"
#include "Pedestrian.h"
#include "AnimationWaitingThread.h"
#include "AnimationTimerThread.h"

#include <QGraphicsView>
#include <QMutex>
#include <QMutexLocker>

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <algorithm>



Animation::Animation(Scene* scene_, QMutex* vMutex_, QMutex* pMutex_) : scene(scene_), vMutex(vMutex_), pMutex(pMutex_) {

    functionPtr vF = boost::bind(&Animation::vNextPosition,boost::ref(*this));
    functionPtr pF = boost::bind(&Animation::pNextPosition,boost::ref(*this));

    vThread = new AnimationWaitingThread(vF, &viewUpdated, vMutex);
    pThread = new AnimationWaitingThread(pF, &viewUpdated, pMutex);

    timerThread = new AnimationTimerThread(&viewUpdated, CONST_DT, scene, this, vMutex, pMutex);
}

Animation::~Animation() {
    while( vThread->isRunning() || pThread->isRunning() || timerThread->isRunning() ){
        viewUpdated.wakeAll();
    }

    delete vThread;
    delete pThread;
    delete timerThread;
}

void Animation::vNextPosition() {
    vehicles.erase(std::remove_if(vehicles.begin(), vehicles.end(), boost::bind(&Vehicle::isNotActive, boost::bind(&PVehicle::get, _1))), vehicles.end());
    for(vector<PVehicle>::iterator it = vehicles.begin(); it != vehicles.end(); ++it) {
       it->get()->calculatePosition();
    }
}

void Animation::pNextPosition() {
    pedestrians.erase(std::remove_if(pedestrians.begin(), pedestrians.end(), boost::bind(&Pedestrian::isNotActive, boost::bind(&PPedestrian::get, _1))), pedestrians.end());
    for(vector<PPedestrian>::iterator it = pedestrians.begin(); it != pedestrians.end(); ++it) {
        it->get()->calculatePosition();
    }

}

void Animation::startAnimation() {
    vThread->start();
    pThread->start();
    timerThread->start();
}

void Animation::stopAnimation() {
    if(vThread)
        vThread->stopThread();
    if(pThread)
        pThread->stopThread();

    timerThread->exit();
    viewUpdated.wakeAll();
}

void Animation::addVehicle(PVehicle v_) {
    vehicles.push_back(v_);
}

void Animation::addPedestrian(PPedestrian p_) {
    pedestrians.push_back(p_);
}

void Animation::updateVehiclesOnScene() {
    for(vector<PVehicle>::iterator it = vehicles.begin(); it != vehicles.end(); ++it) {
        it->get()->updateQItemOnScene();
        it->get()->checkCollisions(scene);
    }
}

void Animation::updatePedestriansOnScene() {
    for(vector<PPedestrian>::iterator it = pedestrians.begin(); it != pedestrians.end(); ++it) {
        it->get()->updateQItemOnScene();
        it->get()->checkCollisions(scene);
    }
}
