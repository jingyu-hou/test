#ifndef QTREEMANGEVIEW_H
#define QTREEMANGEVIEW_H

#include <QTreeView>
#include <QStandardItem>
#include <QTreeWidget>
#include <QMenu>
#include "QTree_Model.h"
#include <QMouseEvent>
class QTreeMangerView : public QTreeView
{
	Q_OBJECT

public:
	QTreeMangerView(QWidget *parent);
    QTreeMangerView(QTree_Model *treeModel,QWidget *parent);
    QTreeMangerView(QTree_Model *treeModel,QWidget *parent,int d);
    ~QTreeMangerView();
    void mousePressEvent(QMouseEvent *event);
    int m_ContexShowStyle;
public:
	void SetModel(QTree_Model *treeModel);
private:
    void ChangeModeIndexPre( const QModelIndex &index);
private slots:
    void contextMenuPostSlot(const QPoint& contextM);
    void contextMenuPreSlot(const QPoint &p);
    void TestSlot();
    //--curve
    void NewCurveSlot();
    void EditCurveSlot();
    void DelCurveSlot();
    void upDataTreeNameSlot(QString,QStringList);
signals:
    void EmitChangeModelIndex(const QModelIndex &index);
    void EmitChangeModeIndexMater(const QModelIndex &index);
    void EmitChangeModeIndexPre(const QModelIndex &index);
    void EmitNewCurve(int,int);
    QString EmitCurveEdit(QString);
    QString EmitCurveDel(QString);
protected:
	//virtual void paintEvent(QPaintEvent *event);
    int m_CurveRow;
    int m_CurveRowInd;
    QString m_strCurName;

private:
	QTree_Model *treeModel_;
};

#endif // QTREEMANGEVIEW_H
