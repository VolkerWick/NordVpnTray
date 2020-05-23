#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QSharedMemory sharedMemory;
    sharedMemory.setKey("89e58d62-3d2d-4ef6-a849-cba25d6064ec");

    int result = -1;

    if (sharedMemory.create(1)) {
        QApplication a(argc, argv);
        MainWindow w;
        result = a.exec();
    }

    return result;
}
