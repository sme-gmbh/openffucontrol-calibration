#include "openffucontrolpts.h"

OpenFFUcontrolPTS::OpenFFUcontrolPTS(QObject *parent, modbus_t* bus) : QObject(parent)
{
    m_bus = bus;
}

OpenFFUcontrolPTS::~OpenFFUcontrolPTS()
{

}

void OpenFFUcontrolPTS::setSlaveAddress(quint16 adr)
{
    m_adr = adr;
}

double OpenFFUcontrolPTS::requestMeasuredTemperature()
{
    int result =0;
    uint16_t rawdata[4] = {0,0,0,0};
    double temperature;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    //result = modbus_read_input_registers(m_bus, 0x0000, 2, (uint16_t*)&rawdata);
    temperature = modbus_get_float(rawdata);
    if (result >= 0)
    {
        //temperature = rawdata;
        emit signal_receivedMeasuredTemperature(temperature);
        return temperature;
    }
    else
    {
        // Error
        return -1000.0;
    }
}

double OpenFFUcontrolPTS::requestCalibrationOffset()
{
    int result;
    float rawdata;
    double temperature;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    result = modbus_read_registers(m_bus, 0x0000, 2, (uint16_t*)&rawdata);
    if (result >= 0)
    {
        temperature = rawdata;
        emit signal_receivedMeasuredTemperature(temperature);
        return temperature;
    }
    else
    {
        // Error
        return -10000.0;
    }
}

void OpenFFUcontrolPTS::writeCalibrationOffset(double offset)
{
    int result;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    result = modbus_write_registers(m_bus, 0x0000, 2, (uint16_t*)&offset);
    if (result >= 0)
    {

    }
    else
    {
        // Error
    }
}
