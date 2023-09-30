#include "Mainwindow.h"
#include <QApplication>
#include <QMutex>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QTextCodec>

static QMutex g_mutex;

//void outputMessage(QtMsgType type, const char* data)
void outputMessage(QtMsgType type, const QMessageLogContext& Context, const QString &data )
{
    if(g_mutex.tryLock(10)==false)
    {
        return;
    }

    QString strData(data);
    QString strLevel;
    int outputLevel = 0;

    switch(type)
    {
    case QtDebugMsg:
    {
        strLevel = QString("[Debug]");
        outputLevel = 5;
        break;
    }
    case QtInfoMsg:
    {
        strLevel = QString("[Info] ");
        outputLevel = 4;
        break;
    }
    case QtWarningMsg:
    {
        strLevel = QString("[Warn] ");
        outputLevel = 3;
        break;
    }
    case QtCriticalMsg:
    {
        strLevel = QString("[Critical]");
        outputLevel = 2;
        break;
    }
    case QtFatalMsg:
    {
        strLevel = QString("[Fatal] ");
        outputLevel = 1;
        break;
    }
    }

    if(outputLevel <= 0)
        return;

    QString strCurrentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString strCurrentData = QString("[%1]").arg(strCurrentDateTime);
    QString strMessage = QString("%1 %2 %3").arg(strCurrentData).arg(strLevel).arg(strData);

    QString strPath = QApplication::applicationDirPath() + "/log/RobotTestToolLog";
    QString strFileTime = QDate::currentDate().toString("yyyyMMdd");
    QString strFileName  = QObject::tr("%1%2.log").arg(strPath).arg(strFileTime);


    QFile file(strFileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << strMessage << "\r\n";
    file.flush();
    file.close();
    g_mutex.unlock();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    //qInstallMessageHandler(outputMessage);

    Mainwindow w;
    w.show();
    return a.exec();
}
