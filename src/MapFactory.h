#ifndef MAPFACTORY_H_
#define MAPFACTORY_H_

#include "Direction.h"
#include "MapElement.h"
#include "Types.h"

#include <boost/property_tree/ptree.hpp>

#include <map>
#include <vector>
#include <string>

using std::map;
using std::vector;
using std::string;

typedef boost::property_tree::ptree ptree;

/**
 * @brief
 * Klasa odpowiada za czytanie pliku w formacie json i tworzenie na jego podstawie grafu i wszystkich elementów mapy
 */
class MapFactory {
private:
    Graph* g;

    ptree propertyTree;

    map<int, Vertex> vertices;
    map<int, PMapElement> verticesElements;
    vector<PMapElement> mapElements;

    void addCross_(const ptree::value_type&);
    void addSpawn_(const ptree::value_type&);
    void addEdges_(int, map<Direction, int>);
    void addEdge_(int, int);
public:
    /// Konstruktor klasy MapFactory, parametrem jest scieżka do pliku.
    MapFactory(const string&);

    /// Tworzy mapę i wszystkie elementy w niej zawarte.
    Graph* createMap();

    /// Zwraca wektor zawierający elementy mapy.
    vector<PMapElement>& getMapElements();

    ~MapFactory();
};

#endif
/* MAPFACTORY_H_ */
