#ifndef INFOMATION_WIDGET_H
#define INFOMATION_WIDGET_H
/**
* @file
*
* Infomation widget class.
*/
#include <QTextBrowser>
class QString;

/**
* @brief Infomation widget.
*
* a widget to show infomation.
* @author shiting
* @date 20181213
*/
class Information_Widget : public QTextBrowser
{
    Q_OBJECT

public:
    static Information_Widget* GetInstance();
    ~Information_Widget() {  }

    void ShowInformation(const QString &info);

private:
    Information_Widget(QWidget *parent = 0) : QTextBrowser(parent) {  }

private:
    static Information_Widget* instance;
};

#endif
