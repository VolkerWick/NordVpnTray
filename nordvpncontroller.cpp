#include "nordvpncontroller.h"

#include <QProcess>
#include <QRegularExpression>
#include <QTimer>
#include <QDebug>

const QRegularExpression splitter("[\\s,-]");

NordVpnController::NordVpnController(QObject *parent)
    : QObject(parent)
    , timer(new QTimer(this))
    , process(new QProcess(this))
{
    process->setProgram("nordvpn");

    connect(process, &QProcess::errorOccurred, this, &NordVpnController::errorOccurred);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &NordVpnController::finished);

    process->setArguments(QStringList{"--version"});
    process->start();

    connect(timer, &QTimer::timeout, this, &NordVpnController::update);
    timer->start(10 * 1000);

    update();
}

void NordVpnController::parseStatus()
{
    status.clear();

    while (!process->atEnd()) {
        QString line = process->readLine().trimmed();
        if (line.contains(":")) {
            QStringList kvp = line.split(": ");
            status[kvp.first()] = kvp.last();
        }
    }

    QString connectionStatus = status["Status"];

    if (connectionStatus == "Connected") {
        emit connected();
    } else if (connectionStatus == "Disconnected") {
        emit disconnected();
    }

    emit updateStatus(status);
}

QString NordVpnController::getNordVpnVersion() const
{
    return nordVpnVersion;
}

void NordVpnController::update()
{
    process->setArguments(QStringList{"status"});
    process->start();
    process->waitForFinished();
}

void NordVpnController::vpnConnect()
{
    process->setArguments(QStringList{"connect"});
    process->start();
    process->waitForFinished();
}

void NordVpnController::vpnDisconnect()
{
    process->setArguments(QStringList{"disconnect"});
    process->start();
    process->waitForFinished();
}


void NordVpnController::errorOccurred(QProcess::ProcessError error) {
    qWarning() << "Error occurred." << process->program() << process->arguments() << error << process->errorString();
}

void NordVpnController::finished(int exitCode, QProcess::ExitStatus exitStatus) {

    if (exitCode == 0 && exitStatus == QProcess::NormalExit) {

        QString arg = process->arguments().first();

        if (arg == "--version") {

            nordVpnVersion =  process->readAllStandardOutput();

            process->setArguments(QStringList{"status"});
            process->start();
            process->waitForFinished();

        } else if (arg == "status") {

            parseStatus();

        }
    }
}
