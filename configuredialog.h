#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>
#include <QtSerialBus/qtserialbusglobal.h>
#include <QtSerialPort/QSerialPort>
#include <qabstractbutton.h>
namespace Ui {
class configureDialog;
}

class configureDialog : public QDialog
{
    Q_OBJECT
    struct Settings {
        QString portName;
        int parity = QSerialPort::NoParity;
        int baud = QSerialPort::Baud115200;
        int dataBits = QSerialPort::Data8;
        int stopBits = QSerialPort::OneStop;
        int responseTime = 50;
        int numberOfRetries = 3;
    };
public:
    explicit configureDialog(QWidget *parent = nullptr);
    ~configureDialog();
    Settings Setting() const;
private slots:

    void on_buttonBox_accepted();

private:
    void showEvent(QShowEvent *) override;
    Ui::configureDialog *ui;
    Settings m_setting;
};

#endif // CONFIGUREDIALOG_H
