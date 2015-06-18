/**
 * @brief The Vehicle class
 *
 */

#ifndef VEHICLE_H_
#define VEHICLE_H_

class MapElement;
class Cross;
class Spawn;

#include "Moveable.h"
#include "Drawable.h"
#include "VehicleState.h"

#include <QColor>
#include <QGraphicsRectItem>
#include <QWaitCondition>

#include <vector>
#include <string>

using std::vector;
using std::string;

/**
 * @brief
 * Klasa odpowiadająca za pojazdy, ich poruszanie się po planszy, komunikację ze skrzyżowaniami, Spawnami, trasę przejazdu.
 */
class Vehicle : public Moveable, public Drawable {
    QGraphicsRectItem* qItem;
    QGraphicsRectItem* qCollisionItem;

    double length;

    int id;
    int startID;
    QColor color;
    float reflex;
    int destinationID;
    VehicleState state;
    Cross* nextCross;
    Cross* lastCross;
    bool onCross;

    vector<int> path;
    string drivingMode;
    int lane;

    double dl;

    vector<MapElement*> collidesMapElements;
    bool collidedVehicles;

    QPointF transformOriginPointToSet;
    double rotationToSet;

    /// Oblicza drogę hamowania.
    double calculateBraking() const;
    /// Ustawia niewidoczny prostokąt służący do detekcji kolizji.
    void setCollisionItem();
    /// Detekuje kolizje i zapewnia ich obsługę.
    void checkCollisions();
    /// Detekuje kolizje z innymi pojazdami i zapewnia ich obsługę.
    void checkCollisionsVehicles();
    /// Ustawia kolejny stan pojazdu.
    void calculateState();
    /// Sprawdzanie czy pojazd jest już za skrzyżowaniem.
    bool checkOutOfCross();
    /// Sprawdzanie czy pojazd wjechał na skrzyżowanie.
    bool checkOnCross();
    /// Sprawdzanie czy pojazd przejechał przez Spawn.
    bool checkOnSpawn(Spawn* s);
    /// Ustawianie odpowiedniej pozycji po wyjściu z zakrętu.
    void positionAfterTurn();
    /// Instrukcje wykonywane po wyjechaniu ze skrzyżowania.
    void getOffCross();


    void moveForward();
    void turnLeft();
    void turnRight();

public:
    /// Konstruktor
    Vehicle(int, string, int, string, float, float, float, int destinationID_ = -1);

    virtual void draw(Scene*);
    /// Obliczanie kolejnej pozycji pojazdu
    virtual void calculatePosition();
    /// Inicjowanie pojazdu, ustawianie pozycji i kierunku początkowego
    void init(Scene*);
    /// Odświeża obiekt qItem na scenie
    void updateQItemOnScene();
    /// Sprawdzanie kolizji i zapisywanie ich do wektora
    void checkCollisions(Scene*);

    /// Sprawdzanie czy obiekt jest aktywny, czy też powinniśmy go usunąć.
    bool isNotActive() const {
        return !qItem->isVisible();
    }

    /// Pobieranie długości pojazdu.
    double getLength() const {
        return length;
    }

    /// Ustawianie pasa, po którym będzie się przemieszczał pojazd.
    void setLane(int lane_) {
        lane = lane_;
    }

    /// Ustawianie drogi, po której pojazd będzie się poruszał.
    void setPath(vector<int>);
    
    virtual ~Vehicle();
};

#endif /* VEHICLE_H_ */



