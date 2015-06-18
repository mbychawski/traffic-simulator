#-------------------------------------------------
#
# Project created by QtCreator 2013-01-03T13:31:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrafficSimulator
TEMPLATE = app

CONFIG += no_keywords
CONFIG += static

SOURCES +=\
    src/Vehicle.cpp \
    src/MainWindow.cpp \
    src/main.cpp \
    src/Simulator.cpp \
    src/Scene.cpp \
    src/Road.cpp \
    src/Cross.cpp \
    src/MapFactory.cpp \
    src/Spawn.cpp \
    src/VehicleCreator.cpp \
    src/CrossRules.cpp \
    src/Animation.cpp \
    src/CameraController.cpp \
    src/Camera.cpp \
    src/Pedestrian.cpp \
    src/ShortestPath.cpp \
    src/SpawningVehiclesRunnable.cpp \
    src/SpawningPedestriansRunnable.cpp \
    src/AnimationWaitingThread.cpp \
    src/AnimationTimerThread.cpp

HEADERS  +=\
    src/VehicleState.h \
    src/VehicleCreator.h \
    src/Vehicle.h \
    src/Types.h \
    src/Spawn.h \
    src/Simulator.h \
    src/Scene.h \
    src/Road.h \
    src/PriorityRules.h \
    src/Position.h \
    src/Moveable.h \
    src/MapFactory.h \
    src/MapElement.h \
    src/MainWindow.h \
    src/Exceptions.h \
    src/Drawable.h \
    src/Direction.h \
    src/CrossRules.h \
    src/Cross.h \
    src/Constants.h \
    src/CameraController.h \
    src/Camera.h \
    src/Animation.h \
    src/Pedestrian.h \
    src/ShortestPath.h \
    src/SpawningVehiclesRunnable.h \
    src/SpawningPedestriansRunnable.h \
    src/AnimationWaitingThread.h \
    src/AnimationTimerThread.h \
    src/SpawningVehiclesRunnable.h


OTHER_FILES += \
    settings/map.json \
    settings/vehicles.json \
    settings/ikonka.png

LIBS += -L{BOOST-LIB-PATH}
INCLUDEPATH += {BOOST-include-PATH}
