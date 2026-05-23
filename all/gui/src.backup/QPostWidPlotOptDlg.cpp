#include "QPostWidPlotOptDlg.h"
#include <QPainter>
#include <QAbstractItemDelegate>
#include <QColorDialog>
using namespace std;
/*
描述：plot option中界面设定操作
*/
QPostWidPlotOptDlg::QPostWidPlotOptDlg(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("绘图选项"));
    m_TabWig = new QTabWidget(this);
    TabPage1 = new QPostWidPlotOpt_Basic(m_TabWig);
	TabPage2 = new QPostWidPlotOpt_Color(m_TabWig);
	TabPage3 = new QPostWidPlotOpt_Labels(m_TabWig);
	TabPage4 = new QPostWidPlotOpt_Normals(m_TabWig);
    TabPage5 = new QPostWidPlotOpt_Other(m_TabWig);

    int index = m_TabWig->addTab(TabPage1, "显示");
	m_TabWig->addTab(TabPage2,"颜色与类型");
	m_TabWig->addTab(TabPage3,"标签");
	m_TabWig->addTab(TabPage4,"矢量");
	m_TabWig->addTab(TabPage5,"其他");

    QVBoxLayout *Vlayout = new QVBoxLayout ();
    Vlayout->addWidget(m_TabWig);
    
	//--button
    QPushButton *m_BtnOK = new QPushButton(tr("确定"));
    QPushButton *m_BtnCancel = new QPushButton(tr("取消"));
    QPushButton *m_BtnApply = new QPushButton(tr("应用"));
    QPushButton *m_BtnDefaults = new QPushButton(tr("默认"));
	
    QHBoxLayout *HLayout1 = new QHBoxLayout();
    HLayout1->addWidget(m_BtnOK);
    HLayout1->addWidget(m_BtnCancel);
    HLayout1->addWidget(m_BtnApply);
    HLayout1->addWidget(m_BtnDefaults);

    QVBoxLayout *MainLayout = new QVBoxLayout();
    //lout->setContentsMargins(2, 2, 2, 2);
    MainLayout->addLayout(Vlayout);
    MainLayout->addLayout(HLayout1);

    this->setLayout(MainLayout);
	connect(m_BtnOK,SIGNAL(clicked()),this,SLOT(BtnOkSlot()));
	connect(m_BtnCancel,SIGNAL(clicked()),this,SLOT(BtnCancelSlot()));
	connect(m_BtnDefaults,SIGNAL(clicked()),this,SLOT(BtnDefaultsSlot()));
	connect(m_BtnApply,SIGNAL(clicked()),this,SLOT(BtnApplySlot()));	
    this->setMinimumHeight(380);
}

QPostWidPlotOptDlg::~QPostWidPlotOptDlg()
{

}
//-slot
/*
描述：取消操作，设定参数不保存，直接退出
*/
void QPostWidPlotOptDlg::BtnCancelSlot()
{
	this->close();
				
}
/*
描述：确定操作，设定参数保存退出
*/
void QPostWidPlotOptDlg::BtnOkSlot()
{
	PutData();
    ExportData();
	emit emitParam(m_OptDlgS);
	this->close();
}
/*
描述：恢复默认操作
*/
void QPostWidPlotOptDlg::BtnDefaultsSlot()
{
	/*int index = m_TabWig->currentIndex();
	if (index == 0){TabPage1->setDefault();
	}else if (index == 1){TabPage2->setDefault();
	}else if (index == 2){TabPage3->setDefault();
	}else if (index == 3){TabPage4->setDefault();
	}else if (index == 4){TabPage5->setDefault();
	}*/
	TabPage1->setDefault();
	TabPage2->setDefault();
	TabPage3->setDefault();
	TabPage4->setDefault();
	TabPage5->setDefault();
}
/*
描述：确定保存文件，立即设定，但对话框不退出
*/
void QPostWidPlotOptDlg::BtnApplySlot()
{
//this->close();
	PutData();
	ExportData();
	emit emitParam(m_OptDlgS);
}
//--设置数据
void QPostWidPlotOptDlg::SetData(OptDlgS OptData)
{
    m_OptDlgS=OptData;
   ///* m_OptDlgS.BaseRenderIndex = m_OptData.BaseRenderIndex;
   // m_OptDlgS.BaseScaleInder = m_OptData.BaseScaleInder;
   // m_OptDlgS.BaseVisIndex = m_OptData.BaseVisIndex;
   // m_OptDlgS.ClrEdgeAStyIndex=m_OptData.ClrEdgeAStyIndex;
   // m_OptDlgS.ClrEdgeAThikIndex=m_OptData.ClrEdgeAThikIndex;
   // m_OptDlgS.ClrEdgesCSPLot=m_OptData.ClrEdgesCSPLot;
   // m_OptDlgS.ClrEdgesFSPlot=m_OptData.ClrEdgesFSPlot;
   // m_OptDlgS.ClrEdgesWHPlot=m_OptData.ClrEdgesWHPlot;

   // m_OptDlgS.LabClrShowElem=m_OptData.LabClrShowElem;
   // m_OptDlgS.LabClrShowFace=m_OptData.LabClrShowFace;
   // m_OptDlgS.LabClrShowNode=m_OptData.LabClrShowNode;
   // m_OptDlgS.LabClrShowNodeSymb=m_OptData.LabClrShowNodeSymb;
   // m_OptDlgS.LabElemChk=m_OptData.LabElemChk;
   // m_OptDlgS.LabFaceChk=m_OptData.LabFaceChk;
   // m_OptDlgS.LabNodeChk=m_OptData.LabNodeChk;
   // m_OptDlgS.LabNodSymbolChk=m_OptData.LabNodSymbolChk;
   // m_OptDlgS.LabNodSizeIndex=m_OptData.LabNodSizeIndex;
   // m_OptDlgS.LabNodSymbolIndex=m_OptData.LabNodSymbolIndex;

    //basic
    TabPage1->m_RenderIndex = m_OptDlgS.BaseRenderIndex;
    TabPage1->m_ScaleIndex = m_OptDlgS.BaseScaleInder;
    TabPage1->m_VisibleIndex = m_OptDlgS.BaseVisIndex;
    TabPage1->m_BaseValue = m_OptDlgS.BaseValue;
    TabPage1->setData();
    //color
    TabPage2->m_ClrEdgeCFSPlot=m_OptDlgS.ClrEdgesCSPLot;
    TabPage2->m_ClrEdgeFSOPlot=m_OptDlgS.ClrEdgesFSPlot;
    TabPage2->m_ClrEdgeWHPlot=m_OptDlgS.ClrEdgesWHPlot;
    TabPage2->m_ClrHisCuvPonit=m_OptDlgS.ClrHisCuvPoint;
    TabPage2->m_ClrHisCuvPonitSize=m_OptDlgS.ClrHisPointfactor;
    TabPage2->m_ClrEdgeAStyIndex=m_OptDlgS.ClrEdgeAStyIndex;
    TabPage2->m_ClrEdgeAThikIndex=m_OptDlgS.ClrEdgeAThikIndex;
    TabPage2->setData();
    //labels
    TabPage3->m_LabClrShowElem=m_OptDlgS.LabClrShowElem;
    TabPage3->m_LabClrShowFace=m_OptDlgS.LabClrShowFace;
    TabPage3->m_LabClrShowNode=m_OptDlgS.LabClrShowNode;
    TabPage3->m_LabClrShowNodeSymb=m_OptDlgS.LabClrShowNodeSymb;
    TabPage3->m_LabShowElemChk = m_OptDlgS.LabElemChk;
    TabPage3->m_LabShowFaceChk = m_OptDlgS.LabFaceChk;
    TabPage3->m_LabShowNodeChk = m_OptDlgS.LabNodeChk;
    TabPage3->m_LabShowNodeSymbChk = m_OptDlgS.LabNodSymbolChk;
    TabPage3->m_symbolsSizeIndex=m_OptDlgS.LabNodSizeIndex;
    TabPage3->m_symbolsStyIndex=m_OptDlgS.LabNodSymbolIndex;
    TabPage3->setData();
    //Normals
    TabPage4->m_NormShowChk=m_OptDlgS.NormShowChk;
    TabPage4->m_NormRadioElemOrSur=m_OptDlgS.NormRadioElemOrSur;
    TabPage4->m_NormClrFacePlot=m_OptDlgS.NormClrFacePlot;
    TabPage4->m_NormClrBeamN1Plot=m_OptDlgS.NormClrBeamN1Plot;
    TabPage4->m_NormClrBeamN2Plot=m_OptDlgS.NormClrBeamN2Plot;
    TabPage4->m_NormClrBeamTangPlot=m_OptDlgS.NormClrBeamTangPlot;
    TabPage4->m_NormStyLenIndex=m_OptDlgS.NormStyLenIndex;
    TabPage4->m_NormStyLinThickIndex=m_OptDlgS.NormStyLinThickIndex;
    TabPage4->m_NormStyArrowHeadIndex=m_OptDlgS.NormStyArrowHeadIndex;
    TabPage4->setData();
}
//--从界面中获取数据
void QPostWidPlotOptDlg::PutData()
{
    //basic
    m_OptDlgS.BaseRenderIndex = TabPage1->m_RenderIndex;
    m_OptDlgS.BaseScaleInder = TabPage1->m_ScaleIndex;
    m_OptDlgS.BaseVisIndex = TabPage1->m_VisibleIndex;
    m_OptDlgS.BaseValue = TabPage1->m_BaseValue;

    //clr
    TabPage2->m_ClrEdgeAStyIndex = TabPage2->m_comboxSty->currentIndex();
    TabPage2->m_ClrEdgeAThikIndex = TabPage2->m_comboxThick->currentIndex();
    TabPage2->m_ClrHisCuvPonitSize= TabPage2->m_EditPointSize->text();

    m_OptDlgS.ClrEdgesCSPLot=TabPage2->m_ClrEdgeCFSPlot;
    m_OptDlgS.ClrEdgesFSPlot=TabPage2->m_ClrEdgeFSOPlot;
    m_OptDlgS.ClrEdgesWHPlot=TabPage2->m_ClrEdgeWHPlot;
    m_OptDlgS.ClrHisCuvPoint=TabPage2->m_ClrHisCuvPonit;
    m_OptDlgS.ClrHisPointfactor=TabPage2->m_ClrHisCuvPonitSize;
    m_OptDlgS.ClrEdgeAStyIndex=TabPage2->m_ClrEdgeAStyIndex;
    m_OptDlgS.ClrEdgeAThikIndex=TabPage2->m_ClrEdgeAThikIndex;
    //label
    TabPage3->m_symbolsSizeIndex = TabPage3->m_comboxSize->currentIndex();
    TabPage3->m_symbolsStyIndex = TabPage3->m_comboxSymbol->currentIndex();

    m_OptDlgS.LabClrShowElem=TabPage3->m_LabClrShowElem;
    m_OptDlgS.LabClrShowFace=TabPage3->m_LabClrShowFace;
    m_OptDlgS.LabClrShowNode=TabPage3->m_LabClrShowNode;
    m_OptDlgS.LabClrShowNodeSymb=TabPage3->m_LabClrShowNodeSymb;
    m_OptDlgS.LabElemChk=TabPage3->m_LabShowElemChk;
    m_OptDlgS.LabFaceChk=TabPage3->m_LabShowFaceChk;
    m_OptDlgS.LabNodeChk=TabPage3->m_LabShowNodeChk;
    m_OptDlgS.LabNodSymbolChk=TabPage3->m_LabShowNodeSymbChk;
    m_OptDlgS.LabNodSizeIndex=TabPage3->m_symbolsSizeIndex;
    m_OptDlgS.LabNodSymbolIndex=TabPage3->m_symbolsStyIndex;
     //Normals
    TabPage4->m_NormStyLenIndex = TabPage4->m_comboxLen->currentIndex();
    TabPage4->m_NormStyLinThickIndex = TabPage4->m_comboxThick->currentIndex();
    TabPage4->m_NormStyArrowHeadIndex = TabPage4->m_comboxArrowHead->currentIndex();

    m_OptDlgS.NormStyLenIndex=TabPage4->m_NormStyLenIndex;
    m_OptDlgS.NormStyLinThickIndex=TabPage4->m_NormStyLinThickIndex;
    m_OptDlgS.NormStyArrowHeadIndex=TabPage4->m_NormStyArrowHeadIndex;

    m_OptDlgS.NormShowChk=TabPage4->m_NormShowChk;
    m_OptDlgS.NormRadioElemOrSur=TabPage4->m_NormRadioElemOrSur;

    m_OptDlgS.NormClrFacePlot=TabPage4->m_NormClrFacePlot;
    m_OptDlgS.NormClrBeamTangPlot=TabPage4->m_NormClrBeamTangPlot;
    m_OptDlgS.NormClrBeamN1Plot=TabPage4->m_NormClrBeamN1Plot;
    m_OptDlgS.NormClrBeamN2Plot=TabPage4->m_NormClrBeamN2Plot;
    
}
//--将数据写入Xml文件
void QPostWidPlotOptDlg::ExportData()
{
	QString FilePathName;
	FilePathName=QCoreApplication::applicationDirPath()+PLOTOPTNAME;
	QFile file(FilePathName);
	if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
		return;
	}
	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartElement("PlotOption");
	QString str;

	writer.writeStartElement("显示");
	str=QString("%1").arg(m_OptDlgS.BaseRenderIndex);
	writer.writeTextElement("Bs1",str);
	str=QString("%1").arg(m_OptDlgS.BaseScaleInder);
	writer.writeTextElement("Bs2",str);
	str=QString("%1").arg(m_OptDlgS.BaseVisIndex);
	writer.writeTextElement("Bs3",str);
	writer.writeEndElement();

	writer.writeStartElement("Color");
	str=QString("%1").arg(m_OptDlgS.ClrEdgesWHPlot);
	writer.writeTextElement("Clr1",str);
	str=QString("%1").arg(m_OptDlgS.ClrEdgesFSPlot);
	writer.writeTextElement("Clr2",str);
	str=QString("%1").arg(m_OptDlgS.ClrEdgesCSPLot);
	writer.writeTextElement("Clr3",str);
    str=QString("%1").arg(m_OptDlgS.ClrHisCuvPoint);
    writer.writeTextElement("Clr4",str);
    str=QString("%1").arg(m_OptDlgS.ClrHisPointfactor);
    writer.writeTextElement("Clr5",str);
	str=QString("%1").arg(m_OptDlgS.ClrEdgeAStyIndex);
	writer.writeTextElement("Clr6",str);
	str=QString("%1").arg(m_OptDlgS.ClrEdgeAThikIndex);
	writer.writeTextElement("Clr7",str);
	writer.writeEndElement();

    writer.writeStartElement("Label");
    str=QString("%1").arg(m_OptDlgS.LabElemChk);
    writer.writeTextElement("Lab1",str);
    str=QString("%1").arg(m_OptDlgS.LabFaceChk);
    writer.writeTextElement("Lab2",str);
    str=QString("%1").arg(m_OptDlgS.LabNodeChk);
    writer.writeTextElement("Lab3",str);
    str=QString("%1").arg(m_OptDlgS.LabNodSymbolChk);
    writer.writeTextElement("Lab4",str);
    str=QString("%1").arg(m_OptDlgS.LabClrShowElem);
    writer.writeTextElement("Lab5",str);
    str=QString("%1").arg(m_OptDlgS.LabClrShowFace);
    writer.writeTextElement("Lab6",str);
    str=QString("%1").arg(m_OptDlgS.LabClrShowNode);
    writer.writeTextElement("Lab7",str);
    str=QString("%1").arg(m_OptDlgS.LabClrShowNodeSymb);
    writer.writeTextElement("Lab8",str);
    str=QString("%1").arg(m_OptDlgS.LabNodSymbolIndex);
    writer.writeTextElement("Lab9",str);
    str=QString("%1").arg(m_OptDlgS.LabNodSizeIndex);
    writer.writeTextElement("Lab10",str);
    writer.writeEndElement();

    //Normals
    writer.writeStartElement("矢量");
    str=QString("%1").arg(m_OptDlgS.NormShowChk);
    writer.writeTextElement("Norm1",str);
    str=QString("%1").arg(m_OptDlgS.NormRadioElemOrSur);
    writer.writeTextElement("Norm2",str);
    str=QString("%1").arg(m_OptDlgS.NormClrFacePlot);
    writer.writeTextElement("Norm3",str);
    str=QString("%1").arg(m_OptDlgS.NormClrBeamTangPlot);
    writer.writeTextElement("Norm4",str);
    str=QString("%1").arg(m_OptDlgS.NormClrBeamN1Plot);
    writer.writeTextElement("Norm5",str);
    str=QString("%1").arg(m_OptDlgS.NormClrBeamN2Plot);
    writer.writeTextElement("Norm6",str);
    str=QString("%1").arg(m_OptDlgS.NormStyLenIndex);
    writer.writeTextElement("Norm7",str);
    str=QString("%1").arg(m_OptDlgS.NormStyLinThickIndex);
    writer.writeTextElement("Norm8",str);
    str=QString("%1").arg(m_OptDlgS.NormStyArrowHeadIndex);
    writer.writeTextElement("Norm9",str);
    writer.writeEndElement();


	writer.writeEndElement();
	writer.writeEndDocument();
	file.close();
}

//*****************************************************
//*****************************************************
//--QPostWidPlotOpt_Basic
//*****************************************************
//*****************************************************
QPostWidPlotOpt_Basic::QPostWidPlotOpt_Basic(QWidget *parent)
: QWidget(parent)
{
    //-1
    QGroupBox *gbBox1 = new QGroupBox("渲染类型",this);
    QWidget *VBox1Widget = new QWidget(gbBox1);
    m_btnGrpRender = new QButtonGroup(VBox1Widget);
     
    m_RadioRender[0] = new QRadioButton(tr("线框"),VBox1Widget);
    m_RadioRender[1] = new QRadioButton(tr("边"),VBox1Widget);//(tr("Hidden"),gbBox1);
    m_RadioRender[2] = new QRadioButton(tr("阴影"),VBox1Widget);//(tr("Filled"),gbBox1);
    m_RadioRender[3] = new QRadioButton(tr("阴影线框"),VBox1Widget);
    m_RadioRender[4] = new QRadioButton(tr("所有边"),VBox1Widget);
    m_RadioRender[5] = new QRadioButton(tr("外边缘"),VBox1Widget);
    m_RadioVisible[0] = new QRadioButton(tr("所有边"),VBox1Widget);
    m_RadioVisible[1] = new QRadioButton(tr("外边缘"),VBox1Widget);
    m_RadioVisible[2] = new QRadioButton(tr("特征边"),VBox1Widget);
    m_RadioVisible[3] = new QRadioButton(tr("自由边"),VBox1Widget);
    m_RadioVisible[4] = new QRadioButton(tr("不显示边"),VBox1Widget);

    m_RadioVisible[0]->setVisible(false);
    m_RadioVisible[1]->setVisible(false); 
    m_RadioVisible[2]->setVisible(false);
    m_RadioVisible[3]->setVisible(false);
    m_RadioVisible[4]->setVisible(false);
    
    m_btnGrpRender->addButton(m_RadioRender[0],0);
    m_btnGrpRender->addButton(m_RadioRender[1],1);
    m_btnGrpRender->addButton(m_RadioRender[2],2);
    m_btnGrpRender->addButton(m_RadioRender[3],3);
    m_btnGrpRender->addButton(m_RadioRender[4],4);
    m_btnGrpRender->addButton(m_RadioRender[5],5);

    m_RadioRender[1]->setVisible(false);
    //m_RadioRender[0]->setMaximumWidth(100);
    QVBoxLayout *VLayout1= new QVBoxLayout(VBox1Widget);
    VLayout1->addWidget(m_RadioRender[0]);
    VLayout1->addWidget(m_RadioRender[1]);
    VLayout1->addWidget(m_RadioRender[2]);
    VLayout1->addWidget(m_RadioRender[3]);
    VLayout1->addWidget(m_RadioRender[4]);
    VLayout1->addWidget(m_RadioRender[5]);
    VLayout1->setContentsMargins(10,20,0,0);
    connect(m_btnGrpRender,SIGNAL(buttonClicked(int)),this,SLOT(setRenderSlot(int)));

    //-2
    QGroupBox *gbBox2 = new QGroupBox("变形放大因子",this);
    QWidget *Vgb2Widget = new QWidget(gbBox2);
    QVBoxLayout *VLayout2 = new QVBoxLayout(Vgb2Widget);
    m_btnGrpScale = new QButtonGroup(Vgb2Widget);
    m_RadioScale[0]= new QRadioButton(tr("自动"),Vgb2Widget);
    m_RadioScale[1] = new QRadioButton(tr("均匀"),Vgb2Widget);
    m_RadioScale[2] = new QRadioButton(tr("不均匀"),Vgb2Widget);
    m_btnGrpScale->addButton(m_RadioScale[0],0);
    m_btnGrpScale->addButton(m_RadioScale[1],1);
    m_btnGrpScale->addButton(m_RadioScale[2],2);
    m_RadioScale[2]->setVisible(false);
    m_RadioScale[1]->setEnabled(false);
    VLayout2->setContentsMargins(10,20,0,0);

    m_valLab = new QLabel(tr("值:"),Vgb2Widget);
    m_valEdit = new QLineEdit(Vgb2Widget);
    QVBoxLayout *V1Layout=new QVBoxLayout();
    V1Layout->addWidget(m_RadioScale[0]);
    V1Layout->addWidget(m_RadioScale[1]);
    V1Layout->addWidget(m_RadioScale[2]);
    
    QHBoxLayout *H1Layout= new QHBoxLayout();
    H1Layout->addWidget(m_valLab);
    H1Layout->addWidget(m_valEdit);
    m_valLab->setEnabled(false);
    m_valEdit->setEnabled(false);

    VLayout2->addLayout(V1Layout);
    VLayout2->addLayout(H1Layout);
    connect(m_btnGrpScale,SIGNAL(buttonClicked(int)),this,SLOT(setScaleSlot(int)));
   
    QGridLayout *MainLayout = new QGridLayout(this);
    MainLayout->addWidget(gbBox1,1,1);
    MainLayout->addWidget(gbBox2,2,1);
    //MainLayout->addWidget(gbBox3,1,2,2,1);
    setLayout(MainLayout);
    //setChecked();
}

QPostWidPlotOpt_Basic::~QPostWidPlotOpt_Basic()
{

}

void QPostWidPlotOpt_Basic::setRenderSlot(int Index)
{
    m_RenderIndex = Index;
}
void QPostWidPlotOpt_Basic::setScaleSlot(int Index)
{
    m_ScaleIndex = Index;
}
void QPostWidPlotOpt_Basic::setVisibleSlot(int Index)
{
    m_VisibleIndex = Index;
}

void QPostWidPlotOpt_Basic::setData()
{
    m_RadioRender[m_RenderIndex]->setChecked(true);
    m_RadioScale[m_ScaleIndex]->setChecked(true);
    m_RadioVisible[m_VisibleIndex]->setChecked(true);	
    if (m_BaseValue<0)
    {
		m_BaseValue = 1;
    }
    QString str ;
    str=QString("%1").arg(m_BaseValue);
    m_valEdit->setText(str);
}
void QPostWidPlotOpt_Basic::setDefault()
{
	m_RenderIndex=0;
	m_ScaleIndex=0;
	m_VisibleIndex=0;
    m_BaseValue=1;
	setData();		
}
//*****************************************************
//*****************************************************
//--QPostWidPlotOpt_Color & style
//*****************************************************

QPostWidPlotOpt_Color::QPostWidPlotOpt_Color(QWidget *parent)
: QWidget(parent)
{
	//m_TabWig=new QTabWidget(this);
	//QPushButton *btn1=new QPushButton(m_TabWig);
	//QPushButton *btn2=new QPushButton(m_TabWig);
	//QPushButton *btn3=new QPushButton(m_TabWig);
	//int index = m_TabWig->addTab(btn1,"b0");
	//index = m_TabWig->addTab(btn2,"b1");
	//index = m_TabWig->addTab(btn3,"b2");
	//QVBoxLayout *lout = new QVBoxLayout(this);
	////lout->setContentsMargins(2, 2, 2, 2);
	//lout->addWidget(m_TabWig);
	//this->setLayout(lout);
	//-1
	QGroupBox *gbBox1 = new QGroupBox("渲染类型", this);
	QLabel *Lab1 = new QLabel(tr("线框中的边:"), gbBox1);
	QLabel *Lab2 = new QLabel(tr("边填充/阴影图:"), gbBox1);
	QLabel *Lab3 = new QLabel(tr("填充颜色/阴影图:"), gbBox1);
    QLabel *Lab4 = new QLabel(tr("历史曲线点颜色:"), gbBox1);
    QLabel *lab5 = new QLabel(tr("点的尺寸:"),gbBox1);
    lab5->setAlignment(Qt::AlignRight);
    m_btnGroup = new QButtonGroup(gbBox1);
	for (int i=0; i<4; i++)
	{
			m_ColorBt[i] = new QPushButton(gbBox1);
			m_btnGroup->addButton(m_ColorBt[i], i);
			m_ColorBt[i]->setMaximumWidth(50);
	}
    m_EditPointSize = new QLineEdit(gbBox1);
	connect(m_btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(setColorScale(int)));
	QGridLayout *GLayout1= new QGridLayout(gbBox1);
	GLayout1->addWidget(Lab1,1,1);   
	GLayout1->addWidget(Lab2,2,1);
	GLayout1->addWidget(Lab3,3,1);
    GLayout1->addWidget(Lab4,4,1);
    GLayout1->addWidget(lab5,5,1);
	GLayout1->addWidget(m_ColorBt[0], 1, 2);
	GLayout1->addWidget(m_ColorBt[1], 2, 2);
	GLayout1->addWidget(m_ColorBt[2], 3, 2);
    GLayout1->addWidget(m_ColorBt[3], 4, 2);
    GLayout1->addWidget(m_EditPointSize, 5, 2);

//-2
	QGroupBox *gbBox2 = new QGroupBox("边属性",this);
	QLabel *LabEdge1 = new QLabel(tr("类型:"), gbBox2);
	QLabel *LabEdge2 = new QLabel(tr("厚度:"), gbBox2);
	m_comboxSty = new QComboBox(gbBox2);
	m_comboxThick = new QComboBox(gbBox2);
	m_comboxSty->setMaximumWidth(100);
	m_comboxThick->setMaximumWidth(100);
	QIcon icon;
	icon.addFile(":/images/welcme.png");
		
	/* m_comboxSty->addItem(icon,tr(""));
		m_comboxSty->addItem(icon,tr(""));*/
		//m_comboxSty->addItem(QIcon(":/images/save.png"));
/*		m_comboxSty->addItem(tr("__________"));
		m_comboxSty->addItem(tr("……………………"));
		m_comboxSty->addItem(tr("― ― ― ― ― ― ―"));*/

    m_comboxSty->addItem(tr("实线"),  Qt::SolidLine);
	m_comboxSty->addItem(tr("虚线"),  Qt::DotLine);
    m_comboxSty->addItem(tr("点"),  Qt::DotLine);
	m_comboxThick->addItem(tr("0.75"));
	m_comboxThick->addItem(tr("1"));
	m_comboxThick->addItem(tr("1.5"));
	m_comboxThick->addItem(tr("2"));

	QGridLayout *GridLayout2 = new QGridLayout(gbBox2);
	GridLayout2->addWidget(LabEdge1,1,2,1,1);
	GridLayout2->addWidget(m_comboxSty,1,3,1,1);
	GridLayout2->addWidget(LabEdge2,1,4,1,1);
	GridLayout2->addWidget(m_comboxThick,1,5,1,1);

//-3
	QGridLayout *MainLayout = new QGridLayout(this);
	MainLayout->addWidget(gbBox1,1,1);
	MainLayout->addWidget(gbBox2,2,1);
	setLayout(MainLayout);
}

QPostWidPlotOpt_Color::~QPostWidPlotOpt_Color()
{

}
void QPostWidPlotOpt_Color::setData()
{
	m_comboxSty->setCurrentIndex(m_ClrEdgeAStyIndex);
	m_comboxThick->setCurrentIndex(m_ClrEdgeAThikIndex);
	m_ColorBt[0]->setStyleSheet("background-color:"+m_ClrEdgeWHPlot);
	m_ColorBt[1]->setStyleSheet("background-color:"+m_ClrEdgeFSOPlot);
	m_ColorBt[2]->setStyleSheet("background-color:"+m_ClrEdgeCFSPlot);
    m_ColorBt[3]->setStyleSheet("background-color:"+m_ClrHisCuvPonit);
    m_EditPointSize->setText(m_ClrHisCuvPonitSize);
}
void QPostWidPlotOpt_Color::setDefault()
{
    m_ClrEdgeAStyIndex=0;
    m_ClrEdgeAThikIndex=0;
    m_ClrEdgeWHPlot="rgb(255,0,0)";
    m_ClrEdgeFSOPlot="rgb(0,255,0)";
    m_ClrEdgeCFSPlot="rgb(0,0,255)";
    m_ClrHisCuvPonit="rgb(255,0,0)";
    m_ClrHisCuvPonitSize="0.02";

    setData();
}
void QPostWidPlotOpt_Color::setColorScale(int btnId)
{
    QColor color;
    color = QColorDialog::getColor(Qt::green, this);
	
    if (color.isValid())
    {
        QString str;
        str=ColorToString(color,"RGB");
        m_ColorBt[btnId]->setStyleSheet("background-color:"+str);
		if (btnId==0){m_ClrEdgeWHPlot=str;
		}else if (btnId==1){m_ClrEdgeFSOPlot=str;
		}else if (btnId==2){m_ClrEdgeCFSPlot=str;
        }else if (btnId==3){m_ClrHisCuvPonit=str;
		}
    }
}
QString QPostWidPlotOpt_Color::ColorToString(const QColor &color, const QString type)
{
    if (type=="RGB"){
    return QString("rgba(%1,%2,%3)")
		.arg(color.red())
		.arg(color.green())
		.arg(color.blue());
    }
	return "";
	//return color.name;
}
//*****************************************************
//*****************************************************
//--QPostWidPlotOpt_Lables
//*****************************************************
QPostWidPlotOpt_Labels::QPostWidPlotOpt_Labels(QWidget *parent)
: QWidget(parent)
{
	//-1
    QGroupBox *gbBox1 = new QGroupBox("标签和符号", this);
    QPushButton *setFont1 = new QPushButton(tr("为所有模型标签设置字体"),gbBox1);
    //QPushButton *setFont2 = new QPushButton(tr("Set Font"),gbBox1);
    m_Lab1 = new QLabel(tr("颜色:"), gbBox1);
    m_Lab2 = new QLabel(tr("颜色:"), gbBox1);
    m_Lab3 = new QLabel(tr("颜色:"), gbBox1);
    m_Lab4 = new QLabel(tr("颜色:"), gbBox1);
    m_btnGroup = new QButtonGroup(gbBox1);
    for (int i=0; i<4; i++)
    {
	    m_ColorBt[i] = new QPushButton(gbBox1);
	    m_btnGroup->addButton(m_ColorBt[i], i);
    }
    connect(m_btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(setColorScaleSlot(int)));
    m_chk1 = new QCheckBox(tr("单元编号"), gbBox1);
    m_chk2 = new QCheckBox(tr("面编号"), gbBox1);
    m_chk3 = new QCheckBox(tr("节点编号"), gbBox1);
    m_chk4 = new QCheckBox(tr("节点形状"), gbBox1);

    m_LabSymbol = new QLabel(tr("标签:"), gbBox1);
    m_LabSize = new QLabel(tr("尺寸:"), gbBox1);
    m_comboxSymbol = new QComboBox(gbBox1);
    m_comboxSize = new QComboBox(gbBox1);

    m_comboxSymbol->addItem(tr("●"));
    m_comboxSymbol->addItem(tr("■"));
    m_comboxSymbol->addItem(tr("◆"));
    m_comboxSymbol->addItem(tr("▲"));
    m_comboxSymbol->addItem(tr("○"));
    m_comboxSymbol->addItem(tr("□"));
    m_comboxSymbol->addItem(tr("◇"));
    m_comboxSymbol->addItem(tr("△"));
    m_comboxSymbol->addItem(tr("﹢"));
    m_comboxSymbol->addItem(tr("×"));

    m_comboxSize->addItem(tr("小"));
    m_comboxSize->addItem(tr("中"));
    m_comboxSize->addItem(tr("大"));
    connect(m_chk1,SIGNAL(stateChanged(int)),this,SLOT(ChkShowElemLabSlot()));
    connect(m_chk2,SIGNAL(stateChanged(int)),this,SLOT(ChkShowFaceLabSlot()));
    connect(m_chk3,SIGNAL(stateChanged(int)),this,SLOT(ChkShowNodeLabSlot()));
    connect(m_chk4,SIGNAL(stateChanged(int)),this,SLOT(ChkShowNodeSysSlot()));
    QGridLayout *GLayout1= new QGridLayout(gbBox1);
    GLayout1->addWidget(setFont1,1,1); 
    GLayout1->addWidget(m_chk1,2,1);   
    GLayout1->addWidget(m_chk2,3,1);
    GLayout1->addWidget(m_chk3,4,1);
    GLayout1->addWidget(m_chk4,5,1);
    GLayout1->addWidget(m_Lab1,2,2);   
    GLayout1->addWidget(m_Lab2,3,2);
    GLayout1->addWidget(m_Lab3,4,2);
    GLayout1->addWidget(m_Lab4,5,2);

    GLayout1->addWidget(m_ColorBt[0], 2, 3);
    GLayout1->addWidget(m_ColorBt[1], 3, 3);
    GLayout1->addWidget(m_ColorBt[2], 4, 3);
    GLayout1->addWidget(m_ColorBt[3], 5, 3);
    GLayout1->addWidget(m_LabSymbol, 6, 2);
    GLayout1->addWidget(m_LabSize, 6, 4);
    GLayout1->addWidget(m_comboxSymbol, 6, 3);
    GLayout1->addWidget(m_comboxSize, 6, 5);

    ////-2
    //			QGroupBox *gbBox2 = new QGroupBox("Edge Attributes",this);
    //			QLabel *LabEdge1 = new QLabel(tr("Style:"), gbBox2);
    //			QLabel *LabEdge2 = new QLabel(tr("Thickness:"), gbBox2);
    //			m_comboxSty = new QComboBox(gbBox2);
    //			m_comboxThick = new QComboBox(gbBox2);
    //	
    //		
    //	 /*	m_comboxSty->addItem(tr("━━━━━━━━━"));
    //			m_comboxSty->addItem(tr("― •― ― ― ― ― ―"));
    //			m_comboxSty->addItem(tr("……………………"));
    //			m_comboxSty->addItem(tr("― ― ― ― ― ― ―"));*/


    //			//m_comboxSty->addItem(tr("Dot"),  Qt:Dot);

    //			QGridLayout *GridLayout2 = new QGridLayout(gbBox2);
    //			GridLayout2->addWidget(LabEdge1,1,1,1,1);
    //			GridLayout2->addWidget(m_comboxSty,1,2,1,1);
    //			GridLayout2->addWidget(LabEdge2,1,3,1,1);
    //			GridLayout2->addWidget(m_comboxThick,1,4,1,1);
    //			
    //			//GridLayout2->addWidget(m_valLab,4,1,1,1);
    //			//GridLayout2->addWidget(m_valEdit,4,2,1,1);
    //			//VLayout2->addLayout(HLayout2);

    //-3
    QGridLayout *MainLayout = new QGridLayout(this);
    MainLayout->addWidget(gbBox1,1,1);
    //MainLayout->addWidget(gbBox2,2,1);
    setLayout(MainLayout);
}
QPostWidPlotOpt_Labels::~QPostWidPlotOpt_Labels()
{

}

void QPostWidPlotOpt_Labels::setColorScaleSlot(int btnId)
{
	QColor color;
	color = QColorDialog::getColor(Qt::green, this);

	if (color.isValid())
	{
		QString str;
		str=ColorToString(color,"RGB");
		m_ColorBt[btnId]->setStyleSheet("background-color:"+str);
		//str = color.name();
        if (btnId==0){m_LabClrShowElem=str;
        }else if (btnId==1){m_LabClrShowFace=str;
        }else if (btnId==2){m_LabClrShowNode=str;
        }else if (btnId==3){m_LabClrShowNodeSymb=str;
        }
	}
}
QString QPostWidPlotOpt_Labels::ColorToString(const QColor &color, const QString type)
{
	if (type=="RGB")
	{
		return QString("rgba(%1,%2,%3)")
			.arg(color.red())
			.arg(color.green())
			.arg(color.blue());
	}
	return "";
	//return color.name;
}
void QPostWidPlotOpt_Labels::ChkShowNodeSysSlot()
{
	bool bFlag = m_chk4->isChecked();
	if (bFlag==true)
	{
        m_comboxSymbol->setEnabled(true);
        m_comboxSize->setEnabled(true);
	    m_LabSymbol->setEnabled(true);
	    m_LabSize->setEnabled(true);
	    m_Lab4->setEnabled(true);
	    m_ColorBt[3]->setEnabled(true);
	}else{
        m_comboxSymbol->setEnabled(false);
        m_comboxSize->setEnabled(false);
        m_LabSymbol->setEnabled(false);
        m_LabSize->setEnabled(false);
        m_Lab4->setEnabled(false);
        m_ColorBt[3]->setEnabled(false);
	}
}
void QPostWidPlotOpt_Labels::ChkShowElemLabSlot()
{
	bool bFlag = m_chk1->isChecked();
	if (bFlag == true)
	{
		m_Lab1->setEnabled(true);
		m_ColorBt[0]->setEnabled(true);
	}else
	{
		m_Lab1->setEnabled(false);
		m_ColorBt[0]->setEnabled(false);
	}
}
void QPostWidPlotOpt_Labels::ChkShowFaceLabSlot()
{
	bool bFlag = m_chk2->isChecked();
	if (bFlag == true)
	{
		m_Lab2->setEnabled(true);
		m_ColorBt[1]->setEnabled(true);
	}else
	{
		m_Lab2->setEnabled(false);
		m_ColorBt[1]->setEnabled(false);
	}
}
void QPostWidPlotOpt_Labels::ChkShowNodeLabSlot()
{
	bool bFlag = m_chk3->isChecked();
	if (bFlag == true)
	{
		m_Lab3->setEnabled(true);
		m_ColorBt[2]->setEnabled(true);
	}else
	{
		m_Lab3->setEnabled(false);
		m_ColorBt[2]->setEnabled(false);
	}

}

void QPostWidPlotOpt_Labels::setDefault()
{
    m_symbolsStyIndex=0;
    m_symbolsSizeIndex=0;
    m_LabClrShowElem="rgb(255,0,0)";
    m_LabClrShowFace="rgb(0,255,0)";
    m_LabClrShowNode="rgb(0,0,255)";
    m_LabClrShowNodeSymb="rgb(0,0,0)";
    m_LabShowElemChk=0;
    m_LabShowFaceChk=0;
    m_LabShowNodeChk=0;
    m_LabShowNodeSymbChk=0;
    setData();
}

void QPostWidPlotOpt_Labels::setData()
{
    m_comboxSymbol->setCurrentIndex(m_symbolsStyIndex);
    m_comboxSize->setCurrentIndex(m_symbolsSizeIndex);
    m_ColorBt[0]->setStyleSheet("background-color:"+m_LabClrShowElem);
    m_ColorBt[1]->setStyleSheet("background-color:"+m_LabClrShowFace);
    m_ColorBt[2]->setStyleSheet("background-color:"+m_LabClrShowNode);
    m_ColorBt[3]->setStyleSheet("background-color:"+m_LabClrShowNodeSymb);
    m_chk1->setChecked(m_LabShowElemChk);
    m_chk2->setChecked(m_LabShowFaceChk);
    m_chk3->setChecked(m_LabShowNodeChk);
    m_chk4->setChecked(m_LabShowNodeSymbChk);
}

//*****************************************************
//*****************************************************
//--QPostWidPlotOpt_Normals
//*****************************************************
QPostWidPlotOpt_Normals::QPostWidPlotOpt_Normals(QWidget *parent)
: QWidget(parent)
{
	m_chkShowNom = new QCheckBox(tr("显示法线"), this);
	QButtonGroup *m_btnGrpVisible = new QButtonGroup(this);
	m_RadioOnElem = new QRadioButton(tr("单元上"),this);
	m_RadioOnSur = new QRadioButton(tr("面上"),this);
	m_btnGrpVisible->addButton(m_RadioOnElem,0);
	m_btnGrpVisible->addButton(m_RadioOnSur,1);
	//-1
	m_gbBox1 = new QGroupBox("颜色", this);
	m_Lab1 = new QLabel(tr("面法线:"), m_gbBox1);
	m_Lab2 = new QLabel(tr("梁切线:"), m_gbBox1);
	m_Lab3 = new QLabel(tr("梁 n1:"), m_gbBox1);
	m_Lab4 = new QLabel(tr("梁 n2:"), m_gbBox1);
	QButtonGroup *m_btnGroup = new QButtonGroup(m_gbBox1);
	for (int i=0; i<4; i++)
	{
		m_ColorBt[i] = new QPushButton(m_gbBox1);
		m_btnGroup->addButton(m_ColorBt[i], i);
		m_ColorBt[i]->setMaximumWidth(50);
	}
	connect(m_btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(setColorScaleSlot(int)));
	connect(m_chkShowNom,SIGNAL(stateChanged(int)),this,SLOT(ChkShowNomSlot()));
	connect(m_btnGrpVisible,SIGNAL(buttonClicked(int)),this,SLOT(setVisibleSlot(int)));
	
	QGridLayout *GLayout1= new QGridLayout(m_gbBox1);

	GLayout1->addWidget(m_Lab1,2,2);   
	GLayout1->addWidget(m_Lab2,3,2);
	GLayout1->addWidget(m_Lab3,4,2);
	GLayout1->addWidget(m_Lab4,5,2);

	GLayout1->addWidget(m_ColorBt[0], 2, 3);
	GLayout1->addWidget(m_ColorBt[1], 3, 3);
	GLayout1->addWidget(m_ColorBt[2], 4, 3);
	GLayout1->addWidget(m_ColorBt[3], 5, 3);

	m_gbBox2 = new QGroupBox("类型",this);


	m_LabSty1 = new QLabel(tr("长度:"), m_gbBox2);
	m_LabSty2 = new QLabel(tr("线厚度:"), m_gbBox2);
	m_LabSty3 = new QLabel(tr("箭头:"), m_gbBox2);
	m_comboxLen = new QComboBox(m_gbBox2);
	m_comboxThick = new QComboBox(m_gbBox2);
	m_comboxArrowHead = new QComboBox(m_gbBox2);

	m_comboxLen->addItem(tr("短"));
	m_comboxLen->addItem(tr("中"));
	m_comboxLen->addItem(tr("长"));

	m_comboxThick->addItem(tr("0.75"));
	m_comboxThick->addItem(tr("1"));
	m_comboxThick->addItem(tr("1.5"));
	m_comboxThick->addItem(tr("2"));

	m_comboxArrowHead->addItem(tr("No"));
	m_comboxArrowHead->addItem(tr("箭头"));
	m_comboxArrowHead->addItem(tr("实心箭头"));
	QGridLayout *GridLayout2 = new QGridLayout(m_gbBox2);
	GridLayout2->addWidget(m_LabSty1,1,1,1,1);
	GridLayout2->addWidget(m_comboxLen,1,2,1,1);
	GridLayout2->addWidget(m_LabSty2,2,1,1,1);
	GridLayout2->addWidget(m_comboxThick,2,2,1,1);
	GridLayout2->addWidget(m_LabSty3,2,3,1,1);
	GridLayout2->addWidget(m_comboxArrowHead,2,4,1,1);	

	QGridLayout *MainLayout = new QGridLayout(this);
	MainLayout->addWidget(m_chkShowNom,1,1);
	MainLayout->addWidget(m_RadioOnElem,1,2);
	MainLayout->addWidget(m_RadioOnSur,1,3);
	MainLayout->addWidget(m_gbBox1,2,1,1,3);
	MainLayout->addWidget(m_gbBox2,3,1,1,3);
	setLayout(MainLayout);
}
QPostWidPlotOpt_Normals::~QPostWidPlotOpt_Normals()
{

}

void QPostWidPlotOpt_Normals::setColorScaleSlot(int btnId)
{
	QColor color;
	color = QColorDialog::getColor(Qt::green, this);

	if (color.isValid())
	{
		QString str;
		str=ColorToString(color,"RGB");
		m_ColorBt[btnId]->setStyleSheet("background-color:"+str);
        if (btnId==0){m_NormClrFacePlot=str;
        }else if (btnId==1){m_NormClrBeamTangPlot=str;
        }else if (btnId==2){m_NormClrBeamN1Plot=str;
        }else if (btnId==3){m_NormClrBeamN2Plot=str;
        }
	}
}
QString QPostWidPlotOpt_Normals::ColorToString(const QColor &color, const QString type)
{
	if (type=="RGB")
	{
		return QString("rgba(%1,%2,%3)")
			.arg(color.red())
			.arg(color.green())
			.arg(color.blue());
	}
	return "";
}
/*
*/
void QPostWidPlotOpt_Normals::ChkShowNomSlot()
{
	bool bFlag = m_chkShowNom->isChecked();
    m_NormShowChk = bFlag;
   
	if (bFlag == true)
	{
		m_gbBox1->setEnabled(true);
		m_gbBox2->setEnabled(true);
		m_RadioOnElem->setEnabled(true);
		m_RadioOnSur->setEnabled(true);
	}else{
		m_gbBox1->setEnabled(false);
		m_gbBox2->setEnabled(false);
		m_RadioOnElem->setEnabled(false);
		m_RadioOnSur->setEnabled(false);
	}
}
/**/
void QPostWidPlotOpt_Normals::setVisibleSlot(int Index)
{
	bool bFlag = m_chkShowNom->isChecked();
    m_NormShowChk = bFlag;
	if (bFlag == true && Index == 0)//on element
	{
		m_gbBox1->setEnabled(true);
		m_Lab2->setEnabled(true);
		m_Lab3->setEnabled(true);
		m_Lab4->setEnabled(true);
		m_ColorBt[1]->setEnabled(true);
		m_ColorBt[2]->setEnabled(true);
		m_ColorBt[3]->setEnabled(true);
        m_NormRadioElemOrSur = 0;
	}
	else if (bFlag == true && Index == 1)//on surface
	{
		m_Lab2->setEnabled(false);
		m_Lab3->setEnabled(false);
		m_Lab4->setEnabled(false);
		m_ColorBt[1]->setEnabled(false);
		m_ColorBt[2]->setEnabled(false);
		m_ColorBt[3]->setEnabled(false);
        m_NormRadioElemOrSur = 1;
	}
}

void QPostWidPlotOpt_Normals::setDefault()
{
    m_NormStyArrowHeadIndex=0;
    m_NormStyLenIndex=0;
    m_NormStyLinThickIndex=0;
    m_NormClrFacePlot="rgb(0,0,0)";
    m_NormClrBeamTangPlot="rgb(0,0,0)";
    m_NormClrBeamN1Plot="rgb(0,0,0)";
    m_NormClrBeamN2Plot="rgb(0,0,0)";
    m_NormShowChk=0;
    m_NormRadioElemOrSur=0;
    setData();
}

void QPostWidPlotOpt_Normals::setData()
{
    m_comboxArrowHead ->setCurrentIndex(m_NormStyArrowHeadIndex);
    m_comboxLen->setCurrentIndex(m_NormStyLenIndex);
    m_comboxThick->setCurrentIndex(m_NormStyLinThickIndex);
    m_ColorBt[0]->setStyleSheet("background-color:"+m_NormClrFacePlot);
    m_ColorBt[1]->setStyleSheet("background-color:"+m_NormClrBeamTangPlot);
    m_ColorBt[2]->setStyleSheet("background-color:"+m_NormClrBeamN1Plot);
    m_ColorBt[3]->setStyleSheet("background-color:"+m_NormClrBeamN2Plot);
    m_chkShowNom->setChecked(m_NormShowChk);
    if (m_NormRadioElemOrSur=0){
        m_RadioOnElem->setChecked(true);
        m_RadioOnSur->setChecked(false);
    }else{
        m_RadioOnElem->setChecked(false);
        m_RadioOnSur->setChecked(true);
    }
}
//*****************************************************
//*****************************************************
//--QPostWidPlotOpt_Other
//*****************************************************
QPostWidPlotOpt_Other::QPostWidPlotOpt_Other(QWidget *parent)
: QWidget(parent)
{
}
QPostWidPlotOpt_Other::~QPostWidPlotOpt_Other()
{

}

void QPostWidPlotOpt_Other::setDefault()
{

}

void QPostWidPlotOpt_Other::setData()
{

}
