#ifndef NORDVPNCONTROLLER_H
#define NORDVPNCONTROLLER_H

#include <QObject>

class QProcess;

class NordVpnController : public QObject
{
    Q_OBJECT
public:
    explicit NordVpnController(QObject *parent = nullptr);

    void update();
    void vpnConnect();
    void vpnDisconnect();

signals:
    void updateStatus(const QString& status);
    void disconnected();
    void connected();

private:
    QString nordvpnCommand(const QStringList& params);
};

#endif // NORDVPNCONTROLLER_H
