#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

class Cross;
class MapElement;

#include "Moveable.h"
#include "Drawable.h"

#include <QColor>
#include <QGraphicsEllipseItem>

#include <vector>
using std::vector;

/**
 * @brief
 * Klasa reprezentująca pieszego
 */
class Pedestrian : public Moveable, public Drawable {
    QGraphicsEllipseItem qItem;

    Cross* cross;
    int streetSide;
    QColor color;

    void checkCollisions();
    void calculateStartPosition(Scene *);
    void calculateSide();
    bool checkTurnPosition();
    vector<MapElement*> collidesMapElements;

public:
    /// Konstruktor
    Pedestrian();

    /// @overload
    virtual void draw(Scene *);
    /// @overload
    virtual void calculatePosition();
    /// Metoda ustawia wszystkie początkowe wartości dla pieszego i dodaje QGraphicsItem do scene
    void init(Scene *);

    /// Aktualizuje położenie qItem na scenie
    void updateQItemOnScene();
    /// Sprawdza czy nastapiły kolizje z elementami mapy
    void checkCollisions(Scene*);

    /// Zwraca true gdy pieszy jest niekatywny
    bool isNotActive() const;

    virtual ~Pedestrian();
};

#endif // PEDESTRIAN_H
