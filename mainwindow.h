#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class NordVpnController;
class QSystemTrayIcon;
class QAction;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    NordVpnController* nordVpnController;
    QSystemTrayIcon* trayIcon;

    bool firstDisconnectOccurred = false;
    bool firstStatusUpdateOccurred = false;

    QAction* connectAction = nullptr;
    QAction* disconnectAction = nullptr;
};
#endif // MAINWINDOW_H
