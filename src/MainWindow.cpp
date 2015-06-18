#include "MainWindow.h"

#include "Scene.h"
#include "Simulator.h"
#include "CameraController.h"
#include "Exceptions.h"

#include <QTime>
#include <QGridLayout>
#include <QPushButton>
#include <QGraphicsView>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QIcon>

#include <fstream>

//Konstruktor
MainWindow::MainWindow(QWidget* parent) : QWidget(parent), simulator(0L) {
    try{
        // ustawienia tytułu okna
        this->setWindowTitle(tr("TrafficSimulator"));
        this->setWindowIcon(QIcon("settings/ikonka.png"));

        //tworzenie elementów GUI
        setupUi();
    
        // optymalizacja QGraphicsView
        graphicsView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
        graphicsView->setOptimizationFlags(QGraphicsView::DontClipPainter);
        graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
        graphicsView->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
        graphicsView->setCacheMode(QGraphicsView::CacheBackground);

        //generator liczb pseudolosowych
        qsrand((uint)QTime::currentTime().msec());

        //tworzenie głównych obiektów symulatora
        scene            = new Scene(this);
        simulator        = new Simulator(scene);
        cameraController = new CameraController(scene,this);

        //przyłączenie graphicsView do graphicsScene
        graphicsView->setScene(scene);

        // obsługa przycisków
        connect(startButton,         SIGNAL( clicked() ), this, SLOT( startButtonClicked()         ) );
        connect(addCameraButton,     SIGNAL( clicked() ), this, SLOT( addCameraButtonClicked()     ) );
        connect(saveCameraLogButton, SIGNAL( clicked() ), this, SLOT( saveCameraLogButtonClicked() ) );
    }
    catch (MyException& ex){
       startButton->setEnabled(false);
       showExceptionDialog( QString::fromStdString( ex.getMsg() ) );
    }

}

void MainWindow::setupUi() {
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11,11,11,11);

    startButton = new QPushButton("Start",this);
    gridLayout->addWidget(startButton,0,1,1,1);

    addCameraButton = new QPushButton("Add Camera",this);
    gridLayout->addWidget(addCameraButton, 0, 3, 1, 1);

    saveCameraLogButton = new QPushButton("Save Log", this);
    gridLayout->addWidget(saveCameraLogButton, 0, 4, 1, 1);

    graphicsView = new QGraphicsView(this);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gridLayout->addWidget(graphicsView,1,0,2,3);

    cameraEventsEdit = new QTextEdit(this);
    cameraEventsEdit->setMinimumWidth(200);
    cameraEventsEdit->setText("Camera Events:");
    cameraEventsEdit->setTextInteractionFlags(Qt::TextSelectableByMouse);
    gridLayout->addWidget(cameraEventsEdit,1,3,1,2);

    setLayout(gridLayout);
 }

void MainWindow::resizeEvent(QResizeEvent*) {
    graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::addCameraButtonClicked() {
    connect(scene, SIGNAL( mousePressed(QGraphicsSceneMouseEvent*) ), cameraController, SLOT( mousePressed(QGraphicsSceneMouseEvent*) ));
    connect(scene, SIGNAL( mouseReleased(QGraphicsSceneMouseEvent*) ), cameraController, SLOT( mouseReleased(QGraphicsSceneMouseEvent*) ));
    cameraEventsEdit->append("Use mouse to place new Camera.");
}

void MainWindow::startButtonClicked() {
    simulator->startSimulation();
    startButton->setEnabled(false);
}

void MainWindow::saveCameraLogButtonClicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save log");

    std::fstream out;
    out.open(fileName.toStdString().c_str(), std::fstream::out);

    out << cameraEventsEdit->toPlainText().toStdString();

    out.close();
}

void MainWindow::cameraEvent(QString info) {
    cameraEventsEdit->append(info);
}

void MainWindow::showExceptionDialog(QString msg) {
   QMessageBox::critical(this,"Map Exception", msg, QMessageBox::Ok);
}

void MainWindow::closeEvent(QCloseEvent *){
    if(simulator)
        simulator->stopSimulation();
}

MainWindow::~MainWindow() {
    if(simulator)
        delete simulator;
}

