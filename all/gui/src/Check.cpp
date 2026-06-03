#include "Check.h"

Check::Check(QWidget *parent1)
    : QDialog(parent1)
{
    setWindowTitle(tr("Check"));

    CheckItemTree = new QTreeWidget(this);
    CheckItemTree->setHeaderHidden(true);

    PartItem = new QTreeWidgetItem(CheckItemTree);
    PartItem->setText(0, tr("Part"));

    NodeSetItem = new QTreeWidgetItem(CheckItemTree);
    NodeSetItem->setText(0, tr("Node Set"));

    ElementSetItem = new QTreeWidgetItem(CheckItemTree);
    ElementSetItem->setText(0, tr("Element Set"));

    SurfaceSetItem = new QTreeWidgetItem(CheckItemTree);
    SurfaceSetItem->setText(0, tr("Surface Set"));

    MaterialItem = new QTreeWidgetItem(CheckItemTree);
    MaterialItem->setText(0, tr("Material"));

    BoundaryItem = new QTreeWidgetItem(CheckItemTree);
    BoundaryItem->setText(0, tr("Boundary"));

    ContactItem = new QTreeWidgetItem(CheckItemTree);
    ContactItem->setText(0, tr("Contact"));

    InitializationItem = new QTreeWidgetItem(CheckItemTree);
    InitializationItem->setText(0, tr("Initialization"));

    ThermalItem = new QTreeWidgetItem(CheckItemTree);
    ThermalItem->setText(0, tr("Thermal Boundary"));

    LodingItem = new QTreeWidgetItem(CheckItemTree);
    LodingItem->setText(0, tr("Loading"));

    MotionItem = new QTreeWidgetItem(CheckItemTree);
    MotionItem->setText(0, tr("Motion"));

    CastingSystemItem = new QTreeWidgetItem(CheckItemTree);
    CastingSystemItem->setText(0, tr("Casting System"));

    HPSystemItem = new QTreeWidgetItem(CheckItemTree);
    HPSystemItem->setText(0, tr("Heat Treatment"));

    HIPSystemItem = new QTreeWidgetItem(CheckItemTree);
    HIPSystemItem->setText(0, tr("HIP System"));

    VariableOutputItem = new QTreeWidgetItem(CheckItemTree);
    VariableOutputItem->setText(0, tr("Variable Output"));

    StepItem = new QTreeWidgetItem(CheckItemTree);
    StepItem->setText(0, tr("Step"));

    QHBoxLayout *VLayout1 = new QHBoxLayout();
    VLayout1->addWidget(CheckItemTree);
    setLayout(VLayout1);

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
