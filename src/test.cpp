#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include "Scene.h"
#include "MapFactory.h"
//#include "VehicleCreator.h"

#include "Cross.h"
#include "Road.h"
#include "Spawn.h"

#include "Exceptions.h"
#include "CrossRules.h"
#include "ShortestPath.h"

#include "Types.h"

#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(TrafficSimulatorTests)

BOOST_AUTO_TEST_CASE( test_loadFiles ) {
    MapFactory map("settings/map.json");
	
    BOOST_CHECK_NO_THROW ( map.createMap() );
    //BOOST_CHECK_NO_THROW ( new VehicleCreator(0L, "settings/map.json") );
}

BOOST_AUTO_TEST_CASE( test_mapElements ) {
    // ROADS
    PMapElement c1(new Cross(0, 0, 0, 0L, std::map<Direction, int>()));
    PMapElement c2(new Cross(1, 0, 80,0L, std::map<Direction, int>()));
    PMapElement c3(new Cross(2, 12, 15, 0L, std::map<Direction, int>()));
	
    BOOST_REQUIRE_NO_THROW (new Road(c1, c2));
    BOOST_REQUIRE_THROW (new Road(c1, c3), MapException );
    BOOST_ASSERT ( (Road(c1, c2)).getLength() > 0 );

    // SPAWN
    BOOST_REQUIRE_NO_THROW(new Spawn(0, c1->getPosition(), c2->getPosition()));
}

BOOST_AUTO_TEST_CASE( test_cross ) {
    // CROSS
    map<Direction, int> neighbors;
    neighbors[N] = 2;
    neighbors[E] = -1;
    neighbors[S] = 5;
    neighbors[W] = -1;
    Cross c(1, 5, 5, 0L, neighbors);
	
    BOOST_ASSERT(c.getRandomDirection(N) == S);
    BOOST_REQUIRE_THROW(c.getNeighborDirection(10), MapException);
    BOOST_ASSERT(c.getNeighborDirection(5) == S);

    // CROSS RULES
    map<Direction, int> dir;
    dir[N] = 2;
    dir[E] = 4;
    dir[S] = 5;
    dir[W] = 3;

    vector<char> sub;
    sub.push_back('N');
    sub.push_back('S');
    CrossRules cr(sub, dir);
    BOOST_ASSERT((cr.getRules(E, W)).empty());
	
	// w lewo skrecamy z podporzadkowanej
    set<Direction> result = cr.getRules(N, E);
    BOOST_CHECK(result.find(E) != result.end() && result.find(W) != result.end() && result.find(S) != result.end());
	
	// w prawo z podporzadkowanej
	set<Direction> result1 = cr.getRules(N, W);
    BOOST_CHECK(result1.find(E) != result1.end());
}

BOOST_AUTO_TEST_CASE( test_graph ) {
    MapFactory map("settings/map.json");
    ShortestPath sp(map.createMap());
    
    BOOST_CHECK_NO_THROW ( sp.getPath(1, 5) );
    BOOST_CHECK_THROW ( sp.getPath(1, -50), MapException );
}


BOOST_AUTO_TEST_CASE( test_animation ) {

}


BOOST_AUTO_TEST_SUITE_END()
