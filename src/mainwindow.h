#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "agilent34970a.h"
#include "openffucontrolpts.h"
#include "pdfoutput.h"
#include <modbus/modbus-rtu.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void openModbus(QString m_interface);
    void closeModbus();

private slots:
    void on_actionConnect_Agilent_triggered();

    void on_actionRead_active_triggered(bool checked);

    void slot_agilentGotData(QString data);

    void on_actionConnect_PTS_triggered(bool checked);

    void slot_readPTS();

    void on_pushButton_PTS2_calibrate_clicked();

    void on_pushButton_PTS2_setSerialNumber_clicked();

    void on_pushButton_PTS2_setReference_clicked();

    void on_pushButton_PTS2_setTestResistorR304_clicked();

private:
    Ui::MainWindow *ui;
    Agilent34970A m_agilent;
    QTimer m_agilentReadTimer;
    QTimer m_PTSreadTimer;

    PDFoutput m_pdf;

    modbus_t *m_bus;

    OpenFFUcontrolPTS *pts_1;
    OpenFFUcontrolPTS *pts_2;

};
#endif // MAINWINDOW_H
