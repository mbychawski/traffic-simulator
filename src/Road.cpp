#include "Road.h"

#include "Scene.h"
#include "Constants.h"
#include "Exceptions.h"

#include <QGraphicsItemGroup>
#include <QBrush>
#include <QPen>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>

Road::Road(PMapElement from_, PMapElement to_) : MapElement(0, 0, -1), from(from_), to(to_){
    Position fromPosition = from->getPosition();
    Position toPosition = to->getPosition();

    if(fromPosition.x == toPosition.x){
        if( (fromPosition.y + CROSS_SIZE) < toPosition.y ) {
            direction = S;
            position = Position(fromPosition.x, fromPosition.y + CROSS_SIZE);
            length = toPosition.y - position.y;
        }
        else if( fromPosition.y > (toPosition.y + CROSS_SIZE) ) {
            direction = N;
            position = Position(fromPosition.x, toPosition.y + CROSS_SIZE);
            length = fromPosition.y - position.y;
        }
        else throw MapException("Crosses to close!");
    }
    else if(fromPosition.y == toPosition.y){
        if( (fromPosition.x + CROSS_SIZE) < toPosition.x ) {
            direction = E;
            position = Position(fromPosition.x + CROSS_SIZE, fromPosition.y);
            length = toPosition.x - position.x;
        }
        else if( fromPosition.x > (toPosition.x + CROSS_SIZE) ) {
            direction = W;
            position = Position(toPosition.x + CROSS_SIZE, toPosition.y);
            length = fromPosition.x - position.x;
        }
        else throw MapException("Crosses to close!");

    }
    else throw MapException("Crosses must be in the same row or column!");
}


void Road::draw(Scene *scene){

    int width, height;

    QBrush asphaltBrush = scene->asphaltBrush;
    QBrush lanesBrush = scene->lanesBrush;
    QBrush pavementBrush = scene->pavementBrush;
    QPen noPen = scene->noPen;

    QGraphicsItemGroup* roadGroup = new QGraphicsItemGroup();
    QGraphicsRectItem * tmpRect = 0L;

    if(direction == N || direction == S){
        width = CROSS_SIZE;
        height = length;

        tmpRect = new QGraphicsRectItem(position.absX() + PAVEMENT_WIDTH ,position.absY() - PAVEMENT_WIDTH, CROSS_SIZE * ELEMENTAL_WIDTH - 2 * PAVEMENT_WIDTH, height * ELEMENTAL_WIDTH + 2 * PAVEMENT_WIDTH, roadGroup);
        tmpRect->setBrush(asphaltBrush);
        tmpRect->setPen(noPen);
        roadGroup->addToGroup(tmpRect);

        tmpRect = new QGraphicsRectItem(position.absX() ,position.absY(), PAVEMENT_WIDTH, height * ELEMENTAL_WIDTH, roadGroup);
        tmpRect->setBrush(pavementBrush);
        tmpRect->setPen(noPen);
        roadGroup->addToGroup(tmpRect);

        tmpRect = new QGraphicsRectItem(position.absX() + CROSS_SIZE * ELEMENTAL_WIDTH - PAVEMENT_WIDTH, position.absY(), PAVEMENT_WIDTH, height * ELEMENTAL_WIDTH, roadGroup);
        tmpRect->setBrush(pavementBrush);
        tmpRect->setPen(noPen);
        roadGroup->addToGroup(tmpRect);

        QGraphicsLineItem * tmpLine = new QGraphicsLineItem(position.absX() + (CROSS_SIZE/2)*ELEMENTAL_WIDTH, position.absY() - PAVEMENT_WIDTH, position.absX() + (CROSS_SIZE/2)*ELEMENTAL_WIDTH, position.absY() + height * ELEMENTAL_WIDTH + PAVEMENT_WIDTH, roadGroup);
        tmpLine->setPen(QPen(lanesBrush,5));
        roadGroup->addToGroup(tmpLine);
    }
    else {
        width = length;
        height = CROSS_SIZE;

        tmpRect =  new QGraphicsRectItem(position.absX() - PAVEMENT_WIDTH ,position.absY() + PAVEMENT_WIDTH, width * ELEMENTAL_WIDTH + 2 * PAVEMENT_WIDTH, height * ELEMENTAL_WIDTH - 2 * PAVEMENT_WIDTH, roadGroup);
        tmpRect->setBrush(asphaltBrush);
        tmpRect->setPen(noPen);
        roadGroup->addToGroup(tmpRect);

        tmpRect = new QGraphicsRectItem(position.absX() ,position.absY(), width * ELEMENTAL_WIDTH, PAVEMENT_WIDTH, roadGroup);
        tmpRect->setBrush(pavementBrush);
        tmpRect->setPen(noPen);
        roadGroup->addToGroup(tmpRect);

        tmpRect =  new QGraphicsRectItem(position.absX() ,position.absY() + CROSS_SIZE * ELEMENTAL_WIDTH - PAVEMENT_WIDTH, width * ELEMENTAL_WIDTH, PAVEMENT_WIDTH, roadGroup);
        tmpRect->setBrush(pavementBrush);
        tmpRect->setPen(noPen);
        roadGroup->addToGroup(tmpRect);

        QGraphicsLineItem * tmpLine = new QGraphicsLineItem(position.absX() - PAVEMENT_WIDTH ,position.absY() + (CROSS_SIZE/2) * ELEMENTAL_WIDTH, position.absX() + width * ELEMENTAL_WIDTH + PAVEMENT_WIDTH, position.absY() + (CROSS_SIZE/2) * ELEMENTAL_WIDTH, roadGroup);
        tmpLine->setPen(QPen(lanesBrush,5));
        roadGroup->addToGroup(tmpLine);
    }

    scene->addItem(roadGroup);
}


