#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H

enum ENUM_WEBSOCKET_STATUS
{
    Enum_NoConnect = -1,
    Enum_DataNull = -2,
    Enum_LockFailed = -3,
};

#define HEART_INTERVAL 15000        // 心跳间隔 单位：ms
#define RECONNECT_INTERVAL 3000     // 重连间隔 单位：ms

// 按钮原始样式
#define ORIGNAL_STYLE "QPushButton{\n	border-style:solid;\n	border-width:1px;\n	color: rgb(0, 0, 0);\n	background-color: rgba(225, 225, 225);\n	border-color:rgba(100, 100, 100, 120);\n}\n\nQPushButton:hover{\n	color:rgba(0,0,0,200);\n	background-color:rgb(240, 248, 248);\n	border-color: rgb(0, 150, 250);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(0,0,0,150);\n	background-color: rgba(225, 225, 225);\n	border-color: rgba(0, 163, 245);\n}"
// 按钮按下后的样式
#define PRESSED_STYLE "QPushButton{\n	border-style:solid;\n	border-width:1px;\n	color:rgb(139, 139, 0);\n	background-color:rgb(248, 248, 216);\n	border-color:rgb(243, 236, 37);\n}\n\nQPushButton:hover{\n	color:rgba(139,139,0);\n	background-color:rgb(255, 255, 240);\n	border-color: rgb(250, 250, 10);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(150,150,0,150);\n	background-color: rgba(248, 248, 216);\n	border-color: rgb(220, 143, 18);\n}"
// 工作正常的样式
#define WORK_STYLE "QPushButton{\n	border-style:solid;\n	border-width:1px;\n	color: rgba(0, 170, 127, 150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color:rgba(0, 255, 127, 120);\n}\n\nQPushButton:hover{\n	color:rgba(0,150,70,200);\n	background-color: rgba(240, 245, 240, 100);\n	border-color: rgb(3, 255, 87);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(0,150,100,150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color: rgba(0, 255, 127,120);\n}"
// 工作异常的样式
#define ERROR_STYLE "QPushButton{\n	border-style:solid;\n	border-width:1px;\n	color:rgb(170, 0, 20);\n	background-color: rgba(245, 245, 245, 30);\n	border-color:rgb(255, 0, 20,120);\n}\n\nQPushButton:hover{\n	color:rgb(210, 0, 31);\n	background-color: rgba(240, 245, 240, 100);\n	border-color: rgb(255, 3, 20);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(170,0,20,150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color: rgba(230, 3, 20);\n}"

// 机器人/岗亭模式 按钮原始样式
#define ORIGNAL_STYLE_ENTRY "QPushButton{\n	border-style:solid;\n	border-width:4px;\n	border-radius:10px;\n	color: rgba(100, 100, 100, 150);\n	background-color: rgba(80, 80, 80, 30);\n	border-color:rgba(100, 100, 100, 120);\n}\n\nQPushButton:hover{\n	color:rgba(80,80,80,200);\n	background-color: rgba(220, 220, 220, 100);\n	border-color: rgba(120, 120, 120,200);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(100,100,100,150);\n	background-color: rgba(80, 80, 80, 30);\n	border-color: rgba(100, 100, 100,120);\n}"
// 机器人/岗亭模式 按钮按下后的样式
#define PRESSED_STYLE_ENTRY "QPushButton{\n	border-style:solid;\n	border-width:4px;\n	border-radius:10px;\n	color:rgba(139, 139, 0,150);\n	background-color:rgba(248, 248, 216,30);\n	border-color:rgba(250, 250, 10,120);\n}\n\nQPushButton:hover{\n	color:rgba(209, 209, 0,200);\n	background-color:rgba(255, 255, 240,100);\n	border-color: rgba(255, 255, 10,200);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(110,110,0,150);\n	background-color: rgba(248, 248, 216,30);\n	border-color: rgba(250, 250, 10,120);\n}"
// 机器人/岗亭模式 工作正常的样式
#define WORK_STYLE_STYLE_ENTRY "QPushButton{\n	border-style:solid;\n	border-width:4px;\n	border-radius:10px;\n	color: rgba(0, 170, 127, 150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color:rgba(0, 255, 127, 120);\n}\n\nQPushButton:hover{\n	color:rgba(0,150,70,200);\n	background-color: rgba(240, 245, 240, 100);\n	border-color: rgba(0, 255, 127,200);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(0,150,100,150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color: rgba(0, 255, 127,120);\n}"
// 机器人/岗亭模式 工作异常的样式
#define ERROR_STYLE_ENTRY "QPushButton{\n	border-style:solid;\n	border-width:4px;\n	border-radius:10px;\n	color:rgb(170, 0, 20);\n	background-color: rgba(245, 245, 245, 30);\n	border-color:rgb(255, 0, 20,120);\n}\n\nQPushButton:hover{\n	color:rgb(210, 0, 31);\n	background-color: rgba(240, 245, 240, 100);\n	border-color: rgb(255, 3, 20);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(170,0,20,150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color: rgba(230, 3, 20,120);\n}"

// 检查Json数据
#define CHECKJSON(szData) \
    if(nullptr == szData) \
        return false; \
                      \
    QJsonParseError err; \
    QJsonDocument jsonDoc = QJsonDocument::fromJson(szData, &err); \
        \
    if(err.error != QJsonParseError::NoError) \
    { \
        qDebug()<<"Json格式错误"; \
        return false; \
    }

// JsonObject -> QString
#define JSONWRITE(jsonObj) \
    QJsonDocument jsonDoc(jsonObj); \
    QString strResult = jsonDoc.toJson(QJsonDocument::Indented);

// 检查bizContent数据
#define CHECKCONTENT(szData) \
    if(nullptr == szData) \
        return false; \
                      \
    QJsonDocument jsonDoc = QJsonDocument::fromJson(szData); \
    QJsonObject jsonObj = jsonDoc.object(); \
    QJsonObject bizContent = jsonObj["bizContent"].toObject(); \
    if(bizContent.empty()) \
    { \
        qWarning()<<"bizContent is empty"; \
        return false; \
    }

#endif // GLOBALDEFINE_H
