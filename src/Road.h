#ifndef ROAD_H
#define ROAD_H

class Scene;

#include "MapElement.h"

#include "Direction.h"
#include "Types.h"

/**
 * @brief
 * Klasa reprezentuje drogę na mapie
 */
class Road : public MapElement {
    Direction direction;
    int length;

    PMapElement from;
    PMapElement to;

public:
    /// Konstruktor
    Road() : MapElement(0, 0, -1) {}
    Road(PMapElement from_, PMapElement to_);

    /// Metoda tworząca i dodająca obiekty GraphicsItem do sceny
    virtual void draw(Scene *scene);

    /// Zwraca długość drogi
    int getLength() const {
        return length;
    }

    virtual ~Road(){ }
};

#endif // ROAD_H
