#ifndef NORDVPNCONTROLLER_H
#define NORDVPNCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QProcess>

class QTimer;

class NordVpnController : public QObject
{
    Q_OBJECT
public:
    explicit NordVpnController(QObject *parent = nullptr);

    void update();
    void vpnConnect(const QString& destination = "");
    void vpnDisconnect();

    QString getNordVpnVersion() const;

signals:
    void updateStatus(const QMap<QString, QString>& status);
    void disconnected();
    void connected();

private slots:
    void errorOccurred(QProcess::ProcessError);
    void finished(int, QProcess::ExitStatus);

private:
    void parseStatus();

    QTimer* timer;
    QProcess* process;
    QString nordVpnVersion;
    QMap<QString, QString> status;
};

#endif // NORDVPNCONTROLLER_H
