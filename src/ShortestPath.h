#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "Types.h"

#include <vector>

typedef boost::graph_traits<Graph>::vertex_iterator VertexIterator;
typedef boost::property_map <Graph, boost::vertex_index_t >::type IndexMap;

typedef boost::iterator_property_map < Vertex*, IndexMap, Vertex, Vertex& > PredecessorMap;
typedef boost::iterator_property_map < Weight*, IndexMap, Weight, Weight& > DistanceMap;

using std::vector;

/**
 * @brief
 * Klasa odpowiadająca za wyznaczanie najkrótszej ścieżki pomiędzy elementami o podanych ID.
 */
class ShortestPath {
    Graph* g;

    int getId(VertexIterator);
    int getId(Vertex);

    Vertex findVertex(int);
public:
    ShortestPath(Graph*);

    /// Zwraca wektor z numerami ID kolejnych skrzyżowań na najkrótszej ścieżce.
    vector<int> getPath(int from_, int to_);

    ~ShortestPath();
};

#endif // SHORTESTPATH_H
