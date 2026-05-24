#include "QFrdDataPro.h"
#include "Information_Widget.h"
#include <qmath.h>
#include <QDateTime>
#include "jacobi_eigenvalue.h"
QFrdDataPro::QFrdDataPro()
{

}

QFrdDataPro::~QFrdDataPro()
{
	m_resultFrdsID.clear();
}
/*-----------------------
  描述：读取Frd文件  
-----------------------*/
bool QFrdDataPro::ReadFileData(FrdDataVIS &FrdVis,QString fileName,resultFrdS &DataFrdShow)
{
    QFile inFile(fileName);
    if(!inFile.exists())
    {
        return false;
    }
    QStringList lists = fileName.split(".");
    QString fileFormat = lists.back();//frd文件

    if (!inFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    Information_Widget *infoW = Information_Widget::GetInstance();
    QDateTime date = QDateTime::currentDateTime();
    QString strTime;
    strTime.append(date.toString("hh:mm:ss yy.MM.dd"));
    infoW->ShowInformation("Start Time: "+strTime);
    //--Clear Vector
    //DataFrdShow.clearStr();
    //
    QTextStream inText(&inFile);
    QString line,strlineTmp;
    //resultFrdS DataFrdShow;
    while(!inText.atEnd())
    {
        line=inText.readLine().simplified();
        if (line.size()>0)
        {
            break;
        }
    }
    QString strId,strTmp;int strFormat;
    QStringList strNodeTmp,strTmpList, strList;
    int nNum1=0,nNum2=0;
    //DataFrdShow.elemBlock.DataElem2.reserve(10000);
    //DataFrdShow.nodalRultBlocks.strComptHeaderName.reserve(1000000);
    //DataFrdShow.nodalRultBlocks.strDataRecord.reserve(1000000);

    if (!inText.atEnd())
	{
        //-模型区信息
        strId=line.mid(0,2);
        if (strId=="1C"){//1C
            DataFrdShow.modelHeader.strName=line.mid(2);
            line=inText.readLine().simplified();
            infoW->ShowInformation("Start Frd File Read:  Find Header...");
        }else{
            infoW->ShowInformation("Not FRD Format");
            return false;//not frd MODEL format
        }
        //-用户区信息
        strId=line.mid(0,2);
        if (strId != "1U"){//1U
            infoW->ShowInformation("Not User Header...");
            //return false;
        }
        while(strId != "2C"){//2C
            if (strId=="1U"){
                strTmp=line.mid(2);
                int indexStr=strTmp.indexOf(" ");
                DataFrdShow.userHeader.strName <<strTmp.mid(0,indexStr);
               /* if (indexStr<0){
                    DataFrdShow.userHeader.strNameDetail <<" ";
                }else{
                    DataFrdShow.userHeader.strNameDetail <<strTmp.mid(indexStr);
                }	*/
                line=inText.readLine().simplified();
                strId = line.mid(0,2);
            }else{
                infoW->ShowInformation("Not User Header...");
                return false;
            }
        }
        //infoW->ShowInformation("Find User Header...");

        //Nodal Point
        //NodalPointXYZ tmpNodePoint;
        NodalPointXYZ *tmpNodePoint = new NodalPointXYZ;
        
        line.replace(QString(" "),QString(","));
        strList=line.split(",");
        if (strList.size()>=2){
            //DataFrdShow.nodalPoint.nNum=strList.at(1).toInt();
            strFormat = strList.at(2).toInt();
            //DataFrdShow.nodalPoint.nFormat=strList.at(2).toInt();
        }else{
            infoW->ShowInformation("Only Read 2C ,no others...");
        }

        line=inText.readLine();
        readNextPrcDataLine(line,strId,strList,strFormat);
        while ((strId != "-3") && (strId == "-1"))
        {
            tmpNodePoint->PointId<<strList.at(0).toInt();
            tmpNodePoint->x<<strList.at(1).toDouble();
            tmpNodePoint->y<<strList.at(2).toDouble();
            tmpNodePoint->z<<strList.at(3).toDouble();
            line=inText.readLine();//.simplified();
            readNextPrcDataLine(line,strId,strList,strFormat);
        }
        //infoW->ShowInformation("Find Nodal Point...");
        FrdVis.Step1_LoadFrdPoints(tmpNodePoint);
        delete tmpNodePoint;


        //Element Block
        elemBlockS *tmpElement = new elemBlockS;
        line=inText.readLine().simplified();
        readNextLine(line,strId,strList);
        if (strId=="3C")
        {
            tmpElement->nNum=strList.at(1).toInt();
            tmpElement->nFormat=strList.at(2).toInt();
        }
        line=inText.readLine().simplified();
        readNextLine(line,strId,strList);
        
        while(strId != "-3")
        {
            if (strId == "-1")
            {
                //DataFrdShow.elemBlock.DataElem1PointId<<strList.at(1).toInt();
                //DataFrdShow.elemBlock.DataElem1PointStyle<<strList.at(2).toInt();
                //DataFrdShow.elemBlock.DataElem1PointGrps<<strList.at(3).toInt();
                //DataFrdShow.elemBlock.DataElem1PointMats<<strList.at(4).toInt();
                tmpElement->DataElem1PointId<<strList.at(1).toInt();
                tmpElement->DataElem1PointStyle<<strList.at(2).toInt();
                tmpElement->DataElem1PointGrps<<strList.at(3).toInt();
                tmpElement->DataElem1PointMats<<strList.at(4).toInt();
                line=inText.readLine().simplified();
                readNextLine(line,strId,strList);
            }
            
            if (strId == "-2")
            {
                if (tmpElement->DataElem1PointStyle[0]==4||tmpElement->DataElem1PointStyle[0]==5)//超过10个节点为两行
                {
                    line=inText.readLine().simplified();
                    readNextLine(line,strId,strNodeTmp);
                    nNum1=strList.size();
                    nNum2=strNodeTmp.size();
                    
                    int *d = new int[nNum1-1+nNum2-1];
                    for (int kk=1;kk<nNum1;kk++){
                        d[kk-1]=strList.at(kk).toInt();
                    }
                    for (int kk=1;kk<nNum2;kk++){
                        d[nNum1-1+kk-1]=strNodeTmp.at(kk).toInt();
                    }
      
                 tmpElement->DataElem2.append(d);
                
                }else{ 
                    nNum1=strList.size();
                    int *d=new int[nNum1-1];
                    for (int kk=1;kk<nNum1;kk++){
                        d[kk-1]=strList.at(kk).toInt();
                    }
                    tmpElement->DataElem2.append(d);  
                }
                line=inText.readLine().simplified();
                readNextLine(line,strId,strList);
            }
        }
        FrdVis.Step2_LoadFrdCells(tmpElement);
        delete tmpElement;
        //qDeleteAll(DataFrdShow.elemBlock.DataElem2);
        //DataFrdShow.elemBlock.DataElem2.clear();

        //Param Header<1PTEST>
        line=inText.readLine().simplified();
        readNextLine(line,strId,strList);
        
        QString strHeaderParam;
        /*QString preStrHeader="";//L101
        while (line !="9999")
        {
            QCoreApplication::processEvents();
            line=inText.readLine().simplified();
            readNextLine(line,strId,strList);
        }*/
        while(line != "9999"&& !inText.atEnd()){//999
            QCoreApplication::processEvents();
            if (strId == "1P"){//1P
                resultStepFrdS tmpStepFrd;    
                //m_resultFrdsID.userHeader.strName <<strTmp.mid(0,indexStr);
                //tmpStepFrd.paraHeader.strName<<line.mid(2,line.indexOf(" ")-1);//STEP
                //tmpStepFrd.paraHeader.strHeadpId<<strList;//strList.at(1);//.toLocal8Bit().constData();
                tmpStepFrd.paraHeader.strInc=strList.at(2).toInt();
                tmpStepFrd.paraHeader.strStep=strList.at(3).toInt();
                line=inText.readLine().simplified();
                line.replace(QString(" "),QString(","));
                strId = line.mid(0,4);
                strList=line.split(",");

                if (strId == "100C"){//100C
                    strTmp=strList.at(0);
                    strTmp=strTmp.remove("100C")+strList.at(1);//.;line.mid(4,line.indexOf(",")).remove(",");//L101
                    strHeaderParam = strTmp;
                    //if (preStrHeader != ""){
                    ////发送数据给vtk进行处理，清空map，保存当前heade
                    //    //emit(m_sendDataMap);
                    //    preStrHeader = strHeaderParam;
                    //    m_sendDataMap.clear();
                    //}
                    tmpStepFrd.nodeResultBlock.strName=strHeaderParam;
                    QString st=strList.at(2);
                    tmpStepFrd.nodeResultBlock.strTimeVal=st.toDouble();
                    tmpStepFrd.nodeResultBlock.strNum=strList.at(3).toUInt();//.toLocal8Bit().constData();
                    //tmpStepFrd.nodeResultBlock.strAnalysisType<<strList.at(4).toShort();//.toLocal8Bit().constData();
                    //tmpStepFrd.nodeResultBlock.strStepNum<<strList.at(5).toInt();//toLocal8Bit().constData();
                    strFormat=strList.at(6).toInt();//.toLocal8Bit().constData();
                    tmpStepFrd.nodeResultBlock.strFormat=strFormat;
                    line=inText.readLine().simplified();//-4
                    readNextLine(line, strId, strList);
                    //if (strId == "-4"){//-4
                    //    tmpStepFrd.nodeResultBlock.strAttrHeaderName=strList.at(1);//.toLocal8Bit().constData();
                    //    strHeaderParam+="-"+strList.at(1);
                    //    tmpStepFrd.nodeResultBlock.strAttrNum=strList.at(2).toInt();//.toLocal8Bit().constData();
                    //    
                    //    //tmpNodalResult.strAttrType<<strList.at(3);//.toLocal8Bit().constData();
                    //    line=inText.readLine().simplified();//-5
                    //    readNextLine(line,strId,strList);             
                    //}

					QString Striii="";
					if (strId =="-4"){//-4
						int St;
						 tmpStepFrd.nodeResultBlock.strAttrHeaderName=strList.at(1);//.toLocal8Bit().constData();
						 strHeaderParam+="-"+strList.at(1);
						if(strList.at(1)=="STRESS"){
							Striii=strList.at(1);
							St=strList.at(2).toInt()+5;//米塞斯应力、第一、二、三主应力和静水压力共5个
							tmpStepFrd.nodeResultBlock.strAttrNum=St;
						}else if(strList.at(1)=="TOSTRAIN"){
							Striii=strList.at(1);
							St=strList.at(2).toInt()+1;
							tmpStepFrd.nodeResultBlock.strAttrNum=St;
						}else{
							tmpStepFrd.nodeResultBlock.strAttrNum=strList.at(2).toInt();
						}
						//tmpNodalResult.strAttrType<<strList.at(3);//.toLocal8Bit().constData();
						line=inText.readLine().simplified();//-5
						readNextLine(line,strId,strList);
					}

                    while(strId =="-5"){//-5
                        tmpStepFrd.nodeResultBlock.strComptHeaderName<<strList.at(1);
                        line =inText.readLine();
                        strlineTmp=line.simplified();//-5
                        readNextLine(strlineTmp,strId,strList);
                        //return true;
                    }

					if(Striii=="STRESS"){
						for(int i=0;i<5;i++){
						QStringList strList12;
						QString strList1;
							if(i==0)strList1="-5 VMStress 1 4 1 3";
							else if(i==1)strList1="-5 PS1 1 4 1 3";
							else if(i==2)strList1="-5 PS2 1 4 1 3";
							else if(i==3)strList1="-5 PS3 1 4 1 3";
							else if(i==4)strList1="-5 HydS 1 4 1 3";
                        strList1.replace(QString(" "),QString(","));
					    strList12=strList1.split(",");
						//strList=strList12.split;
						tmpStepFrd.nodeResultBlock.strComptHeaderName<<strList12.at(1);
						}
					}else if(Striii=="TOSTRAIN"){
					    QStringList strList12;
						QString strList1;
						strList1="-5 VMStrain 1 4 1 3";
                        strList1.replace(QString(" "),QString(","));
					    strList12=strList1.split(",");
						tmpStepFrd.nodeResultBlock.strComptHeaderName<<strList12.at(1);
					}
                    while(strId != "-3"){
						QString S1,S0;
                        if (strId == "-1"){//-1
                            //readNextStepNewPrcDataLine(line,strId,iCurDataId ,d,1);
                            readNextPrcDataLine(line,strId,strTmpList,1);//处理数字为-1的这行数字
                            line=inText.readLine();
                            readNextPrcDataLine(line,strId,strList,1);

							int Numb=strList.size();
							if(Striii=="STRESS"){
								double equ[11],equ0[11],p3;
								for(int jj=0;jj<Numb-1;jj++){
                                    equ[jj]=strList.at(jj+1).toDouble();
									equ0[jj]=strTmpList.at(jj+1).toDouble();
								}
								equ[6]=1.0/qSqrt(2.0)*qSqrt(qPow(equ[0]-equ[1],2)+
									qPow(equ[1]-equ[2],2)+qPow(equ[2]-equ[0],2)+6*(equ[3]*equ[3]+equ[4]*equ[4]+equ[5]*equ[5]));
								equ0[6]=1.0/qSqrt(2.0)*qSqrt(qPow(equ0[0]-equ0[1],2)+
									qPow(equ0[1]-equ0[2],2)+qPow(equ0[2]-equ0[0],2)+6*(equ0[3]*equ0[3]+equ0[4]*equ0[4]+equ0[5]*equ0[5]));
                		    		
								int it_max=100;
								double v[9],d[3],v0[9],d0[3];
								int it_num,rot_num,it_num0,rot_num0;
								double a[9],a0[9];
								a0[0]=equ0[0];a0[1]=equ0[3];a0[2]=equ0[5];
								a0[3]=equ0[3];a0[4]=equ0[1];a0[5]=equ0[4];
								a0[6]=equ0[5];a0[7]=equ0[4];a0[8]=equ0[2];
								jacobi_eigenvalue ( 3, a0,it_max, v0, d0,it_num0,rot_num0);
								equ0[7]=d0[2];equ0[8]=d0[1];equ0[9]=d0[0];equ0[10]=(equ0[0]+equ0[1]+equ0[2])/3.0;
								a[0]=equ[0];a[1]=equ[3];a[2]=equ[5];
								a[3]=equ[3];a[4]=equ[1];a[5]=equ[4];
								a[6]=equ[5];a[7]=equ[4];a[8]=equ[2];
								jacobi_eigenvalue ( 3, a,it_max, v, d,it_num,rot_num);
								equ[7]=d[2];equ[8]=d[1];equ[9]=d[0];equ[10]=(equ[0]+equ[1]+equ[2])/3.0;
								for(int i=6;i<11;i++){
									S0=QString::number(equ0[i],'d',6);
									S1=QString::number(equ[i],'d',6);
								strTmpList.append(S0);
								strList.append(S1);
								}
							}else if(Striii=="TOSTRAIN"){
								double equ[7],equ0[7];
								for(int jj=0;jj<Numb-1;jj++){
                                   equ[jj]=strList.at(jj+1).toDouble();
								   equ0[jj]=strTmpList.at(jj+1).toDouble();
								}
                               equ[6]=2.0/3.0/qSqrt(2.0)*qSqrt(qPow(equ[0]-equ[1],2)+
									qPow(equ[1]-equ[2],2)+qPow(equ[2]-equ[0],2)+6*(equ[3]*equ[3]+equ[4]*equ[4]+equ[5]*equ[5]));
							   equ0[6]=2.0/3.0/qSqrt(2.0)*qSqrt(qPow(equ0[0]-equ0[1],2)+
									qPow(equ0[1]-equ0[2],2)+qPow(equ0[2]-equ0[0],2)+6*(equ0[3]*equ0[3]+equ0[4]*equ0[4]+equ0[5]*equ0[5]));
								S0=QString::number(equ0[6],'d',6);
								S1=QString::number(equ[6],'d',6);
								strTmpList.append(S0);
								strList.append(S1);
							}

                            if (strId == "-1"){
                                nNum1=strTmpList.size();
                                double *d = new double[nNum1-1];
                                for (int kk=1;kk<nNum1;kk++){
                                    d[kk-1]=strTmpList.at(kk).toDouble();
                                }
                                tmpStepFrd.nodeResultBlock.dataIndex<<strTmpList.at(0).toInt();
                                tmpStepFrd.nodeResultBlock.strDataRecord<<d;

                                nNum2=strList.size();
                                double *d1 = new double[nNum2-1];
                                for (int kk=1;kk<nNum2;kk++){
                                    d1[kk-1]=strList.at(kk).toDouble();
                                }
                                tmpStepFrd.nodeResultBlock.dataIndex<<strList.at(0).toInt();
                                tmpStepFrd.nodeResultBlock.strDataRecord<<d1; 
                                line=inText.readLine();
                                readNextPrcDataLine(line,strId,strList,1);
                            }else if(strId == "-2"){//-2
                                while(strId=="-2"){
                                    strTmpList<<strList;
                                    line=inText.readLine();
                                    readNextPrcDataLine(line,strId,strList,1);
                               }
                                nNum1=strTmpList.size();
                                double *d = new double[nNum1-1];
                                
                                for (int kk=1;kk<nNum1;kk++){
                                    d[kk-1]=strTmpList.at(kk).toDouble();
                                }
                               tmpStepFrd.nodeResultBlock.dataIndex<<strTmpList.at(0).toInt();
                               tmpStepFrd.nodeResultBlock.strDataRecord<<d;
                            }else{//-3
                                nNum1=strTmpList.size();
                                double *d = new double[nNum1-1];

                                for (int kk=1;kk<nNum1;kk++){
                                    d[kk-1]=strTmpList.at(kk).toDouble();
                                }
                                tmpStepFrd.nodeResultBlock.dataIndex<<strTmpList.at(0).toInt();
                                tmpStepFrd.nodeResultBlock.strDataRecord<<d;
                                line=inText.readLine().simplified();
                                readNextLine(line,strId,strList);
                                /*if (preStrHeader==""){
                                    preStrHeader=strHeaderParam.split("-").at(0);
                                }
                                m_sendDataMap.insert(strHeaderParam,tmpStepFrd);*/
                            //DataFrdShow.paraHeader.insert(DataFrdShow.paraHeader.size(),tmpParaHeader);
                            //DataFrdShow.nodalRultBlocks.insert(DataFrdShow.nodalRultBlocks.size(),tmpNodalResult);
                                FrdVis.Step3_LoadFrdResults(&tmpStepFrd);
                                break;
                            }
                        } 
                        if (strId == "-3")
                        {
                            FrdVis.Step3_LoadFrdResults(&tmpStepFrd);
                            line=inText.readLine().simplified();
                            readNextLine(line,strId,strList);
                            break;
                        }
                    }
                    if (strId == "9999"){
                        line=inText.readLine().simplified();
                        readNextLine(line,strId,strList);
                        //	infoW->ShowInformation("Read Next 1P...");
                    }
                    if (line == "9999"){
                        infoW->ShowInformation("Read Frd File End");
                        break;
                    }
                }else{
                    infoW->ShowInformation("Read 100C Error...");
                    break;
                }
            }//-3
        }//9999 end
    }
  
    date = QDateTime::currentDateTime();
    strTime.append(" ~ ");
    strTime.append(date.toString("hh:mm:ss yy.MM.dd"));
    infoW->ShowInformation("Process Time: "+strTime);
    inFile.close();
    //--
    return true;
}


void QFrdDataPro::readNextLine(QString line, QString &strId, QStringList &strList)
{
	line.replace(QString(" "),QString(","));
	strId = line.mid(0,2);
	strList=line.split(",");
}
/*
function:
description:
解析 “2.003*10E-213.005*10E-3”数据有合并的一组
*/
void QFrdDataPro::readNextPrcDataLine(QString line, QString &strId, QStringList &strList,int strFormat)
{
    strList.clear();
    int nSize = line.size();
    QString str;
    if (strFormat == 1)
    {
        strId = line.mid(1,2);
        //strId.replace(QString(" "),QString(""));
        str=line.mid(3,10);
        str=str.simplified();//replace(QString(" "),QString(""));
        if (str!=""){
            strList<<str;
        }

        for(int i=13;i<nSize;i=i+12){
            str = line.mid(i,12);
            strList<<str.simplified();//replace(QString(" "),QString("")); 
        }
    }
    else if (strFormat ==0)
    {
        strId = line.mid(1,2);
        strId.replace(QString(" "),QString(""));
        str=line.mid(3,5);
        strList<<str.replace(QString(" "),QString(""));
        for(int i=8;i<nSize;i=i+12){
            str = line.mid(i,12);
            strList<<str.replace(QString(" "),QString("")); 
        }
    }
    else if (strFormat ==2)
    {
        strId="";
    }

}

/*-----------------------
  描述：读取dat文件  
-----------------------*/
bool QFrdDataPro::ReadFileDat(QFile *file)
{ 
    Information_Widget *infoW = Information_Widget::GetInstance();
    QDateTime date = QDateTime::currentDateTime();
    QString strTime;
    strTime.append(date.toString("hh:mm:ss yy.MM.dd"));
    infoW->ShowInformation("Start Time: "+strTime);
	resultDatS datS;
    QTextStream inText(file);
    QString line,strId;
    while(!inText.atEnd())
	{
		QString strList,Sname;
        line=inText.readLine().simplified();
        if (line.size()>0)
        {
			readNextLine2(line,strId,strList,Sname);
			if(strId=="1"){
				datS.SetName<<Sname;
				datS.SetTime<<strList;
			}else if(strId=="2"){
				datS.Force<<strList;
			}
        }
		strList.clear();
    }
    datS.SetName.removeDuplicates();
	m_resultDat=datS;
    date = QDateTime::currentDateTime();
    strTime.append(" ~ ");
    strTime.append(date.toString("hh:mm:ss yy.MM.dd"));
    infoW->ShowInformation("Process Time: "+strTime);
    return true;
}

void QFrdDataPro::readNextLine2(QString line, QString &strId, QString &strList,QString &Sname)
{
	QStringList strList1;
	strList1=line.split(" ");
	if(strList1.size()>3){
		strId="1";
		strList=strList1.at(5)+","+strList1.at(8);
		Sname=strList1.at(5);
	}else{
		strId="2";
		strList=strList1.at(0)+","+strList1.at(1)+","+strList1.at(2);
	}
}