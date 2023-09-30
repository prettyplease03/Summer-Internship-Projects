#include "RobotDataParse.h"
#include "GlobalDefine.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

RobotDataParse::RobotDataParse()
{

}

RobotDataParse::RobotDataParse(const RobotDataParse&)
{

}

RobotDataParse::~RobotDataParse()
{

}

QString RobotDataParse::generateHeartMsg()
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("devUniqueID", "Tr600-20221210123");

    jsonObj.insert("cmd", 201);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateLogin(QString strUser, QString strPwsd)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("account", strUser);
    bizcontent.insert("password", strPwsd);

    jsonObj.insert("cmd", 202);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateSetDevUniqueId(QString strId)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("devUniqueID", strId);

    jsonObj.insert("cmd", 203);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateSetNICIp(QString strIp1, QString strMask1, QString strIp2, QString strMask2)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("ip1", strIp1);
    bizcontent.insert("mask1", strMask1);
    bizcontent.insert("ip2", strIp2);
    bizcontent.insert("mask2", strMask2);

    jsonObj.insert("cmd", 204);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateTriggerTest(int nNo)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("triggerNo", nNo);

    jsonObj.insert("cmd", 301);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateRelayTest(int nNo, int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("relayNo", nNo);
    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 302);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateDisplayTest(QStringList strlist, int nColor)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;
    QJsonArray jsonArray;

    bizcontent.insert("fullColour", nColor);
    for(int i=0; i<strlist.size(); i++)
    {
        QJsonObject text;
        text.insert("lineNo", i+1);
        text.insert("text", strlist.at(i));
        jsonArray.append(text);
    }
    bizcontent.insert("displayText", jsonArray);

    jsonObj.insert("cmd", 303);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateVolumeTest(int nVolume)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("volume", nVolume);

    jsonObj.insert("cmd", 304);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateRadarTest(int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 305);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateRailingTest(int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 306);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateLightTest(int nIndex, int nOperation, int nValue)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("index", nIndex);
    bizcontent.insert("operation", nOperation);
    bizcontent.insert("pwmValue", nValue);

    jsonObj.insert("cmd", 307);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateRobotDisplayTest(QStringList strlist, int nColor)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;
    QJsonArray jsonArray;

    bizcontent.insert("fullColour", nColor);
    for(int i=0; i<strlist.size(); i++)
    {
        QJsonObject text;
        text.insert("column", i+1);
        text.insert("text", strlist.at(i));
        jsonArray.append(text);
    }
    bizcontent.insert("displayText", jsonArray);

    jsonObj.insert("cmd", 401);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateCtrlLightTest(int nIndex, int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("index", nIndex);
    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 402);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateNetTest(int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 403);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateAudioTest(int nVolume, int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("operation", nOperation);
    if(0 == nOperation || 2 == nOperation)
        bizcontent.insert("volume", nVolume);

    jsonObj.insert("cmd", 404);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateCameraTest(int nIndex, int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("index", nIndex);
    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 405);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateTelescopicArmTest(int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 406);

    jsonObj.insert("bizContent", bizcontent);


    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateReaderTest(int nIndex, int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    if(0 != nOperation && 1 != nOperation)
        bizcontent.insert("index", nIndex);
    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 407);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateEntryMachineTest(int nIndex, int nOperation, int nCardNum)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("index", nIndex);
    bizcontent.insert("operation", nOperation);
    if(5 == nOperation)
        bizcontent.insert("cardNum", nCardNum);

    jsonObj.insert("cmd", 408);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateExitMachineTest(int nIndex, int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("index", nIndex);
    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 409);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateInputSignalTest(int nSignalNo)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("signalNo", nSignalNo);

    jsonObj.insert("cmd", 410);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateRelayRobotTest(int nRelayNo, int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("relayNo", nRelayNo);
    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 411);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateScanerTest(int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 412);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateEntryBurnTest(int nIndex, int nOperation, int nCardNum)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("index", nIndex);
    bizcontent.insert("operation", nOperation);
    if(3 == nOperation)
        bizcontent.insert("cardNum", nCardNum);

    jsonObj.insert("cmd", 413);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}

QString RobotDataParse::generateExitBurnTest(int nIndex, int nOperation)
{
    QJsonObject jsonObj;
    QJsonObject bizcontent;

    bizcontent.insert("index", nIndex);
    bizcontent.insert("operation", nOperation);

    jsonObj.insert("cmd", 414);
    jsonObj.insert("bizContent", bizcontent);

    JSONWRITE(jsonObj);
    return strResult;
}














