#include "PElSfSetDlg.h"
#include <vtkRenderer.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include <vtkCellPicker.h>
#include <vtkSmartPointer.h>
#include <vtkIdTypeArray.h>

#include <vtkProperty.h>
#include "vtkVISUnShadeMesh.h"
vtkStandardNewMacro(ElInteractorStyle);
vtkStandardNewMacro(PointInteractorStyle);
PElSfSetDlg::PElSfSetDlg(QWidget *parent)
    : QDialog(parent)
{
    m_gpbox1=new QGroupBox(tr("创建类型"));
    m_PSetBtn = new QRadioButton(tr("点集合"),m_gpbox1);
    m_ElSetBtn = new QRadioButton(tr("单元集合"),m_gpbox1);
    m_SurfSetBtn = new QRadioButton(tr("表面集合"),m_gpbox1);
    QButtonGroup *groupBtn = new QButtonGroup();
    groupBtn->addButton(m_PSetBtn,      0);
    groupBtn->addButton(m_ElSetBtn,     1);
    groupBtn->addButton(m_SurfSetBtn,   2);
    m_PSetBtn->setChecked(true);

    m_checkSurf=new QCheckBox(tr("仅表面"),m_gpbox1);
    //m_fineSelect=new QCheckBox(tr("精选"),m_gpbox1);

    QHBoxLayout *layoutH1 = new QHBoxLayout(m_gpbox1);
    layoutH1->addWidget(m_PSetBtn);
    layoutH1->addWidget(m_ElSetBtn);
    layoutH1->addWidget(m_SurfSetBtn);
    layoutH1->addWidget(m_checkSurf);
   // layoutH1->addWidget(m_fineSelect);

    //--点集合
    m_gbWigPset = new QGroupBox(tr("点集合"));
    QVBoxLayout *m_gbWigPsetMainLayout = new QVBoxLayout(m_gbWigPset);
    QHBoxLayout *m_gbWigPsetH1=new QHBoxLayout();
    QHBoxLayout *m_gbWigPsetH2=new QHBoxLayout();
    m_PointSetClrBtn = new QPushButton(tr("清除"));
    m_PointSetEditBtn=new QPushButton(tr("编辑"));
    m_PointSetOkBtn = new QPushButton(tr("保存"));
    m_pickPointSetBtn = new QPushButton(tr("创建"));//QToolButton(m_gbWigPset);
    m_lab1=new QLabel(tr("名称:"),m_gbWigPset);
    m_editPsetName=new QLineEdit(m_gbWigPset);
    m_gbWigPsetH1->addWidget(m_lab1);
    m_gbWigPsetH1->addWidget(m_editPsetName);
    m_gbWigPsetH1->addWidget(m_pickPointSetBtn);
    m_gbWigPsetH1->addWidget(m_PointSetClrBtn);
    m_gbWigPsetH1->addWidget(m_PointSetEditBtn);
    m_gbWigPsetH1->addWidget(m_PointSetOkBtn);
    m_pickPointSetBtn->setIcon(QIcon(":/images/arrow.png"));
   
   
    //m_ElSetEditBtn=new QPushButton(tr("编辑"));
   // m_ElSetOkBtn = new QPushButton(tr("确定"));
    m_lab1_1=new QLabel(tr("点集合列表："),m_gbWigPset);
    m_PsetComb=new MyCombox(m_gbWigPset);
    m_gbWigPsetH2->addWidget(m_lab1_1);
    m_gbWigPsetH2->addWidget(m_PsetComb);
    m_gbWigPsetH2->addStretch();
    m_gbWigPsetMainLayout->addLayout(m_gbWigPsetH1);
    m_gbWigPsetMainLayout->addLayout(m_gbWigPsetH2);

    //--单元集合
    m_gbWigElset = new QGroupBox(tr("单元集合"));
    QVBoxLayout *m_gbWigElsetMainLayout = new QVBoxLayout(m_gbWigElset);
    QHBoxLayout *m_gbWigElsetH1=new QHBoxLayout();
    QHBoxLayout *m_gbWigElsetH2=new QHBoxLayout();
    //QHBoxLayout *m_gbWigElsetH1 = new QHBoxLayout(m_gbWigElset);
    m_pickElSetBtn = new QPushButton(tr("创建"));
    m_ElSetEditBtn=new QPushButton(tr("编辑"));
    m_ElSetOkBtn = new QPushButton(tr("保存"));
    m_ElSetClrBtn = new QPushButton(tr("清除"));
    m_lab2=new QLabel(tr("名称:"),m_gbWigElset);
    m_editElsetName=new QLineEdit(m_gbWigElset);
    m_gbWigElsetH1->addWidget(m_lab2);
    m_gbWigElsetH1->addWidget(m_editElsetName);
    m_gbWigElsetH1->addWidget(m_pickElSetBtn);
    m_gbWigElsetH1->addWidget(m_ElSetClrBtn);
    m_gbWigElsetH1->addWidget(m_ElSetEditBtn);
    m_gbWigElsetH1->addWidget(m_ElSetOkBtn);
    m_gbWigElsetH1->addStretch();
    m_pickElSetBtn->setIcon(QIcon(":/images/arrow.png"));
    m_gbWigElsetMainLayout->addLayout(m_gbWigElsetH1);

    m_lab2_1=new QLabel(tr("单元集合列表："),m_gbWigElset);
    m_ElSetComb=new MyCombox(m_gbWigElset);
    m_gbWigElsetH2->addWidget(m_lab2_1);
    m_gbWigElsetH2->addWidget(m_ElSetComb);
    m_gbWigElsetH2->addStretch();
    m_gbWigElsetMainLayout->addLayout(m_gbWigElsetH1);
    m_gbWigElsetMainLayout->addLayout(m_gbWigElsetH2);

   /* m_pickElSetBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_pickElSetBtn->setCheckable(true);
    m_pickElSetBtn->setText(tr("创建单元"));*/

    m_gbWigSfset = new QGroupBox(tr("表面集合"));
    QVBoxLayout *m_gbWigSurfsetMainLayout = new QVBoxLayout(m_gbWigSfset);
    QHBoxLayout *m_gbWigSfsetH0 = new QHBoxLayout(m_gbWigSfset);
    QHBoxLayout *m_gbWigSfsetH1 = new QHBoxLayout(m_gbWigSfset);
    QHBoxLayout *m_gbWigSfsetH2 = new QHBoxLayout(m_gbWigSfset);

    m_SurfPickerRBtn= new QRadioButton(tr("拾取"),m_gbWigSfset);
    m_SurfSelectRBtn = new QRadioButton(tr("框选"),m_gbWigSfset);
    m_gbWigSfsetH0->addWidget(m_SurfPickerRBtn);
    m_gbWigSfsetH0->addWidget(m_SurfSelectRBtn);
    m_gbWigSfsetH0->addStretch();

    m_pickSurfSetBtn = new QPushButton(tr("创建"));
    m_SurfSetClrBtn = new QPushButton(tr("清除"));
    m_SurfSetOkBtn = new QPushButton(tr("保存"));
    m_SurfSetEditBtn = new QPushButton(tr("编辑"));
    m_lab3=new QLabel(tr("名称:"),m_gbWigSfset);
    m_editSurfsetName=new QLineEdit(m_gbWigSfset);
    m_gbWigSfsetH1->addWidget(m_lab3);
    m_gbWigSfsetH1->addWidget(m_editSurfsetName);
    m_gbWigSfsetH1->addWidget(m_pickSurfSetBtn);
    m_gbWigSfsetH1->addWidget(m_SurfSetClrBtn);
    m_gbWigSfsetH1->addWidget(m_SurfSetEditBtn);
    m_gbWigSfsetH1->addWidget(m_SurfSetOkBtn);
    m_gbWigSfsetH1->addStretch();
    m_pickSurfSetBtn->setIcon(QIcon(":/images/arrow.png"));

    m_lab3_1=new QLabel(tr("表面集合列表："),m_gbWigSfset);
    m_SurfSetComb=new MyCombox(m_gbWigSfset);
    m_gbWigSfsetH2->addWidget(m_lab3_1);
    m_gbWigSfsetH2->addWidget(m_SurfSetComb);
    m_gbWigSfsetH2->addStretch();
 
    m_gbWigSurfsetMainLayout->addLayout(m_gbWigSfsetH0);
    m_gbWigSurfsetMainLayout->addLayout(m_gbWigSfsetH1);
    m_gbWigSurfsetMainLayout->addLayout(m_gbWigSfsetH2);
    //m_pickElSetBtn = new QPushButton(tr("创建"));
    
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->addWidget(m_gpbox1);
    mainlayout->addWidget(m_gbWigPset);
    mainlayout->addWidget(m_gbWigElset);
    mainlayout->addWidget(m_gbWigSfset);

    setLayout(mainlayout);

    QButtonGroup *groupPointBtn = new QButtonGroup();
    groupPointBtn->addButton(m_pickPointSetBtn,      0);
    groupPointBtn->addButton(m_PointSetClrBtn,   1);
    groupPointBtn->addButton(m_PointSetEditBtn,     2);
    groupPointBtn->addButton(m_PointSetOkBtn,   3);

    QButtonGroup *groupELBtn = new QButtonGroup();
    groupELBtn->addButton(m_pickElSetBtn, 0);
    groupELBtn->addButton(m_ElSetClrBtn, 1);
    groupELBtn->addButton(m_ElSetEditBtn,     2);
    groupELBtn->addButton(m_ElSetOkBtn,   3);

    QButtonGroup *groupSurfBtn = new QButtonGroup();
    groupSurfBtn->addButton(m_pickSurfSetBtn, 0);
    groupSurfBtn->addButton(m_SurfSetClrBtn, 1);
    groupSurfBtn->addButton(m_SurfSetEditBtn,     2);
    groupSurfBtn->addButton(m_SurfSetOkBtn,   3);

    QButtonGroup *groupSurfRBtn=new QButtonGroup();
    groupSurfRBtn->addButton(m_SurfPickerRBtn,0);
    groupSurfRBtn->addButton(m_SurfSelectRBtn,1);
    connect(groupBtn, SIGNAL(buttonClicked(int)), this, SLOT(StyleSlot(int)));    
    connect(groupPointBtn,SIGNAL(buttonClicked(int)),this, SLOT(PointGroupBtnSlot(int)));
    connect(groupELBtn,SIGNAL(buttonClicked(int)),this,SLOT(ElGroupBtnSlot(int)));
    connect(groupSurfBtn,SIGNAL(buttonClicked(int)),this,SLOT(SurfGroupBtnSlot(int)));
    connect(groupSurfRBtn,SIGNAL(buttonClicked(int)),this,SLOT(SurfGroupRBtnSlot(int)));
 

    connect(m_PsetComb,SIGNAL(clicked(int,int)),this,SLOT(PointcomBoxSlot()));
    connect(m_ElSetComb,SIGNAL(clicked(int,int)),this,SLOT(ElcomBoxSlot()));
    connect(m_SurfSetComb,SIGNAL(clicked(int,int)),this,SLOT(SurfcomBoxSlot()));
    connect(m_checkSurf,SIGNAL(clicked()),this,SLOT(ChangePElIncludeSlot()));
    //connect(m_fineSelect,SIGNAL(clicked()),this,SLOT(ChangeFineorNotSlot()));
    inpObj_=0;
    pointPickCall_ = 0;
    m_hightLightPointStyle=NULL;m_hightLightSurfPickerStyle=NULL;m_hightLightElStyle=NULL;


    m_bfine=false;

    m_SurfSelectRBtn->hide();m_SurfPickerRBtn->hide();
    //m_checkSurf->hide();
    //m_fineSelect->hide(); 
    //--init
    m_ActorPElSet=0;m_ActorSurf=0;
    inpObj_=0;
    m_AllPointSetSaveMap.clear();m_AllElSetSaveMap.clear();
    m_AllSurfSetSaveMap.clear();
}

PElSfSetDlg::~PElSfSetDlg()
{

    clean();

    if (m_surfVTKPolyData!=NULL)
    {
        m_surfVTKPolyData->Delete();m_surfVTKPolyData=0;
    }
      
    if (m_PolyDataInput!=NULL)
    {
        m_PolyDataInput->Delete();m_PolyDataInput=0;
    }
    
    if (m_OrgWholePointDataInput!=NULL)
    {
        m_OrgWholePointDataInput->Delete();m_OrgWholePointDataInput=0;
    }
    
   /* if (m_OrgWholeCellDataInput!=NULL)
    {
        m_OrgWholeCellDataInput->Delete();m_OrgWholeCellDataInput=0;
    }*/
    if (m_WholePointDataInput!=NULL)
    {
        m_WholePointDataInput->Delete();m_WholePointDataInput=0;
    }
    if (m_WholeCellDataInput!=NULL)
    {
        m_WholeCellDataInput->Delete();m_WholeCellDataInput=0;
    }
    
    if (m_WholeSurfDataInput!=NULL)
    {
        m_WholeSurfDataInput->Delete();m_WholeSurfDataInput=0;
    }
    if (m_areaPicker!=NULL)
    {
         m_areaPicker->Delete();m_areaPicker=0;
    }
}
void PElSfSetDlg::clean()
{
    if (m_ActorPElSet != NULL)
    {
        if (inpObj_->GetBindedRenderer()!=NULL)
        {
            inpObj_->GetBindedRenderer()->RemoveActor(m_ActorPElSet);
        }
        m_ActorPElSet->Delete(); m_ActorPElSet=NULL;
    }

    if (m_hightLightElStyle !=NULL) 
    {
        m_hightLightElStyle->ClearCurrentSelectedMap();
    }
    if (m_hightLightPointStyle !=NULL)
    {
        m_hightLightPointStyle->ClearCurrentSelectedMap();
    }
   
    QMapIterator<QString, QStringList> it3(this->m_AllPointSetSaveMap);
    while (it3.hasNext()){
        it3.next();
        m_AllPointSetSaveMap.remove(it3.key());
    }
    m_AllPointSetSaveMap.clear();
    
    QMapIterator<QString, QStringList> it4(this->m_AllElSetSaveMap);
    while (it4.hasNext()){
        it4.next();
      /*  QMapIterator<int,vtkActor*> it44 (it4.value());
        while (it44.hasNext())
        {   
            it44.next();
            it44.value()->GetProperty()->EdgeVisibilityOff();
            it44.value()->VisibilityOff();
            inpObj_->GetBindedRenderer()->RemoveActor(it44.value());
        }*/
        m_AllElSetSaveMap.remove(it4.key());
    }
    m_AllElSetSaveMap.clear();

    //QMapIterator<QString, QMap<int,vtkActor*> > it5(this->m_AllSurfSetSaveMap);
     QMapIterator<QString, QStringList> it5(this->m_AllSurfSetSaveMap);
    while (it5.hasNext()){
        it5.next();
       /* QMapIterator<int,vtkActor*> it55 (it5.value());
        while (it55.hasNext())
        {   
            it55.next();
            it55.value()->GetProperty()->EdgeVisibilityOff();
            it55.value()->VisibilityOff();
            inpObj_->GetBindedRenderer()->RemoveActor(it55.value());
        }*/
        m_AllSurfSetSaveMap.remove(it5.key());
    }
    m_AllSurfSetSaveMap.clear();

    //--
    m_hightLightElStyle=0;
    m_hightLightPointStyle=0;
    m_hightLightSurfPickerStyle=0;

	//--COMBOX EDIT CLEAR
	m_PsetComb->clear();m_ElSetComb->clear();m_SurfSetComb->clear();
	m_editPsetName->clear();m_editElsetName->clear();m_editSurfsetName->clear();
}
void PElSfSetDlg::StyleSlot(int Id)
{
    if (Id<0)return;
    switch (Id){
        case 0: m_gbWigPset->show();m_checkSurf->show();
                m_gbWigElset->hide();m_gbWigSfset->hide();break;
        case 1: m_gbWigElset->show();m_checkSurf->hide();
                m_gbWigPset->hide();m_gbWigSfset->hide();break;
        case 2: m_gbWigSfset->show();m_checkSurf->hide();
                m_gbWigElset->hide();m_gbWigPset->hide();break;
        default:m_gbWigPset->show();m_checkSurf->hide();
                m_gbWigElset->hide();m_gbWigSfset->hide();break;        
    }
}
//checkbox 
//      check: only surface data
//      no check: all data     
void PElSfSetDlg::ChangePElIncludeSlot()
{
    if (m_checkSurf->isChecked())//仅表面
    {
       m_WholePointDataInput = m_PolyDataInput;
      // m_WholeCellDataInput = m_PolyDataInput;
       if (!inpObj_){
           inpObj_->SetMeshActorShow(false);
       }
       //m_AllSurfDataInput = m_PolyDataInput;
    }else{
       m_WholePointDataInput = m_OrgWholePointDataInput;
       //m_WholeCellDataInput = m_OrgWholeCellDataInput;
      // m_AllSurfDataInput = m_OrgAllSurfDataInput;
    }
}
//1:fine；0:select (can't edit)
//void PElSfSetDlg::ChangeFineorNotSlot()
//{    
//    m_bfine=m_fineSelect->isChecked();//true;   
//}
void PElSfSetDlg::PointGroupBtnSlot(int btnId)
{
    if (btnId<0)return;
    if (!inpObj_)  return;
    m_pickPointSetBtn->setDown(false);
    m_PointSetClrBtn->setDown(false);
    m_PointSetEditBtn->setDown(false);
    m_PointSetOkBtn->setDown(false);

    switch (btnId){
        case 0:{
            //inpObj_->SetMeshActorShow(false);
            m_pickPointSetBtn->setDown(true);
            PointSetSelectedPick();
           }break;
        case 1:{
            m_PointSetClrBtn->setDown(true);
            PointSetClear();
            inpObj_->SetMeshActorShow(true);
           }break;
        case 2:{
            inpObj_->SetMeshActorShow(false);
            m_PointSetEditBtn->setDown(true);
            PointSetEdit();
           }break;
        case 3:{
            m_PointSetOkBtn->setDown(true);
            PointSetOk();
            inpObj_->SetMeshActorShow(true);
           }break;
        default:break;
    }
}

void PElSfSetDlg::ElGroupBtnSlot(int btnId)
{
    if (btnId<0)return;
    if (!inpObj_)  return;
   /* m_pickPointSetBtn->setDown(false);
    m_PointSetClrBtn->setDown(false);
    m_PointSetEditBtn->setDown(false);
    m_PointSetOkBtn->setDown(false);*/

    m_pickElSetBtn->setDown(false);
    m_ElSetClrBtn->setDown(false);
    m_ElSetEditBtn->setDown(false);
    m_ElSetOkBtn->setDown(false);

    inpObj_->SetMeshActorShow(false);//20210809

    switch (btnId){
        case 0:{
            //m_pickPointSetBtn->setDown(true);
            //PointSetSelectedPick();
            //inpObj_->SetMeshActorShow(false);//20210809
            m_pickElSetBtn->setDown(true);
            ElSetSelectedPick();
           }break;
        case 1:{
            m_ElSetClrBtn->setDown(true);
            ElSetClear();
            inpObj_->SetMeshActorShow(true);
           }break;
        case 2:{
            //inpObj_->SetMeshActorShow(false);//20210809
            ElSetEdit();
           }break;
        case 3:{
            //m_PointSetOkBtn->setDown(true);
            //PointSetOk();
            ElSetOK();
            inpObj_->SetMeshActorShow(true);
           }break;
        default:break;
    }
}

void PElSfSetDlg::SurfGroupBtnSlot(int btnId)
{
    if (btnId<0)return;
    if (!inpObj_)  return;
    /*m_pickPointSetBtn->setDown(false);
    m_PointSetClrBtn->setDown(false);
    m_PointSetEditBtn->setDown(false);
    m_PointSetOkBtn->setDown(false);*/
   
    switch (btnId){
        case 0:{
            //m_pickPointSetBtn->setDown(true);
            inpObj_->SetMeshActorShow(false);
            SurfSetSelectedPick();
               }break;
        case 1:{
            //m_PointSetClrBtn->setDown(true);
            SurfSetClear();
            inpObj_->SetMeshActorShow(true);
               }break;
        case 2:{
            //m_PointSetEditBtn->setDown(true);
            //PointSetEdit();
            inpObj_->SetMeshActorShow(false);
            SurfSetEdit();
               }break;
        case 3:{
            //m_PointSetOkBtn->setDown(true);
           // PointSetOk();
            SurfSetOK();
            inpObj_->SetMeshActorShow(true);
               }break;
        default:break;
    }
}
void PElSfSetDlg::SurfGroupRBtnSlot(int btnId)
{
    if (btnId<0)return;
}
void PElSfSetDlg::PointSetClear()
{
   if (m_hightLightPointStyle==0) return; 
   m_hightLightPointStyle->ClearCurrentSelectedMap();
   m_hightLightPointStyle->SetLeftButtonUp(false);
}
void PElSfSetDlg::ElSetClear()
{
    if (m_hightLightElStyle ==NULL) return;
    m_hightLightElStyle->ClearCurrentSelectedMap();
    m_hightLightElStyle->SetLeftButtonUp(false);
}
void PElSfSetDlg::SurfSetClear()
{
    if (m_hightLightSurfPickerStyle ==NULL) return;
    m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    m_hightLightSurfPickerStyle->SetLeftButtonUp(false);

}
void PElSfSetDlg::PointSetOk()
{
   if (m_hightLightPointStyle==NULL) return;
    
    DecodeGenerateS tmpData;
    tmpData.iDataGenerate=1;//按点解析
    tmpData.strDataList=m_hightLightPointStyle->GetFilterId();//GetCurrentSelected(m_PointSetSaveMap);
    m_hightLightPointStyle->SetFilterActor(tmpData.strDataList);
    if (tmpData.strDataList.isEmpty())
    {
        QMessageBox::StandardButton bt= QMessageBox::question(this, tr(""), tr("当前并未选择，是否保存为空？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);

        if(bt == QMessageBox::No) return;
        else tmpData.strDataList<<"empty";
    }
    m_hightLightPointStyle->SetLeftButtonUp(false);
    QString str=m_editPsetName->text();
    if (m_PsetComb->findText(str)==-1){
        m_PsetComb->addItem(str);
    }
    int idcomb=m_PsetComb->findText(str);
    if (idcomb!=-1)
    {
        m_PsetComb->setCurrentIndex(idcomb);
    }
    inpObj_->SetNset(str, tmpData);
    emitCurrentPSet(str);
    //m_hightLightPointStyle->ClearCurrentSelectedMap();
    emitCurrentSetsData(str,COM_ENUM_NSET,tmpData);
    m_AllPointSetSaveMap.insert(str, tmpData.strDataList);

    //保存当前点集合 名字+数据(Name,(IdShow,Pos));
   /* NElSurfChsS tmp;
    tmp.strName="PSet1";
    tmp.iType=0;
    emitPsetHighLight(tmp);*/
}
void PElSfSetDlg::ElSetOK()
{
 if (m_hightLightElStyle==NULL) return;
    
    DecodeGenerateS tmpData;
    tmpData.iDataGenerate=1;//按点解析
    tmpData.strDataList=m_hightLightElStyle->GetFilterId();//m_hightLightElStyle->GetCurrentSelected(m_ElSetSaveMap);
    m_hightLightElStyle->SetFilterActor(tmpData.strDataList);
    
    if (tmpData.strDataList.isEmpty())
    {
        QMessageBox::StandardButton bt= QMessageBox::question(this, tr(""), tr("当前并未选择，是否保存为空？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);

        if(bt == QMessageBox::No) return;
        else tmpData.strDataList<<"empty";
    }
	
    m_hightLightElStyle->SetLeftButtonUp(false);
	
    QString str=m_editElsetName->text();// m_editPsetName->text();
    if (m_ElSetComb->findText(str)==-1){
        m_ElSetComb->addItem(str);
    }
    int idcomb=m_ElSetComb->findText(str);
    if (idcomb!=-1)
    {
        m_ElSetComb->setCurrentIndex(idcomb);
    }
	m_AllElSetSaveMap.insert(str, tmpData.strDataList);//m_ElSetSaveMap);

	QStringList tmpElementData(tmpData.strDataList);
	for (int kk=0;kk<tmpElementData.size();kk++)
	{
		int dd=tmpElementData.at(kk).toInt()+inpObj_->GetCellBaseId();//获取ElementBaseId
		tmpElementData[kk]=QString("%1").arg(dd);
	}
	tmpData.strDataList=tmpElementData;
    inpObj_->SetElset(str, tmpData);
    emitCurrentElSet(str);
    //m_hightLightPointStyle->ClearCurrentSelectedMap();
	 emitCurrentSetsData(str,COM_ENUM_ELSET,tmpData);
    
    //保存当前点集合 名字+数据(Name,(IdShow,Pos));
   /* NElSurfChsS tmp;
    tmp.strName="PSet1";
    tmp.iType=0;
    emitPsetHighLight(tmp);*/
}
void PElSfSetDlg::SurfSetOK()
{
    if (m_hightLightSurfPickerStyle==NULL) return;
    QStringList strDataList=m_hightLightSurfPickerStyle->GetFilterId();//GetCurrentSelected(m_SurfSetSaveMap);
    m_hightLightSurfPickerStyle->SetFilterActor(strDataList);
    if (strDataList.isEmpty()){
        QMessageBox::warning(this, tr(""), tr("当前并未选择，不能保存为空"), QMessageBox::Ok, QMessageBox::Ok);
        return;  
    }
    //m_hightLightSurfPickerStyle->SetLeftButtonUp(false);
    QString strText=m_editSurfsetName->text();// m_editPsetName->text();
    if (m_SurfSetComb->findText(strText)==-1){
        m_SurfSetComb->addItem(strText);
    }
    int idcomb=m_SurfSetComb->findText(strText);
    if (idcomb!=-1)
    {
        m_SurfSetComb->setCurrentIndex(idcomb);
    }
    //--<-->surface data
    const QMap<QString, QString> *varMap =inpObj_->GetElementSurfFilterMap();
    QMap<QString, QString>::const_iterator it=varMap->begin();
    QMap<QString,QString> elsetSnElementIdMap;
    for(int kk=0;kk<strDataList.size();kk++){ 
        int deta=strDataList.at(kk).toInt()-1; 
        QStringList strLTmp=(it+deta).value().split(",");
        QString strTmp1=strLTmp.at(0);
        QString strTmp2=strLTmp.at(1);
        if(elsetSnElementIdMap.find(strTmp2)==elsetSnElementIdMap.end()){
            elsetSnElementIdMap.insert(strTmp2,strTmp1);
        }else{
            QString strT1=elsetSnElementIdMap[strTmp2];
            strT1=strT1+","+strTmp1;
            elsetSnElementIdMap.insert(strTmp2,strT1);
        }
    }
    
     QStringList StrListSurfNameSn;StrListSurfNameSn.clear();
     QMap<QString, QString>::Iterator it2 = elsetSnElementIdMap.begin();
     for (it2; it2!=elsetSnElementIdMap.end(); it2++)
     {
         DecodeGenerateS tmpData;
         tmpData.iDataGenerate = 1;
         tmpData.strDataList=it2.value().split(",");
         QString strName = strText+"_S"+it2.key();
         StrListSurfNameSn<<strName+",S"+it2.key();
         inpObj_->SetElset(strName, tmpData);
         emitCurrentElSet(strName);
         emitCurrentSetsData(strName, COM_ENUM_ELSET, tmpData);
     }
     DecodeGenerateS tmpData;
     tmpData.iDataGenerate=1;//按点解析
     tmpData.strDataList=StrListSurfNameSn;
     inpObj_->SetSurfset(strText, StrListSurfNameSn);
     emitCurrentSurfSet(strText);
     emitCurrentSetsData(strText,COM_ENUM_SURFSET,tmpData);
     //m_hightLightPointStyle->ClearCurrentSelectedMap();
     m_AllSurfSetSaveMap.insert(strText, strDataList);
}
void PElSfSetDlg::PointSetEdit()
{
  
    if (m_hightLightElStyle!=0)
    {
        m_hightLightElStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->ClearCurrentSelectedMap();
    }
    if (m_hightLightSurfPickerStyle!=0)
    {
        m_hightLightSurfPickerStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    }  
    m_hightLightPointStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    m_hightLightPointStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    m_hightLightPointStyle->ClearCurrentSelectedMap();
    m_ir->SetInteractorStyle(m_hightLightPointStyle);
    m_hightLightPointStyle->SetLeftButtonUp(true);
    m_hightLightPointStyle->setMapData(m_AllPointSetSaveMap[m_PsetComb->currentText()]);
}
void PElSfSetDlg::ElSetEdit()
{
   
    if (m_hightLightPointStyle!=0)
    {
        m_hightLightPointStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->ClearCurrentSelectedMap();
    }  
    if (m_hightLightSurfPickerStyle!=0)
    {
        m_hightLightSurfPickerStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    }  
    m_hightLightElStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    m_hightLightElStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    m_hightLightElStyle->ClearCurrentSelectedMap();
    m_ir->SetInteractorStyle(m_hightLightElStyle);
    m_hightLightElStyle->SetLeftButtonUp(true);
    m_hightLightElStyle->setMapData(m_AllElSetSaveMap[m_ElSetComb->currentText()]);
}

void PElSfSetDlg::SurfSetEdit()
{

    if (m_hightLightPointStyle!=0)
    {
        m_hightLightPointStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->ClearCurrentSelectedMap();
    } 
    if (m_hightLightElStyle!=0)
    {
        m_hightLightElStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->ClearCurrentSelectedMap();
    } 
    m_hightLightSurfPickerStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    m_hightLightSurfPickerStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    m_ir->SetInteractorStyle(m_hightLightSurfPickerStyle);
    m_hightLightSurfPickerStyle->SetLeftButtonUp(true);
    m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    m_hightLightSurfPickerStyle->setMapData(m_AllSurfSetSaveMap[m_SurfSetComb->currentText()]);

}
//--不包含内部点的点集合选择
void PElSfSetDlg::PointSetSelectedPick()
{
    if (!inpObj_)  return;
 
    if (m_hightLightSurfPickerStyle != NULL)
    {
        m_hightLightSurfPickerStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    }

    if (m_hightLightElStyle!=NULL)
    {
        m_hightLightElStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->ClearCurrentSelectedMap();
    }

    m_ActorSurf->VisibilityOff();
    m_ActorPElSet->VisibilityOn();

    if (m_hightLightPointStyle==NULL)
    {
        m_hightLightPointStyle = PointInteractorStyle::New();
        m_hightLightPointStyle->SetPoints(m_WholePointDataInput);//(m_PolyDataInput);
        m_hightLightPointStyle->SetPointsOld(inpObj_->GetWholeData("hell"));
        m_hightLightPointStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_ir->SetInteractorStyle(m_hightLightPointStyle);
    }else{
        m_hightLightPointStyle->SetPoints(m_WholePointDataInput);//(m_PolyDataInput);
        m_hightLightPointStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->ClearCurrentSelectedMap();
        m_ir->SetInteractorStyle(m_hightLightPointStyle);
    }
    m_hightLightPointStyle->StartSelect();
    m_hightLightPointStyle->SetLeftButtonUp(true);

    NElSurfChsS tmp;
    tmp.strName="新建";
    tmp.iType=COM_ENUM_NSET;
    emitPsetHighLight(tmp);
}

void PElSfSetDlg::ElSetSelectedPick()
{
    if (!inpObj_)  return;

    if (m_hightLightPointStyle != NULL)
    {
        m_hightLightPointStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->ClearCurrentSelectedMap();
    }
    if (m_hightLightSurfPickerStyle != NULL)
    {
        m_hightLightSurfPickerStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    }

    m_ActorSurf->VisibilityOff();
    m_ActorPElSet->VisibilityOn();

    if (m_hightLightElStyle == NULL)
    {
        m_hightLightElStyle = ElInteractorStyle::New();
        m_hightLightElStyle->SetPolyDataOld(inpObj_->GetWholeData("hell"));
        m_hightLightElStyle->SetPolyData(m_WholeCellDataInput);//(m_PolyDataInput);//(input);
        m_hightLightElStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer()); 
        
        m_hightLightElStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_ir->SetInteractorStyle(m_hightLightElStyle);
    }else{  
        m_hightLightElStyle->SetPolyData(m_WholeCellDataInput);//(m_PolyDataInput);//(input);
        m_hightLightElStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer()); 
        m_hightLightElStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_ir->SetInteractorStyle(m_hightLightElStyle);
        m_hightLightElStyle->ClearCurrentSelectedMap();    
    }
    m_hightLightElStyle->StartSelect();
    m_hightLightElStyle->SetLeftButtonUp(true);

    NElSurfChsS tmp;
    tmp.strName="新建";
    tmp.iType=COM_ENUM_ELSET;
    emitPsetHighLight(tmp);
}


void PElSfSetDlg::SurfSetSelectedPick()
{
   if (!inpObj_)  return;

  if (m_hightLightPointStyle != NULL)
    {
        m_hightLightPointStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->ClearCurrentSelectedMap();
    }
  
    if (m_hightLightElStyle!=NULL)
    {
        m_hightLightElStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->ClearCurrentSelectedMap();
    }

    m_ActorPElSet->VisibilityOff();
    m_ActorSurf->VisibilityOn();

    if (m_hightLightSurfPickerStyle==0)
    {
        m_hightLightSurfPickerStyle = ElInteractorStyle::New();
        m_hightLightSurfPickerStyle->SetPolyDataOld(inpObj_->GetSurfGridData());
        m_hightLightSurfPickerStyle->SetPolyData(m_WholeSurfDataInput); 
        m_hightLightSurfPickerStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_ir->SetInteractorStyle(m_hightLightSurfPickerStyle);
    }else{
        m_hightLightSurfPickerStyle->SetPolyData(m_WholeSurfDataInput);
        m_hightLightSurfPickerStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_ir->SetInteractorStyle(m_hightLightSurfPickerStyle);
        m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    }
    //m_hightLightSurfPickerStyle->setFineStyle(m_bfine);
    m_hightLightSurfPickerStyle->StartSelect();

    
    //m_hightLightSurfPickerStyle->SetInteractor(m_ir);
    m_hightLightSurfPickerStyle->SetLeftButtonUp(true);

    NElSurfChsS tmp;
    tmp.strName="新建";
    tmp.iType=COM_ENUM_SURFSET;
    emitPsetHighLight(tmp);
}
//void PElSfSetDlg::SetObject(InpDataVIS *inpObj)
//{
//    clean();
//    inpObj_=InpObj;
//    m_renWin = inpObj_->GetBindedRenderer()->GetRenderWindow();
//    m_ir = m_renWin->GetInteractor();
//}
void PElSfSetDlg::UpDataInpElSetDataSlot(QStringList strActorNameProp)//
{
	int nSize=strActorNameProp.size();
	if (nSize<0)return;
	disElSetMap_.clear();

	for (int kk=0;kk<nSize;kk++)
	{
		QStringList strL = strActorNameProp.at(kk).split(",");
		bool f=false;
		if(strL[0].toInt()==1)f=true;
		//data.strName=strL[1];
		//inpVIS_.SetBCCheckVisible(f,data);
		if (f==true){//创建数据
			disElSetMap_.insert(strL[1],inpObj_->GetWholeData(strL[1]));
			//vtkIdFilter 

		}
		//ActorPElSetMap_
	}
	CreateFilterData(strActorNameProp);
}
// ListData->inp file elementset Name List
//
void PElSfSetDlg::CreateFilterData(QStringList ListData)
{
	ActorPElSetMap_.clear();ActorSurfSetMap_.clear();
	QMap<QString, vtkUnstructuredGrid*>::iterator it=disElSetMap_.begin();
	for (it;it!=disElSetMap_.end();it++)
	{
		vtkIdFilter *idFilter= vtkIdFilter::New();
		idFilter->SetInput(it.value());
		idFilter->SetIdsArrayName("WelCME");
		idFilter->PointIdsOn();
		idFilter->FieldDataOn();
		idFilter->CellIdsOn();
		idFilter->Update();

		//------点集合滤波；
		vtkVertexGlyphFilter *PointFilter = vtkVertexGlyphFilter::New();
		PointFilter->SetInputConnection(idFilter->GetOutputPort());
		PointFilter->Update();
		m_WholePointDataInput=PointFilter->GetOutput();
		//------单元集合滤波；
		vtkCellCenters *cellFilter = vtkCellCenters::New();
		cellFilter->SetInputConnection(idFilter->GetOutputPort());
		cellFilter->VertexCellsOn();
		cellFilter->Update();
		m_WholeCellDataInput=cellFilter->GetOutput();


		vtkExtractEdges *edge = vtkExtractEdges::New();
		edge->SetInput(it.value());//内部显示
		vtkDataSetMapper *mapper = vtkDataSetMapper::New();
		mapper->SetInput(edge->GetOutput());//(inpObj_->GetWholeData());
		mapper->ScalarVisibilityOff();

		m_ActorPElSet=vtkActor::New();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
		m_ActorPElSet->SetMapper(mapper);
		m_ActorPElSet->GetProperty()->SetPointSize(5);
		ActorPElSetMap_.insert(it.key(), m_ActorPElSet);
		//--点、单元仅进行表面选择

		vtkDataSetSurfaceFilter *ELPointOnlySurFilter=vtkDataSetSurfaceFilter::New();
		ELPointOnlySurFilter->SetInputConnection(idFilter->GetOutputPort());
		ELPointOnlySurFilter->Update();
		m_PolyDataInput=ELPointOnlySurFilter->GetOutput();

		//--表面集合拾取
		//vtkIdFilter *idSurfFilter= vtkIdFilter::New();
		//idSurfFilter->SetInput(inpObj_->GetSurfGridData());
		//idSurfFilter->SetIdsArrayName("WelCME");
		//idSurfFilter->CellIdsOn();
		//idSurfFilter->Update();

		//vtkCellCenters *cellSurfFilter = vtkCellCenters::New();
		//cellSurfFilter->SetInputConnection(idSurfFilter->GetOutputPort());
		//cellSurfFilter->VertexCellsOn();
		//cellSurfFilter->Update();
		//m_WholeSurfDataInput=cellSurfFilter->GetOutput();

		//vtkDataSetMapper *SurfMapper = vtkDataSetMapper::New();
		//SurfMapper->SetInput(inpObj_->GetSurfGridData());
		//SurfMapper->ScalarVisibilityOff();

		//m_ActorSurf=vtkActor::New();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
		//m_ActorSurf->SetMapper(SurfMapper);
		//m_ActorSurf->GetProperty()->SetPointSize(5);
		//ActorSurfSetMap_.insert(it.key(),m_ActorSurf);

		m_areaPicker = vtkAreaPicker::New();
		m_ir->SetPicker(m_areaPicker);

		m_OrgWholePointDataInput = m_WholePointDataInput;
		ActorPElSetMap_[it.key()]->GetProperty()->SetColor(0,0,1);
		//m_ActorPElSet->GetProperty()->SetColor(0,0,1);
		//m_ActorSurf->GetProperty()->SetColor(0,0,1);
		//m_ActorSurf->GetProperty()->SetRepresentationToSurface();
		//inpObj_->GetBindedRenderer()->AddActor(m_ActorPElSet);
		//inpObj_->GetBindedRenderer()->AddActor(m_ActorSurf);
		inpObj_->GetBindedRenderer()->AddActor(ActorPElSetMap_[it.key()]);

		idFilter->Delete();idFilter=NULL;
		//idSurfFilter->Delete();idSurfFilter=NULL; 
		//SurfMapper->Delete();SurfMapper=NULL;
		//m_ActorPElSet->VisibilityOff();
		//m_ActorSurf->VisibilityOff();
		ActorPElSetMap_[it.key()]->VisibilityOff();
	}
	inpObj_->SetMeshActorShow(false);
}
void PElSfSetDlg::InitInpDataSlot(InpDataVIS *InpObj)
{
    clean(); 
    inpObj_=InpObj;
    m_renWin = inpObj_->GetBindedRenderer()->GetRenderWindow();
    m_ir = m_renWin->GetInteractor();

    vtkIdFilter *idFilter= vtkIdFilter::New();
    idFilter->SetInput(inpObj_->GetWholeData("hell"));
    idFilter->SetIdsArrayName("WelCME");
    idFilter->PointIdsOn();
    idFilter->FieldDataOn();
    idFilter->CellIdsOn();
    idFilter->Update();

    //------点集合滤波；
    vtkVertexGlyphFilter *PointFilter = vtkVertexGlyphFilter::New();
    PointFilter->SetInputConnection(idFilter->GetOutputPort());
    PointFilter->Update();
    m_WholePointDataInput=PointFilter->GetOutput();
    //------单元集合滤波；
    vtkCellCenters *cellFilter = vtkCellCenters::New();
    cellFilter->SetInputConnection(idFilter->GetOutputPort());
    cellFilter->VertexCellsOn();
    cellFilter->Update();
    m_WholeCellDataInput=cellFilter->GetOutput();


	vtkExtractEdges *edge = vtkExtractEdges::New();
	edge->SetInput(inpObj_->GetWholeData("hell"));//内部显示
    vtkDataSetMapper *mapper = vtkDataSetMapper::New();
    mapper->SetInput(edge->GetOutput());//(inpObj_->GetWholeData());
    mapper->ScalarVisibilityOff();

    m_ActorPElSet=vtkActor::New();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    m_ActorPElSet->SetMapper(mapper);
    m_ActorPElSet->GetProperty()->SetPointSize(5);
    //--点、单元仅进行表面选择

    vtkDataSetSurfaceFilter *ELPointOnlySurFilter=vtkDataSetSurfaceFilter::New();
    ELPointOnlySurFilter->SetInputConnection(idFilter->GetOutputPort());
    ELPointOnlySurFilter->Update();
    m_PolyDataInput=ELPointOnlySurFilter->GetOutput();

    //vtkPolyDataMapper *mapper2 = vtkPolyDataMapper::New();
    //mapper2->SetInput(ELPointOnlySurFilter->GetOutput());
    //mapper2->ScalarVisibilityOff();


    //--表面集合拾取
    vtkIdFilter *idSurfFilter= vtkIdFilter::New();
    idSurfFilter->SetInput(inpObj_->GetSurfGridData());
    idSurfFilter->SetIdsArrayName("WelCME");
    idSurfFilter->CellIdsOn();
    idSurfFilter->Update();

    vtkCellCenters *cellSurfFilter = vtkCellCenters::New();
    cellSurfFilter->SetInputConnection(idSurfFilter->GetOutputPort());
    cellSurfFilter->VertexCellsOn();
    cellSurfFilter->Update();
    m_WholeSurfDataInput=cellSurfFilter->GetOutput();

    vtkDataSetMapper *SurfMapper = vtkDataSetMapper::New();
    SurfMapper->SetInput(inpObj_->GetSurfGridData());
    SurfMapper->ScalarVisibilityOff();

    m_ActorSurf=vtkActor::New();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    m_ActorSurf->SetMapper(SurfMapper);
    m_ActorSurf->GetProperty()->SetPointSize(5);

    m_areaPicker = vtkAreaPicker::New();
    m_ir->SetPicker(m_areaPicker);

    m_OrgWholePointDataInput = m_WholePointDataInput;
    //m_OrgWholeCellDataInput = m_WholeCellDataInput;
    //m_OrgWholeSurfDataInput = m_WholeSurfDataInput;*/
    m_ActorPElSet->GetProperty()->SetColor(0,0,1);
    //m_ActorPSet->GetProperty()->SetEdgeVisibility(true);
   // m_ActorPSet->GetProperty()->SetRepresentationToWireframe();

   // m_ActorSurf->GetProperty()->SetColor(1,0,0);
    m_ActorSurf->GetProperty()->SetColor(0,0,1);
    //m_ActorSurf->GetProperty()->SetEdgeVisibility(true);
    m_ActorSurf->GetProperty()->SetRepresentationToSurface();
	inpObj_->GetBindedRenderer()->AddActor(m_ActorPElSet);
    inpObj_->GetBindedRenderer()->AddActor(m_ActorSurf);
    

    idFilter->Delete();idFilter=NULL;
    idSurfFilter->Delete();idSurfFilter=NULL;
    //surfaceFilter->Delete();surfaceFilter=NULL;
   // PElsurfaceFilter->Delete();PElsurfaceFilter=NULL;
   // PointFilter->Delete();PointFilter=NULL;
   // cellFilter->Delete();cellFilter=NULL;
    //input->Delete();input=NULL;
   // mapper->Delete();mapper=NULL;  
    SurfMapper->Delete();SurfMapper=NULL;
    m_ActorPElSet->VisibilityOff();
    m_ActorSurf->VisibilityOff();
	//inpObj_->SetMeshActorShow(false);
   
}
void PElSfSetDlg::PointcomBoxSlot()
{
    NElSurfChsS tmp;
    tmp.strName="新建";//m_PsetComb->currentText();
    tmp.iType=COM_ENUM_NSET;
    emitPsetHighLight(tmp);
    //QMap<int,Actor>m_AllPointSetSaveMap[tmp.strName]
    m_editPsetName->setText(m_PsetComb->currentText());
    m_hightLightPointStyle->ClearCurrentSelectedMap();
    m_hightLightPointStyle->setMapData(m_AllPointSetSaveMap[m_PsetComb->currentText()]);
    m_hightLightPointStyle->SetLeftButtonUp(false);
    
    if (m_hightLightElStyle!=NULL)
    {
        m_hightLightElStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->ClearCurrentSelectedMap();
    }
    if (m_hightLightSurfPickerStyle!=NULL)
    {
        m_hightLightSurfPickerStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    }
}

void PElSfSetDlg::ElcomBoxSlot()
{
    NElSurfChsS tmp;
    tmp.strName="新建";//m_PsetComb->currentText();
    tmp.iType=COM_ENUM_ELSET;
    emitPsetHighLight(tmp);
    //QMap<int,Actor>m_AllPointSetSaveMap[tmp.strName]
    m_editElsetName->setText(m_ElSetComb->currentText());
    m_hightLightElStyle->ClearCurrentSelectedMap();
    m_hightLightElStyle->setMapData(m_AllElSetSaveMap[m_ElSetComb->currentText()]);
    m_hightLightElStyle->SetLeftButtonUp(false);
    if (m_hightLightPointStyle!=NULL)
    {
        m_hightLightPointStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->ClearCurrentSelectedMap();
    }
    if (m_hightLightSurfPickerStyle!=NULL)
    {
        m_hightLightSurfPickerStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    }
}
void PElSfSetDlg::SurfcomBoxSlot()
{
    NElSurfChsS tmp;
    tmp.strName="新建";//m_PsetComb->currentText();
    tmp.iType=COM_ENUM_SURFSET;
    emitPsetHighLight(tmp);
    //QMap<int,Actor>m_AllPointSetSaveMap[tmp.strName]
    m_editSurfsetName->setText(m_SurfSetComb->currentText());
    m_hightLightSurfPickerStyle->ClearCurrentSelectedMap();
    m_hightLightSurfPickerStyle->setMapData(m_AllSurfSetSaveMap[m_SurfSetComb->currentText()]);
    m_hightLightSurfPickerStyle->SetLeftButtonUp(false);
    if (m_hightLightPointStyle!=NULL)
    {
        m_hightLightPointStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightPointStyle->ClearCurrentSelectedMap();
    }
    if (m_hightLightElStyle!=NULL)
    {
        m_hightLightElStyle->SetDefaultRenderer(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->InitSetRender(m_ir->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        m_hightLightElStyle->ClearCurrentSelectedMap();
    }
}