#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "configuredialog.h"
#include "masterthread.h"
#include "messageerrordialog.h"
#include "rfiddata.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initWindow();
signals:
    void error(const QString &s);
private slots:
    void onConnect();
    void writeCmd();

    void continuosUpdate();
    void readOneTime();
    void checkUpdate();
    void response(const QByteArray &data);
private:
    void transferCmd(const QByteArray &data, const RFIDdata::TypeData type );
    void enableButtonWhenConnect(bool enable);
    QTimer *m_timeRepeat;
    QSerialPort m_serialPort;
    Ui::MainWindow *ui;
    configureDialog *m_configureDialog = nullptr;
    MessageErrorDialog *m_messageErrorDialog = nullptr;
    RFIDdata::TypeData m_typeCmd;
    MasterThread m_masterThread;
};
#endif // MAINWINDOW_H
