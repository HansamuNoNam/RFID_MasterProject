#include "configuredialog.h"
#include "ui_configuredialog.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QtGui>
configureDialog::configureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configureDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Rec/images/settings.png"));
    this->setWindowTitle("Configure");
    this->setWindowModality(Qt::WindowModal);
}

configureDialog::~configureDialog()
{
    delete ui;
}
configureDialog::Settings configureDialog::Setting() const
{
    return m_setting;
}
void configureDialog::on_buttonBox_accepted()
{
    m_setting.portName = ui->portCombo->currentText();
    m_setting.parity = ui->parityCombo->currentIndex();
    if(m_setting.parity > 0)
    {
        m_setting.parity++;
    }
    m_setting.baud = ui->baudCombo->currentText().toInt();
    m_setting.dataBits = ui->dataBitsCombo->currentText().toInt();
    m_setting.stopBits = ui->stopBitsCombo->currentText().toInt();
    m_setting.responseTime = ui->timeoutSpinner->value();
    m_setting.numberOfRetries = ui->retriesSpinner->value();
}

void configureDialog::showEvent(QShowEvent *)
{
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        ui->portCombo->addItem(info.portName());
    }
    if(m_setting.parity > 0)
    {
        ui->parityCombo->setCurrentIndex(m_setting.parity - 1);
    }
    else{
        ui->parityCombo->setCurrentIndex(m_setting.parity);
    }
    ui->baudCombo->setCurrentText(QString::number(m_setting.baud));
    ui->dataBitsCombo->setCurrentText(QString::number(m_setting.dataBits));
    ui->stopBitsCombo->setCurrentText(QString::number(m_setting.stopBits));
    ui->timeoutSpinner->setValue(m_setting.responseTime);
    ui->retriesSpinner->setValue(m_setting.numberOfRetries);
    ui->portCombo->setFocus();
}
