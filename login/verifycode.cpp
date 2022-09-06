#include "verifycode.h"
#include "ui_verifycode.h"
#include "config.h"
#include <QDebug>
#include<QProcess>
#include<QThread>

#define VERIFY_TIME 60

VerifyCode::VerifyCode(QString m_phone_number, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyCode)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);

    QImage Image;
    Image.load(":/img/flag.png");
    QPixmap pixmap = QPixmap::fromImage(Image);
    int with = ui->flag->width();
    int height = ui->flag->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    //QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui->flag->setPixmap(fitpixmap);

    time_flag = 0;
    verify_code = "";
    phone_number = m_phone_number;
    ui->label_send->hide();
    ui->label_error->hide();
}

VerifyCode::~VerifyCode()
{
    delete ui;
}


void VerifyCode::slot_time_true()
{
    if (time_flag)
    {
        time_flag--;
        QString temp = QString("%1s后可重新获取验证码").arg(time_flag);
        ui->pushButton->setText(temp);
        QTimer::singleShot(1000, this, SLOT(slot_time_true()));
    } else {
        ui->pushButton->setText("重新获取验证码");
    }
}



void VerifyCode::on_pushButton_clicked()
{

    //获取短信获取接口
    QString qstrFileName=(QApplication::applicationDirPath()+"/config.cnf");
    ConfigFile cfg;
    if(!cfg.fileName(qstrFileName))
    {
        qDebug()<<"配置文件不存在,%1"<<qstrFileName;
        return;
    }

    QString url = cfg.Get("config","getMessageApi").toString();

    //组装数据

    QJsonObject json;
    json.insert("phone",phone_number);
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);


    //发送验证码请求
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager();
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(sendMessageFinished(QNetworkReply*)));
    Q_ASSERT(connRet);
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = naManager->post(request,byteArray);
}

//验证码发送完成
void VerifyCode::sendMessageFinished(QNetworkReply* reply){
    //对返回数据进行判断
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //逻辑
    if(!statusCode.isValid()){
        ui->label_error->setText("发送验证码失败，请检查网络连接！");
        ui->label_error->show();
    }else{
        qDebug()<<reply->readAll();
        ui->label_send->show();
        ui->label_send->setText(QString("请输入发送至%1的6位验证码，有效期10分钟。\
                                        如未收到，请尝试重新获取验证码。").arg(phone_number));
                                        ui->label_send->show();
                                time_flag = VERIFY_TIME;
                QString temp = QString("%1s后可重新获取验证码").arg(time_flag);
        ui->pushButton->setText(temp);

        //启动定时器
        QTimer::singleShot(1000, this, SLOT(slot_time_true()));
    }
}

void VerifyCode::on_lineEdit_6_textEdited(const QString &arg1)
{
    verify_code += arg1;
    //    qDebug()<<verify_code;

    //    //完整验证码已输入-校验验证码
    //    //demo
    //    if ("123456" != verify_code)
    //    {
    //        qDebug()<<"后续验证接口！";
    //    }
}

void VerifyCode::on_lineEdit_1_textChanged(const QString &arg1)
{
    verify_code += arg1;
    ui->lineEdit_2->setFocus();
}

void VerifyCode::on_lineEdit_2_textEdited(const QString &arg1)
{
    verify_code += arg1;
    ui->lineEdit_3->setFocus();
}

void VerifyCode::on_lineEdit_3_textEdited(const QString &arg1)
{
    verify_code += arg1;
    ui->lineEdit_4->setFocus();
}

void VerifyCode::on_lineEdit_4_textEdited(const QString &arg1)
{
    verify_code += arg1;
    ui->lineEdit_5->setFocus();
}

void VerifyCode::on_lineEdit_5_textEdited(const QString &arg1)
{
    verify_code += arg1;
    ui->lineEdit_6->setFocus();
}


//下一步：登录
void VerifyCode::on_pushButton_3_clicked()
{
    //获取登录接口
    QString qstrFileName=(QApplication::applicationDirPath()+"/config.cnf");
    ConfigFile cfg;
    if(!cfg.fileName(qstrFileName))
    {
        qDebug()<<"配置文件不存在,%1"<<qstrFileName;
        return;
    }

    QString url = cfg.Get("config","loginApi").toString();

    //组装数据

    QJsonObject json;
    json.insert("phone",phone_number);
    json.insert("loginType","APP手机");
    json.insert("code",verify_code);
    qDebug()<<"verfycode:"<<verify_code;
    //verify_code重置
    verify_code = "";

    ui->lineEdit_1->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_1->setFocus();


    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);


    //发送登录请求
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager();
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(loginFinished(QNetworkReply*)));
    Q_ASSERT(connRet);
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = naManager->post(request,byteArray);
}

void VerifyCode::loginFinished(QNetworkReply* reply){
    //对返回数据进行判断
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //逻辑
    if(!statusCode.isValid()){
        ui->label_error->setText("登录失败！");
        ui->label_error->show();
    }else {

        QByteArray resultByte =  reply->readAll();


        //打印信息
        QJsonDocument resultDoc=QJsonDocument::fromJson(resultByte);
        QJsonObject json=resultDoc.object();
        QString msg = json.value("msg").toString();
        int code = json.value("code").toInt();
        if(code!=200){
            ui->label_error->setText(msg);
            ui->label_error->show();
        }else {
            //成功
            QJsonObject data= json.value("data").toObject();
            QString token = data.value("tokenValue").toString();

            ui->label_error->setText("登录成功，即将调起ue!!!");
            ui->label_error->show();
            //1.存储token
            QString qstrFileName=(QApplication::applicationDirPath()+"/config.cnf");
            ConfigFile cfg;
            if(!cfg.fileName(qstrFileName))
            {
                qDebug()<<"配置文件不存在,%1"<<qstrFileName;
                return;
            }

            QString tokenPath = cfg.Get("config","tokenPath").toString();
            QString uePath = cfg.Get("config","ueExePath").toString();
            QFile file(tokenPath);


            if(file.open(QIODevice::ReadWrite |  QIODevice::Text | QFile::Truncate))
            {
                file.write(token.toLatin1().data());
                file.close();
                qDebug()<<"文件关闭";
            }

             QProcess::startDetached(uePath,QStringList());

        }
    }

}

