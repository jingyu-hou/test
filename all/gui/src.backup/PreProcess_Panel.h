#ifndef QFROWARDPRC_H
#define QFROWARDPRC_H

#include <QHeaderView>
#include <QWidget>
#include <QDockWidget>
#include <QVBoxLayout>
#include "QTreeMangerView.h"

//*--热等静压
class QPreHIPPrc : public QWidget//QDockWidget
{
	Q_OBJECT

public:
	QPreHIPPrc(QWidget *parent=NULL);
	~QPreHIPPrc();
public:
    QTreeMangerView *m_TreeView ;
    QTree_Model		*m_TreeModel;
public:
    void setStyle();
    void InsertPart(QString strName);
    public slots:
    void ChangeModelIndexSlot( const QModelIndex & index );
private:
	
};

//*--热处理
class QPreHPPrc : public QWidget//QDockWidget
{
    Q_OBJECT

public:
    QPreHPPrc(QWidget *parent=NULL);
    ~QPreHPPrc();
public:
    QTreeMangerView *m_TreeView ;
    QTree_Model		*m_TreeModel;
public:
    void setStyle();
private:

};

//*--热挤压
class QPreHEPrc : public QWidget//QDockWidget
{
    Q_OBJECT

public:
    QPreHEPrc(QWidget *parent=NULL);
    ~QPreHEPrc();
public:
    QTreeMangerView *m_TreeView ;
    QTree_Model		*m_TreeModel;
public:
    void setStyle();
private:

};

#endif // QFROWARDPRC_H
