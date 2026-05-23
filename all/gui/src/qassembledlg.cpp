#include "qassembledlg.h"
QAssembleDlg* QAssembleDlg::instance = 0;

QAssembleDlg::QAssembleDlg(QWidget *parent)
    : QWidget(parent)
{
    QGroupBox *m_group = new QGroupBox(tr("集合"),this);
    m_group->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    QVBoxLayout *VLayout = new QVBoxLayout(m_group);
    m_TableWiget = new QTableWidget(m_group);
    m_TableWiget->setColumnCount(3);
    m_TableWiget->setRowCount(1);
    m_TableWiget->verticalHeaderItem(1);
    QStringList strList;
    strList<<"点集合"<<"单元集合"<<"面集合";
    m_TableWiget->setHorizontalHeaderLabels(strList);
    QComboBox *combox1=new QComboBox();
    combox1->addItem("Point Set1");
    combox1->addItem("Point Set2");
    combox1->setFrame(false);
    m_TableWiget->setCellWidget(0,0,combox1);

    QComboBox *combox2=new QComboBox();
    combox2->addItem("Element Set1");
    combox2->addItem("Element Set2");
    combox2->setFrame(false);
    m_TableWiget->setCellWidget(0,1,combox2);

    QComboBox *combox3=new QComboBox();
    combox3->addItem("Surface Set1");
    combox3->addItem("Surface Set2");
    combox3->setFrame(false);
    m_TableWiget->setCellWidget(0,2,combox3);
    /*QTableWidgetItem *tmpItem1=new QTableWidgetItem(tr("点集合"));
    m_TableWiget->setItem(0,0,tmpItem1);
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("单元集合"));
    m_TableWiget->setItem(0,1,tmpItem2);
    QTableWidgetItem *tmpItem3=new QTableWidgetItem(tr("面集合"));
    m_TableWiget->setItem(0,2,tmpItem3);*/
    VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addWidget(m_group);
}

QAssembleDlg::~QAssembleDlg()
{

}
QAssembleDlg* QAssembleDlg::GetInstance()
{

    if (instance == 0)
    {
        instance = new QAssembleDlg;
    }

    return instance;
}