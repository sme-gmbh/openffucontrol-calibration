#ifndef AGILENT34970A_H
#define AGILENT34970A_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

class Agilent34970A : public QObject
{
    Q_OBJECT
public:
    explicit Agilent34970A(QObject *parent = nullptr);

    bool open(QString portName);
    void close();
    bool configure();

private:
    QSerialPort* m_port;
    void sendCommand(QString command);
    QByteArray rxBuffer;
    void delay(int milliseconds);

signals:
    void signal_receivedRaw(QString data);

public slots:
    bool slot_read();

private slots:
    void slot_readyRead();
};

#endif // AGILENT34970A_H
