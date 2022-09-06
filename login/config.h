#ifndef CONFIG_H
#define CONFIG_H

/*
    配置类：
        用于获取：
            1. 短信获取接口
            2. 短信验证接口
        用于设置：
            1.token文件的存储位置和名称


*/
#include<QVariant>
#include <QSettings>
#include<QDir>

class ConfigFile{
public:
    ConfigFile();
    bool fileName(QString qstrfilename);
    virtual ~ConfigFile(void);
    void Set(QString,QString,QVariant);
    QVariant Get(QString,QString);
private:
    QString m_qstrFileName;
    QSettings *m_psetting;
};

#endif // CONFIG_H


