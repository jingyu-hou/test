#ifndef CHECK_H
#define CHECK_H
#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDesktopWidget>
#include <QApplication>
#include <QHBoxLayout>

class Check : public QDialog
{
    Q_OBJECT
public:
    Check(QWidget *parent = NULL);
    ~Check();

    enum Status { Pass = 0, Warning = 1, Error = 2 };

    void addResult(QTreeWidgetItem *parent, const QString &text, int status);
    void clearResults();

    QTreeWidget *CheckItemTree;
    QTreeWidgetItem *PartItem, *MaterialItem, *BoundaryItem, *ContactItem,
        *InitializationItem, *ThermalItem, *LodingItem, *MotionItem,
        *CastingSystemItem, *HPSystemItem, *HIPSystemItem,
        *VariableOutputItem, *StepItem, *NodeSetItem, *ElementSetItem,
        *SurfaceSetItem;

    int screenX, screenY;
};
#endif
