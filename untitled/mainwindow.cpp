#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->le_OBUID->setText("1234");
    ui->le_cardrestmon->setText("450");
    ui->le_cardbalance->setText("400");
    QStringList strList;
    strList<<"00"<<"80"<<"ff";
    ui->cob_errorCode->addItems(strList);

    WebSocketServer = new QWebSocketServer(QStringLiteral("testServer"), QWebSocketServer::NonSecureMode, this);
    webSocket = NULL;

    connect(WebSocketServer, &QWebSocketServer::newConnection, this, &MainWindow::onNewConnection);
    connect(WebSocketServer, &QWebSocketServer::destroyed, this, [=](){
        webSocket = NULL;
        qDebug()<<("destroyed");
    });

    WebSocketServer->listen(QHostAddress::Any,9906);
    connect(ui->PressButton, &QPushButton::clicked, this, &MainWindow::pushButton_1_clicked);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::sendB2);

    connect(ui->b_sendB2, &QPushButton::clicked, this, &MainWindow::sendB2);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onNewConnection()
{
    qDebug() << "onNewConnection";
    webSocket = WebSocketServer->nextPendingConnection();
    connect(webSocket, &QWebSocket::binaryMessageReceived, this, &MainWindow::onBinaryMessageReceived);
    connect(webSocket, &QWebSocket::disconnected, this, &MainWindow::onDisconnection);
    ui->SmallWin1->append(QDateTime::currentDateTime().toString() + " :有新连接进来了：" + webSocket->localAddress().toString());
}

void MainWindow::take(QAbstractSocket::SocketError error)
{
    return;
}

void MainWindow::pushButton_1_clicked()
{
    QString msg = ui->SmallWin2->toPlainText();
    webSocket->sendTextMessage(msg);
    ui->SmallWin2->append(QDateTime::currentDateTime().toString() + "服务端给客户端发送消息：" + msg.toUtf8().toHex());

}

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

struct cmd0
{
   char cmdType;
   int seconds;
   QString dateTime;
   char laneMode;
   char BST_Interval;
   char TxPower;
   char PLL_ChannelID;
   char TransMode;
   char AuthMode;
   char Ef040pt;
   short int Ef040ffSet;
   short int Ef04Len;
   char reserved;
};

struct cmd1
{
    char cmdType;
    int OBUID;
    double OBUDivFactor;
};

struct cmd6
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

struct b0
{
    char frameType;
    char RSUStatus;
    char PSAMNum;
    QString PSAMVer1_Term[6];
    char RSUAlgId;
    char RSUManuID;
    uchar RSUID[3];
    short int RSUVersion;
    char Authorization;
    char workstatus;
    int Reserved;


};

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

void MainWindow::parseC0(QByteArray command_0)
{
    struct cmd0 cmd0;
    cmd0.cmdType = command_0[32];
    cmd0.seconds = (command_0[33]&0xff << 24) | (command_0[34]&0xff << 16) | (command_0[35]&0xff << 8) | (command_0[36]&0xff);
    for (int i = 0;i < 7;i++)
    {
        cmd0.dateTime[i] = command_0[37+i];
        qDebug() << cmd0.dateTime[i];
    }
    cmd0.laneMode = command_0[44]&0xff;
    cmd0.BST_Interval = command_0[45]&0xff;
    cmd0.TxPower = command_0[46]&0xff;
    cmd0.PLL_ChannelID = command_0[47]&0xff;
    cmd0.TransMode = command_0[48]&0xff;
    cmd0.AuthMode = command_0[49]&0xff;
    cmd0.Ef040pt = command_0[50]&0xff;
    cmd0.Ef040ffSet = ((command_0[51]&0xff << 8) |(command_0[52]&0xff));
    cmd0.Ef04Len = ((command_0[53]&0xff << 8) | (command_0[54]&0xff));
    cmd0.reserved = command_0[55]&0xff;

}

void MainWindow::parseC1(QByteArray command_1)
{
    struct cmd1 cmd;
    cmd.cmdType = command_1[32];
    cmd.OBUID = int(((command_1[33]&0xff) << 24) | ((command_1[34]&0xff) << 16) | ((command_1[35]&0xff) << 8) | (command_1[36]&0xff));
    OBUID = cmd.OBUID;
    QString hexvalue = QString("%1").arg(cmd.OBUID, 8, 16, QLatin1Char( '0' ));
    qDebug() << "OBUID: " << hexvalue;
}

void MainWindow::parseC6(QByteArray command_6)
{
    struct cmd6 c6;
    c6.OBUID = int(((command_6[33]&0xff) << 24) | ((command_6[34]&0xff) << 16) | ((command_6[35]&0xff) << 8) | (command_6[36]&0xff));
    QString hexvalue = QString("%1").arg(c6.OBUID, 8, 16, QLatin1Char( '0' ));
    qDebug() << "OBUID: " << hexvalue;
    c6.ConsumeMoney = int(((command_6[52]&0xff) << 24) | ((command_6[53]&0xff) << 16) | ((command_6[54]&0xff) << 8) | (command_6[55]&0xff));
    qDebug() << "<扣款额> Consume Money: " << c6.ConsumeMoney << "元";
}

void MainWindow::sendB0()
{
    QByteArray message;
    struct FrameHeader cmd;
    message = setHeader(cmd);
    struct b0 b0;

    b0.frameType = 0xb0;
    message.append(b0.frameType);
    b0.RSUStatus = 0x00;
    message.append(b0.RSUStatus);
    b0.PSAMNum = 0x01;
    message.append(b0.PSAMNum);
    b0.PSAMVer1_Term->fill(0x00);
    while(b0.PSAMNum > 0)
    {
        char PSAMVer = 0x00;
        message.append(PSAMVer);
        message.append(b0.PSAMVer1_Term->toUtf8());
        b0.PSAMNum -= 0x01;
    }
    b0.RSUAlgId = 0x00;
    message.append(b0.RSUAlgId);
    b0.RSUManuID = 0x00;
    message.append(b0.RSUManuID);
    message.append(b0.RSUID[0] = 0x00);
    message.append(b0.RSUID[1] = 0x00);
    message.append(b0.RSUID[2] - 0x00);
    b0.RSUVersion = 0x0000;
    message.append(b0.RSUVersion >> 8 &0xff);
    message.append(b0.RSUVersion &0xff);
    b0.Authorization = 0x02;
    message.append(b0.Authorization);
    b0.workstatus = 0x00;
    message.append(b0.workstatus);
    b0.Reserved = 0x00;
    message.append(b0.Reserved >> 24 &0xff);
    message.append(b0.Reserved >> 16 &0xff);
    message.append(b0.Reserved >> 8 &0xff);
    message.append(b0.Reserved &0xff);

    int totallen = message.size() - 4;
    message.insert(5,message[2]);
    message.insert(6,message[2]);
    message.insert(7,totallen >> 8 & 0xff);
    message.insert(8,totallen & 0xff);

    uchar msg[2048];
    memcpy(&msg, message.constData(), totallen+8);

    uint16_t crc_val;
    crc_val = crc16_java(&msg[2], totallen + 6);
    message.append(crc_val >> 8 & 0xFF);
    message.append(crc_val & 0xFF);

    webSocket->sendBinaryMessage(message);
}

void MainWindow::sendB2()
{
    QByteArray message;
    struct FrameHeader b2_header;
    message = setHeader(b2_header);

    struct b2 b2;
    b2.frameType = 0xb2;
    message.append(b2.frameType);

    bool ok;
    QString OBUID = ui->le_OBUID->text();
    int OBUID_int = OBUID.toUInt(&ok,16);
    message.append(OBUID_int >> 24 &0xff);
    message.append(OBUID_int >> 16 &0xff);
    message.append(OBUID_int >> 8 &0xff);
    message.append(OBUID_int &0xff);

    QString error_code = ui->cob_errorCode->currentText();
    message.append(error_code.toInt(&ok,16));
    b2.IssueId = 0x00;
    for (int i = 7; i >= 0; i--) {
        message.append(b2.IssueId >> i*8 &0xff);
    }
    b2.contract_type = 0x00;
    b2.contract_ver = 0x00;
    message.append(b2.contract_type + b2.contract_ver);
    b2.serialNumber = 0x00;
    for (int i = 7; i >= 0; i--) {
        message.append(b2.serialNumber >> i*8 &0xff);
    }
    b2.dateofIssue = "20230713";
    for(int i= 0; i < b2.dateofIssue.length(); i+=2) {
        QChar first = b2.dateofIssue.at(i);
        QChar second = b2.dateofIssue.at(i+1);
        message.append(first.digitValue()*16 + second.digitValue());
    }
    b2.dateofExpire = "20230714";
    for(int i= 0; i < b2.dateofExpire.length(); i+=2) {
        QChar first = b2.dateofExpire.at(i);
        QChar second = b2.dateofExpire.at(i+1);
        message.append(first.digitValue()*16 + second.digitValue());
    }

    message.append(b2.equitmentCV = 0x00);

    b2.OBUStatus = 0x00;
    message.append(b2.OBUStatus >> 24 & 0xff);
    message.append(b2.OBUStatus >> 16 & 0xff);
    message.append(b2.OBUStatus >> 8 & 0xff);
    message.append(b2.OBUStatus & 0xff);

    message.append(b2.authMode = 0x00);
    b2.random = 0x00;
    for (int i = 7;i >= 0;i--) {
        message.append(b2.random >> 8*i &0xff);
    }
    message.append(b2.algorithmID = 0x00);
    char zero = 0x00;
    if((b2.authMode ^ 0x00) == 0x00) {
        b2.VehicleInfo.fill(zero,96); //79字节密文+补0
    } else if((b2.authMode ^ 0x01)&&(b2.algorithmID ^ 0x00) == 0x00) {
        b2.VehicleInfo.fill(zero,96); //88字节密文+补0
    } else if((b2.authMode ^ 0x01)&&(b2.algorithmID ^ 0x04) == 0x00){
        b2.VehicleInfo.fill(zero,96); //96字节密文
    }
    message.append(b2.VehicleInfo);
    message.append(b2.cardStatus = 0x00);

    QString cardRestMoney = ui->le_cardrestmon->text();
    b2.cardRestMoney = cardRestMoney.toInt();

    message.append(b2.cardRestMoney >> 24 &0xff);
    message.append(b2.cardRestMoney >> 16 &0xff);
    message.append(b2.cardRestMoney >> 8 &0xff);
    message.append(b2.cardRestMoney &0xff);
    message.append(b2.IssuerInfo.fill(zero,50));
    message.append(b2.LastStation.fill(zero,43));
    message.append(b2.readEf04Status = 0x00);
    message.append(b2.Ef04Info.fill(zero,1));
    b2.B3CostTm = 0x00;
    message.append(b2.B3CostTm >> 24 & 0xff);
    message.append(b2.B3CostTm >> 16 & 0xff);
    message.append(b2.B3CostTm >> 8 & 0xff);
    message.append(b2.B3CostTm & 0xff);
    b2.B4CostTm = 0x00;
    message.append(b2.B4CostTm >> 24 & 0xff);
    message.append(b2.B4CostTm >> 16 & 0xff);
    message.append(b2.B4CostTm >> 8 & 0xff);
    message.append(b2.B4CostTm & 0xff);
    message.append(b2.ApprovedLoad.fill(zero,7));

    int totallen = message.size() - 4;
    message.insert(5,message[2]);
    message.insert(6,message[2]);
    message.insert(7,totallen >> 8 & 0xff);
    message.insert(8,totallen & 0xff);

    uchar msg[2048];
    memcpy(&msg, message.constData(), totallen+8);

    uint16_t crc_val;
    crc_val = crc16_java(&msg[2], totallen + 6);
    message.append(crc_val >> 8 & 0xFF);
    message.append(crc_val & 0xFF);

    webSocket->sendBinaryMessage(message);
}

void MainWindow::sendB5()
{
    QByteArray message;
    struct FrameHeader b5_header;
    message = setHeader(b5_header);

    struct b5 b5;
    message.append(b5.frameType = 0xb5);

    //save OBUID from command c1 to b5
    b5.OBUID = OBUID;
    message.append((b5.OBUID >> 24) &0xff);
    message.append((b5.OBUID >> 16) &0xff);
    message.append((b5.OBUID >> 8) &0xff);
    message.append(b5.OBUID &0xff);

    message.append(b5.error_code = 0x00);

    for (int i=0; i<6; i++) {
        b5.PSAMNo[i] = 0x00;
        message.append(b5.PSAMNo[i]);
    }
    QDateTime dateTime = QDateTime::currentDateTime();
    b5.TransTime = dateTime.toString("yyyyMMddhhmmss");
    for (int i = 0; i < b5.TransTime.length(); i += 2)
    {
        QChar first = b5.TransTime[i];
        QChar second = b5.TransTime[i+1];
        message.append(first.digitValue() * 16 + second.digitValue());
    }
    message.append(b5.TransType = 0x09);
    b5.TAC = 0x00;
    message.append((b5.TAC >> 24) &0xff);
    message.append((b5.TAC >> 16) &0xff);
    message.append((b5.TAC >> 8) &0xff);
    message.append(b5.TAC &0xff);
    b5.ICCPayserial = 0x00;
    message.append((b5.ICCPayserial >> 8) &0xff);
    message.append(b5.ICCPayserial &0xff);
    b5.PSAMTranSerial = 0x00;
    message.append((b5.PSAMTranSerial >> 24) &0xff);
    message.append((b5.PSAMTranSerial >> 16) &0xff);
    message.append((b5.PSAMTranSerial >> 8) &0xff);
    message.append(b5.PSAMTranSerial &0xff);

    QString cardBalance = ui->le_cardbalance->text();
    b5.CardBalance = cardBalance.toInt();

    message.append((b5.CardBalance >> 24) &0xff);
    message.append((b5.CardBalance >> 16) &0xff);
    message.append((b5.CardBalance >> 8) &0xff);
    message.append(b5.CardBalance &0xff);

    message.append(b5.AlgorithmID = 0x04);

    b5.MAC2 = 0x00;
    message.append((b5.MAC2 >> 24) &0xff);
    message.append((b5.MAC2 >> 16) &0xff);
    message.append((b5.MAC2 >> 8) &0xff);
    message.append(b5.MAC2 &0xff);

    message = setEnding(message);
    webSocket->sendBinaryMessage(message);
}

void MainWindow::onBinaryMessageReceived(QByteArray msg)
{
    ui->SmallWin1->append(QDateTime::currentDateTime().toString() + "：客户端发来消息：" + msg.toHex());

    qDebug() << "服务端收到信息：" << msg.toHex();

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

        int len = int((message[5]&0xff << 24)  | (message[6]&0xff << 16) | (message[7]&0xff << 8) | (message[8]&0xff));

        ushort crc_sent = ushort(((message[len+8]&0xff) << 8) | (message[len+9]&0xff));

        //currentIndex = index of the last member
        currentIndex = len+9;

        //calculate crc value
        uchar copy[2048];
        memcpy(&copy, message.constData(), len+8);
        u_int16_t crc_val = crc16_java(&copy[2], len + 6);

        //compare two crc values
        if ((crc_sent ^ crc_val) == 0x0000)
        {
            qDebug() << "校验成功";

            //save current correct dataframe into list
            cleanData.append(message.left(len+10));

            //remove current message
            message.remove(0,currentIndex+1);
            continue;
        }
        else
        {
            return;
        }
    }

    //obtain cmd type from clean data
    char command_type = char(cleanData[0].at(32)&0xff);
    if(uchar(cleanData[0].at(33) & 0xff) == 0xc4)
        command_type = 0xc4;

    switch (uint8_t(command_type))
    {
    case 0xc0:
        {
            qDebug() << "command_type matches C0: <初始化指令>";
            parseC0(cleanData[0]);
            cleanData.removeFirst();
            sendB0();
            timer->start(5000);
            break;
        }

    case 0xc1:
        {
            qDebug() << "command_type matches C1: <继续交易指令>";
            parseC1(cleanData[0]);
            sendB5();
            cleanData.removeFirst();
            break;
        }
    case 0xc4:
        {
            qDebug() << "command_type matches C4: <开关路侧单元指令>";
            break;
        }
    case 0xc6:
        {
            qDebug() << "command_type matches C6: <消费交易指令>";
            parseC6(cleanData[0]);
            break;
        }
    }
}

void MainWindow::onDisconnection()
{
    webSocket = NULL;
    ui->SmallWin1->append(QDateTime::currentDateTime().toString() + "：客户端断开了连接");
}




