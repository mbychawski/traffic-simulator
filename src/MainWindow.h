#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class Scene;
class Simulator;
class CameraController;

class QGridLayout;
class QPushButton;
class QTextEdit;
class QGraphicsView;

#include <QWidget>

/**
 * @brief
 * Klasa głównego okna aplikacji
 */
class MainWindow : public QWidget {
    Q_OBJECT
    
public:
    /// Wskaźnik na obiekt Scene
    Scene*            scene;
    /// Wskaźnik na obiekt Simulator
    Simulator*        simulator;
    /// Wskaźnik na obiekt CameraController
    CameraController* cameraController;

    /// Konstruktor
    MainWindow(QWidget* parent = 0);

    ~MainWindow();

public Q_SLOTS:
   /// Slot wykonujący dzialanie po naciśnięciu przycisku Start
   void startButtonClicked();
   /// Slot wykonujący dzialanie po naciśnięciu przycisku Add Camera
   void addCameraButtonClicked();
   /// Slot wykonujący dzialanie po naciśnięciu przycisku Save Log
   void saveCameraLogButtonClicked();
   /// Slot przyjmujący wiadomości od kamer. Wyświetla je w QTextEdit
   void cameraEvent(QString);
   /// Slot wyświeltający okienko z treścią wyjątku
   void showExceptionDialog(QString);

protected:
    QGridLayout*   gridLayout;
    QPushButton*   startButton;
    QPushButton*   addCameraButton;
    QPushButton*   saveCameraLogButton;
    QTextEdit*     cameraEventsEdit;
    QGraphicsView* graphicsView;

    void setupUi();
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
