#ifndef OPENFFUCONTROLPTS_H
#define OPENFFUCONTROLPTS_H

#include <QObject>
#include <QThread>
#include <modbus/modbus-rtu.h>


class OpenFFUcontrolPTS : public QObject
{
    Q_OBJECT
public:
    explicit OpenFFUcontrolPTS(QObject *parent, modbus_t* bus);
    ~OpenFFUcontrolPTS();

    void setSlaveAddress(quint16 adr);

    double requestMeasuredTemperature();
    double requestCalibrationOffset();
    void writeCalibrationOffset(double offset);
    double requestResistanceReference();
    void writeResistanceReference(double r);
    double requestResistanceR304();
    void writeResistanceR304(double r);
    uint32_t requestSerialNumber();
    void writeSerialNumber(u_int32_t serial);

private:
    typedef union{
        double d;
        uint8_t b[8];
    } T_double_conversion;

    typedef union{
        float f;
        uint8_t b[4];
    } T_float_conversion;

    typedef union{
        uint32_t n;
        uint8_t b[4];
    } T_uint32_conversion;

    QString m_interface;
    modbus_t *m_bus;
    quint16 m_adr;

signals:
    void signal_receivedMeasuredTemperature(double temperature);
    void signal_receivedCalibrationOffset(double offset);
    void signal_receivedResistanceReference(double r);
    void signal_receivedResistanceR304(double r);
    void signal_receivedSerialNumber(u_int32_t serial);
};

#endif // OPENFFUCONTROLPTS_H
