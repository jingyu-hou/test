#include "QMaterDataWig.h"
using namespace std;

QMaterDataWig::QMaterDataWig(QWidget *parent)
	: QWidget(parent)
{
    m_Widge = new QWidget(this);
    m_Gridlayout1 = new QGridLayout(m_Widge);
    
    for (int i=0;i<TYPENUM;i++)
    {
        m_TableWidget[i] = new QTableWidget(m_Widge);
        m_TableWidget[i]->setVisible(false);
        if (i==M_ENUM_METALPOWDER){
            m_tabWidMetalPowder = new QTabWidget(m_Widge);
            QPushButton *btn= new QPushButton("test1");
            m_widMetalPowderParam = new QTableWidget(m_Widge);
            m_tabWidMetalPowder->addTab(m_widMetalPowderParam,QIcon(":/images/NewIcon.png"),tr("粉末特性"));
            m_tabWidMetalPowder->addTab(m_TableWidget[i],QIcon(":/images/NewIcon.png"),tr("粉末硬化"));
            m_tabWidMetalPowder->setCurrentIndex(0);
            m_tabWidMetalPowder->setVisible(false);
        }
        if (i==M_ENUM_EXPANS){//    
            m_EditLabExpandZero.edit=new QLineEdit(tr("10"),m_Widge);
            m_EditLabExpandZero.label=new QLabel(tr("参数设置"),m_Widge);
            m_EditLabExpandZero.edit->setEnabled(false);
        }
		if (i==M_ENUM_Rate_Dependent_Plastic){// luo   
			m_RDPModetype.edit=new QLineEdit(tr("HansenSpittel"),m_Widge);
			m_RDPModetype.label=new QLabel(tr("类型"),m_Widge);
			m_RDPModetype.edit->setEnabled(false);
		}
		if (i==M_ENUM_Damage){// luo   
            m_DamageModetype.edit=new QLineEdit(tr("Normalized C&L"),m_Widge);
            m_DamageModetype.label=new QLabel(tr("类型"),m_Widge);
            m_DamageModetype.edit->setEnabled(false);
        }
	}
   
    PageIndex = 0;

    //--chart 图表
    m_Widge1 = new QWidget(this);
    chartViewers[0] = new QChartViewer(m_Widge1);
    chartViewers[0]->hide();
    chartViewers[1] = new QChartViewer(m_Widge1);
    chartViewers[1]->hide();
    chartViewers[2] = new QChartViewer(m_Widge1);
    chartViewers[2]->hide();
    chartViewers[3] = new QChartViewer(m_Widge1);
    chartViewers[3]->hide();
    chartViewers[4] = new QChartViewer(m_Widge1);
    chartViewers[4]->hide();
    chartViewers[5] = new QChartViewer(m_Widge1);
    chartViewers[5]->hide();
    chartViewers[6] = new QChartViewer(m_Widge1);
    chartViewers[6]->hide();
    chartViewers[7] = new QChartViewer(m_Widge1);
    chartViewers[7]->hide();
    chartViewers[8] = new QChartViewer(m_Widge1);
    chartViewers[8]->hide();
    chartViewers[9] = new QChartViewer(m_Widge1);
    chartViewers[9]->hide();

    m_mainLayout = new QHBoxLayout(this);

  //malloc(tmpMcmp.data,&cpmt,sizeof(MdataList));
 }

QMaterDataWig::~QMaterDataWig()
{
	dataTmpY = NULL;
    delete dataTmpY;
    dataTmpX = NULL;
    delete dataTmpX;
}
/*
function:ChgWidget
describ: Load the page and show or hide
*/
void QMaterDataWig::ChgWidgetSlot(int ShowIndex, QString strName,QString strFileName)
{
    PageIndex = ShowIndex; 

    for (int i=0;i<TYPENUM;i++){
        m_TableWidget[i]->setVisible(false);
        if (PageIndex==i){
            m_TableWidget[PageIndex]->setVisible(true); 
            m_EditLabExpandZero.edit->setVisible(false);
            m_EditLabExpandZero.label->setVisible(false);
			m_RDPModetype.edit->setVisible(false);
			m_RDPModetype.label->setVisible(false);
			m_DamageModetype.edit->setVisible(false);
			m_DamageModetype.label->setVisible(false);
			m_tabWidMetalPowder->setVisible(false);
            if (PageIndex==M_ENUM_METALPOWDER){
                m_tabWidMetalPowder->setVisible(true);
            }else if (PageIndex == M_ENUM_METALPOWDER){
                m_tabWidMetalPowder->setVisible(true);
            }else if (PageIndex == M_ENUM_EXPANS){
                m_EditLabExpandZero.edit->setVisible(true);
                m_EditLabExpandZero.label->setVisible(true);
            }else if (PageIndex == M_ENUM_Damage){
                m_DamageModetype.edit->setVisible(true);
                m_DamageModetype.label->setVisible(true);
            }else if (PageIndex == M_ENUM_Rate_Dependent_Plastic){
                m_RDPModetype.edit->setVisible(true);
                m_RDPModetype.label->setVisible(true);
            }
        }
    }
    m_DataList[PageIndex].strName = strName;
    setTabWidgetHeader(PageIndex);
    if (PageIndex == M_ENUM_METALPOWDER){
        m_Gridlayout1->addWidget(m_tabWidMetalPowder,2,0,1,3);
    }else if(PageIndex == M_ENUM_EXPANS){
        m_Gridlayout1->addWidget(m_EditLabExpandZero.label,0,0,1,1);//温度选择
        m_Gridlayout1->addWidget(m_EditLabExpandZero.edit,0,1,1,1);
        m_Gridlayout1->addWidget(m_TableWidget[PageIndex],2,0,1,3);
	}else if(PageIndex == M_ENUM_Rate_Dependent_Plastic){//luo
		m_Gridlayout1->addWidget(m_RDPModetype.label,0,0,1,1);
        m_Gridlayout1->addWidget(m_RDPModetype.edit,0,1,1,1);
        m_Gridlayout1->addWidget(m_TableWidget[PageIndex],2,0,1,3);
	}else if(PageIndex == M_ENUM_Damage){//luo
		m_Gridlayout1->addWidget(m_DamageModetype.label,0,0,1,1);
		m_Gridlayout1->addWidget(m_DamageModetype.edit,0,1,1,1);
		m_Gridlayout1->addWidget(m_TableWidget[PageIndex],2,0,1,3);
	}else{
		m_Gridlayout1->addWidget(m_TableWidget[PageIndex],2,0,1,3);
	}


   // m_chartHlayout->addWidget(chartViewers[PageIndex]);
    m_mainLayout->addWidget(m_Widge);
    m_mainLayout->addWidget(m_Widge1);
    setLayout(m_mainLayout);
    //--
    int MultiNum=1;//数据包含的组数（绘制图像时，进行分组）
    _WELCME_CHART_PROP_ propData;
   
    QMap<QString,QMap<int,MCompoment> >::Iterator iitt = MFileData.begin();
    for (iitt; iitt!=MFileData.end();iitt++)
    {
        if (strFileName == iitt.key())
        {
            Mdata=iitt.value();
            
            MCompoment tmpCmp;
            MCell tmpCell;
            QMap<int, MCompoment>::Iterator it = Mdata.begin();
            int nowPageId;
            int num=0 ;

            for (it; it != Mdata.end(); it++)
            {
                QString str=it->strStyle;
                QStringList strListR,strListC;
                nowPageId=it->PageIndex;
 
                if (ShowIndex == nowPageId)
                {
                     m_DataList[nowPageId].strName = str;//strName;
                     propData.Title=str;

                     switch (ShowIndex){
					case M_ENUM_Dynamic_Recrystallization:{
						num = it->dataRowNum;      
						dataTmpY = new double[num];
						dataTmpX = new QString[num];
						m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
						m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
						m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

						for (int i=0;i<it->dataColNum;i++){
							strListC.append(it->data.at(i).strName);
						}
						m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

						for (int curRow=0;curRow<it->dataRowNum;curRow++)
						{
							for(int j=0;j<it->dataColNum;j++)
							{
								m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+j).Data));
								if (j==0){
									dataTmpY[curRow]=it->data.at(curRow*2+j).Data.toDouble();
								}else if(j==1){
									dataTmpX[curRow]=it->data.at(curRow*2+j).Data;
								}
							}
							m_TableWidget[nowPageId]->item(curRow,0)->setFlags(m_TableWidget[nowPageId]->item(curRow,0)->flags()&~Qt::ItemIsEnabled);
						}
						}break;
					case M_ENUM_Damage:{
						QString str=it->strStyleParam;
						m_DataList[PageIndex].strStyleParam=str;
						m_DamageModetype.edit->setText(str);

						num = it->dataRowNum;      
						dataTmpY = new double[num];
						dataTmpX = new QString[num];
						m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
						m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
						m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

						for (int i=0;i<it->dataColNum;i++){
							strListC.append(it->data.at(i).strName);
						}
						m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

						for (int curRow=0;curRow<it->dataRowNum;curRow++)
						{
							for(int j=0;j<it->dataColNum;j++)
							{
								m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+j).Data));
								if (j==0){
									dataTmpY[curRow]=it->data.at(curRow*2+j).Data.toDouble();
								}else if(j==1){
									dataTmpX[curRow]=it->data.at(curRow*2+j).Data;
								}     
							}
							m_TableWidget[nowPageId]->item(curRow,0)->setFlags(m_TableWidget[nowPageId]->item(curRow,0)->flags()&~Qt::ItemIsEnabled);
						}
						}break;
					case M_ENUM_Rate_Dependent_Plastic:{
						QString str=it->strStyleParam;
						QString str2=str;
						m_DataList[PageIndex].strStyleParam=str;
						str=it->strStyleParam1;
						m_DataList[PageIndex].strStyleParam1=str;
						if(str!="")str2+=" 塑性功="+str;
						m_RDPModetype.edit->setText(str2);

						num = it->dataRowNum;      
						dataTmpY = new double[num];
						dataTmpX = new QString[num];
						m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
						m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
						m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

						for (int i=0;i<it->dataColNum;i++){
							strListC.append(it->data.at(i).strName);
						}
						m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

						for (int curRow=0;curRow<it->dataRowNum;curRow++)
						{
							for(int j=0;j<it->dataColNum;j++)
							{
								m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+j).Data));
								if (j==0){
									dataTmpY[curRow]=it->data.at(curRow*2+j).Data.toDouble();
								}else if(j==1){
									dataTmpX[curRow]=it->data.at(curRow*2+j).Data;
								}     
							}
							m_TableWidget[nowPageId]->item(curRow,0)->setFlags(m_TableWidget[nowPageId]->item(curRow,0)->flags()&~Qt::ItemIsEnabled);
						} 
						}break;
					case M_ENUM_METALPOWDER:{//金属粉末
                         num = it->dataRowNum2;      
                         dataTmpY = new double[num];
                         dataTmpX = new QString[num];
                         m_widMetalPowderParam->verticalHeaderItem(it->dataColNum);
                         m_widMetalPowderParam->setColumnCount(it->dataColNum);
                         m_widMetalPowderParam->setRowCount(it->dataRowNum);

                         for (int i=0;i<it->dataColNum;i++){
                             strListC.append(it->data.at(i).strName);
                         }
                         m_widMetalPowderParam->setHorizontalHeaderLabels(strListC);

                         for (int curRow=0;curRow<it->dataRowNum;curRow++){
                             for(int j=0;j<it->dataColNum;j++){
                                 m_widMetalPowderParam->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+j).Data));
                             }
                             m_widMetalPowderParam->item(curRow,0)->setFlags(m_widMetalPowderParam->item(curRow,0)->flags()&~Qt::ItemIsEnabled);
                         } 

                         m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum2);
                         m_TableWidget[nowPageId]->setColumnCount(it->dataColNum2);
                         m_TableWidget[nowPageId]->setRowCount(it->dataRowNum2);

                         int nowStartRow=(it->dataColNum)*(it->dataRowNum);
                         strListC.clear();
                         for (int i=0;i<it->dataColNum2;i++){
                             strListC.append(it->data.at(nowStartRow+i).strName);
                         }
                         m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

                         for (int curRow=0;curRow<it->dataRowNum2;curRow++){
                           for(int j=0;j<it->dataColNum2;j++){
                               QString strItem=it->data.at(nowStartRow+curRow*2+j).Data;
                               m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(strItem));
                               if (j==0){
                                  dataTmpY[curRow+j]=it->data.at(nowStartRow+curRow*2+j).Data.toDouble();
                                  //dataTmpX[curRow]=it->data.at(nowStartRow+curRow*2+j).Data;
                                }else if(j==1){
                                  dataTmpX[curRow+j-1]=it->data.at(nowStartRow+curRow*2+j).Data;
                                  //dataTmpY[curRow]=it->data.at(nowStartRow+curRow*2+j).Data.toDouble();
                                }     
                             }
                         } 
                       }break;
                     case  M_ENUM_PLASTIC:{//应力-塑型关系
                         num = it->dataRowNum;      
                         dataTmpY = new double[num];
                         dataTmpX = new QString[num];
                         dataTmpT = new QString[num];
                         m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
                         m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
                         m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

                         for (int i=0;i<it->dataColNum;i++){
                             strListC.append(it->data.at(i).strName);
                         }
                         m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

                         for (int curRow=0;curRow<it->dataRowNum;curRow++)
                         {
                             for(int j=0;j<it->dataColNum;j++)
                             {
                                 m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*(it->dataColNum)+j).Data));
                                 if (j==0){
                                     dataTmpY[curRow]=it->data.at(curRow*(it->dataColNum)+j).Data.toDouble();
                                 }else if(j==1){
                                     dataTmpX[curRow]=it->data.at(curRow*(it->dataColNum)+j).Data;
                                 }
                                 if (j==2){
                                     dataTmpT[curRow]=it->data.at(curRow*(it->dataColNum)+j).Data;
                                 }
                              
                             }
                         } 
                                              
                         if (it->dataColNum == 3){
                             for (int jj=1;jj<num;jj++){   
                                 if(dataTmpT[0] == dataTmpT[jj]){
                                     MultiNum++; //获得相同数的个数及有几组DataTmpY需要显示
                                 }
                             }
                             for (int kk=0;kk<num;kk+=MultiNum){
                                 propData.strListLabel<<"T-"+dataTmpT[kk];
                             }
                         }
                       }break;
                     case  M_ENUM_ELASTIC:{//弹性模量-泊松系数
                         num = it->dataRowNum;      
                         dataTmpY = new double[num];
                         dataTmpX = new QString[num];
                         dataTmpT = new QString[num];
                         m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
                         m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
                         m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

                         for (int i=0;i<it->dataColNum;i++){
                             strListC.append(it->data.at(i).strName);
                         }
                         m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

                         for (int curRow=0;curRow<it->dataRowNum;curRow++)
                         {
                             for(int j=0;j<it->dataColNum;j++)
                             {
                                 m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*(it->dataColNum)+j).Data));
                                 if (j==0){
                                     dataTmpY[curRow]=it->data.at(curRow*(it->dataColNum)+j).Data.toDouble();
                                 }else if(j==1){
                                     dataTmpX[curRow]=it->data.at(curRow*(it->dataColNum)+j).Data;
                                 }   
                                 if (j==2){
                                     dataTmpT[curRow]=it->data.at(curRow*(it->dataColNum)+j).Data;
                                 }
                             }
                         }  
                         //for (int jj=1;jj<num;jj++){   
                         //    if(dataTmpX[0] == dataTmpX[jj]){
                         //        MultiNum++; //获得相同数的个数及有几组DataTmpY需要显示
                         //    }
                         //}
                       }break;
                     case  M_ENUM_DENSITY:
					 case  M_ENUM_EXPANS:{//膨胀
                         QString str=it->strStyleParam;
                         m_DataList[PageIndex].strStyleParam=str;
                         str=str.right(str.size()-str.indexOf("=")-1);
                         m_EditLabExpandZero.edit->setText(str);
                        
                         num = it->dataRowNum;      
                         dataTmpY = new double[num];
                         dataTmpX = new QString[num];
                         m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
                         m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
                         m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

                         for (int i=0;i<it->dataColNum;i++){
                             strListC.append(it->data.at(i).strName);//2020-8-6，多了zero
                         }
                         m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

                         for (int curRow=0;curRow<it->dataRowNum;curRow++)//2020-8-6增加zero，是的多了一个行数据
                         {
                             for(int j=0;j<it->dataColNum;j++)
                             {
                                 m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+j).Data));
                                 if (j==0){
                                     dataTmpY[curRow]=it->data.at(curRow*2+j).Data.toDouble();
                                 }else if(j==1){
                                     dataTmpX[curRow]=it->data.at(curRow*2+j).Data;
                                 }     
                             }
                         } 
                       } break;   
                     case  M_ENUM_CONDUCT:
                     case  M_ENUM_SPECIFICE:{
                         num = it->dataRowNum;      
                         dataTmpY = new double[num];
                         dataTmpX = new QString[num];
                         m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
                         m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
                         m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

                         for (int i=0;i<it->dataColNum;i++){
                             strListC.append(it->data.at(i).strName);
                         }
                         m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

                         for (int curRow=0;curRow<it->dataRowNum;curRow++)
                         {
                             for(int j=0;j<it->dataColNum;j++)
                             {
                                 m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+j).Data));
                                 if (j==0){
                                     dataTmpY[curRow]=it->data.at(curRow*2+j).Data.toDouble();
                                 }else if(j==1){
                                     dataTmpX[curRow]=it->data.at(curRow*2+j).Data;
                                 }     
                             }
                         } 
                       }break;
                     default:break; }
               
                 if (ShowIndex !=M_ENUM_ELASTIC ){
                             
                        const char **tmp = new const char*[num];
                        QByteArray *b = new QByteArray[num]; 
                        const char *imageMap = 0;
                        for (int tt=0; tt < num; tt++){   
                            b[tt] = dataTmpX[tt].toAscii().data();
                            tmp[tt] = b[tt]; 
                        }

                        for (int SS=0;SS<it->dataColNum;SS++){
                            if (SS==0)propData.yLabel=it->data.at(SS).strName;
                            else if (SS==1)propData.xLabel=it->data.at(SS).strName;
                        }
                        
                        if(MultiNum==1){
                            StringArray dataX = StringArray(tmp, num);
                            vector<DoubleArray> dataY ;
                            dataY.push_back(DoubleArray(dataTmpY, num));
                            ChartShow2XY(dataX,dataY,imageMap,propData);
                        }else{
                           int tmpNum=int(num/MultiNum);
                           StringArray dataX = StringArray(tmp, num);
                          
                           vector<DoubleArray> dataY ;
                           for (int kk=0;kk<num;kk+=MultiNum){
                               dataY.push_back(DoubleArray(dataTmpY+kk,MultiNum));
                           }
                            ChartShow2XY(dataX,dataY,imageMap,propData);
                        }
                        dataTmpX = NULL;dataTmpY = NULL;dataTmpT = NULL; b=NULL;tmp=NULL;;
                        delete(dataTmpY);delete(dataTmpX);delete(dataTmpT);delete(tmp);delete(b);
                        break;
                 }else{
                     const char **tmp = new const char*[num];
                     QByteArray *b = new QByteArray[num]; 
                     const char *imageMap = 0;
                     for (int tt=0; tt < num; tt++){   
                         b[tt] = dataTmpT[tt].toAscii().data();
                         tmp[tt] = b[tt]; 
                     }

                     for (int SS=0;SS<it->dataColNum;SS++){
                         if (SS==0)propData.yLabel=it->data.at(SS).strName;
                         else if (SS==1)propData.yLabel2=it->data.at(SS).strName;
                         else if (SS==2)propData.xLabel=it->data.at(SS).strName;
                     }
                    
                     StringArray dataX = StringArray(tmp, num);
                     vector<DoubleArray> dataY ;
                     double *dataY1 = new double[num] ;
                     for (int nn=0;nn<num;nn++){
                        dataY1[nn]=dataTmpX[nn].toDouble();
                     }
                     
                     dataY.push_back(DoubleArray(dataY1, num));
                     dataY.push_back(DoubleArray(dataTmpY, num));
                     ChartShowDoubleY(dataX,dataY,imageMap,propData);
                    
                     dataTmpX = NULL;dataTmpY = NULL;dataTmpT = NULL; b=NULL;tmp=NULL;dataY1=NULL;
                     delete(dataTmpY);delete(dataTmpX);delete(dataTmpT);delete(tmp);delete(b);delete(dataY1);
                     break;
                 }//if(ShowIndex != M_ENUM_ELASTIC)
                }//if (ShowIndex == nowPageId)
            } //for(it; it != Mdata.end(); it++) 
            break;
        } //if(ShowIndex == nowPageId)     
    }//for (iitt; iitt!=MFileData.end();iitt++)
}

/*
function:setTabWidgetHeader
describ: set the tabwidge's V&H Header
*/
void QMaterDataWig::setTabWidgetHeader(int showIndex)
{
    QStringList strListR,strListC;
    if(m_TableWidget[showIndex]->columnCount()>0){return;}//已經存在內容則退出
    switch (showIndex){
    case M_ENUM_DENSITY:{	
        m_TableWidget[showIndex]->verticalHeaderItem(1);
        m_TableWidget[showIndex]->setRowCount(1);
        strListR.append("1");
        //strListR.append("2");
        m_TableWidget[showIndex]->setVerticalHeaderLabels(strListR);
        strListC.append("Mass Density");
        m_TableWidget[showIndex]->horizontalHeaderItem(1);
        m_TableWidget[showIndex]->setColumnCount(1);
        m_TableWidget[showIndex]->setHorizontalHeaderLabels(strListC);
    }break;
    case M_ENUM_EXPANS:{//热膨胀系数		
        m_TableWidget[showIndex]->verticalHeaderItem(1);
        m_TableWidget[showIndex]->setRowCount(1);
        strListR.append("1");
        m_TableWidget[showIndex]->setVerticalHeaderLabels(strListR);
        strListC.append("Expansion Coeff alpha");
        m_TableWidget[showIndex]->horizontalHeaderItem(1);
        m_TableWidget[showIndex]->setColumnCount(1);
		m_TableWidget[showIndex]->setHorizontalHeaderLabels(strListC);
    }break;
    case M_ENUM_ELASTIC:{				
        m_TableWidget[showIndex]->verticalHeaderItem(1);
        m_TableWidget[showIndex]->setRowCount(1);
		strListR.append("1");
		m_TableWidget[showIndex]->setVerticalHeaderLabels(strListR);
		strListC<<"Young's Modulus"<<"Poisson's Ratio";
		m_TableWidget[showIndex]->horizontalHeaderItem(2);
		m_TableWidget[showIndex]->setColumnCount(2);
		m_TableWidget[showIndex]->setHorizontalHeaderLabels(strListC);
	}break;
    case M_ENUM_PLASTIC:{
		m_TableWidget[showIndex]->verticalHeaderItem(1);
		m_TableWidget[showIndex]->setRowCount(1);
		strListR.append("1");
		m_TableWidget[showIndex]->setVerticalHeaderLabels(strListR);
		strListC<<"Yield Stress(Pa)"<<"Plastic Strain";
		m_TableWidget[showIndex]->horizontalHeaderItem(2);
		m_TableWidget[showIndex]->setColumnCount(2);
		m_TableWidget[showIndex]->setHorizontalHeaderLabels(strListC);
	}break;
    case M_ENUM_CONDUCT:{
		m_TableWidget[showIndex]->verticalHeaderItem(1);
		m_TableWidget[showIndex]->setRowCount(1);
		strListR.append("1");
		m_TableWidget[showIndex]->setVerticalHeaderLabels(strListR);
		strListC.append("Conductivity");
		m_TableWidget[showIndex]->horizontalHeaderItem(1);
		m_TableWidget[showIndex]->setColumnCount(1);
		m_TableWidget[showIndex]->setHorizontalHeaderLabels(strListC);
	}break;
    case M_ENUM_SPECIFICE:{
		m_TableWidget[showIndex]->verticalHeaderItem(1);
		m_TableWidget[showIndex]->setRowCount(1);
		strListR.append("1");
		m_TableWidget[showIndex]->setVerticalHeaderLabels(strListR);
		strListC.append("Specific Heat");
		m_TableWidget[showIndex]->horizontalHeaderItem(1);
		m_TableWidget[showIndex]->setColumnCount(1);
		m_TableWidget[showIndex]->setHorizontalHeaderLabels(strListC);
	}break;
    case M_ENUM_METALPOWDER:{
        m_TableWidget[showIndex]->verticalHeaderItem(1);
        m_TableWidget[showIndex]->setRowCount(1);
        strListR.append("1");
        m_TableWidget[showIndex]->setVerticalHeaderLabels(strListR);
        strListC<<"pb/σy"<<"Relative density";
        m_TableWidget[showIndex]->horizontalHeaderItem(2);
        m_TableWidget[showIndex]->setColumnCount(2);
        m_TableWidget[showIndex]->setHorizontalHeaderLabels(strListC);

        strListR.clear();strListC.clear();
        m_widMetalPowderParam->verticalHeaderItem(1);
        m_widMetalPowderParam->setRowCount(5);
        //strListR<<"β角度"<<"R形状控制参数"<<"ρ0初始密度"<<"d0/σy"<<"σ_threshold";
        strListR<<"1"<<"2"<<"3"<<"4"<<"5";
        m_widMetalPowderParam->setVerticalHeaderLabels(strListR);
        strListC<<"Parameter"<<"Value";
        m_widMetalPowderParam->horizontalHeaderItem(2);
        m_widMetalPowderParam->setColumnCount(2);
        m_widMetalPowderParam->setHorizontalHeaderLabels(strListC);
        QStringList strListPara;
        strListPara<<"β角度"<<"R形状控制参数"<<"d0/σy"<<"σ_threshold"<<"ρ0初始密度";
        for (int kk=0; kk<strListPara.size();kk++){
            m_widMetalPowderParam->setItem(kk,0,new QTableWidgetItem(strListPara[kk]));
            m_widMetalPowderParam->item(kk,0)->setFlags(m_widMetalPowderParam->item(kk,0)->flags()&~Qt::ItemIsEnabled);
        }
    }break;
	/*case M_ENUM_Damage:{
		m_TableWidget[showIndex]->verticalHeaderItem(1);
		m_TableWidget[showIndex]->setRowCount(1);
		strListR.append("1");
		m_TableWidget[showIndex]->setVerticalHeaderLabels(strListR);
		strListC.append("Damage");
		m_TableWidget[showIndex]->horizontalHeaderItem(1);
		m_TableWidget[showIndex]->setColumnCount(1);
		m_TableWidget[showIndex]->setHorizontalHeaderLabels(strListC);
	}break;
	case M_ENUM_Rate_Dependent_Plastic:{
		m_TableWidget[showIndex]->verticalHeaderItem(1);
		m_TableWidget[showIndex]->setRowCount(1);
		strListR.append("1");
		m_TableWidget[showIndex]->setVerticalHeaderLabels(strListR);
		strListC.append("应变速率相关");
		m_TableWidget[showIndex]->horizontalHeaderItem(1);
		m_TableWidget[showIndex]->setColumnCount(1);
		m_TableWidget[showIndex]->setHorizontalHeaderLabels(strListC);
	}break;*/
	default:break;
    } 
   // CheckTempSlot();
}

void QMaterDataWig::HideALL()
{
   /* m_CheckTemp[PageIndex]->setVisible(false);*/
    m_TableWidget[PageIndex]->setVisible(false);
    /*m_EditLabExpandZero->setVisible(false);
    m_AddBtn[PageIndex]->setVisible(false);
    m_DelBtn[PageIndex]->setVisible(false);
    m_InsertBtn[PageIndex]->setVisible(false);*/
    this->setWindowTitle("");
}
/*
   write data to TabWidget
*/
void QMaterDataWig::SetTabWidgetData(int indexP)
{	
    QMap<int, MCompoment>::Iterator it = Mdata.begin();
    PageIndex = indexP; 
    for (int i=0;i<TYPENUM;i++){
        m_TableWidget[i]->setVisible(false);
        if (PageIndex==i){
            m_TableWidget[PageIndex]->setVisible(true); 
            if (PageIndex!=M_ENUM_METALPOWDER){
                m_tabWidMetalPowder->setVisible(false);
            }else if (PageIndex == M_ENUM_METALPOWDER){
                m_tabWidMetalPowder->setVisible(true);
            }
        }
    }

    if (PageIndex == M_ENUM_METALPOWDER){
        m_Gridlayout1->addWidget(m_tabWidMetalPowder,2,0,1,3);
    }else if (PageIndex != M_ENUM_METALPOWDER){
        m_Gridlayout1->addWidget(m_TableWidget[PageIndex],2,0,1,3);//温度选择
    }
    //m_chartHlayout->addWidget(chartViewers[PageIndex]);
    // We hide unused chart viewers.
    m_mainLayout->addWidget(m_Widge);
    m_mainLayout->addWidget(m_Widge1);
    setLayout(m_mainLayout);
//--
    MCompoment tmpCmp;
    MCell tmpCell;

    int nowPageId;
    int num=0;
   
    if ((it+indexP)==Mdata.end())//没有在Mdata中则进行添加
    {
        QStringList strtmp;
   //    strtmp<<"Density"<<"Elastic"<<"Plastic"<<"Expansion"<<"Conductivity"<<"SpecificHeat"<<"MetalPowder"<<
			//"RateDependent"<<"Damage"<<"DynamicRecrystallization";//luo

	   strtmp<<"密度"<<"弹性"<<"塑性"<<"热膨胀"<<"导热"<<"比热"<<"金属粉末"<<
		"应变率相关"<<"损伤"<<"动态再结晶";//0~9

        m_DataList[indexP].strName = strtmp[indexP];//strName;
        setTabWidgetHeader(indexP);
        return;
    }
    int MultiNum=1;
    for (it; it != Mdata.end(); it++)
	{
        num = it->dataRowNum2;      
        //double *dataTmpY = new double[num];//QString *dataTmpX = new QString[num];
        dataTmpY = new double[num];
        dataTmpX = new QString[num];
        QString str=it->strStyle;
        QStringList strListR,strListC;
        nowPageId=it->PageIndex;
        m_DataList[nowPageId].strName = str;//strName;
      
        _WELCME_CHART_PROP_ tmpChartProp;
        tmpChartProp.Title=str; 
        if (nowPageId == M_ENUM_METALPOWDER)
        {
            m_widMetalPowderParam->verticalHeaderItem(it->dataColNum);
            m_widMetalPowderParam->setColumnCount(it->dataColNum);
            m_widMetalPowderParam->setRowCount(it->dataRowNum);

            for (int i=0;i<it->dataColNum;i++){
                strListC.append(it->data.at(i).strName);
            }
            m_widMetalPowderParam->setHorizontalHeaderLabels(strListC);

            for (int curRow=0;curRow<it->dataRowNum;curRow++)
            {
                for(int j=0;j<it->dataColNum;j++)
                {
                    m_widMetalPowderParam->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+j).Data));
                    //if (j==0){
                    //    dataTmpY[curRow+j]=it->data.at(curRow*2+j).Data.toDouble();
                    //}else if(j==1){
                    //    dataTmpX[curRow+j-1]=it->data.at(curRow*2+j).Data;
                    //    //dataTmpX.push_back(it->data.at(curRow+j).Data.toDouble());
                    //}     
                }
            } 


            m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum2);
            m_TableWidget[nowPageId]->setColumnCount(it->dataColNum2);
            m_TableWidget[nowPageId]->setRowCount(it->dataRowNum2);

            int nowStartRow=(it->dataColNum)*(it->dataRowNum);
            strListC.clear();
            for (int i=0;i<it->dataColNum2;i++){
                strListC.append(it->data.at(nowStartRow+i).strName);
            }
            m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);
            
            for (int curRow=0;curRow<it->dataRowNum2;curRow++)
            {
                for(int j=0;j<it->dataColNum2;j++)
               {
                    m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(nowStartRow+curRow*2+j).Data));  
                    if (j==0){
                        dataTmpY[curRow]=it->data.at(nowStartRow+curRow*2+j).Data.toDouble();
                    }else if(j==1){
                        dataTmpX[curRow]=it->data.at(nowStartRow+curRow*2+j).Data;
                    }     
                }
            } 
        }else{
            switch (nowPageId)
            {
             case M_ENUM_PLASTIC: {//应力-塑型关系
                 num = it->dataRowNum;      
                 dataTmpY = new double[num];
                 dataTmpX = new QString[num];
                 dataTmpT = new QString[num];
                 m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
                 m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
                 m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

                 for (int i=0;i<it->dataColNum;i++){
                     strListC.append(it->data.at(i).strName);
                 }
                 m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

                 for (int curRow=0;curRow<it->dataRowNum;curRow++)
                 {
                     for(int j=0;j<it->dataColNum;j++)
                     {
                         m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*(it->dataColNum)+j).Data));
                         if (j==0){
                             dataTmpY[curRow]=it->data.at(curRow*(it->dataColNum)+j).Data.toDouble();
                         }else if(j==1){
                             dataTmpX[curRow]=it->data.at(curRow*(it->dataColNum)+j).Data;
                         }
                         if(j==2){
                            dataTmpT[curRow]=it->data.at(curRow*(it->dataColNum)+j).Data;
                         }
                     }
                 } 
                 if (it->dataColNum == 3){
                     for (int jj=1;jj<num;jj++){   
                         if(dataTmpT[0] == dataTmpT[jj]){
                             MultiNum++; //获得相同数的个数及有几组DataTmpY需要显示
                         }
                     } 
                     for (int kk=0;kk<num;kk+=MultiNum){
                         tmpChartProp.strListLabel<<"T-"+dataTmpT[kk];
                     }
                 }
                
               }break;
			 case M_ENUM_ELASTIC: {//弹性模量-泊松系数
                 num = it->dataRowNum;      
                 dataTmpY = new double[num];
                 dataTmpX = new QString[num];
                 dataTmpT = new QString[num];
                 m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
                 m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
                 m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

                 for (int i=0;i<it->dataColNum;i++){
                     strListC.append(it->data.at(i).strName);
                 }
                 m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);

                 for (int curRow=0;curRow<it->dataRowNum;curRow++)
                 {
                     for(int j=0;j<it->dataColNum;j++)
                     {
                         m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+j).Data));
                         if (j==0){
                             dataTmpY[curRow]=it->data.at(curRow*2+j).Data.toDouble();
                         }else if(j==1){
                             dataTmpX[curRow]=it->data.at(curRow*2+j).Data;
                         }
                         if (j==2){
                             dataTmpT[curRow]=it->data.at(curRow*2+j).Data;
                         }
                     }
                 } 

               }break;
			/* case M_ENUM_EXPANS:{
				 num = it->dataRowNum;      
				 dataTmpY = new double[num];
				 dataTmpX = new QString[num];
				 m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
				 m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
				 m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

				 for (int i=0;i<it->dataColNum;i++){
					 strListC.append(it->data.at(i).strName);
				 }
				 m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);


				 for (int curRow=0;curRow<it->dataRowNum;curRow++)
				 {
					 for(int j=0;j<it->dataColNum;j++)
					 {
						 m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+1+j).Data));
						 if (j==0){
							 dataTmpY[curRow]=it->data.at(curRow*2+1+j).Data.toDouble();
						 }else if(j==1){
							 dataTmpX[curRow]=it->data.at(curRow*2+1+j).Data;
						 }     
					 }
				 } 
				}break;		*/		
             default:
                 num = it->dataRowNum;      
                 dataTmpY = new double[num];
                 dataTmpX = new QString[num];
                 m_TableWidget[nowPageId]->verticalHeaderItem(it->dataColNum);
                 m_TableWidget[nowPageId]->setColumnCount(it->dataColNum);
                 m_TableWidget[nowPageId]->setRowCount(it->dataRowNum);

                 for (int i=0;i<it->dataColNum;i++){
                     strListC.append(it->data.at(i).strName);
                 }
                 m_TableWidget[nowPageId]->setHorizontalHeaderLabels(strListC);


                 for (int curRow=0;curRow<it->dataRowNum;curRow++)
                 {
                     for(int j=0;j<it->dataColNum;j++)
                     {
                         m_TableWidget[nowPageId]->setItem(curRow,j,new QTableWidgetItem(it->data.at(curRow*2+j).Data));
                         if (j==0){
                             dataTmpY[curRow]=it->data.at(curRow*2+j).Data.toDouble();
                         }else if(j==1){
                             dataTmpX[curRow]=it->data.at(curRow*2+j).Data;
                         }     
                     }
                 } 
            }break;
          }
         
        //--CHART SHOW
        if (nowPageId !=M_ENUM_ELASTIC )
        {
            const char **tmp = new const char*[num];
            QByteArray *b = new QByteArray[num]; 
            const char *imageMap = 0;  
            for (int tt=0;tt<num;tt++){   
                b[tt] = dataTmpX[tt].toAscii().data();
                tmp[tt]=b[tt]; 
            }

            for (int SS=0;SS<it->dataColNum;SS++){
                if (SS==0)tmpChartProp.xLabel=it->data.at(SS).strName;
                else if (SS==1)tmpChartProp.yLabel=it->data.at(SS).strName;
            }

            if(MultiNum==1){
                StringArray dataX = StringArray(tmp, num);
                vector<DoubleArray> dataY ;
                dataY.push_back(DoubleArray(dataTmpY, num));
                ChartShow2XY(dataX,dataY,imageMap,tmpChartProp);
            }else{
                int tmpNum=int(num/MultiNum);
                StringArray dataX = StringArray(tmp, tmpNum);
                vector<DoubleArray> dataY ;
                for (int kk=0;kk<tmpNum;kk++){
                    dataY.push_back(DoubleArray(dataTmpY+kk,MultiNum));
                }
                ChartShow2XY(dataX,dataY,imageMap,tmpChartProp);
            }
            dataTmpY = NULL;dataTmpX = NULL;dataTmpT = NULL;b=NULL;tmp=NULL;
            delete(dataTmpY);delete(dataTmpX);delete(dataTmpT);delete(tmp);delete(b);
        }else{//双纵轴
            const char **tmp = new const char*[num];
            QByteArray *b = new QByteArray[num]; 
            const char *imageMap = 0;
            for (int tt=0; tt < num; tt++){   
                b[tt] = dataTmpT[tt].toAscii().data();
                tmp[tt] = b[tt]; 
            }

            for (int SS=0;SS<it->dataColNum;SS++){
                if (SS==0)tmpChartProp.yLabel=it->data.at(SS).strName;
                else if (SS==1)tmpChartProp.yLabel2=it->data.at(SS).strName;
                else if (SS==2)tmpChartProp.xLabel=it->data.at(SS).strName;
                
            }

            StringArray dataX = StringArray(tmp, num);
            vector<DoubleArray> dataY ;
            double *dataY1 = new double[num] ;
            for (int nn=0;nn<num;nn++){
                dataY1[nn]=dataTmpX[nn].toDouble();
            }

            dataY.push_back(DoubleArray(dataY1, num));
            dataY.push_back(DoubleArray(dataTmpY, num));
            ChartShowDoubleY(dataX,dataY,imageMap,tmpChartProp);

            dataTmpX = NULL;dataTmpY = NULL;dataTmpT = NULL; b=NULL;tmp=NULL;dataY1=NULL;
            delete(dataTmpY);delete(dataTmpX);delete(dataTmpT);delete(tmp);delete(b);delete(dataY1);
            break;
        }
    }

    emit emitMaterList(Mdata);//emitMaterList(m_DataList);
}

//--SLOT
/*
function: CheckTempSlot
describ: check the Tempture's checkbox is checked or not,then 
		 add the coloume "tempture" in the TabWiget.
*/

//void QMaterDataWig::CheckTempSlot()
//{
//	int i=0,ColCnt =0;
//	bool bFlag=false;
//	QString str="";
//	QStringList strListC;
//
//    
//	bFlag = m_CheckTemp[PageIndex]->isChecked();
//	if (true == bFlag)
//	{
//	    ColCnt = m_TableWidget[PageIndex]->columnCount();		
//	    for (i=0; i<ColCnt; i++)
//	    {
//		    str=m_TableWidget[PageIndex]->horizontalHeaderItem(i)->text();
//		    if (str == "Temp"){//定位到当前的单元格
//			    break;
//		    }
//		    strListC.append(str);
//	    }
//	    if (i == ColCnt){
//		    m_TableWidget[PageIndex]->horizontalHeaderItem(ColCnt+1);
//		    m_TableWidget[PageIndex]->setColumnCount(ColCnt+1);
//		    strListC.append("Temp");
//		    m_TableWidget[PageIndex]->setHorizontalHeaderLabels(strListC);
//	    }
//	}else{
//		ColCnt = m_TableWidget[PageIndex]->columnCount();
//		for (i=0; i<ColCnt; i++)
//		{
//			str=m_TableWidget[PageIndex]->horizontalHeaderItem(i)->text();
//			if (str == "Temp"){//定位到当前的单元格
//				m_TableWidget[PageIndex]->removeColumn(i);
//				m_TableWidget[PageIndex]->takeHorizontalHeaderItem(i);
//				break;
//			}
//		}
//	}
//}
////*增
//void QMaterDataWig::AddLineSlot()
//{
//    int curRow = m_TableWidget[PageIndex]->rowCount();
//    if (curRow >= 0){
//        m_TableWidget[PageIndex]->insertRow(curRow);
//        for(int j=0;j<m_TableWidget[PageIndex]->columnCount();j++){
//           m_TableWidget[PageIndex]->setItem(curRow,j,new QTableWidgetItem());
//        }
//    }
//}
////*删除
//void QMaterDataWig::DelLineSlot()
//{
//    int curRow = m_TableWidget[PageIndex]->currentRow();
//    if (curRow!=-1)
//    {
//        m_TableWidget[PageIndex]->removeRow(curRow);
//    }
//}
//
//
////*插入
//void QMaterDataWig::InsertLineBtn()
//{
//    int curRow = m_TableWidget[PageIndex]->currentRow();
//
//    m_TableWidget[PageIndex]->insertRow(curRow);
//}
/*
//向TabWidget导入数据
void QMaterDataWig::ImportData()
{
	QFile materFile ;
	m_TabWidget[PageIndex]->setItem(0,0,new QTableWidgetItem(m_DataList[PageIndex+1].Data1));
	//m_TmpDataList[M_ENUM_DENSITY+1].Data1=m_TabWidget->item(0,0)->text();
	//m_DataList[M_ENUM_DENSITY+1].Data1=m_TmpDataList[M_ENUM_DENSITY+1].Data1;
	//QString strTmp;
	//strTmp=m_TabWidget->item(0,0)->text();

}*/
//从TabWidget导出数据
void QMaterDataWig::ExportData()
{
	QString fileName =QFileDialog::getSaveFileName(this,tr("Save as xml"),QString(),"XML files(*.xml)");
	if (fileName == ""){
		return;
	}
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
		return;
	}
	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartElement("Material");
	int pIndex=0;int row,col;
	for (pIndex=0;pIndex<10;pIndex++)
	{
        if (pIndex == M_ENUM_METALPOWDER)
		{
            row = m_widMetalPowderParam->rowCount();
            col = m_widMetalPowderParam->columnCount();
            if (m_DataList[pIndex].strName=="")
            {
                continue;
            }
            writer.writeStartElement(m_DataList[pIndex].strName);
            for (int i = 0; i<row; i++)
            {
                for (int j=0; j<col;j++)
                {
                    QString str,str1;
                    str=m_widMetalPowderParam->horizontalHeaderItem(j)->text();
                    str.replace(" ", "_");
                    str.replace("'", "-");
                    str1="NULL";
                    if (m_widMetalPowderParam->item(i,j) != NULL){
                        str1=m_widMetalPowderParam->item(i,j)->text(); 
                    }
                    writer.writeTextElement(str, str1);
                }
            }

            row = m_TableWidget[pIndex]->rowCount();
            col = m_TableWidget[pIndex]->columnCount();
           // writer.writeStartElement(m_DataList[pIndex].strName);
            for (int i=0; i<row; i++)
            {
                for (int j=0; j<col ; j++)
                {
                    QString str,str1;
                    str=m_TableWidget[pIndex]->horizontalHeaderItem(j)->text();
                    str.replace(" ", "_");
                    str.replace("'", "-");
                    str.replace("pb/σy", "pb");
                    str1="NULL";
                    if (m_TableWidget[pIndex]->item(i,j) != NULL){
                        str1=m_TableWidget[pIndex]->item(i,j)->text(); 
                    }
                    if (j==0){
                        m_DataList[j].Data1 = str1;
                    }else if(j!=0 && str!="Temp_c"){
                        m_DataList[j].Data2 = str1;
                    }else{
                        m_DataList[j].TempData = str1;
                    }
                    writer.writeTextElement(str, str1);
                }
			}
		}else{
            row = m_TableWidget[pIndex]->rowCount();
            col = m_TableWidget[pIndex]->columnCount();
            if (m_DataList[pIndex].strName==""){continue;}
            writer.writeStartElement(m_DataList[pIndex].strName);
			if (m_DataList[pIndex].strStyleParam!=""){
                QString strP0,strP1,strP2;
                strP0=m_DataList[pIndex].strStyleParam.simplified();
				if(pIndex==M_ENUM_Damage){strP0.remove(QRegExp("\\s"));strP0.remove("&");}
				QString st=m_DataList[pIndex].strName.toLower();
				if(st=="expansion"){
					strP1="zero";
				}else{
					strP1=strP0.left(strP0.indexOf("="));
				}
                strP2=strP0.right(strP0.size()-strP0.indexOf("=")-1);
                writer.writeTextElement(strP1,strP2);
			}
			if (m_DataList[pIndex].strStyleParam1!=""){
				QString strP0,strP1,strP2;
				strP0=m_DataList[pIndex].strStyleParam1.simplified();
				strP1="PlasticWork";
				strP2=strP0;
				writer.writeTextElement(strP1,strP2);
			}

		    for (int i=0; i<row; i++)
		    {
			    for (int j=0; j<col ; j++)
			    {
				    QString str,str1;
				    str=m_TableWidget[pIndex]->horizontalHeaderItem(j)->text();
                    str.replace(" ", "_");
                    str.replace("'", "-");
                    str.replace("(℃)","_c");
                    str.replace("(","_");
                    str.replace(")","_");
                    str.replace(")","_");
					if(str=="Conductivity")str=str+"_";

				    str1="NULL";
				    if (m_TableWidget[pIndex]->item(i,j) != NULL){
					    str1=m_TableWidget[pIndex]->item(i,j)->text(); 
				    }
				    if (j==0){
					    m_DataList[j].Data1 = str1;
				    }else if(j!=0 && str!="Temp_c"){
					    m_DataList[j].Data2 = str1;
				    }else{
					    m_DataList[j].TempData = str1;
				    }
				    writer.writeTextElement(str, str1);
			    }
		    }
        }
		writer.writeEndElement();
	}
	
	writer.writeEndElement();
	writer.writeEndDocument();
	file.close();
	//QDomDocument document;
	//QString strHeader("Material Data")//("version=\"1.0\"encoding=\"UTF-8\"");
	//document.appendChild(document.createProcessingInstruction("xml",strHeader));
	//
	//QDomElement root_elem=document.createElement("items");
	//root_elem.setAttribute(m_TabWidget[PageIndex],1);
	//document.appendChild(root_elem);

	//QDomElement item1=document.createElement("item");
	//item1.setAttribute("src","&lt;&lg");
	//item1.setAttribute("dst","<>");
	//root_elem.appendChild(item1);

	//QDomElement item2=document.createElement("item");
	//item2.setAttribute("src","&quot;&apos;&amp");
	//item2.setAttribute("dst","<>");
	//root_elem.appendChild(item2);

	//QDomElement item3=document.createElement("item");
	//item3.setAttribute("src",tr("测试数据"));
	//item3.setAttribute("dst",tr("一二三四"));
	//root_elem.appendChild(item3);
	//
	//QTextStream out(&file);
	//document.save(out,4);//,QDomNode::EncodingFromTextStream);
	//file.close();
}
//向TabWidget写入数据
bool QMaterDataWig::ImportData()
{
	QString fileName =QFileDialog::getOpenFileName(this,tr("Read xml"),QString(),"XML files(*.xml)");//getSaveFileName(this,tr("Read xml"),QString(),"XML files(*.xml)");
	if (fileName == ""){
		return false;
	}
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
		qDebug()<<"ERROR Open File";
		return false;
	}

    QStringList lists = fileName.split(".");
    QStringList listName =lists.first().split("/");
    m_NowFileName = listName.back();//lists.back();

	QString m_data;
	QXmlStreamReader Reader(&file);
    QXmlStreamReader::TokenType type;
    QString strtmp;
	while (!Reader.atEnd())
	{
		type = Reader.readNext();
        strtmp=Reader.name().toString();
		if (type == QXmlStreamReader::StartElement)
		{
			if (strtmp=="Material")
			{
                 ParseEntry(Reader);
			}
		}	
	}
    file.close();
    WriteImportDataToMFile(m_NowFileName,Mdata);
   

    if (Reader.hasError())//解析出错时
    {
        //return false;
       // QMessageBox::warning(this,tr("Import File"),tr("Read Material File Error"),QMessageBox::Ok,QMessageBox::Ok);
    }
    return true; 
}
void QMaterDataWig::ParseEntry(QXmlStreamReader &reader)
{
    MCompoment tmpMcmp;
    QStringRef strPageTmp;
    Mdata.clear();
    
    QXmlStreamReader::TokenType type;
    while (!reader.atEnd())
    {
        type = reader.readNext();
        QString strName = reader.name().toString();
        if(type == QXmlStreamReader::EndElement)
        {
            if(reader.name()=="Material")
                break;
        }
       /* if (type==QXmlStreamReader::StartElement)
        {
            QString strName = reader.name().toString();

            while(strName=="Density"){
                type= reader.readNext();
                QString strN = reader.name().toString();
                if (type == QXmlStreamReader::StartElement)
                {   
                    while(chgTheNameToInt(strName)==-1){
                        strN = reader.name().toString();
                    }
                    if (chgTheNameToInt(strN)==1)
                    {
                        strName = strN;
                    }
                }
            }
            if (strName=="Elastic")
            {
                type= reader.readNext();
                if (type == QXmlStreamReader::StartElement)
                {
                    strName = reader.name().toString();
                }
            }

            if (strName=="Plastic")
            {
                type= reader.readNext();
                if (type == QXmlStreamReader::StartElement)
                {
                    strName = reader.name().toString();
                }
            }
            if (strName=="Expansion")
            {
                type= reader.readNext();
                if (type == QXmlStreamReader::StartElement)
                {
                    strName = reader.name().toString();
                }
            }*/
        if (type==QXmlStreamReader::StartElement)
		{
            if (strName=="Density"){
                ParamGet(M_ENUM_DENSITY, reader);
            }else if (strName=="Elastic"){
                ParamGet(M_ENUM_ELASTIC, reader);
            }else if (strName=="Plastic"){
                ParamGet(M_ENUM_PLASTIC, reader);
            }else if (strName=="Expansion"){
                ParamGet(M_ENUM_EXPANS, reader);
            }else if (strName=="Conductivity"){
                ParamGet(M_ENUM_CONDUCT, reader);
            }else if (strName=="SpecificHeat"){
                ParamGet(M_ENUM_SPECIFICE, reader);
            }else if (strName=="MetalPowder"){
                ParamGet(M_ENUM_METALPOWDER, reader);
            }else if (strName=="RateDependent"){
                ParamGet(M_ENUM_Rate_Dependent_Plastic, reader);
            }else if (strName=="Damage"){
                ParamGet(M_ENUM_Damage, reader);
            }else if (strName=="DynamicRecrystallization"){
                ParamGet(M_ENUM_Dynamic_Recrystallization, reader);
            }                                
        }
        //if (type==QXmlStreamReader::StartElement)
        //{
        //    QStringRef strRef = reader.name();
        //    int NameId = chgTheNameToInt(strRef);
        //    MCell tmpCellData;
        //    MCompoment tmpMcmp1;
        //    tmpMcmp1=tmpMcmp;

        //    switch(NameId){
        //    case 1: ParseEntry(reader); break;
        //    case 10:   
        //      tmpMcmp.id = 0;
        //      tmpMcmp.strStyle = "Density";
        //      tmpMcmp.dataColNum = 1;
        //      tmpMcmp.dataRowNum++;
        //      tmpMcmp.PageIndex=M_ENUM_DENSITY;
        //      tmpCellData.strName = "Mass_Density";
        //      tmpCellData.Data = reader.readElementText();
        //      tmpMcmp.data.push_back(tmpCellData);
        //      Mdata.insert(0, tmpMcmp);
        //     break;
        //      //m_DataList[PageIndex].Data1= reader.readElementText();break;
        //    case 11:
        //      tmpMcmp1.dataColNum=2;
        //      //tmpMcmp1.dataRowNum++;
        //      tmpCellData.strName = "Temp";
        //      tmpCellData.Data= reader.readElementText();
        //      tmpMcmp1.data.push_back(tmpCellData);
        //      tmpMcmp = tmpMcmp1;
        //      Mdata.insert(0,tmpMcmp);
        //     break;
        //      //m_DataList[PageIndex].TempData= reader.readElementText();break;
        //    case 20:
        //      tmpMcmp1.id=1;
        //      tmpMcmp1.strStyle="Elastic";
        //      tmpMcmp.PageIndex=M_ENUM_ELASTIC;
        //      tmpMcmp1.dataColNum=1;
        //      tmpMcmp1.dataRowNum++;
        //      tmpCellData.strName ="Young-s_Modulus";
        //      tmpCellData.Data = reader.readElementText();
        //      tmpMcmp1.data.push_back(tmpCellData);
        //      tmpMcmp = tmpMcmp1;
        //      Mdata.insert(1,tmpMcmp);
        //     break;
        //      //m_DataList[PageIndex].Data1= reader.readElementText();break;
        //    case 21:
        //      tmpMcmp1.id=1;
        //      tmpMcmp1.dataColNum=2;
        //      tmpCellData.strName="Poisson-s_Ratio";
        //      tmpCellData.Data= reader.readElementText();
        //      tmpMcmp1.data.push_back(tmpCellData);
        //      tmpMcmp = tmpMcmp1;
        //      Mdata.insert(1,tmpMcmp);
        //     // m_DataList[PageIndex].Data2= reader.readElementText();break;
        //    default:
        //      break;
        //    }
        //}
    }
}

void QMaterDataWig::ParamGet(int index, QXmlStreamReader &reader)
{
	if (index<0 ){
		return ;
	}

	int NameId=0;
	QString strRef ;
	QXmlStreamReader::TokenType type;
	MCompoment OldMcmp;
	OldMcmp.FileName=m_NowFileName;
	if (index==M_ENUM_DENSITY){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if(type == QXmlStreamReader::EndElement){
				if(strRef=="Density")break;    
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData; MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;

				switch(NameId){
				case 1:return;break;
				case 10:{   
					OldMcmp.id = 0;
					OldMcmp.strStyle = "Density";
					OldMcmp.dataColNum = 1;
					OldMcmp.dataRowNum++;
					OldMcmp.PageIndex=M_ENUM_DENSITY;
					tmpCellData.strName = "密度";
					tmpCellData.Data = reader.readElementText();
					OldMcmp.data.push_back(tmpCellData);
					Mdata.insert(M_ENUM_DENSITY, OldMcmp);
						}break;
					//m_DataList[PageIndex].Data1= reader.readElementText();break;
				case 999:{
					tmpMcmp1.dataColNum=2;
					tmpMcmp1.bTemper=true;
					//tmpMcmp1.dataRowNum++;
					tmpCellData.strName = "温度(℃)";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(M_ENUM_DENSITY,OldMcmp);
						 }break; 
				default:break;
				}
			}
		}
	}else if (index == M_ENUM_ELASTIC){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if (type == QXmlStreamReader::Invalid)
			{
				std::string tmpStr=reader.errorString().toStdString();
			}
			if(type == QXmlStreamReader::EndElement){
				if(strRef=="Elastic")break;    
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData;MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;

				switch(NameId){
				case 1:return;break;
				case 20:{
					tmpMcmp1.id=M_ENUM_ELASTIC;
					tmpMcmp1.strStyle="Elastic";
					tmpMcmp1.PageIndex=M_ENUM_ELASTIC;
					tmpMcmp1.dataColNum=1;
					tmpMcmp1.dataRowNum++;
					tmpCellData.strName ="杨氏模量";
					tmpCellData.Data = reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(M_ENUM_ELASTIC,OldMcmp);
						}break;
					//m_DataList[PageIndex].Data1= reader.readElementText();break;
				case 21:{
					tmpMcmp1.id=M_ENUM_ELASTIC;
					tmpMcmp1.dataColNum=2;
					tmpCellData.strName="泊松比";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(M_ENUM_ELASTIC,OldMcmp);
					//m_DataList[PageIndex].Data2= reader.readElementText();break;
						}break; 
				case 999:{
					tmpMcmp1.id=M_ENUM_ELASTIC;
					tmpMcmp1.bTemper=true;
					tmpMcmp1.dataColNum=3;
					tmpCellData.strName="温度(℃)";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(M_ENUM_ELASTIC,OldMcmp);
					//m_DataList[PageIndex].Data2= reader.readElementText();break;
						 }break;
				default:break;
				}
			}
		}

	}else if (index == M_ENUM_PLASTIC){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if(type == QXmlStreamReader::EndElement)
			{
				if(strRef=="Plastic")break;    
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData;MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;

				switch(NameId){
				case 1:return;break;
				case 30:{
					tmpMcmp1.id=M_ENUM_PLASTIC;
					tmpMcmp1.strStyle="Plastic";
					tmpMcmp1.PageIndex=M_ENUM_PLASTIC;
					tmpMcmp1.dataColNum=1;
					tmpMcmp1.dataRowNum++;
					tmpCellData.strName ="屈服应力(Pa)";
					tmpCellData.Data = reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(M_ENUM_PLASTIC,OldMcmp);
						}break;
					//m_DataList[PageIndex].Data1= reader.readElementText();break;
				case 31:{
					tmpMcmp1.id=M_ENUM_PLASTIC;
					tmpMcmp1.dataColNum=2;
					tmpCellData.strName="塑性应变";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(M_ENUM_PLASTIC,OldMcmp);
					// m_DataList[PageIndex].Data2= reader.readElementText();break;
						}break; 
				case 999:{
					tmpMcmp1.id=M_ENUM_PLASTIC;
					tmpMcmp1.bTemper=true;
					tmpMcmp1.dataColNum=3;
					tmpCellData.strName="温度(℃)";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(M_ENUM_PLASTIC,OldMcmp);
					// m_DataList[PageIndex].Data2= reader.readElementText();break;
						 }break;
				default:break;
				}
			}
		}
	}else if (index == M_ENUM_EXPANS){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if(type == QXmlStreamReader::EndElement)
			{
				if(strRef=="Expansion")break;    
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData;MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;
				static QString strStatic;

				switch(NameId){
				case 1:return;break;
				case 41:{
					tmpMcmp1.id=M_ENUM_EXPANS;
					tmpMcmp1.strStyle="Expansion";
					tmpMcmp1.PageIndex=M_ENUM_EXPANS;
					tmpMcmp1.strStyleParam=strStatic;
					tmpMcmp1.dataColNum=1;
					tmpMcmp1.dataRowNum++;
					tmpCellData.strName ="热膨胀";
					tmpCellData.Data = reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(M_ENUM_EXPANS,OldMcmp);
						}break;
				case 40:{
					strStatic=reader.readElementText();
					//tmpMcmp1.id=M_ENUM_EXPANS;
					//tmpMcmp1.strStyle="Expansion";
					//tmpMcmp1.strStyleParam=reader.readElementText();
					//tmpMcmp1.PageIndex=M_ENUM_EXPANS;
					//tmpMcmp1.dataColNum=1;
					//tmpMcmp1.dataRowNum=0;
					//tmpCellData.strName ="Expansion Coeff alpha";
					//tmpCellData.Data = reader.readElementText();
					//tmpMcmp1.data.push_back(tmpCellData);
					// OldMcmp = tmpMcmp1;
					// Mdata.insert(M_ENUM_EXPANS,OldMcmp);
						}break;
					//m_DataList[PageIndex].Data1= reader.readElementText();break;
				case 999:{
					tmpMcmp1.id=M_ENUM_EXPANS;
					tmpMcmp1.bTemper=true;
					tmpMcmp1.dataColNum=2;
					tmpCellData.strName="温度(℃)";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(M_ENUM_EXPANS,OldMcmp);
					// m_DataList[PageIndex].Data2= reader.readElementText();break;
						 }break;
				default:break;
				}
			}
		}
	}else if (index == M_ENUM_CONDUCT){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if(type == QXmlStreamReader::EndElement)
			{
				if(strRef=="Conductivity")break;    
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData;MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;
				switch(NameId){
				case 1:return;break;
				case 50:{
					tmpMcmp1.id=index;
					tmpMcmp1.strStyle="Conductivity";
					tmpMcmp1.PageIndex=index;
					tmpMcmp1.dataColNum=1;
					tmpMcmp1.dataRowNum++;
					tmpCellData.strName ="导热";
					tmpCellData.Data = reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 999:{
					tmpMcmp1.id=index;
					tmpMcmp1.bTemper=true;
					tmpMcmp1.dataColNum=2;
					tmpCellData.strName="温度(℃)";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						 }break;
				default:break;
				}
			}
		}
	}else if (index == M_ENUM_SPECIFICE){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if(type == QXmlStreamReader::EndElement)
			{
				if(strRef=="SpecificHeat")break;    
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData;MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;

				switch(NameId){
				case 1:return;break;
				case 60:{
					tmpMcmp1.id=index;
					tmpMcmp1.strStyle="SpecificHeat";
					tmpMcmp1.PageIndex=index;
					tmpMcmp1.dataColNum=1;
					tmpMcmp1.dataRowNum++;
					tmpCellData.strName ="比热";
					tmpCellData.Data = reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 999:{
					tmpMcmp1.id=index;
					tmpMcmp1.dataColNum=2;
					tmpMcmp1.bTemper=true;
					tmpCellData.strName="温度(℃)";//??C//C(o)
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						 }break;
				default:break;
				}
			}
		}
	}else if (index == M_ENUM_METALPOWDER){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if(type == QXmlStreamReader::EndElement)
			{
				if(strRef=="MetalPowder")break;    
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData;MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;

				switch(NameId){
				case 1:return;break;
				case 70:{
					tmpMcmp1.id=index;
					tmpMcmp1.strStyle="MetalPowder";
					tmpMcmp1.PageIndex=index;
					tmpMcmp1.dataColNum=1;
					tmpMcmp1.dataRowNum++;
					tmpCellData.strName ="参数";
					tmpCellData.Data = reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 71:{
					tmpMcmp1.id=index;
					tmpMcmp1.dataColNum=2;
					tmpCellData.strName="值";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 72:{
					tmpMcmp1.id=index;
					tmpMcmp1.dataColNum=2;
					tmpMcmp1.dataColNum2=2;
					tmpMcmp1.dataRowNum2++;
					tmpCellData.strName="pb/σy";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 73:{
					tmpMcmp1.id=index;
					tmpMcmp1.dataColNum=2;
					tmpCellData.strName="相对密度";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				default:break;
				}
			}
		}
	}else if (index == M_ENUM_Rate_Dependent_Plastic){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if(type == QXmlStreamReader::EndElement)
			{
				if(strRef=="RateDependent")break;    
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData;MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;
				static QString strStatic;

				switch(NameId){
				case 1:return;break;
				case 15:{
					strStatic=reader.readElementText();
						}break;
				case 14:{
					tmpMcmp1.id=index;
					tmpMcmp1.strStyle="RateDependent";
					tmpMcmp1.PageIndex=index;
					tmpMcmp1.strStyleParam= reader.readElementText();
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 70:{
					tmpMcmp1.id=index;
					tmpMcmp1.strStyle="RateDependent";
					tmpMcmp1.PageIndex=index;
                    tmpMcmp1.strStyleParam1=strStatic;//luo
					tmpMcmp1.dataColNum=1;
					tmpMcmp1.dataRowNum++;
					tmpCellData.strName ="参数";
					tmpCellData.Data = reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 71:{
					tmpMcmp1.id=index;
					tmpMcmp1.dataColNum=2;
					tmpCellData.strName="值";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				default:break;
				}
			}
		}
	}else if (index == M_ENUM_Damage){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if(type == QXmlStreamReader::EndElement)
			{
				if(strRef=="Damage")break;
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData;MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;

				switch(NameId){
				case 1:return;break;
				case 14:{
					tmpMcmp1.id=index;
					tmpMcmp1.strStyle="Damage";
					tmpMcmp1.PageIndex=index;
					tmpMcmp1.strStyleParam= reader.readElementText();
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 70:{
					tmpMcmp1.id=index;
					tmpMcmp1.strStyle="Damage";
					tmpMcmp1.PageIndex=index;
					tmpMcmp1.dataColNum=1;
					tmpMcmp1.dataRowNum++;
					tmpCellData.strName ="参数";
					tmpCellData.Data = reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 71:{
					tmpMcmp1.id=index;
					tmpMcmp1.dataColNum=2;
					tmpCellData.strName="值";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				default:break;
				}
			}
		}
	}else if (index == M_ENUM_Dynamic_Recrystallization){
		while (NameId !=1)
		{
			type = reader.readNext();
			strRef = reader.name().toString();
			if(type == QXmlStreamReader::EndElement)
			{
				if(strRef=="DynamicRecrystallization")break;    
			}
			if (type == QXmlStreamReader::StartElement)
			{
				NameId = chgTheNameToInt(strRef);
				MCell tmpCellData;MCompoment tmpMcmp1;
				tmpMcmp1=OldMcmp;
				switch(NameId){
				case 1:return;break;
				case 70:{
					tmpMcmp1.id=index;
					tmpMcmp1.strStyle="DynamicRecrystallization";
					tmpMcmp1.PageIndex=index;
					tmpMcmp1.dataColNum=1;
					tmpMcmp1.dataRowNum++;
					tmpCellData.strName ="参数";
					tmpCellData.Data = reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				case 71:{
					tmpMcmp1.id=index;
					tmpMcmp1.dataColNum=2;
					tmpCellData.strName="值";
					tmpCellData.Data= reader.readElementText();
					tmpMcmp1.data.push_back(tmpCellData);
					OldMcmp = tmpMcmp1;
					Mdata.insert(index,OldMcmp);
						}break;
				default:break;
				}
			}
		}
	}
}
int QMaterDataWig::chgTheNameToInt(QString strRef)
{
    if (strRef=="Density" ||strRef=="Elastic"||strRef=="Plastic"||strRef=="Expansion"||
        strRef=="Conductivity"||strRef=="SpecificHeat"||strRef=="MetalPowder"||
		strRef=="RateDependent"||strRef=="Damage"||strRef=="DynamicRecrystallization"){return 1;
    }else if (strRef=="密度"){return 10;
    }else if (strRef=="温度_c"){return 999;
    }else if (strRef=="杨氏模量"){return 20;
    }else if (strRef=="泊松比"){return 21;
    }else if (strRef=="屈服应力_Pa_"){return 30;
    }else if (strRef=="塑性应变"){return 31;
    }else if (strRef=="zero"){return 40;
    }else if (strRef=="热膨胀"){return 41;
    }else if (strRef=="导热"){return 50;
    }else if (strRef=="比热"){return 60;
    }else if (strRef=="参数"){return 70;
    }else if (strRef=="值"){return 71;
    }else if (strRef=="pb"){return 72;
    }else if (strRef=="相对密度"){return 73;
	}else if (strRef=="PlasticWork"){return 15;
	}else if(strRef=="HansenSpittel"||strRef=="CockroftLatham"||strRef=="NormalizedCL"||
		strRef=="McClintock"||strRef=="Freudenthal"||strRef=="RiceTracy"||
		strRef=="Oyane"||strRef=="Oyanenegative"||strRef=="Ayada"||
		strRef=="Ayadanegative"||strRef=="Osakada"||strRef=="Brozzo"||
		strRef=="ZhaoKuhn"||strRef=="MPSUTS"){return 14;
	}

	/*if (strRef=="Density" ||strRef=="Elastic"||strRef=="Plastic"||strRef=="Expansion"||
        strRef=="Conductivity"||strRef=="SpecificHeat"||strRef=="MetalPowder"||
		strRef=="RateDependent"||strRef=="Damage"||strRef=="DynamicRecrystallization"){return 1;
    }else if (strRef=="Mass_Density"){return 10;
    }else if (strRef=="Temp_c"){return 999;
    }else if (strRef=="Young-s_Modulus"){return 20;
    }else if (strRef=="Poisson-s_Ratio"){return 21;
    }else if (strRef=="Yield_Stress_Pa_"){return 30;
    }else if (strRef=="Plastic_Strain"){return 31;
    }else if (strRef=="zero"){return 40;
    }else if (strRef=="Expansion_Coeff_alpha"){return 41;
    }else if (strRef=="Conductivity_"){return 50;
    }else if (strRef=="Specific_Heat"){return 60;
    }else if (strRef=="Parameter"){return 70;
    }else if (strRef=="Value"){return 71;
    }else if (strRef=="pb"){return 72;
    }else if (strRef=="Relative_density"){return 73;
    }else if (strRef=="参数"){return 11;
    }else if (strRef=="值"){return 12;
	}else if (strRef=="PlasticWork"){return 15;
	}else if(strRef=="HansenSpittel"||strRef=="CockroftLatham"||strRef=="NormalizedCL"||
		strRef=="McClintock"||strRef=="Freudenthal"||strRef=="RiceTracy"||
		strRef=="Oyane"||strRef=="Oyanenegative"||strRef=="Ayada"||
		strRef=="Ayadanegative"||strRef=="Osakada"||strRef=="Brozzo"||
		strRef=="ZhaoKuhn"||strRef=="MPSUTS"){return 14;
	}*/
    return -1;
}

//--
void QMaterDataWig::ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap)
{
    // Each demo module can display a number of charts. We display all of them. 
    MaterialChart *d = materialCharts+0;//demoCharts + 2;//demoIndex;
    for (int tt=0;tt<d->noOfCharts;tt++)
    {
        imageMap = 0;
        chartViewers[tt]->setChart(d->createChart(dataX, dataY, &imageMap));
        chartViewers[tt]->setImageMap(imageMap);
        delete chartViewers[tt]->getChart();
    }
    // We hide unused chart viewers.
    for (int kk = 0; kk < noOfChartViewers; ++kk){
        chartViewers[kk]->setVisible(kk < d->noOfCharts);
    }    
    flowLayoutCharts();
}
void QMaterDataWig::ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap,_WELCME_CHART_PROP_ prop)
{
    // Each demo module can display a number of charts. We display all of them. 
    MaterialChart *d = materialCharts+0;//demoCharts + 2;//demoIndex;
    for (int tt=0;tt<d->noOfCharts;tt++)
    {
        imageMap = 0;
        chartViewers[tt]->setChart(d->propChart(dataX, dataY, &imageMap, prop));
        chartViewers[tt]->setImageMap(imageMap);
        delete chartViewers[tt]->getChart();
    }
    // We hide unused chart viewers.
    for (int kk = 0; kk < noOfChartViewers; ++kk){
        chartViewers[kk]->setVisible(kk < d->noOfCharts);
    }    
    flowLayoutCharts();
}
void QMaterDataWig::ChartShowDoubleY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap,_WELCME_CHART_PROP_ prop)
{
    // Each demo module can display a number of charts. We display all of them. 
    MaterialChart *d = materialCharts+2;//demoCharts + 2;//demoIndex;
    for (int tt=0;tt<d->noOfCharts;tt++)
    {
        imageMap = 0;
        chartViewers[tt]->setChart(d->propChart(dataX, dataY, &imageMap, prop));
        chartViewers[tt]->setImageMap(imageMap);
        delete chartViewers[tt]->getChart();
    }
    // We hide unused chart viewers.
    for (int kk = 0; kk < noOfChartViewers; ++kk){
        chartViewers[kk]->setVisible(kk < d->noOfCharts);
    }    
    flowLayoutCharts();
}
//--flow layout
void QMaterDataWig::flowLayoutCharts()
{
    
    const int margin = 5;

    // initial cursor position
    int cursorX = margin;
    int cursorY = margin;

    // initial page size
    int pageWidth = 0;
    int pageHeight = 0;

    // the width limit, beyond which wrapping will occur
    int wrapWidth = chartViewers[0]->parentWidget()->parentWidget()->width();

    for (int i = 0; i < noOfChartViewers; ++i)
    {
        // only layout visible charts
        if (!chartViewers[i]->isVisible())
            continue;

        int chartWidth =  chartViewers[i]->width();
        int chartHeight = chartViewers[i]->height();

        if (cursorX + margin + chartWidth > wrapWidth)
        {
            // no enough position in current line, so move to next line
            cursorX = margin;
            cursorY = pageHeight + margin;
        }

        // put chart in current line
        chartViewers[i]->setGeometry(cursorX, cursorY, chartWidth, chartHeight);

        // advance cursor
        cursorX += chartWidth + margin;

        if (pageWidth < cursorX)
            pageWidth = cursorX;
        if (pageHeight < cursorY + chartHeight)
            pageHeight = cursorY + chartHeight;
    }

    // resize the container to the proper size
    chartViewers[0]->parentWidget()->setFixedSize(pageWidth, pageHeight);
    
}

void QMaterDataWig::WriteImportDataToMFile(QString m_NowFileName, QMap<int,MCompoment> Mdata)
{
    //throw std::logic_error("The method or operation is not implemented.");
     MFileData.insert(m_NowFileName, Mdata);//存入数据
}

//--将文件中Inp数据写入到Mdata中;