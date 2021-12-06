#include "pdfoutput.h"

// If defined draw text block rectangles to show their position
//#define DEBUGRECTS

PDFoutput::PDFoutput(QObject *parent) :
    QObject(parent)
{
    pdfWriter = NULL;

    font_mini.setFamily("Arial");
    font_mini.setPointSize(7);
    font_mini.setLetterSpacing(QFont::PercentageSpacing, 98.0);

    font_std.setFamily("Arial");
    font_std.setPointSize(10);
    font_std.setLetterSpacing(QFont::PercentageSpacing, 98.0);

    font_mono.setFamily("Nimbus mono");
    font_mono.setPointSize(10);
    font_mono.setLetterSpacing(QFont::PercentageSpacing, 88.0);

    font_big.setFamily("Arial");
    font_big.setPointSize(12);
    font_big.setBold(true);
    font_big.setLetterSpacing(QFont::PercentageSpacing, 98.0);

    font_veryBig.setFamily("Arial");
    font_veryBig.setPointSize(14);
    font_veryBig.setBold(true);
    font_veryBig.setLetterSpacing(QFont::PercentageSpacing, 98.0);

    font_small.setFamily("Arial");
    font_small.setPointSize(6);
    font_small.setLetterSpacing(QFont::PercentageSpacing, 98.0);

    font_bold.setFamily("Arial");
    font_bold.setPointSize(10);
    font_bold.setBold(true);
    font_bold.setLetterSpacing(QFont::PercentageSpacing, 98.0);

    font_sme.setFamily("Arial");
    font_sme.setPointSize(36);
    font_sme.setBold(true);
    font_sme.setLetterSpacing(QFont::PercentageSpacing, 98.0);

    font_dokumentbezeichnung.setFamily("Arial");
    font_dokumentbezeichnung.setPointSize(11);
    font_dokumentbezeichnung.setBold(true);
    font_dokumentbezeichnung.setLetterSpacing(QFont::PercentageSpacing, 98.0);

    font_copyright.setFamily("Arial");
    font_copyright.setPointSize(5);
    font_copyright.setLetterSpacing(QFont::PercentageSpacing, 96.5);

    pen_std.setColor(Qt::black);
    pen_std.setWidth(2);

    pen_Bold.setColor(Qt::black);
    pen_Bold.setJoinStyle(Qt::MiterJoin);
    pen_Bold.setWidth(12);

    pen_dot.setColor(Qt::darkGray);
    pen_dot.setWidth(5);
    pen_dot.setStyle(Qt::CustomDashLine);
    QVector<qreal> dashes;
    qreal space = 6;
    dashes << 3 << space;
    pen_dot.setDashPattern(dashes);

    pen_sme.setColor(QColor(127, 0, 127));
    pen_sme.setWidth(0);
}

const QString &PDFoutput::filename() const
{
    return m_filename;
}

void PDFoutput::setFilename(QString filename)
{
    m_filename = filename;
    if (pdfWriter != NULL)
        delete pdfWriter;

    pdfWriter = new QPrinter(QPrinter::HighResolution);
    pdfWriter->setOutputFormat(QPrinter::PdfFormat);
    pdfWriter->setOutputFileName(filename);
    pdfWriter->setPageSize(QPdfWriter::A4);
    pdfWriter->setOrientation(QPrinter::Portrait);
    pdfWriter->setResolution(600); // DPI
    pdfWriter->setFullPage(false);
}

int PDFoutput::mm_to_pixel(double mm)
{
    // Always 600 dpi printing
    return (600.0 * mm / 25.4);
}

QRect PDFoutput::pageRect()
{
    QRect rect;

    rect.setWidth(mm_to_pixel(210.0));
    rect.setHeight(mm_to_pixel(297.0));

    return rect;
}

void PDFoutput::newPage()
{
    pdfWriter->newPage();
    m_currentPage++;
}

void PDFoutput::printCalibrationProtocol(CalibrationData calData)
{
    QPainter painter;
    QRect pageRect = this->pageRect();

    m_currentPage = 1;
    m_pages = 1;
    m_operator = calData.operatorName;

    painter.begin(pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    // printing
    this->paintCalData(&painter, calData);

    painter.end();
}

void PDFoutput::paintTemplatePaper(QPainter *painter, QString ueberschrift)
{
    QRect pageRect = this->pageRect();
    int section = (pageRect.width() - 400) / 100;
    QRect tmpRect;

    // SME Logo
    tmpRect.setWidth(section * 100);
    tmpRect.setHeight(400);
    tmpRect.moveTopLeft(QPoint(200, 200));

    painter->setPen(pen_sme);
    painter->setFont(font_sme);
    painter->drawText(tmpRect.adjusted(40,0,0,0), Qt::AlignTop | Qt::AlignLeft, "SME");

    // Dokumentbezeichnung
    painter->setPen(pen_std);
    painter->setFont(font_dokumentbezeichnung);

    painter->drawText(tmpRect.adjusted(0,0,-40,0), Qt::AlignTop | Qt::AlignRight, ueberschrift);

    painter->drawLine(tmpRect.bottomLeft(), tmpRect.bottomRight());


    // Copyright
//    painter->setFont(font_copyright);
//    painter->drawText(tmpRect.adjusted(50,-50,0,-50), Qt::AlignBottom | Qt::AlignLeft, "openFFUcontrol");


    // Copyright, Seitenzahlen
    tmpRect.setWidth(section * 100);
    tmpRect.setHeight(300);
    tmpRect.moveTopLeft(QPoint(200, pageRect.height()-500));

    painter->drawLine(tmpRect.topLeft(), tmpRect.topRight());

    painter->setPen(pen_std);
    painter->setFont(font_mini);
    QRectF boundingRect;
    painter->drawText(tmpRect.adjusted(40,0,0,0), Qt::AlignVCenter | Qt::AlignLeft,
                      "File: \n\n",
                      &boundingRect);

    painter->drawText(tmpRect.adjusted(boundingRect.width() + 80, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft,
                      QFileInfo(m_filename).fileName() + "\n\n");

    QString username = "Calibration Operator: ";

    if (!this->m_operator.isEmpty())
    {
        username += this->m_operator.trimmed();
    }
    else
    {
        #ifdef Q_OS_UNIX
            username += qgetenv("USER");
        #endif
        #ifdef Q_WS_WIN
            username += qgetenv("USERNAME");
        #endif
        #ifdef Q_OS_WIN
            username += qgetenv("USERNAME");
        #endif
    }


    int numDigits = 1 + log10(m_pages);
    painter->drawText(tmpRect.adjusted(0,0,-40,0), Qt::AlignVCenter | Qt::AlignRight,
                      username + "\n" +
                      "Date/Time " + QDateTime::currentDateTime().toString("dd.MM.yyyy', 'hh:mm:ss") + "\n" +
                      QString().sprintf(" Page %0*lu of %0*lu", numDigits, m_currentPage, numDigits, m_pages));
}

int PDFoutput::paintCalData(QPainter* painter, CalibrationData calData)
{
    this->paintTemplatePaper(painter, " \nPrecision Temperature Sensor\nCalibration Protocol");

    QRect pageRect = this->pageRect();
    int section = (pageRect.width() - 400) / 100;
    QRect tmpRect;

    tmpRect.setWidth(section * 14);
    tmpRect.setHeight(mm_to_pixel(10.0));
    tmpRect.moveTopLeft(QPoint(200, mm_to_pixel(40.0)));

    painter->setFont(font_bold);

    tmpRect.setRight(200 + section * 100);

#ifdef DEBUGRECTS
    painter->drawRect(tmpRect);
#endif
    painter->drawText(tmpRect.adjusted(40,0,0,0), Qt::AlignVCenter | Qt::AlignLeft, "Sensor Serial Number " + QString().setNum(calData.serialNumber));

    painter->setFont(font_mono);

    tmpRect.moveTop(mm_to_pixel(50.0));
    tmpRect.setBottom(pageRect.height() - 600);

    QRectF boundingRect;

#ifdef DEBUGRECTS
    painter->drawRect(tmpRect);
#endif

    // Langtext
    int y_langtext = 0;
    QString langtext;

    langtext += "Calibration operator name:        " + calData.operatorName + "\n";
    langtext += "Calibration temperature master:   " + calData.master + "\n";
    langtext += "Modbus slave ID:                  " + QString().sprintf("%10d", calData.slaveID) + "\n";
    langtext += "AD converter reference resistor:  " + QString().sprintf("%10.5lf Ohms", calData.resistance_reference) + "\n";
    langtext += "AD converter test resistor R304:  " + QString().sprintf("%10.5lf Ohms", calData.resistance_r304) + "\n";
    langtext += "Calibrated at temperature:        " + QString().sprintf("%10.5lf °C", calData.temperature_calibratedAt) + "\n";
    langtext += "Calibrated offset:                " + QString().sprintf("%10.5lf °C", calData.temperature_offset) + "\n";

    QStringList langtext_lineList = langtext.split('\n');
    foreach (QString line, langtext_lineList)
    {
        painter->drawText(tmpRect.adjusted(40, y_langtext, 0, 0),
                          Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap | Qt::TextDontClip | Qt::TextExpandTabs,
                          line,
                          &boundingRect);
        if (!boundingRect.isNull())
            y_langtext += boundingRect.height();
    }

    return 0;
}
