#ifndef _INPDATASOURCE_H
#define _INPDATASOURCE_H

#include <vector>
#include <map>
using namespace std;
#include <QString>
#include <QStringList>
#include "CRWManage.h"

class vtkDoubleArray;
class vtkUnstructuredGrid;
class vtkVISUnstructuredGridSource;
class vtkCell;
//struct 单元集合对应的单元数据;

typedef struct _ELEMENT_SURF_{
    int id;//当前分配的Id号码
    int Sn;//第几个面片
    QList<int> data;//对应的data
    QList<int> dataSort;
    void clean()
    {
        id=0;Sn=0;
        data.clear();
        dataSort.clear();
    }
}ElementSufS;

typedef struct _DECODER_GENERATE_{
    int iDataGenerate;//"0:Generate(有间隔);1:“”(无间隔)"
    QStringList strDataList;
}DecodeGenerateS;

typedef struct _SURFACE_CHOOSE_{
    QString strNElementName;//_press_s4,_press_s3,_press_s2,_press_s1
    QString strNFaceName;//s4,s3,s2,s1
}DecodeSurfaceS;
class InpDataSource
{
public:
    InpDataSource();
    ~InpDataSource();

    bool InitGridFromFrd(ReadInpResultS *Inp);
    bool InitBCFromFrd(ReadInpResultS *Inp);
    bool SetInpDataSource(const QString &header = "");
    vtkUnstructuredGrid* GetWholeData(QString str);
    vtkUnstructuredGrid* GetSurfGridData();
    vtkVISUnstructuredGridSource* GetSourceGrid();
    vtkVISUnstructuredGridSource* GetNSetSourceGrid();
    vtkVISUnstructuredGridSource* GetElSourceGrid();
    vtkVISUnstructuredGridSource* GetSurfaceSourceGrid();
    bool SetInpNSetDataSource(const QString &header);
    bool SetInpElSetDataSource(const QString &header);
    bool SetInpSurfDataSource(const QString &header);
    //bool IsScalarValuesDiff(const QString &scalar);

    //--InpSurf获取   
    bool InpRowDataToSurf(ReadInpResultS *Inp);
    const QMap<QString,QString>* GetElementSurfFilterData();
public:
	int n2D3D;
    int m_iCelltype;//"CAX4,C3D6..."
	int CellBaseId_;//ELement not stard from id=1；
	QMap<int, int> InpPointIdMap_;
    QMap<QString, DecodeGenerateS> varNSetMap_;//点集合数据
    QMap<QString, DecodeGenerateS> varElSetMap_;//单元集合数据
    QMap<QString,vtkUnstructuredGrid*> disBCSourceMap_;//点数据集合;
    QMap<QString,vtkUnstructuredGrid*> disELSourceMap_;//单元数据集合;
    QMap<QString, QStringList> varSurfaceSetMap_;//表面集合
    QMap<QString,vtkUnstructuredGrid*> disSurfSourceMap_;//表面据集合;
	
private:
    int getInpCellType(QString strType);//获取inp中单元的类型，进行分类
    vtkUnstructuredGrid* BCDisplacementGrid(const QString &header);
    vtkUnstructuredGrid* ELDisplacementGrid(const QString &header);
    vtkUnstructuredGrid* SurfDisplacementGrid(const QString &header);

    bool GetNCellData(QStringList *tmpStrList, QString header);
    bool GetElCellData(QStringList *CellListData, QString header);
    bool GetSurfCellData(QVector<QStringList> *CellListData,QStringList *CellNewStyle,QString header);
    bool GetChooseData(QStringList *strL,QString Sn);
    vtkCell* NewVTKCellS(int InpCellType, QString strFace);
    void Del3DSameFace(int iType,QStringList data);
private:
    vtkUnstructuredGrid *wholeGrid_;

    vtkVISUnstructuredGridSource *gSource_;
    vtkVISUnstructuredGridSource *gBCSource_;//边界中点集合
    vtkVISUnstructuredGridSource *gELSource_;//材料中单元集合
    vtkVISUnstructuredGridSource *gSurfaceSource_;//材料中表面集合

    //vector<vtkUnstructuredGrid*> dispGridVec_;
  
    QMap<int,QString> m_ElementData;


    //--表面集合独立提取
    vtkUnstructuredGrid *wholeSurfGrid_;
	//<QString ,vtkUnstructuredGrid> 
    //QMap<int, QMap<int,int*> >  m_ElementSurfmap;
    //QMap<int, QMap<int,ElementSufS> > m_ElementSurfmap;
    //--QString(sort[3,1,2,4]) ,QString(OldElementId,Sn,itype(cell type),No sort Data)
    QMap<QString, QString> m_ElementSurfmap;
};

#endif // INPDATASOURCE_H
