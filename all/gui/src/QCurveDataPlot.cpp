#include "QCurveDataPlot.h"
//*****************************************************
//*****************************************************
//--QCurveDataPlot //曲线显示
//*****************************************************
//*****************************************************
QCurveDataPlot::QCurveDataPlot(QWidget *parent)
: QDialog(parent)
{
    setWindowTitle("新建");
    QWidget *Hwid1 = new QWidget(this);
    Hwid1->setMaximumHeight(300);//去掉黄色LOGO
    QWidget *HwidBtn = new QWidget(this);
    QWidget *HwidBtn1 = new QWidget(this);
    QWidget *HwidBtnName = new QWidget(this);
    QHBoxLayout *Hlayout = new QHBoxLayout(HwidBtn);
    m_CancelBtn = new QPushButton(tr("取消"),HwidBtn);
    m_OKBtn = new QPushButton(tr("应用"),HwidBtn);

	m_ImportBtn = new QPushButton(tr("导入"),HwidBtn);
    m_ExportBtn = new QPushButton(tr("导出"),HwidBtn);

    Hlayout->addStretch();
	Hlayout->addWidget(m_OKBtn);
    Hlayout->addWidget(m_CancelBtn);
	Hlayout->addWidget(m_ImportBtn);
    Hlayout->addWidget(m_ExportBtn);
    

    QHBoxLayout *Hlayout1 = new QHBoxLayout(HwidBtnName);
    QLabel *lab = new QLabel(tr("曲线名称"),HwidBtnName);
    m_CurveName = new QLineEdit(HwidBtnName); 
    Hlayout1->addWidget(lab);
    Hlayout1->addWidget(m_CurveName);

    QHBoxLayout *Hlayout3= new QHBoxLayout(HwidBtn1);
    m_TabAddBtn = new QPushButton(tr("增加"),HwidBtn1);
    m_TabDelBtn = new QPushButton(tr("删除"),HwidBtn1);
    m_TabInsertBtn = new QPushButton(tr("插入"),HwidBtn1);
    m_TabPlotBtn = new QPushButton(tr("绘图"),HwidBtn1);
    Hlayout3->addWidget(m_TabAddBtn);
    Hlayout3->addWidget(m_TabDelBtn);
    Hlayout3->addWidget(m_TabInsertBtn);
    Hlayout3->addWidget(m_TabPlotBtn);
    Hlayout3->addStretch();

    QHBoxLayout *Hlayout2 = new QHBoxLayout(Hwid1);

    m_TabCurveWiget = new QTableWidget(Hwid1);
    m_TabCurveWiget->setColumnCount(2);
    m_TabCurveWiget->setRowCount(1);
    m_TabCurveWiget->verticalHeaderItem(1);
    m_TabCurveWiget->setItem(0,0,new QTableWidgetItem());
    m_TabCurveWiget->setItem(0,1,new QTableWidgetItem());
    m_TabCurveWiget->setMinimumWidth(300);
    m_TabCurveWiget->setMinimumHeight(100);

    QStringList strList;
    strList<<"x"<<"y";
    m_TabCurveWiget->setHorizontalHeaderLabels(strList);
    m_TabCurveWiget->setFocusPolicy(Qt::NoFocus);
    //--menu
    m_TabCurveWiget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_popMenu=new QMenu;
    Hlayout2->addWidget(m_TabCurveWiget);
    chartViewers = new QChartViewer(Hwid1); 
    Hlayout2->addWidget(chartViewers);
 
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addWidget(HwidBtnName);
    MainLayout->addWidget(HwidBtn1);
    MainLayout->addWidget(Hwid1);
    MainLayout->addWidget(HwidBtn);
    //MainLayout->addLayout(Hlayout1);
    //MainLayout->addLayout(Hlayout3);
    //MainLayout->addLayout(Hlayout2);
    //MainLayout->addLayout(Hlayout);
    setLayout(MainLayout);
    Plot();
    m_popMenu->addAction(tr("Copy"),this,SLOT(TableCopyBtnSlot()));//(ExpandAllSlot()));
    m_popMenu->addAction(tr("Paste"),this,SLOT(TablePasteBtnSlot()));//(CollapseAllSlot()));
    m_popMenu->addAction(tr("Clear"),this,SLOT(TableClearRBtnSlot()));

    connect(m_TabCurveWiget, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextTableMenuSlot(const QPoint &)));
    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(CurveOkBtnSlot()));
	connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(CurveCanleBtnSlot()));
    connect(m_TabPlotBtn,SIGNAL(clicked()),this,SLOT(CurveApplyBtnSlot()));
    connect(m_TabAddBtn,SIGNAL(clicked()),this,SLOT(CurveAddBtnSlot()));
	connect(m_TabCurveWiget,SIGNAL(returnPressed(const QPoint &)),this,SLOT(CurveAddBtnSlot()));//
    connect(m_TabInsertBtn,SIGNAL(clicked()),this,SLOT(CurveInsertBtnSlot()));
    connect(m_TabDelBtn,SIGNAL(clicked()),this,SLOT(CurveDelBtnSlot()));

	connect(m_ImportBtn,SIGNAL(clicked()),this,SLOT(ImportSlot()));
	connect(m_ExportBtn,SIGNAL(clicked()),this,SLOT(ExportSlot()));
}

QCurveDataPlot::~QCurveDataPlot()
{

}
//应用，将参数写入Inp
void QCurveDataPlot::CurveApplyBtnSlot()
{
	writeToCurveInpData();
      //this->accept();
}
void QCurveDataPlot::writeToCurveInpData()
{
    m_HIPCurveNode.strCommentaryName = "曲线定义";
    m_HIPCurveNode.strCurveStyle = "Amplitude";
    m_HIPCurveNode.strName = m_CurveName->text();
    m_HIPCurveNode.strTime = "TOTAL TIME";
    m_HIPCurveNode.ThirdStrTime = 1;//默认都有时间

    int nRow=m_TabCurveWiget->rowCount();
    if (nRow>0){
        m_HIPCurveNode.strDataX.clear();
        m_HIPCurveNode.strDataY.clear();
    }
    for (int i=0;i<nRow;i++){
        m_HIPCurveNode.strDataX<<m_TabCurveWiget->item(i,0)->text();
        m_HIPCurveNode.strDataY<<m_TabCurveWiget->item(i,1)->text();
    }

    Plot();
}
void QCurveDataPlot::CurveOkBtnSlot()
{
	writeToCurveInpData();
    this->accept();//保存并退出
}
void QCurveDataPlot::CurveCanleBtnSlot()
{
    this->close();//退出
}
//--tab增加行
void QCurveDataPlot::CurveAddBtnSlot()
{
    int nRow = m_TabCurveWiget->rowCount();
    if (nRow >= 0){
        m_TabCurveWiget->insertRow(nRow);
        for(int j=0;j<m_TabCurveWiget->columnCount();j++){
            m_TabCurveWiget->setItem(nRow,j,new QTableWidgetItem());
        }
    }
}
//--tab插入行(当前行的上一行增加)
void QCurveDataPlot::CurveInsertBtnSlot()
{
    int curRow = m_TabCurveWiget->currentRow();
    if (curRow<0)curRow=0;
    m_TabCurveWiget->insertRow(curRow);
    for(int j=0;j<m_TabCurveWiget->columnCount();j++){
        m_TabCurveWiget->setItem(curRow,j,new QTableWidgetItem(tr("")));
    }
}
//--tab删除行
void QCurveDataPlot::CurveDelBtnSlot()
{
    int curRow = m_TabCurveWiget->currentRow();

    if (curRow!=-1){
        m_TabCurveWiget->removeRow(curRow);
    }
}
//--从读取的Inp文件中写入数据到界面
void QCurveDataPlot::SetData(InpCurveInpS HIPCurveNode)
{
    m_CurveName->setText(HIPCurveNode.strName);
    int nRowSize = HIPCurveNode.strDataX.size();
    m_TabCurveWiget->setRowCount(nRowSize);
    for (int i=0; i<nRowSize; i++){
        QTableWidgetItem *itemX = new QTableWidgetItem(HIPCurveNode.strDataX.at(i));
        QTableWidgetItem *itemY = new QTableWidgetItem(HIPCurveNode.strDataY.at(i));
        m_TabCurveWiget->setItem(i,0,itemX);
        m_TabCurveWiget->setItem(i,1,itemY);
    }
    writeToCurveInpData();
}

//-绘图
void QCurveDataPlot::Plot()
{
    int nRow=m_TabCurveWiget->rowCount();
    int nCol=m_TabCurveWiget->columnCount();
    int num=nRow;
    QString *dataTmpX = new QString[num];
    double *dataTmpY = new double[num];   
    for (int i=0; i<nRow; i++){
        dataTmpX[i] = m_TabCurveWiget->item(i,0)->text();
        dataTmpY[i] = m_TabCurveWiget->item(i,1)->text().toDouble();
    }
    const char **tmp = new const char*[num];
    QByteArray *b = new QByteArray[num]; 
    const char *imageMap = 0;
    for (int tt=0; tt < num; tt++){   
        b[tt] = dataTmpX[tt].toLatin1().data();
        tmp[tt] = b[tt]; 
    }
    StringArray dataX = StringArray(tmp, num);
    vector<DoubleArray> dataY ;
    dataY.push_back(DoubleArray(dataTmpY, num));
    ChartShow2XY(dataX,dataY,imageMap);
    dataTmpX = NULL;dataTmpY = NULL;b=NULL;tmp=NULL;
    delete(dataTmpY);delete(dataTmpX);delete(tmp);delete(b);
}
void QCurveDataPlot::ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap)
{
    // Each demo module can display a number of charts. We display all of them. 
    MaterialChart *d = materialCharts+0;//demoCharts + 2;//demoIndex;
    for (int tt=0;tt<d->noOfCharts;tt++)
    {
        imageMap = 0;
        chartViewers->setChart(d->createChart(dataX, dataY, &imageMap));
        chartViewers->setImageMap(imageMap);
        delete chartViewers->getChart();
    }
   
    // We hide unused chart viewers.
    //for (int kk = 0; kk < noOfChartViewers; ++kk){
        chartViewers->setVisible(d->noOfCharts);
    //}   
    //flowLayoutCharts();
}
//--flow layout
void QCurveDataPlot::flowLayoutCharts()
{

    const int margin = 5;

    // initial cursor position
    int cursorX = margin;
    int cursorY = margin;

    // initial page size
    int pageWidth = 0;
    int pageHeight = 0;
    int wrapWidth =chartViewers->parentWidget()->parentWidget()->width();
    // the width limit, beyond which wrapping will occur
    int chartWidth =  chartViewers->width();
    int chartHeight = chartViewers->height();

    if (cursorX + margin + chartWidth > wrapWidth)
    {
        // no enough position in current line, so move to next line
        cursorX = margin;
        cursorY = pageHeight + margin;
    }

    // put chart in current line
    chartViewers->setGeometry(cursorX, cursorY, chartWidth, chartHeight);

    // advance cursor
    cursorX += chartWidth + margin;

    if (pageWidth < cursorX)
        pageWidth = cursorX;
    if (pageHeight < cursorY + chartHeight)
        pageHeight = cursorY + chartHeight;

    // resize the container to the proper size
    chartViewers->parentWidget()->setFixedSize(pageWidth, pageHeight);
}
void QCurveDataPlot::contextTableMenuSlot(const QPoint &p)
{
    m_popMenu->exec(QCursor::pos());
}
void QCurveDataPlot::TableCopyBtnSlot()
{
    QList<QTableWidgetSelectionRange> ranges=m_TabCurveWiget->selectedRanges();
    int nCount=ranges.count();
    if (nCount<=0)return;

    QString strSelectText;
    for (int i=0;i<nCount;i++)
    {
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();

        for (int j=topRow;j<=bottomRow;j++)
        {
            for (int kk=leftColumn;kk<=rightColumn;kk++){
                strSelectText+=m_TabCurveWiget->item(j,kk)->text();
                if (kk<rightColumn){
                    strSelectText+="\t";
                }      
            }
            if (j<bottomRow){ 
                strSelectText+="\n";
            }
        }
    }
    QClipboard *clipboard= QApplication::clipboard();
    clipboard->setText(strSelectText);
}
void QCurveDataPlot::TablePasteBtnSlot()
{
    QClipboard *clipBoard= QApplication::clipboard();
    QString strPasterText=clipBoard->text(QClipboard::Clipboard);
    QStringList strRowList=strPasterText.split("\n",QString::SkipEmptyParts);
    int nPasteRow=strRowList.count();//需要粘贴的行数
    int nPasteCol=0;
    for (int i=0;i<nPasteRow;i++)
    {
        QStringList strColumnList=strRowList.at(i).split("\t",QString::SkipEmptyParts);
        nPasteCol=strColumnList.count();
    }

    QList<QTableWidgetSelectionRange> ranges=m_TabCurveWiget->selectedRanges();
    int nCount=ranges.count();
    if (nCount<=0)return;

    for (int i=0;i<nCount;i++)
    {
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();
        int nSizeCol=abs(rightColumn-leftColumn)+1;
        int nSizeRow=abs(bottomRow-topRow)+1;//所选片段的大小

        //if (nSizeCol !=nPasteCol ||nSizeRow!=nPasteRow)
        //{
        //    return ;//
        //}
        if (nSizeRow >= nPasteRow){
            for (int j=topRow,tt=0;j<=bottomRow;j++,tt++){
                if (tt>=nPasteRow)continue;
                QStringList strColumnList=strRowList.at(tt).split("\t",QString::SkipEmptyParts);
                for (int kk=leftColumn,mm=0;kk<=rightColumn;kk++,mm++){
                    if (mm>=nPasteCol)continue;
                    m_TabCurveWiget->item(j,kk)->setData(Qt::DisplayRole,strColumnList.at(mm).toDouble());    
                } 
            }
        }else{//增加table行数，进行数值填充
            int nRow = m_TabCurveWiget->rowCount();
            int iInSertRow=nPasteRow+topRow-nRow;//需要进行增行数
            for (int i = nRow,tt=0; i<nRow+iInSertRow; i++,tt++){//增加Table行数
                m_TabCurveWiget->insertRow(i);
                for(int j=0;j<=m_TabCurveWiget->columnCount();j++){
                    QTableWidgetItem *itemX =new QTableWidgetItem();  
                    QVariant data;
                    itemX->setData(Qt::DisplayRole, data.toDouble());
                    m_TabCurveWiget->setItem(i,j,itemX);
                }              
            }
            //进行赋值及填充
            for (int j=topRow,tt=0;j<=topRow+nPasteRow;j++,tt++){  
                if (tt>=nPasteRow)continue;
                QStringList strColumnList=strRowList.at(tt).split("\t",QString::SkipEmptyParts);
                for (int kk=leftColumn,mm=0;kk<=rightColumn;kk++,mm++){
                    if (mm>=nPasteCol) continue;
                    m_TabCurveWiget->item(j,kk)->setData(Qt::DisplayRole,strColumnList.at(mm).toDouble());    
                } 
            }
        }
    }
}
void QCurveDataPlot::TableClearRBtnSlot()//clear all data
{
    //m_TableData->clearContents();
    QList<QTableWidgetSelectionRange> ranges=m_TabCurveWiget->selectedRanges();
    int nCount=ranges.count();
    if (nCount<=0)return;


    for (int i=0;i<nCount;i++){
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();

        for (int j=topRow;j<=bottomRow;j++){
            for (int kk=leftColumn;kk<=rightColumn;kk++){
                m_TabCurveWiget->item(j,kk)->setText("");
            }    
        }
    }
}

void QCurveDataPlot::ImportSlot()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Read file...",  QDir::currentPath(), "txt Files (*.txt)");
	if (fileName.size()==0){
		return;
	}
	QFile file(fileName);
	if(!file.exists()){
		return ;
	}
	if (!file.open(QIODevice::ReadOnly)){
		return ;
	}
	QTextStream inText(&file);
	QString data;
	int nRow;
	int Ii=0;
	while(!inText.atEnd()){
		data=inText.readLine();
		data.toLower();
		if(data.contains("name",Qt::CaseInsensitive)){
			QStringList dat=data.split(":");
			m_CurveName->setText(dat.at(1));
			break;
		}
	}
	while(!inText.atEnd()){
		nRow = m_TabCurveWiget->rowCount();
		data=inText.readLine();
		data.toLower();
		if(nRow>Ii){
			m_TabCurveWiget->insertRow(nRow);
		}
		Ii++;
		QStringList dat=data.split(",");
		QTableWidgetItem *itemX = new QTableWidgetItem(dat.at(0));
		QTableWidgetItem *itemY = new QTableWidgetItem(dat.at(1));
		m_TabCurveWiget->setItem(nRow,0,itemX);
		m_TabCurveWiget->setItem(nRow,1,itemY);
	}
	file.close();
}

void QCurveDataPlot::ExportSlot()
{
    int nRow=m_TabCurveWiget->rowCount();
	QString dataX,dataY,data;
	if(nRow<=0)return;
	QString fileName = QFileDialog::getSaveFileName(this,
        "Choose a file name", QDir::currentPath(),
        ("txt Files (*.txt)"));
    if (fileName.isEmpty())return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))return;
	data="曲线名称(Name):"+m_CurveName->text()+"\n";
	file.write(data.toAscii());
    for (int i=0;i<nRow;i++){
        dataX=m_TabCurveWiget->item(i,0)->text();
        dataY=m_TabCurveWiget->item(i,1)->text();
		if(dataX==""&&dataY=="")continue;
		data=dataX+","+dataY+"\n";
		file.write(data.toAscii());
    }
	file.close();
}