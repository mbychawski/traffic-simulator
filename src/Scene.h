#ifndef SCENE_H_
#define SCENE_H_

class MainWindow;
class Animation;

#ifndef Q_MOC_RUN

#endif

#include "Types.h"
#include "Animation.h"
#include "ShortestPath.h"

#include "Cross.h"
#include "Road.h"
#include "Spawn.h"

#include <QtCore>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QWaitCondition>

#include <vector>
#include <string>
using std::vector;
using std::string;

/**
 * @brief
 * Klasa odpowiadająca za graficzną prezentację symulacji, dziedziczy po QGraphicsScene i rysuje poszczególne elementy.
 */
class Scene : public QGraphicsScene {
    Q_OBJECT

public:
    Scene(QObject * parent = 0);
    ~Scene();

    /// Metoda rysująca wszystkie obiekty MapElement.
    void draw();
    /// Metoda inicjująca Scene. Argumentem jest ścieżka do pliku mapy (format: JSON).
    void init(const string&);

    /// Metoda zwraca wszystkie obiekty MapElements.
    vector<PMapElement>& getMapElements();
    /// Metoda służy do wyznaczenia najkrótszej ścieżki pomiędzy dwoma wierzchołkami grafu.
    vector<int> getShortestPath(int, int);

    /// Metoda zwraca elementy mapy kolidujące z podanym.
    vector<MapElement*> collidingMapElements(QGraphicsItem*);
    /// Metoda zwraca pojazdy kolidujące z podanym.
    vector<Vehicle*> collidingVehicles(QGraphicsItem*);
    /// Metoda określa czy podany element koliduje z innymi pojazdami.
    bool collidingVehicles(QGraphicsItem*, Vehicle*);

    /// Metoda służąca do odświeżania widoku.
    void updateViewPort();

public Q_SLOTS:
    void addItemSlot(QGraphicsItem*,QWaitCondition*);
    void addVehicleToAnimationSlot(PVehicle, Animation*, QMutex *);
    void addPedestrianToAnimationSlot(PPedestrian, Animation*, QMutex*);

Q_SIGNALS:
    void mousePressed(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseMoved(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseReleased(QGraphicsSceneMouseEvent* mouseEvent);
    void addItemSignal(QGraphicsItem*, QWaitCondition*);

protected:
    vector<PMapElement> mapElements;
    ShortestPath* shortestPath;

    const QBrush asphaltBrush;
    const QBrush lanesBrush;
    const QBrush pavementBrush;
    const QPen noPen;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent*  );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent*   );
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);

    friend class Road;
    friend class Cross;
    friend class Spawn;
};
#endif
/* SCENE_H_ */
