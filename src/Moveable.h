/**
 * Moveable.h
 * Author: Wookie
 */

#ifndef MOVEABLE_H_
#define MOVEABLE_H_

#include "Direction.h"

/**
 * @brief
 * Klasa bazowa dla klas, które będą animowane
 */
class Moveable {

protected:
    float maxSpeed;
    float acceleration;
    float currentSpeed;
    double dx, dy;

	Direction direction;
    Direction nextDirection;

public:
    /// Konstruktor
    Moveable(float maxSpeed_, float acceleration_) : maxSpeed(maxSpeed_), acceleration(acceleration_), currentSpeed(0), dx(0), dy(0) {}

    /// Metoda obliczająca następną pozycję
    virtual void calculatePosition() = 0;
    /// Metoda ustawiająca kierunek ruchu
    void setDirection(Direction d_) {
        direction = d_;
    }
};

#endif /* MOVEABLE_H_ */
