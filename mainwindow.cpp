#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "nordvpncontroller.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , nordVpnController(new NordVpnController(this))
    , trayIcon(new QSystemTrayIcon(this))
{
    ui->setupUi(this);

    QMenu* contextMenu = new QMenu(this);

    QSettings settings;
    QStringList ignoredCountries = settings.value("ignoredCountries", QStringList{}).toStringList();

    QMenu* locationsMenu = contextMenu->addMenu("Locations...");
    for (const QString& country: nordVpnController->countries()) {
        if (!ignoredCountries.contains(country)) {
            QMenu* countryMenu = locationsMenu->addMenu(country);
            for (const QString& city : nordVpnController->cities(country)) {
                countryMenu->addAction(city, [=]() { nordVpnController->vpnConnect(country, city); });
            }
        }
    }

    contextMenu->addSeparator();

    contextMenu->addAction("Connect", [=]() { nordVpnController->vpnConnect(); });
    contextMenu->addAction("Disconnect", [=]() { nordVpnController->vpnDisconnect(); });

    contextMenu->addSeparator();

    contextMenu->addAction("Exit", []() { qApp->exit(); });

    trayIcon->setContextMenu(contextMenu);
    trayIcon->setIcon(QIcon(":/resources/invalid.png"));
    trayIcon->show();

    connect(nordVpnController, &NordVpnController::connected, this, [=]() { trayIcon->setIcon(QIcon(":/resources/connected.png")); });
    connect(nordVpnController, &NordVpnController::disconnected, this, [=]() { trayIcon->setIcon(QIcon(":/resources/disconnected.png"));});
    connect(nordVpnController, &NordVpnController::updateStatus, this, [=](const QString& status) { trayIcon->setToolTip(status); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

