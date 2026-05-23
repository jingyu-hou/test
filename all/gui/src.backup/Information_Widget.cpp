#include <QProcess>
#include <QString>
#include "Information_Widget.h"

Information_Widget* Information_Widget::instance = 0;

/**
* @brief get object of the class
*
* get the object of this class.
* @return Information_Widget*
* @author shiting
* @date 20181214
*/
Information_Widget* Information_Widget::GetInstance()
{
    if (instance == 0)
    {
        instance = new Information_Widget;
    }
    return instance;
}

/**
* @brief show information.
*
* show information at current widget.
* @param[in] info the information to show.
* @return void
* @author shiting
* @date 20181214
*/
void Information_Widget::ShowInformation(const QString &info)
{
    this->append(info);
    this->moveCursor(QTextCursor::End);
}
