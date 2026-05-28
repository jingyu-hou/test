#include <vector>
#include <map>
using namespace std;
#include <QLabel>
#include <QComboBox>
#include <QToolButton>
#include <QGridLayout>
#include <QSpacerItem>
#include <QString>
#include <QStringList>
#include <vtkCallbackCommand.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include "../Information_Widget.h"
#include "FrdDataVIS.h"
#include "XYPlot_Panel.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QHeaderView>
#include <qmath.h>
//XYPlot_Panel::XYPlot_Panel(QWidget *parent) : QWidget(parent), frdVISObj_(0), selectedPointId_(-1)
XYPlot_Panel::XYPlot_Panel(QWidget *parent) : QDialog(parent), frdVISObj_(0), selectedPointId_(-1)
{
    m_HisCurveDlg = 0;
   
    QLabel *lb2 = new QLabel(tr("选点类型："),this);

    PickStyleComb_=new QComboBox(this);
    QStringList strList;
    //strList<<tr("添加点")<<tr("删除点")<<tr("删除所有点");
    strList<<tr("添加点")<<tr("删除所有点");
    PickStyleComb_->clear();
    PickStyleComb_->addItems(strList);
   
    //--时间、位置历史曲线
    m_gbHisCurve = new QGroupBox(this);
    m_gbHisCurve->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbHisCurve->setTitle(tr("历史曲线"));
    QLabel *lb = new QLabel(tr("变量:"), m_gbHisCurve);
    QLabel *lb3 = new QLabel(tr("曲线类型："),m_gbHisCurve);
	QLabel *lb22= new QLabel(tr("集合名称:"), m_gbHisCurve);
    PickCurveComb_=new QComboBox(m_gbHisCurve);
	SetNameComb_=new QComboBox(m_gbHisCurve);
    strList.clear();
    strList<<tr("时间历史曲线")<<tr("位置历史曲线")<<tr("压力曲线");
    PickCurveComb_->clear();
    PickCurveComb_->addItems(strList);
	SetNameComb_->clear();
   
    m_TableWiget=new QTableWidget(m_gbHisCurve);
    QStringList strListw;
    strListw<<"Id"<<"X"<<"Y"<<"Z";
    m_TableWiget->setColumnCount(strListw.size());
    m_TableWiget->verticalHeaderItem(1);
    m_TableWiget->setHorizontalHeaderLabels(strListw);
    for (int j=0;j<strListw.size();j++){
        m_TableWiget->horizontalHeader()->setResizeMode(j,QHeaderView::Stretch);
    }
    QHBoxLayout *layoutH00 = new QHBoxLayout();
    layoutH00->addWidget(lb);
	QHBoxLayout *layoutH000 = new QHBoxLayout();
    layoutH000->addWidget(lb22);
	layoutH000->addWidget(SetNameComb_);
    QHBoxLayout *layoutH0=new QHBoxLayout();
    layoutH0->addWidget(lb3);
    layoutH0->addWidget(PickCurveComb_);
    QHBoxLayout *layoutH1=new QHBoxLayout();
    layoutH1->addWidget(m_TableWiget);
    QHBoxLayout *layoutH2=new QHBoxLayout();
    m_xRbtn=new QRadioButton(tr("x"),m_gbHisCurve);
    m_yRbtn=new QRadioButton(tr("y"),m_gbHisCurve);
    m_zRbtn=new QRadioButton(tr("z"),m_gbHisCurve);
    m_disRbtn=new QRadioButton(tr("Dis"),m_gbHisCurve);
    layoutH2->addWidget(m_xRbtn);layoutH2->addWidget(m_yRbtn);
    layoutH2->addWidget(m_zRbtn);layoutH2->addWidget(m_disRbtn);

    QVBoxLayout *layoutGBHis=new QVBoxLayout(m_gbHisCurve);

    layoutGBHis->addLayout(layoutH0);
	layoutGBHis->addLayout(layoutH000);
    layoutGBHis->addLayout(layoutH00);
    layoutGBHis->addLayout(layoutH1);
    layoutGBHis->addLayout(layoutH2);

    pickPointBT_ = new QToolButton(this);
    pickPointBT_->setIcon(QIcon(":/images/arrow.png"));
    pickPointBT_->setText(tr("选择点"));
    pickPointBT_->setCheckable(true);
    pickPointBT_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    createPlotBT_ = new QPushButton(tr("创建曲线"), this);
    
    m_gbPoint=new QGroupBox(this);
    m_gbPoint->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    m_gbPoint->setTitle(tr("点设置"));
    
    m_pointSizeLab = new QLabel(tr("点大小:"),m_gbPoint);
    m_pointSizeEdit = new QLineEdit(m_gbPoint);
    m_pointClrBtn = new QPushButton(tr("颜色"),m_gbPoint);
    m_PointLabChk = new QCheckBox(tr("标签"),m_gbPoint);
    m_PointApplyBtn = new QPushButton(tr("应用"),m_gbPoint);
    m_pointClrBtn->setFixedWidth(50);m_PointApplyBtn->setFixedWidth(50);
    QHBoxLayout *layoutGB=new QHBoxLayout(m_gbPoint);
    layoutGB->addWidget(m_pointSizeLab);
    layoutGB->addWidget(m_pointSizeEdit);
    layoutGB->addWidget(m_pointClrBtn);
    layoutGB->addWidget(m_PointLabChk);
    layoutGB->addWidget(m_PointApplyBtn);
    m_PointLabChk->setChecked(true);
    QGridLayout *mainLay = new QGridLayout;
    //mainLay->addWidget(lb, 0, 0);
    //mainLay->addWidget(varComb_, 0, 1);
    mainLay->addWidget(lb2,1,0);
    mainLay->addWidget(PickStyleComb_,1,1);
    mainLay->addWidget(m_gbPoint, 2, 0, 1, 2);
    //mainLay->addWidget(lb3,3,0);
    //mainLay->addWidget(PickCurveComb_,3,1);
    mainLay->addWidget(m_gbHisCurve, 3, 0, 1, 2);
    mainLay->addWidget(pickPointBT_, 4, 0);
    mainLay->addWidget(createPlotBT_, 4, 1);
 
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLay->addItem(verticalSpacer, 5, 0);
    this->setLayout(mainLay);


    pointPickCallback_ = 0;
    m_CombIndex=0;
    connect(pickPointBT_, SIGNAL(toggled(bool)), this, SLOT(SwitchPointPick(bool)));
    connect(createPlotBT_, SIGNAL(clicked()), this, SLOT(CreateXYPlot()));
    connect(PickStyleComb_,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeCombStyleSlot(int)));
    connect(PickCurveComb_,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeCombCurveSlot(int)));
    connect(m_pointClrBtn, SIGNAL(clicked()), this, SLOT(PointClrBtnSlot()));
    connect(m_PointApplyBtn,SIGNAL(clicked()),this,SLOT(PointApplyBtnSlot()));
   
    m_xRbtn->hide();m_yRbtn->hide();m_zRbtn->hide();m_disRbtn->hide();
    m_ChgxyzShow = new QButtonGroup(this);
    m_ChgxyzShow->addButton(m_xRbtn, 0); m_ChgxyzShow->addButton(m_yRbtn, 1);
    m_ChgxyzShow->addButton(m_zRbtn, 2); m_ChgxyzShow->addButton(m_disRbtn, 3);
    m_xRbtn->setChecked(true);m_yRbtn->setChecked(false);
    m_zRbtn->setChecked(false);m_disRbtn->setChecked(false);

}

XYPlot_Panel::~XYPlot_Panel()
{
    if (pointPickCallback_)  pointPickCallback_->Delete();
    m_SelectedPointS.clear();
}

void XYPlot_Panel::EnsureRenderDialog()
{
    if (m_HisCurveDlg) return;
    m_HisCurveDlg = XYPlot_RenderDialog::GetInstance();
    connect(m_HisCurveDlg->m_ExprotDataBtn,SIGNAL(clicked()),this,SLOT(SaveDataSlot()), Qt::UniqueConnection);
    plot_.SetVtkRender(m_HisCurveDlg->GetRenderer());
}

void XYPlot_Panel::PointClrBtnSlot()
{
    QColor color = QColorDialog::getColor(QColor(Qt::green), this);
    if (!color.isValid())  return;
    m_pointClrBtn->setStyleSheet(QString("background:rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
    m_pointColor.r=(double)(color.red()/255.0);m_pointColor.g=(double)(color.green()/255.0);
    m_pointColor.b=(double)(color.blue()/255.0);
}
void XYPlot_Panel::PointApplyBtnSlot()
{
    m_pointSize=m_pointSizeEdit->text().toDouble();
    frdVISObj_->SetColorSize(m_pointSize, m_pointColor);
    frdVISObj_->SetPickPointLabVisible(m_PointLabChk->isChecked());
}
void XYPlot_Panel::InitPlotData(FrdDataVIS *frdObj)
{
    frdVISObj_ = frdObj;
    //get all variables and assign to gui's combobox
  /*  const map<QString, QStringList> *varMap = frdVISObj_->GetScalarInfo();
    QString header(varMap->begin()->first);
    QString headerName(header.split("-").at(0));
    QStringList vars; QStringList poistionVars;
    for (map<QString, QStringList>::const_iterator cit = varMap->begin(); cit != varMap->end(); ++cit)
    {
        if (cit->first.split("-").at(0) == headerName){  
           QString headerName2(cit->first.split("-").at(1));
           QStringList TmpList=cit->second;
           QStringList varList;varList.clear();
           for (int kk=0;kk<TmpList.size();kk++){
               varList<<headerName2+"-"+TmpList.at(kk);
           }
           vars.append(varList);  
        }
        if(!poistionVars.contains(cit->first.split("-").at(0)))
            poistionVars.append(cit->first.split("-").at(0));    
    }
    varComb_->clear();
    varComb_->addItems(vars);
    varPositionComb_->clear();
    varPositionComb_->addItems(poistionVars);*/
    frdVISObj_->ClearHisPointVtkShow();
    m_SelectedPointS.clear();
    m_TableWiget->clearContents();
}
void XYPlot_Panel::InitPlotDatData(resultDatS DatObj)
{
	DatObj_ = DatObj;
	SetNameComb_->addItems(DatObj_.SetName);
}
//--时间、历史曲线切换
void XYPlot_Panel::ChangeCombCurveSlot(int curId)
{
    if(curId==0){
		m_TableWiget->show();
        m_xRbtn->hide();m_yRbtn->hide();m_zRbtn->hide();m_disRbtn->hide();
    }else if(curId==1){
		m_TableWiget->show();
        m_xRbtn->show();m_yRbtn->show();m_zRbtn->show();m_disRbtn->show();
    }else if(curId==2){
		m_TableWiget->hide();
        m_xRbtn->show();m_yRbtn->show();m_zRbtn->show();m_disRbtn->show();
    }
}
//--添加点，删除所有点
void XYPlot_Panel::ChangeCombStyleSlot(int curId)
{
    m_CombIndex=curId;
}

void XYPlot_Panel::ReleaseCurrentPointPick()
{
    if (!frdVISObj_)  return;
    vtkRenderWindow *renWin = frdVISObj_->GetBindedRenderer()->GetRenderWindow();
    vtkRenderWindowInteractor *ir = renWin->GetInteractor();
    if (pointPickCallback_ == 0){
       return;
    } 
    //ir->RemoveObservers(vtkCommand::LeftButtonPressEvent, pointPickCallback_); 
    ir->RemoveObservers(vtkCommand::SelectionChangedEvent, pointPickCallback_); 
}
void XYPlot_Panel::SwitchPointPick(bool flag)
{
	if(PickCurveComb_->currentText()=="压力曲线")return;
    if (!frdVISObj_)  return;

    vtkRenderWindow *renWin = frdVISObj_->GetBindedRenderer()->GetRenderWindow();
    vtkRenderWindowInteractor *ir = renWin->GetInteractor();
    
    if (pointPickCallback_ == 0){
        pointPickCallback_ = vtkCallbackCommand::New();
        pointPickCallback_->SetCallback(Callback_PickPoint);
        pointPickCallback_->SetClientData(this); 
        
    }
   
    if(!flag ){
        ir->RemoveObservers(vtkCommand::LeftButtonPressEvent, pointPickCallback_);
        //ir->RemoveObservers(vtkCommand::SelectionChangedEvent, pointPickCallback_);
    }else{
       ir->AddObserver(vtkCommand::LeftButtonPressEvent, pointPickCallback_);
       /* vtkSmartPointer *m_bandstyle=vtkSmartPointer::New();
        m_bandstyle->RenderOnMouseMoveOff();
        m_bandstyle
       ir->AddObserver(vtkCommand::SelectionChangedEvent, pointPickCallback_);*/
    }
}
//#include <vtkCellPicker.h>
//#include <vtkIdTypeArray.h>
//#include <vtkSmartPointer.h>
//#include <vtkSelectionNode.h>
//#include <vtkSelection.h>
//#include <vtkExtractSelection.h>
//#include <vtkProperty.h>
void XYPlot_Panel::Callback_PickPoint(vtkObject *caller, unsigned long, void *clientdata, void*)
{
    vtkRenderWindowInteractor *ir = vtkRenderWindowInteractor::SafeDownCast(caller);
    vtkRenderWindow *renWin = ir->GetRenderWindow();
    
    XYPlot_Panel *w = static_cast<XYPlot_Panel*>(clientdata);
    if (ir == 0 || w == 0)  return;
    int winx, winy;
    ir->GetEventPosition(winx, winy);
    //vtkCellPicker *pickerCell= vtkCellPicker::New();
    //pickerCell->SetTolerance(0.01);
    //vtkRenderer *renderer = renWin->GetRenderers()->GetFirstRenderer();
    //pickerCell->Pick(winx, winy, 0, renderer);
    //int CellId = pickerCell->GetCellId()+1;
    //QString info1(QString("cellId%1(%1) picked.").arg(CellId));
    //Information_Widget::GetInstance()->ShowInformation(info1);
    //if (CellId)
    //{
    //    vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New(); 
    //    ids->SetNumberOfComponents(1);
    //    ids->InsertNextValue(CellId);
    //    vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New(); 
    //    selectionNode->SetFieldType(vtkSelectionNode::CELL);
    //    selectionNode->SetContentType(vtkSelectionNode::INDICES);
    //    selectionNode->SetSelectionList(ids);

    //    vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
    //    selection->AddNode(selectionNode);

    //    //vtkSmartPointer<vtkExtractSelection> Exselection = vtkSmartPointer<vtkExtractSelection>::New();
    //    //Exselection->

    //    vtkAbstractMapper3D* CellMapper = pickerCell->GetMapper();
    //    vtkActor* cellActor = pickerCell->GetActor();

    //    cellActor->GetProperty()->EdgeVisibilityOn();
    //    cellActor->GetProperty()->SetEdgeColor(0,0,1);
    //    cellActor->GetProperty()->SetLineWidth(10);
    //}
   

    vtkPointPicker *picker = vtkPointPicker::New();
    picker->SetTolerance(0.01);
    vtkRenderer *renderer = renWin->GetRenderers()->GetFirstRenderer();
    picker->Pick(winx, winy, 0, renderer);
   /* vtkAssemblyPath *path=picker->GetPath();
    double cen[3];
    ((vtkActor*)(path->GetFirstNode()->GetViewProp()))->GetMapper()->GetCenter(cen);
    double pos[3];
    renderer->GetActiveCamera()->GetPosition(pos);
    vtkMapper *mm=((vtkActor*)(path->GetFirstNode()->GetViewProp()))->GetMapper();
   // picker-> (pos,cen,0.1,path,(vtkProp3D*)(path->GetFirstNode()->GetViewProp()), mm);
    int id=picker->GetPointId();*/

    int pointId = picker->GetPointId();

    double xyz[3];
    picker->GetPickPosition(xyz);
    if (pointId<0){
        Information_Widget::GetInstance()->ShowInformation("No Acotor selected.");
        return;
    }
    QString str=picker->GetMapper()->GetClassName();
    
    w->UpdataSelectedPoint(picker->GetActor(),str,pointId+1, xyz[0], xyz[1], xyz[2]);//Frd data 对应的ID值从1开始，因此需要+1;
    //cout << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << endl;
}

void XYPlot_Panel::UpdataSelectedPoint(vtkActor *actor,QString strMapperName,int pointId, double x, double y, double z)
{
    if (!frdVISObj_)return;
    pointId=frdVISObj_->GetPointId(m_varPositionComb,x,y,z);
    selectedPointId_=pointId;
    SelectedPointS TmpSelectP;
    TmpSelectP.selectedPointId_=pointId;
    TmpSelectP.selectedPointX_=x;
    TmpSelectP.selectedPointY_=y;
    TmpSelectP.selectedPointZ_=z;
    
   if (m_CombIndex==0){//添加
       if(selectedPointId_<0) {
           Information_Widget::GetInstance()->ShowInformation("No point picked.");
           return;
       }
       if (strMapperName!="vtkDataSetMapper"){
           Information_Widget::GetInstance()->ShowInformation("Point has picked...");
           return;
       }
       QString info(QString("Point%1(%2,%3,%4) picked.").arg(pointId).arg(x).arg(y).arg(z));
       Information_Widget::GetInstance()->ShowInformation(info);
        QMapIterator<int,SelectedPointS> it(m_SelectedPointS);
        while (it.hasNext()){
            it.next();
            if(m_SelectedPointS[it.key()].selectedPointId_ == TmpSelectP.selectedPointId_){
                return;
            }
        }
        if(frdVISObj_->SetHisPointVtkShow(pointId,x,y,z)){
            m_SelectedPointS.insert(m_SelectedPointS.size(),TmpSelectP);
            AddTableData(TmpSelectP);
        }
   }else{// if (m_CombIndex==2){//CLEAR ALL
       if(clearAllDisp()){
         QString info(QString("Point%1(%2,%3,%4) picked.").arg(pointId).arg(x).arg(y).arg(z));
         Information_Widget::GetInstance()->ShowInformation(info);
       }
    }
}
void XYPlot_Panel::RemoveSelectedPoint(vtkActor *actor,int pointId, double x, double y, double z)
{
    selectedPointId_=pointId;
    SelectedPointS TmpSelectP;
    TmpSelectP.selectedPointId_=pointId;
    TmpSelectP.selectedPointX_=x;
    TmpSelectP.selectedPointY_=y;
    TmpSelectP.selectedPointZ_=z;
   
    //if (delPickPointBtn_->isChecked()){//删除点
    //    QMapIterator<int,SelectedPointS> it(m_SelectedPointS);
    //    while (it.hasNext()){
    //        it.next();
    //        if(m_SelectedPointS[it.key()].selectedPointId_==TmpSelectP.selectedPointId_){
    //            frdVISObj_->RemoveHisPointVtkShow(actor);
    //            m_SelectedPointS.remove(it.key());
    //            return;
    //        }
    //    }
    //}
}
void XYPlot_Panel::InitPoint(double p,VTKColorS color)
{
    m_pointSize = p;
    m_pointColor = color;
    m_pointSizeEdit->setText(QString("%1").arg(p));
    m_pointClrBtn->setStyleSheet(QString("background:rgb(%1, %2, %3)").arg(m_pointColor.r*255).arg(m_pointColor.g*255).arg(m_pointColor.b*255));

}
bool XYPlot_Panel::clearAllDisp()
{
    if (!frdVISObj_)return false;
    int nRow=m_TableWiget->rowCount();
    if (!m_SelectedPointS.isEmpty())
    {
       QMessageBox::StandardButton bt= QMessageBox::warning(this, tr("删除"), tr("确定删除？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
        if (bt==QMessageBox::Yes){
            m_SelectedPointS.clear();
            frdVISObj_->ClearHisPointVtkShow();
            for (int kk=0;kk<nRow;kk++){  
                m_TableWiget->removeRow(0);
            }
        }   
        return false;
    }
    return true;
}
void XYPlot_Panel::AddTableData(SelectedPointS data)
{
    int nRow = m_TableWiget->rowCount();
    m_TableWiget->setRowCount(nRow+1);
   // for (int kk=0;kk<m_TableWiget->columnCount();kk++)
   // {
    QTableWidgetItem *tmpItem=new QTableWidgetItem();
    tmpItem->setData(Qt::DisplayRole,data.selectedPointId_);
    m_TableWiget->setItem(nRow,0,tmpItem);

    tmpItem=new QTableWidgetItem();
    tmpItem->setData(Qt::DisplayRole,data.selectedPointX_);
    m_TableWiget->setItem(nRow,1,tmpItem);
    tmpItem=new QTableWidgetItem();
    tmpItem->setData(Qt::DisplayRole,data.selectedPointY_);
    m_TableWiget->setItem(nRow,2,tmpItem);
    tmpItem=new QTableWidgetItem();
    tmpItem->setData(Qt::DisplayRole,data.selectedPointZ_);
    m_TableWiget->setItem(nRow,3,tmpItem);
    //}
}
void XYPlot_Panel::CreateXYPlot()
{
	infNase.clear();
    if (!frdVISObj_&&DatObj_.SetTime.size()==0)  return;
    EnsureRenderDialog();
    //if (selectedPointId_ < 0)  return;
    if(PickCurveComb_->currentIndex()==0){
        CreateXYTimePlot();
    }else if(PickCurveComb_->currentIndex()==1){
        CreateXYPositionPlot();
	}else if(PickCurveComb_->currentIndex()==2){
		CreateXYPressPlot();
    }  
    //plot_.Update();
    m_HisCurveDlg->Show();
}
//时间历史曲线
void XYPlot_Panel::CreateXYTimePlot()
{
    QString var(m_varCombParam);//(varComb_->currentText());
    plot_.DestroyXYPlot();
    int iPlotNum=m_SelectedPointS.size();
    plot_.AllocateXYPlot(iPlotNum);
    vector<bool> showFlag(iPlotNum, true);
    plot_.CreateXYPlot(showFlag);
    plot_.SetXAxisTitle(const_cast<char*>("Time"));
    plot_.SetYAxisTitle(var.toLatin1().data());
    for (int i=0;i<iPlotNum;i++)
    {
        map<double, double> timeValueMap(frdVISObj_->GetPointScalar_TimeValueMap(m_SelectedPointS[i].selectedPointId_-1, var));
        if (timeValueMap.empty())  return;
        plot_.SetGlyphType(i, const_cast<char*>("Point"));
        plot_.SetLegendLabel(i,(QString("Id=%1").arg(m_SelectedPointS[i].selectedPointId_)).toLatin1().data());
        for (map<double, double>::iterator it = timeValueMap.begin(); it != timeValueMap.end(); ++it)
        {
            plot_.AddPointXValue(it->first);
            plot_.AddPointYValue(i, it->second);
        }
    }
}
//压力曲线
void XYPlot_Panel::CreateXYPressPlot()
{
    QString var("Press");
    plot_.DestroyXYPlot();
    plot_.AllocateXYPlot(1);
    vector<bool> showFlag(1, true);
    plot_.CreateXYPlot(showFlag);
    plot_.SetXAxisTitle(const_cast<char*>("Time"));
	plot_.SetYAxisTitle(const_cast<char*>("Press"));
	plot_.SetGlyphType(0, const_cast<char*>("Point"));
	plot_.SetPlotCurveLines(true);
	plot_.SetColor(0, 1.0, 0.0, 0.0);
	int iDis=m_ChgxyzShow->checkedId();
	int Nsize=DatObj_.SetTime.size();
    QString comSetName=SetNameComb_->currentText();
	int i=SetNameComb_->currentIndex();
    for (int j=0;j<Nsize;j++)
    { 
		QStringList Str=DatObj_.SetTime.at(j).split(",");
		QString SetName=Str.at(0);
		if(SetName==comSetName){	
			QString BCD;
			double Time=Str.at(1).toDouble();
			double Valuess;
			QStringList Str1=DatObj_.Force.at(j).split(",");
			if(iDis!=3){
				Valuess=Str1.at(iDis).toDouble();
			    BCD=Str.at(1)+","+Str1.at(iDis);
			}else {
				double Valuess0=Str1.at(0).toDouble();
				double Valuess1=Str1.at(1).toDouble();
				double Valuess2=Str1.at(2).toDouble();
				Valuess=qSqrt(Valuess0*Valuess0+Valuess1*Valuess1+Valuess2*Valuess2);
				 BCD=Str.at(1)+","+QString("%1").arg(Valuess);
			}
			infNase<<BCD;
			plot_.AddPointXValue(Time);
			plot_.AddPointYValue(0,Valuess);
			Str1.clear();
		}
		Str.clear();
    }
}
//位置历史曲线
void XYPlot_Panel::CreateXYPositionPlot()
{
    QString var(m_varPositionComb+"-"+m_varCombParam.split("-").at(1));//varPositionComb_->currentText()+"-"+varComb_->currentText());
    plot_.DestroyXYPlot();

    int iPlotNum=m_SelectedPointS.size();
    if (iPlotNum==1)return;

    plot_.AllocateXYPlot(1);
    vector<bool> showFlag(1, true);
    plot_.CreateXYPlot(showFlag);
    plot_.SetXAxisTitle(const_cast<char*>("Position"));
    plot_.SetYAxisTitle(var.toLatin1().data());
    plot_.SetColor(0, 1.0, 0.0, 0.0);
    plot_.SetGlyphType(0, const_cast<char*>("Point"));
    plot_.SetPlotCurveLines(true);
    //plot_.SetLegendLabel(0,(QString("Id=%1").arg(m_SelectedPointS[i].selectedPointId_)).toLatin1().data());
    double dDis=0;
    for (int i=0;i<iPlotNum;i++)
    {
        map<double, double> PoistionValueMap(frdVISObj_->GetPointScalar_TimeValueMap(m_SelectedPointS[i].selectedPointId_-1, var));
        if (PoistionValueMap.empty())  return;
        if(i!=0){
           int iDis=m_ChgxyzShow->checkedId();
           if (iDis==0){
                dDis = fabs(m_SelectedPointS[i].selectedPointX_-m_SelectedPointS[0].selectedPointX_);
           }else if (iDis == 1){
                dDis = fabs(m_SelectedPointS[i].selectedPointY_-m_SelectedPointS[0].selectedPointY_);
           }else if (iDis == 2){
                dDis = fabs(m_SelectedPointS[i].selectedPointZ_-m_SelectedPointS[0].selectedPointZ_);
           }else if (iDis == 3){
                dDis = sqrt(pow(m_SelectedPointS[i].selectedPointX_-m_SelectedPointS[0].selectedPointX_,2)+
                       pow(m_SelectedPointS[i].selectedPointY_-m_SelectedPointS[0].selectedPointY_,2)+
                       pow(m_SelectedPointS[i].selectedPointZ_-m_SelectedPointS[0].selectedPointZ_,2));
           }  
        }      
        plot_.AddPointXValue(dDis);
        double tt=PoistionValueMap.begin()->second;
        plot_.AddPointYValue(0,PoistionValueMap.begin()->second);   
    }
}
void XYPlot_Panel::SaveDataSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Choose a file name", ".",
        ("txt Files (*.txt)"));
    if (fileName.isEmpty())return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))return;
    if (PickCurveComb_->currentIndex()==0)//时间历史曲线
    {
        QString data;
        data = "*Time History Curve\n";//
        file.write(data.toAscii());
        data="Style="+m_varCombParam+"\n";//varComb_->currentText()+"\n";//类型
        file.write(data.toAscii());
        int iPlotNum=m_SelectedPointS.size();
        for (int i=0;i<iPlotNum;i++)
        {
            map<double, double> timeValueMap(frdVISObj_->GetPointScalar_TimeValueMap(m_SelectedPointS[i].selectedPointId_-1, m_varCombParam));//varComb_->currentText()));
            if (timeValueMap.empty())  return;
            data=QString("Id=%1(%2,%3,%4)\n").arg(m_SelectedPointS[i].selectedPointId_).arg(m_SelectedPointS[i].selectedPointX_).arg(m_SelectedPointS[i].selectedPointY_).arg(m_SelectedPointS[i].selectedPointZ_);/*;*/
            file.write(data.toAscii());
            for (map<double, double>::iterator it = timeValueMap.begin(); it != timeValueMap.end(); ++it)
            {
                data=QString("%1,%2\n").arg(it->first).arg(it->second);
                file.write(data.toAscii());
            }
        }
	}else if (PickCurveComb_->currentIndex()==1){//位置历史曲线
        QString data;
        data = "*Position Curve\n";//
        file.write(data.toAscii());
        data="Style=" + m_varPositionComb;//varPositionComb_->currentText();
        data+="-"+m_varCombParam.split("-").at(1)+"\n";//varComb_->currentText()+"\n";//类型
        file.write(data.toAscii());

        data="Position Distance="+m_ChgxyzShow->checkedButton()->text()+"\n";
        file.write(data.toAscii());
        int iPlotNum=m_SelectedPointS.size();

		 data="节点号      X        Y        Z       值\n";/*;*/
         file.write(data.toAscii());
        for (int i=0;i<iPlotNum;i++)
        {
            map<double, double> PoistionValueMap(frdVISObj_->GetPointScalar_TimeValueMap(m_SelectedPointS[i].selectedPointId_-1, m_varPositionComb+"-"+m_varCombParam.split("-").at(1)));//varComb_->currentText()));
            if (PoistionValueMap.empty())  return;
            //data=QString("Id=%1(%2,%3,%4)\n").arg(m_SelectedPointS[i].selectedPointId_).arg(m_SelectedPointS[i].selectedPointX_).arg(m_SelectedPointS[i].selectedPointY_).arg(m_SelectedPointS[i].selectedPointZ_);/*;*/
            //file.write(data.toAscii());
			data=QString("%1,%2,%3,%4,%5\n").arg(m_SelectedPointS[i].selectedPointId_).arg(m_SelectedPointS[i].selectedPointX_).arg(m_SelectedPointS[i].selectedPointY_).arg(m_SelectedPointS[i].selectedPointZ_).arg(PoistionValueMap.begin()->second);/*;*/
            file.write(data.toAscii());
            /*data=QString("%1\n").arg(PoistionValueMap.begin()->second);
            file.write(data.toAscii());*/
        }
	}else if(PickCurveComb_->currentIndex()==2){
		QString data;
		data = "*Press-Time Curve\n";//
		file.write(data.toAscii());
		QString Direction;
		int iDis=m_ChgxyzShow->checkedId();
		if(iDis==0)Direction="X";
		else if(iDis==1)Direction="Y";
		else if(iDis==2)Direction="Z";
		else if(iDis==3)Direction="合力";
		data = "压力曲线集合名称："+SetNameComb_->currentText()+"  压力方向:"+Direction+"\n";//
		file.write(data.toAscii());
		data="时间  压力值\n";/*;*/
		file.write(data.toAscii());
		for(int Ij=0;Ij<infNase.size();Ij++){
		data=infNase.at(Ij)+"\n";
		file.write(data.toAscii());
		}
	}
    file.close();
}
//--更新变量
void XYPlot_Panel::UpDataComb(int iStyle, QString str)
{
    if (iStyle == 1){
        QStringList strList=str.split(":"); 
        m_varPositionComb=strList.at(0);  
        m_varCombParam=m_varPositionComb.split("-").at(1)+"-"+strList.at(1);
    }else{
        m_varPositionComb="undeformed";
    }
    
}
