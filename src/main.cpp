#include "MainWindow.h"

#include <QApplication>
#include "Exceptions.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setGeometry(200,200,800,600);
    return a.exec();
}
