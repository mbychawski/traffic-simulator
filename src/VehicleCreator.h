#ifndef VEHICLECREATOR_H
#define VEHICLECREATOR_H

class Scene;

#include "Types.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <string>

using std::vector;
using std::string;

typedef boost::property_tree::ptree ptree;

/**
 * @brief
 * Klasa odpowiadająca za odczytanie z pliku konfiguracji pojazdów oraz stworzenie ich i dostarczenie do odpowiedniego elementu Spawn.
 */
class VehicleCreator {
    Scene* scene;
    ptree propertyTree;
    vector<PVehicle> vehicles;

    void addVehicles(const ptree&);
    void addToSpawn(int, PVehicle);

public:
    /// Konstruktor klasy, drugim argumentem jest ścieżka do pliku konfiguracyjnego.
    VehicleCreator(Scene*, const string&);
};

#endif // VEHICLECREATOR_H
