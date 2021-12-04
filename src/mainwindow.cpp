#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_agilentReadTimer.setInterval(2000);
    connect(&m_agilentReadTimer, SIGNAL(timeout()), &m_agilent, SLOT(slot_read()));
    connect(&m_agilent, SIGNAL(signal_receivedRaw(QString)), this, SLOT(slot_agilentGotData(QString)));
    m_PTSreadTimer.setInterval(1000);
    connect(&m_PTSreadTimer, SIGNAL(timeout()), this, SLOT(slot_readPTS()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openModbus(QString m_interface)
{
    QByteArray interface_ba = m_interface.toLocal8Bit();
    m_bus = modbus_new_rtu(interface_ba.data(), 115200, 'N', 8, 2);
    if (m_bus == NULL) {
        //        emit signal_newEntry(LogEntry::Error, "EbmModbus", "Unable to open interface");
        return;
    }

    if (modbus_connect(m_bus) == -1) {
        //        emit signal_newEntry(LogEntry::Info, "EbmModbus", QString("Unable to connect to device: ") + QString(modbus_strerror(errno)));
        modbus_free(m_bus);
        return;
    }

    pts_1 = new OpenFFUcontrolPTS(this, m_bus);
    pts_2 = new OpenFFUcontrolPTS(this, m_bus);
    m_PTSreadTimer.start();
    fprintf(stderr, "EbmModbus::open(): Modbus interface configured and connected.\n");
}

void MainWindow::closeModbus()
{
    m_PTSreadTimer.stop();
    delete pts_1;
    delete pts_2;
    modbus_close(m_bus);
    modbus_free(m_bus);
}


void MainWindow::on_actionConnect_Agilent_triggered()
{
    m_agilent.open("/dev/ttyS0");
    m_agilent.configure();
}

void MainWindow::on_actionRead_active_triggered(bool checked)
{
    if (checked)
        m_agilentReadTimer.start();
    else
        m_agilentReadTimer.stop();
}

void MainWindow::slot_agilentGotData(QString data)
{
    ui->plainTextEdit_agilentResponse->appendPlainText(data);
}

void MainWindow::on_actionConnect_PTS_triggered(bool checked)
{
    if (checked)
        openModbus("/dev/ttyS0");
    else
        closeModbus();
}

void MainWindow::slot_readPTS()
{
    pts_1->setSlaveAddress((int)ui->doubleSpinBox_PTS1_address->value());
//    pts_2->setSlaveAddress((int)ui->doubleSpinBox_PTS2_address->value());
    double temp_1 = pts_1->requestMeasuredTemperature();
//    double temp_2 = pts_2->requestMeasuredTemperature();

    ui->doubleSpinBox_PTS1_temp->setValue(temp_1);
//    ui->doubleSpinBox_PTS2_temp->setValue(temp_2);
}

