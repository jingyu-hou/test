#ifndef QTREEMANGERDOCK_H
#define QTREEMANGERDOCK_H

#include <QDockWidget>

class QTreeMangerDock : public QDockWidget
{
	Q_OBJECT

public:
	QTreeMangerDock(QWidget *parent);
	~QTreeMangerDock();

public:
	void SetWidth(int MinWidth,int MaxWidth);
private:

};

#endif // QTREEMANGERDOCK_H
