#include "verifycode.h"
#include "ui_verifycode.h"
#include <QDebug>


#define VERIFY_TIME 6

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

    //发送验证码请求
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager();
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(sendMessageFinished(QNetworkReply*)));
    Q_ASSERT(connRet);
    request.setUrl(QUrl("http://227.0.3.1:8090/d0SayHello"));
    QNetworkReply* reply = naManager->get(request);
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
    qDebug()<<verify_code;

    //完整验证码已输入-校验验证码
    //demo
    if ("123456" != verify_code)
    {
        qDebug()<<"后续验证接口！";
    }
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


