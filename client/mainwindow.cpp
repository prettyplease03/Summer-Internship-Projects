#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    webSocket = new QWebSocket();
    connect(webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(webSocket, SIGNAL(textMessageReceived(const QString&)), this, SLOT(onTextMsg(const QString&)));
    connect(webSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(ui->ListenBu, SIGNAL(clicked()), this ,SLOT(ListenBu()));
    connect(ui->ListenBu_2, SIGNAL(clicked()), this, SLOT(ListenBu_2()));
    connect(ui->sendData, SIGNAL(clicked()), this, SLOT(sendData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//连接websocket服务器的url
void MainWindow::connectUrl(QString url)
{
    m_url = QUrl(url);
    webSocket->open(m_url);

}

void MainWindow::onConnected()
{
    ui->RiZhi->append(QDateTime::currentDateTime().toString() + "连接成功！");
}

void MainWindow::onDisconnected()
{
    ui->RiZhi->append(QDateTime::currentDateTime().toString() + "断开连接！");
}

//开始监听
void MainWindow::ListenBu()
{
    QString str = ui->DuanKou->toPlainText();
    connectUrl(str);
}

//停止监听
void MainWindow::ListenBu_2()
{
    webSocket->close();
}

void MainWindow::sendTextMsg(const QString &message)
{
    if (!m_bConnected)
    {
        return;
    }
    webSocket->sendTextMessage(message);
}

void MainWindow::onTextMsg(const QString& msg)
{
    ui->RiZhi->append(QDateTime::currentDateTime().toString() + "：客户端发来消息：" + msg);
    return;
}

//发送数据
void MainWindow::sendData()
{
    QString str = QDateTime::currentDateTime().toString() + "：给服务端发送消息：" + ui->ShuJu->toPlainText();
    ui->RiZhi->setText(str);
    webSocket->sendTextMessage(ui->ShuJu->toPlainText());
}


