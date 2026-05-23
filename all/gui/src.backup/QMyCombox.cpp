#include "QMyCombox.h"
MyCombox::MyCombox(QWidget *parent /* = NULL */,int id):QComboBox(parent)
{
    m_Wigid=id;
    m_oldId=-1;
    m_currentId = 0;

}
MyCombox::~MyCombox()
{

}
//void MyCombox::currentIndexChanged(int id)
//{
//     m_currentId=currentIndex();
//     emit clicked(m_Wigid,m_Wigid);
//     QComboBox::currentIndexChanged(id);
//}
void MyCombox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        QAbstractItemView *pItemView = this->view();
        int ItemViewY = pItemView->height();
        int height=this->height();
        //控件上坐标xy位置
        int curX=QCursor::pos().x()-mapToGlobal(geometry().topLeft()).x()+geometry().x();
        int curY=QCursor::pos().y()-mapToGlobal(geometry().topLeft()).y()+geometry().y();

        int itemViewX = pItemView->width();
        int Width =this->width();
        if(curX>0 && curX<0.8*Width && curY>0){
            m_currentId=currentIndex(); 
            emit clicked(m_Wigid,m_currentId);
            hidePopup();
        }
    }else if(e->button() == Qt::RightButton){
        QAbstractItemView *pItemView = this->view();
        int ItemViewY = pItemView->height();
        int height=this->height();
        //控件上坐标xy位置
        int curX=QCursor::pos().x()-mapToGlobal(geometry().topLeft()).x()+geometry().x();
        int curY=QCursor::pos().y()-mapToGlobal(geometry().topLeft()).y()+geometry().y();

        int itemViewX = pItemView->width();
        int Width =this->width();
        if(curX>0 && curX<0.8*Width && curY>0){
            m_currentId=currentIndex(); 
            emit RightClicked(m_Wigid,m_currentId);
            hidePopup();
        }
    }

    //this->hidePopup();
    QComboBox::mousePressEvent(e);
}

void MyCombox::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        QAbstractItemView *pItemView = this->view();
        int ItemViewY = pItemView->height();
        int height=this->height();
        //控件上坐标xy位置
        int curX=QCursor::pos().x()-mapToGlobal(geometry().topLeft()).x()+geometry().x();
        int curY=QCursor::pos().y()-mapToGlobal(geometry().topLeft()).y()+geometry().y();

        int itemViewX = pItemView->width();
        int Width =this->width();
        if(curX>0 && curX<0.8*Width && curY>0){
            m_currentId=currentIndex();
            emit doubleclicked(m_Wigid,m_currentId);
            hidePopup();
        }
    }
    //this->hidePopup();
    QComboBox::mousePressEvent(e);
}

void MyCombox::hidePopup()
{
    QAbstractItemView *pItemView = this->view();
    int ItemViewY = pItemView->height();
    int height=this->height();
    //控件上坐标xy位置
    int curX=QCursor::pos().x()-mapToGlobal(geometry().topLeft()).x()+geometry().x();
    int curY=QCursor::pos().y()-mapToGlobal(geometry().topLeft()).y()+geometry().y();

    int itemViewX = pItemView->width();
    int Width =this->width();

    //  if (curY<height||curY>ItemViewY+height||curX<0||curX>itemViewX)
    //  {
    QComboBox::hidePopup();
    //  }
}
void MyCombox::showPopup()
{
    QAbstractItemView *pItemView = this->view();
    int ItemViewY = pItemView->height();
    int height=this->height();
    //控件上坐标xy位置
    int curX=QCursor::pos().x()-mapToGlobal(geometry().topLeft()).x()+geometry().x();
    int curY=QCursor::pos().y()-mapToGlobal(geometry().topLeft()).y()+geometry().y();

    int itemViewX = pItemView->width();
    int Width =this->width();
    if(curX>0.8*Width && curX<Width && curY>0){
        QComboBox::showPopup();
    }   
}

