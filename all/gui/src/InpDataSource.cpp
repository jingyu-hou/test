#include "InpDataSource.h"
#include <vtkObjectFactory.h>
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkHexahedron.h>//1
#include <vtkWedge.h>//2
#include <vtkTetra.h>//3
#include <vtkQuadraticHexahedron.h>//4
#include <vtkQuadraticWedge.h>//5
#include <vtkQuadraticTetra.h>//6
#include <vtkTriangle.h>//7
#include <vtkQuadraticTriangle.h>//8
#include <vtkQuad.h>//9
#include <vtkQuadraticQuad.h>//10
#include <vtkUnstructuredGrid.h>
#include <vtkVISUnstructuredGridSource.h>
#include <vtkLine.h>//画线
#include <vtkPolyLine.h>
#include "CRWManage.h"

InpDataSource::InpDataSource()
{
    wholeGrid_ = 0;
    wholeSurfGrid_ = 0;
	CellBaseId_ = 0;
    gSource_ = vtkVISUnstructuredGridSource::New();
    gBCSource_ = vtkVISUnstructuredGridSource::New();
    gELSource_ = vtkVISUnstructuredGridSource::New();
    gSurfaceSource_ = vtkVISUnstructuredGridSource::New();
}

InpDataSource::~InpDataSource()
{
    if (wholeGrid_)
    {
        wholeGrid_->Delete();
        wholeGrid_ = 0;
    }
    if (wholeSurfGrid_)
    {
        wholeSurfGrid_->Delete();
        wholeSurfGrid_ = 0;
    }

    gSource_->Delete();
    gBCSource_->Delete();
    gELSource_->Delete();
    gSurfaceSource_->Delete();

    disELSourceMap_.clear();
    disSurfSourceMap_.clear();
    varElSetMap_.clear();
    varSurfaceSetMap_.clear();
    m_ElementData.clear();

        
    //varNSetMap_.clear();
    //varElSetMap_.clear();
    //varSurfaceSetMap_.clear();
    //m_ElementData.clear();
   
   /* for (vector<vtkUnstructuredGrid*>::iterator it = dispGridVec_.begin(); it != dispGridVec_.end(); ++it)
    {
        (*it)->Delete();
    }
    dispGridVec_.clear();*/
    disBCSourceMap_.clear();
    disELSourceMap_.clear();
    disSurfSourceMap_.clear();
    m_ElementSurfmap.clear();
	InpPointIdMap_.clear();
}
bool InpDataSource::InitGridFromFrd(ReadInpResultS *Inp)
{
    if (Inp == 0) return false;
    int nPoint = Inp->TmpNodeInpS.strData.size();
    int nCell = Inp->TmpElInpS.strData.size();
    if (nPoint <= 0 || nCell <= 0) return false;

    QString analysisType = Inp->TmpElInpS.strELType.mid(0, 2).toLower();
    if (analysisType.isEmpty() && Inp->TmpElInpS.ElementType.size() > 0) {
        analysisType = Inp->TmpElInpS.ElementType.at(0).mid(0, 2).toLower();
    }
    n2D3D = (analysisType == "c3") ? 3 : 2;

    int maxPointId = 0;
    for (int i = 0; i < nPoint; ++i) {
        QStringList nodeData = Inp->TmpNodeInpS.strData.at(i).split(",", QString::SkipEmptyParts);
        if (nodeData.size() > 0) {
            int nodeId = nodeData.at(0).simplified().toInt();
            if (nodeId > maxPointId) maxPointId = nodeId;
        }
    }
    if (maxPointId <= 0) return false;

    PointCoordinate pnt;
    vtkPoints *points = vtkPoints::New();
    points->SetNumberOfPoints(maxPointId);
    for (int i = 0; i < nPoint; ++i) {
        QStringList nodeData = Inp->TmpNodeInpS.strData.at(i).split(",", QString::SkipEmptyParts);
        if (nodeData.size() < 3) continue;
        int nodeId = nodeData.at(0).simplified().toInt();
        if (nodeId <= 0) continue;
        pnt.x = nodeData.at(1).simplified().toDouble();
        pnt.y = nodeData.at(2).simplified().toDouble();
        pnt.z = (n2D3D == 3 && nodeData.size() > 3) ? nodeData.at(3).simplified().toDouble() : 0.0;
        int vtkPointId = nodeId - 1;
        points->SetPoint(vtkPointId, pnt.x, pnt.y, pnt.z);
        InpPointIdMap_[nodeId] = vtkPointId;
    }

    wholeGrid_ = vtkUnstructuredGrid::New();
    wholeGrid_->Allocate(nCell);
    QList<QString> cellPointsList = Inp->TmpElInpS.strData;
    QStringList firstCell = cellPointsList.at(0).split(",", QString::SkipEmptyParts);
    CellBaseId_ = firstCell.size() > 0 ? firstCell.at(0).simplified().toInt() - 1 : 0;

    for (int i = 0; i < nCell; ++i) {
        QStringList cellData = cellPointsList.at(i).split(",", QString::SkipEmptyParts);
        int nodeNumber = cellData.size() - 1;
        int inpCellType = 0;
        if (n2D3D == 2) {
            switch (nodeNumber) {
            case 3: inpCellType = 7; break;
            case 4: inpCellType = 9; break;
            case 6: inpCellType = 8; break;
            case 8: inpCellType = 10; break;
            default: break;
            }
        } else {
            switch (nodeNumber) {
            case 4: inpCellType = 3; break;
            case 6: inpCellType = 2; break;
            case 8: inpCellType = 1; break;
            case 10: inpCellType = 6; break;
            case 15: inpCellType = 5; break;
            case 20: inpCellType = 4; break;
            default: break;
            }
        }

        vtkCell *cell = 0;
        switch (inpCellType) {
        case 1: cell = vtkHexahedron::New(); break;
        case 2: cell = vtkWedge::New(); break;
        case 3: cell = vtkTetra::New(); break;
        case 4: cell = vtkQuadraticHexahedron::New(); break;
        case 5: cell = vtkQuadraticWedge::New(); break;
        case 6: cell = vtkQuadraticTetra::New(); break;
        case 7: cell = vtkTriangle::New(); break;
        case 8: cell = vtkQuadraticTriangle::New(); break;
        case 9: cell = vtkQuad::New(); break;
        case 10: cell = vtkQuadraticQuad::New(); break;
        default: break;
        }
        if (!cell) continue;

        int cellTypeVtk = cell->GetCellType();
        int np = cell->GetNumberOfPoints();
        if (cellData.size() < np + 1) {
            cell->Delete();
            continue;
        }
        vtkIdList *idList = cell->GetPointIds();
        bool validCell = true;
        for (int j = 0; j < np; ++j) {
            int nodeId = cellData.at(j + 1).simplified().toInt();
            if (InpPointIdMap_.find(nodeId) == InpPointIdMap_.end()) {
                validCell = false;
                break;
            }
            idList->SetId(j, InpPointIdMap_[nodeId]);
        }
        if (validCell && cellTypeVtk == VTK_QUADRATIC_HEXAHEDRON) {
            int t12 = idList->GetId(12), t13 = idList->GetId(13), t14 = idList->GetId(14), t15 = idList->GetId(15);
            idList->SetId(12, idList->GetId(16)); idList->SetId(13, idList->GetId(17));
            idList->SetId(14, idList->GetId(18)); idList->SetId(15, idList->GetId(19));
            idList->SetId(16, t12); idList->SetId(17, t13); idList->SetId(18, t14); idList->SetId(19, t15);
        } else if (validCell && cellTypeVtk == VTK_QUADRATIC_WEDGE) {
            int t9 = idList->GetId(9), t10 = idList->GetId(10), t11 = idList->GetId(11);
            idList->SetId(9, idList->GetId(12)); idList->SetId(10, idList->GetId(13)); idList->SetId(11, idList->GetId(14));
            idList->SetId(12, t9); idList->SetId(13, t10); idList->SetId(14, t11);
        }
        if (validCell) wholeGrid_->InsertNextCell(cellTypeVtk, idList);
        cell->Delete();
    }
    wholeGrid_->SetPoints(points);
    points->Delete();
    return true;
}
vtkVISUnstructuredGridSource* InpDataSource::GetSourceGrid()
{
    return gSource_;
}
vtkUnstructuredGrid* InpDataSource::GetWholeData(QString str)
{
	if (str.isEmpty()){
		return wholeGrid_;//返回全部
	}else{
		if (disSurfSourceMap_.find(str)!=disSurfSourceMap_.end()){//have no
			return disBCSourceMap_[str];
		}
		return wholeGrid_;
	}
    
}
vtkUnstructuredGrid* InpDataSource::GetSurfGridData()
{
    return wholeSurfGrid_;
}
//--点集合
vtkVISUnstructuredGridSource* InpDataSource::GetNSetSourceGrid()
{
    return gBCSource_;
}
//--单元集合
vtkVISUnstructuredGridSource* InpDataSource::GetElSourceGrid()
{
    return gELSource_;
}
//--表面集合
vtkVISUnstructuredGridSource* InpDataSource::GetSurfaceSourceGrid()
{
    return gSurfaceSource_;
}
//--整个网格
bool InpDataSource::SetInpDataSource(const QString &header)
{
    if (header.isEmpty()){ //init grid
        gSource_->SetSourceGrid(wholeGrid_);
        return true;
    }else{ //displacement grid
        return false;
    }
}
//--获取点集合的grid;
bool InpDataSource::SetInpNSetDataSource(const QString &header)
{
   if (header.isEmpty()){ //init grid
       gBCSource_->SetSourceGrid(wholeGrid_);
       return true;
    }else{ //displacement grid
        if (disBCSourceMap_.find(header)==disBCSourceMap_.end()){
            vtkUnstructuredGrid *g = BCDisplacementGrid(header);
            if (g){
                disBCSourceMap_.insert(header,g);
                gBCSource_->SetSourceGrid(g);
                return true;
            }
            else return false;
        }
        gBCSource_->SetSourceGrid(disBCSourceMap_[header]);
        return true;
    }
}
//--获取单元集合的grid;
bool InpDataSource::SetInpElSetDataSource(const QString &header)
{
    if (header.isEmpty()){ //init grid
        gELSource_->SetSourceGrid(wholeGrid_);
        return true;
    }else{ //displacement grid
         if (disELSourceMap_.find(header)==disELSourceMap_.end()){
            vtkUnstructuredGrid *g = ELDisplacementGrid(header);
            if (g){
                disELSourceMap_.insert(header,g);
                gELSource_->SetSourceGrid(g);
                return true;
            }
            else return false;
         }
         gELSource_->SetSourceGrid(disELSourceMap_[header]);
         return true;
    }
}
//--获取表面集合的grid;
bool InpDataSource::SetInpSurfDataSource(const QString &header)
{
    if (header.isEmpty()){ //init grid
        gSurfaceSource_->SetSourceGrid(wholeGrid_);
        return true;
    }else{ //displacement grid
        if (disSurfSourceMap_.find(header)==disSurfSourceMap_.end()){
            vtkUnstructuredGrid *g = SurfDisplacementGrid(header);
            if (g){
                disSurfSourceMap_.insert(header,g);
                gSurfaceSource_->SetSourceGrid(g);
                return true;
            }
            else return false;
        }
         gSurfaceSource_->SetSourceGrid(disSurfSourceMap_[header]);
         return true;
    }
}
//获取inp中单元的类型，进行分类
int InpDataSource::getInpCellType(QString strType)
{
    //throw std::logic_error("The method or operation is not implemented.");
    int intCellType=9;
    if (strType=="c3d6"){intCellType = 2;
    }else if(strType=="c3d15"){intCellType = 5;
    }else if(strType=="c3d8"||strType=="c3d8r"){intCellType = 1;
    }else if(strType=="c3d20"||strType=="c3d20r"){intCellType = 4;
    }else if(strType=="cps3"||strType=="cpe3"||strType=="cax3"){intCellType = 7;
    }else if(strType=="cps6"||strType=="cpe6"||strType=="cax6"){intCellType = 8;
    }else if(strType=="cps4"||strType=="cps4r"||strType=="cpe4"||strType=="cpe4r"||
        strType=="cax4"||strType=="cax4Rr"){intCellType = 9;
    }else if (strType=="cps8"||strType=="cps8r"||strType=="cpe8"||strType=="cpe8r"||
        strType=="cax8"||strType=="cax8r"){intCellType = 10;
    }else if(strType=="c3d4"||strType=="c3d4t"){intCellType = 3;
    }else if(strType=="c3d10"||strType=="c3d10t"){intCellType = 6;
    }
    return intCellType;
}
//--进行集合的数据Map整理;
bool InpDataSource::InitBCFromFrd(ReadInpResultS *Inp)
{
    QList<QString> strElData = Inp->TmpElInpS.strData;
    for (int i=0;i<strElData.size();i++)
    {
        QString strEl=strElData.at(i);
        //m_ElementData.insert(i,strEl);
        m_ElementData.insert(strEl.split(",",QString::SkipEmptyParts).at(0).toInt()-1,strEl);
    }
   DecodeGenerateS GenerateSetData;
   QStringList NSetName = Inp->TmpNsetInps.strNSetName;
   for (int i=0;i<NSetName.size();i++){
        GenerateSetData.strDataList = Inp->TmpNsetInps.strData.at(i);
        if(Inp->TmpNsetInps.strNSetStyle.at(i).toLower()=="generate"){GenerateSetData.iDataGenerate = 0;//有间隔
        }else{GenerateSetData.iDataGenerate = 1;//无间隔，直接去点
        }
        varNSetMap_.insert(NSetName.at(i),GenerateSetData);
   }

   QStringList ESetName = Inp->TmpElSetInps.strElSetName;
   
   for (int i=0;i<ESetName.size();i++){ 
       if(Inp->TmpElSetInps.strElSetStyle.at(i).toLower()=="generate"){GenerateSetData.iDataGenerate = 0;//有间隔
       }else{GenerateSetData.iDataGenerate = 1;//无间隔，直接去点
       }
       GenerateSetData.strDataList = Inp->TmpElSetInps.strData.at(i).split(",",QString::SkipEmptyParts);
       varElSetMap_.insert(ESetName.at(i),GenerateSetData);
   }

    //DecodeSurfaceS tmpSurfaceData;
    QList<QString> strSurfaceData = Inp->TmpSurfaceInps.strData;
    int iTmpCnt=0;
    int iBaseCnt=0;//数据增加基准
    for (int i=0;i<Inp->TmpSurfaceInps.strSurfaceName.size();i++)
    {
        iTmpCnt=Inp->TmpSurfaceInps.strSurfaceDataNum.at(i).toInt();

        QStringList strSurfList;
        for (int j=0;j<iTmpCnt;j++){
              strSurfList<<strSurfaceData.at(iBaseCnt+j); 
         }
        iBaseCnt+=iTmpCnt;
        varSurfaceSetMap_.insert(Inp->TmpSurfaceInps.strSurfaceName.at(i),strSurfList);
    }
    return true;
}
//--点集合的显示；
vtkUnstructuredGrid* InpDataSource::BCDisplacementGrid(const QString &header)
{
   if (!wholeGrid_)  return 0;
  
   int nCell=0;
   QStringList tmpStrList;
   bool b=GetNCellData(&tmpStrList,header);
   nCell=tmpStrList.size();
   if (b==false) {return 0;}
   vtkUnstructuredGrid *ungrid = vtkUnstructuredGrid::New();
   ungrid->Allocate(nCell);
   vtkPoints *points = wholeGrid_->GetPoints();

   for(int i = 0; i < nCell; ++i){
       vtkCell *cell = vtkQuad::New();
       if (cell){
           int cellType_vtk = cell->GetCellType();
           int np = cell->GetNumberOfPoints();
           vtkIdList *idList = cell->GetPointIds();
          
           for (int j = 0; j < np; ++j){
               //idList->SetId(j,tmpStrList.at(i).toInt()-1);
			   idList->SetId(j,InpPointIdMap_[tmpStrList.at(i).toInt()]);
           }
          
           ungrid->InsertNextCell(cellType_vtk, idList);
           cell->Delete();
       }
   }
   ungrid->SetPoints(points);
   return ungrid;
}
//--单元集合的显示；
vtkUnstructuredGrid* InpDataSource::ELDisplacementGrid(const QString &header)
{
    if (!wholeGrid_)  return 0;

    
    QStringList tmpStrList;
    bool b=GetElCellData(&tmpStrList,header);
    int nCell=tmpStrList.size();
    if (b==false) {return 0;}
    vtkUnstructuredGrid *ungrid = vtkUnstructuredGrid::New();
    ungrid->Allocate(nCell);
    vtkPoints *points = wholeGrid_->GetPoints();
    // int  inpCellType=wholeGrid_->GetCellType();
    for(int i = 0; i < nCell; ++i){
        vtkCell *cell = NULL;

		int m_iCelltype0;
		QStringList DataType=tmpStrList.at(i).split(",");
		int NodeNumber=DataType.size()-1;
		if(n2D3D==2){
			switch(NodeNumber){
				case 3:{m_iCelltype0 =7;}break;
			    case 4:{m_iCelltype0 =9;}break;
		     	case 6:{m_iCelltype0 =8;}break;
				case 8:{m_iCelltype0 =10;}break;
				default:{;} break; 
			}
		}else{
			switch(NodeNumber){
			    case 4:{m_iCelltype0 =3;}break;
			    case 6:{m_iCelltype0 =2;}break;
			    case 8:{m_iCelltype0 =1;}break;
			    case 10:{m_iCelltype0 =6;}break;
			    case 15:{m_iCelltype0 =5;}break;
			    case 20:{m_iCelltype0 =4;}break;	
				default:{;} break; 
			}
		}

        switch (m_iCelltype0){
        case 1:{cell = vtkHexahedron::New();}break;
        case 2:{cell = vtkWedge::New();} break;
        case 3:{cell = vtkTetra::New();}break;
        case 4:{cell = vtkQuadraticHexahedron::New();}break; 
        case 5:{cell = vtkQuadraticWedge::New();}break; 
        case 6:{cell = vtkQuadraticTetra::New();}break; 
        case 7:{cell = vtkTriangle::New();}break;
        case 8:{cell = vtkQuadraticTriangle::New();}break; 
        case 9:{cell = vtkQuad::New();}break;   
        case 10:{cell = vtkQuadraticQuad::New();}break;
        default:{;} break;   
        }
        if (cell){
            int cellType_vtk = cell->GetCellType();
            int np = cell->GetNumberOfPoints();
            vtkIdList *idList = cell->GetPointIds();

            QStringList tmpList = tmpStrList.at(i).split(",");
            for (int j = 0; j < np; ++j){ 
                //int itmpStr=tmpList.at(j+1).simplified().toInt()-1;
				//idList->SetId(j,itmpStr);
				int itmpStr=tmpList.at(j+1).simplified().toInt();
                idList->SetId(j,InpPointIdMap_[itmpStr]);//itmpStr);
            }
            if (cellType_vtk == VTK_QUADRATIC_HEXAHEDRON)
            {
                int t12 = idList->GetId(12);
                int t13 = idList->GetId(13);
                int t14 = idList->GetId(14);
                int t15 = idList->GetId(15);
                idList->SetId(12, idList->GetId(16));
                idList->SetId(13, idList->GetId(17));
                idList->SetId(14, idList->GetId(18));
                idList->SetId(15, idList->GetId(19));
                idList->SetId(16, t12);
                idList->SetId(17, t13);
                idList->SetId(18, t14);
                idList->SetId(19, t15);
            }
            else if (cellType_vtk == VTK_QUADRATIC_WEDGE)
            {
                int t9 = idList->GetId(9);
                int t10 = idList->GetId(10);
                int t11 = idList->GetId(11);
                idList->SetId(9, idList->GetId(12));
                idList->SetId(10, idList->GetId(13));
                idList->SetId(11, idList->GetId(14));
                idList->SetId(12, t9);
                idList->SetId(13, t10);
                idList->SetId(14, t11);
            }
            ungrid->InsertNextCell(cellType_vtk, idList);
            cell->Delete();
        }
    }
    ungrid->SetPoints(points);
    return ungrid;
}
//--表面集合的显示；
vtkUnstructuredGrid* InpDataSource::SurfDisplacementGrid(const QString &header)
{
    if (!wholeGrid_)  return 0;
    QStringList tmpStrList,CellNewStyle;
    QVector<QStringList> tmpVList;
    bool b=GetSurfCellData(&tmpVList,&CellNewStyle,header);   
    int nCell=0;
    for (int i=0;i<tmpVList.size();i++){
        nCell+=tmpVList.at(i).size();
    }
    if (b==false) {return 0;}
    vtkUnstructuredGrid *ungrid = vtkUnstructuredGrid::New();
    ungrid->Allocate(nCell);
    vtkPoints *points = wholeGrid_->GetPoints();
    for (int i=0; i<CellNewStyle.size(); i++){//s2,s3
		int m_iCelltype=9;//单元类型,先初始化
        QStringList tmpList = tmpVList.at(i);
        for(int j = 0; j < tmpVList.at(i).size(); ++j){
			QStringList dataList=tmpList.at(j).split(",");
			int Ind=dataList.size()-1;
			m_iCelltype=dataList.at(Ind).toInt();
			dataList.takeAt(Ind);
            vtkCell *cell = NewVTKCellS(m_iCelltype,CellNewStyle.at(i));
            if (cell){
              int cellType_vtk = cell->GetCellType();
              int np = cell->GetNumberOfPoints();
              vtkIdList *idList = cell->GetPointIds();

              for (int kk = 0; kk < np; ++kk){ 
               idList->SetId(kk,dataList.at(kk).toInt()-1);
				//  idList->SetId(kk,InpPointIdMap_[dataList.at(kk).toInt()]);//dataList.at(kk).toInt()-1);
              }
              ungrid->InsertNextCell(cellType_vtk, idList);
              cell->Delete();
            }
        }
    }
   
    ungrid->SetPoints(points);
    return ungrid;
}
//--点集合中数据转换
bool InpDataSource::GetNCellData(QStringList *CellListData, QString header)
{
    QStringList tmpList;
    QMap<QString, DecodeGenerateS>::Iterator it=varNSetMap_.begin();
    DecodeGenerateS GenerateSetData;
    for (it;it!=varNSetMap_.end();it++){ //数据转换
        if (header==it.key()){
            GenerateSetData = it.value();
            tmpList = GenerateSetData.strDataList;
            int NListSize=tmpList.size();
            if (NListSize<=0)return false;
            int iTmp=tmpList.at(0).split(",").size();
            if (GenerateSetData.iDataGenerate==0 && NListSize==1 && iTmp == 3){//只有一行，“1,1592,1 -->第1个：开始点；第2个：结束点；第三个间隔数字”
                QStringList StrWholeLine=tmpList.at(0).split(",");
                for (int kk =StrWholeLine.at(0).toInt();kk<=StrWholeLine.at(1).toInt();kk=kk+StrWholeLine.at(2).toInt()){
                    QString str = QString("%1").arg(kk);
                    *CellListData<<str;  
                }
                break;
            }else{//按点数解析
                iTmp = 0;
                for (int j=0; j<NListSize;j++){
                    iTmp+=tmpList.at(j).split(",").size();
                    *CellListData<<tmpList.at(j).split(",",QString::SkipEmptyParts);
                }
                break;
            }
        }
    }
    return true;
}
/*
//--单元集合中，数据转换
*/
bool InpDataSource::GetElCellData(QStringList *CellListData,QString header)
{
    QStringList tmpList;
    QMap<QString, DecodeGenerateS>::Iterator it=varElSetMap_.begin();
    DecodeGenerateS GenerateSetData;
    for (it;it!=varElSetMap_.end();it++){ //数据转换
        if (header==it.key()){
            GenerateSetData=it.value();
            tmpList = GenerateSetData.strDataList;
            int NListSize=tmpList.size();
            if (NListSize<=0)return false;

            if (GenerateSetData.iDataGenerate==0 && NListSize==3){//只有一行，“1,1592,1 -->第1个：开始点；第2个：结束点；第三个间隔数字”
                for(int kk=tmpList.at(0).toInt();kk<=tmpList.at(1).toInt();kk+=tmpList.at(2).toInt()){
                    *CellListData<<m_ElementData.find(kk-1).value(); 
                }
                break;
            }else{//按点数解析
                for (int j=0; j<NListSize;j++){
                    if (m_ElementData.find(tmpList.at(j).toInt()-1)!=m_ElementData.end()){
                        *CellListData<<m_ElementData.find(tmpList.at(j).toInt()-1).value();
                    } 
                }
                break;
            }
        }
        //return false;
    }
    return true;
}
/*
//--表面集合中，数据转换
*/
bool InpDataSource:: GetSurfCellData(QVector<QStringList> *CellListData,QStringList *CellNewStyle, QString header)
{
    QStringList tmpList;
    QMap<QString, QStringList>::Iterator it=varSurfaceSetMap_.begin();
    QStringList qListString;
    for (it; it != varSurfaceSetMap_.end(); it++){ //数据转换
        if (header==it.key()){
           qListString=it.value();
           int nSize=qListString.size();
           
           for (int i=0;i<nSize;i++){
               QStringList strL1;
               QStringList tmpStrL=qListString.at(i).split(",",QString::SkipEmptyParts);
               QString strElsetName = tmpStrL.at(0).simplified();
               QString strCellNewStyle = tmpStrL.at(1).simplified();
               GetElCellData(&strL1,strElsetName);
               GetChooseData(&strL1,strCellNewStyle);
               CellListData->push_back(strL1);
               *CellNewStyle<<strCellNewStyle;
           }
         
           break;
        }
        //return false;
    }
    //QStringList tmpStrList;
    //bool b=GetElCellData(&tmpStrList,header);
    return true;
}
//--表面集合中获取当前面的点;
bool InpDataSource::GetChooseData(QStringList *strRowData,QString strFName)
{
    QStringList tmpStrL;
    QStringList StrL = *strRowData;
    for (int i=0;i<StrL.size();i++)
	{
        QString strTmp="";
		int m_iCelltype0;
		QStringList DataType=StrL.at(i).split(",");
		int NodeNumber=DataType.size()-1;
		if(n2D3D==2){
			switch(NodeNumber){
				case 3:{m_iCelltype0 =7;}break;
			    case 4:{m_iCelltype0 =9;}break;
		     	case 6:{m_iCelltype0 =8;}break;
				case 8:{m_iCelltype0 =10;}break;
				default:{;} break; 
			}
		}else{
			switch(NodeNumber){
			    case 4:{m_iCelltype0 =3;}break;
			    case 6:{m_iCelltype0 =2;}break;
			    case 8:{m_iCelltype0 =1;}break;
			    case 10:{m_iCelltype0 =6;}break;
			    case 15:{m_iCelltype0 =5;}break;
			    case 20:{m_iCelltype0 =4;}break;	
				default:{;} break; 
			}
		}
		switch (m_iCelltype0){
        case 1://8节点6面体->四边形
        case 4:{//20节点6面体->四边形
             QString strTmp,strTmp1,strTmp2,strTmp3;strTmp.clear();strTmp1.clear();strTmp2.clear();strTmp3.clear();
             if (strFName =="s1"){
                 strTmp =StrL.at(i).split(",").at(1).simplified();
                 strTmp1=StrL.at(i).split(",").at(2).simplified();
                 strTmp2=StrL.at(i).split(",").at(3).simplified();
                 strTmp3 =StrL.at(i).split(",").at(4).simplified();
                 strTmp.append(",").append(strTmp1).append(",").append(strTmp2).append(",").append(strTmp3);
             }else if(strFName=="s2"){
                 strTmp =StrL.at(i).split(",").at(5).simplified();
                 strTmp1=StrL.at(i).split(",").at(8).simplified();
                 strTmp2=StrL.at(i).split(",").at(7).simplified();
                 strTmp3 =StrL.at(i).split(",").at(6).simplified();
                 strTmp.append(",").append(strTmp1).append(",").append(strTmp2).append(",").append(strTmp3);
             }else if(strFName=="s3"){
                 strTmp =StrL.at(i).split(",").at(1).simplified();
                 strTmp1=StrL.at(i).split(",").at(5).simplified();
                 strTmp2=StrL.at(i).split(",").at(6).simplified();
                 strTmp3 =StrL.at(i).split(",").at(2).simplified();
                 strTmp.append(",").append(strTmp1).append(",").append(strTmp2).append(",").append(strTmp3);
             }else if(strFName=="s4"){
                 strTmp =StrL.at(i).split(",").at(2).simplified();
                 strTmp1=StrL.at(i).split(",").at(6).simplified();
                 strTmp2=StrL.at(i).split(",").at(7).simplified();
                 strTmp3 =StrL.at(i).split(",").at(3).simplified();
                 strTmp.append(",").append(strTmp1).append(",").append(strTmp2).append(",").append(strTmp3);
             }else if(strFName=="s5"){
                 strTmp =StrL.at(i).split(",").at(3).simplified();
                 strTmp1=StrL.at(i).split(",").at(7).simplified();
                 strTmp2=StrL.at(i).split(",").at(8).simplified();
                 strTmp3 =StrL.at(i).split(",").at(4).simplified();
                 strTmp.append(",").append(strTmp1).append(",").append(strTmp2).append(",").append(strTmp3);
             }else if(strFName=="s6"){
                 strTmp =StrL.at(i).split(",").at(4).simplified();
                 strTmp1=StrL.at(i).split(",").at(8).simplified();
                 strTmp2=StrL.at(i).split(",").at(5).simplified();
                 strTmp3 =StrL.at(i).split(",").at(1).simplified();
                 strTmp.append(",").append(strTmp1).append(",").append(strTmp2).append(",").append(strTmp3);
             } 
			 QString jj3=QString::number(m_iCelltype0,10);
             strTmp.append(",").append(jj3);
             tmpStrL<<strTmp;
          }break; 
        case 3://4节点4面体->三角形
        case 6:{//10节点4面体->三角形
            QString strTmp,strTmp1,strTmp2; strTmp.clear();strTmp1.clear();strTmp2.clear();
             if (strFName =="s1"){
                 strTmp =StrL.at(i).split(",").at(1).simplified();
                 strTmp1=StrL.at(i).split(",").at(2).simplified();
                 strTmp2=StrL.at(i).split(",").at(3).simplified();
             }else if(strFName=="s2"){
                 strTmp =StrL.at(i).split(",").at(1).simplified();
                 strTmp1=StrL.at(i).split(",").at(4).simplified();
                 strTmp2=StrL.at(i).split(",").at(2).simplified();
             }else if(strFName=="s3"){
                 strTmp =StrL.at(i).split(",").at(2).simplified();
                 strTmp1=StrL.at(i).split(",").at(4).simplified();
                 strTmp2=StrL.at(i).split(",").at(3).simplified();
             }else if(strFName=="s4"){
                 strTmp =StrL.at(i).split(",").at(3).simplified();
                 strTmp1=StrL.at(i).split(",").at(4).simplified();
                 strTmp2=StrL.at(i).split(",").at(1).simplified();
             }
           strTmp.append(",").append(strTmp1).append(",").append(strTmp2);
		   QString jj3=QString::number(m_iCelltype0,10);
           strTmp.append(",").append(jj3);
           tmpStrL<<strTmp;
        }break;//vtkTetra::New();}break;
     case 7:
     case 8:{//三角形-->直线
         QString strTmp,strTmp1;strTmp.clear();strTmp1.clear();
         if (strFName =="s1"){
             strTmp =StrL.at(i).split(",").at(1).simplified();
             strTmp1=StrL.at(i).split(",").at(2).simplified();
         }else if(strFName=="s2"){
             strTmp =StrL.at(i).split(",").at(2).simplified();
             strTmp1=StrL.at(i).split(",").at(3).simplified();
         }else if(strFName=="s3"){
             strTmp =StrL.at(i).split(",").at(3).simplified();
             strTmp1=StrL.at(i).split(",").at(1).simplified();
         }
         strTmp.append(",").append(strTmp1);
		 QString jj3=QString::number(m_iCelltype0,10);
         strTmp.append(",").append(jj3);
         tmpStrL<<strTmp;
        }break;
     case 2:
     case 5:{
         QString strTmp,strTmp1,strTmp2,strTmp3;strTmp.clear();strTmp1.clear();strTmp2.clear();strTmp3.clear();
         if (strFName =="s1"){//1-2-3三角形
             strTmp =StrL.at(i).split(",").at(1).simplified();
             strTmp1=StrL.at(i).split(",").at(2).simplified();
             strTmp2=StrL.at(i).split(",").at(3).simplified();
             strTmp.append(",").append(strTmp1).append(",").append(strTmp2);
			 QString jj3=QString::number(m_iCelltype0,10);
             strTmp.append(",").append(jj3);
             tmpStrL<<strTmp;
         }else if(strFName=="s2"){//-4-5-6
             strTmp =StrL.at(i).split(",").at(4).simplified();
             strTmp1=StrL.at(i).split(",").at(5).simplified();
             strTmp2=StrL.at(i).split(",").at(6).simplified();
             strTmp.append(",").append(strTmp1).append(",").append(strTmp2);
			 QString jj3=QString::number(m_iCelltype0,10);
             strTmp.append(",").append(jj3);
             tmpStrL<<strTmp;
         }else if(strFName=="s3"){//1-2-5-4
             strTmp =StrL.at(i).split(",").at(1).simplified();
             strTmp1=StrL.at(i).split(",").at(2).simplified();
             strTmp2=StrL.at(i).split(",").at(5).simplified();
             strTmp3 =StrL.at(i).split(",").at(4).simplified();
             strTmp.append(",").append(strTmp1).append(",").append(strTmp2).append(",").append(strTmp3);
			 QString jj3=QString::number(m_iCelltype0,10);
             strTmp.append(",").append(jj3);
             tmpStrL<<strTmp;
         }else if(strFName=="s4"){//2-3-6-5
             strTmp =StrL.at(i).split(",").at(2).simplified();
             strTmp1=StrL.at(i).split(",").at(3).simplified();
             strTmp2=StrL.at(i).split(",").at(6).simplified();
             strTmp3 =StrL.at(i).split(",").at(5).simplified();
             strTmp.append(",").append(strTmp1).append(",").append(strTmp2).append(",").append(strTmp3);
			 QString jj3=QString::number(m_iCelltype0,10);
             strTmp.append(",").append(jj3);
             tmpStrL<<strTmp;
         }else if(strFName=="s5"){//3-1-4-6
             strTmp =StrL.at(i).split(",").at(3).simplified();
             strTmp1=StrL.at(i).split(",").at(1).simplified();
             strTmp2=StrL.at(i).split(",").at(4).simplified();
             strTmp3 =StrL.at(i).split(",").at(6).simplified();
             strTmp.append(",").append(strTmp1).append(",").append(strTmp2).append(",").append(strTmp3);
			 QString jj3=QString::number(m_iCelltype0,10);
             tmpStrL<<strTmp;
         }
       }break;
     case 9: //4节点4边形--s1~s4:则表示为直线
     case 10:{//8节点4边形--s1~s4:则表示为直线
           QString strTmp,strTmp1; strTmp.clear();strTmp1.clear();
         if (strFName =="s1"){
             strTmp =StrL.at(i).split(",").at(1).simplified();
             strTmp1=StrL.at(i).split(",").at(2).simplified();
         }else if(strFName=="s2"){
             strTmp =StrL.at(i).split(",").at(2).simplified();
             strTmp1=StrL.at(i).split(",").at(3).simplified();
         }else if(strFName=="s3"){
             strTmp =StrL.at(i).split(",").at(3).simplified();
             strTmp1=StrL.at(i).split(",").at(4).simplified();
         }else if(strFName=="s4"){
             strTmp =StrL.at(i).split(",").at(4).simplified();
             strTmp1=StrL.at(i).split(",").at(1).simplified(); 
         }
         strTmp.append(",").append(strTmp1);
		 QString jj3=QString::number(m_iCelltype0,10);
         strTmp.append(",").append(jj3);
         tmpStrL<<strTmp;
       }break;   
      default:{;} break;  
        }
    }
   
    *strRowData=tmpStrL;
    return true;
}
/*
inpcellType: 判断何种单元类型，
iFace:s1,s2,s3,s4,s5,s6
*/
vtkCell* InpDataSource::NewVTKCellS(int InpCellType, QString strFName)
{
      vtkCell *cell = NULL; 
      switch (InpCellType){
        case 1://8节点6面体->四边形
        case 4:{//20节点6面体->四边形
            //if (strFName =="S1"){
            //}else if(strFName=="S2"){
            // 
            //}else if(strFName=="S3"){
 
            //}else if(strFName=="S4"){
            //  
            //}else if(strFName=="S5"){
 
            //}else if(strFName=="S6"){
            //  
            //} 
            cell = vtkQuad::New();
          }break; 
        case 3://4节点4面体->三角形
        case 6:{//10节点4面体->三角形
       /*     if (strFName =="S1"){
               
            }else if(strFName=="S2"){

            }else if(strFName=="S3"){
             
            }else if(strFName=="S4"){

            }*/
            cell = vtkTriangle::New();
          }break;//vtkTetra::New();}break;
        case 7:
        case 8:{//三角形---直线
            if (strFName =="s1"){
            }else if(strFName=="s2"){
            }else if(strFName=="s3"){
            }
             cell = vtkLine::New();
          }break; 
        case 9: //4节点4边形--s1~s4:则表示为直线
        case 10:{//8节点4边形--s1~s4:则表示为直线
           /* if (strFName =="S1"){
            }else if(strFName=="S2"){
            }else if(strFName=="S3"){
            }else if(strFName=="S4"){
            }*/
            cell = vtkLine::New();
          }break;   
        case 2://楔形
        case 5:{
            if (strFName =="s1"){//1-2-3三角形
                cell = vtkTriangle::New();
            }else if(strFName=="s2"){//-4-5-6
                cell = vtkTriangle::New();
            }else if(strFName=="s3"){//1-2-5-4
                cell = vtkQuad::New();
            }else if(strFName=="s4"){//2-3-6-5
                cell = vtkQuad::New();
            }else if(strFName=="s5"){//3-1-4-6
                cell = vtkQuad::New();
            }
          }break;
        default:{;} break;  

      }
    return cell;
}


//--获取表面集合的拓扑结构
bool InpDataSource::InpRowDataToSurf(ReadInpResultS *Inp)
{
    if (Inp == 0)  return false;
    int nPoint = Inp->TmpNodeInpS.strData.size();//nodalPoint.nNum;
    PointCoordinate p;
    vtkPoints *points = vtkPoints::New();
	QStringList TmpBasePoint=(Inp->TmpNodeInpS.strData.at(0).split(","));
    points->SetNumberOfPoints(nPoint+TmpBasePoint.at(0).toInt()-1);
    QString AnalysisType=Inp->TmpElInpS.strELType.mid(0,2).toLower();
	if(AnalysisType=="")AnalysisType=Inp->TmpElInpS.ElementType.at(0).mid(0,2).toLower();
	if(AnalysisType=="c3")n2D3D=3;
	else n2D3D=2;
	AnalysisType.clear();
    for (int i = 0; i < nPoint; ++i)
    {
        QStringList TmpNodeData=Inp->TmpNodeInpS.strData.at(i).split(",");
       
        p.x=TmpNodeData.at(1).toDouble();
        p.y=TmpNodeData.at(2).toDouble();
        if (n2D3D==3){//体
            p.z=TmpNodeData.at(3).toDouble();
        }else if(n2D3D==2){//面
            p.z=0;//mFrdData.nodalPoint.Data.z
        }
        points->SetPoint(i+TmpBasePoint.at(0).toInt()-1,p.x,p.y,p.z);
    }

    int nCell = Inp->TmpElInpS.strData.size();//Inp->elemBlock.nNum;
   
    QList<QString> cellPointsList=Inp->TmpElInpS.strData;//单元编号和单元节点号
    int inpCellType=9;

    for(int i = 0; i < nCell; ++i){
        QStringList DataType=Inp->TmpElInpS.strData.at(i).split(",");
        int NodeNumber=DataType.size()-1;
        if(n2D3D==2){
            switch(NodeNumber){
                case 3:{inpCellType =7;}break;
                case 4:{inpCellType =9;}break;
                case 6:{inpCellType =8;}break;
                case 8:{inpCellType =10;}break;
                default:{;} break;
            }
        }else{
            switch(NodeNumber){
                case 4:{inpCellType =3;}break;//
                case 6:{inpCellType =2;}break;//
                case 8:{inpCellType =1;}break;//
                case 10:{inpCellType =6;}break;
                case 15:{inpCellType =5;}break;
                case 20:{inpCellType =4;}break;	
                default:{;} break; 
            }
        }
        m_iCelltype = inpCellType;
        Del3DSameFace(inpCellType,DataType);
    }

    int ncell= m_ElementSurfmap.count();
    wholeSurfGrid_ = vtkUnstructuredGrid::New();
    wholeSurfGrid_->Allocate(nCell);
    QMap<QString,QString>::Iterator it=m_ElementSurfmap.begin();

    for(it; it!=m_ElementSurfmap.end(); ++it){
        vtkCell *cell = NULL;
        QStringList SnData=it.value().split(",");
		inpCellType =SnData.at(2).toInt();
        switch (inpCellType){
            case 1:{cell = vtkQuad::New();}break;//ok vtkHexahedron::New();}break;
            case 2:{//ok
                    if ((SnData.size()-3)==4) cell = vtkQuad::New();
                    else cell = vtkTriangle::New();
               }break;//vtkWedge::New();} break;
            case 3:{cell = vtkTriangle::New();}break;//ok vtkTetra::New();}break;
            case 4:{cell = vtkQuadraticQuad::New();}break;//(wait data for test) vtkQuadraticHexahedron::New();}break; 
            case 5:{
                    if ((SnData.size()-3)==8) cell = vtkQuadraticQuad::New();//(wait data for test)
                    else cell = vtkQuadraticTriangle::New();//(wait data for test)
              }break;//vtkQuadraticWedge::New();}break; 
            case 6:{cell = vtkQuadraticTriangle::New();}break;//(wait data for test)vtkQuadraticTetra::New();}break; 
            case 7:{cell = vtkLine::New();}break;//(ok) vtkTriangle::New();}break;
            case 8:{cell = vtkPolyLine::New();}break;//vtkQuadraticTriangle::New();}break; 
            case 9:{cell = vtkLine::New();}break;//(ok)vtkQuad::New();}break;   
            case 10:{cell = vtkPolyLine::New();}break;//vtkQuadraticQuad::New();}break;
            default:{;} break;
		}

		//--no more point(eg:only support C3D6,not support c3d15 )
		/*int nSnDataSize=SnData.size();
		if (nSnDataSize<2)return false;
			nSnDataSize=nSnDataSize-2;
			switch(nSnDataSize){
				case 2:{cell = vtkLine::New();}break;
				case 3:{cell = vtkTriangle::New();}break;
				case 4:{cell = vtkQuad::New();}break;
				default:{;} break; 
			}
		*/

        if (cell){
            int cellType_vtk = cell->GetCellType();
            int np = cell->GetNumberOfPoints();
            vtkIdList *idList = cell->GetPointIds();
            //QStringList tmpStrList = SnData.at//cellPointsList.at(i).split(",");
            for (int j = 0; j < np; ++j){
               // idList->SetId(j, SnData.at(j+2).toInt()-1);
				idList->SetId(j, InpPointIdMap_[SnData.at(j+3).toInt()]);//SnData.at(j+2).toInt()-1);
            }
            wholeSurfGrid_->InsertNextCell(cellType_vtk, idList);
            cell->Delete();
         }
    }
    wholeSurfGrid_->SetPoints(points);
    points->Delete();
    return true;
}
//Del the same 3d face
void InpDataSource::Del3DSameFace(int iType,QStringList tmpStrList)
{
    switch(iType){
        case 1:{//C3D8(C3D8R)
                QList<int> sortD;
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(4).toInt();
                qSort(sortD.begin(),sortD.end());
                QString strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                QString noSorD;
                noSorD =tmpStrList.at(0)+",1,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(4);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }


                sortD.clear();
                sortD<<tmpStrList.at(5).toInt();
                sortD<<tmpStrList.at(8).toInt();
                sortD<<tmpStrList.at(7).toInt();
                sortD<<tmpStrList.at(6).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD =tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(5)+","+
                    tmpStrList.at(8)+","+
                    tmpStrList.at(7)+","+
                    tmpStrList.at(6);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap .remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(5).toInt();
                sortD<<tmpStrList.at(6).toInt();
                sortD<<tmpStrList.at(2).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD =tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(5)+","+
                    tmpStrList.at(6)+","+
                    tmpStrList.at(2);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(6).toInt();
                sortD<<tmpStrList.at(7).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.size();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD =tmpStrList.at(0)+",4,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(6)+","+
                    tmpStrList.at(7)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(7).toInt();
                sortD<<tmpStrList.at(8).toInt();
                sortD<<tmpStrList.at(4).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.size();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",5,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(7)+","+
                    tmpStrList.at(8)+","+
                    tmpStrList.at(4);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(8).toInt();
                sortD<<tmpStrList.at(5).toInt();
                sortD<<tmpStrList.at(1).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.size();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD =tmpStrList.at(0)+",6,"+QString("%1,").arg(iType)+
                    tmpStrList.at(4)+","+
                    tmpStrList.at(8)+","+
                    tmpStrList.at(5)+","+
                    tmpStrList.at(1);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }
           }break;
        case 2:{//c3d6 wedge
                QList<int> sortD;
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                QString noSorD;
                noSorD =tmpStrList.at(0)+",1,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(5).toInt();
                sortD<<tmpStrList.at(6).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(4)+","+
                    tmpStrList.at(5)+","+
                    tmpStrList.at(6);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }


                sortD.clear();
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(5).toInt();
                sortD<<tmpStrList.at(4).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(5)+","+
                    tmpStrList.at(4);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(6).toInt();
                sortD<<tmpStrList.at(5).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",4,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(6)+","+
                    tmpStrList.at(5);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD, noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(6).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",5,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(4)+","+
                    tmpStrList.at(6);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD, noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }
           }break;
        case 3:{//c3d4
                QList<int> sortD;
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                QString noSorD;
                noSorD =tmpStrList.at(0)+",1,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(2).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(4)+","+
                    tmpStrList.at(2);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }


                sortD.clear();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(4)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(1).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",4,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(4)+","+
                    tmpStrList.at(1);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD, noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }
           }break;
        case 4:{//(cps4/cps8r/cpe8/cpe8r.cax8/cax8r)->change ids;
           /* QList<int> sortD;
            sortD<<tmpStrList.at(1).toInt();
            sortD<<tmpStrList.at(9).toInt();
            sortD<<tmpStrList.at(2).toInt();
            sortD<<tmpStrList.at(10).toInt();
            sortD<<tmpStrList.at(3).toInt();
            sortD<<tmpStrList.at(11).toInt();
            sortD<<tmpStrList.at(4).toInt();
            sortD<<tmpStrList.at(12).toInt();
            qSort(sortD.begin(),sortD.end());
            QString strSortD=QString("%1").arg(sortD.at(0));
            for (int kk=1;kk<sortD.count();kk++){
                strSortD+=","+QString("%1").arg(sortD.at(kk));
            }
            QString noSorD;
            noSorD =tmpStrList.at(0)+",1,"+
                tmpStrList.at(1)+","+
                tmpStrList.at(9)+","+
                tmpStrList.at(2)+","+
                tmpStrList.at(10)+","+
                tmpStrList.at(3)+","+
                tmpStrList.at(11)+","+
                tmpStrList.at(4)+","+
                tmpStrList.at(12);
            if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                m_ElementSurfmap.insert(strSortD,noSorD);
            }else{
                m_ElementSurfmap.remove(strSortD);
            }


            sortD.clear();
            sortD<<tmpStrList.at(5).toInt();
            sortD<<tmpStrList.at(8).toInt();
            sortD<<tmpStrList.at(7).toInt();
            sortD<<tmpStrList.at(6).toInt();
            qSort(sortD.begin(),sortD.end());
            strSortD=QString("%1").arg(sortD.at(0));
            for (int kk=1;kk<sortD.count();kk++){
                strSortD+=","+QString("%1").arg(sortD.at(kk));
            }
            noSorD =tmpStrList.at(0)+",2,"+
                tmpStrList.at(5)+","+
                tmpStrList.at(8)+","+
                tmpStrList.at(7)+","+
                tmpStrList.at(6);
            if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                m_ElementSurfmap.insert(strSortD,noSorD);
            }else{
                m_ElementSurfmap .remove(strSortD);
            }

            sortD.clear();
            sortD<<tmpStrList.at(1).toInt();
            sortD<<tmpStrList.at(5).toInt();
            sortD<<tmpStrList.at(6).toInt();
            sortD<<tmpStrList.at(2).toInt();
            qSort(sortD.begin(),sortD.end());
            strSortD=QString("%1").arg(sortD.at(0));
            for (int kk=1;kk<sortD.count();kk++){
                strSortD+=","+QString("%1").arg(sortD.at(kk));
            }
            noSorD =tmpStrList.at(0)+",3,"+
                tmpStrList.at(1)+","+
                tmpStrList.at(5)+","+
                tmpStrList.at(6)+","+
                tmpStrList.at(2);
            if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                m_ElementSurfmap.insert(strSortD,noSorD);
            }else{
                m_ElementSurfmap.remove(strSortD);
            }

            sortD.clear();
            sortD<<tmpStrList.at(2).toInt();
            sortD<<tmpStrList.at(6).toInt();
            sortD<<tmpStrList.at(7).toInt();
            sortD<<tmpStrList.at(3).toInt();
            qSort(sortD.begin(),sortD.end());
            strSortD=QString("%1").arg(sortD.at(0));
            for (int kk=1;kk<sortD.size();kk++){
                strSortD+=","+QString("%1").arg(sortD.at(kk));
            }
            noSorD =tmpStrList.at(0)+",4,"+
                tmpStrList.at(2)+","+
                tmpStrList.at(6)+","+
                tmpStrList.at(7)+","+
                tmpStrList.at(3);
            if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                m_ElementSurfmap.insert(strSortD,noSorD);
            }else{
                m_ElementSurfmap.remove(strSortD);
            }

            sortD.clear();
            sortD<<tmpStrList.at(3).toInt();
            sortD<<tmpStrList.at(7).toInt();
            sortD<<tmpStrList.at(8).toInt();
            sortD<<tmpStrList.at(4).toInt();
            qSort(sortD.begin(),sortD.end());
            strSortD=QString("%1").arg(sortD.at(0));
            for (int kk=1;kk<sortD.size();kk++){
                strSortD+=","+QString("%1").arg(sortD.at(kk));
            }
            noSorD = tmpStrList.at(0)+",5,"+
                tmpStrList.at(3)+","+
                tmpStrList.at(7)+","+
                tmpStrList.at(8)+","+
                tmpStrList.at(4);
            if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                m_ElementSurfmap.insert(strSortD,noSorD);
            }else{
                m_ElementSurfmap.remove(strSortD);
            }

            sortD.clear();
            sortD<<tmpStrList.at(4).toInt();
            sortD<<tmpStrList.at(8).toInt();
            sortD<<tmpStrList.at(5).toInt();
            sortD<<tmpStrList.at(1).toInt();
            qSort(sortD.begin(),sortD.end());
            strSortD=QString("%1").arg(sortD.at(0));
            for (int kk=1;kk<sortD.size();kk++){
                strSortD+=","+QString("%1").arg(sortD.at(kk));
            }
            noSorD =tmpStrList.at(0)+",6,"+
                tmpStrList.at(4)+","+
                tmpStrList.at(8)+","+
                tmpStrList.at(5)+","+
                tmpStrList.at(1);
            if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                m_ElementSurfmap.insert(strSortD,noSorD);
            }else{
                m_ElementSurfmap.remove(strSortD);
            }*/
           }break;
		case 6:{//c3d10
				QList<int> sortD;
				sortD<<tmpStrList.at(1).toInt();
				sortD<<tmpStrList.at(5).toInt();
				sortD<<tmpStrList.at(2).toInt();
				sortD<<tmpStrList.at(6).toInt();
				sortD<<tmpStrList.at(3).toInt();
			    sortD<<tmpStrList.at(7).toInt();
				qSort(sortD.begin(), sortD.end());
				QString strSortD=QString("%1").arg(sortD.at(0));
				for (int kk=1;kk<sortD.count();kk++){
					strSortD+=","+QString("%1").arg(sortD.at(kk));
				}
				QString noSorD;
				noSorD =tmpStrList.at(0)+",1,"+QString("%1,").arg(iType)+
					tmpStrList.at(1)+","+
					tmpStrList.at(5)+","+
					tmpStrList.at(2)+","+
					tmpStrList.at(6)+","+
					tmpStrList.at(3)+","+
					tmpStrList.at(7);
				if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
					m_ElementSurfmap.insert(strSortD,noSorD);
				}else{
					m_ElementSurfmap.remove(strSortD);
				}

				sortD.clear();
				sortD<<tmpStrList.at(1).toInt();
				sortD<<tmpStrList.at(8).toInt();
				sortD<<tmpStrList.at(4).toInt();
				sortD<<tmpStrList.at(9).toInt();
				sortD<<tmpStrList.at(2).toInt();
				sortD<<tmpStrList.at(5).toInt();
				qSort(sortD.begin(),sortD.end());
				strSortD=QString("%1").arg(sortD.at(0));
				for (int kk=1;kk<sortD.count();kk++){
					strSortD+=","+QString("%1").arg(sortD.at(kk));
				}
				noSorD = tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
					tmpStrList.at(1)+","+
					tmpStrList.at(8)+","+
					tmpStrList.at(4)+","+
					tmpStrList.at(9)+","+
					tmpStrList.at(2)+","+
					tmpStrList.at(5);
				if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
					m_ElementSurfmap.insert(strSortD,noSorD);
				}else{
					m_ElementSurfmap.remove(strSortD);
				}


				sortD.clear();
				sortD<<tmpStrList.at(2).toInt();
				sortD<<tmpStrList.at(9).toInt();
				sortD<<tmpStrList.at(4).toInt();
				sortD<<tmpStrList.at(10).toInt();
				sortD<<tmpStrList.at(3).toInt();
				sortD<<tmpStrList.at(6).toInt();
				qSort(sortD.begin(),sortD.end());
				strSortD=QString("%1").arg(sortD.at(0));
				for (int kk=1;kk<sortD.count();kk++){
					strSortD+=","+QString("%1").arg(sortD.at(kk));
				}
				noSorD = tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
					tmpStrList.at(2)+","+
					tmpStrList.at(9)+","+
					tmpStrList.at(4)+","+
					tmpStrList.at(10)+","+
					tmpStrList.at(3)+","+
					tmpStrList.at(6);
				if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
					m_ElementSurfmap.insert(strSortD,noSorD);
				}else{
					m_ElementSurfmap.remove(strSortD);
				}

				sortD.clear();
				sortD<<tmpStrList.at(3).toInt();
				sortD<<tmpStrList.at(10).toInt();
				sortD<<tmpStrList.at(4).toInt();
				sortD<<tmpStrList.at(8).toInt();
				sortD<<tmpStrList.at(1).toInt();
				sortD<<tmpStrList.at(7).toInt();
				qSort(sortD.begin(),sortD.end());
				strSortD=QString("%1").arg(sortD.at(0));
				for (int kk=1;kk<sortD.count();kk++){
					strSortD+=","+QString("%1").arg(sortD.at(kk));
				}
				noSorD = tmpStrList.at(0)+",4,"+QString("%1,").arg(iType)+
					tmpStrList.at(3)+","+
					tmpStrList.at(10)+","+
					tmpStrList.at(4)+","+
					tmpStrList.at(8)+","+
					tmpStrList.at(1)+","+
					tmpStrList.at(7);
				if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
					m_ElementSurfmap.insert(strSortD, noSorD);
				}else{
					m_ElementSurfmap.remove(strSortD);
				}
			}break;
        case 7:{//CPS3(CPE3/CAX3)
                QList<int> sortD;
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(2).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                QString noSorD;
                noSorD =tmpStrList.at(0)+",1,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(2);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }


                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(1).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(1);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }              
           }break;
        case 8:{//(CPS6/CPE6/CAX6)
                QList<int> sortD;
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(7).toInt();
                sortD<<tmpStrList.at(2).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                QString noSorD;
                noSorD =tmpStrList.at(0)+",1,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(7)+","+
                    tmpStrList.at(2);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(8).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(8)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }


                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(9).toInt();
                sortD<<tmpStrList.at(1).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(9)+","+
                    tmpStrList.at(1);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }  
           }break;
        case 9:{//CAX4(CPS4/CPS4R/CPE4/CPE4R/CAX4/CAX4R)
                QList<int> sortD;
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(2).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                QString noSorD;
                noSorD =tmpStrList.at(0)+",1,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(2);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }


                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(4).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(4);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(1).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",4,"+QString("%1,").arg(iType)+
                    tmpStrList.at(4)+","+
                    tmpStrList.at(1);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD, noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }
           }break;
        case 10:{//(CPS8/CPS8R/CPE8/CPE8R/CAX8/CAX8R)
                QList<int> sortD;
                sortD<<tmpStrList.at(1).toInt();
                sortD<<tmpStrList.at(5).toInt();
                sortD<<tmpStrList.at(2).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                QString noSorD;
                noSorD =tmpStrList.at(0)+",1,"+QString("%1,").arg(iType)+
                    tmpStrList.at(1)+","+
                    tmpStrList.at(5)+","+
                    tmpStrList.at(2);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(2).toInt();
                sortD<<tmpStrList.at(10).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(10)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }


                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(11).toInt();
                sortD<<tmpStrList.at(4).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(11)+","+
                    tmpStrList.at(4);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(12).toInt();
                sortD<<tmpStrList.at(1).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",4,"+QString("%1,").arg(iType)+
                    tmpStrList.at(4)+","+
                    tmpStrList.at(12)+","+
                    tmpStrList.at(1);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD, noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }
           }break;
        default:break;
     } 
}

const QMap<QString, QString>* InpDataSource::GetElementSurfFilterData()
{
    return &m_ElementSurfmap;
}