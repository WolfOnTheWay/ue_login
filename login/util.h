﻿#ifndef UTIL_H
#define UTIL_H

#include<QObject>
#include<QString>
#include<QRegExpValidator>

class Util:public QObject{
    Q_OBJECT
public:
    static bool isLegalPhone(const  QString &phoneNum);
};

#endif // UTIL_H

