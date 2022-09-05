#include "login.h"
#include "ui_login.h"
#include <QImage>
#include <verifycode.h>

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

void login::on_pushButton_clicked()
{
    //点击下一步
    VerifyCode *vc = new VerifyCode(ui->lineEdit->text());
    this->hide();
    vc->show();

}
