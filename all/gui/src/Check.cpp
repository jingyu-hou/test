#include "Check.h"
#include <QVBoxLayout>

Check::Check(QWidget *parent1)
    : QDialog(parent1)
{
    setWindowTitle(tr("检查"));
    setAttribute(Qt::WA_DeleteOnClose, false);

    CheckItemTree = new QTreeWidget(this);
    CheckItemTree->setHeaderHidden(true);

    PartItem = new QTreeWidgetItem(CheckItemTree);
    PartItem->setText(0, tr("部件"));

    NodeSetItem = new QTreeWidgetItem(CheckItemTree);
    NodeSetItem->setText(0, tr("节点集"));

    ElementSetItem = new QTreeWidgetItem(CheckItemTree);
    ElementSetItem->setText(0, tr("单元集"));

    SurfaceSetItem = new QTreeWidgetItem(CheckItemTree);
    SurfaceSetItem->setText(0, tr("面集"));

    MaterialItem = new QTreeWidgetItem(CheckItemTree);
    MaterialItem->setText(0, tr("材料"));

    BoundaryItem = new QTreeWidgetItem(CheckItemTree);
    BoundaryItem->setText(0, tr("边界条件"));

    ContactItem = new QTreeWidgetItem(CheckItemTree);
    ContactItem->setText(0, tr("接触"));

    InitializationItem = new QTreeWidgetItem(CheckItemTree);
    InitializationItem->setText(0, tr("初始化"));

    ThermalItem = new QTreeWidgetItem(CheckItemTree);
    ThermalItem->setText(0, tr("热边界"));

    LodingItem = new QTreeWidgetItem(CheckItemTree);
    LodingItem->setText(0, tr("载荷"));

    MotionItem = new QTreeWidgetItem(CheckItemTree);
    MotionItem->setText(0, tr("运动"));

    CastingSystemItem = new QTreeWidgetItem(CheckItemTree);
    CastingSystemItem->setText(0, tr("铸造系统"));
    CastingSystemItem->setHidden(true);

    HPSystemItem = new QTreeWidgetItem(CheckItemTree);
    HPSystemItem->setText(0, tr("热处理"));

    HIPSystemItem = new QTreeWidgetItem(CheckItemTree);
    HIPSystemItem->setText(0, tr("HIP系统"));

    VariableOutputItem = new QTreeWidgetItem(CheckItemTree);
    VariableOutputItem->setText(0, tr("变量输出"));

    StepItem = new QTreeWidgetItem(CheckItemTree);
    StepItem->setText(0, tr("分析步"));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(CheckItemTree);
    setLayout(mainLayout);

    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screwnRect = desktopWidget->screenGeometry();
    screenX = screwnRect.width();
    screenY = screwnRect.height();
    this->setMinimumWidth(screenX / 7.0);
    this->resize(screenX / 4.5, screenY / 3.0);
    this->setWindowIcon(QIcon(":/images/welcme.png"));
}

Check::~Check()
{
}

void Check::addResult(QTreeWidgetItem *parent, const QString &text, int status)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, text);
    if (status == Error) {
        item->setForeground(0, Qt::red);
    } else if (status == Warning) {
        item->setForeground(0, QColor(200, 150, 0));
    } else {
        item->setForeground(0, QColor(0, 128, 0));
    }
}

void Check::clearResults()
{
    QTreeWidgetItem *items[] = {
        PartItem, NodeSetItem, ElementSetItem, SurfaceSetItem,
        MaterialItem, BoundaryItem, ContactItem, InitializationItem,
        ThermalItem, LodingItem, MotionItem, CastingSystemItem,
        HPSystemItem, HIPSystemItem, VariableOutputItem, StepItem
    };
    for (int i = 0; i < 16; i++) {
        while (items[i]->childCount() > 0) {
            delete items[i]->takeChild(0);
        }
    }
}
