#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "nordvpncontroller.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>

const int timer_delay = 20 * 1000;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , nordVpnController(new NordVpnController(this))
    , trayIcon(new QSystemTrayIcon(this))
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    QMenu* contextMenu = new QMenu(this);

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
    connect(timer, &QTimer::timeout,this, [&]() { nordVpnController->update(); });

    nordVpnController->update();

    timer->start(timer_delay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

