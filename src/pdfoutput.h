#ifndef PDFOUTPUT_H
#define PDFOUTPUT_H

#include <QObject>
#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrintPreviewWidget>
#include <QPdfWriter>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextTableCell>
#include <QTextTableCellFormat>
#include <QPixmap>
#include <QImage>
#include <QFont>
#include <QFontInfo>
#include <QFontDatabase>
#include <QPen>
#include <QBrush>
#include <QDateTime>
#include <QMessageBox>
#include <qmath.h>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>


class PDFoutput : public QObject
{
    Q_OBJECT
public:
    explicit PDFoutput(QObject *parent = nullptr);

    typedef struct {
        QString operatorName;
        QString master;
        quint32 serialNumber;
        quint16 slaveID;
        double temperature_calibratedAt;
        double temperature_offset;
        double resistance_reference;
        double resistance_r304;

    } CalibrationData;

    const QString &filename() const;

private:
    QPrinter* pdfWriter;
    QString m_operator;
    QString m_filename;

    unsigned long m_currentPage;    // aktuelle Seitennummer
    unsigned long m_pages;   // gesamte Seitenzahl
    unsigned long m_currentLine; // aktuelle Zeile

    QFont font_mini;
    QFont font_std;
    QFont font_mono;
    QFont font_big;
    QFont font_veryBig;
    QFont font_small;
    QFont font_bold;
    QFont font_sme;
    QFont font_dokumentbezeichnung;
    QFont font_copyright;

    QPen pen_std;
    QPen pen_Bold;
    QPen pen_dot;
    QPen pen_sme;

    void paintTemplatePaper(QPainter* painter, QString ueberschrift);
    int paintCalData(QPainter* painter, CalibrationData calData);

    int mm_to_pixel(double mm);
    QRect pageRect();
    void newPage();

signals:

public slots:
    void printCalibrationProtocol(CalibrationData calData);
    void setFilename(QString filename);

};

#endif // PDFOUTPUT_H
