#include "QSectionPropDlg.h"

QSectionPropDlg::QSectionPropDlg(QWidget *parent)
	: QDialog(parent)
{
	CreatGUI();
    m_IndexSectionName = 0;
}

QSectionPropDlg::~QSectionPropDlg()
{

}
void QSectionPropDlg::CreatGUI()
{
    this->setWindowTitle(tr("创建截面"));
    QGroupBox *gBox = new QGroupBox(this);
    QLabel *LName = new QLabel(tr("名称:"),this);
    m_NameEidt = new QLineEdit(this);
    QHBoxLayout *HLayout1 = new QHBoxLayout(gBox);
    HLayout1->addWidget(LName);
    HLayout1->addWidget(m_NameEidt);
    gBox->setStyleSheet("QGroupBox{border:none}"); 

    //--gbox1
    QGroupBox *gBox1 = new QGroupBox(tr("类别"),this);
    QVBoxLayout *VLayout2 = new QVBoxLayout(gBox1);
    m_btnCategory = new QButtonGroup(gBox1);
    m_RadioSolid = new QRadioButton(tr("实体"),gBox1);
    m_RadioShell = new QRadioButton(tr("壳"),gBox1);
    m_btnCategory->addButton(m_RadioSolid,0);
    m_btnCategory->addButton(m_RadioShell,1);
    VLayout2->addWidget(m_RadioSolid);
    VLayout2->addWidget(m_RadioShell);
    VLayout2->addStretch();
    m_RadioSolid->setChecked(true);
    m_RadioShell->setDisabled(true);
    //--gBox2
    QGroupBox *gBox2 = new QGroupBox(tr("类型"),this);
    QVBoxLayout *VLayout3 = new QVBoxLayout(gBox2);
    m_ListWidget1 = new QListWidget(gBox2);
    m_ListWidget2 = new QListWidget(gBox2);
    VLayout3->addWidget(m_ListWidget1);
    VLayout3->addWidget(m_ListWidget2);
    m_strListWidget1<<"Homogeneous"<<"Generalized plane strain";
    m_strListWidget2<<"Homogeneous"<<"Composite";
    for (int i=0;i<m_strListWidget1.size();i++)
    {
        m_ListWidget1->addItem(m_strListWidget1[i]);
    }
    for (int i=0;i<m_strListWidget2.size();i++)
    {
        m_ListWidget2->addItem(m_strListWidget2[i]);
    }
    m_ListWidget2->hide();
    //--gBox3
    QGroupBox *gBox3 = new QGroupBox(this);
    QHBoxLayout *HLayout4 = new QHBoxLayout(gBox3);
    m_BtnOK=new QPushButton(tr("确定"),gBox3);
    m_BtnCancel=new QPushButton(tr("取消"),gBox3);
    HLayout4->addStretch();
    HLayout4->addWidget(m_BtnOK);
    HLayout4->addWidget(m_BtnCancel);
    gBox3->setStyleSheet("QGroupBox{border:none}"); 
    //--gBox4
    m_gBox4 = new QGroupBox(tr("Solid,Homogeneous"),this);
    QHBoxLayout *HLayout5 = new QHBoxLayout(m_gBox4);
    m_SetionWidData = new QSectionDataWig(m_gBox4);
    HLayout5->addWidget(m_SetionWidData);
    //m_SetionWidData->CreatGUI(0);

    //--main
    QGridLayout *MainLayout = new QGridLayout(this);
    MainLayout->addWidget(gBox,1,1,1,2);
    MainLayout->addWidget(gBox1,2,1,1,1);
    MainLayout->addWidget(gBox2,2,2,1,1);
    MainLayout->addWidget(m_gBox4,3,1,1,2);
    MainLayout->addWidget(gBox3,4,1,1,2);
    this->setLayout(MainLayout);

    connect(m_BtnOK,SIGNAL(clicked()),this,SLOT(BtnOkSlot()));
    connect(m_BtnCancel,SIGNAL(clicked()),this,SLOT(BtnCancelSlot()));
    connect(m_btnCategory,SIGNAL(buttonClicked(int)),this ,SLOT(SetCategorySlot(int)));
    connect(m_ListWidget1,SIGNAL(currentRowChanged(int)),this,SLOT(SectionListSlot1(int)));
    connect(m_ListWidget2,SIGNAL(currentRowChanged(int)),this,SLOT(SectionListSlot2(int)));
    //connect(m_SetionWidData->m_comList)
    m_gBox4->hide();
}
//PutData 从界面读出数据
void QSectionPropDlg::PutData(SectionS &para)
{
    para.Name = m_Name;
    para.id = m_IndexSectionName;
}
//SetData 向界面写入数据
void QSectionPropDlg::SetData(SectionS para)
{
    m_para=para;
    m_Name = para.Name;
    m_NameEidt->setText(m_Name);
    m_IndexSectionName = para.id;
    m_SetionWidData->m_comList = para.MaterPara.MaterList;
    m_SetionWidData->m_Combox->clear();
    m_SetionWidData->m_Combox->addItems(m_SetionWidData->m_comList);
    if (m_SetionWidData->m_comList.size()>=m_para.MaterPara.MaterListIndex)
    {
         m_SetionWidData->m_Combox->setCurrentIndex(m_para.MaterPara.MaterListIndex);
    }
   

    //m_para.Name=para.Name;
   // m_para.id =para.id;
    ;
}
//SetData 向界面写入MatrialList
void QSectionPropDlg::SetData(QStringList m_comList)
{
    m_SetionWidData->m_comList = m_comList;
    m_SetionWidData->m_Combox->clear();
    m_SetionWidData->m_Combox->addItems(m_SetionWidData->m_comList);
    m_SetionWidData->m_Combox->setCurrentIndex(m_para.MaterPara.MaterListIndex);
}
//-Slot
//保存当前Section设置(id，style，name)
void QSectionPropDlg::BtnOkSlot()
{
    SectionS tmpSection;
    tmpSection.id = m_IndexSectionName+1;
    m_Name=m_NameEidt->text();
    tmpSection.Name = m_Name;
    int indexStyle;
    if (m_RadioShell->isChecked()){
        tmpSection.strCategory = tr("Shell");
    }else{
        tmpSection.strCategory = tr("Solid");   
    } 
    QString strBoxName = m_gBox4->title();
    int tmpI = strBoxName.indexOf(",");
    tmpSection.Styles =strBoxName.mid(tmpI+1);
    m_para.MaterPara.MaterListIndex = m_SetionWidData->m_Combox->currentIndex();
    emit emitSectionParam(tmpSection);
    this->accept();
}

void QSectionPropDlg::BtnCancelSlot()
{
    this->close();
}
void QSectionPropDlg::SectionListSlot1(int Row)
{
    if (Row <0){
        return;
    }
    int iCategory = m_btnCategory->checkedId();//获取需要显示的界面
    QString str = m_ListWidget1->item(Row)->text();
    int index=0;
    if (iCategory==0){//Solid
        if (str=="Homogeneous"){
            index=0;
            m_gBox4->setTitle(tr("Solid,Homogeneous"));
        }else{
            index=0;
            m_gBox4->setTitle(tr("Solid,Generalized plane strain"));
        }
    }
   
    //m_SetionWidData->CreatGUI(index);
}
void QSectionPropDlg::SectionListSlot2(int Row)
{
    if (Row <0){
        return;
    }
    int iCategory = m_btnCategory->checkedId();//获取需要显示的界面
    QString str = m_ListWidget2->item(Row)->text();
    int index=0;
    if (iCategory==1){//Shell
        index = 1;
        if (str=="Homogeneous"){
             m_gBox4->setTitle(tr("Shell,Homogeneous"));
        }else{
             m_gBox4->setTitle(tr("Shell,Composite"));
        } 
    }
   // m_SetionWidData->CreatGUI(index);
}
//根据当前Category不同，显示不同的List
void QSectionPropDlg::SetCategorySlot(int Index)
{
    if (Index == 0){
        m_ListWidget1->show();
        m_ListWidget2->hide();
    }else if (Index == 1){
        m_ListWidget2->show();
        m_ListWidget1->hide();
    }  
}