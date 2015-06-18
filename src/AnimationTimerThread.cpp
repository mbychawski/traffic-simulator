#include "AnimationTimerThread.h"
#include "Scene.h"
#include "Animation.h"

#include <QWaitCondition>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>

AnimationTimerThread::AnimationTimerThread(QWaitCondition* waitCondition_, double interval_, Scene* scene_, Animation* animation_, QMutex* vMutex_, QMutex * pMutex_):
    waitCondition(waitCondition_), interval(interval_), scene(scene_), animation(animation_), vMutex(vMutex_), pMutex(pMutex_)
{ }

void AnimationTimerThread::run(){
    QTimer timer;

    connect(&timer, SIGNAL( timeout() ), this, SLOT( updateView() ),Qt::QueuedConnection );

    timer.setInterval(interval);
    timer.start();

    exec();
    timer.stop();
}

void AnimationTimerThread::updateView() {
    if (vMutex->tryLock()) {
        animation->updateVehiclesOnScene();
        vMutex->unlock();
    }

    if(pMutex->tryLock()){
        animation->updatePedestriansOnScene();
        pMutex->unlock();
    }

    scene->updateViewPort();
    waitCondition->wakeAll();
}

