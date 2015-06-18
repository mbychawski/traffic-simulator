#ifndef SPAWN_H
#define SPAWN_H

class Scene;
class Vehicle;

#include "MapElement.h"
#include "Direction.h"
#include "Types.h"

#include <vector>
using std::vector;

/**
 * @brief
 * Klasa reprezentująca pojedynczy element służący jak punkt początkowy i końcowy dla pojazdów i pieszych.
 */
class Spawn : public MapElement {
    vector<PVehicle> vehiclesToStart;
    Direction direction;
    void calculateDirection(Position);
    Position calculateStartPosition(int);

public:
    /// Konstruktor.
    Spawn(int, Position, Position);
    /// Konstruktor.
    Spawn(int, double, double, Position);

    /// Dodawanie pojazdu do kolejki oczekujących na start.
    void addVehicle(PVehicle);

    /// Wyświetla pojazdy na mapie.
    PVehicle startVehicle();
    /// Zwraca informację, czy Spawn posiada jeszcze elementy do wystartowania.
    bool hasVehiclesToStart();
    /// Metoda odpowiadająca za rysowanie elementu.
    virtual void draw(Scene*);

    /// Pobieranie kierunku, w którym skierowany jest Spawn.
    Direction getDirection() const {
        return direction;
    }

    static bool isNotSpawn(const PMapElement el){
        if(dynamic_cast<Spawn*>(el.get()))
            return false;
        else
            return true;
    }

    /// Metoda sprawdza czy dany Spawn ma jeszcze pojazdy do wystartowania.
    static bool spawnHasVehiclesToStart(const PMapElement el){
        return !(dynamic_cast<Spawn*>(el.get())->hasVehiclesToStart());
    }

    virtual ~Spawn();
};

#endif // SPAWN_H
