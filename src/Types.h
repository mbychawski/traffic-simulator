#ifndef TYPES_H
#define TYPES_H

#ifndef Q_MOC_RUN
    #include <boost/smart_ptr/shared_ptr.hpp>
    #include <boost/graph/adjacency_list.hpp>
    #include <boost/graph/graph_traits.hpp>
    #include <boost/function.hpp>
#endif // Q_MOC_RUN

class MapElement;
class Vehicle;
class Camera;
class Pedestrian;

typedef boost::shared_ptr<MapElement> PMapElement;
typedef boost::shared_ptr<Vehicle> PVehicle;
typedef boost::shared_ptr<Camera> PCamera;
typedef boost::shared_ptr<Pedestrian> PPedestrian;

typedef int Weight;
typedef boost::property<boost::edge_weight_t, Weight> WeightProperty;

struct VertexInfo {
    int id;
};

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, VertexInfo, WeightProperty> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

typedef boost::function<void(void)> functionPtr;

#endif // TYPES_H
