#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>
#include <QDebug>

const auto key = "41e7e4c2-56c7-44bf-b7a7-fb291c059084";

int main(int argc, char *argv[])
{
    int result = 0;

    QApplication::setOrganizationName("volkerwick");
    QApplication::setOrganizationDomain("volkerwick.github.io");
    QApplication::setApplicationName("NordVpnTray");

    QSharedMemory sharedMemory(key);

    if (sharedMemory.create(1)) {
        QApplication a(argc, argv);

        MainWindow w;
        result = a.exec();
    } else {
        qInfo() << qAppName() << "is already running. No need to run again.";
    }

    sharedMemory.detach();

    return result;
}
