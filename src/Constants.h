#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const double ELEMENTAL_WIDTH = 20;

const double CROSS_SIZE = 6;

const double PAVEMENT_WIDTH = ELEMENTAL_WIDTH * 0.75;
const double LANE_WIDTH = ELEMENTAL_WIDTH * 0.1;
const double SPAWN_LENGTH = ELEMENTAL_WIDTH * 6;

const double VEHICLE_WIDTH = 0.8; // * szerokosc lane
const double VEHICLE_CAR_LENGTH = ELEMENTAL_WIDTH * 1.5;
const double VEHICLE_TRUCK_LENGTH = ELEMENTAL_WIDTH * 2.5;
const double VEHICLE_LANE_WIDTH = (ELEMENTAL_WIDTH * CROSS_SIZE / 2 - PAVEMENT_WIDTH - LANE_WIDTH) / 2;
const int    VEHICLE_SPAWN_INTERVAL = 2;

const double PEDESTRIAN_RADIUS = 0.5 * PAVEMENT_WIDTH;
const double PEDESTRIAN_OFFSET = (PAVEMENT_WIDTH - 2 * PEDESTRIAN_RADIUS) / 2;
const double PEDESTRIAN_SPEED = 20;
const int    NUMBER_OF_PEDESTRIANS = 15;

const double ROADWAY_WIDTH = (CROSS_SIZE * ELEMENTAL_WIDTH - PAVEMENT_WIDTH * 2);

const double CONST_DT = 40;

const std::string PRIORITY_COLOR = "white";

#endif // CONSTANTS_H
