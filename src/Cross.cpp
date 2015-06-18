#include "Cross.h"

#include "Scene.h"
#include "PriorityRules.h"
#include "CrossRules.h"
#include "Exceptions.h"
#include "Position.h"

#include <QBrush>
#include <QPen>
#include <QTime>
#include <QGraphicsItemGroup>
#include <QGraphicsEllipseItem>
#include <QString>
#include <boost/bind.hpp>

#include <algorithm>

#include <set>
#include <algorithm>
using std::set;

Q_DECLARE_METATYPE(MapElement*)

void Cross::draw(Scene *scene){
    QBrush asphaltBrush = scene->asphaltBrush;
    QBrush lanesBrush = scene->lanesBrush;
    QBrush pavementBrush = scene->pavementBrush;
    QBrush priorityBrush = QBrush(QColor(QString::fromStdString(PRIORITY_COLOR)));
    QPen noPen = scene->noPen;

    crossGroup = new QGraphicsItemGroup();
    QGraphicsRectItem* tmpRect = 0L;

    tmpRect = new QGraphicsRectItem(position.absX(), position.absY(), CROSS_SIZE * ELEMENTAL_WIDTH, CROSS_SIZE * ELEMENTAL_WIDTH, crossGroup);
    tmpRect->setBrush(pavementBrush);
    tmpRect->setPen(noPen);
    crossGroup->addToGroup(tmpRect);

    tmpRect = new QGraphicsRectItem(position.absX() + PAVEMENT_WIDTH, position.absY() + PAVEMENT_WIDTH, CROSS_SIZE * ELEMENTAL_WIDTH - 2 * PAVEMENT_WIDTH, CROSS_SIZE * ELEMENTAL_WIDTH - 2 * PAVEMENT_WIDTH , crossGroup);
    tmpRect->setBrush(asphaltBrush);
    tmpRect->setPen(noPen);
    crossGroup->addToGroup(tmpRect);

    if(CrossRules* cr = dynamic_cast<CrossRules*>(priorityRules)) {
        vector<QGraphicsEllipseItem*> dots = drawDot(cr);
        std::for_each(dots.begin(), dots.end(), boost::bind(&QGraphicsItemGroup::addToGroup, boost::ref(crossGroup), _1));
        //std::for_each(dots.begin(), dots.end(), boost::bind(&QGraphicsScene::addItem, boost::ref(scene), _1));
    }

    crossGroup->setData(1, qVariantFromValue( static_cast<MapElement*>(this) ));

    scene->addItem(crossGroup);
}

vector<QGraphicsEllipseItem*> Cross::drawDot(CrossRules* cr) {
    vector<QGraphicsEllipseItem*> result;
    for(map<Direction, int>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
        if(cr->isSubordinate((*it).first)) {
            QBrush priorityBrush = QBrush(QColor(QString::fromStdString(PRIORITY_COLOR)));
            QPen noPen = QPen(Qt::NoPen);

            QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem();
            ellipse->setBrush(priorityBrush);
            ellipse->setPen(noPen);
            Position p(0, 0);
            if((*it).first == N) {
                p.x = position.absX() + PAVEMENT_WIDTH + ROADWAY_WIDTH / 4 - PEDESTRIAN_RADIUS / 2;
                p.y = position.absY() + PAVEMENT_WIDTH;
            }
            else if((*it).first == E) {
                p.x = position.absX() + CROSS_SIZE * ELEMENTAL_WIDTH - 3 * PAVEMENT_WIDTH / 2;
                p.y = position.absY() + PAVEMENT_WIDTH + ROADWAY_WIDTH / 4 - PEDESTRIAN_RADIUS / 2;
            }
            else if((*it).first == S) {
                p.x = position.absX() + CROSS_SIZE * ELEMENTAL_WIDTH - PAVEMENT_WIDTH - ROADWAY_WIDTH / 4 - PEDESTRIAN_RADIUS / 2;
                p.y = position.absY() + CROSS_SIZE * ELEMENTAL_WIDTH - PAVEMENT_WIDTH - PEDESTRIAN_RADIUS;
            }
            else if((*it).first == W) {
                p.x = position.absX() + PAVEMENT_WIDTH;
                p.y = position.absY() + CROSS_SIZE * ELEMENTAL_WIDTH -  PAVEMENT_WIDTH - ROADWAY_WIDTH / 4 - PEDESTRIAN_RADIUS / 2;
            }

            ellipse->setRect(p.x, p.y, PEDESTRIAN_RADIUS, PEDESTRIAN_RADIUS);
            result.push_back(ellipse);
        }
    }

    return result;
}

void Cross::registerVehicle(Direction direction_, Vehicle* v_) {
    waitingVehicles[direction_].push_back(v_);
}

void Cross::unregisterVehicle(Direction direction_) {
    waitingVehicles[direction_].erase(waitingVehicles[direction_].begin());
}

bool Cross::canGo(Direction from_, Direction to_) {
    bool result = true;
    from_ = static_cast<Direction>((from_ + 2) % 4);
    set<Direction> priority = priorityRules->getRules(from_, to_);
    for(set<Direction>::const_iterator it = priority.begin(); it != priority.end(); ++it) {
        if(waitingVehicles[(*it)].size() != 0)
            result = false;
    }

    return result;
}

Direction Cross::getNeighborDirection(int id_) {
    const map<Direction, int>::const_iterator it = std::find_if(neighbors.begin(), neighbors.end(), boost::bind(&map<Direction, int>::value_type::second, _1) == id_);
    if(it != neighbors.end())
        return (*it).first;
    else
        throw MapException("No such neighbor ID!");
}

Direction Cross::getRandomDirection(int from_) {
    vector<Direction> directions;
    for(map<Direction, int>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
        if((*it).second != -1 && (*it).first != from_)
            directions.push_back((*it).first);

    int rand;
    if(directions.size() > 0)
        rand = qrand() % directions.size();
    else
        throw MapException("Wrong cross neighbors!");

    return directions.at(rand);
}

Direction Cross::getPedestrianRandomDirection() {
    return getRandomDirection(-1);
}

Position Cross::calculatePedestrianPosition(Direction direction_, int side_) {
    Position p(position.absX(), position.absY());

    double sideDelta = PAVEMENT_WIDTH / 2 + side_ * (ROADWAY_WIDTH + PAVEMENT_WIDTH);
    double delta = PEDESTRIAN_OFFSET / 4 + PEDESTRIAN_RADIUS;

    if(direction_ == N) {
        p.x += sideDelta;
        p.y += delta;
    }
    else if(direction_ == E) {
        p.x += CROSS_SIZE * ELEMENTAL_WIDTH - delta;
        p.y += sideDelta;
    }
    else if(direction_ == S) {
        p.x += CROSS_SIZE * ELEMENTAL_WIDTH - sideDelta;
        p.y += CROSS_SIZE * ELEMENTAL_WIDTH - delta;
    }
    else if(direction_ == W) {
        p.x += delta;
        p.y += CROSS_SIZE * ELEMENTAL_WIDTH -sideDelta;
    }

    return p;
}

bool Cross::isCross(const PMapElement el) {
    if( dynamic_cast<Cross*>( el.get() ) == 0) return true;
    else return false;
}
