#include "QThermalBoundaryDlg.h"

QTBConst::QTBConst(QWidget *parent1)
      : QDialog(parent1)
{
  setWindowTitle("常数");
  ConstMotionParameters=new QLineEdit();
  ConstMotionParameters->setText("");

  QLabel *text=new QLabel();
  text->setText("常量：");
  
  QHBoxLayout *Hlayout = new QHBoxLayout();
  QHBoxLayout *Hlayout2 = new QHBoxLayout();
  m_OKBtn = new QPushButton(tr("确定"));
  m_CancelBtn = new QPushButton(tr("取消"));

  Hlayout->addStretch();
  Hlayout->addWidget(text);
  Hlayout->addWidget(ConstMotionParameters);
  Hlayout->addStretch();

  Hlayout2->addStretch();
  Hlayout2->addWidget(m_OKBtn);
  Hlayout2->addWidget(m_CancelBtn);
  Hlayout2->addStretch();

  QVBoxLayout *MainLayout0 = new QVBoxLayout(this);
  MainLayout0->addLayout(Hlayout);
  MainLayout0->addLayout(Hlayout2);
  setLayout(MainLayout0);

  connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(m_OKBtnS()));
  connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(m_CancelBtnS()));
}

void QTBConst::SetInpDataC(FSystem FSData){
    ConstMotionParameters->setText(FSData.MotionParameter);
	WriteToFSystemInps();
}

void QTBConst::WriteToFSystemInps(){
	CMPOutPut=ConstMotionParameters->text();
}

void QTBConst:: m_OKBtnS()
{
  WriteToFSystemInps();
  this->accept();
}

void QTBConst:: m_CancelBtnS()
{
  this->close();
}

QTBConst::~QTBConst()
{

}


 ThermalBoundary::ThermalBoundary(QWidget *parent)
 {
	 //开始初始化索引
	 int JJII=0;
	 for(int II=0;II<20;II++){
		 for(int JJ=0;JJ<2;JJ++){
			 count01[JJ][II]=0;
			 IndexNumber01[JJII]=0;
			 count02[JJ][II]=0;
			 IndexNumber02[JJII]=0;
			 count03[JJ][II]=0;
			 IndexNumber03[JJII]=0;
			 count04[JJ][II]=0;
			 IndexNumber04[JJII]=0;
			 JJII++;
		 }
	 }
	//初始化索引结束
	 setWindowTitle("热边界");
	 QHBoxLayout *Hlayout = new QHBoxLayout();
     QHBoxLayout *Hlayout2 = new QHBoxLayout();

	 m_ApplyBtn = new QPushButton(tr("应用"));
	 m_AddBtn = new QPushButton(tr("增加"));
	 m_DelBtn = new QPushButton(tr("删除"));
	 m_HBPreBtn = new QPushButton(tr("上一步"));
	 m_HBNextBtn = new QPushButton(tr("下一步"));
	 Hlayout->addStretch();
	 Hlayout->addWidget(m_AddBtn);
	 Hlayout->addWidget(m_DelBtn);
	 Hlayout->addStretch();
	 Hlayout2->addWidget(m_ApplyBtn);
	 Hlayout2->addWidget(m_HBPreBtn);
	 Hlayout2->addWidget(m_HBNextBtn);

	 QVBoxLayout *VLayout = new QVBoxLayout();
	 m_TableWiget = new QTableWidget();
	 //m_TableWiget->setColumnCount(11);
	 m_TableWiget->setColumnCount(8);
	 m_TableWiget->setRowCount(1);
	 m_TableWiget->verticalHeaderItem(1);
	 QStringList strList;
	 //strList<<"对流表面集合"<<"对流环境温度"<<"环境温度"<<"对流换热类型"<<"对流换热系数"<<"辐射表面集合"<<"辐射环境温度"<<"环境温度"<<"辐射类型"<<"辐射系数"<<"热边界名称";
	 strList<<"表面集合"<<"环境温度类型"<<"环境温度"<<"对流换热类型"<<"对流换热系数"<<"辐射类型"<<"辐射系数"<<"热边界名称";
	 m_TableWiget->setHorizontalHeaderLabels(strList);
	/*for (int j=0;j<strList.size();j++){
		 m_TableWiget->horizontalHeader()->setResizeMode(j,QHeaderView::ResizeToContents);
	 }*/

	 for (int i=0; i<1;i++)
	 {   
		MyCombox *TmpCombox=new MyCombox(this,i);
        //TmpCombox->addItem("新建");
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,0,TmpCombox);
        m_comboxPSetSurf1.insert(i,TmpCombox);

        TmpCombox=new MyCombox(this,i);
		TmpCombox->addItem("无");
        TmpCombox->addItem("常数");
        TmpCombox->addItem("随时间变化");
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,1,TmpCombox);
		m_AmbientTemperatureC.insert(i,TmpCombox);
   
		QIcon icon;
		icon.addFile(":/images/edit.png");
        QPushButton* m_Parameter = new QPushButton();
		m_Parameter->setIcon(icon);
	    m_TableWiget->setCellWidget(i,2,m_Parameter);
		connect(m_Parameter,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));
 
		TmpCombox=new MyCombox(this,i);
		TmpCombox->addItem("无");
        TmpCombox->addItem("常数");
        TmpCombox->addItem("随时间变化");
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,3,TmpCombox);
		m_ConvectionCoefficient.insert(i,TmpCombox);
		
        QPushButton* m_Parameter1 = new QPushButton();
		m_Parameter1->setIcon(icon);
	    m_TableWiget->setCellWidget(i,4,m_Parameter1);
		connect(m_Parameter1,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));

		/*TmpCombox=new MyCombox(this,i);
        TmpCombox->addItem("新建");
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,5,TmpCombox);
        m_comboxPSetSurf2.insert(i,TmpCombox);

		MyCombox *TmpCombox11=new MyCombox(this,i);
		TmpCombox11->addItem("无");
        TmpCombox11->addItem("常数");
        TmpCombox11->addItem("随时间变化");
        TmpCombox11->setFrame(false);
        m_TableWiget->setCellWidget(i,6,TmpCombox11);
		m_AmbientTemperatureR.insert(i,TmpCombox11);
   
        QPushButton* m_Parameter11 = new QPushButton();
		m_Parameter11->setIcon(icon);
	    m_TableWiget->setCellWidget(i,7,m_Parameter11);
		connect(m_Parameter11,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));*/

		TmpCombox=new MyCombox(this,i);
		TmpCombox->addItem("无");
        TmpCombox->addItem("常数");
        TmpCombox->addItem("随时间变化");
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,5,TmpCombox);
		m_RadiationCoefficient.insert(i,TmpCombox);
   
        QPushButton* m_Parameter2 = new QPushButton();
		m_Parameter2->setIcon(icon);
	    m_TableWiget->setCellWidget(i,6,m_Parameter2);
		connect(m_Parameter2,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));

		QTableWidgetItem* tmpItem2=new QTableWidgetItem(tr("TB0"));
        m_TableWiget->setItem(i,7,tmpItem2);
		//m_TableWiget->item(i,7)->setFlags((Qt::ItemFlags)32);
		m_ThermalBoundaryName.append("TB0");
    }
	m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_TableWiget->horizontalHeader()->setResizeMode(7, QHeaderView::Interactive);
	VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout);
    MainLayout->addLayout(Hlayout2);
    setLayout(MainLayout);
    this->setMinimumWidth(900);

	connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplyHBSlot()));
    connect(m_AddBtn,SIGNAL(clicked()),this,SLOT(AddBtnHBSlot()));
    connect(m_DelBtn,SIGNAL(clicked()),this,SLOT(DelBtnHBSlot()));
    connect(m_HBPreBtn,SIGNAL(clicked()),this,SLOT(PreHBBtnSlot()));
    connect(m_HBNextBtn,SIGNAL(clicked()),this,SLOT(NextHBBtnSlot()));  
 }

 ThermalBoundary::~ThermalBoundary()
 {

 }

 void ThermalBoundary::AddBtnHBSlot()
 {
	 int nRow =m_TableWiget->rowCount();
	 m_TableWiget->insertRow(nRow);

	 MyCombox *TmpCombox=new MyCombox(this,nRow);
	 //TmpCombox->addItem("新建");
	 TmpCombox->setFrame(false);
	 m_TableWiget->setCellWidget(nRow,0,TmpCombox);
	 m_comboxPSetSurf1.insert(nRow,TmpCombox);
	 connect(TmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comboxSlot(int,int)));

	 //MyCombox *TmpCombox001=new MyCombox(this,nRow);
	 ////TmpCombox001->addItem("新建");
	 //TmpCombox001->setFrame(false);
	 //m_TableWiget->setCellWidget(nRow,5,TmpCombox001);
	 //m_comboxPSetSurf2.insert(nRow,TmpCombox001);
	 //connect(TmpCombox001,SIGNAL(clicked(int,int)),this,SLOT(comboxSlot(int,int)));

	 //查找所有面并显示
	 MyCombox *ff = new MyCombox(this,m_comboxPSetSurf1.size());
	 //MyCombox *ff1 = new MyCombox(this,m_comboxPSetSurf2.size());
	 for (int i=0;i<nRow;i++){
		 ff=(MyCombox*)(m_TableWiget->cellWidget(i,0));
		 //ff1=(MyCombox*)(m_TableWiget->cellWidget(i,5));
		 //if (ff!=NULL&&ff1!=NULL){
		 if (ff!=NULL){
			 for (int kk=0;kk<ff->count();kk++){
				 TmpCombox->addItem(ff->itemText(kk));
				 TmpCombox->setItemData(kk,"2",(Qt::UserRole+3));
				 /*TmpCombox001->addItem(ff1->itemText(kk));
				 TmpCombox001->setItemData(kk,"2",(Qt::UserRole+3));*/
			 }
			 break;
		 }
	 }
	 m_comboxPSetSurf1.insert(nRow,TmpCombox);
	 //m_comboxPSetSurf2.insert(nRow,TmpCombox001);

	 TmpCombox=new MyCombox(this,nRow);
	 TmpCombox->addItem("无");
	 TmpCombox->addItem("常数");
	 TmpCombox->addItem("随时间变化");
	 TmpCombox->setFrame(false);
	 m_TableWiget->setCellWidget(nRow,1,TmpCombox);
	 m_AmbientTemperatureC.insert(nRow,TmpCombox);

	 QIcon icon;
	 icon.addFile(":/images/edit.png");
	 QPushButton* m_Parameter = new QPushButton();
	 m_Parameter->setIcon(icon);
	 m_TableWiget->setCellWidget(nRow,2,m_Parameter);
	 connect(m_Parameter,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));

	 TmpCombox=new MyCombox(this,nRow);
	 TmpCombox->addItem("无");
	 TmpCombox->addItem("常数");
	 TmpCombox->addItem("随时间变化");
	 TmpCombox->setFrame(false);
	 m_TableWiget->setCellWidget(nRow,3,TmpCombox);
	 m_ConvectionCoefficient.insert(nRow,TmpCombox);

	 QPushButton* m_Parameter1 = new QPushButton();
	 m_Parameter1->setIcon(icon);
	 m_TableWiget->setCellWidget(nRow,4,m_Parameter1);
	 connect(m_Parameter1,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));

	 /*MyCombox *TmpCombox11=new MyCombox(this,nRow);
	 TmpCombox11->addItem("无");
	 TmpCombox11->addItem("常数");
	 TmpCombox11->addItem("随时间变化");
	 TmpCombox11->setFrame(false);
	 m_TableWiget->setCellWidget(nRow,6,TmpCombox11);
	 m_AmbientTemperatureR.insert(nRow,TmpCombox11);

	 QPushButton* m_Parameter11 = new QPushButton();
	 m_Parameter11->setIcon(icon);
	 m_TableWiget->setCellWidget(nRow,7,m_Parameter11);
	 connect(m_Parameter11,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));*/

	 TmpCombox=new MyCombox(this,nRow);
	 TmpCombox->addItem("无");
	 TmpCombox->addItem("常数");
	 TmpCombox->addItem("随时间变化");
	 TmpCombox->setFrame(false);
	 m_TableWiget->setCellWidget(nRow,5,TmpCombox);
	 m_RadiationCoefficient.insert(nRow,TmpCombox);

	 QPushButton* m_Parameter2 = new QPushButton();
	 m_Parameter2->setIcon(icon);
	 m_TableWiget->setCellWidget(nRow,6,m_Parameter2);
	 connect(m_Parameter2,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));

	 QString TBnn="TB"+QString::number(nRow,7);
	 QTableWidgetItem* tmpItem2=new QTableWidgetItem(TBnn);
	 m_TableWiget->setItem(nRow,7,tmpItem2);
	 //m_TableWiget->item(nRow,7)->setFlags((Qt::ItemFlags)32);
	 m_ThermalBoundaryName.append(TBnn);

 }
 void ThermalBoundary::DelBtnHBSlot()
 {
	 int curRow =m_TableWiget->currentRow();
	 if(curRow==0)curRow=m_TableWiget->rowCount()-1;
	 int INumber1,INumber11,INumber2,INumber22,INumber3,INumber33,INumber4,INumber44;
	 if (curRow!=-1){
		 int Num1,Num11,Num2,Num22,Num3,Num33,Num4,Num44;
		 m_TableWiget->removeRow(curRow);
		 if(m_ThermalBoundaryName.size()-1>=curRow)m_ThermalBoundaryName.remove(curRow);
		 INumber1=IndexNumber01[2*curRow];
		 INumber11=IndexNumber01[2*curRow+1];
		 INumber2=IndexNumber02[2*curRow];
		 INumber22=IndexNumber02[2*curRow+1];
		 INumber3=IndexNumber03[2*curRow];
		 INumber33=IndexNumber03[2*curRow+1];
		 INumber4=IndexNumber04[2*curRow];
		 INumber44=IndexNumber04[2*curRow+1];

		 Num1=count01[0][curRow];
		 Num11=count01[1][curRow];
		 Num2=count02[1][curRow];
		 Num22=count02[1][curRow];
		 Num3=count03[1][curRow];
		 Num33=count03[1][curRow];
		 Num4=count04[1][curRow];
		 Num44=count04[1][curRow];
		 if(Num1==1){
			 count01[0][curRow]=0;
			 m_ATCurveDataListDlgC.takeAt(INumber1);
		 }else if(Num11==2){
			 count01[1][curRow]=0;
			 m_ATCurveDataListDlgC02.takeAt(INumber11);
		 }

		 if(Num2==1){
			 count02[0][curRow]=0;
			  m_CCCurveDataListDlg.takeAt(INumber2);
		 }else if(Num22==2){
			 count02[1][curRow]=0;
			 m_CCCurveDataListDlg02.takeAt(INumber22);
		 }

		 /*if(Num3==1){
			 count03[0][curRow]=0;
			 m_ATCurveDataListDlgR.takeAt(INumber3);
		 }else if(Num33==2){
			 count03[1][curRow]=0;
			 m_ATCurveDataListDlgR02.takeAt(INumber33);
		 }*/

		 if(Num4==1){
			 count04[0][curRow]=0;
			 m_RCCurveDataListDlg.takeAt(INumber4);
		 }else if(Num44==2){
			 count04[1][curRow]=0;
			 m_RCCurveDataListDlg02.takeAt(INumber44);
		 }

	 }
 }
 void ThermalBoundary::ApplyHBSlot()
 {
	 WriteHBToInp();
 }
 void ThermalBoundary::CancelHBSlot()
 {
  this->close();
 }
 void ThermalBoundary::SetInpDataHB(ReadInpResultS ReadInpData)
{
	 m_strListSurf.clear();//重新加载inp文件则进行清除;
	 m_ThermalBoundaryName.clear();
	 int istep1;
	 int nRowSize = m_TableWiget->rowCount();
	 //for (int i=0;i<nRowSize;i++){ //清除全部内容，进行重新加载
		// m_TableWiget->removeRow(i);
	 //}
	 nRowSize=m_TableWiget->rowCount();
	 int nFSSysRowSize=0;//热边界中表格的行数
	 int Ns1,Ns2;
	 Ns1=ReadInpData.TmpHPSystemFInps.size();
	 Ns2=ReadInpData.TmpHPSystemRInps.size();
	 for(int i=0;i<Ns1;i++){
		 QString step1=ReadInpData.TmpHPSystemFInps.at(i).strStep;
		 QString strIn1=ReadInpData.TmpHPSystemFInps.at(i).strInclude;
		 QString Name1=ReadInpData.TmpHPSystemFInps.at(i).strCommentaryName;
		 for(int j=i+1;j<Ns1;j++){
			 QString step2=ReadInpData.TmpHPSystemFInps.at(j).strStep;
			 QString strIn2=ReadInpData.TmpHPSystemFInps.at(j).strInclude;
			 QString Name2=ReadInpData.TmpHPSystemFInps.at(i).strCommentaryName;
			 /*if(step2==step1&&strIn2==strIn1||Name1==Name2&&Name1!=""&&strIn2==strIn1){
				 ReadInpData.TmpHPSystemFInps.removeAt(j);
				 Ns1--;
			 }*/

			 if(step2==step1&&strIn2==strIn1){
				 ReadInpData.TmpHPSystemFInps.removeAt(j);
				 Ns1--;
			 }

			 /*if(step2==step1&&strIn2==strIn1||strIn2==strIn1&&Name1==Name2){
				 ReadInpData.TmpHPSystemFInps.removeAt(j);
				 Ns1--;
			 }*/
		 }
	 }

	 for(int i=0;i<Ns2;i++){
		 QString step1=ReadInpData.TmpHPSystemRInps.at(i).strStep;
		 QString strIn1=ReadInpData.TmpHPSystemRInps.at(i).strInclude;
		 QString Name1=ReadInpData.TmpHPSystemRInps.at(i).strCommentaryName;
		 for(int j=i+1;j<Ns2;j++){
			 QString step2=ReadInpData.TmpHPSystemRInps.at(j).strStep;
			 QString strIn2=ReadInpData.TmpHPSystemRInps.at(j).strInclude;
			 QString Name2=ReadInpData.TmpHPSystemRInps.at(i).strCommentaryName;
			 /*if(step2==step1&&strIn2==strIn1||Name1==Name2&&Name1!=""&&strIn2==strIn1){
				 ReadInpData.TmpHPSystemRInps.removeAt(j);
				 Ns2--;
			 }*/

			  if(step2==step1&&strIn2==strIn1){
				 ReadInpData.TmpHPSystemRInps.removeAt(j);
				 Ns2--;
			 }

			/* if(step2==step1&&strIn2==strIn1||strIn2==strIn1&&Name1==Name2){
				 ReadInpData.TmpHPSystemRInps.removeAt(j);
				 Ns2--;
			 }*/	

		 }
	 }
     QStringList nFSSysRowSize0;
	 if (ReadInpData.TmpHPSystemRInps.size()>0 || ReadInpData.TmpHPSystemFInps.size()>0){
		 for (int tt=0;tt<ReadInpData.TmpHPSystemFInps.size();tt++){
			 if (ReadInpData.TmpHPSystemFInps.at(tt).strStyle=="film" && ReadInpData.TmpHPSystemFInps.at(tt).strInclude=="1"){
				 nFSSysRowSize++;
				nFSSysRowSize0<<ReadInpData.TmpHPSystemFInps.at(tt).strStep;
			 }
		 }
		 for (int tt=0;tt<ReadInpData.TmpHPSystemRInps.size();tt++){
			 if (ReadInpData.TmpHPSystemRInps.at(tt).strStyle=="radiate" && ReadInpData.TmpHPSystemRInps.at(tt).strInclude=="1"){
				 QString sTR=ReadInpData.TmpHPSystemRInps.at(tt).strStep;
				 bool Istr=nFSSysRowSize0.contains(sTR);
				 if(!Istr)nFSSysRowSize++;  
			 }
		 }
	 }

	 if (nRowSize<nFSSysRowSize)
	 {
		 for(int nRow=nRowSize;nRow<nFSSysRowSize;nRow++){
			 m_TableWiget->insertRow(nRow);
			 MyCombox *TmpCombox=new MyCombox(this,nRow);
			 //TmpCombox->addItem("新建");
			 TmpCombox->setFrame(false);
			 m_TableWiget->setCellWidget(nRow,0,TmpCombox);
			 m_comboxPSetSurf1.insert(nRow,TmpCombox);

			 TmpCombox=new MyCombox(this,nRow);
			 TmpCombox->addItem("无");
			 TmpCombox->addItem("常数");
			 TmpCombox->addItem("随时间变化");
			 TmpCombox->setFrame(false);
			 m_TableWiget->setCellWidget(nRow,1,TmpCombox);
			 m_AmbientTemperatureC.insert(nRow,TmpCombox);

			 QIcon icon;
			 icon.addFile(":/images/edit.png");
			 QPushButton* m_Parameter = new QPushButton();
			 m_Parameter->setIcon(icon);
			 m_TableWiget->setCellWidget(nRow,2,m_Parameter);
			 connect(m_Parameter,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));


			 TmpCombox=new MyCombox(this,nRow);
			 TmpCombox->addItem("无");
			 TmpCombox->addItem("常数");
			 TmpCombox->addItem("随时间变化");
			 TmpCombox->setFrame(false);
			 m_TableWiget->setCellWidget(nRow,3,TmpCombox);
			 m_ConvectionCoefficient.insert(nRow,TmpCombox);


			 QPushButton* m_Parameter1 = new QPushButton();
			 m_Parameter1->setIcon(icon);
			 m_TableWiget->setCellWidget(nRow,4,m_Parameter1);
			 connect(m_Parameter1,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));

			 /*MyCombox *TmpCombox021=new MyCombox(this,nRow);
			 TmpCombox021->addItem("新建");
			 TmpCombox021->setFrame(false);
			 m_TableWiget->setCellWidget(nRow,5,TmpCombox021);
			 m_comboxPSetSurf2.insert(nRow,TmpCombox021);

			 MyCombox *TmpCombox11=new MyCombox(this,nRow);
			 TmpCombox11->addItem("无");
			 TmpCombox11->addItem("常数");
			 TmpCombox11->addItem("随时间变化");
			 TmpCombox11->setFrame(false);
			 m_TableWiget->setCellWidget(nRow,6,TmpCombox11);
			 m_AmbientTemperatureR.insert(nRow,TmpCombox11);


			 QPushButton* m_Parameter11 = new QPushButton();
			 m_Parameter11->setIcon(icon);
			 m_TableWiget->setCellWidget(nRow,7,m_Parameter11);
			 connect(m_Parameter11,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));*/


			 TmpCombox=new MyCombox(this,nRow);
			 TmpCombox->addItem("无");
			 TmpCombox->addItem("常数");
			 TmpCombox->addItem("随时间变化");
			 TmpCombox->setFrame(false);
			 m_TableWiget->setCellWidget(nRow,5,TmpCombox);
			 m_RadiationCoefficient.insert(nRow,TmpCombox);


			 QPushButton* m_Parameter2 = new QPushButton();
			 m_Parameter2->setIcon(icon);
			 m_TableWiget->setCellWidget(nRow,6,m_Parameter2);
			 connect(m_Parameter2,SIGNAL(clicked()),this,SLOT(ConstantChangesSlot()));

			 QString TBnn="TB"+QString::number(nRow,7);
			 QTableWidgetItem* tmpItem2=new QTableWidgetItem(TBnn);
			 m_TableWiget->setItem(nRow,7,tmpItem2);
			 m_TableWiget->item(nRow,7)->setFlags((Qt::ItemFlags)32);
			 //m_ThermalBoundaryName.append(TBnn);
		 }
	 }else if (nRowSize>nFSSysRowSize&&nRowSize>1){
		 for (int i=nFSSysRowSize;i<nRowSize;i++){
			 m_TableWiget->removeRow(i);
		 }
	 }

	 nRowSize=m_TableWiget->rowCount();
	 QStringList PSurfSetList;
	 PSurfSetList=ReadInpData.TmpNsetInps.strNSetName;
	 m_PSetList = ReadInpData.TmpSurfaceInps.strSurfaceName;
	 int i=0,j=0;
	 for (i=0;i<nRowSize;i++){
		 MyCombox *tmpCombox =(MyCombox*)m_TableWiget->cellWidget(i, 0);
		 //MyCombox *tmpCombox2 =(MyCombox*)m_TableWiget->cellWidget(i, 5);
		 tmpCombox->clear();
		 //tmpCombox2->clear();
		 //tmpCombox->addItem("新建"); 
		 //tmpCombox2->addItem("新建"); 
		 for (int j=0;j<ReadInpData.TmpSurfaceInps.strSurfaceName.size();j++){
			 if (ReadInpData.TmpSurfaceInps.strSurfaceName.at(j)!=""){
				 tmpCombox->addItem(ReadInpData.TmpSurfaceInps.strSurfaceName.at(j));
				 tmpCombox->setItemData(j,"2",(Qt::UserRole+3));
				 /*tmpCombox2->addItem(ReadInpData.TmpSurfaceInps.strSurfaceName.at(j));
				 tmpCombox2->setItemData(j,"2",(Qt::UserRole+3));*/
			 }
		 } 
		 m_comboxPSetSurf1.insert(i,tmpCombox);
		 connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comboxSlot(int,int)));	 
		/* m_comboxPSetSurf2.insert(i,tmpCombox2);
		 connect(tmpCombox2,SIGNAL(clicked(int,int)),this,SLOT(comboxSlot(int,int)));*/
	 }

	 SurfFRS0 FRS;
	 int iBaseNum=0;
	 for (int tt=0;tt<ReadInpData.TmpSurfaceInps.strSurfaceName.size();tt++){
		 int idataNum=ReadInpData.TmpSurfaceInps.strSurfaceDataNum.at(tt).toInt();
		 for (int j=iBaseNum;j<iBaseNum+idataNum;j++){
			 ReadInpData.TmpSurfaceInps.strData.at(j);
			 FRS.strName<<ReadInpData.TmpSurfaceInps.strSurfaceName.at(tt);
			 FRS.strNum<<ReadInpData.TmpSurfaceInps.strSurfaceDataNum.at(tt);
			 FRS.strInclude<<ReadInpData.TmpSurfaceInps.strData.at(j).split(",",QString::SkipEmptyParts).at(0);
			 FRS.strIncludeSurfNum<<ReadInpData.TmpSurfaceInps.strData.at(j).split(",",QString::SkipEmptyParts).at(1);
		 }
		 iBaseNum+=idataNum;
	 }
	 m_surfFRS=FRS;

	 //对流
	 int iKKRow=0;
	 for(int tt=0;tt<ReadInpData.TmpHPSystemFInps.size();tt++)
	 {
		 int istep=ReadInpData.TmpHPSystemFInps.at(tt).strStep.toInt()-1;
		 istep1=istep;
		 if (ReadInpData.TmpHPSystemFInps.at(tt).strStyle=="film" && ReadInpData.TmpHPSystemFInps.at(tt).strInclude=="1"){
                //--查找集合名称
                QString str=ReadInpData.TmpHPSystemFInps.at(tt).strElsetName;
                int id= FRS.strInclude.indexOf(str);
                QString strTmpName=FRS.strName.at(id);
                id=m_comboxPSetSurf1[iKKRow]->findText(strTmpName);
                if (id!=-1){
                    m_comboxPSetSurf1[iKKRow]->setCurrentIndex(id);
				}

                //--环境温度
                str=ReadInpData.TmpHPSystemFInps.at(tt).strAmplitudeName;//has name,f(t)->otherwise constant
                //if(istep!=ReadInpData.TmpHPSystemFInps.at(tt).strStep.toInt())istep=tt;
				if (str==""){//constant(strEnvirTemprat)
                    str="常数";
					int id=m_AmbientTemperatureC[istep]->findText(str);
					m_AmbientTemperatureC[istep]->setCurrentIndex(id);
					QTBConst *C_constShow1=new QTBConst(this);
					count01[0][istep]=1;
					FSystem FSData;
					FSData.MotionParameter=ReadInpData.TmpHPSystemFInps.at(tt).strEnvirTemprat;
					C_constShow1->SetInpDataC(FSData);
					IndexNumber01[2*istep]=m_ATCurveDataListDlgC.size();
					m_ATCurveDataListDlgC.append(C_constShow1);
				}else{ 
					str="随时间变化";
					int id=m_AmbientTemperatureC[istep]->findText(str);
					m_AmbientTemperatureC[istep]->setCurrentIndex(id);
					QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
					InpCurveInpS tmpCurData;
					QString ST1=ReadInpData.TmpHPSystemFInps.at(tt).strAmplitudeName;
					tmpCurData.strName=ST1;
					count01[1][istep]=2;
					IndexNumber01[2*istep+1]=m_ATCurveDataListDlgC02.size();
					int jj2=ReadInpData.TmpCurveInps.size();
					for(int I1=0;I1<jj2;I1++){
						QString st=ReadInpData.TmpCurveInps.at(I1).strName.simplified().toLower();
						if(st.toLower()==ST1.toLower()){
							tmpCurData.strDataX=ReadInpData.TmpCurveInps.at(I1).strDataX;
							tmpCurData.strDataY=ReadInpData.TmpCurveInps.at(I1).strDataY;
							tmpCurve->SetData(tmpCurData);
							m_ATCurveDataListDlgC02.append(tmpCurve);
						}	
					}	
				}
                //--对流系数
                str=ReadInpData.TmpHPSystemFInps.at(tt).strFilmRadiateAmpName;//has name,f(t)->otherwise constant  
                if (str==""){//constant(strfilmeAmp)
                    str="常数";
                    int id=m_ConvectionCoefficient[istep]->findText(str);
					m_ConvectionCoefficient[istep]->setCurrentIndex(id);
					QTBConst *C_constShow1=new QTBConst(this);
					count02[0][istep]=1;
					FSystem FSData;
					FSData.MotionParameter=ReadInpData.TmpHPSystemFInps.at(tt).strCofficient;
					C_constShow1->SetInpDataC(FSData);
					IndexNumber02[2*istep]=m_CCCurveDataListDlg.size();
					m_CCCurveDataListDlg.append(C_constShow1);
                }else{
					str="随时间变化";
					int id=m_ConvectionCoefficient[istep]->findText(str);
					m_ConvectionCoefficient[istep]->setCurrentIndex(id);
					QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
					InpCurveInpS tmpCurData;
					QString ST1=ReadInpData.TmpHPSystemFInps.at(tt).strFilmRadiateAmpName;
					tmpCurData.strName=ST1;
					count02[1][istep]=2;
					IndexNumber02[2*istep+1]=m_CCCurveDataListDlg02.size();
					int jj2=ReadInpData.TmpCurveInps.size();
					for(int I1=0;I1<jj2;I1++){
						QString st=ReadInpData.TmpCurveInps.at(I1).strName.simplified().toLower();
						if(st==ST1){
							tmpCurData.strDataX=ReadInpData.TmpCurveInps.at(I1).strDataX;
							tmpCurData.strDataY=ReadInpData.TmpCurveInps.at(I1).strDataY;
							tmpCurve->SetData(tmpCurData);
							m_CCCurveDataListDlg02.append(tmpCurve);
						}	
					}
				}
				//QString stLo1=ReadInpData.TmpHPSystemFInps.at(tt).strCommentaryName;
				QString stLo1="TB"+QString::number(ReadInpData.TmpHPSystemFInps.at(tt).strStep.toInt()-1,10);
				int stLo2=ReadInpData.TmpHPSystemFInps.at(tt).strStep.toInt();
				m_ThermalBoundaryName2<<stLo1;
				TB.insert(stLo2,stLo1);
				iKKRow++;

				//QString TBnn=ReadInpData.TmpHPSystemFInps.at(tt).strCommentaryName;
				QTableWidgetItem* tmpItem2=new QTableWidgetItem(stLo1);
				m_TableWiget->item(istep,7)->setFlags((Qt::ItemFlags)32);
				m_TableWiget->setItem(istep,7,tmpItem2);
		 }	
	 }

	 //--辐射
	 iKKRow=0;
	 for (int tt=0;tt<ReadInpData.TmpHPSystemRInps.size();tt++)
	 {
		 int istep=ReadInpData.TmpHPSystemRInps.at(tt).strStep.toInt()-1;
		 istep1=istep;
		 int nRow=m_TableWiget->rowCount();
		 if (ReadInpData.TmpHPSystemRInps.at(tt).strStyle=="radiate" && ReadInpData.TmpHPSystemRInps.at(tt).strInclude=="1"){
                QString str=ReadInpData.TmpHPSystemRInps.at(tt).strElsetName;
    //            int id= FRS.strInclude.indexOf(str);
    //            QString strTmpName=FRS.strName.at(id);
    //            id=m_comboxPSetSurf2[istep]->findText(strTmpName);
    //            if (id!=-1){
    //                m_comboxPSetSurf2[istep]->setCurrentIndex(id);
    //            }
    //            //--环境温度
    //            str=ReadInpData.TmpHPSystemRInps.at(tt).strAmplitudeName;//has name,f(t)->otherwise constant  
				////if(istep!=ReadInpData.TmpHPSystemRInps.at(tt).strStep.toInt())istep=tt;
				//if (str==""){//constant(strEnvirTemprat)
    //                str="常数";
				//	int id=m_AmbientTemperatureR[istep]->findText(str);
				//	m_AmbientTemperatureR[istep]->setCurrentIndex(id);
				//	QTBConst *C_constShow1=new QTBConst(this);
				//	count03[0][istep]=1;
				//	FSystem FSData;
				//	FSData.MotionParameter=ReadInpData.TmpHPSystemRInps.at(tt).strEnvirTemprat;
				//	C_constShow1->SetInpDataC(FSData);
				//	IndexNumber03[2*istep]=m_ATCurveDataListDlgR.size();
				//	m_ATCurveDataListDlgR.append(C_constShow1);
				//}else{ 
				//	str="随时间变化";
				//	int id=m_AmbientTemperatureR[istep]->findText(str);
				//	m_AmbientTemperatureR[istep]->setCurrentIndex(id);
				//	QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
				//	InpCurveInpS tmpCurData;
				//	QString ST1=ReadInpData.TmpHPSystemRInps.at(tt).strAmplitudeName;
				//	tmpCurData.strName=ST1;
				//	count03[1][istep]=2;
				//	IndexNumber03[2*istep+1]=m_ATCurveDataListDlgR02.size();
				//	int jj2=ReadInpData.TmpCurveInps.size();
				//	for(int I1=0;I1<jj2;I1++){
				//		QString st=ReadInpData.TmpCurveInps.at(I1).strName.simplified().toLower();
				//		if(st==ST1){
				//			tmpCurData.strDataX=ReadInpData.TmpCurveInps.at(I1).strDataX;
				//			tmpCurData.strDataY=ReadInpData.TmpCurveInps.at(I1).strDataY;
				//			tmpCurve->SetData(tmpCurData);
				//			m_ATCurveDataListDlgR02.append(tmpCurve);
				//		}	
				//	}
				//}
              // --辐射
                str=ReadInpData.TmpHPSystemRInps.at(tt).strFilmRadiateAmpName;//has name,f(t)->otherwise constant  
				if (str==""){//constant(strfilmeAmp)
                   str="常数";
				   int id=m_RadiationCoefficient[istep]->findText(str);
				   m_RadiationCoefficient[istep]->setCurrentIndex(id);
				   QTBConst *C_constShow1=new QTBConst(this);
				   count04[0][istep]=1;
				   FSystem FSData;
				   FSData.MotionParameter=ReadInpData.TmpHPSystemRInps.at(tt).strCofficient;
				   C_constShow1->SetInpDataC(FSData);
				   IndexNumber04[2*istep]=m_RCCurveDataListDlg.size();
				   m_RCCurveDataListDlg.append(C_constShow1);
                }else{
					str="随时间变化";
					int id=m_RadiationCoefficient[istep]->findText(str);
					m_RadiationCoefficient[istep]->setCurrentIndex(id);
					QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
					InpCurveInpS tmpCurData;
					QString ST1=ReadInpData.TmpHPSystemRInps.at(tt).strFilmRadiateAmpName;
					tmpCurData.strName=ST1;
					count04[1][istep]=2;
					IndexNumber04[2*istep+1]=m_RCCurveDataListDlg02.size();
					int jj2=ReadInpData.TmpCurveInps.size();
					for(int I1=0;I1<jj2;I1++){
						QString st=ReadInpData.TmpCurveInps.at(I1).strName.simplified().toLower();
						if(st==ST1){
							tmpCurData.strDataX=ReadInpData.TmpCurveInps.at(I1).strDataX;
							tmpCurData.strDataY=ReadInpData.TmpCurveInps.at(I1).strDataY;
							tmpCurve->SetData(tmpCurData);
							m_RCCurveDataListDlg02.append(tmpCurve);
						}
					}
				}
				//QString stLo1=ReadInpData.TmpHPSystemRInps.at(tt).strCommentaryName;
				QString stLo1="TB"+QString::number(ReadInpData.TmpHPSystemRInps.at(tt).strStep.toInt()-1,10);
				int stLo2=ReadInpData.TmpHPSystemRInps.at(tt).strStep.toInt();
				m_ThermalBoundaryName2<<stLo1;
				TB.insert(stLo2,stLo1);
				iKKRow++;

				//QString TBnn=ReadInpData.TmpHPSystemFInps.at(tt).strCommentaryName;
				QTableWidgetItem* tmpItem2=new QTableWidgetItem(stLo1);
				m_TableWiget->item(istep,7)->setFlags((Qt::ItemFlags)32);
				m_TableWiget->setItem(istep,7,tmpItem2);
		 } 
	 }
   
	 nRowSize=m_TableWiget->rowCount();
	 for(int i=0;i<nRowSize;i++){
		 QString name=((MyCombox*)m_TableWiget->cellWidget(i,0))->currentText();
		// QString name1=((MyCombox*)m_TableWiget->cellWidget(i,5))->currentText();
		 if(name=="新建"&&nRowSize!=1){
			 m_TableWiget->removeRow(i);
			 nRowSize--;
			 i=0;
		 }
	 }
	 m_ThermalBoundaryName2.removeDuplicates();
	 for(int i=0;i<m_ThermalBoundaryName2.size();i++){
		 m_ThermalBoundaryName.append(m_ThermalBoundaryName2.at(i));
	 }
	 WriteHBToInp();
	 emit emitHotName2(TB);
 }

 void ThermalBoundary::WriteHBToInp()
 {
	InpCurveInpS    nodeCurveList;
	int nRowSize=m_TableWiget->rowCount();
	HPSystemFInpS nodeFList;
	HPSystemRInpS nodeRList;
	int TabRowCnt = m_TableWiget->rowCount();

	if (TabRowCnt>0){
		m_OutPutTBFList.clear();
		m_OutPutTBRList.clear();
	}
	for (int i=0; i<TabRowCnt; i++){
		QStringList strInclude,strSurfaceNum;
		QStringList strIncludeR,strSurfaceNumR;
		MyCombox *ff = (MyCombox*)(m_TableWiget->cellWidget(i, 0));//对流表面集合
		//MyCombox *ff1 = (MyCombox*)(m_TableWiget->cellWidget(i, 5));//辐射表面集合
		QString str=ff->currentText();
		//QString str1=ff1->currentText();
		//热边界名称
		QString str2= m_TableWiget->item(i,7)->text();
		int id=m_surfFRS.strName.indexOf(str);
		//int id1=m_surfFRS.strName.indexOf(str1);//辐射
		//**********************************************************对流
		if (id !=-1){
			int ibaseNum=m_surfFRS.strNum.at(id).toInt();
			for (int kk=id;kk<id+ibaseNum;kk++){
				strInclude<<m_surfFRS.strInclude.at(kk); 
				QString str0=m_surfFRS.strIncludeSurfNum.at(kk);
				strSurfaceNum<<str0.remove("S",Qt::CaseInsensitive);
			} 
            nodeFList.strCommentaryName=str2;
			QString strTmpEnvirTemprat,strTmpAmplitudeName,StrType;
			//温度
			StrType=m_AmbientTemperatureC[i]->currentText();
			if(StrType=="常数"){
				int II=IndexNumber01[2*i];
				nodeFList.strAmplitudeName="";
				nodeFList.strEnvirTemprat=m_ATCurveDataListDlgC.at(II)->CMPOutPut;
			}else if(StrType=="随时间变化"){
				int II=IndexNumber01[2*i+1];
				nodeFList.strEnvirTemprat="1";
				nodeFList.strAmplitudeName=m_ATCurveDataListDlgC02.at(II)->m_CurveName->text();
			}
			//对流系数
			StrType=m_ConvectionCoefficient[i]->currentText();
			if(StrType=="常数"){
				int II=IndexNumber02[2*i];
				nodeFList.strCofficient=m_CCCurveDataListDlg.at(II)->CMPOutPut;
				nodeFList.strFilmRadiateAmpName="";
			}else if(StrType=="随时间变化"){
				int II=IndexNumber02[2*i+1];
				nodeFList.strCofficient="1";
				nodeFList.strFilmRadiateAmpName=m_CCCurveDataListDlg02.at(II)->m_CurveName->text();
			}
			nodeFList.strStyle="*film";
			nodeFList.strElsetName=strInclude.join(",");
			nodeFList.strSurfaceNum=strSurfaceNum.join(",");
			m_OutPutTBFList.append(nodeFList);
		} 
		//************************************************************辐射
		MyCombox *ff1 = (MyCombox*)(m_TableWiget->cellWidget(i, 5));//对流表面集合
		str=ff1->currentText();
		//if (id11 !=-1){
		if (str !="无"&&id!=-1){
			int ibaseNum=m_surfFRS.strNum.at(id).toInt();
			for (int kk=id;kk<id+ibaseNum;kk++){
				strIncludeR<<m_surfFRS.strInclude.at(kk); 
				QString str0=m_surfFRS.strIncludeSurfNum.at(kk);
				strSurfaceNumR<<str0.remove("S",Qt::CaseInsensitive);
			} 
			QString strTmpEnvirTemprat,strTmpAmplitudeName,StrType;
			nodeRList.strCommentaryName=str2;
			//温度
			StrType=m_AmbientTemperatureC[i]->currentText();
			if(StrType=="常数"){
				int II=IndexNumber01[2*i];
				nodeRList.strAmplitudeName="";
				nodeRList.strEnvirTemprat=m_ATCurveDataListDlgC.at(II)->CMPOutPut; 
			}else if(StrType=="随时间变化"){
				int II=IndexNumber01[2*i+1];
				nodeRList.strEnvirTemprat="1";
				nodeRList.strAmplitudeName=m_ATCurveDataListDlgC02.at(II)->m_CurveName->text();	
			}
			//辐射系数
			StrType=m_RadiationCoefficient[i]->currentText();
			if(StrType=="常数"){
				int II=IndexNumber04[2*i];
				nodeRList.strCofficient=m_RCCurveDataListDlg.at(II)->CMPOutPut;
				nodeRList.strFilmRadiateAmpName="";
			}else if(StrType=="随时间变化"){
				int II=IndexNumber04[2*i+1];
				nodeRList.strCofficient="1";
				nodeRList.strFilmRadiateAmpName=m_RCCurveDataListDlg02.at(II)->m_CurveName->text();
			}
			nodeRList.strStyle="*radiate";
			nodeRList.strElsetName=strIncludeR.join(",");
			nodeRList.strSurfaceNum=strSurfaceNumR.join(",");
			m_OutPutTBRList.append(nodeRList);
		}
	}

	m_CurveList.clear();
	int DX=m_ATCurveDataListDlgC02.size();
	for(int II=0;II<DX;II++){
		nodeCurveList=m_ATCurveDataListDlgC02.at(II)->m_HIPCurveNode;
		m_CurveList.append(nodeCurveList);
	}
	//DX=m_ATCurveDataListDlgR02.size();
	//for(int II=0;II<DX;II++){
	//	nodeCurveList=m_ATCurveDataListDlgR02.at(II)->m_HIPCurveNode;
	//	m_CurveList.append(nodeCurveList);
	//}

	DX=m_CCCurveDataListDlg02.size();//对流系数随时间变化
	for(int II=0;II<DX;II++){
		nodeCurveList=m_CCCurveDataListDlg02.at(II)->m_HIPCurveNode;
		m_CurveList.append(nodeCurveList);
	}

	DX=m_RCCurveDataListDlg02.size();//辐射系数随时间变化
	for(int II=0;II<DX;II++){
		nodeCurveList=m_RCCurveDataListDlg02.at(II)->m_HIPCurveNode;
		m_CurveList.append(nodeCurveList);
	}
}

 void ThermalBoundary::ConstantChangesSlot()
 {
	 int Row,index,x,y,Column,sign;
	 QString strIdName;
	 QPushButton *pushBtn = dynamic_cast<QPushButton*>(this->sender());
	 x=pushBtn->frameGeometry().x();
	 y=pushBtn->frameGeometry().y();
	 QModelIndex index02=m_TableWiget->indexAt(QPoint(x,y));
	 Row=index02.row();
	 Column=index02.column();
	 if (Row <0||Column<0){return;}
	 if(Column==2){
		 strIdName=m_AmbientTemperatureC[Row]->currentText();
	 }else if(Column==4){
		 strIdName=m_ConvectionCoefficient[Row]->currentText();
	 /*}else if(Column==7){
		 strIdName=m_AmbientTemperatureR[Row]->currentText();*/
	 }else if(Column==6){
		 strIdName=m_RadiationCoefficient[Row]->currentText();
	 }else{
		 return;
	 }
	 if(strIdName=="常数"){
		 if(Column==2){
			 sign=count01[0][Row];
		 }else if(Column==4){
			 sign=count02[0][Row];
		/* }else if(Column==7){
			 sign=count03[0][Row];*/
		 }else if(Column==6){
			 sign=count04[0][Row];
		 }
		 if(sign!=1){
			 QTBConst *FSConstShow=new QTBConst(this);
			 FSConstShow->show(); 
			 if(Column==2){
				 count01[0][Row]=1;
				 m_ATCurveDataListDlgC.append(FSConstShow);
				 IndexNumber01[2*Row]=m_ATCurveDataListDlgC.size()-1;
			 }else if(Column==4){
				 count02[0][Row]=1;
				 m_CCCurveDataListDlg.append(FSConstShow);
				 IndexNumber02[2*Row]=m_CCCurveDataListDlg.size()-1;
			/* }else if(Column==7){
				 count03[0][Row]=1;
				 m_ATCurveDataListDlgR.append(FSConstShow);
				 IndexNumber03[2*Row]=m_ATCurveDataListDlgR.size()-1;*/
			 }else if(Column==6){
				 count04[0][Row]=1;
				 m_RCCurveDataListDlg.append(FSConstShow);
				 IndexNumber04[2*Row]=m_RCCurveDataListDlg.size()-1;
			 }
		 }else{
			 int IN,IN1;
			 IN1=2*Row;
			 if(Column==2){
				 IN=IndexNumber01[IN1];
				 m_ATCurveDataListDlgC.at(IN)->exec();
			 }else if(Column==4){
				 IN=IndexNumber02[IN1];
				 m_CCCurveDataListDlg.at(IN)->exec();
			/* }else if(Column==7){
				 IN=IndexNumber03[IN1];
				 m_ATCurveDataListDlgR.at(IN)->exec();*/
			 }else if(Column==6){
				 IN=IndexNumber04[IN1];
				 m_RCCurveDataListDlg.at(IN)->exec();
			 }
		 }
	 }else if(strIdName=="随时间变化"){
		 if(Column==2){
			 sign=count01[1][Row];
		 }else if(Column==4){
			 sign=count02[1][Row];
		/* }else if(Column==7){
			 sign=count03[1][Row];*/
		 }else if(Column==6){
			 sign=count04[1][Row];
		 }
		 if(sign!=2){
			 QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
			 QString strName;
			 int ret = tmpCurve->exec();   
			 if (ret == QDialog::Accepted){//ok
				 strName=tmpCurve->m_HIPCurveNode.strName;
				 if (strName == ""){return;}
			 }
			 if(Column==2){
				m_ATCurveDataListDlgC02.append(tmpCurve);
				count01[1][Row]=2;
				IndexNumber01[2*Row+1]=m_ATCurveDataListDlgC02.size()-1;
			 }else if(Column==4){
				 m_CCCurveDataListDlg02.append(tmpCurve);/////////////****
				 count02[1][Row]=2;
				 IndexNumber02[2*Row+1]=m_CCCurveDataListDlg02.size()-1;
			/* }else if(Column==7){
				 m_ATCurveDataListDlgR02.append(tmpCurve);
				 count03[1][Row]=2;
				 IndexNumber03[2*Row+1]=m_ATCurveDataListDlgR02.size()-1;*/
			 }else if(Column==6){
				 m_RCCurveDataListDlg02.append(tmpCurve);
				 count04[1][Row]=2;
				 IndexNumber03[2*Row+1]=m_RCCurveDataListDlg02.size()-1;
			 }  
		 }else{
			 int IN,IN1;
			 IN1=2*Row+1;

			 if(Column==2){
				 IN=IndexNumber01[IN1];
				 m_ATCurveDataListDlgC02.at(IN)->exec();
			 }else if(Column==4){
				 IN=IndexNumber02[IN1];
				 m_CCCurveDataListDlg02.at(IN)->exec();
			/* }else if(Column==7){
				 IN=IndexNumber03[IN1];
				 m_ATCurveDataListDlgR02.at(IN)->exec();*/
			 }else if(Column==6){
				 IN=IndexNumber04[IN1];
				 m_RCCurveDataListDlg02.at(IN)->exec();
			 }  
		 }	   
	 }else{
		 return;
	 }
 }

 void ThermalBoundary::comboxSlot(int comWigId,int index0)
{  
   QString str,str0,str1;
   //int iTmp;
   MyCombox *btn=(MyCombox*)sender();
   int x=btn->mapToParent(QPoint(0,0)).x();//获取鼠标点击的x坐标
   int y=btn->mapToParent(QPoint(0,0)).y();//获取鼠标点击的y坐标
   QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));
   int col=index.column();//获取鼠标点击中表格列序号
   if (index0<0){return;}

  str =((MyCombox*)m_TableWiget->cellWidget(comWigId, col))->currentText();
    
  if (str==""||str=="新建")return;
  //--设置属性
 
  NElSurfChsS tmpChs;
  tmpChs.strName =str;
  tmpChs.iType =COM_ENUM_SURFSET;
  emit emitPsetHBHighLight(tmpChs);
}

 void ThermalBoundary::PreHBBtnSlot()
 {
	WriteHBToInp();
	this->accept();
 }

 void ThermalBoundary::NextHBBtnSlot()
 {
	WriteHBToInp();
	m_ThermalBoundaryName.clear();
	int TabRowCnt = m_TableWiget->rowCount();
	for(int i=0;i<TabRowCnt;i++){
		QString str =m_TableWiget->item(i,7)->text();
		m_ThermalBoundaryName.append(str);
	}
	m_TBName HN;
	m_ThermalBoundaryName;
	int it=m_ThermalBoundaryName.size();
	for(int i=0;i<it;i++){
		for(int j=i+1;j<it;j++){
			if(m_ThermalBoundaryName.at(i)==m_ThermalBoundaryName.at(j)){
				m_ThermalBoundaryName.remove(j);
				it--;
				i=0;
				j=0;
			}
		}
	}

    HN.m_ThermalBoundaryName2=m_ThermalBoundaryName;
	emit emitHotName(HN);
	this->accept();
 }

 void ThermalBoundary::AppendSurfSetComboxTBSlot(QString str)
 {
	 int nRow = m_TableWiget->rowCount();
	 for (int i=0; i<nRow; i++)
	 {
		 MyCombox *TmpcomboxPset0 = (MyCombox*)m_TableWiget->cellWidget(i, 0);
		 MyCombox *TmpcomboxPset1 = (MyCombox*)m_TableWiget->cellWidget(i, 5);//new MyCombox(this, nRow);
		 if (TmpcomboxPset0->findText(str) == -1&&TmpcomboxPset1->findText(str) == -1)
		 {
			 TmpcomboxPset0->addItem(str);
			 TmpcomboxPset1->addItem(str);
		 }
	 }
 }

 