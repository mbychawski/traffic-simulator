#ifndef CROSSRULES_H
#define CROSSRULES_H

#include "PriorityRules.h"
#include "Direction.h"

#include <map>
#include <vector>
#include <set>

using std::map;
using std::vector;
using std::set;

/**
 * @brief
 * Klasa podająca informacje o pierwszeństwie przejazdu.
 */
class CrossRules : public PriorityRules {
     vector<int> rules; // -2 - podporzadkowana, -1 - nie istnieje

public:
    /// Konstruktor
    CrossRules(vector<char> subordinate_, map<Direction, int> directions_);
    /// Metoda zwraca zasady pierwszeństwa
    virtual set<Direction> getRules(Direction from_, Direction to_);
    bool isSubordinate(Direction direction_);
};

#endif // CROSSRULES_H
