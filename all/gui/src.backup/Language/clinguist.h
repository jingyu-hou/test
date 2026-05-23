#ifndef CLINGUIST_H
#define CLINGUIST_H

#include <QObject>
#include <QTranslator>

enum WelLanguage
{
    zh_cn,
    en_us,
    undefined
};

class CLinguist : public QObject
{
    Q_OBJECT

private:
    CLinguist();

public:
    //当前语言 简体中文、英文、未定义
    WelLanguage m_CurrentLanguage; //
    //切换语言
    void ChangeLanguage(WelLanguage lan);
    static CLinguist *GetLinguistInstance();
private:
    static CLinguist *linguist;
    QTranslator *m_trans;
    public:
signals:
    void LanguageChanged(int);
};

#endif // CLINGUIST_H
