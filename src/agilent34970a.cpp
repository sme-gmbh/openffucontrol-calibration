#include "agilent34970a.h"
#include <QThread>
#include <QEventLoop>
#include <QTimer>

Agilent34970A::Agilent34970A(QObject *parent) : QObject(parent)
{

}

bool Agilent34970A::open(QString portName)
{
    m_port = new QSerialPort(portName);
    m_port->setBaudRate(QSerialPort::Baud19200);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setParity(QSerialPort::NoParity);
    m_port->setStopBits(QSerialPort::OneStop);
    m_port->setFlowControl(QSerialPort::HardwareControl);
    connect(m_port, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
    bool openOK = m_port->open(QIODevice::ReadWrite);
    m_port->setBreakEnabled(false);
    m_port->setTextModeEnabled(true);

    return openOK;
}

void Agilent34970A::close()
{
    if (m_port->isOpen())
        m_port->close();
    delete m_port;
}

bool Agilent34970A::configure()
{
    sendCommand("*RST");
    delay(250);
    sendCommand("*RST");
    delay(250);
    sendCommand("ROUT:SCAN (@)");
    delay(250);
    //sendCommand("ROUT:SCAN (@101,102,103,104,105,106,107)");
    sendCommand("ROUT:SCAN (@107)");
    delay(250);
    sendCommand("SENS:FUNC \"TEMP\",(@101:107)");
    delay(250);
    sendCommand("UNIT:TEMP C,(@101:107)");
    delay(250);
    sendCommand("SENS:TEMP:TRAN:TYPE FRTD,(@101:107)");
    delay(250);
    sendCommand("SENS:TEMP:TRAN:FRTD:TYPE 85,(@101:107)");
    delay(250);
    sendCommand("SENS:TEMP:TRAN:FRTD:RES:REF 1000.0,(@101:106)");
    delay(250);
    sendCommand("SENS:TEMP:TRAN:FRTD:RES:REF 100.014,(@107)");
    delay(250);
    sendCommand("SENS:TEMP:NPLC 20,(@101:107)");
    delay(250);
    sendCommand("FORM:READ:ALAR OFF");
    delay(250);
    sendCommand("FORM:READ:CHAN OFF");
    delay(250);
    sendCommand("FORM:READ:TIME OFF");
    delay(250);
    sendCommand("FORM:READ:UNIT OFF");
    delay(250);
    sendCommand("");
    delay(250);
    sendCommand("");
    delay(250);
    return true;
}

bool Agilent34970A::slot_read()
{
    sendCommand("READ?");
}

void Agilent34970A::sendCommand(QString command)
{
    command.append("\r\n");
    m_port->write(command.toUtf8());
}

void Agilent34970A::slot_readyRead()
{
    while (m_port->canReadLine())
    {
        rxBuffer += m_port->readLine();
        emit signal_receivedRaw(rxBuffer);
        rxBuffer.clear();
    }
}

inline void Agilent34970A::delay(int milliseconds)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(milliseconds);
    loop.exec();
}
