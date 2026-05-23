#ifndef QMYTABLEWIDGET_H
#define QMYTABLEWIDGET_H

#include <QTableWidget>

class QMyTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    QMyTableWidget(QWidget *parent);
    ~QMyTableWidget();
   bool eventFilter(QObject *o,QEvent *e);
private:
    
};

#endif // QMYTABLEWIDGET_H
