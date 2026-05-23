#include "QMyTableWidget.h"
#include <QAbstractButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QHeaderView>
#include <QStylePainter>


QMyTableWidget::QMyTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    QAbstractButton *btn=findChild<QAbstractButton*>();
        if (btn)
        {
            btn->setText(tr("相"));
        }
        btn->installEventFilter(this);
        QStyleOptionHeader opt;
        opt.text=btn->text();
        QSize s=(btn->style()->sizeFromContents(QStyle::CT_HeaderSection,&opt,QSize(),btn).expandedTo(QApplication::globalStrut()));
        if (s.isValid())
        {
            this->verticalHeader()->setMinimumWidth(s.width());
        }
   // QHBoxLayout *HBlayout=new QHBoxLayout(this);
    //HBlayout->addWidget(btn);
    //setLayout(HBlayout);
} 

QMyTableWidget::~QMyTableWidget()
{

}
bool QMyTableWidget::eventFilter(QObject *o,QEvent *e)
{
    if (e->type()==QEvent::Paint)
    {
        QAbstractButton *btn=qobject_cast<QAbstractButton*>(o);
        if (btn)
        {
            QStyleOptionHeader opt;
            opt.init(btn);
            QStyle::State state=QStyle::State_None;
            if (btn->isEnabled())
                state|=QStyle::State_Enabled;
            if (btn->isActiveWindow())
                state|=QStyle::State_Active;
            if (btn->isDown())
                state|=QStyle::State_Sunken;
            opt.state=state;
            opt.rect=btn->rect();
            opt.text=btn->text();
            opt.position=QStyleOptionHeader::OnlyOneSection;
            QStylePainter painter(btn);

            painter.drawControl(QStyle::CE_Header,opt);
            return true;
        }
    }
    return false;

}