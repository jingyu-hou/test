#ifndef SARIBBONAPPLICATIONBUTTON_H
#define SARIBBONAPPLICATIONBUTTON_H
#include <QPushButton>
#include "SARibbonGlobal.h"
class SA_RIBBON_EXPORT SARibbonApplicationButton : public QPushButton
{
    Q_OBJECT
public:
    SARibbonApplicationButton(QWidget* parent);
};

#endif // SARIBBONAPPLICATIONBUTTON_H
//class  SA_RIBBON_EXPORT SARibbonApplicationButton : public QWidget
//{
//    Q_OBJECT
// public:
//   // SARibbonApplicationButton(QWidget* parent);
//    SARibbonApplicationButton(const QPixmap& icon, const QString& text, QWidget *parent)
//    {
//        QLabel* label_Icon = new QLabel(this);

//        label_Icon->setFixedSize(32,32);
//        label_Icon->setScaledContents(true);
//        label_Icon->setPixmap(icon);

//        QLabel* label_text = new QLabel(text, this);
//        QHBoxLayout* layout = new QHBoxLayout;
//        layout->setContentsMargins(20,6,50,6);

//        layout->setSpacing(10);
//        layout->addWidget(label_Icon);
//        layout->addWidget(label_text);
//        setLayout(layout);
//        setFixedWidth(240);
//    }
//    ~SARibbonApplicationButton()
//    {

//    ;
//    }
//protected:
//    void paintEvent(QPaintEvent *e)
//    {
//        QStyleOption opt;
//        opt.init(this);
//        QPainter p(this);
//        style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
//    }
//private:


//} ;

