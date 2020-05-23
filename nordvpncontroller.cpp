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

QString NordVpnController::nordvpnCommand(const QStringList &params)
{
    QProcess p;
    p.start("nordvpn", params);
    p.waitForFinished(1000);

    return p.readAllStandardOutput();
}

void NordVpnController::update()
{
    QString status = nordvpnCommand(QStringList{"status"});

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



