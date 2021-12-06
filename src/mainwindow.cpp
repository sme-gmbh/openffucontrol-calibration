#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_agilentReadTimer.setInterval(2000);
    connect(&m_agilentReadTimer, SIGNAL(timeout()), &m_agilent, SLOT(slot_read()));
    connect(&m_agilent, SIGNAL(signal_receivedRaw(QString)), this, SLOT(slot_agilentGotData(QString)));
    m_PTSreadTimer.setInterval(2000);
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

    struct timeval response_timeout;
    response_timeout.tv_sec = 1;
    response_timeout.tv_usec = 0;
    modbus_set_response_timeout(m_bus, &response_timeout);

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
        openModbus("/dev/ttyUSB0");
    else
        closeModbus();
}

void MainWindow::slot_readPTS()
{
    pts_1->setSlaveAddress((int)ui->doubleSpinBox_PTS1_address->value());
    pts_2->setSlaveAddress((int)ui->doubleSpinBox_PTS2_address->value());

    uint32_t serial_1 = pts_1->requestSerialNumber();
    uint32_t serial_2 = pts_2->requestSerialNumber();

    ui->doubleSpinBox_PTS1_serialNumber->setValue(serial_1);
    ui->doubleSpinBox_PTS2_serialNumber->setValue(serial_2);

    double temp_1 = pts_1->requestMeasuredTemperature();
    double temp_2 = pts_2->requestMeasuredTemperature();

    ui->doubleSpinBox_PTS1_temp->setValue(temp_1);
    ui->doubleSpinBox_PTS2_temp->setValue(temp_2);

    double offset_1 = pts_1->requestCalibrationOffset();
    ui->doubleSpinBox_PTS1_offset->setValue(offset_1);

    double offset_2 = pts_2->requestCalibrationOffset();
    ui->doubleSpinBox_PTS2_offset->setValue(offset_2);

    double resistance_reference_1 = pts_1->requestResistanceReference();
    double resistance_reference_2 = pts_2->requestResistanceReference();
    ui->doubleSpinBox_PTS1_ResistorReference->setValue(resistance_reference_1);
    ui->doubleSpinBox_PTS2_ResistorReference->setValue(resistance_reference_2);

    double resistance_R304_1 = pts_1->requestResistanceR304();
    double resistance_R304_2 = pts_2->requestResistanceR304();
    ui->doubleSpinBox_PTS1_ResistorR304->setValue(resistance_R304_1);
    ui->doubleSpinBox_PTS2_ResistorR304->setValue(resistance_R304_2);
}

void MainWindow::on_pushButton_PTS2_calibrate_clicked()
{
    double offset = ui->doubleSpinBox_PTS1_temp->value() - ui->doubleSpinBox_PTS2_temp->value();
//    pts_2->setSlaveAddress((int)ui->doubleSpinBox_PTS2_address->value());
//    pts_2->writeCalibrationOffset(offset);

    m_pdf.setFilename("/home/pdiener/calibration/PTS-calibration-" + QString().sprintf("%06d", (int)ui->doubleSpinBox_PTS2_serialNumber->value()) + ".pdf");
    PDFoutput::CalibrationData calData;

    calData.operatorName = "Peter Diener";
    calData.master = "PTS #" + QString().setNum((int)ui->doubleSpinBox_PTS1_serialNumber->value()) + " @" + QString().sprintf("%.5lf °C",
                                                                                                                              ui->doubleSpinBox_PTS1_offset->value());
    calData.serialNumber = ui->doubleSpinBox_PTS2_serialNumber->value();
    calData.slaveID = ui->doubleSpinBox_PTS2_address->value();
    calData.temperature_calibratedAt = ui->doubleSpinBox_PTS1_temp->value();
    calData.temperature_offset = offset;
    calData.resistance_reference = ui->doubleSpinBox_PTS2_ResistorReference->value();
    calData.resistance_r304 = ui->doubleSpinBox_PTS2_ResistorR304->value();

    m_pdf.printCalibrationProtocol(calData);
    QDesktopServices::openUrl(QUrl(m_pdf.filename()));
}

void MainWindow::on_pushButton_PTS2_setSerialNumber_clicked()
{
    bool ok;
    uint32_t serialNumber_new = QInputDialog::getInt(this, "New serial Number for PTS 2", "Serial #", 1, 0, 1000000, 1, &ok);
    if (ok)
        pts_2->writeSerialNumber(serialNumber_new);
}


void MainWindow::on_pushButton_PTS2_setReference_clicked()
{
    bool ok;
    double resistance = QInputDialog::getDouble(this, "Set Reference Resistor", "R=", 5000.0, 0.0, 10000.0, 5, &ok);
    if (ok)
        pts_2->writeResistanceReference(resistance);
}



void MainWindow::on_pushButton_PTS2_setTestResistorR304_clicked()
{
    bool ok;
    double resistance = QInputDialog::getDouble(this, "Set Test Resistor R304", "R=", 100.0, 0.0, 10000.0, 5, &ok);
    if (ok)
        pts_2->writeResistanceR304(resistance);
}

