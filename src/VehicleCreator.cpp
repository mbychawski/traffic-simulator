#include "VehicleCreator.h"
#include "Vehicle.h"
#include "Scene.h"
#include "Spawn.h"
#include "Exceptions.h"

#include <exception>

VehicleCreator::VehicleCreator(Scene* scene_, const string &path) {
    scene = scene_;

    if(!path.empty()) {
        try {
            boost::property_tree::read_json(path, propertyTree);
        }
        catch(std::exception ex) {
            throw VehicleException(ex.what());
        }
    }

    BOOST_FOREACH(const ptree::value_type &v, propertyTree.get_child("")) {
            addVehicles(v.second);
    }
}

/*
 *	Prywatne.
 */
void VehicleCreator::addVehicles(const ptree &v_) {
    int id = v_.get<int>("id");
    string type = v_.get<string>("type");
    string color = v_.get<string>("color");
    int startID = v_.get<int>("startID");

    float maxSpeed = v_.get<float>("maxSpeed");
    float acceleration  = v_.get<float>("acceleration") / 10;
    float reflex = v_.get<float>("reflex");

    if(v_.get<string>("driveMode") == "random") {
        PVehicle vehicle(new Vehicle(id, type, startID, color, maxSpeed, acceleration, reflex));
        vehicles.push_back(vehicle);
        addToSpawn(startID, vehicle);
    }
    else if(v_.get<string>("driveMode") == "destination") {
        int destinationID = v_.get<int>("destinationID");

        PVehicle vehicle(new Vehicle(id, type, startID, color, maxSpeed, acceleration, reflex, destinationID));

        vehicles.push_back(vehicle);
        addToSpawn(startID, vehicle);
        vehicle->setPath(scene->getShortestPath(startID, destinationID));
    }
}

void VehicleCreator::addToSpawn(int spawnID_, PVehicle v_) {
    vector<PMapElement> mapElements = scene->getMapElements();
    vector<PMapElement>::const_iterator it = mapElements.begin();

    while((++it) != mapElements.end()) {
        if(spawnID_ == it->get()->getID()) {
            // downcasting
            MapElement* m = it->get();
            Spawn* s = static_cast<Spawn*>(m);
            if(s)
                s->addVehicle(v_);
            else
                throw VehicleException("Wrong start ID!");

            break;
        }
    }
}
