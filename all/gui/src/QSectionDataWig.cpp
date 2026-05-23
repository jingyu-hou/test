#include "QSectionDataWig.h"
#include "QMaterManagerDlg.h"

QSectionDataWig::QSectionDataWig(QWidget *parent)
    : QWidget(parent)
{
 //   CreatGUI();
    QGroupBox *gbox = new QGroupBox(this);
    QHBoxLayout *HLayout = new QHBoxLayout(gbox);
    QLabel *MaterName = new QLabel(tr("Material:"),gbox);
    m_Combox = new QComboBox(gbox);
    m_Combox->addItems(m_comList);
    m_BtnMaterCreate = new QPushButton("Create",gbox);
    HLayout->addWidget(MaterName);
    HLayout->addWidget(m_Combox);
    HLayout->addWidget(m_BtnMaterCreate);
    setLayout(HLayout);
    connect(m_BtnMaterCreate,SIGNAL(clicked()),this,SLOT(BtnMaterCreateSlot()));
    gbox->setStyleSheet("QGroupBox{border:none}"); 
}

QSectionDataWig::~QSectionDataWig()
{

}
void QSectionDataWig::CreatGUI(int index)
{
//    if (index == 0)
//    {       
      /*  QGroupBox *gbox = new QGroupBox(this);
        QHBoxLayout *HLayout = new QHBoxLayout(gbox);
        QLabel *MaterName = new QLabel(tr("Material:"),gbox);
        m_Combox = new QComboBox(gbox);
        m_Combox->addItems(m_comList);
        m_BtnMaterCreate = new QPushButton("Create",gbox);
        HLayout->addWidget(MaterName);
        HLayout->addWidget(m_Combox);
        HLayout->addWidget(m_BtnMaterCreate);
        setLayout(HLayout);
        connect(m_BtnMaterCreate,SIGNAL(clicked()),this,SLOT(BtnMaterCreateSlot()));
        gbox->setStyleSheet("QGroupBox{border:none}"); */
//    }else{
    //    QGroupBox *gbox = new QGroupBox(this);
    //    QHBoxLayout *HLayout = new QHBoxLayout(gbox);
    //    QLabel *MaterName = new QLabel(tr("Material:"),gbox);
    //    m_Combox = new QComboBox(gbox);
    //    m_BtnMaterCreate = new QPushButton("Create",gbox);
    //    HLayout->addWidget(MaterName);
    //    HLayout->addWidget(m_Combox);
    //    HLayout->addWidget(m_BtnMaterCreate);
    //    setLayout(HLayout);
    //    connect(m_BtnMaterCreate,SIGNAL(clicked()),this,SLOT(BtnMaterCreateSlot()));
    //}
}
//
void QSectionDataWig::SetData(SectionS para)
{
    m_para.id = para.MaterPara.id;
}
void QSectionDataWig::BtnMaterCreateSlot()
{
    m_MaterManagerDlg=QMaterManagerDlg::GetInstance();
    int ret = QMaterManagerDlg::GetInstance()->BtnCreateSlot();
    
    if (ret == 0){
        return;
    }
    QString strName;
    strName = m_MaterManagerDlg->m_MaterProDlg->m_Name;
    if (m_Combox->findText(strName)==-1)
    {
        m_comList.append(strName);
        m_Combox->addItem(strName);
    }
}
