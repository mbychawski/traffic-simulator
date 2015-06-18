/**
 * MapElement.h
 * Klasa abstrakcyjna, bazowa dla wszystkich elementów mapy.
 */
#ifndef MAPELEMENT_H_
#define MAPELEMENT_H_

#include "Drawable.h"

/**
 * @brief
 * Klasa bazowa dla Drogi, Skrzyżowania i miejsc tworzenia pojazdów
 */
class MapElement : public Drawable {
    protected:
        int id;

	public:
        /// Konstruktor
        MapElement() : Drawable(0,0), id(-1) {}
        /// Konstruktor
        MapElement(double x_, double y_, int id_) : Drawable(x_, y_), id(id_) {}
        /// Konstruktor
        MapElement(Position position_, int id_) : Drawable(position_), id(id_) {}
        /// Konstruktor
        MapElement(const MapElement& mapElement) : Drawable(mapElement.position), id(mapElement.id) {}

        /// @overload
        virtual void draw(Scene *scene) = 0;

        /// Metoda zwraca id elementu mapy
        int getID() const {
            return id;
        }

        /// Metoda ustawia id
        void setID(const int id_) {
            id = id_;
        }
};

#endif
/* MAPELEMENT_H_ */
