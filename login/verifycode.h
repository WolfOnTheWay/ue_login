#ifndef VERIFYCODE_H
#define VERIFYCODE_H

#include <QWidget>
#include <QTimer>
#include <QtCore/QJsonDocument>
#include <QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
namespace Ui {
class VerifyCode;
}

class VerifyCode : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyCode(QString m_phone_number, QWidget *parent = 0);
    ~VerifyCode();


private slots:
    void on_pushButton_clicked();
    void slot_time_true();

    void on_lineEdit_1_textChanged(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_lineEdit_3_textEdited(const QString &arg1);

    void on_lineEdit_4_textEdited(const QString &arg1);

    void on_lineEdit_5_textEdited(const QString &arg1);

    void on_lineEdit_6_textEdited(const QString &arg1);

    void sendMessageFinished(QNetworkReply* reply);



private:
    Ui::VerifyCode *ui;
    int time_flag;   //验证码获取倒计时flag
    QString phone_number;
    QString verify_code;    //输入的验证码
};

#endif // VERIFYCODE_H
