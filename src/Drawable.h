/**
 * Drawable.h
 * Klasa abstrakcyjna służąca do rysowania elementów. Przeciążana jest w klasach pochodnych metoda draw().
 */

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

class Scene;

#include "Position.h"

/**
 * @brief
 * Klasa bazowa dla obiektów, które będą wyświetlane na scenie
 */
class Drawable {

protected:
    Position position;

public:
    /// Konstruktor
    Drawable(Position position_) : position(position_) {}
    /// Konstruktor
    Drawable(double x_, double y_, double z_ = 0) : position(Position(x_, y_, z_)) {}
    virtual void draw(Scene* scene) = 0;

    /// Zwraca pozycję obiektu
    Position getPosition() const {
        return position;
    }

    /// Ustawia pozycję obiektu
    void setPosition(const double x_, const double y_) {
        position.x = x_;
        position.y = y_;
    }

    /// Ustawia pozycję obiektu
    void setPosition(const Position position_) {
        position.x = position_.x;
        position.y = position_.y;
    }
};

#endif
/* DRAWABLE_H_ */
