#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "nordvpncontroller.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , nordVpnController(new NordVpnController(this))
    , trayIcon(new QSystemTrayIcon(this))
{
    ui->setupUi(this);

    QMenu* contextMenu = new QMenu(this);

    for (auto destination : QStringList{"de", "ch", "be", "uk", "us"}) {
        contextMenu->addAction(destination, [=]() { nordVpnController->vpnConnect(destination); });
    }

    contextMenu->addSeparator();

    connectAction = contextMenu->addAction("Connect", [=]() { nordVpnController->vpnConnect(); });
    connectAction->setEnabled(false);

    disconnectAction = contextMenu->addAction("Disconnect", [=]() { nordVpnController->vpnDisconnect(); });
    disconnectAction->setEnabled(false);

    contextMenu->addSeparator();

    contextMenu->addAction("Exit", []() { qApp->exit(); });

    trayIcon->setContextMenu(contextMenu);
    trayIcon->setIcon(QIcon(":/resources/invalid.png"));
    trayIcon->setToolTip(tr("Please check whether nordvpn is installed."));

    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, [=]() {
        nordVpnController->vpnConnect();
    });

    connect(nordVpnController, &NordVpnController::connected, this, [=]() {
        trayIcon->setIcon(QIcon(":/resources/connected.png"));
    });

    connect(nordVpnController, &NordVpnController::disconnected, this, [=]() {
        if (!firstDisconnectOccurred) {
            firstDisconnectOccurred = true;

            // auto connect only the very first time a disconnect was detected
            nordVpnController->vpnConnect();
        } else {
            trayIcon->showMessage(tr("Disconnected"), tr("You are disconnected, click here to reconnect."), QSystemTrayIcon::Warning, 30000);
        }

        trayIcon->setIcon(QIcon(":/resources/disconnected.png"));
    });

    connect(nordVpnController, QOverload<const QMap<QString, QString>&>::of(&NordVpnController::updateStatus), this, [&](const QMap<QString, QString> status) {

        qDebug() << "updateStatus signal; " << firstStatusUpdateOccurred;
        if (!firstStatusUpdateOccurred) {
            firstStatusUpdateOccurred = true;

            // we have established that nordvpn is installed, so we can enable the connect/disconnect actions
            if (connectAction) {
                connectAction->setEnabled(true);
            }

            if (connectAction) {
                disconnectAction->setEnabled(true);
            }
        }

        QStringList tooltip;
        for (auto k: status.keys()) {
            tooltip << QString("<b>%1</b>: %2").arg(k).arg(status[k]);
        }
        trayIcon->setToolTip(tooltip.join("<br/>"));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

