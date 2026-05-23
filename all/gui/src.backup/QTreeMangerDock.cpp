#include "QTreeMangerDock.h"

QTreeMangerDock::QTreeMangerDock(QWidget *parent)
: QDockWidget(parent)
{
	this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
	this->setAllowedAreas(Qt::LeftDockWidgetArea);

}

QTreeMangerDock::~QTreeMangerDock()
{

}
void QTreeMangerDock::SetWidth(int MinWidth, int MaxWidth)
{
	this->setMaximumWidth(MaxWidth);
	this->setMinimumWidth(MinWidth);
}