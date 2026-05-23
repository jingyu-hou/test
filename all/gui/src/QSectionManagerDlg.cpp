#include "QSectionManagerDlg.h"


QSectionManagerDlg* QSectionManagerDlg::instance = 0;
QSectionManagerDlg::QSectionManagerDlg(QWidget *parent)
:QWidget(parent)//: QDialog(parent)
{
    CreatGUI();
}

QSectionManagerDlg::~QSectionManagerDlg()
{
    ;
}

QSectionManagerDlg* QSectionManagerDlg::GetInstance()
{

    if (instance == 0)
    {
        instance = new QSectionManagerDlg;
    }

    return instance;
}
//--管理截面界面
void QSectionManagerDlg::CreatGUI()
{
    this->setWindowTitle(tr("Section Manager"));

    QGroupBox *m_group = new QGroupBox(tr("截面"),this);
    m_group->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    QGridLayout *GridLayout = new QGridLayout(m_group);
    m_BtnEdit = new QPushButton(tr("Edit"),m_group);
    m_BtnCreate = new QPushButton(tr("Create"),m_group);
    m_BtnDel = new QPushButton(tr("删除"),m_group);

    GridLayout->addWidget(m_BtnCreate,0,0,1,1);
    GridLayout->addWidget(m_BtnEdit,0,1,1,1);
    GridLayout->addWidget(m_BtnDel,0,2,1,1);
    QWidget *Vwid1 = new QWidget(this);
  
    m_TabWiget = new QTableWidget(m_group);
 
    //Vlayout->addWidget(m_TabWiget);
    GridLayout->addWidget(m_TabWiget,1,0,1,3);

    QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
    m_TabWiget->setHorizontalHeaderItem(0, __qtablewidgetitem);
    QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
    m_TabWiget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
    QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
    m_TabWiget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
    m_TabWiget->setColumnCount(2);
    QStringList str;
    str<<"Name"<<"Type";
    m_TabWiget->setHorizontalHeaderLabels(str);

    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addWidget(m_group);

    connect(m_BtnCreate,SIGNAL(clicked()),this,SLOT(BtnCreateSlot()));
    connect(m_BtnEdit,SIGNAL(clicked()),this,SLOT(BtnEditSlot()));
    connect(m_BtnDel,SIGNAL(clicked()),this,SLOT(BtnDelSlot()));
    //connect(m_TabWiget,SIGNAL(currentRowChanged(int)),this,SLOT(SectionManageListSlot(int)));
    m_BtnCreate->hide();
    m_BtnEdit->hide();
    m_BtnDel->hide();
    m_TabWiget->hide();
    m_group->hide();
    
}
//SLOT
//--创建截面属性
int QSectionManagerDlg::BtnCreateSlot()
{
    m_SectionDlg = new QSectionPropDlg(this);
    QString str(QString("Section-%1").arg(m_para.id));
    m_para.Name = str;
    m_SectionDlg->SetData(m_para);
    
   //  
   // m_SectionDlg->setModal(true);
   // m_SectionDlg->show();
   // m_SectionDlg->raise();//最上层
   // m_SectionDlg->activateWindow();//激活
    connect(m_SectionDlg,SIGNAL(emitSectionParam(SectionS)),this,SLOT(UpDataListSlot(SectionS)));
    int ret=m_SectionDlg->exec();
    if (ret == QDialog::Accepted){
        m_SectionProListDlg.append(m_SectionDlg);
        return 1;
       
    }else if(ret == QDialog::Rejected){
        return 0;
        
    }
    return 0;
    
    
}
//删除截面
void QSectionManagerDlg::BtnDelSlot()
{

}
//编辑截面
void QSectionManagerDlg::BtnEditSlot()
{
    int iCurrentRow;
    iCurrentRow = m_TabWiget->currentRow();
    if (iCurrentRow<0){
        int bt = QMessageBox::warning(this, tr("Waring"), tr("Please Select a Section to Edit..."), 
        QMessageBox::Ok,QMessageBox::Ok);
        return;
    }
    
   // m_SectionProListDlg.at(iCurrentRow)->m_SetionWidData->m_comList=m_para.MaterPara.MaterList;
    m_SectionProListDlg.at(iCurrentRow)->m_NameEidt->setEnabled(false);
    m_SectionProListDlg.at(iCurrentRow)->SetData(m_para.MaterPara.MaterList);
    m_SectionProListDlg.at(iCurrentRow)->exec();
   /* m_SectionProListDlg.at(iCurrentRow)->show();
    m_SectionProListDlg.at(iCurrentRow)->raise();
    m_SectionProListDlg.at(iCurrentRow)->activateWindow();*/
}

//void QSectionManagerDlg::SectionManageListSlot(int Row)
//{
//
//}
//更新WidgetList:Section的列表信息
void QSectionManagerDlg::UpDataListSlot(SectionS para)
{
    m_para.id = para.id;
    m_para.Name= para.Name;
    m_para.strCategory=para.strCategory;
    m_para.Styles = para.Styles;
    int RowCnt = m_TabWiget->rowCount();
    
    QString strName = para.Name;
    QString strStyle = para.strCategory+","+para.Styles;
    int j=0;
    for (int i=0; i<RowCnt; i++)
    {
        if(strName == m_TabWiget->item(i,j)->text())
        {
           /* int bt = QMessageBox::warning(this, tr("Waring"), 
                tr("This Name already exited!"), 
                QMessageBox::Ok, QMessageBox::Ok);  */
            m_TabWiget->item(i,j)->setText(strName);
            m_TabWiget->item(i,j+1)->setText(strStyle);
            return ;//para;
        }
    }
    m_TabWiget->setRowCount(RowCnt+1);
    m_TabWiget->setItem(RowCnt,j,new QTableWidgetItem());
    m_TabWiget->setItem(RowCnt,j+1,new QTableWidgetItem());
    m_TabWiget->item(RowCnt,j)->setText(strName);
    m_TabWiget->item(RowCnt,j+1)->setText(strStyle);
    
    //--发送给QPartDlg 整个的Setion
    //return para;
    
    //}
}
//更新WidgetList:Section中Combox的列表信息
void QSectionManagerDlg::UpDataMater(SectionS para){
    //SectionS tmp;
    //tmp=para;
    m_para.MaterPara.MaterList = para.MaterPara.MaterList;
}