#ifndef QFRDACTORWIDGE_H
#define QFRDACTORWIDGE_H

#include <QObject>
#include <QListWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <vector>
using namespace std;
class QFrdActorWidge : public QWidget
{
    Q_OBJECT

public:
    QFrdActorWidge(QWidget *parent);
    ~QFrdActorWidge();
public:
    QListWidget *widgetList_;
    void creatGui();
    void upDataListWidget(const vector<int>*);//更新Widget的List
public slots:
    void  ActorSlot();
signals:
    void emitClickAcotrListWig(vector<int>);
private:
    
};

#endif // QFRDACTORWIDGE_H
