#include "nordvpncontroller.h"

#include <QProcess>
#include <QRegularExpression>
#include <QTimer>

#include <system_error>

const QRegularExpression splitter("[\\s,-]");

NordVpnController::NordVpnController(QObject *parent)
    : QObject(parent)
    , timer(new QTimer(this))
{
    QString result = nordvpnCommand(QStringList{"--version"});
    if (result.isEmpty()) {
        throw std::runtime_error(std::string("Please check whether nordvpn is installed!"));
    }

    QStringList countries = nordvpnCommand(QStringList{"countries"}).split(splitter, QString::SkipEmptyParts);
    for (const QString& country : countries) {
        QStringList cities = nordvpnCommand(QStringList{"cities", country}).split(splitter, QString::SkipEmptyParts);
        locations.insert(country, cities);
    }

    connect(timer, &QTimer::timeout, this, &NordVpnController::update);
    timer->start(10 * 1000);

    update();
}

QString NordVpnController::nordvpnCommand(const QStringList &params)
{
    QProcess p;
    p.start("nordvpn", params);
    p.waitForFinished(2000);

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

void NordVpnController::vpnConnect(const QString &country, const QString &city)
{
    nordvpnCommand(QStringList{"connect", country, city});
    update();
}



