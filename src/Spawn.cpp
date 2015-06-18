#include "Spawn.h"
#include "Scene.h"
#include "Direction.h"
#include "Exceptions.h"
#include "Vehicle.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>
#include <QMutex>
#include <QDataStream>

Q_DECLARE_METATYPE(MapElement*)

Spawn::Spawn(int id_, Position position_, Position neighborPos_)
    : MapElement(position_, id_) {
    calculateDirection(neighborPos_);
}

Spawn::Spawn(int id_, double x_, double y_, Position neighborPos_)
    : MapElement(x_, y_, id_) {
    calculateDirection(neighborPos_);
}

Spawn::~Spawn() {
    //vehiclesToStart.clear();
}

void Spawn::draw(Scene *scene) {
    QBrush asphaltBrush = scene->asphaltBrush;
    QBrush lanesBrush = scene->lanesBrush;
    QBrush pavementBrush = scene->pavementBrush;
    QPen noPen = scene->noPen;

    QGraphicsItemGroup* spawnGroup = new QGraphicsItemGroup();
    QGraphicsRectItem * tmpRect = 0L;


    if(direction == N || direction == S){

        tmpRect = new QGraphicsRectItem(position.absX() + PAVEMENT_WIDTH ,position.absY(), CROSS_SIZE * ELEMENTAL_WIDTH - 2 * PAVEMENT_WIDTH, SPAWN_LENGTH, spawnGroup);
        tmpRect->setBrush(asphaltBrush);
        tmpRect->setPen(noPen);
        spawnGroup->addToGroup(tmpRect);

        tmpRect = new QGraphicsRectItem(position.absX() ,position.absY(), PAVEMENT_WIDTH,  SPAWN_LENGTH, spawnGroup);
        tmpRect->setBrush(pavementBrush);
        tmpRect->setPen(noPen);
        spawnGroup->addToGroup(tmpRect);

        tmpRect = new QGraphicsRectItem(position.absX() + CROSS_SIZE * ELEMENTAL_WIDTH - PAVEMENT_WIDTH, position.absY(), PAVEMENT_WIDTH, SPAWN_LENGTH, spawnGroup);
        tmpRect->setBrush(pavementBrush);
        tmpRect->setPen(noPen);
        spawnGroup->addToGroup(tmpRect);

        QGraphicsLineItem * tmpLine = new QGraphicsLineItem(position.absX() + (CROSS_SIZE/2)*ELEMENTAL_WIDTH,position.absY(), position.absX() + (CROSS_SIZE/2)*ELEMENTAL_WIDTH, position.absY() + SPAWN_LENGTH, spawnGroup);
        tmpLine->setPen(QPen(lanesBrush,5));
        spawnGroup->addToGroup(tmpLine);

    }
    else {
        tmpRect = new QGraphicsRectItem(position.absX() ,position.absY() + PAVEMENT_WIDTH, SPAWN_LENGTH, CROSS_SIZE * ELEMENTAL_WIDTH - 2 * PAVEMENT_WIDTH, spawnGroup);
        tmpRect->setBrush(asphaltBrush);
        tmpRect->setPen(noPen);
        spawnGroup->addToGroup(tmpRect);

        tmpRect = new QGraphicsRectItem(position.absX() ,position.absY(), SPAWN_LENGTH, PAVEMENT_WIDTH, spawnGroup);
        tmpRect->setBrush(pavementBrush);
        tmpRect->setPen(noPen);
        spawnGroup->addToGroup(tmpRect);

        tmpRect = new QGraphicsRectItem(position.absX() ,position.absY() + CROSS_SIZE*ELEMENTAL_WIDTH - PAVEMENT_WIDTH, SPAWN_LENGTH, PAVEMENT_WIDTH, spawnGroup);
        tmpRect->setBrush(pavementBrush);
        tmpRect->setPen(noPen);
        spawnGroup->addToGroup(tmpRect);

        QGraphicsLineItem * tmpLine = new QGraphicsLineItem(position.absX() ,position.absY() + (CROSS_SIZE/2)*ELEMENTAL_WIDTH, position.absX() + SPAWN_LENGTH, position.absY() + (CROSS_SIZE/2)*ELEMENTAL_WIDTH, spawnGroup);
        tmpLine->setPen(QPen(lanesBrush,5));
        spawnGroup->addToGroup(tmpLine);
    }

    spawnGroup->setData(1, qVariantFromValue( static_cast<MapElement*>(this) ) );
    scene->addItem(spawnGroup);
}

PVehicle Spawn::startVehicle() {
    PVehicle v = vehiclesToStart.back();

    int lane = vehiclesToStart.size() % 2; // wybieramy pas z którego będzie startował Vehicle
    v->setDirection(direction);
    v->setLane(lane);
    v->setPosition(calculateStartPosition(lane));

    vehiclesToStart.pop_back();

    return v;
}

bool Spawn::hasVehiclesToStart() {
    if(!vehiclesToStart.empty())
        return true;
    else
        return false;
}

Position Spawn::calculateStartPosition(int lane_) {
    double x = position.absX(), y = position.absY();
    // lane_ = 0 to pas wewnetrzny
    switch(direction) {
        case N:
            x += ELEMENTAL_WIDTH * CROSS_SIZE / 2 + LANE_WIDTH / 2 + (1 - VEHICLE_WIDTH) * VEHICLE_LANE_WIDTH / 2 + lane_ * VEHICLE_LANE_WIDTH;
            break;
        case S:
            x += ELEMENTAL_WIDTH * CROSS_SIZE / 2 - LANE_WIDTH / 2 - (1 - VEHICLE_WIDTH) * VEHICLE_LANE_WIDTH / 2 - (1 + lane_) * VEHICLE_LANE_WIDTH;
            break;
        case E:
            y += ELEMENTAL_WIDTH * CROSS_SIZE / 2 + LANE_WIDTH / 2 + (1 - VEHICLE_WIDTH) * VEHICLE_LANE_WIDTH / 2 + lane_ * VEHICLE_LANE_WIDTH;
            break;
        case W:
            y += ELEMENTAL_WIDTH * CROSS_SIZE / 2 - LANE_WIDTH / 2 + (1 - VEHICLE_WIDTH) * VEHICLE_LANE_WIDTH / 2 - (1 + lane_) * VEHICLE_LANE_WIDTH;
            break;
    }
    return Position(x, y);
}

void Spawn::addVehicle(PVehicle v_) {
    vehiclesToStart.push_back(v_);
}

/*
 * Prywatne.
 */
void Spawn::calculateDirection(Position neighborPos_) {
    if(neighborPos_.x == position.x) {
        if(neighborPos_.y < position.y) direction = N;
        else direction = S;
    }
    else if(neighborPos_.y == position.y) {
        if(neighborPos_.x < position.x) direction = W;
        else direction = E;
    }
    else throw MapException("Spawn doesn't fit to cross!");
}
