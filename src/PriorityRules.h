/**
 * PriorityRules.h
 * Klasa bazowa dla wszystkich systemów obsługi pierwszeństwa przejazdu na skrzyżowaniach (np. zwykłe reguły pierwszeństwa, sygnalizacja świetlna).
 */

#ifndef PRIORITYRULES_H_
#define PRIORITYRULES_H_

#include "Direction.h"
#include <set>
/**
 * @brief
 * Klasa bazowa dla CrossRules. Dwustopniowa herarchia klas umożliwia późniejszy rozwój. Np. dodanie sygnalizacji świetlnej
 */
class PriorityRules {
public:
    PriorityRules() {}
    virtual std::set<Direction> getRules(Direction, Direction) = 0;
};

#endif /* PRIORITYRULES_H_ */
