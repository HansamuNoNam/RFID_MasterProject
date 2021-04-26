/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "masterthread.h"

#include <QSerialPort>
#include <QTime>

MasterThread::MasterThread(QObject *parent) :
    QThread(parent)
{

}

//! [0]
MasterThread::~MasterThread()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}
//! [0]

//! [1] //! [2]
void MasterThread::transaction(QSerialPort  *serialPort, const QByteArray &request, int waitTimeout = 1000,
                               int retriesSpinner = 1)
{
//! [1]
    const QMutexLocker locker(&m_mutex);
    m_serialPort = serialPort;
    m_waitTimeout = waitTimeout;
    m_request = request;
    m_retriesSpinner = retriesSpinner;
//! [3]
    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}
//! [4]
void MasterThread::run()
{
    m_mutex.lock();
    int countRetry = 0;
    QSerialPort  *serialPort;
    QByteArray request;
    int waitTimeout;
    int retriesSpinner;
    m_mutex.unlock();
    while (!m_quit) {
        m_mutex.lock();
        serialPort = m_serialPort;
        request = m_request;
        waitTimeout = m_waitTimeout;
        retriesSpinner = m_retriesSpinner;
        countRetry = 0;
        m_mutex.unlock();
        serialPort->clear(QSerialPort::Output);
        while (countRetry < retriesSpinner) {
            serialPort->write(request);
            qDebug() << "Send" << request.toHex();
            if (serialPort->waitForBytesWritten(waitTimeout)) {
                if (serialPort->waitForReadyRead(waitTimeout)) {
                    QByteArray responseData = serialPort->readAll();
                    while (serialPort->waitForReadyRead(1))
                        responseData += serialPort->readAll();
                    qDebug() << "Respond" << responseData.toHex();
                    emit this->response(responseData);
                    break;
                }
                else
                {
                    countRetry++;
                    if(countRetry < retriesSpinner){
                        MasterThread::msleep(10);
                    }
                    else{
                        emit error(tr("Data can't respond %1")
                                 .arg(QTime::currentTime().toString()));
                        break;
                    }
                }
    //! [9] //! [11]
            }
            else {
                emit error(tr("Data can't write %1")
                             .arg(QTime::currentTime().toString()));
            }
        }
        m_mutex.lock();
        m_cond.wait(&m_mutex);
        m_mutex.unlock();
    }
//! [13]
}

