#include"util.h"

//验证手机号是否正确
bool Util::isLegalPhone(const  QString &phoneNum){
    QRegExp regx("^1([358][0-9]|4[579]|66|7[0135678]|9[89])[0-9]{8}$");
    QRegExpValidator regs(regx, 0);
    QString pNum = phoneNum;
    int pos = 0;
    QValidator::State res = regs.validate(pNum, pos);
    if (QValidator::Acceptable == res) {
        return true;
    }
    else {
        return false;
    }
}
