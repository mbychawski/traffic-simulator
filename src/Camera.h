/**
 * Camera.h
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_

class QGraphicsEllipseItem;
class Scene;
struct Position;

#include <QObject>
#include <QTime>
#include <QBrush>
#include <QPointF>
#include <QPen>
#include <QString>
#include <QList>
#include <QRectF>

/**
 * @brief
 * Klasa reprezentująca pojedyńczą kamerę. Odpowiada za jej rysowanie i wykrywanie obiektów.
 */
class Camera : public QObject {
    Q_OBJECT

    static int counter;

    int id;
    double x, y;
    double range;
    int angle;
    int viewAngle;

    QTime lastCheck;
    QBrush brush;
    QPen pen;
    QPointF textPos;

    QGraphicsEllipseItem* cameraGraphicsItem;
    Scene* scene;

public:
    /// Konstruktor
    Camera(qreal, qreal, qreal, qreal, int, Scene*);

    /// Tworzy odpowiadający kamerze QGraphicsItem i umiezcza go na scenie
    void draw(Scene*);
    /// Wykrywa pojazdy i emituje sygnał połączony z głównym oknem aplikacji
    void check();

public Q_SLOTS:
    /// slot na który przychodzą powiadomienia o zminionych obszarach na QGraphicsView
    void sceneChanged(const QList<QRectF>&);

Q_SIGNALS:
    /// Sygnał emitowany do okna głównego
    void cameraEvent(QString);

private:
    QPointF addNoiseToPos(Position);
    QString numToStr(double);
    QString numToStr(int);

};

#endif /* CAMERA_H_ */
