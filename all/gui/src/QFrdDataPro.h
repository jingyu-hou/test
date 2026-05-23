#ifndef QFRDDATAPRO_H
#define QFRDDATAPRO_H
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <vector>
#include "ADefineQ.h"
#include <QCoreApplication>
#include  "PostProcess/FrdDataVIS.h"
using namespace std;



//--Frd文件
typedef struct _PointXYZ_S{
    QList<int> PointId;
    QVector<double> x;
    QVector<double> y;
    QVector<double> z;  
   /* _PointXYZ_S(){
        PointId.clear();
        x.clear();
        y.clear();
        z.clear();
    }*/
}NodalPointXYZ;
//typedef struct _ELEM_NODE_S{
//	vector<QStringList> Enode;
//}ElemNodalS;
typedef struct _MODELERESULT_S_{
    //QString strId;
    QString strName;
    _MODELERESULT_S_(){
        //	strId="1C";
        strName="";
    }
    //void clear(){
    //	strId.clear();
    //	strName.clear();
    //}
}modelHeaderS;

typedef struct _USERESULT_S_{
    //QString strId;
    QStringList strName;
    //QStringList strNameDetail; 
    //_USERESULT_S_(){
    //	strId="1U";
    //}
    //void clear(){
    //strId.clear();
    //strName.clear();
    //}
}userHeaderS;

typedef struct _NODALRESULT_S_{ 
    //QString strId;
    //QString strNum;
    //QString strFormat;
    int nNum;
    int nFormat;
    //QString strStartNodal;
    QList<int> DataId;
    NodalPointXYZ Data;
    //QString strEnd;
    _NODALRESULT_S_(){
        //strId="2C";
        nNum=1000;
        nFormat=1;//0:short/1:long/2:binary(coordinaters float)/3:binary(coordinaters double)
        //strEnd = "-3";
        //strStartNodal = "-1";
    }
}nodalPointS;
typedef struct _RESULT_ELEM_S 
{
    //QString strID;
    //QString strNum;
    //QString strFormat;
    int  nNum;
    int  nFormat;
    //QString strStartElem1;
    //QString strStartElem2;

    QList<int> DataElem1PointId;   //单元编号
    QList<int> DataElem1PointStyle;//单元类型
    QList<int> DataElem1PointGrps; //单元群组
    QList<int> DataElem1PointMats; //单元材料编号(目前按单元号进行显示隐藏)
    //ElemNodalS DataElem2;//单元包含的所有节点及顺序
    QList<int*> DataElem2;
    //QString strEnd;
    _RESULT_ELEM_S(){
        //strID="3C";
        nNum =1000;
        nFormat=1;
        //strEnd ="-3";
        //strStartElem1="-1";
        //strStartElem2="-2";
        //DataElem2.reserve(1000000);
    }
    ~_RESULT_ELEM_S(){
        int isize =DataElem2.size();
        for (int k=0;k<isize;k++){
            delete[] DataElem2[k];
            DataElem2[k]=0;
        }   
    }
}elemBlockS;

typedef struct _PARAHEADS_{
    //QString strID;//1P
    //QStringList strName;//STEP
    //QStringList strHeadpId;//当前出现次数(value)
    //QStringList strInc;//
    //QStringList strStep;//载荷步
    int strInc;//
    int strStep;//载荷步
    _PARAHEADS_(){
        //strID="1P";
    }
}paraHeaderS;
typedef struct _NODALRESULTBLOCKS_{
    //QString strID;//100C
    QString strName;//L101
    double strTimeVal;
    int strNum;//总数据个数
    //QStringList strAnyText;//
    //QList<short> strAnalysisType;//类型分析0=staic，1=time step,2=freq.,3=load step,4=user named
    //QList<int> strStepNum;//stepNum
    short strFormat;//0=short;1=long;2=binary;


    //QString strAttrHeaderId;//-4"Attribute header record(Dataset)"
    QString strAttrHeaderName;//"Attribute header Name"--DISP
    int	strAttrNum;//strAttrHeaderName包含子部分个数
    //QList<short> strAttrType;//0=与材料无关；1=与材料相关；3=Element data at nodes(not used)
    //QString strComptHearderID;//-5
    //QStringList strComptHeaderName;//--D1/D2/D3/ALL...
    QStringList strComptHeaderName;//--D1/D2/D3/ALL...
    //QList<short> strComptType;//1=scalar;2=vector with 3componets;4=matrix;12=vector with 3 amplitudes/3phase-angles;14=tensor with...
    //QString strComptMenu;//1
    //QList<short> strComptInd;//When type=4,then(Column number);others SubComponet index or row number.
    //int nComptExist;//0=data provided/1=data to be calc/2...
    //QString strComptICName;//When type=2(1ALL)
    //QString strDataValId1;//-1
    //QString strDataValId2;//-2
    //QStringList strDataRecord;
    QList<int> dataIndex;
    QList<double*> strDataRecord;

    _NODALRESULTBLOCKS_(){
        //strID="100C";
        //strEnd="-3";
        //strFrdEnd="9999";
        //strAttrHeaderId="-4";
        //strComptHearderID="-5";
        //strComptMenu="1";
        //strDataValId1 ="-1";
        //strDataValId2 ="-2";
        //strComptHeaderName.reserve(1000000);
        //strDataRecord.reserve(1000000);
    }
    ~_NODALRESULTBLOCKS_(){
        int isize =strDataRecord.size();
        for (int k=0;k<isize;k++){
            delete[] strDataRecord[k];
            strDataRecord[k]=0;
        }   
    }
}nodalResultBlockS;

typedef struct _RESULT_S_ {
    modelHeaderS modelHeader;
    userHeaderS  userHeader;
    nodalPointS  nodalPoint;
    elemBlockS	 elemBlock;
    nodalResultBlockS nodeResultBlock;
    paraHeaderS       paraHeader;
    void clear(){
        //modelHeader.clear();
        //userHeader.clear();
    }
}resultFrdS;

typedef struct _RESULTS_Dat_{
	QStringList SetName;
	QList<QString> SetTime;//<集合+时间>
	QList<QString> Force;//数据
}resultDatS;

typedef struct _RESULT_STEP_S_ {
    paraHeaderS       paraHeader;
    nodalResultBlockS nodeResultBlock;
    void clear(){
    //modelHeader.clear();
    //userHeader.clear();
    }
}resultStepFrdS;
//------------------------------

class QFrdDataPro 
{
public:
    QFrdDataPro();
    ~QFrdDataPro();
public:
    resultFrdS m_resultFrdsID;
	resultDatS m_resultDat;
    bool ReadFileData(FrdDataVIS &T, QString fileName,resultFrdS &Data);
	bool ReadFileDat(QFile *file);
    void readNextLine(QString line ,QString &strId,QStringList &strList);
	void readNextLine2(QString line ,QString &strId,QString &strList,QString &Sname);
    void readNextPrcDataLine(QString line, QString &strId, QStringList &strList,int strFormat);
    //QMap<QString, resultStepFrdS> m_sendDataMap;
private:
    //signals:
    //void emitMapData(QMap<QString, resultStepFrdS>);
};

#endif // QRRDDATAPRO_H
