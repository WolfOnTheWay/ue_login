# include "config.h"


ConfigFile::ConfigFile()
{

}
bool ConfigFile::fileName(QString qstrfilename){
    if (qstrfilename.isEmpty())
    {
        m_qstrFileName = "config.cnf";
    }
    else
    {
        m_qstrFileName = qstrfilename;
    }

    QDir dir;
    if ( !dir.exists(m_qstrFileName) ){

        return false;
    }
    m_psetting = new QSettings(m_qstrFileName, QSettings::IniFormat);
    return true;
}

ConfigFile::~ConfigFile()
{
    delete m_psetting;
    m_psetting = NULL;
}

void ConfigFile::Set(QString qstrNodeName,QString qstrKeyName,QVariant qvarvalue)
{
    m_psetting->setValue(QString("/%1/%2").arg(qstrNodeName).arg(qstrKeyName), qvarvalue);
}

QVariant ConfigFile::Get(QString qstrNodeName,QString qstrKeyName)
{
    QVariant qvar = m_psetting->value(QString("/%1/%2").arg(qstrNodeName).arg(qstrKeyName));
    return qvar;
}
