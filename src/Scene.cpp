#include "Scene.h"

#include "MainWindow.h"
#include "Constants.h"
#include "CameraController.h"
#include "Vehicle.h"
#include "Pedestrian.h"
#include "MapFactory.h"

#include <QList>
#include <QListIterator>
#include <QGraphicsItem>
#include <QVariant>
#include <QMutexLocker>

Q_DECLARE_METATYPE(MapElement*)
Q_DECLARE_METATYPE(Vehicle*)

Scene::Scene(QObject * parent_) : QGraphicsScene( parent_ ), shortestPath(0L),
      asphaltBrush  ( QBrush( QColor(40,43,42)    ) ),
      lanesBrush    ( QBrush( QColor(245,245,245) ) ),
      pavementBrush ( QBrush( QColor(180,180,180) ) ),
      noPen         ( QPen(Qt::NoPen) ) {

    this->setBackgroundBrush(QBrush(QColor(0,104,10)));
    connect(this, SIGNAL(addItemSignal(QGraphicsItem*, QWaitCondition*)), this, SLOT(addItemSlot(QGraphicsItem*, QWaitCondition*)), Qt::QueuedConnection);
}

Scene::~Scene(){
    if(shortestPath)
        delete shortestPath;
}

void Scene::init(const string &path) {

    MapFactory     mapFactory( path );
    Graph* g     = mapFactory.createMap();
    mapElements  = mapFactory.getMapElements();

    shortestPath = new ShortestPath(g);

    draw();
}

void Scene::draw() {
    for(vector<PMapElement>::iterator it =mapElements.begin(); it != mapElements.end(); ++it){
        it->get()->draw(this);
    }
}

vector<MapElement*> Scene::collidingMapElements(QGraphicsItem* item_){
    QList<QGraphicsItem*> items = this->collidingItems(item_);
    QListIterator<QGraphicsItem*> i(items);
    vector<MapElement*> result;


    while(i.hasNext()){
        QGraphicsItem* it = i.next();
        if(it->data(1).value<MapElement*>() != 0 ){
            MapElement* mapEl = it->data(1).value<MapElement *>();
            result.push_back(mapEl);
        }
    }

    return result;
}

vector<Vehicle*> Scene::collidingVehicles(QGraphicsItem* item_){
    QList<QGraphicsItem*> items = this->collidingItems(item_);
    QListIterator<QGraphicsItem*> i(items);

    vector<Vehicle*> result;

    while(i.hasNext()){
        QGraphicsItem* it = i.next();
        if(it->data(2).value<Vehicle*>() != 0 ){
            Vehicle * v = it->data(2).value<Vehicle*>();
            result.push_back(v);
        }
    }
    return result;
}

bool Scene::collidingVehicles(QGraphicsItem* item_, Vehicle* v_) {
    QList<QGraphicsItem*> items = this->collidingItems(item_);
    QListIterator<QGraphicsItem*> i(items);

    while(i.hasNext()){
        QGraphicsItem* it = i.next();
        if(it->data(2).value<Vehicle*>() && it->data(2).value<Vehicle*>() != v_)
            return true;
    }

    return false;
}

vector<PMapElement>& Scene::getMapElements() {
    return mapElements;
}

vector<int> Scene::getShortestPath(int from_, int to_) {
    return shortestPath->getPath(from_, to_);
}

void Scene::updateViewPort(){
    this->views()[0]->viewport()->update();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Q_EMIT mousePressed(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    Q_EMIT mouseMoved(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    Q_EMIT mouseReleased(event);
}

void Scene::addItemSlot(QGraphicsItem * item, QWaitCondition * ) {
    addItem(item);
    //waitCondition->wakeAll();
}

void Scene::addVehicleToAnimationSlot(PVehicle v_, Animation* animation_, QMutex * mutex_){
    QMutexLocker locker(mutex_);
    v_->init(this);
    animation_->addVehicle(v_);
    animation_->updateVehiclesOnScene();
    updateViewPort();
}

void Scene::addPedestrianToAnimationSlot(PPedestrian p_, Animation * animation_,QMutex* mutex_) {
    QMutexLocker locker(mutex_);
    p_->init(this);
    animation_->addPedestrian(p_);
    animation_->updatePedestriansOnScene();
    updateViewPort();
}
