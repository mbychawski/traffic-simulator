#ifndef ANIMATIONTIMERTHREAD_H
#define ANIMATIONTIMERTHREAD_H

class QWaitCondition;
class Scene;
class Animation;
class QMutex;

#include <QThread>

/**
 * @brief
 * Klasa wątku animacji. Uruchamiająca timer i wybudzająca w odpowiednich momentach wątki obliczające
 */
class AnimationTimerThread : public QThread {
    Q_OBJECT

    QWaitCondition* waitCondition;
    double interval;
    Scene* scene;
    Animation* animation;
    QMutex* vMutex;
    QMutex* pMutex;

public:
    /// Konstruktor
    AnimationTimerThread(QWaitCondition*, double, Scene*, Animation*, QMutex*, QMutex*);
    /// @overload
    void run();

public Q_SLOTS:
    /// wywołuje odpowiednie metody w Animation. Działa tylko w wątku GUI
    void updateView();
};

#endif // ANIMATIONTIMERTHREAD_H
