/**
 * CameraController.h
 * Klasa zarządzająca i przechowująca kamery. Obsługuje także zdarzenia myszy zachodzące w trakcie tworzenia kamery.
 */

#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

class Scene;

class QGraphicsSceneMouseEvent;
class QPen;
class QBrush;
class QMutex;
class QPointF;

#include <QObject>
#include <QPointF>
#include <QMutex>

#include "Types.h"

#include <vector>
using std::vector;

/**
 * @brief
 * Klasa odpowiadająca za tworzenie kamer
 */
class CameraController : public QObject {
    Q_OBJECT

    vector<PCamera> cameras;
    Scene* scene;
    QPointF startPoint;

    QMutex mutex;

public:
    /// Konstruktor
    CameraController(Scene* scene_, QObject* parent_ = 0);

public Q_SLOTS:
    /// Slot odbierający zdarzenie naciśnięcia myszy
    void mousePressed(QGraphicsSceneMouseEvent*);
    /// Slot odbierający zdarzenie zwolnienia myszy
    void mouseReleased(QGraphicsSceneMouseEvent*);

};

#endif
/* CAMERACONTROLLER_H_ */
