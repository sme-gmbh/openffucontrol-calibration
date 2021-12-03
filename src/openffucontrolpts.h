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

private:
    QString m_interface;
    modbus_t *m_bus;
    quint16 m_adr;

signals:
    void signal_receivedMeasuredTemperature(double temperature);
    void signalReceivedCalibrationOffset(double offset);
};

#endif // OPENFFUCONTROLPTS_H
