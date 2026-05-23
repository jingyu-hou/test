#include "QPostWigResultOutput.h"
#include <QMessageBox>
#include <QScrollArea>
#include <vtkCallbackCommand.h>
/*
 后处理中结果显示界面：
        1.主要完成云图、等值线等
        2.列表显示...
*/
QPostWigResultOutput::QPostWigResultOutput(QWidget *parent)
	: QWidget(parent)
{
    m_gbBox0= new QGroupBox(this);
    m_gbBox0->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbBox0->setTitle(tr("类型"));
    m_BtNephogram = new QPushButton(tr("云图"),m_gbBox0);
    m_BtEqui = new QPushButton(tr("等高线"),m_gbBox0);
    m_SectionBtn = new QPushButton(tr("剖面"),m_gbBox0);
    
 
    QHBoxLayout *gbH0Layout1=new QHBoxLayout(m_gbBox0);
    gbH0Layout1->addWidget(m_BtNephogram);
    gbH0Layout1->addWidget(m_BtEqui);
    gbH0Layout1->addWidget(m_SectionBtn);


    m_gbBox1 = new QGroupBox(this);
    m_gbBox1->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbBox1->setTitle(tr("云图/等值线"));
    
    QLabel *ColorScalarLab = new QLabel(tr("彩色带份数:"),m_gbBox1);
    QLabel *TransparentLab = new QLabel(tr("透明度:"),m_gbBox1);
    m_SpinBoxClr = new QSpinBox(m_gbBox1);
    m_SpinBoxClr->setRange(2,50);
    m_SpinBoxClr->setValue(m_VisParam.m_iClrScalar);
    m_SliderTranc =new QSlider(m_gbBox1);
    m_SliderTranc->setOrientation(Qt::Horizontal);
    m_SliderTranc->setRange(0,100);
    m_SliderTranc->setValue(0);

    m_gbBox11 = new QGroupBox(m_gbBox1);
    m_gbBox11->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbBox11->setTitle(tr("最值设置"));
    m_gbBox11->setCheckable(true);
    m_gbBox11->setChecked(false);
    QLabel *CounterMaxLab= new QLabel(tr("最大值:"),m_gbBox11);
    QLabel *CounterMinLab= new QLabel(tr("最小值:"),m_gbBox11);
    m_CounterMax = new QLineEdit(m_gbBox11);
    m_CounterMin = new QLineEdit(m_gbBox11);

    QGridLayout *gLayout11 = new QGridLayout(m_gbBox11);
    gLayout11->addWidget(CounterMinLab,0,1);
    gLayout11->addWidget(m_CounterMin,0,2);
    gLayout11->addWidget(CounterMaxLab,1,1);
    gLayout11->addWidget(m_CounterMax,1,2);

    m_gbBox12 = new QGroupBox(m_gbBox1);
    m_gbBox12->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbBox12->setTitle(tr("网格显示"));
    m_gbBox12->setCheckable(true);
    m_gbBox12->setChecked(false);

    m_Undeformed = new QCheckBox(tr("未变形"));
    m_GridColorBtn = new QPushButton(tr("颜色"));
    m_GridColorBtn->setMaximumWidth(50);
    QHBoxLayout *HLayout = new QHBoxLayout(m_gbBox12);
    HLayout->addWidget(m_Undeformed);
    HLayout->addWidget(m_GridColorBtn);

    m_gbBox3 = new QGroupBox(this);
    m_gbBox3->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbBox3->setTitle(tr("截面"));
    //m_gbBox13->s//setUsesScrollButtons

    QVBoxLayout *gLayout3 =new QVBoxLayout(m_gbBox3);  
    m_SectionAddBtn = new QPushButton(tr("+"),m_gbBox3);
    m_SectionDelBtn = new QPushButton(tr("-"),m_gbBox3);
	m_InsideOut = new QCheckBox(tr("内/外切割"),m_gbBox3);
    m_tabView = new QTabWidget(m_gbBox3);
   /* QScrollArea *areaScroll_ = new QScrollArea(m_tabView);
    areaScroll_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    areaScroll_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    areaScroll_->setWidgetResizable(true);*/
    //areaScroll_->setWidget(m_tabView);
    QHBoxLayout *gLayoutH3_1=new QHBoxLayout();
    gLayoutH3_1->addWidget(m_SectionAddBtn);
    gLayoutH3_1->addWidget(m_SectionDelBtn);
	QHBoxLayout *gLayoutH3_2=new QHBoxLayout();
	gLayoutH3_2->addWidget(m_InsideOut);
    QHBoxLayout *gLayoutH3_3=new QHBoxLayout();
    gLayoutH3_3->addWidget(m_tabView);

    gLayout3->addLayout(gLayoutH3_1);
    gLayout3->addLayout(gLayoutH3_2);
	gLayout3->addLayout(gLayoutH3_3);

    short mmk=0;
    QGridLayout *gLayout = new QGridLayout(m_gbBox1);
    gLayout->addWidget(m_gbBox12,      mmk++,    0, 1, 3);
    gLayout->addWidget(m_gbBox11,      mmk++,    0, 1, 3);
    gLayout->addWidget(ColorScalarLab, mmk,      0);
    gLayout->addWidget(m_SpinBoxClr,   mmk++,    2);
    gLayout->addWidget(TransparentLab, mmk,      0);
    gLayout->addWidget(m_SliderTranc,  mmk++,    2);
    //gLayout->addWidget(m_gbBox13,      mmk++,    0, 1, 3);


    QVBoxLayout *MainLayout=new QVBoxLayout(this);
    MainLayout->addWidget(m_gbBox0);
    MainLayout->addWidget(m_gbBox1);
    MainLayout->addWidget(m_gbBox3);
    MainLayout->addStretch();
    this->setLayout(MainLayout);

    m_BtIndex=0;
    m_BtNephogram->setMaximumWidth(100);
    m_BtEqui->setMaximumWidth(100);

    connect(m_BtNephogram,SIGNAL(clicked()),this,SLOT(BtnNephogramSlot()));
	connect(m_BtEqui,SIGNAL(clicked()),this,SLOT(BtnEquiSlot()));
    connect(m_SpinBoxClr,SIGNAL(valueChanged(int)),this,SLOT(SpinBoxClrSlot(int)));
    connect(m_SliderTranc,SIGNAL(valueChanged(int)),this,SLOT(SliderTrancSlot(int)));


    connect(m_Undeformed, SIGNAL(clicked()),this,SLOT(SetChgShowSlot()));//网格显示->undeformed、
    connect(m_GridColorBtn,SIGNAL(clicked()),this,SLOT(gridColorBtnSlot()));
    connect(m_gbBox12,SIGNAL(clicked()),this,SLOT(ChkGridSlot()));//grid->网格显示与否;
    connect(m_SectionBtn,SIGNAL(clicked()),this, SLOT(BtnSectionSlot()));//截面显示；
    connect(m_SectionAddBtn,SIGNAL(clicked()),this,SLOT(BtnSectionAddSlot()));//Add Table 
    connect(m_SectionDelBtn,SIGNAL(clicked()),this,SLOT(BtnSectionDelSlot()));//Del Table
    
    m_VisParam.m_iOrigCurrentChg = 0;
    m_VisParam.bCurGrid=false;
    cutIdCnt=0;
    m_tabView->hide();
    m_gbBox3->hide();
}

QPostWigResultOutput::~QPostWigResultOutput()
{
   
    QMapIterator<int, CutParamWidget*> it(m_CutWidgetMap);
    while (it.hasNext())
    {
        it.next();
        m_CutWidgetMap.remove(it.key());
    }
    m_CutWidgetMap.clear();
    cutIdCnt=0;
}
void QPostWigResultOutput::gridColorBtnSlot()
{
    QColor color = QColorDialog::getColor(QColor(Qt::green), this);
    if (!color.isValid())  return;
    m_GridColorBtn->setStyleSheet(QString("background:rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
    m_VisParam.m_gridShowColor.r=(double) color.red()/255.0;
    m_VisParam.m_gridShowColor.g=(double) color.green()/255.0;
    m_VisParam.m_gridShowColor.b=(double) color.blue()/255.0;
    emit emitScalar(m_VisParam);
}
//--grid show or not
void QPostWigResultOutput::ChkGridSlot()
{
    m_VisParam.bCurGrid = m_gbBox12->isChecked(); 
    emit emitScalar(m_VisParam);
}
/*
描述：云图、等值线形变前后显示选择
*/
void QPostWigResultOutput::SetChgShowSlot()
{
    if (m_Undeformed->isChecked()){//0->无变形,1->有变形
        m_VisParam.m_iOrigCurrentChg=0;
    }else{
        m_VisParam.m_iOrigCurrentChg=1;
    } 
     emit emitScalar(m_VisParam);
}
/*
描述：云图显示
*/
void QPostWigResultOutput::BtnNephogramSlot()
{
    m_gbBox1->show();
    m_gbBox3->hide();
    m_BtIndex=0;
    m_VisParam.m_iStyle=m_BtIndex;
    m_VisParam.bContour=true;
    m_VisParam.bContourMinMax=m_gbBox11->isChecked();
    m_VisParam.contourMin=m_CounterMin->text().toDouble();
    m_VisParam.contourMax=m_CounterMax->text().toDouble();
    QMapIterator<int, CutParamWidget*> it(m_CutWidgetMap);
    while (it.hasNext()){
        it.next();
        int cutId=it.key();
        //it.value()->ChangeCutZoneState(false);
        //it.value()->ChangeCutVTKWidgetState(false);
        //it.value()->m_gbBox3->setChecked(false);
        SectionOutLineSlot(cutId,false);
        SectionZoneSlot(cutId,false);
        SectionShowSlot(cutId,false,0);
    }
    emit emitScalar(m_VisParam);
}
/*
描述：等值线显示
*/
void QPostWigResultOutput::BtnEquiSlot()
{
    m_gbBox1->show();
    m_gbBox3->hide();
    m_BtIndex=2;
    m_VisParam.m_iStyle=m_BtIndex;
    m_VisParam.bContour=true;
    QMapIterator<int, CutParamWidget*> it(m_CutWidgetMap);
    while (it.hasNext()){
        it.next();
        int cutId=it.key();
        SectionOutLineSlot(cutId,false);
        SectionZoneSlot(cutId,false);
        SectionShowSlot(cutId,false,0);
    }
    emit emitScalar(m_VisParam);
}
/*
描述：颜色等级划分设定
*/
void QPostWigResultOutput::SpinBoxClrSlot(int iVal)
{
    m_VisParam.m_iClrScalar=iVal;
    emit emitScalar(m_VisParam);
}
/*
描述：透明度设定
*/
void QPostWigResultOutput::SliderTrancSlot(int iVal)
{
    double factor = 1.0 - double(iVal)/m_SliderTranc->maximum();
    m_VisParam.m_dTrans=factor;
    emit emitScalar(m_VisParam);
}
/*
initfrd
*/
void QPostWigResultOutput::InitSectionDataSlot(FrdDataVIS *frdObj)
{
    frdVISObj_ = frdObj;
    const map<QString, QStringList>*varMap =frdVISObj_->GetScalarInfo();
	if(varMap->size()<=0)return;
    QString header(varMap->begin()->first+":"+varMap->begin()->second.at(0));
    m_Var = header;
    //--clear
     QMapIterator<int, CutParamWidget*> it(m_CutWidgetMap);
     while (it.hasNext()){
         it.next();
         int cutId=it.key();
         SectionOutLineSlot(cutId, false);
         SectionZoneSlot(cutId, false);
         SectionShowSlot(cutId, false,0);
     }
     m_CutWidgetMap.clear();
     cutIdCnt=0;
}
/*
描述：剖面
*/
void QPostWigResultOutput::BtnSectionSlot()
{
    if (!frdVISObj_)return;
    m_gbBox1->hide();
    m_gbBox3->show();
    m_VisParam.bContour=false;//关闭整个的云图显示
    m_VisParam.bCurGrid=false;
    emit emitScalar(m_VisParam);
    
    QMapIterator<int, CutParamWidget*> it(m_CutWidgetMap);
    while (it.hasNext()){
        it.next();
        int cutId=it.key();
        SectionOutLineSlot(cutId, it.value()->m_OutLineVisible->isChecked());
        SectionZoneSlot(cutId, it.value()->m_ZoneVisible->isChecked());
        SectionShowSlot(cutId, it.value()->m_gbBox3->isChecked(),0);
    }
    
    //frdVISObj_->Update();  

}
/*
描述：剖面：cutAdd
*/
void QPostWigResultOutput::BtnSectionAddSlot()
{ 
	if (m_Var.isEmpty())return;
    int id = m_CutWidgetMap.size()+1;
    cutIdCnt++;
    CutParamWidget *TmpTab = new CutParamWidget(id, m_gbBox3);
	if (ActorListId_.empty()){
		TmpTab->ActorListId_.clear();
		TmpTab->ActorListId_<<"1";
	}else{
		TmpTab->ActorListId_=ActorListId_;
	}
    m_tabView->show();
	QString strTableName(tr("内切割"));
	if(m_InsideOut->isChecked()){
		strTableName=tr("外切割");
	}

    m_tabView->addTab(TmpTab, strTableName+QString("-%1").arg(id));  
    m_tabView->setMovable(true);
    m_tabView->setUsesScrollButtons(true);
    m_tabView->setMinimumHeight(400);
    m_tabView->setCurrentIndex(m_tabView->count()-1);
    m_CutWidgetMap.insert(id, TmpTab);

    TmpTab->InitCutZones();
    //frdVISObj_->CreateCutObjects(id, m_Var);//,m_Var.split(":").at(0));
    m_VisParam.bContour=false;//关闭整个的云图显示
    m_VisParam.bCurGrid=false;
    m_VisParam.CutIdList.clear();
    QMapIterator<int, CutParamWidget*> it(m_CutWidgetMap);
    while (it.hasNext()){
        it.next();
        int cutId=it.key();
        m_VisParam.CutIdList.push_back(cutId);
    }

    emit emitScalar(m_VisParam);


	vtkCallbackCommand *cb = vtkCallbackCommand::New();
	cb->SetCallback(CutParamWidget::CallBack_ChangeCutUIValues);
	cb->SetClientData(TmpTab);

	for (int kk=0;kk<ActorListId_.size();kk++)
	{
		frdVISObj_->CreateCutObjects(ActorListId_.at(kk).toInt(),id, m_InsideOut->isChecked(),m_Var,"original");//->isTristate());//,m_Var.split(":").at(0));
		frdVISObj_->SetCutZoneVisible(ActorListId_.at(kk).toInt(),id,true);//
		frdVISObj_->SetCutVTKWidgetVisible(ActorListId_.at(kk).toInt(),id,true);//
		
		//add callback begin
	  
		frdVISObj_->AddCutCallBack(ActorListId_.at(kk).toInt(),id, cb);
		frdVISObj_->Update();
    
	    double bounds[6],xyz[3];
		bool f1 = frdVISObj_->GetCutZoneBounds(ActorListId_.at(kk).toInt(),id,bounds);
		bool f2 = frdVISObj_->GetCutPlanePosition(ActorListId_.at(kk).toInt(),id,xyz);
		if (f1 && f2)
		{
			TmpTab->InitBoundsValue(bounds);
			TmpTab->SetCutPositionWidgetValue(xyz);
		}
    }
    //add callback end
    connect(TmpTab, SIGNAL(CutNormalChanged(int,double,double,double)), this, SLOT(SectionNormalXYSlot(int,double,double,double)));
    connect(TmpTab, SIGNAL(CutPositionChanged(int,double,double,double)), this, SLOT(SectionPosSlot(int,double,double,double)));  
    connect(TmpTab, SIGNAL(CutVTKWidgetStateChanged(int,bool)), this, SLOT(SectionOutLineSlot(int,bool)));
    connect(TmpTab, SIGNAL(CutZoneStateChanged(int,bool)), this, SLOT(SectionZoneSlot(int,bool)));
    connect(TmpTab, SIGNAL(CutContourStateChanged(int,bool,int)),this,SLOT(SectionShowSlot(int,bool,int)));
    connect(TmpTab, SIGNAL(CutContourLevelChanged(int,int)),this,SLOT(SectionLevelSlot(int,int)));
}
void QPostWigResultOutput::BtnSectionDelSlot()
{
    int index = m_tabView->currentIndex();
	if (index<0)
	{
		return;
	}
    QStringList strListT=m_tabView->tabText(index).split("-");
    int cutId = strListT.at(1).toInt();
	for (int kk=0;kk<ActorListId_.size();kk++)
	{
		frdVISObj_->DeleteCut(ActorListId_.at(kk).toInt(),cutId);
	}
    frdVISObj_->Update();
    m_tabView->removeTab(index);
    if (m_tabView->count() < 1)
    {
        m_tabView->setVisible(false);
    }
    m_CutWidgetMap.remove(cutId);
}
void QPostWigResultOutput::upDateScalar(QString scalar)
{
    m_Var = scalar;
}
void QPostWigResultOutput::SectionNormalXYSlot(int cutId, double x, double y, double z)
{
	for (int kk=0;kk<ActorListId_.size();kk++)
	{
		frdVISObj_->SetCutPlaneNormal(ActorListId_.at(kk).toInt(),cutId,x,y,z);
	}
    frdVISObj_->Update();
}
void QPostWigResultOutput::SectionPosSlot(int cutId,double x,double y, double z)
{
	for (int kk=0;kk<ActorListId_.size();kk++)
	{
		frdVISObj_->SetCutPlanePosition(ActorListId_.at(kk).toInt(),cutId,x,y,z);
	}
    frdVISObj_->Update();
}
void QPostWigResultOutput::SectionOutLineSlot(int cutId,bool bShow)
{
	for (int kk=0;kk<ActorListId_.size();kk++)
	{
		frdVISObj_->SetCutVTKWidgetVisible(ActorListId_.at(kk).toInt(),cutId,bShow);
	}
    frdVISObj_->Update();
}
void QPostWigResultOutput::SectionZoneSlot(int cutId,bool bShow)
{
	for (int kk=0;kk<ActorListId_.size();kk++)
	{
		frdVISObj_->SetCutZoneVisible(ActorListId_.at(kk).toInt(),cutId,bShow);
	}
    frdVISObj_->Update();
}
void QPostWigResultOutput::SectionShowSlot(int cutId,bool bShow,int type)
{
	for (int kk=0;kk<ActorListId_.size();kk++)
	{
		frdVISObj_->SetCutContourState(ActorListId_.at(kk).toInt(),cutId,bShow,type);
	}
    frdVISObj_->Update();
}
void QPostWigResultOutput::SectionLevelSlot(int cutId,int level)
{
	for (int kk=0;kk<ActorListId_.size();kk++)
	{
		 frdVISObj_->SetCutContourLevel(ActorListId_.at(kk).toInt(),cutId,level);
	}
    frdVISObj_->Update();
}

void QPostWigResultOutput::setActorList( QStringList strL)
{
	ActorListId_=strL;
}
/////////////////////////////////////////////////////////
//cutParamWidget
/////////////////////////////////////////////////////////
//#include <vtkImplicitPlaneWidget.h>
CutParamWidget::CutParamWidget(int cutId, QWidget *parent) : QWidget(parent), cutId_(cutId)
{
    m_gbBox0 = new QGroupBox(this);
    //m_gbBox0->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    //m_gbBox0->setTitle(tr("法线"));
    m_ZoneVisible = new QCheckBox(tr("实体显示"), m_gbBox0);
    m_OutLineVisible=new QCheckBox(tr("截平面"), m_gbBox0);
    QHBoxLayout *gbBox0LayoutH1_1 = new QHBoxLayout();
    gbBox0LayoutH1_1->addWidget(m_ZoneVisible);
    gbBox0LayoutH1_1->addWidget(m_OutLineVisible);
	QHBoxLayout *gbBox0LayoutH1_2 = new QHBoxLayout();
    QVBoxLayout *gbBox0LayoutV= new QVBoxLayout(m_gbBox0);
    gbBox0LayoutV->addLayout(gbBox0LayoutH1_1);
	gbBox0LayoutV->addLayout(gbBox0LayoutH1_2);

    m_gbBox1 = new QGroupBox(this);
    m_gbBox1->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbBox1->setTitle(tr("法线"));
    //m_gbBox1->setCheckable(true);
    //m_gbBox1->setChecked(false);

    QVBoxLayout *gbBox1LayoutV= new QVBoxLayout(m_gbBox1);
    QHBoxLayout *gbBox1LayoutH1_1 = new QHBoxLayout();
    QButtonGroup *m_BtnGroupNormal = new QButtonGroup(m_gbBox1);
    m_NormalXBtn=new QPushButton(tr("X"),m_gbBox1);
    m_NormalYBtn=new QPushButton(tr("Y"),m_gbBox1);
    m_NormalZBtn=new QPushButton(tr("Z"),m_gbBox1);
    m_BtnGroupNormal->addButton(m_NormalXBtn,0);
    m_BtnGroupNormal->addButton(m_NormalYBtn,1);
    m_BtnGroupNormal->addButton(m_NormalZBtn,2);

    gbBox1LayoutH1_1->addWidget(m_NormalXBtn);
    gbBox1LayoutH1_1->addWidget(m_NormalYBtn);
    gbBox1LayoutH1_1->addWidget(m_NormalZBtn);
    QHBoxLayout *gbBox1LayoutH1_2 = new QHBoxLayout();
    m_LineEditNormalX=new QLineEdit(m_gbBox1);
    m_LineEditNormalY=new QLineEdit(m_gbBox1);
    m_LineEditNormalZ=new QLineEdit(m_gbBox1);
    gbBox1LayoutH1_2->addWidget(m_LineEditNormalX);
    gbBox1LayoutH1_2->addWidget(m_LineEditNormalY);
    gbBox1LayoutH1_2->addWidget(m_LineEditNormalZ);
    gbBox1LayoutV->addLayout(gbBox1LayoutH1_1);
    gbBox1LayoutV->addLayout(gbBox1LayoutH1_2);

    m_gbBox2 = new QGroupBox(this);
    m_gbBox2->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbBox2->setTitle(tr("位置"));
    //m_gbBox1->setCheckable(true);
    //m_gbBox1->setChecked(false);

    QVBoxLayout *gbBox2LayoutV= new QVBoxLayout(m_gbBox2);
    QLabel *labX=new QLabel(tr("X:"));
    QLabel *labY=new QLabel(tr("Y:"));
    QLabel *labZ=new QLabel(tr("Z:"));
    m_LineEditPosX = new QLineEdit(m_gbBox2);
    m_LineEditPosY = new QLineEdit(m_gbBox2);
    m_LineEditPosZ = new QLineEdit(m_gbBox2);
    m_SliderX = new QSlider(Qt::Horizontal ,m_gbBox2);
    m_SliderY = new QSlider(Qt::Horizontal, m_gbBox2);
    m_SliderZ = new QSlider(Qt::Horizontal, m_gbBox2);

	

    QHBoxLayout *gbBox2LayoutH1_1 = new QHBoxLayout();
    gbBox2LayoutH1_1->addWidget(labX);
    gbBox2LayoutH1_1->addWidget(m_LineEditPosX);
    gbBox2LayoutH1_1->addWidget(m_SliderX);
   
    QHBoxLayout *gbBox2LayoutH1_2 = new QHBoxLayout();
    gbBox2LayoutH1_2->addWidget(labY);
    gbBox2LayoutH1_2->addWidget(m_LineEditPosY);
    gbBox2LayoutH1_2->addWidget(m_SliderY);

    QHBoxLayout *gbBox2LayoutH1_3 = new QHBoxLayout();
    gbBox2LayoutH1_3->addWidget(labZ);
    gbBox2LayoutH1_3->addWidget(m_LineEditPosZ);
    gbBox2LayoutH1_3->addWidget(m_SliderZ);
  

    gbBox2LayoutV->addLayout(gbBox2LayoutH1_1);
    gbBox2LayoutV->addLayout(gbBox2LayoutH1_2);
    gbBox2LayoutV->addLayout(gbBox2LayoutH1_3);


    m_gbBox3 = new QGroupBox(this);
    m_gbBox3->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbBox3->setTitle(tr("云图"));
    m_gbBox3->setCheckable(true);
    m_gbBox3->setChecked(true);
    QVBoxLayout *gbBox3LayoutV = new QVBoxLayout(m_gbBox3);
    m_ContourBtn = new QPushButton(tr("云图"), m_gbBox3);
    m_ContourLineBtn = new QPushButton(tr("等值线"), m_gbBox3);
    m_CoutourClrLab = new QLabel(tr("颜色等级:"), m_gbBox3);
    m_CoutourLevelSpinB = new QSpinBox(m_gbBox3);
    QHBoxLayout *gbBox3LayoutH1_1 = new QHBoxLayout();
    gbBox3LayoutH1_1->addWidget(m_ContourBtn);
    gbBox3LayoutH1_1->addWidget(m_ContourLineBtn);
    gbBox3LayoutH1_1->addWidget(m_CoutourClrLab);
    gbBox3LayoutH1_1->addWidget(m_CoutourLevelSpinB);
    gbBox3LayoutV->addLayout(gbBox3LayoutH1_1);
   
    QButtonGroup *m_BtnGroupContour = new QButtonGroup(m_gbBox3);
    m_BtnGroupContour->addButton(m_ContourBtn,0);
    m_BtnGroupContour->addButton(m_ContourLineBtn,1);


    QVBoxLayout *mainlayout=new QVBoxLayout(this);
    mainlayout->addWidget(m_gbBox0);
    mainlayout->addWidget(m_gbBox1);
    mainlayout->addWidget(m_gbBox2);
    mainlayout->addWidget(m_gbBox3);
    this->setLayout(mainlayout);

    m_LineEditNormalX->setValidator(new QDoubleValidator);
    m_LineEditNormalY->setValidator(new QDoubleValidator);
    m_LineEditNormalZ->setValidator(new QDoubleValidator);
    m_LineEditPosX->setValidator(new QDoubleValidator);
    m_LineEditPosY->setValidator(new QDoubleValidator);
    m_LineEditPosZ->setValidator(new QDoubleValidator);

    //init;
    m_ZoneVisible->setChecked(true);
    m_OutLineVisible->setChecked(true);

    connect(m_ZoneVisible,SIGNAL(clicked(bool)),this,SLOT(ChangeCutZoneState(bool)));
    connect(m_OutLineVisible,SIGNAL(clicked(bool)),this,SLOT(ChangeCutVTKWidgetState(bool)));
    connect(m_BtnGroupNormal, SIGNAL(buttonClicked(int)), this, SLOT(ChangeCutNormal(int)));
    
    connect(m_SliderX,SIGNAL(sliderMoved(int)),this,SLOT(MoveCut(int)));
    connect(m_SliderY,SIGNAL(sliderMoved(int)),this,SLOT(MoveCut(int)));
    connect(m_SliderZ,SIGNAL(sliderMoved(int)),this,SLOT(MoveCut(int)));

    connect(m_LineEditNormalX, SIGNAL(editingFinished()), this, SLOT(ChangeCutNormal2()));
    connect(m_LineEditNormalY, SIGNAL(editingFinished()), this, SLOT(ChangeCutNormal2()));
    connect(m_LineEditNormalZ, SIGNAL(editingFinished()), this, SLOT(ChangeCutNormal2()));
    connect(m_LineEditPosX, SIGNAL(editingFinished()), this, SLOT(ChangeCutPosition()));
    connect(m_LineEditPosY, SIGNAL(editingFinished()), this, SLOT(ChangeCutPosition()));
    connect(m_LineEditPosZ, SIGNAL(editingFinished()), this, SLOT(ChangeCutPosition()));
    connect(m_gbBox3,SIGNAL(clicked()),this,SLOT(ChangeCutContourState()));//云图显示与否
    connect(m_BtnGroupContour, SIGNAL(buttonClicked(int)), this, SLOT(ChangeCutContourType(int)));
    connect(m_CoutourLevelSpinB,SIGNAL(valueChanged(int)), this, SLOT(ChangeCutContourLevel(int)));
}
void CutParamWidget::CallBack_ChangeCutUIValues(vtkObject *caller, unsigned long, void *clientdata, void*)
{
    vtkImplicitPlaneWidget *cutPlane = vtkImplicitPlaneWidget::SafeDownCast(caller);
    if (cutPlane == 0)  return;
    CutParamWidget *w = static_cast<CutParamWidget*>(clientdata);
    double normal[3];
    cutPlane->GetNormal(normal);
    w->m_LineEditNormalX->setText(QString("%1").arg(normal[0]));
    w->m_LineEditNormalY->setText(QString("%1").arg(normal[1]));
    w->m_LineEditNormalZ->setText(QString("%1").arg(normal[2]));
    double origin[3];
    cutPlane->GetOrigin(origin);
    w->SetCutPositionWidgetValue(origin);
}

void CutParamWidget::ChkSectionContourSlot()
{
     
    emit CutVTKWidgetStateChanged(cutId_, m_gbBox3->isChecked());
}

void CutParamWidget::InitCutZones()
{
   m_CoutourLevelSpinB->setValue(20);
   m_LineEditNormalX->setText("1");
   m_LineEditNormalY->setText("0");
   m_LineEditNormalZ->setText("0");

}

void CutParamWidget::InitCutContourVariables(const QStringList &scalars)
{
    //ui->comboBox_contourVar->insertItems(0, scalars);
}

void CutParamWidget::InitBoundsValue(double *bounds)
{
    for (int i = 0; i < 6; ++i)
    {
        bounds_[i] = bounds[i];
    }
}

void CutParamWidget::SetCutPositionWidgetValue(double *xyz)
{
    m_LineEditPosX->setText(QString("%1").arg(xyz[0]));
    m_LineEditPosY->setText(QString("%1").arg(xyz[1]));
    m_LineEditPosZ->setText(QString("%1").arg(xyz[2]));

    //update slider widget
    int vX = (xyz[0]-bounds_[0])/(bounds_[1]-bounds_[0]) * 100;
    int vY = (xyz[1]-bounds_[2])/(bounds_[3]-bounds_[2]) * 100;
    int vZ = (xyz[2]-bounds_[4])/(bounds_[5]-bounds_[4]) * 100;
    m_SliderX->setValue(vX);
    m_SliderY->setValue(vY);
    m_SliderZ->setValue(vZ);
   
}

void CutParamWidget::ChangeCutZoneState(bool isShow)
{
    emit CutZoneStateChanged(cutId_, isShow);
}

void CutParamWidget::ChangeCutVTKWidgetState(bool isShow)
{
    emit CutVTKWidgetStateChanged(cutId_, isShow);
}
void CutParamWidget::ChangeCutNormal(int BtnId)
{

    if (BtnId < 0)  return;
    if (BtnId == 0)
    {
        m_LineEditNormalX->setText("1");
        m_LineEditNormalY->setText("0");
        m_LineEditNormalZ->setText("0");
    }
    else if (BtnId == 1)
    {
        m_LineEditNormalX->setText("0");
        m_LineEditNormalY->setText("1");
        m_LineEditNormalZ->setText("0");
    }
    else if (BtnId == 2)
    {
        m_LineEditNormalX->setText("0");
        m_LineEditNormalY->setText("0");
        m_LineEditNormalZ->setText("1");
    }
    double x = m_LineEditNormalX->text().toDouble();
    double y = m_LineEditNormalY->text().toDouble();
    double z = m_LineEditNormalZ->text().toDouble();
    emit CutNormalChanged(cutId_, x, y, z);
}
void CutParamWidget::ChangeCutNormal2()
{
    double x = m_LineEditNormalX->text().toDouble();
    double y = m_LineEditNormalY->text().toDouble();
    double z = m_LineEditNormalZ->text().toDouble();
    emit CutNormalChanged(cutId_, x, y, z);
}
void CutParamWidget::ChangeCutPosition()
{
    double x = m_LineEditPosX->text().toDouble();
    double y = m_LineEditPosY->text().toDouble();
    double z = m_LineEditPosZ->text().toDouble();
    emit CutPositionChanged(cutId_, x, y, z);
}

void CutParamWidget::MoveCut(int value)
{
    QSlider *slider = qobject_cast<QSlider*>(sender());
    double x, y, z;
    if (slider == m_SliderX)
    {
        x = bounds_[0] + (bounds_[1]-bounds_[0]) * value/100.0;
        y = m_LineEditPosY->text().toDouble();
        z = m_LineEditPosZ->text().toDouble();
    }
    else if (slider == m_SliderY)
    {
        y = bounds_[2] + (bounds_[3]-bounds_[2]) * value/100.0;
        x = m_LineEditPosX->text().toDouble();
        z = m_LineEditPosZ->text().toDouble();
    }
    else
    {
        z = bounds_[4] + (bounds_[5]-bounds_[4]) * value/100.0;
        x = m_LineEditPosX->text().toDouble();
        y = m_LineEditPosY->text().toDouble();
    }
    emit CutPositionChanged(cutId_, x, y, z);
}

void CutParamWidget::ChangeCutContourState()
{
    int type=0;//default contour;
    bool isShow =m_gbBox3->isChecked();
    emit CutContourStateChanged(cutId_, isShow, type);
}

void CutParamWidget::ChangeCutContourType(int type)
{
    emit CutContourStateChanged(cutId_, true, type);
}
void CutParamWidget::ChangeCutContourVariable(const QString &scalar)
{
    emit CutContourVariableChanged(cutId_, scalar);
}

void CutParamWidget::ChangeCutContourLevel(int level)
{
    emit CutContourLevelChanged(cutId_, level);
}


