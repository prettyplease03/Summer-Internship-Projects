#ifndef ROBOTDATAPARSE_H
#define ROBOTDATAPARSE_H

#include <QObject>

class RobotDataParse : public QObject
{
    Q_OBJECT

public:
    QString generateHeartMsg();
    QString generateLogin(QString strUser, QString strPwsd);
    QString generateSetDevUniqueId(QString strId);
    QString generateSetNICIp(QString strIp1, QString strMask1, QString strIp2, QString strMask2);
    QString generateTriggerTest(int nNo);
    QString generateRelayTest(int nNo, int nOperation);
    QString generateDisplayTest(QStringList strlist, int nColor);
    QString generateVolumeTest(int nVolume);
    QString generateRadarTest(int nOperation);
    QString generateRailingTest(int nOperation);
    QString generateLightTest(int nIndex, int nOperation, int nValue);
    QString generateRobotDisplayTest(QStringList strlist, int nColor);
    QString generateCtrlLightTest(int nIndex, int nOperation);
    QString generateNetTest(int nOperation);
    QString generateAudioTest(int nVolume, int nOperation);
    QString generateCameraTest(int nIndex, int nOperation);
    QString generateTelescopicArmTest(int nOperation);
    QString generateReaderTest(int nIndex, int nOperation);
    QString generateEntryMachineTest(int nIndex, int nOperation, int nCardNum);
    QString generateExitMachineTest(int nIndex, int nOperation);
    QString generateInputSignalTest(int nSignalNo);
    QString generateRelayRobotTest(int nRelayNo, int nOperation);
    QString generateScanerTest(int nOperation);
    QString generateEntryBurnTest(int nIndex, int nOperation, int nCardNum);
    QString generateExitBurnTest(int nIndex, int nOperation);

public:
    static RobotDataParse* getInstance()
    {
        static RobotDataParse m_instance;
        return &m_instance;
    }
    ~RobotDataParse();

private:
    RobotDataParse();
    RobotDataParse(const RobotDataParse&);
    RobotDataParse& operator=(RobotDataParse const&);
};

#endif // ROBOTDATAPARSE_H
