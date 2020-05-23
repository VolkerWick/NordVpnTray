#include "nordvpncontroller.h"

#include <QProcess>

#include <system_error>

NordVpnController::NordVpnController(QObject *parent)
    : QObject(parent)
{
    QProcess p;
    p.start("nordvpn --version");
    p.waitForFinished(1000);

    if (p.readAllStandardOutput().isEmpty()) {
        throw std::runtime_error(std::string("Please check whether nordvpn is installed!"));
    }
}

void NordVpnController::nordvpnCommand(const QStringList &params)
{
    QProcess p;
    p.start("nordvpn", params);
    p.waitForFinished(1000);
}

void NordVpnController::update()
{
    nordvpnCommand(QStringList{"status"});

    QString status = QString(p.readAllStandardOutput());
    emit updateStatus(status);

    if (status.contains("Status: Connected")) {
        emit connected();
    } else {
        emit disconnected();
    }
}

void NordVpnController::vpnConnect()
{
    nordvpnCommand(QStringList{"connect"});
    update();
}

void NordVpnController::vpnDisconnect()
{
    nordvpnCommand(QStringList{"disconnect"});
    update();
}



