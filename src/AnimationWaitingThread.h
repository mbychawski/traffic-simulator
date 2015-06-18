#ifndef ANIMATIONWAITINGTHREAD_H
#define ANIMATIONWAITINGTHREAD_H

class QWaitCondition;
class QMutex;

#include "Types.h"

#include <QThread>

/**
 * @brief
 * Klasa wątków obliczeniowych (dla Pojazdów i Pieszych). Po każdym cyklu run czeka na wybudzenie po odświerzeniu widoku
 */
class AnimationWaitingThread : public QThread
{
    functionPtr f;
    QWaitCondition* waitCondition;
    QMutex* mutex;

    volatile bool go;

public:
    /// Konstruktor
    AnimationWaitingThread(functionPtr, QWaitCondition*, QMutex*);

    /// @overload
    void run();

    /// Zatrzymuje dzialanie wątku poprzez wyjście z pętli w metodzie run
    void stopThread();
};

#endif // ANIMATIONWAITINGTHREAD_H
