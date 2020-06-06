#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>
#include <QDebug>

const auto key = "ad11d1a8-332f-4a83-8ee7-e00fdb87f2fd";

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
        qInfo() << "NordVpnTray is already running. No need to run again.";
    }

    sharedMemory.detach();

    return result;
}
