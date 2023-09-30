#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


using namespace std;
bool bisconnected  = false;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->te_ip->setText("127.0.0.1");
    ui->te_port->setText("9906");

    webSocket = new QWebSocket();

    init_cob();

    connect(webSocket, &QWebSocket::connected, this, [=](){
        ui->te_ConnectionStatus->append(QDateTime::currentDateTime().toString() + " 连接成功");
        reconnect->stop();
        six_sec_timer->stop();
        bisconnected = true;
    });

    connect(webSocket, &QWebSocket::disconnected, this, [=](){
        webSocket->close();
        ui->te_ConnectionStatus->append("连接已断开");
        ui->b_Connect->setEnabled(true);
        bisconnected = false;
    });

    connect(webSocket, &QObject::destroyed, this, [=](){
        webSocket = NULL;
        qDebug()<<("destroyed");
    });

    connect(ui->b_Connect, &QPushButton::clicked, this, &MainWindow::connectButton_clicked);
    connect(ui->b_c0, &QPushButton::clicked, this, &MainWindow::sendCommand_c0);
    connect(ui->b_c1, &QPushButton::clicked, this, &MainWindow::sendCommand_c1);
    connect(ui->b_c4, &QPushButton::clicked, this, &MainWindow::sendCommand_c4);
    connect(ui->b_c6, &QPushButton::clicked, this, &MainWindow::sendCommand_c6);

    connect(webSocket, &QWebSocket::binaryMessageReceived, this, &MainWindow::onBinaryMessageReceived);

    six_sec_timer = new QTimer(this);
    connect(six_sec_timer, &QTimer::timeout, this, &MainWindow::onTimeOut);

    reconnect = new QTimer(this);
    connect(reconnect, &QTimer::timeout, this, &MainWindow::connectButton_clicked);


    QString path("/Users/houjintong/test/build-untitled-Desktop_Qt_5_12_12_clang_64bit-Debug/untitled.app/Contents/MacOS/untitled");
    process = new QProcess(this);
    process->start(path);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectUrl(QString url)
{
    m_url = QUrl(url);
    webSocket->open(m_url);
}

void MainWindow::connectButton_clicked()
{
    QString ip = ui->te_ip->toPlainText();
    QString port = ui->te_port->toPlainText();
    connectUrl("ws://" + ip + ":" + port); //ws://127.0.0.1:9906
}

void MainWindow::onTimeOut()
{
    qDebug() << "heart count:" << heart_count;
    qDebug() << "disconnect loop count:" << disconnect_loop_count;

    if(heart_count == 0) {
        disconnect_loop_count++;
        if(disconnect_loop_count == 3) {
            qDebug() << "Lost Connection to Server";
            disconnect_loop_count = 0;
            if(!reconnect->isActive())
                reconnect->start(2000);
        }
    } else {
        qDebug() << "Connected to Server";
        heart_count = 0;
    }
}

struct Data
{
    short int stx;
    char ver;
    QString len;
    uint16_t crc;
    int data;

    qint64 epochTime;
    int needRe;
    int timeout;
    int dev_id;
    char dev_no;
    //DATA Detail
    char cmd_type;
    char getseq()
    {
        seq = seq + 0x01;
        return seq;
    }
private:
    char seq;
};

struct FrameHeader
{
    short int stx;
    char ver;
    QString len;
    uint16_t crc;
    int data;

    qint64 epochTime;
    int needRe;
    int timeout;
    int dev_id;
    char dev_no;

    char getseq()
    {
        seq = seq + 0x01;
        return seq;
    }
private:
    char seq;
};

struct c2
{
    char cmd_type;
    char OBUID[4];
    char stop_type;
    uint16_t crc;
};

struct c4
{
    char RSCTL;
    char cmd_type;
    char controlType;
};

struct c6
{
    char cmd_type;
    int OBUID;
    QByteArray CardDicFactor;
    char TradeMode;
    QByteArray PSAMNo;
    int ConsumeMoney;
    QByteArray PurchaseTime;
    QByteArray Station;
    short int Ef040ffSet;
    short int Ef04Len;
    QByteArray Ef04Info;
};

short crctab16[] =
{
    0, 4489, 8978, 12955, 17956, 22445, 25910, 29887,
    -29624, -25151, -20646, -16685, -13716, -9243, -5762, -1801, 4225,
     264, 13203, 8730, 22181, 18220, 30135, 25662, -25399, -29376,
     -16421, -20910, -9491, -13468, -1537, -6026, 8450, 12427,
     528, 5017, 26406, 30383, 17460, 21949, -21174, -17213, -29096,
     -24623, -5266, -1305, -14212, -9739, 12675, 8202, 4753, 792,
     30631, 26158, 21685, 17724, -16949, -21438, -24871, -28848,
     -1041, -5530, -9987, -13964, 16900, 21389, 24854, 28831, 1056,
     5545, 10034, 14011, -12724, -8251, -4770, -809, -30616, -26143,
     -21638, -17677, 21125, 17164, 29079, 24606, 5281, 1320, 14259,
     9786, -8499, -12476, -545, -5034, -26391, -30368, -17413,
     -21902, 25350, 29327, 16404, 20893, 9506, 13483, 1584, 6073,
     -4274, -313, -13220, -8747, -22166, -18205, -30088, -25615, 29575,
     25102, 20629, 16668, 13731, 9258, 5809, 1848, -49, -4538, -8995,
     -12972, -17941, -22430, -25863, -29840, -31736, -27263, -22758,
     -18797, -15828, -11355, -7874, -3913, 2112, 6601, 11090, 15067,
     20068, 24557, 28022, 31999, -27511, -31488, -18533, -23022, -11603,
     -15580, -3649, -8138, 6337, 2376, 15315, 10842, 24293, 20332, 32247,
     27774, -23286, -19325, -31208, -26735, -7378, -3417, -16324, -11851,
     10562, 14539, 2640, 7129, 28518, 32495, 19572, 24061, -19061, -23550,
     -26983, -30960, -3153, -7642, -12099, -16076, 14787, 10314, 6865, 2904,
     32743, 28270, 23797, 19836, -14836, -10363, -6882, -2921, -32728, -28255,
     -23750, -19789, 19012, 23501, 26966, 30943, 3168, 7657, 12146, 16123,
     -10611, -14588, -2657, -7146, -28503, -32480, -19525, -24014, 23237,
     19276, 31191, 26718, 7393, 3432, 16371, 11898, -6386, -2425, -15332,
     -10859, -24278, -20317, -32200, -27727, 27462, 31439, 18516, 23005,
     11618, 15595, 3696, 8185, -2161, -6650, -11107, -15084, -20053, -24542,
     -27975, -31952, 31687, 27214, 22741, 18780, 15843, 11370, 7921, 3960
};

uint16_t crc16_java(uint8_t *puchMsg, uint32_t usDataLen)
{
    int16_t fcs = -1;
    int i;
    uint16_t res;

    for (i = 0; i<usDataLen; i++)
    {
        fcs = (short)(((fcs & 0xFFFF) >> 8) ^ crctab16[(fcs ^ puchMsg[i]) & 0xFF]);
    }

    fcs = (int16_t)(~fcs);

    res = fcs;
    return res;
}

QByteArray setHeader(struct FrameHeader cmd)
{
    QByteArray message;
    cmd.stx = 0xffff;
    cmd.ver = 0x00;
    char seq_cmd = cmd.getseq();
    cmd.data = 0;
    message.append(cmd.stx >> 8 & 0xff);
    message.append(cmd.stx & 0xff);
    message.append(cmd.ver);
    message.append(seq_cmd);
    message.append(cmd.data >> 24 & 0xff);
    message.append(cmd.data >> 16 & 0xff);
    message.append(cmd.data >> 8 & 0xff);
    message.append(cmd.data & 0xff);
    QDateTime dateTime = QDateTime::currentDateTime();
    cmd.epochTime = dateTime.toMSecsSinceEpoch();
    QByteArray epochTime_arr;
    epochTime_arr.resize(8);

    for (int i = 0; i < epochTime_arr.size(); i++)
    {
        epochTime_arr[i] = cmd.epochTime >> i*8 & 0xff;
    }
    message.append(epochTime_arr);

    QString dateTime_str = dateTime.toString("yyyyMMddhhmmss");
    for (int i = 0; i < dateTime_str.length(); i += 2)
    {
        QChar first = dateTime_str[i];
        QChar second = dateTime_str[i+1];
        message.append(first.digitValue() * 16 + second.digitValue());
    }

    cmd.needRe = 1;
    message.append(cmd.needRe & 0xFF);

    cmd.timeout = 100;
    message.append(QString::number(cmd.timeout,16));

    cmd.dev_id = 0;
    message.append(cmd.dev_id);

    cmd.dev_no = 0x01;
    message.append(cmd.dev_no);

    return message;
}

QByteArray setEnding(QByteArray data)
{
    int totallen = data.size() - 4;
    char ver = 0x00;
    data.insert(5,ver);
    data.insert(6,ver);
    data.insert(7,totallen >> 8 & 0xFF);
    data.insert(8,totallen & 0xFF);

    uchar msg[2048];
    memcpy(&msg, data.constData(), totallen+8);

    uint16_t crc;
    crc = crc16_java(&msg[2], totallen + 6);
    data.append(crc >> 8 & 0xFF);
    data.append(crc & 0xFF);

    return data;
}

void MainWindow::sendCommand_c0()
{
    QByteArray message;

    struct Data cmd1;
    cmd1.stx = 0xffff;
    cmd1.ver = 0x00;
    char seq_c0 = cmd1.getseq();

    message.append(cmd1.stx & 0xff);
    message.append(cmd1.stx >> 8 & 0xff);
    message.append(cmd1.ver);
    message.append(seq_c0);

    const char* data = ui->te_DATA->toPlainText().toStdString().c_str();
    message.append(data[3] >> 8 & 0xff);
    message.append(data[2] >> 8 & 0xff);
    message.append(data[1] >> 8 & 0xff);
    message.append(data[0] >> 8 & 0xff);

    QDateTime dateTime = QDateTime::currentDateTime();
    cmd1.epochTime = dateTime.toMSecsSinceEpoch();
    QByteArray epochTime_arr;
    epochTime_arr.resize(8);
    for (int i = 0; i < epochTime_arr.size(); i++)
    {
        epochTime_arr[i] = cmd1.epochTime >> i*8 & 0xff;
    }
    message.append(epochTime_arr);

    QString dateTime_str = dateTime.toString("yyyyMMddhhmmss");
    for (int i = 0; i < dateTime_str.length(); i += 2)
    {
        QChar first = dateTime_str[i];
        QChar second = dateTime_str[i+1];
        message.append(first.digitValue() * 16 + second.digitValue());
    }

    cmd1.needRe = 1;
    message.append(cmd1.needRe & 0xFF);

    cmd1.timeout = 100;
    message.append(QString::number(cmd1.timeout,16));

    cmd1.dev_id = 0;
    message.append(cmd1.dev_id);

    cmd1.dev_no = 0x01;
    message.append(cmd1.dev_no);

    cmd1.cmd_type = 0xc0;
    message.append(cmd1.cmd_type);
    //接unix time stamp
    qint64 unix_sec = dateTime.toSecsSinceEpoch();
    int seconds = int(unix_sec);
    message.append((seconds >> 24)&0xff);
    message.append((seconds >> 16)&0xff);
    message.append((seconds >> 8)&0xff);
    message.append(seconds&0xff);
    //接dateTime
    for (int i = 0; i < dateTime_str.length(); i += 2)
    {
        QChar first = dateTime_str[i];
        QChar second = dateTime_str[i+1];
        message.append(first.digitValue() * 16 + second.digitValue());
    }

    bool ok;
    QString laneMode_str = ui->cob_LaneMode->currentText();
    message.append(laneMode_str.toInt(&ok,16));

    QChar bstInt = 20;
    message.append(bstInt);

    QString txPower_str = ui->te_TxPower->toPlainText();
    message.append(txPower_str.toInt(&ok,16));

    QString pllChannelID = ui->le_channelID->text();
    message.append(pllChannelID.toInt(&ok,16));

    char transMode = 0x01;
    message.append(transMode);

    QString authMode_str = ui->cob_AuthMode->currentText();
    message.append(authMode_str.toInt(&ok,16));

    QString pt_str = ui->cob_Ef04Opt->currentText();
    message.append(pt_str.toInt(&ok,16));

    QString ffSet_str = ui->le_ef040ffset->text();
    message.append(ffSet_str.toUInt(&ok,16) >> 8 & 0xff);
    message.append(ffSet_str.toUInt(&ok,16) & 0xff);


    QString len_str = ui->le_ef04len->text();
    message.append(len_str.toUInt(&ok,16) >> 8 & 0xff);
    message.append(len_str.toUInt(&ok,16) & 0xff);


    QString rev = ui->le_reversed->text();
    message.append(rev.toInt(&ok,16));


    int totallen = message.size() - 4;
    message.insert(5,cmd1.ver);
    message.insert(6,cmd1.ver);
    message.insert(7,totallen >> 8 & 0xFF);
    message.insert(8,totallen & 0xFF);

    uchar msg[2048];
    memcpy(&msg, message.constData(), totallen+8);
    // qDebug() << message.toHex();
    // qDebug() << QByteArray((char*)msg, totallen+8).toHex();

    //insert crc values at the end
    cmd1.crc = crc16_java(&msg[2], totallen + 6);
    message.append(cmd1.crc >> 8 & 0xFF);
    message.append(cmd1.crc & 0xFF);

    const char* error("error"); //define error information
/*  拆包测试1：
    qDebug() << message.toHex();
    QByteArray first = message.left(((totallen+15)/2)+2);
    qDebug() << first.toHex();
    QByteArray sec = message.right(((totallen+15)/2)+1);
    qDebug() << sec.toHex();

    //拆包测试2:
    qDebug() << message.toHex();
    QByteArray first = message.left((totallen+15)/2);
    qDebug() << first.toHex();
    QByteArray mid= message.mid(((totallen+15)/2),3);
    qDebug() << mid.toHex();
    QByteArray last = message.right(((totallen+15)/2) - 7);
    qDebug() << last.toHex();

    //insert excess information in the middle of messages
    message.append(0xab);
    message.append(0xcd);
    message.append(0xef);
    message.prepend(error);
    */

    //send message
    if(webSocket)
        webSocket->sendBinaryMessage(message);
}

void MainWindow::sendCommand_c1()
{
    struct Data c1;
    QByteArray message;
    c1.stx = 0xFFFF;
    c1.ver = 0x00;
    char seq_c0 = c1.getseq();

    message.append(c1.stx & 0xff);
    message.append(c1.stx >> 8 & 0xff);
    message.append(c1.ver);
    message.append(seq_c0);

    const char* data = ui->te_DATA->toPlainText().toStdString().c_str();
    message.append(data[3] >> 8 & 0xff);
    message.append(data[2] >> 8 & 0xff);
    message.append(data[1] >> 8 & 0xff);
    message.append(data[0] >> 8 & 0xff);

    QDateTime dateTime = QDateTime::currentDateTime();
    c1.epochTime = dateTime.toMSecsSinceEpoch();
    QByteArray epochTime_arr;
    epochTime_arr.resize(8);
    for (int i = 0; i < epochTime_arr.size(); i++)
    {
        epochTime_arr[i] = c1.epochTime >> i*8 & 0xff;
    }
    message.append(epochTime_arr);

    QString dateTime_str = dateTime.toString("yyyyMMddhhmmss");
    for (int i = 0; i < dateTime_str.length(); i += 2)
    {
        QChar first = dateTime_str[i];
        QChar second = dateTime_str[i+1];
        message.append(first.digitValue() * 16 + second.digitValue());
    }

    c1.needRe = 1;
    message.append(c1.needRe & 0xFF);
    c1.timeout = 100;
    message.append(QString::number(c1.timeout,16));
    c1.dev_id = 0;
    message.append(c1.dev_id);
    c1.dev_no = 0x01;
    message.append(c1.dev_no);
    c1.cmd_type = 0xc1;
    message.append(c1.cmd_type&0xff);

    int OBUID_c1 = OBUID;
    message.append(OBUID_c1 >> 24 &0xff);
    message.append(OBUID_c1 >> 16 &0xff);
    message.append(OBUID_c1 >> 8 &0xff);
    message.append(OBUID_c1 &0xff);
    //车载单元一级分散因子
    double OBUDivFactor = 0;
    QByteArray OBUDivFactor_arr;
    int i;
    int length = sizeof(double);

    char* p = (char*)&OBUDivFactor;
    for (i = 0; i < length; i++)
    {
     OBUDivFactor_arr[i] = *p++;
    }

    message.append(OBUDivFactor_arr);

    int totallen = message.size() - 4;

    message.insert(5,c1.ver);
    message.insert(6,c1.ver);
    message.insert(7,totallen >> 8 & 0xFF);
    message.insert(8,totallen & 0xFF);

    uchar msg[2048];
    memcpy(&msg, message.constData(), totallen+8);

    //insert crc values at the end
    c1.crc = crc16_java(&msg[2], totallen + 6);
    message.append(c1.crc >> 8 & 0xFF);
    message.append(c1.crc & 0xFF);

    webSocket->sendBinaryMessage(message);
}

void MainWindow::sendCommand_c2()
{
    QByteArray message;
    struct FrameHeader cmd;
    message = setHeader(cmd);

    struct c2 c2;
    c2.cmd_type = 0xc2;
    message.append(c2.cmd_type);

    c2.OBUID[0] = 0x00;
    c2.OBUID[1] = 0x00;
    c2.OBUID[2] = 0x00;
    c2.OBUID[3] = 0x00;
    message.append(c2.OBUID[0]);
    message.append(c2.OBUID[1]);
    message.append(c2.OBUID[2]);
    message.append(c2.OBUID[3]);

    c2.stop_type = 0x01;
    message.append(c2.stop_type);

    message = setEnding(message);
    qDebug() << message.toHex();

    webSocket->sendBinaryMessage(message);
}

void MainWindow::sendCommand_c4()
{
    QByteArray message;
    struct FrameHeader cmd;
    message = setHeader(cmd);

    struct c4 c4;
    c4.RSCTL = 0x00;
    message.append(c4.RSCTL);
    c4.cmd_type = 0xc4;
    message.append(c4.cmd_type);
    c4.controlType = 0x01;
    message.append(c4.controlType);

    message = setEnding(message);
    if(webSocket)
        webSocket->sendBinaryMessage(message);
}

void MainWindow::sendCommand_c6()
{
    QByteArray message;
    struct FrameHeader cmd;
    message = setHeader(cmd);

    struct c6 c6;
    message.append(c6.cmd_type = 0xc6);
    c6.OBUID = OBUID;
    message.append(c6.OBUID >> 24 &0xff);
    message.append(c6.OBUID >> 16 &0xff);
    message.append(c6.OBUID >> 8 &0xff);
    message.append(c6.OBUID &0xff);
    c6.CardDicFactor.resize(8);
    c6.CardDicFactor.fill(0x00);
    message.append(c6.CardDicFactor);
    c6.TradeMode = 0x00;
    message.append(c6.TradeMode);
    c6.PSAMNo.resize(6);
    c6.PSAMNo.fill(0x00);
    message.append(c6.PSAMNo);
    c6.ConsumeMoney = initialBalance - moneyAfterDeduction;
    message.append(c6.ConsumeMoney >> 24 & 0xff);
    message.append(c6.ConsumeMoney >> 16 & 0xff);
    message.append(c6.ConsumeMoney >> 8 & 0xff);
    message.append(c6.ConsumeMoney & 0xff);
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTime_str = dateTime.toString("yyyyMMddhhmmss");
    for (int i = 0; i < dateTime_str.length(); i += 2)
    {
        QChar first = dateTime_str[i];
        QChar second = dateTime_str[i+1];
        message.append(first.digitValue() * 16 + second.digitValue());
    }
    c6.Station.resize(43);
    c6.Station.fill(0x00);
    message.append(c6.Station);
    c6.Ef040ffSet = 0x0000;
    message.append(c6.Ef040ffSet);
    c6.Ef04Len = 0x0001;
    message.append(c6.Ef04Len);
    c6.Ef04Info.resize(c6.Ef04Len);
    c6.Ef04Info.fill(0x11);
    message.append(c6.Ef04Info);

    message = setEnding(message);

    if(webSocket)
        webSocket->sendBinaryMessage(message);
}

void MainWindow::init_cob()
{
    QStringList strList;
    strList<<"3"<<"4";
    ui->cob_LaneMode->addItems(strList);
    QStringList authMode;
    authMode<<"00"<<"01";
    ui->cob_AuthMode->addItems(authMode);
    QStringList pt;
    pt<<"0"<<"1";
    ui->cob_Ef04Opt->addItems(pt);
}

struct b2
{
    char frameType;
    int OBUID;
    char error_code;
    qint64 IssueId;
    char contract_type;
    char contract_ver;
    qint64 serialNumber;
    QString dateofIssue;
    QString dateofExpire;
    char equitmentCV;
    qint16 OBUStatus;
    char authMode;
    qint64 random;
    char algorithmID;
    QByteArray VehicleInfo;
    char cardStatus;
    int cardRestMoney;
    QByteArray IssuerInfo;
    QByteArray LastStation;
    char readEf04Status;
    QByteArray Ef04Info;
    int B3CostTm;
    int B4CostTm;
    int EF04CostTm;
    QByteArray ApprovedLoad;
};

struct b5
{
    char frameType;
    int OBUID;
    char error_code;
    char PSAMNo[6];
    QString TransTime;
    char TransType;
    int TAC;
    short int ICCPayserial;
    int PSAMTranSerial;
    int CardBalance;
    char AlgorithmID;
    int MAC2;
};

void MainWindow::parse_b2(QByteArray msg)
{
    struct b2 b2;
    if (!(uchar(msg[37]&0xff) == 0x00))
        qDebug() << "error code doesn't match 0x00";
    b2.OBUID = int(((msg[33]&0xff) << 24) | ((msg[34]&0xff) << 16) | ((msg[35]&0xff) << 8) | (msg[36]&0xff));
    QString hexvalue = QString("%1").arg(b2.OBUID, 8, 16, QLatin1Char( '0' ));
    qDebug() << "OBUID: " << hexvalue;
    b2.cardRestMoney = uint(((msg[175]&0xff) << 24) | ((msg[176]&0xff) << 16) | ((msg[177]&0xff) << 8) | (msg[178]&0xff));
    qDebug() << "<卡余额> Card Rest Money: " << b2.cardRestMoney << "元";
    OBUID = b2.OBUID;
    initialBalance = b2.cardRestMoney;
}

void MainWindow::parse_b5(QByteArray msg)
{
    struct b5 b5;
    b5.OBUID = int(((msg[33]&0xff) << 24) | ((msg[34]&0xff) << 16) | ((msg[35]&0xff) << 8) | (msg[36]&0xff));
    OBUID = b5.OBUID;
    QString hexvalue = QString("%1").arg(b5.OBUID, 8, 16, QLatin1Char( '0' ));
    qDebug() << "<车载单元MAC地址> OBUID: " << hexvalue;
    b5.CardBalance = uint(((msg[62]&0xff) << 24) | ((msg[63]&0xff) << 16) | ((msg[64]&0xff) << 8) | (msg[65]&0xff));
    qDebug() << "<交易后余额> Card Balance: " << b5.CardBalance << "元";
    moneyAfterDeduction = b5.CardBalance;
}

void MainWindow::onBinaryMessageReceived(QByteArray msg)
{
    qDebug() << "客户端收到信息：" << msg.toHex();

    s.append((QByteArray) msg);
    QByteArray message = s;

    QByteArray lookingfor;
    lookingfor.append(0xff);
    lookingfor.append(0xff);

    int currentIndex = 0;

    while(message.contains(lookingfor) && (currentIndex < message.size() != 0))
    {
        int start = message.indexOf(lookingfor,0);
        message.remove(0,start);

        int len = int(((message[5]&0xff) << 24)  | ((message[6]&0xff) << 16) | ((message[7]&0xff) << 8) | ((message[8])&0xff));

        ushort crc_sent = ushort(((message[len+8]&0xff) << 8) | (message[len+9]&0xff));

        //currentIndex = index of the last member
        currentIndex = len+9;

        uchar copy[2048];
        memcpy(&copy, message.constData(), len+8);
        u_int16_t crc_val = crc16_java(&copy[2], len + 6);

        if ((crc_sent ^ crc_val) == 0x0000)
        {
            qDebug() << "crc值校验成功";

            cleanData.append(message.left(len+10));
            //qDebug() << "cleandata[0]: " << cleanData[0].toHex();

            s.remove(0,currentIndex+1);
            message.remove(0,currentIndex+1);
            //qDebug() << "s after remove: " << s.toHex();
            continue;
        }

    }

    char frame_type = char(cleanData[0].at(32)&0xff);

    switch (uint8_t(frame_type))
    {
    case 0xb0:
        {
            qDebug() << "frame_type matches B0 <设备状态信息帧>";
            break;
        }
    case 0xb2:
        {
            qDebug() << "frame_type matches B2 <信息帧>";
            char error_code = cleanData[0].at(37);
            if((uchar)error_code == 0x80) {
                qDebug() << "<Heart!!!>";
                heart_count++;
                if(!six_sec_timer->isActive())
                  six_sec_timer->start(6000);

            } else if((uchar)error_code == 0x00){
                qDebug() << "<通讯区域内存在此车载单元>";
                parse_b2(cleanData[0]);
                sendCommand_c1();

            } else if ((uchar)error_code == 0xff){
                qDebug() << "<测试帧>";
                sendCommand_c2();
            }
            break;
        }
    case 0xb5:
        {
            qDebug() << "frame_type matches B5 <交易信息帧>";
            parse_b5(cleanData[0]);
            sendCommand_c6();
            break;
        }
    }
    cleanData.removeFirst();
}
