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
    int result = 0;
    T_float_conversion rawdata;
    double temperature;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    result = modbus_read_input_registers(m_bus, 0x0000, 2, (uint16_t*)rawdata.b);

    // Sort data according to endianess
    T_float_conversion sortedData;
    sortedData.b[0] = rawdata.b[0];
    sortedData.b[1] = rawdata.b[1];
    sortedData.b[2] = rawdata.b[2];
    sortedData.b[3] = rawdata.b[3];
    temperature = sortedData.f;
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
    T_float_conversion rawdata;
    double temperature;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    result = modbus_read_registers(m_bus, 0x0000, 2, (uint16_t*)rawdata.b);
    if (result >= 0)
    {
        // Sort data according to endianess
        T_float_conversion sortedData;
        sortedData.b[0] = rawdata.b[0];
        sortedData.b[1] = rawdata.b[1];
        sortedData.b[2] = rawdata.b[2];
        sortedData.b[3] = rawdata.b[3];
        temperature = sortedData.f;
        emit signal_receivedCalibrationOffset(temperature);
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
    // Sort data according to endianess
    T_float_conversion rawdata;
    rawdata.f = offset;
    T_float_conversion sortedData;
    sortedData.b[0] = rawdata.b[0];
    sortedData.b[1] = rawdata.b[1];
    sortedData.b[2] = rawdata.b[2];
    sortedData.b[3] = rawdata.b[3];
    result = modbus_write_registers(m_bus, 0x0000, 2, (uint16_t*)sortedData.b);
    if (result >= 0)
    {

    }
    else
    {
        // Error
    }
}

double OpenFFUcontrolPTS::requestResistanceReference()
{
    int result = 0;
    T_float_conversion rawdata;
    double resistance;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    result = modbus_read_registers(m_bus, 0x0004, 2, (uint16_t*)rawdata.b);

    // Sort data according to endianess
    T_float_conversion sortedData;
    sortedData.b[0] = rawdata.b[0];
    sortedData.b[1] = rawdata.b[1];
    sortedData.b[2] = rawdata.b[2];
    sortedData.b[3] = rawdata.b[3];
    resistance = sortedData.f;
    if (result >= 0)
    {
        emit signal_receivedResistanceReference(resistance);
        return resistance;
    }
    else
    {
        // Error
        return -1000.0;
    }
}

void OpenFFUcontrolPTS::writeResistanceReference(double r)
{
    int result;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    // Sort data according to endianess
    T_float_conversion rawdata;
    rawdata.f = r;
    T_float_conversion sortedData;
    sortedData.b[0] = rawdata.b[0];
    sortedData.b[1] = rawdata.b[1];
    sortedData.b[2] = rawdata.b[2];
    sortedData.b[3] = rawdata.b[3];
    result = modbus_write_registers(m_bus, 0x0004, 2, (uint16_t*)sortedData.b);
    if (result >= 0)
    {

    }
    else
    {
        // Error
    }
}

double OpenFFUcontrolPTS::requestResistanceR304()
{
    int result = 0;
    T_float_conversion rawdata;
    double resistance;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    result = modbus_read_registers(m_bus, 0x0008, 2, (uint16_t*)rawdata.b);

    // Sort data according to endianess
    T_float_conversion sortedData;
    sortedData.b[0] = rawdata.b[0];
    sortedData.b[1] = rawdata.b[1];
    sortedData.b[2] = rawdata.b[2];
    sortedData.b[3] = rawdata.b[3];
    resistance = sortedData.f;
    if (result >= 0)
    {
        emit signal_receivedResistanceReference(resistance);
        return resistance;
    }
    else
    {
        // Error
        return -1000.0;
    }
}

void OpenFFUcontrolPTS::writeResistanceR304(double r)
{
    int result;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    // Sort data according to endianess
    T_float_conversion rawdata;
    rawdata.f = r;
    T_float_conversion sortedData;
    sortedData.b[0] = rawdata.b[0];
    sortedData.b[1] = rawdata.b[1];
    sortedData.b[2] = rawdata.b[2];
    sortedData.b[3] = rawdata.b[3];
    result = modbus_write_registers(m_bus, 0x0008, 2, (uint16_t*)sortedData.b);
    if (result >= 0)
    {

    }
    else
    {
        // Error
    }
}

uint32_t OpenFFUcontrolPTS::requestSerialNumber()
{
    int result = 0;
    T_uint32_conversion rawdata;
    double resistance;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    result = modbus_read_registers(m_bus, 0x000C, 2, (uint16_t*)rawdata.b);

    // Sort data according to endianess
    T_uint32_conversion sortedData;
    sortedData.b[0] = rawdata.b[0];
    sortedData.b[1] = rawdata.b[1];
    resistance = sortedData.n;
    if (result >= 0)
    {
        emit signal_receivedResistanceReference(resistance);
        return resistance;
    }
    else
    {
        // Error
        return 0;
    }
}

void OpenFFUcontrolPTS::writeSerialNumber(u_int32_t serial)
{
    int result;
    modbus_set_slave(m_bus, m_adr);
    // Bus clearance time
    QThread::msleep(100);
    // Sort data according to endianess
    T_uint32_conversion rawdata;
    rawdata.n = serial;
    T_uint32_conversion sortedData;
    sortedData.b[0] = rawdata.b[0];
    sortedData.b[1] = rawdata.b[1];
    sortedData.b[2] = rawdata.b[2];
    sortedData.b[3] = rawdata.b[3];
    result = modbus_write_registers(m_bus, 0x000C, 2, (uint16_t*)sortedData.b);
    if (result >= 0)
    {

    }
    else
    {
        // Error
    }
}
