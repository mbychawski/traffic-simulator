
#ifndef CROSS_H_
#define CROSS_H_

class Scene;
class PriorityRules;
class CrossRules;
class QGraphicsItemGroup;
class QGraphicsEllipseItem;


#include "MapElement.h"
#include "Types.h"
#include "Constants.h"
#include "Direction.h"

#include <map>
#include <vector>

using std::vector;
using std::map;


/**
 * @brief
 * Klasa reprezentujaca skrzyzowanie
 */
class Cross : public MapElement {
    PriorityRules* priorityRules;

    map<Direction, int> neighbors;
    map<Direction, vector<Vehicle*> > waitingVehicles;

    vector<QGraphicsEllipseItem*> drawDot(CrossRules*);

public:
     QGraphicsItemGroup* crossGroup;

    /// Konstruktor
    Cross(int id_, double x_, double y_, PriorityRules* rules_, map<Direction, int> neighbors_)
        : MapElement(x_, y_, id_), priorityRules(rules_), neighbors(neighbors_) { }
    /// Konstruktor
    Cross(int id_, Position position_, PriorityRules* rules_, map<Direction, int> neighbors_)
        : MapElement(position_, id_), priorityRules(rules_), neighbors(neighbors_) {}

    virtual ~Cross() {}

    /// Metoda odpowiadająca za wyświetlenie skrzyżowania na scenie
    virtual void draw(Scene* scene);

    /// Rejestracja pojazdu w skrzyżowaniu
    void registerVehicle(Direction direction_, Vehicle* v_);
    /// Wyrejestrowanie pojazdu
    void unregisterVehicle(Direction direction_);
    /// Metoda zwraca przyzwolenie na wjazd na skrzyżowanie
    bool canGo(Direction from_, Direction to_);

    /// Metoda zwraca kierunek w którym znajduje się sąsiad o danym ID
    Direction getNeighborDirection(int id_);
    /// Metoda podaje losowy z dostepnych kierunków
    Direction getRandomDirection(int from_);

    /// Metoda obliczająca pozycję startową dla przechodnia oraz informująca go gdzie jest chodnik
    Position calculatePedestrianPosition(Direction direction_, int side_);
    /// Zwraca losowy z dostepnych kierunków dla ruchu pieszych
    Direction getPedestrianRandomDirection();

    /// funkcja sprawdzająca czy obiekt wskazywany przez PMapElement jest skrzyżowaniem
    static bool isCross(const PMapElement el);
};

#endif /* CROSS_H_ */
