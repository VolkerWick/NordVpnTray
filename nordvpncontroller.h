#ifndef NORDVPNCONTROLLER_H
#define NORDVPNCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QList>

class QProcess;
class QTimer;

class NordVpnController : public QObject
{
    Q_OBJECT
public:
    explicit NordVpnController(QObject *parent = nullptr);

    void update();
    void vpnConnect();
    void vpnDisconnect();
    void vpnConnect(const QString& country, const QString& city);

    QList<QString> countries() const { return locations.keys(); }
    QList<QString> cities(const QString& country) const { return locations[country]; }

signals:
    void updateStatus(const QString& status);
    void disconnected();
    void connected();

private:
    QString nordvpnCommand(const QStringList& params);
    static QString sanitize(const QString& s);

    QMap<QString, QStringList> locations;
    QTimer* timer;
};

#endif // NORDVPNCONTROLLER_H
