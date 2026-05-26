#include "QHIPsystemDlg.h"

QHIPsystemDlg::QHIPsystemDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("制度"));
    QWidget *Hwid1 = new QWidget(this);
    QHBoxLayout *Hlayout = new QHBoxLayout();
    m_NextHipSysBtn = new QPushButton(tr("下一步"));
    m_PreHipBtn = new QPushButton(tr("上一步"));
    m_DelBtn = new QPushButton(tr("删除"));
    
    Hlayout->addWidget(m_PreHipBtn);
    Hlayout->addWidget(m_NextHipSysBtn);
    Hlayout->addWidget(m_DelBtn);

    QVBoxLayout *VLayout = new QVBoxLayout();
    m_TableWiget = new QTableWidget();
    m_TableWiget->setColumnCount(3);
    m_TableWiget->setRowCount(2);
    m_TableWiget->verticalHeaderItem(1);
    QStringList strList;
    strList<<"载荷"<<"集合"<<"工艺曲线";
    m_TableWiget->setHorizontalHeaderLabels(strList);
	m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_TableWiget->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);

    //晶粒度尺寸
    QTableWidgetItem *tmpItem=new QTableWidgetItem(tr("温度"));
    m_TableWiget->setItem(0,0,tmpItem);
    MyCombox *comboxPSet0=new MyCombox(this,0);
    //comboxPSet0->addItem("Point Set1");//("temperature_load");//("Point Set1");
    //comboxPSet0->addItem("Point Set2");//("pressure_load");//("Point Set2");
    comboxPSet0->setFrame(false);
    m_TableWiget->setCellWidget(0,1,comboxPSet0);
    m_comboxPSetSurf1.insert(0,comboxPSet0);//push_back(comboxPSet0);

    MyCombox *tmpCurve=new MyCombox(this,0);
   // m_comboxCurveNew[0]=new MyCombox(this,0);
    //m_comboxCurveNew[0]->addItem("曲线1");
    //m_comboxCurveNew[0]->addItem("曲线2");
    tmpCurve->addItem("新建");
    tmpCurve->setFrame(false);
    m_TableWiget->setCellWidget(0,2,tmpCurve);
    m_comboxCurveNew.insert(0,tmpCurve);
    connect(tmpCurve,SIGNAL(clicked(int,int)),this,SLOT(comBoxSlot()));
    //connect(m_comboxCurveNew[0],SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxEditSlot(int,int)));
    //晶粒度
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("压力"));
    m_TableWiget->setItem(1,0,tmpItem2);
    MyCombox *comboxPSet1=new MyCombox(this,1);
    //comboxPSet1->addItem("Point Set1");//("temperature_load");//("Point Set1");
    //comboxPSet1->addItem("Point Set2");//("pressure_load");//("Point Set2");
    comboxPSet1->setFrame(false);
    m_TableWiget->setCellWidget(1,1,comboxPSet1);
    m_comboxPSetSurf1.insert(0,comboxPSet1);//push_back(comboxPSet1);

    tmpCurve=new MyCombox(this,1);
    //m_comboxCurveNew[1]->addItem("曲线1");
    //m_comboxCurveNew[1]->addItem("曲线2");
    tmpCurve->addItem("新建");
    tmpCurve->setFrame(false);
    m_TableWiget->setCellWidget(1,2,tmpCurve);
    m_comboxCurveNew.insert(1,tmpCurve);
    VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout);
    setLayout(MainLayout);
    this->setMinimumWidth(350);
    connect(m_NextHipSysBtn,SIGNAL(clicked()),this,SLOT(NextHipSysBtnSlot()));
    connect(m_PreHipBtn,SIGNAL(clicked()),this,SLOT(PreHipSysBtnSlot()));
    connect(m_comboxCurveNew[1],SIGNAL(clicked(int,int)),this,SLOT(comBoxSlot()));
    m_DelBtn->hide();
}

QHIPsystemDlg::~QHIPsystemDlg()
{

}
//Row:combox id ; index:current id
void QHIPsystemDlg::NewCurve(int Row,int index)      
{
    QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
    int ret = tmpCurve->exec();   
    if (ret == QDialog::Accepted){//ok
        QString strName=tmpCurve->m_HIPCurveNode.strName;
        if (strName == ""){return;}
        m_CurveDataDlg.insert(strName,tmpCurve);
        m_comboxCurveNew[Row]->insertItem(index, strName);
        m_comboxCurveNew[Row]->setCurrentIndex(index);
        //--获取整个comboxList   
        QStringList strListName;
        for (int mm=0;mm<m_comboxCurveNew[Row]->count();mm++){
            QString strT=m_comboxCurveNew[Row]->itemText(mm).toUpper();
            if (strT=="新建")
            {continue;}  
            strListName<< m_comboxCurveNew[Row]->itemText(mm);
        }
        emitHIPCurveData(strListName, tmpCurve->m_HIPCurveNode);//更新整个Curve(全局),同步热处理数据
        emitUpDataCurveTreeName(strName,strListName);
        //--其他COMBOXList均更新
        int nTabRowCnt=m_TableWiget->rowCount();
        int NexIndex;
        for(int kk=0; kk<nTabRowCnt; kk++){
            NexIndex=m_comboxCurveNew[kk]->findText("新建");
            if (kk != Row){
                m_comboxCurveNew[kk]->insertItem(index,strName);
            }
        }
    }else{//reject

    }
}
//Row:combox id ; index:current id
void QHIPsystemDlg::DelCurve(QString strName)      
{
    if(!m_CurveDataDlg.contains(strName))return;
    m_CurveDataDlg.remove(strName);

    QStringList strListName;
    int nRow=m_TableWiget->rowCount();

    for (int mm=0; mm<nRow; mm++){
        MyCombox *tmpBtn=(MyCombox*)m_TableWiget->cellWidget(mm,2);
        strListName.clear();
        for (int tt=0;tt<tmpBtn->count();tt++){
            if (tmpBtn->itemText(tt).toUpper()=="新建"){continue;}
            if (tmpBtn->itemText(tt)==strName){
                tmpBtn->removeItem(tt);
            }
            if (tmpBtn->itemText(tt).toUpper()=="新建"){continue;}//remove后，判断
            strListName<< tmpBtn->itemText(tt);                
        }  
    }
    //emitHIPCurveData(strListName,m_CurveDataDlg[strName]->m_HIPCurveNode);
     InpCurveInpS tmpCurveNode;
    emitHIPCurveData(strListName,tmpCurveNode);
    emitUpDataCurveTreeName("AmpCurve", strListName);//tree中名字全部更新  

}
//--Tree界面中曲线显示
void QHIPsystemDlg::updataFromTreeSlot(QString strName)
{
    if(!m_CurveDataDlg.contains(strName))return;
    QString strNowName="";
    if(m_CurveDataDlg[strName]->exec()){
        strNowName=m_CurveDataDlg[strName]->m_CurveName->text();

        QStringList strListName;
        int nRow=m_TableWiget->rowCount();
       
        for (int mm=0; mm<nRow; mm++){
             MyCombox *tmpBtn=(MyCombox*)m_TableWiget->cellWidget(mm,2);
             for (int tt=0;tt<tmpBtn->count();tt++){
                 if (tmpBtn->itemText(tt).toUpper()=="新建"){continue;}
                 if (tmpBtn->itemText(tt)==strName){
                     tmpBtn->setItemText(tt,strNowName);
                 }
                 strListName<< tmpBtn->itemText(tt);                
             }  
             m_CurveDataDlg.insert(strNowName,m_CurveDataDlg[strName]);
             break;
        }
        emitHIPCurveData(strListName,m_CurveDataDlg[strName]->m_HIPCurveNode);
        emitUpDataCurveTreeName("AmpCurve", strListName);//tree中名字全部更新   
    } 
}
//--双击进入编辑界面
//void QHIPsystemDlg::comBoxEditSlot(int comWigId, int index)
//{
   // QString strIdName;
   //  int Row = comWidId;
   // if (Row <0){return;}
   // strIdName =  m_comboxCurveNew[Row]->currentText();
//}
//--保存热等静压制度及曲线数据
void QHIPsystemDlg::PreHipSysBtnSlot()
{
   WriteHIPsysToInp();
   this->accept();
   
}
void QHIPsystemDlg::NextHipSysBtnSlot()
{
	WriteHIPsysToInp();
	this->accept();

}
void QHIPsystemDlg::WriteHIPsysToInp()
{
    HIPSystemInpS   nodeList;
    InpCurveInpS    nodeCurveList;
    int TabRowCnt = m_TableWiget->rowCount();
    int TabColCnt = m_TableWiget->columnCount();

    if (TabRowCnt>0){m_nodeHIPSystemList.clear();}

    for(int i=0; i<TabRowCnt; i++){
        nodeList.strCommentaryName=m_TableWiget->item(i,0)->text();
        nodeList.strAmplitudeName=m_comboxCurveNew[i]->currentText();
        nodeList.strPsetName=((MyCombox*)(m_TableWiget->cellWidget(i, 1)))->currentText();//单元//m_comboxPSetSurf1[i]->currentText();
        if (nodeList.strCommentaryName=="温度"){
            nodeList.strStyle = "Boundary";
        }else if(nodeList.strCommentaryName=="压力"){
            nodeList.strStyle = "Dsload";
        }
        nodeList.strCofficient="1";
		nodeList.SolutionName="5";
        m_nodeHIPSystemList.append(nodeList);
    }

    int nCurCnt = m_comboxCurveNew[1]->count()-1;//除去新建的
    if (nCurCnt>0){m_nodeCurveList.clear();}
    MyCombox *tmp=(MyCombox*)(m_TableWiget->cellWidget(0, 2));
    for (int i=0;i<tmp->count();i++){
        QString strName= tmp->itemText(i);
        if (strName.toUpper()=="新建")continue;
        nodeCurveList=m_CurveDataDlg[strName]->m_HIPCurveNode;
        m_nodeCurveList.append(nodeCurveList);
    }
   
}
//将读取的INP数据显示在当前的表格中
void QHIPsystemDlg::SetInpData(ReadInpResultS ReadInpData)
{
    int nRowSize = m_TableWiget->rowCount();
    int nHipSysRowSize = ReadInpData.TmpHIPSystemInps.size();
    int nHipCurveSize=ReadInpData.TmpCurveInps.size();
	//if(nHipSysRowSize<=0)return;//多工步需要取消该注释
    if (nRowSize >= nHipSysRowSize)
    {
        QStringList PSurfSetList;
        //for (int kk=0; kk<nHipSysRowSize; kk++){
        //    PSetList<<ReadInpData.TmpHIPSystemInps.at(kk).strPsetName;
       // }
		PSurfSetList=ReadInpData.TmpNsetInps.strNSetName;
        int i=0,j=0;
        for (i=0;i<nRowSize;i++){
            //m_comboxPSetSurf1[i]->clear();
             MyCombox *tmpCombox =(MyCombox*)m_TableWiget->cellWidget(i, 1);
            tmpCombox->clear();
             if (m_TableWiget->item(i,0)->text()=="温度")//温度下面不包含surface
            {
                for (j=0;j<PSurfSetList.size();j++){
                    tmpCombox->addItem(PSurfSetList.at(j));
                    tmpCombox->setItemData(j,"0",(Qt::UserRole+3));
                }
            }else{//压力下面不包含节点NSet
                for (int j=0;j<ReadInpData.TmpSurfaceInps.strSurfaceName.size();j++){
                    if (ReadInpData.TmpSurfaceInps.strSurfaceName.at(j)!=""){
                        tmpCombox->addItem(ReadInpData.TmpSurfaceInps.strSurfaceName.at(j));
                        tmpCombox->setItemData(j,"2",(Qt::UserRole+3));
                    }
                } 
            }
            tmpCombox->addItem("新建"); 
            m_comboxPSetSurf1.insert(i,tmpCombox);
			connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxSlot()));
        }  

        QStringList curveList;
        for (int kk=0;kk<nHipCurveSize;kk++){
            curveList<<ReadInpData.TmpCurveInps.at(kk).strName.toUpper();
        }
        for (int i=0;i<nRowSize;i++){
            m_comboxCurveNew[i]->clear();
            m_comboxCurveNew[i]->addItems(curveList);
            m_comboxCurveNew[i]->addItem("新建");
        } 

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //多工步才用到以下内容
		for(int i=0;i<ReadInpData.TmpBCInps.size();i++){
			QString str=ReadInpData.TmpBCInps.at(i).SolutionName;
			QString strTmpName =ReadInpData.TmpBCInps.at(i).strPSetName;
            QString str1=ReadInpData.TmpBCInps.at(i).strURstyle;
			QString str2=ReadInpData.TmpBCInps.at(i).strAmplitudeName;
			//if(str=="5"&&str1=="11,11"){//多工步需要取消注释
			if(str1=="11,11"){//多工步需要删除该行
				int id=((MyCombox*)m_TableWiget->cellWidget(0, 1))->findText(strTmpName);
				((MyCombox*)m_TableWiget->cellWidget(0, 1))->setCurrentIndex(id); 
				id=m_comboxCurveNew[0]->findText(str2.toUpper());
				m_comboxCurveNew[0]->setCurrentIndex(id);
			}
		}
		for(int i=0;i<ReadInpData.TmpHIPSystemInps.size();i++){//--查找并显示
            QString strTmpName = ReadInpData.TmpHIPSystemInps.at(i).strPsetName;
			QString str=ReadInpData.TmpHIPSystemInps.at(i).SolutionName;
			QString str2 =ReadInpData.TmpHIPSystemInps.at(i).strAmplitudeName;
			//if(str=="5"){//多工步需要取消注释
				int id=((MyCombox*)m_TableWiget->cellWidget(1, 1))->findText(strTmpName);
				((MyCombox*)m_TableWiget->cellWidget(1, 1))->setCurrentIndex(id);
				id=m_comboxCurveNew[1]->findText(str2.toUpper());
				m_comboxCurveNew[1]->setCurrentIndex(id);
			//}//多工步需要取消注释
        }
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
    //--将Curve Data进行保存
    m_CurveDataDlg.clear();
      for (int jj=0; jj<m_comboxCurveNew[0]->count(); jj++){
        QString str=m_comboxCurveNew[0]->itemText(jj);
        if (str!="新建"){
            QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
            InpCurveInpS tmpCurData;
            tmpCurData.strName=str;
            tmpCurData.strDataX=ReadInpData.TmpCurveInps.at(jj).strDataX;
            tmpCurData.strDataY=ReadInpData.TmpCurveInps.at(jj).strDataY;
            tmpCurve->SetData(tmpCurData);
            m_CurveDataDlg.insert(str,tmpCurve);
        }
      }
 
     //WriteHIPsysToInp(); 

      HIPSystemInpS   nodeList;
      InpCurveInpS    nodeCurveList;
      int TabRowCnt = m_TableWiget->rowCount();
      int TabColCnt = m_TableWiget->columnCount();

      if (nHipSysRowSize!=0){//不进行写入
          if (TabRowCnt>0){m_nodeHIPSystemList.clear();}

          for(int i=0; i<TabRowCnt; i++){
              nodeList.strCommentaryName=m_TableWiget->item(i,0)->text();
              nodeList.strAmplitudeName=m_comboxCurveNew[i]->currentText();
              nodeList.strPsetName=((MyCombox*)m_TableWiget->cellWidget(i, 1))->currentText();
              if (nodeList.strCommentaryName=="温度"){
                  nodeList.strStyle = "Boundary";
              }else if(nodeList.strCommentaryName=="压力"){
                  nodeList.strStyle = "Dsload";
              }
              nodeList.strCofficient="1";

              m_nodeHIPSystemList.append(nodeList);
          }
      }
      int nCurCnt = m_comboxCurveNew[1]->count()-1;//除去新建的
      if (nCurCnt>0){m_nodeCurveList.clear();}
      //for (int i=0; i<nCurCnt; i++){
     /* for (int i=0; i<m_CurveDataDlg.size(); i++){
          nodeCurveList=m_CurveDataDlg.at(i)->m_HIPCurveNode; 
          m_nodeCurveList.append(nodeCurveList);
      }*/
      MyCombox *tmp=(MyCombox*)(m_TableWiget->cellWidget(0, 2));
      for (int i=0;i<tmp->count();i++){
          QString strName= tmp->itemText(i);
          if (strName.toUpper()=="新建")continue;
          nodeCurveList=m_CurveDataDlg[strName]->m_HIPCurveNode;
          m_nodeCurveList.append(nodeCurveList);
      }
}
//---发送高亮显示
void QHIPsystemDlg::comBoxSlot()
{
    MyCombox *btn=(MyCombox*) sender();
    int x=btn->mapToParent(QPoint(0,0)).x();
    int y=btn->mapToParent(QPoint(0,0)).y();
    QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));

    int iRow=index.row(); 
    int iCol=index.column();

    if (iRow<0 || iCol<0) return;
    MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(iRow, iCol);
    switch(iCol){
        case 1:{
            int id=m_comboxPSetSurf1.key(tmpBtn);
            if (id==-1)return;
            int ind= m_comboxPSetSurf1[id]->currentIndex();
            NElSurfChsS tmpChs;
            tmpChs.strName =m_comboxPSetSurf1[id]->currentText();
            tmpChs.iType = m_comboxPSetSurf1[id]->itemData(ind,Qt::UserRole+3).toInt();
            emit emitPsetHighLight(tmpChs);
           }break;
        case 2:{
            int id=m_comboxCurveNew.key(tmpBtn);
            if (id==-1)return;
            int ind= m_comboxCurveNew[id]->currentIndex();
            QString strIdName =  m_comboxCurveNew[id]->currentText();

            if (strIdName == "新建"){
                NewCurve(id,ind);
                return ;
            }

            //进行界面显示
            if (m_CurveDataDlg.contains(strIdName))
            {
                if(m_CurveDataDlg[strIdName]->exec()){
                   QStringList strListName;
                    for (int mm=0;mm<m_comboxCurveNew[id]->count();mm++){
                        if (m_comboxCurveNew[id]->itemText(mm).toUpper()=="新建"){continue;}
                        strListName<< m_comboxCurveNew[id]->itemText(mm);
                    }
                    emitHIPCurveData(strListName,m_CurveDataDlg[strIdName]->m_HIPCurveNode);
                } 
              }
            }break;
        default:break;
    }
}


//--来自HP中数据同步更新HIP
void QHIPsystemDlg::updateHIPCurveDataSlot(QStringList CurveNameList,InpCurveInpS CurveData)
{
    int nRow = m_TableWiget->rowCount();
    //m_nodeCurveList.clear();
    if (nRow>0){//获取当前对应的combox，进行数据写入并更新其他行；
        //bool bf=false;
        //for(int i=0;i<m_nodeCurveList.count();i++){
        //    if ( m_nodeCurveList.at(i).strName==CurveData.strName) {
        //         m_nodeCurveList.operator [](i)=CurveData;//修改数据
        //         //for (int tt=0;tt<m_CurveDataDlg.count();tt++){//
        //             //if (m_CurveDataDlg[tt]->m_CurveName->text()==CurveData.strName)
        //                 m_CurveDataDlg[CurveData.strName]->SetData(CurveData);     
        //         //}
        //         bf=true;break;      
        //    } 
        //}
        //if (!bf){//combox 新加一item及新数据
           // m_nodeCurveList = CurveNameList;
            //--combox更新
            for (int kk=0;kk<nRow;kk++){
                MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(kk, 2);//读取第一行，第三列的
                int ind=tmpBtn->currentIndex();
                QString strT=tmpBtn->currentText();
                tmpBtn->clear();
                tmpBtn->addItems(CurveNameList);
                tmpBtn->addItem("新建");
                if(strT==CurveData.strName){
                    tmpBtn->setCurrentIndex(ind);
                }else{
                    int tmpID=tmpBtn->findText(CurveData.strName);
                    tmpBtn->setCurrentIndex(tmpID);
                }
            }  
            //--
            QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);//curveDlg更新；
            tmpCurve->SetData(CurveData);
            m_CurveDataDlg.insert(CurveData.strName,tmpCurve);
            //--更新tree
            QStringList strListT;
            for (int kk=0;kk<nRow;kk++){
                MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(kk, 2);
                for (int tt=0;tt<tmpBtn->count();tt++){
                if (tmpBtn->itemText(tt).toUpper()=="新建")continue;
                strListT<<tmpBtn->itemText(tt);
                }
                break;     
            }
            emitUpDataCurveTreeName(CurveData.strName,strListT);
        //}  
    }else{//表中没有数据，更新读取到的inp中的 CURVE对应数值即可；

    }
    
}

//--Surface下更新“压力”下的表面集合;
void QHIPsystemDlg::AppendSurfSetComboxSlot(QString str)
{
    int nRow = m_TableWiget->rowCount();
    for (int i=0; i<nRow; i++)
    {
        MyCombox *TmpcomboxPset = (MyCombox*)m_TableWiget->cellWidget(i, 1);//new MyCombox(this, nRow);
        if (TmpcomboxPset->findText(str) == -1 && m_TableWiget->item(i,0)->text() == tr("压力"))
        {
            TmpcomboxPset->addItem(str); 
            int id=m_comboxPSetSurf1.key(TmpcomboxPset);
            int ind= m_comboxPSetSurf1[id]->findText(str);
            TmpcomboxPset->setItemData(ind, "2", (Qt::UserRole+3));
            //if (id==-1)//return;
           // {
                m_comboxPSetSurf1.insert(ind,TmpcomboxPset);
           // }
           
            
            
        }         
    }
}
