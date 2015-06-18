#include "ShortestPath.h"

#include "Exceptions.h"

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <utility>

using std::pair;

ShortestPath::ShortestPath(Graph* g_) : g(g_) {}

ShortestPath::~ShortestPath() {
    //delete g;
}

Vertex ShortestPath::findVertex(int id_) {
    VertexIterator it, end;
    for(boost::tie(it, end) = boost::vertices(*g); it != end; ++it) {
        if(getId(it) == id_)
            return *it;
    }

    throw MapException("Wrong vertex id!");
}

int ShortestPath::getId(VertexIterator it) {
    return ((*g)[*(it)]).id;
}

int ShortestPath::getId(Vertex v) {
    return ((*g)[v]).id;
}

vector<int> ShortestPath::getPath(int from_, int to_) {
    Vertex from = findVertex(from_);
    Vertex to = findVertex(to_);

    vector<Vertex> predecessors(boost::num_vertices(*g));
    vector<Weight> distances(boost::num_vertices(*g));

    IndexMap indexMap;
    PredecessorMap predecessorMap(&predecessors[0], indexMap);
    DistanceMap distanceMap(&distances[0], indexMap);
    boost::dijkstra_shortest_paths(*g, from, boost::distance_map(distanceMap).predecessor_map(predecessorMap));

    vector<int> path;
    Vertex v = to;

    path.push_back(getId(to));
    for(Vertex u = predecessorMap[v]; u != v; v = u, u = predecessorMap[v]) {
        if(getId(u) != from_) {
            path.push_back(getId(u));
        }
    }

    std::reverse(path.begin(), path.end());
    path.erase(path.begin()); // usuwamy pierwszy element bo ze spawnu pojedzie prosto do niego
    return path;
}
