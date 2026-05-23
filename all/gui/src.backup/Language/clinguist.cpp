#include "clinguist.h"

#include <QCoreApplication>

CLinguist *CLinguist::linguist = 0;

CLinguist::CLinguist()
    
{
    m_trans = new QTranslator(qApp);
}

CLinguist *CLinguist::GetLinguistInstance()
{
    if (0==linguist)
    {
        linguist = new CLinguist;
    }
    return linguist;
}
void CLinguist::ChangeLanguage(WelLanguage lan)
{
    if (lan == this->m_CurrentLanguage || lan == undefined)
    {
        return;
    }
    bool ret;
    switch(lan){
        case zh_cn:{
            if (NULL != m_trans)
            {
                qApp->removeTranslator(m_trans);
            }
            ret = m_trans->load(QCoreApplication::applicationDirPath()+"/ff1_zh.qm");
            if (ret)
            {
                qApp->installTranslator(m_trans);
            }
          }break;
        case en_us:{
            if (0 != m_trans)
            {
                qApp->removeTranslator(m_trans);
            }
            ret = m_trans->load(QCoreApplication::applicationDirPath()+"/ff1_en.qm");
            if (ret)
            {
                qApp->installTranslator(m_trans);
            }

          }break;
        default:break;
    }
    if (ret)
    {
        this->m_CurrentLanguage=lan;
        emit LanguageChanged(lan);
    }


}