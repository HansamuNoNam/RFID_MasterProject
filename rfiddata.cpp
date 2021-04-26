#include "rfiddata.h"


RFIDdata::RFIDdata(QByteArray dataframe)
    : m_data(dataframe)
    , m_matchStatus(false)
{

}
RFIDdata::RFIDdata(char addressSalve, QByteArray data, TypeData type)
    : m_data("RCode")
{
    switch (type) {
    case cmdReadDataRfid:
        m_data = "RCode";
        break;
    case cmdWriteDataRfid:
        m_data = "WCode";
        break;
    default:
        break;
    }
    m_data += addressSalve;
    m_data += data;
    unsigned short checkCodeVal;
    checkCodeVal = RFIDdata::checkCode(m_data, m_data.length());
    char byte = (checkCodeVal & (unsigned short)0x00FF);
    m_data += byte;
    byte = ((checkCodeVal >> 8) & (unsigned short)0x00FF);
    m_data += byte;
}
RFIDdata::~RFIDdata()
{

}
QByteArray RFIDdata::getDataFrame()
{
    return m_data;
}
char RFIDdata::getAddressSlave()
{
    return m_data.at(m_addressSlaveIndex);
}
char RFIDdata::getCardReadingStatus()
{
    if(m_data.length() - 2 < m_cardReadingStatusIndex && m_data.size() >= 0)
        return (char) 0;
    return m_data.at(m_cardReadingStatusIndex);
}
char RFIDdata::getCardSignalStrength()
{
    if(m_data.length() - 2 < m_cardSignalStrengthStatusIndex && m_data.size() >= 0)
        return (char) 0;
    return m_data.at(m_cardSignalStrengthStatusIndex);
}
QByteArray RFIDdata::getCardData()
{
    QByteArray cardData;
    if(m_data.length() - 2 < m_cardDataIndex && m_data.size() >= 0)
        return cardData;
    for (char i = 0; i < 8; i++)
    {
        cardData.push_back(m_data.at(m_cardDataIndex + i));
    }
    return cardData;
}
char RFIDdata::getWriteStatus()
{
    return m_data.at(m_writeStatusIndex);
}
bool RFIDdata::isMatchCheckCode()
{
    unsigned short checkCodeVal;
    checkCodeVal = RFIDdata::checkCode(m_data, m_data.length() - 2);
    unsigned short checkCodeSample = (unsigned short)m_data.at(m_data.length() - 2) && (unsigned short)m_data.at(m_data.length() - 1);
    return checkCodeSample == checkCodeVal;
}
unsigned short RFIDdata::checkCode(QByteArray data, unsigned int len)
{
    unsigned int i, j; //#define wPolynom 0xA001
    unsigned int wCrc = 0xffff;
    unsigned int wPolynom = 0xA001;
    /*---------------------------------------------------------------------------------*/
    for (i = 0; i < len; i++)
    {
        wCrc ^= data.at(i);
        for (j = 0; j < 8; j++)
        {
            if (wCrc &0x0001){
                wCrc = (wCrc >> 1) ^ wPolynom;

            }
            else{
                wCrc = wCrc >> 1;
            }
        }
    }
    return (unsigned short)wCrc;
}
