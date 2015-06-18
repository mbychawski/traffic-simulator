#include "MapFactory.h"
#include "Scene.h"
#include "Spawn.h"
#include "Cross.h"
#include "CrossRules.h"
#include "Exceptions.h"
#include "Constants.h"

#include <exception>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/detail/json_parser_error.hpp>
#include <boost/foreach.hpp>

#include <sstream>
typedef boost::graph_traits<Graph>::vertex_iterator VertexIterator;
typedef boost::property_map <Graph, boost::vertex_index_t >::type IndexMap;

MapFactory::MapFactory(const string& path) {
    if(!path.empty()) {
        try {
            boost::property_tree::read_json(path, propertyTree);
        }
        catch (boost::property_tree::json_parser::json_parser_error& err){
            std::ostringstream msg;
            msg << err.message() << ": " << err.filename() << std::endl;
            throw MapException(msg.str());
        }
    }
}

vector<PMapElement>& MapFactory::getMapElements() {
    return mapElements;
}

Graph* MapFactory::createMap() {
    g = new Graph();

    BOOST_FOREACH(const ptree::value_type &v, propertyTree.get_child("")) {
        if(v.second.get<string>("type") == "cross")
            addCross_(v);
        else if(v.second.get<string>("type") == "spawn")
            addSpawn_(v);
    }

    return g;
}

MapFactory::~MapFactory() {
    //vertices.clear();
    //mapElements.clear();
    //verticesElements.clear();
}

/*
 *	Prywatne.
 */

void MapFactory::addCross_(const ptree::value_type &v) {
    int id = v.second.get<int>("id");
    if(id < 0)
        throw MapException("Wrong ID");

    if(vertices.find(id) == vertices.end()) {
        double x = v.second.get<double>("x");
        double y = v.second.get<double>("y");
        map<Direction, int> neighbors;
        neighbors[N] = v.second.get<int>("N");
        neighbors[E] = v.second.get<int>("E");
        neighbors[S] = v.second.get<int>("S");
        neighbors[W] = v.second.get<int>("W");

        // tworzenie PriorityRules w zaleznosci od typu
        PriorityRules *p;
        if(v.second.get<string>("crossType") == "rules") {
            string s = v.second.get<string>("subordinate");
            vector<char> subordinate;
            unsigned int i = 0;
            while(i < s.length()) {
                subordinate.push_back(s.at(i));
                i++;
            }

            p = new CrossRules(subordinate, neighbors);
        }
        else
            throw MapException("Wrong rules type!");

        // dostep do wlasnosci wierzcholka
        Vertex ver = boost::add_vertex(*g);
        ((*g)[ver]).id = id;
        vertices[id] = ver;

        PMapElement c(new Cross(id, x, y, p, neighbors));
        mapElements.push_back(c);
        verticesElements[id] = c;

        addEdges_(id, neighbors);
    }
    else
        throw MapException("Not unique MapElement id");
}

void MapFactory::addSpawn_(const ptree::value_type &v) {
    int id = v.second.get<int>("id");
    if(id < 0)
        throw MapException("Wrong ID!");

    if(vertices.find(id) == vertices.end()) {
        double x = v.second.get<double>("x");
        double y = v.second.get<double>("y");
        int next = v.second.get<int>("next");

        // dodawanie drogi
        if(next < id){
            if(vertices.find(next) != vertices.end()) {
                Vertex ver = boost::add_vertex(*g);
                vertices[id] = ver;
                ((*g)[ver]).id = id;

                PMapElement s(new Spawn(id, x, y, verticesElements[next]->getPosition()));
                mapElements.push_back(s);
                verticesElements[id] = s;

                addEdge_(id, next);
            }
            else
                throw MapException("No element with such ID.");
        }
    }
    else
        throw MapException("Not unique MapElement id");
}

void MapFactory::addEdges_(int id_, map<Direction, int> neighbors_) {
    map<Direction, int>::iterator it;
    for(it = neighbors_.begin(); it != neighbors_.end(); ++it)
        // jeżeli jest dolaczony do jakiegos innego wierzcholka
        // i jeżeli jego id_ jest wieksze, to wtedy na pewno tamten stworzony
        if((*it).second != -1 && id_ > (*it).second) {
            if(vertices.find((*it).second) != vertices.end())
                addEdge_(id_, (*it).second);
            else
                throw MapException("No element with such ID.");
        }
}

void MapFactory::addEdge_(int v1_, int v2_) {
    Road* r = new Road(verticesElements[v1_], verticesElements[v2_]);
    mapElements.push_back(PMapElement(r));

    Weight w = (r->getLength() + 2 * CROSS_SIZE);
    boost::add_edge(vertices[v1_], vertices[v2_], w, *g);
}
