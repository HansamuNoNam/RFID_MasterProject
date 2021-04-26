#include <QString>
#include <QtCore>
#include <QByteArray>
#include "Endian.h"

#ifndef RFIDDATA_H
#define RFIDDATA_H

class RFIDdata
{
public:
    enum TypeData
    {
        cmdReadDataRfid = 0,
        resReadDataRfid,
        cmdWriteDataRfid,
        resWriteDataRfid
    };
    enum StatusWrite
    {
        successfully = 0x00,
        cardLockwrite = 0x01,
        cardNotDetected = 0x02,
    };

public:
    RFIDdata(QByteArray dataframe);
    RFIDdata(char addressSalve, QByteArray data, TypeData type);
    ~RFIDdata();

    QByteArray getDataFrame();
    char getAddressSlave();
    char getCardReadingStatus();
    char getCardSignalStrength();
    QByteArray getCardData();
    char getWriteStatus();
    bool isMatchCheckCode();
private:
    unsigned short checkCode(QByteArray data, unsigned int len);
    const int m_addressSlaveIndex = 5;
    // index of data format of read card data respond from slave device
    const int m_cardReadingStatusIndex = 6;
    const int m_cardSignalStrengthStatusIndex = 7;
    const int m_cardDataIndex = 8;
    // index of data format of write card data cmd form master device
    const int m_cardDataInputIndex = 6;
    // index of data format of write card data respond from slave device
    const int m_writeStatusIndex = 6;


    QByteArray m_data;
    bool m_matchStatus = false;
};

#endif // RFIDDATA_H
