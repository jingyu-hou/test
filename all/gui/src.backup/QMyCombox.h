#ifndef QMYCOMBOX_H
#define QMYCOMBOX_H
#include <QMouseEvent>
#include <QComboBox>
#include <QAbstractItemView>
class MyCombox : public QComboBox
{
    Q_OBJECT
public:
    explicit MyCombox(QWidget *parent = NULL, int id = 0);
    ~MyCombox();
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
public:
    void hidePopup();
    void showPopup();
    int m_Wigid;
    int m_currentId;
    int m_oldId;
    //virtual void currentIndexChanged(int id);
signals:
    void clicked(int id,int m_currentId);// Left
    void doubleclicked(int id,int m_currentId);//
    void RightClicked(int id, int m_currentId);
    //void currentIndexChanged(int id, int m_currentId);
};

#endif // QMYCOMBOX_H
