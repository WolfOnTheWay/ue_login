#include "login.h"
#include "ui_login.h"
#include <QImage>
#include <verifycode.h>
#include "util.h"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
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

    ui->label_check->hide();    //手机号错误提醒，默认隐藏
}

login::~login()
{
    delete ui;
}


//点击下一步
void login::on_pushButton_clicked()
{
    //进行手机号验证
    QString phone = ui->lineEdit->text();
    if(!Util::isLegalPhone(phone)){
        ui->lineEdit->text()="";
        ui->label_check->setText("手机号错误，请重新填写");
        ui->label_check->show();
    }else {
        VerifyCode *vc = new VerifyCode(ui->lineEdit->text());
        this->hide();
        vc->show();
    }
}
