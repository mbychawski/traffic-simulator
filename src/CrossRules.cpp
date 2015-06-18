#include "CrossRules.h"

#include "Exceptions.h"

#include <algorithm>

#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

CrossRules::CrossRules(vector<char> subordinate_, map<Direction, int> directions_) : PriorityRules() {
    std::transform(directions_.begin(), directions_.end(), std::back_inserter(rules), boost::bind(&map<Direction, int>::value_type::second, _1));

    for(vector<char>::iterator it = subordinate_.begin(); it != subordinate_.end(); ++it) {
        if((*it) == 'N')
            rules[0] = -2;
        else if((*it) == 'E')
            rules[1] = -2;
        else if((*it) == 'S')
            rules[2] = -2;
        else if((*it) == 'W')
            rules[3] = -2;
        else
            throw MapException("Wrong cross subordinates!");
    }
}

set<Direction> CrossRules::getRules(Direction from_, Direction to_)  {
    set<int> priority;

    if(from_ % 2 == to_ % 2) {    // jedzie prosto
        if(rules[from_] == -2) {
            for(vector<int>::iterator it = rules.begin(); it != rules.end(); ++it)
                if((*it) >= 0) // jeśli istnieje i jest z pierwszeństwem
                    priority.insert(std::distance(rules.begin(), it));

            // jeśli jeszcze nie ustępujemy, to z prawej musimy ustąpić
            if(priority.find((from_ + 3) % 4) == priority.end() && rules[(from_ + 3) % 4] != -1)
                priority.insert((from_ + 3) % 4);
        }
        else if(rules[from_] >= 0) {
            // jeśli na drodze z pierwszeństwem to tylko temu z prawej
            if(rules[(from_ + 3) % 4] >= 0)
                priority.insert((from_ + 3) % 4);

        }
    }
    else if((from_ + 3) % 4 == to_) { // w prawo
        if(rules[from_] == -2) {
            if(rules[(from_ + 1) % 4] >= 0) // temu z lewej ustępujemy jeśli ma pierwszeństwo
                priority.insert((from_ + 1) % 4);
            if(rules[(from_ + 2) % 4] >= 0) // temu na wprost
                priority.insert((from_ + 2) % 4);
        }
    }
    else if((from_ + 1) % 4 == to_) { // w lewo
        if(rules[from_] == -2) {
            for(vector<int>::iterator it = rules.begin(); it != rules.end(); ++it)
                if((*it) >= 0) // jeśli istnieje i jest z pierwszeństwem
                    priority.insert(std::distance(rules.begin(), it));

            // jeżeli po prawej jest droga
            if(priority.find((from_ + 3) % 4) == priority.end() && rules[(from_ + 3) % 4] != -1)
                priority.insert((from_ + 3) % 4);

            // jeżeli na wprost jest droga
            if(priority.find((from_ + 2) % 4) == priority.end() && rules[(from_ + 2) % 4] != -1)
                priority.insert((from_ + 2) % 4);

        }
        else if(rules[from_] >= 0) {
            // jeśli na drodze z pierwszeństwem i z naprzeciwka też z pierwszeństwem to jemu
            if(rules[(from_ + 2) % 4] >= 0)
                priority.insert((from_ + 2) % 4);

            // jeśli na drodze z pierwszeństwem i z prawej też z pierwszeństwem to jemu
            if(rules[(from_ + 3) % 4] >= 0)
                priority.insert((from_ + 3) % 4);
        }

    }
    else
        throw VehicleException("Wrong from_ and to_ direction!");

    set<Direction> result;
    for(set<int>::iterator it = priority.begin(); it != priority.end(); ++it)
        result.insert(static_cast<Direction>(*it));

    return result;
}

bool CrossRules::isSubordinate(Direction direction_) {
    if(rules[direction_] != -2)
        return false;
    else
        return true;
}
