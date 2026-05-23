#include "QMaterManagerDlg.h"


QMaterManagerDlg* QMaterManagerDlg::instance = 0;
QMaterManagerDlg::QMaterManagerDlg(QWidget *parent)
: QWidget(parent)
    //: QDialog(parent)
{
      CreatGUI();
      init();
       
}

QMaterManagerDlg::~QMaterManagerDlg()
{
   /* if (m_MaterProDlg!=NULL)
    {
        delete m_MaterProDlg;
        m_MaterProDlg = NULL;
    }*/
    m_MaterProListDlg.clear();
}

QMaterManagerDlg* QMaterManagerDlg::GetInstance()
{

    if (instance == 0)
    {
        instance = new QMaterManagerDlg();
    }

    return instance;
}

void QMaterManagerDlg::init()
{
    //m_MaterProDlg = NULL;
}
//--管理材料界面
void QMaterManagerDlg::CreatGUI()
{
    this->setWindowTitle(tr("Material Manager"));
    QGroupBox *m_group = new QGroupBox(tr("材料"),this);
    m_group->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    QGridLayout *GridLayout = new QGridLayout(m_group);
    m_BtnEdit = new QPushButton(tr("Edit"),m_group);
    m_BtnCreate = new QPushButton(tr("Create"),m_group);
    m_BtnDel = new QPushButton(tr("删除"),m_group);

    GridLayout->addWidget(m_BtnCreate,0,0,1,1);
    GridLayout->addWidget(m_BtnEdit,0,1,1,1);
    GridLayout->addWidget(m_BtnDel,0,2,1,1);
    m_ListWiget = new QListWidget(m_group);
    GridLayout->addWidget(m_ListWiget,1,0,1,3);

    QVBoxLayout *MainLayout = new QVBoxLayout(this); 
    MainLayout->addWidget(m_group);

    connect(m_BtnCreate,SIGNAL(clicked()),this,SLOT(BtnCreateSlot()));
    connect(m_BtnEdit,SIGNAL(clicked()),this,SLOT(BtnEditSlot()));
    connect(m_BtnDel,SIGNAL(clicked()),this,SLOT(BtnDelSlot()));
    connect(m_ListWiget,SIGNAL(currentRowChanged(int)),this,SLOT(MaterManageListSlot(int)));
    m_group->hide();
    m_BtnEdit->hide();
    m_BtnCreate->hide();
    m_BtnDel->hide();
    m_ListWiget->hide();
}


/*
1.description:--创建材料属性
2.
*/
int QMaterManagerDlg::BtnCreateSlot()
{ 	 
    //QMaterPropDlg *tmpM = new QMaterPropDlg(this);

    //QString str(QString("Material-%1").arg(m_para.id));
    //m_para.Name = str;
    //SectionS tmpSpara;
    //tmpSpara.MaterPara = m_para;
    ////m_MaterProDlg.push_back(QMaterPropDlg::GetInstance());
    //m_MaterProDlg.push_back(QMaterPropDlg::GetInstance());
    //QMaterPropDlg::GetInstance()->SetData(tmpSpara);
    //QMaterPropDlg::GetInstance()->show(); 
    //QMaterPropDlg::GetInstance()->raise();//最上层
    //QMaterPropDlg::GetInstance()->activateWindow();//激活
    //connect(QMaterPropDlg::GetInstance(),SIGNAL(emitMaterParam(SectionS)),this,SLOT(UpDataListSlot(SectionS)));
    

    //QMaterPropDlg *tmpM = new QMaterPropDlg(this);

    //QString str(QString("Material-%1").arg(m_para.id));
    //m_para.Name = str;
    //SectionS tmpSpara;
    //tmpSpara.MaterPara = m_para;
    //tmpM->SetData(tmpSpara);
    //m_MaterProListDlg.append(tmpM);
    //tmpM->show(); 
    //tmpM->raise();//最上层
    //tmpM->activateWindow();//激活
    //connect(tmpM,SIGNAL(emitMaterParam(SectionS)),this,SLOT(UpDataListSlot(SectionS)));
    
    m_MaterProDlg = m_MaterProDlg->GetInstance();//;= new QMaterPropDlg(this);
	//m_TTTDlg2= QHPTTTDlg->GetInstance2();

    //QString str(QString("Material-%1").arg(m_para.id));
    //m_para.Name = str;
    SectionS tmpSpara;
    tmpSpara.MaterPara = m_para;
    m_MaterProDlg->SetData(tmpSpara);

    //m_MaterProListDlg.append(m_MaterProDlg);
    //m_MaterProDlg->show(); 
    //m_MaterProDlg->raise();//最上层
    //m_MaterProDlg->activateWindow();//激活
    connect(m_MaterProDlg,SIGNAL(emitMaterParam(SectionS)),this,SLOT(UpDataListSlot(SectionS)));

    int ret = m_MaterProDlg->exec();
    //m_MaterProDlg->show();
    if (ret ==  QDialog::Accepted){
        m_MaterProListDlg.append(m_MaterProDlg);     
        return 1;
    }else if ( ret == QDialog::Rejected){
        return 0;
    } 
     return 0;
}

//--编辑材料属性
void QMaterManagerDlg::BtnEditSlot()
{
    int iCurrentRow;
    iCurrentRow = m_ListWiget->currentRow();
    if (iCurrentRow<0)
    {
        return;
    }
    //m_MaterProListDlg.at(iCurrentRow)->m_BarMenu->hide();
    //m_MaterProListDlg.at(iCurrentRow)->m_NameEidt->setEnabled(false);
    m_MaterProListDlg.at(iCurrentRow)->exec();
}
//--从section的Combox列表中，选择进行当前的编辑
bool QMaterManagerDlg::EditMaterial(int index)
{
    if (index<0) {return 0;}
    m_MaterProDlg->GetInstance()->m_NameEdit->setEnabled(false);
    int ret=m_MaterProDlg->GetInstance()->exec();

    if (ret == QDialog::Accepted){    
       return 1;
    }else if (ret == QDialog::Rejected){
       return 0;
    } 
    return 0;

}
//--删除材料属性
void QMaterManagerDlg::BtnDelSlot()
{
    int iCurrentRow;
    iCurrentRow = m_ListWiget->currentRow();
    if (iCurrentRow<0)
    {
        int bt = QMessageBox::warning(this, tr("Waring"), tr("Please Select a Material to delete..."), 
                                      QMessageBox::Ok,QMessageBox::Ok);
        return;
    }
    QMessageBox::StandardButton bt = QMessageBox::warning(this, tr("Waring"), 
                                     tr("Do you want to Delete the Material?"), 
                                     QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if (bt == QMessageBox::Yes)
    {
        m_MaterProListDlg.at(iCurrentRow)->m_IndexMaterName-=1;
        //connect(m_MaterProListDlg.at(iCurrentRow),SIGNAL(emitMaterParam(SectionS)),this,SLOT(UpDataListSlot(SectionS)));
       
        m_para.id = m_MaterProListDlg.at(iCurrentRow)->m_IndexMaterName;//para.MaterPara.id;
        m_para.MaterList.removeOne(m_para.Name);
        SectionS tmpSection;
        tmpSection.MaterPara = m_para;
        emit emitUpMaterListData(tmpSection);
        m_MaterProListDlg.removeAt(iCurrentRow);
        QListWidgetItem *item = m_ListWiget->takeItem(iCurrentRow);
        delete item;
    }
}

void QMaterManagerDlg::MaterManageListSlot(int Row)
{
    if (Row < 0)
    {
        return;
    }
    QString m_Name;
    m_Name = m_ListWiget->item(Row)->text();
}
//更新列表显示List
void QMaterManagerDlg::UpDataListSlot(SectionS para)
{
    m_para.id = para.MaterPara.id;
    int RowCnt = m_ListWiget->count();
    QString strName = para.MaterPara.Name;
    m_para.MaterList<<strName;
    para.MaterPara.MaterList = m_para.MaterList;
    for (int i=0; i<RowCnt; i++)
    {
        if (strName == m_ListWiget->item(i)->text())
        {
           // int bt = QMessageBox::warning(this, tr("Waring"), 
           //                                  tr("This Name already exited!"), 
           //                                  QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
    }
   m_ListWiget->addItem(strName);
   emit emitUpMaterListData(para);
}