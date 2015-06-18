#include "AnimationWaitingThread.h"

#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>
#include <QTime>

AnimationWaitingThread::AnimationWaitingThread(functionPtr f_, QWaitCondition * waitCondition_, QMutex * mutex_)
    : f(f_), waitCondition(waitCondition_), mutex(mutex_) {
    go = true;
}

void AnimationWaitingThread::run() {
    QMutexLocker locker(mutex);
    while(go){
            f();
            waitCondition->wait(mutex);
    }
}

void AnimationWaitingThread::stopThread() {
    go = false;
}
