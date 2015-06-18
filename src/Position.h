#ifndef POSITION_H
#define POSITION_H

#include "Constants.h"

/**
 * @brief
 * Klasa reprezentuje punkt w trójwymiarowej przestrzeni
 */
struct Position{
    double x;
    double y;
    double z;

    Position(double x_, double y_, double z_ = 0) : x(x_), y(y_), z(z_) {}
    Position(const Position& position_) {
        x = position_.x;
        y = position_.y;
        z = position_.z;
    }

    double absX() const {
        return ELEMENTAL_WIDTH * x;
    }

    double absY() const {
        return ELEMENTAL_WIDTH * y;
    }

    double absZ() const {
        return ELEMENTAL_WIDTH * z;
    }
};

#endif // POSITION_H
