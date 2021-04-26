#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configuredialog.h"
#include <QDebug>
#include <QtGui>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_timeRepeat = new QTimer(parent);
    ui->setupUi(this);
    this->setFixedSize(501,276);
    m_configureDialog = new configureDialog(this);
    m_messageErrorDialog = new MessageErrorDialog(this);
    this->setWindowIcon(QIcon(":/Rec/images/RFID-horizontal-512.png"));
    this->setWindowTitle("ASTI-RFID Write Card");
    this->setWindowModality(Qt::WindowModal);
    initWindow();

}

MainWindow::~MainWindow()
{
    delete ui;
    m_serialPort.close();
}

void MainWindow::initWindow()
{
    ui->readText->setReadOnly(true);
//    QLineEdit::inputMask():
    ui->WritelineEdit->setInputMask("00000;0");
    ui->cardDetectBar->setValue(0);
    ui->strengthBar->setValue(0);
    connect(ui->actionConfigure, &QAction::triggered, m_configureDialog, &QDialog::show);
    connect(ui->actionConnect, &QAction::changed,this, &MainWindow::onConnect);
    connect(&m_masterThread, &MasterThread::error,m_messageErrorDialog, &MessageErrorDialog::messageShow);
    connect(&m_masterThread, &MasterThread::error,this, &MainWindow::checkUpdate);
    connect(this, &MainWindow::error, m_messageErrorDialog, &MessageErrorDialog::messageShow);
    connect(ui->writeButton, &QPushButton::clicked, this, &MainWindow::writeCmd);
    connect(&m_masterThread, &MasterThread::response, this, &MainWindow::response);
    connect(ui->readButton, &QPushButton::clicked, this, &MainWindow::readOneTime);
    connect(ui->actionUpdate, &QAction::changed, this, &MainWindow::continuosUpdate);
    connect(m_timeRepeat, &QTimer::timeout, this, &MainWindow::readOneTime);
    enableButtonWhenConnect(false);
//    connect(ui->actionUpdate,&QAction::changed(),this, &)
}

void MainWindow::onConnect()
{
    if(m_configureDialog->Setting().portName == tr(""))
    {
        emit error(tr("Port name empty, \nPlease check again."));
        ui->actionConnect->setChecked(false);
        return;
    }
    if(ui->actionConnect->isChecked()){
        m_serialPort.setPortName(m_configureDialog->Setting().portName);
        if(!m_serialPort.open(QIODevice::ReadWrite))
        {
            emit error("Can't connect!");
        }
        m_serialPort.setParity(QSerialPort::Parity(m_configureDialog->Setting().parity));
        m_serialPort.setBaudRate(m_configureDialog->Setting().baud);
        m_serialPort.setDataBits(QSerialPort::DataBits(m_configureDialog->Setting().dataBits));
        enableButtonWhenConnect(true);
        ui->statusbar->showMessage(tr("Connected: %1,  %2,  Timeout: %3,  Retries: %4").
                                   arg(m_configureDialog->Setting().portName)
                                   .arg(m_configureDialog->Setting().baud)
                                   .arg(m_configureDialog->Setting().responseTime)
                                   .arg(m_configureDialog->Setting().numberOfRetries), 0);
    }
    else{
        m_serialPort.close();
        enableButtonWhenConnect(false);
        ui->statusbar->showMessage("Disconnected!!", 0);
        m_timeRepeat->stop();
    }
}
void MainWindow::writeCmd()
{

    if(ui->WritelineEdit->displayText() == tr("00000"))
    {
        emit error("Name index is empty!!\nPlease check again");
        return;
    }
    QByteArray data = ui->headerRFIDBox->currentText().toUtf8();
    data += ui->WritelineEdit->displayText().toUtf8();
    transferCmd(data, RFIDdata::cmdWriteDataRfid);
}
void MainWindow::readOneTime()
{
    QByteArray data = "";
    transferCmd(data, RFIDdata::cmdReadDataRfid);
}
void MainWindow::continuosUpdate()
{
    if(ui->actionUpdate->isChecked())
    {
        m_timeRepeat->start(100);
    }
    else
    {
        m_timeRepeat->stop();
    }
}
void MainWindow::checkUpdate()
{
    if(m_timeRepeat->isActive())
        ui->actionUpdate->setChecked(false);
}
void MainWindow::response(const QByteArray &data)
{
    RFIDdata rfidData(data);
    QString str;
    switch (m_typeCmd)
    {
    case RFIDdata::cmdReadDataRfid:
        ui->cardDetectBar->setValue(rfidData.getCardReadingStatus() ? 100 : 0);
        ui->strengthBar->setValue(rfidData.getCardSignalStrength() * 100/7);
        str = QString::fromUtf8(rfidData.getCardData());
        qDebug() << "String Card: "<< str;
        ui->readText->setPlainText(str);
        break;
    case RFIDdata::cmdWriteDataRfid:
        switch(rfidData.getWriteStatus()){
        case RFIDdata::successfully:
            ui->writeStLable->setText("Success");
            break;
        case RFIDdata::cardNotDetected:
            ui->writeStLable->setText("Can't detect");
            break;
        case RFIDdata::cardLockwrite:
            ui->writeStLable->setText("Card lock");
            break;

        }

        break;
    case RFIDdata::resReadDataRfid:
    case RFIDdata::resWriteDataRfid:
        break;
    }
}
void MainWindow::enableButtonWhenConnect(bool enable)
{
    if(enable)
    {
        ui->actionConfigure->setEnabled(false);
        ui->actionUpdate->setEnabled(true);
        ui->readButton->setEnabled(true);
        ui->writeButton->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->groupBox_2->setEnabled(true);
        ui->protocoComboBox->setEnabled(true);
        ui->slaveAddEditNum->setEnabled(true);
    }
    else
    {
        ui->actionConfigure->setEnabled(true);
        ui->actionUpdate->setEnabled(false);
        ui->readButton->setEnabled(false);
        ui->writeButton->setEnabled(false);
        ui->groupBox->setEnabled(false);
        ui->groupBox_2->setEnabled(false);
        ui->protocoComboBox->setEnabled(false);
        ui->slaveAddEditNum->setEnabled(false);
    }

}
void MainWindow::transferCmd(const QByteArray &data, const RFIDdata::TypeData type)
{
    RFIDdata dataPacket(ui->slaveAddEditNum->value(), data, type);
    m_typeCmd = type;
    m_masterThread.transaction(&m_serialPort, dataPacket.getDataFrame() ,m_configureDialog->Setting().responseTime, m_configureDialog->Setting().numberOfRetries);
}
