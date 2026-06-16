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
#include <vtkLine.h>//鐢荤嚎
#include <vtkQuadraticEdge.h>
#include <vtkPolyLine.h>
#include "CRWManage.h"
#include <QDebug>

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

    // Merge multi-line element definitions into single lines.
    // Solver uses <=16 entries per line; elements with >15 nodes span
    // continuation lines that carry node IDs without an element ID prefix.
    {
        QList<QString> merged;
        QString accumulated;
        int expectedNextElId = -1;
        for (int i = 0; i < cellPointsList.size(); ++i) {
            QString line = cellPointsList.at(i).trimmed();
            if (line.isEmpty()) continue;
            QStringList parts = line.split(",", QString::SkipEmptyParts);
            if (parts.isEmpty()) continue;
            int firstVal = parts.at(0).simplified().toInt();

            if (accumulated.isEmpty()) {
                accumulated = line;
                expectedNextElId = firstVal + 1;
            } else if (firstVal == expectedNextElId && parts.size() >= 4) {
                // Starts with the next expected element ID + enough nodes 鈫?new element
                merged.append(accumulated);
                accumulated = line;
                expectedNextElId = firstVal + 1;
            } else {
                // Continuation line (no element ID)
                accumulated += "," + line;
            }
        }
        if (!accumulated.isEmpty()) {
            merged.append(accumulated);
        }
        cellPointsList = merged;
        nCell = cellPointsList.size();
    }

    QStringList firstCell = cellPointsList.at(0).split(",", QString::SkipEmptyParts);
    CellBaseId_ = firstCell.size() > 0 ? firstCell.at(0).simplified().toInt() - 1 : 0;

    int insertedCells = 0;
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
        default: qDebug() << "InitGridFromFrd: unsupported cell type" << inpCellType; break;
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
        if (validCell) {
            wholeGrid_->InsertNextCell(cellTypeVtk, idList);
            ++insertedCells;
        }
        cell->Delete();
    }
    wholeGrid_->SetPoints(points);
    points->Delete();

    if (insertedCells == 0 || wholeGrid_->GetNumberOfCells() <= 0) {
        wholeGrid_->Delete();
        wholeGrid_ = 0;
        return false;
    }
    return true;
}
vtkVISUnstructuredGridSource* InpDataSource::GetSourceGrid()
{
    return gSource_;
}
vtkUnstructuredGrid* InpDataSource::GetWholeData(QString str)
{
	if (str.isEmpty() || str == "hell"){
		return wholeGrid_;//杩斿洖鍏ㄩ儴
	}else{
		if (disSurfSourceMap_.find(str)!=disSurfSourceMap_.end()){//have no
			return disSurfSourceMap_[str];
		}
		return wholeGrid_;
	}
    
}
vtkUnstructuredGrid* InpDataSource::GetSurfGridData()
{
    return wholeSurfGrid_;
}
// Point set source grid.
vtkVISUnstructuredGridSource* InpDataSource::GetNSetSourceGrid()
{
    return gBCSource_;
}
//--鍗曞厓闆嗗悎
vtkVISUnstructuredGridSource* InpDataSource::GetElSourceGrid()
{
    return gELSource_;
}
//--琛ㄩ潰闆嗗悎
vtkVISUnstructuredGridSource* InpDataSource::GetSurfaceSourceGrid()
{
    return gSurfaceSource_;
}
//--鏁翠釜缃戞牸
bool InpDataSource::SetInpDataSource(const QString &header)
{
    if (header.isEmpty()){ //init grid
        gSource_->SetSourceGrid(wholeGrid_);
        return true;
    }else{ //displacement grid
        return false;
    }
}
//--鑾峰彇鐐归泦鍚堢殑grid;
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
//--鑾峰彇鍗曞厓闆嗗悎鐨刧rid;
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
//--鑾峰彇琛ㄩ潰闆嗗悎鐨刧rid;
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
// Return internal cell type id for INP element type.
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
        strType=="cax4"||strType=="cax4r"){intCellType = 9;
    }else if (strType=="cps8"||strType=="cps8r"||strType=="cpe8"||strType=="cpe8r"||
        strType=="cax8"||strType=="cax8r"){intCellType = 10;
    }else if(strType=="c3d4"||strType=="c3d4t"){intCellType = 3;
    }else if(strType=="c3d10"||strType=="c3d10t"){intCellType = 6;
    }
    return intCellType;
}
//--杩涜闆嗗悎鐨勬暟鎹甅ap鏁寸悊;
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
        if(Inp->TmpNsetInps.strNSetStyle.at(i).toLower()=="generate"){GenerateSetData.iDataGenerate = 0;//鏈夐棿闅?        }else{GenerateSetData.iDataGenerate = 1;//鏃犻棿闅旓紝鐩存帴鍘荤偣
        }
        varNSetMap_.insert(NSetName.at(i),GenerateSetData);
   }

   QStringList ESetName = Inp->TmpElSetInps.strElSetName;
   
   for (int i=0;i<ESetName.size();i++){ 
       if(Inp->TmpElSetInps.strElSetStyle.at(i).toLower()=="generate"){GenerateSetData.iDataGenerate = 0;//鏈夐棿闅?       }else{GenerateSetData.iDataGenerate = 1;//鏃犻棿闅旓紝鐩存帴鍘荤偣
       }
       GenerateSetData.strDataList = Inp->TmpElSetInps.strData.at(i).split(",",QString::SkipEmptyParts);
       varElSetMap_.insert(ESetName.at(i),GenerateSetData);
   }

    //DecodeSurfaceS tmpSurfaceData;
    QList<QString> strSurfaceData = Inp->TmpSurfaceInps.strData;
    int iTmpCnt=0;
    int iBaseCnt=0;//鏁版嵁澧炲姞鍩哄噯
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
// Build point-set display grid.
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
//--鍗曞厓闆嗗悎鐨勬樉绀猴紱
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

		int m_iCelltype0 = 0;
		QStringList DataType=tmpStrList.at(i).split(",");
		int NodeNumber=DataType.size()-1;
		if(n2D3D==2){
			switch(NodeNumber){
				case 3:{m_iCelltype0 =7;}break;
			    case 4:{m_iCelltype0 =9;}break;
		     	case 6:{m_iCelltype0 =8;}break;
				case 8:{m_iCelltype0 =10;}break;
				default: qDebug() << "ELDisplacementGrid: unsupported node count" << NodeNumber; break; 
			}
		}else{
			switch(NodeNumber){
			    case 4:{m_iCelltype0 =3;}break;
			    case 6:{m_iCelltype0 =2;}break;
			    case 8:{m_iCelltype0 =1;}break;
			    case 10:{m_iCelltype0 =6;}break;
			    case 15:{m_iCelltype0 =5;}break;
			    case 20:{m_iCelltype0 =4;}break;	
				default: qDebug() << "ELDisplacementGrid: unsupported node count" << NodeNumber; break;
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
        default: qDebug() << "ELDisplacementGrid: unsupported cell type" << m_iCelltype0; break;
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
            ungrid->InsertNextCell(cellType_vtk, idList);
            cell->Delete();
        }
    }
    ungrid->SetPoints(points);
    return ungrid;
}
//--琛ㄩ潰闆嗗悎鐨勬樉绀猴紱
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
		int m_iCelltype=9;//鍗曞厓绫诲瀷,鍏堝垵濮嬪寲
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
//--鐐归泦鍚堜腑鏁版嵁杞崲
bool InpDataSource::GetNCellData(QStringList *CellListData, QString header)
{
    if (CellListData == 0) return false;
    QMap<QString, DecodeGenerateS>::Iterator it = varNSetMap_.find(header);
    if (it == varNSetMap_.end()) return false;
    DecodeGenerateS data = it.value();
    QStringList tokens;
    for (int i = 0; i < data.strDataList.size(); ++i) {
        tokens << data.strDataList.at(i).split(",", QString::SkipEmptyParts);
    }
    if (tokens.isEmpty()) return false;
    if (data.iDataGenerate == 0 && tokens.size() >= 3) {
        int start = tokens.at(0).simplified().toInt();
        int end = tokens.at(1).simplified().toInt();
        int step = tokens.at(2).simplified().toInt();
        if (step == 0) return false;
        for (int id = start; id <= end; id += step) *CellListData << QString::number(id);
    } else {
        for (int i = 0; i < tokens.size(); ++i) *CellListData << tokens.at(i).simplified();
    }
    return !CellListData->isEmpty();
}
/*
//--鍗曞厓闆嗗悎涓紝鏁版嵁杞崲
*/
bool InpDataSource::GetElCellData(QStringList *CellListData, QString header)
{
    if (CellListData == 0) return false;
    QMap<QString, DecodeGenerateS>::Iterator it = varElSetMap_.find(header);
    if (it == varElSetMap_.end()) return false;
    DecodeGenerateS data = it.value();
    QStringList tokens;
    for (int i = 0; i < data.strDataList.size(); ++i) {
        tokens << data.strDataList.at(i).split(",", QString::SkipEmptyParts);
    }
    if (tokens.isEmpty()) return false;
    QStringList elementIds;
    if (data.iDataGenerate == 0 && tokens.size() >= 3) {
        int start = tokens.at(0).simplified().toInt();
        int end = tokens.at(1).simplified().toInt();
        int step = tokens.at(2).simplified().toInt();
        if (step == 0) return false;
        for (int id = start; id <= end; id += step) elementIds << QString::number(id);
    } else {
        for (int i = 0; i < tokens.size(); ++i) elementIds << tokens.at(i).simplified();
    }
    for (int i = 0; i < elementIds.size(); ++i) {
        int elementId = elementIds.at(i).toInt();
        if (m_ElementData.find(elementId - 1) != m_ElementData.end()) {
            *CellListData << m_ElementData.find(elementId - 1).value();
        } else {
            qDebug() << "GetElCellData: element" << elementId << "not found in element data map";
        }
    }
    return !CellListData->isEmpty();
}
/*
//--琛ㄩ潰闆嗗悎涓紝鏁版嵁杞崲
*/
bool InpDataSource:: GetSurfCellData(QVector<QStringList> *CellListData,QStringList *CellNewStyle, QString header)
{
    QStringList tmpList;
    QMap<QString, QStringList>::Iterator it=varSurfaceSetMap_.begin();
    QStringList qListString;
    for (it; it != varSurfaceSetMap_.end(); it++){ //鏁版嵁杞崲
        if (header==it.key()){
           qListString=it.value();
           int nSize=qListString.size();
           
           for (int i=0;i<nSize;i++){
               QStringList strL1;
               QStringList tmpStrL=qListString.at(i).split(",",QString::SkipEmptyParts);
               if (tmpStrL.size() < 2) continue;
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
    return false;
}
//--琛ㄩ潰闆嗗悎涓幏鍙栧綋鍓嶉潰鐨勭偣;
bool InpDataSource::GetChooseData(QStringList *strRowData, QString strFName)
{
    if (strRowData == 0) return false;
    QString faceName = strFName.toLower();
    QStringList outRows;
    QStringList rows = *strRowData;
    for (int i = 0; i < rows.size(); ++i) {
        QStringList parts = rows.at(i).split(",", QString::SkipEmptyParts);
        if (parts.size() < 2) continue;
        int nodeNumber = parts.size() - 1;
        int cellType = 0;
        if (n2D3D == 2) {
            switch (nodeNumber) {
            case 3: cellType = 7; break;
            case 4: cellType = 9; break;
            case 6: cellType = 8; break;
            case 8: cellType = 10; break;
            default: qDebug() << "GetChooseData: unsupported 2D node count" << nodeNumber; continue;
            }
        } else {
            switch (nodeNumber) {
            case 4: cellType = 3; break;
            case 6: cellType = 2; break;
            case 8: cellType = 1; break;
            case 10: cellType = 6; break;
            case 15: cellType = 5; break;
            case 20: cellType = 4; break;
            default: qDebug() << "GetChooseData: unsupported 3D node count" << nodeNumber; continue;
            }
        }

        QList<int> faceNodes;
        if (cellType == 1 || cellType == 4) {
            static const int hex8Faces[6][4] = {
                {1, 2, 3, 4}, {5, 8, 7, 6}, {1, 5, 6, 2},
                {2, 6, 7, 3}, {3, 7, 8, 4}, {4, 8, 5, 1}
            };
            static const int hex20Faces[6][8] = {
                {1, 2, 3, 4, 9, 10, 11, 12},
                {5, 8, 7, 6, 16, 15, 14, 13},
                {1, 5, 6, 2, 17, 13, 18, 9},
                {2, 6, 7, 3, 18, 14, 19, 10},
                {3, 7, 8, 4, 19, 15, 20, 11},
                {4, 8, 5, 1, 20, 16, 17, 12}
            };
            int face = faceName.mid(1).toInt() - 1;
            if (face < 0 || face >= 6) continue;
            int faceSize = (cellType == 4) ? 8 : 4;
            for (int k = 0; k < faceSize; ++k) {
                int idx = (cellType == 4) ? hex20Faces[face][k] : hex8Faces[face][k];
                faceNodes << idx;
            }
        } else if (cellType == 3 || cellType == 6) {
            static const int tet4Faces[4][3] = {{1,2,3}, {1,4,2}, {2,4,3}, {3,4,1}};
            static const int tet10Faces[4][6] = {{1,2,3,5,6,7}, {1,4,2,8,9,5}, {2,4,3,9,10,6}, {3,4,1,10,8,7}};
            int face = faceName.mid(1).toInt() - 1;
            if (face < 0 || face >= 4) continue;
            int faceSize = (cellType == 6) ? 6 : 3;
            for (int k = 0; k < faceSize; ++k) {
                int idx = (cellType == 6) ? tet10Faces[face][k] : tet4Faces[face][k];
                faceNodes << idx;
            }
        } else if (cellType == 7 || cellType == 8 || cellType == 9 || cellType == 10) {
            int face = faceName.mid(1).toInt();
            int maxFace = (cellType == 7 || cellType == 8) ? 3 : 4;
            if (face <= 0 || face > maxFace) continue;
            int next = (face == maxFace) ? 1 : face + 1;
            faceNodes << face << next;
        } else if (cellType == 2 || cellType == 5) {
            static const int wedgeFaces[5][8] = {
                {1,2,3,0,7,8,9,0}, {4,5,6,0,13,14,15,0},
                {1,2,5,4,7,11,13,10}, {2,3,6,5,8,12,14,11},
                {3,1,4,6,9,10,15,12}
            };
            int face = faceName.mid(1).toInt() - 1;
            if (face < 0 || face >= 5) continue;
            int faceSize = (face < 2) ? ((cellType == 5) ? 6 : 3) : ((cellType == 5) ? 8 : 4);
            for (int k = 0; k < faceSize; ++k) {
                int idx = wedgeFaces[face][k];
                if (idx > 0) faceNodes << idx;
            }
        }

        if (faceNodes.isEmpty()) continue;
        QString row = parts.at(faceNodes.at(0)).simplified();
        for (int k = 1; k < faceNodes.size(); ++k) row += "," + parts.at(faceNodes.at(k)).simplified();
        row += "," + QString::number(cellType);
        outRows << row;
    }
    *strRowData = outRows;
    return true;
}
   
/*
inpcellType: 鍒ゆ柇浣曠鍗曞厓绫诲瀷锛?iFace:s1,s2,s3,s4,s5,s6
*/
vtkCell* InpDataSource::NewVTKCellS(int InpCellType, QString strFName)
{
    vtkCell *cell = NULL;
    switch (InpCellType) {
    case 1:
        cell = vtkQuad::New();
        break;
    case 4:
        cell = vtkQuadraticQuad::New();
        break;
    case 3:
        cell = vtkTriangle::New();
        break;
    case 6:
        cell = vtkQuadraticTriangle::New();
        break;
    case 7:
        cell = vtkLine::New();
        break;
    case 8:
        cell = vtkQuadraticEdge::New();
        break;
    case 9:
        cell = vtkLine::New();
        break;
    case 10:
        cell = vtkQuadraticEdge::New();
        break;
    case 2:
    case 5:
        if (strFName == "s1" || strFName == "s2") {
            cell = (InpCellType == 5) ? static_cast<vtkCell*>(vtkQuadraticTriangle::New()) : static_cast<vtkCell*>(vtkTriangle::New());
        } else if (strFName == "s3" || strFName == "s4" || strFName == "s5") {
            cell = (InpCellType == 5) ? static_cast<vtkCell*>(vtkQuadraticQuad::New()) : static_cast<vtkCell*>(vtkQuad::New());
        }
        break;
    default:
        qDebug() << "NewVTKCellS: unsupported cell type" << InpCellType;
        break;
    }
    return cell;
}


// Build exterior surface grid from INP element rows.
bool InpDataSource::InpRowDataToSurf(ReadInpResultS *Inp)
{
    if (Inp == 0)  return false;
    int nPoint = Inp->TmpNodeInpS.strData.size();//nodalPoint.nNum;
    PointCoordinate p; p.x = 0; p.y = 0; p.z = 0;
    vtkPoints *points = vtkPoints::New();
    if (nPoint < 1) return false;
	QStringList TmpBasePoint=(Inp->TmpNodeInpS.strData.at(0).split(","));
    if (TmpBasePoint.isEmpty()) return false;
    points->SetNumberOfPoints(nPoint+TmpBasePoint.at(0).toInt()-1);
    QString AnalysisType=Inp->TmpElInpS.strELType.mid(0,2).toLower();
    if (Inp->TmpElInpS.ElementType.isEmpty()) return false;
	if(AnalysisType=="")AnalysisType=Inp->TmpElInpS.ElementType.at(0).mid(0,2).toLower();
	if(AnalysisType=="c3")n2D3D=3;
	else n2D3D=2;
	AnalysisType.clear();
    for (int i = 0; i < nPoint; ++i)
    {
        QStringList TmpNodeData=Inp->TmpNodeInpS.strData.at(i).split(",");
       
        if (TmpNodeData.size() < (n2D3D==3 ? 4 : 3)) continue;
        p.x=TmpNodeData.at(1).toDouble();
        p.y=TmpNodeData.at(2).toDouble();
        if (n2D3D==3){
            p.z=TmpNodeData.at(3).toDouble();
        }else if(n2D3D==2){
            p.z=0;
        }
        points->SetPoint(i+TmpBasePoint.at(0).toInt()-1,p.x,p.y,p.z);
    }

    int nCell = Inp->TmpElInpS.strData.size();//Inp->elemBlock.nNum;

    QList<QString> cellPointsList=Inp->TmpElInpS.strData;//鍗曞厓缂栧彿鍜屽崟鍏冭妭鐐瑰彿

    // Merge multi-line element definitions into single lines
    {
        QList<QString> merged;
        QString accumulated;
        int expectedNextElId = -1;
        for (int i = 0; i < cellPointsList.size(); ++i) {
            QString line = cellPointsList.at(i).trimmed();
            if (line.isEmpty()) continue;
            QStringList parts = line.split(",", QString::SkipEmptyParts);
            if (parts.isEmpty()) continue;
            int firstVal = parts.at(0).simplified().toInt();

            if (accumulated.isEmpty()) {
                accumulated = line;
                expectedNextElId = firstVal + 1;
            } else if (firstVal == expectedNextElId && parts.size() >= 4) {
                merged.append(accumulated);
                accumulated = line;
                expectedNextElId = firstVal + 1;
            } else {
                accumulated += "," + line;
            }
        }
        if (!accumulated.isEmpty()) {
            merged.append(accumulated);
        }
        cellPointsList = merged;
        nCell = cellPointsList.size();
    }

    int inpCellType=9;

    for(int i = 0; i < nCell; ++i){
        QStringList DataType=cellPointsList.at(i).split(",");
        int NodeNumber=DataType.size()-1;
        if(n2D3D==2){
            switch(NodeNumber){
                case 3:{inpCellType =7;}break;
                case 4:{inpCellType =9;}break;
                case 6:{inpCellType =8;}break;
                case 8:{inpCellType =10;}break;
                default: qDebug() << "InpRowDataToSurf: unsupported 2D node count" << NodeNumber; break;
            }
        }else{
            switch(NodeNumber){
                case 4:{inpCellType =3;}break;//
                case 6:{inpCellType =2;}break;//
                case 8:{inpCellType =1;}break;//
                case 10:{inpCellType =6;}break;
                case 15:{inpCellType =5;}break;
                case 20:{inpCellType =4;}break;	
                default: qDebug() << "InpRowDataToSurf: unsupported 3D node count" << NodeNumber; break; 
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
			if (SnData.size() < 3) continue;
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
                    if ((SnData.size()-3)==8) cell = vtkQuadraticQuad::New();
                    else cell = vtkQuadraticTriangle::New();
              }break;//vtkQuadraticWedge::New();}break; 
            case 6:{cell = vtkQuadraticTriangle::New();}break;//(wait data for test)vtkQuadraticTetra::New();}break; 
            case 7:{cell = vtkLine::New();}break;//(ok) vtkTriangle::New();}break;
            case 8:{cell = vtkQuadraticEdge::New();}break;//vtkQuadraticTriangle::New();}break; 
            case 9:{cell = vtkLine::New();}break;//(ok)vtkQuad::New();}break;   
            case 10:{cell = vtkQuadraticEdge::New();}break;//vtkQuadraticQuad::New();}break;
            default: qDebug() << "InpRowDataToSurf: unsupported cell type" << inpCellType; break;
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
            if (SnData.size() < np + 3) { cell->Delete(); continue; }
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
	// Validate list size for element type (1 ID + N nodes)
	int expectedNodes = 0;
	switch(iType) {
		case 1: expectedNodes = 8; break;  // C3D8
		case 2: expectedNodes = 6; break;  // C3D6
		case 3: expectedNodes = 4; break;  // C3D4
		case 4: expectedNodes = 20; break; // C3D20
		case 5: expectedNodes = 15; break; // C3D15
		case 6: expectedNodes = 10; break; // C3D10
		case 7: expectedNodes = 3; break;  // CPS3
		case 8: expectedNodes = 6; break;  // CPS6
		case 9: expectedNodes = 4; break;  // CPS4
		case 10: expectedNodes = 8; break; // CPS8
		default: return;
	}
	if (tmpStrList.size() <= expectedNodes) return;
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
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(6).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD =tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(5)+","+
                    tmpStrList.at(8)+","+
                    tmpStrList.at(3)+","+
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
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.size();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD =tmpStrList.at(0)+",4,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(6)+","+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(8).toInt();
                sortD<<tmpStrList.at(4).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.size();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",5,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(3)+","+
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
            const int faces[6][8] = {
                {1, 2, 3, 4, 9, 10, 11, 12},
                {5, 8, 7, 6, 16, 15, 14, 13},
                {1, 5, 6, 2, 17, 13, 18, 9},
                {2, 6, 7, 3, 18, 14, 19, 10},
                {3, 7, 8, 4, 19, 15, 20, 11},
                {4, 8, 5, 1, 20, 16, 17, 12}
            };
            for (int face = 0; face < 6; ++face) {
                QList<int> sortD;
                for (int k = 0; k < 8; ++k) sortD << tmpStrList.at(faces[face][k]).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD = QString("%1").arg(sortD.at(0));
                for (int kk = 1; kk < sortD.count(); ++kk) strSortD += "," + QString("%1").arg(sortD.at(kk));
                QString noSorD = tmpStrList.at(0) + "," + QString::number(face + 1) + "," + QString("%1,").arg(iType);
                noSorD += tmpStrList.at(faces[face][0]);
                for (int k = 1; k < 8; ++k) noSorD += "," + tmpStrList.at(faces[face][k]);
                if (m_ElementSurfmap.find(strSortD) == m_ElementSurfmap.end()) {
                    m_ElementSurfmap.insert(strSortD, noSorD);
                } else {
                    m_ElementSurfmap.remove(strSortD);
                }
            }
            break;
           /* QList<int> sortD;
            sortD<<tmpStrList.at(1).toInt();
            sortD<<tmpStrList.at(9).toInt();
            sortD<<tmpStrList.at(2).toInt();
            sortD<<tmpStrList.at(6).toInt();
            sortD<<tmpStrList.at(3).toInt();
            sortD<<tmpStrList.at(7).toInt();
            sortD<<tmpStrList.at(4).toInt();
            sortD<<tmpStrList.at(8).toInt();
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
                tmpStrList.at(6)+","+
                tmpStrList.at(3)+","+
                tmpStrList.at(7)+","+
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
            sortD<<tmpStrList.at(3).toInt();
            sortD<<tmpStrList.at(6).toInt();
            qSort(sortD.begin(),sortD.end());
            strSortD=QString("%1").arg(sortD.at(0));
            for (int kk=1;kk<sortD.count();kk++){
                strSortD+=","+QString("%1").arg(sortD.at(kk));
            }
            noSorD =tmpStrList.at(0)+",2,"+
                tmpStrList.at(5)+","+
                tmpStrList.at(8)+","+
                tmpStrList.at(3)+","+
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
            sortD<<tmpStrList.at(3).toInt();
            sortD<<tmpStrList.at(3).toInt();
            qSort(sortD.begin(),sortD.end());
            strSortD=QString("%1").arg(sortD.at(0));
            for (int kk=1;kk<sortD.size();kk++){
                strSortD+=","+QString("%1").arg(sortD.at(kk));
            }
            noSorD =tmpStrList.at(0)+",4,"+
                tmpStrList.at(2)+","+
                tmpStrList.at(6)+","+
                tmpStrList.at(3)+","+
                tmpStrList.at(3);
            if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                m_ElementSurfmap.insert(strSortD,noSorD);
            }else{
                m_ElementSurfmap.remove(strSortD);
            }

            sortD.clear();
            sortD<<tmpStrList.at(3).toInt();
            sortD<<tmpStrList.at(3).toInt();
            sortD<<tmpStrList.at(8).toInt();
            sortD<<tmpStrList.at(4).toInt();
            qSort(sortD.begin(),sortD.end());
            strSortD=QString("%1").arg(sortD.at(0));
            for (int kk=1;kk<sortD.size();kk++){
                strSortD+=","+QString("%1").arg(sortD.at(kk));
            }
            noSorD = tmpStrList.at(0)+",5,"+
                tmpStrList.at(3)+","+
                tmpStrList.at(3)+","+
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
        case 5:{// C3D15: two quadratic triangles and three quadratic quads.
            const int triFaces[2][6] = {
                {1, 2, 3, 7, 8, 9},
                {4, 6, 5, 12, 11, 10}
            };
            const int quadFaces[3][8] = {
                {1, 4, 5, 2, 10, 13, 11, 7},
                {2, 5, 6, 3, 11, 14, 12, 8},
                {3, 6, 4, 1, 12, 15, 10, 9}
            };
            for (int face = 0; face < 2; ++face) {
                QList<int> sortD;
                for (int k = 0; k < 6; ++k) sortD << tmpStrList.at(triFaces[face][k]).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD = QString("%1").arg(sortD.at(0));
                for (int kk = 1; kk < sortD.count(); ++kk) strSortD += "," + QString("%1").arg(sortD.at(kk));
                QString noSorD = tmpStrList.at(0) + "," + QString::number(face + 1) + "," + QString("%1,").arg(iType);
                noSorD += tmpStrList.at(triFaces[face][0]);
                for (int k = 1; k < 6; ++k) noSorD += "," + tmpStrList.at(triFaces[face][k]);
                if (m_ElementSurfmap.find(strSortD) == m_ElementSurfmap.end()) {
                    m_ElementSurfmap.insert(strSortD, noSorD);
                } else {
                    m_ElementSurfmap.remove(strSortD);
                }
            }
            for (int face = 0; face < 3; ++face) {
                QList<int> sortD;
                for (int k = 0; k < 8; ++k) sortD << tmpStrList.at(quadFaces[face][k]).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD = QString("%1").arg(sortD.at(0));
                for (int kk = 1; kk < sortD.count(); ++kk) strSortD += "," + QString("%1").arg(sortD.at(kk));
                QString noSorD = tmpStrList.at(0) + "," + QString::number(face + 3) + "," + QString("%1,").arg(iType);
                noSorD += tmpStrList.at(quadFaces[face][0]);
                for (int k = 1; k < 8; ++k) noSorD += "," + tmpStrList.at(quadFaces[face][k]);
                if (m_ElementSurfmap.find(strSortD) == m_ElementSurfmap.end()) {
                    m_ElementSurfmap.insert(strSortD, noSorD);
                } else {
                    m_ElementSurfmap.remove(strSortD);
                }
            }
            }break;
		case 6:{//c3d10
				QList<int> sortD;
				sortD<<tmpStrList.at(1).toInt();
				sortD<<tmpStrList.at(5).toInt();
				sortD<<tmpStrList.at(2).toInt();
				sortD<<tmpStrList.at(6).toInt();
				sortD<<tmpStrList.at(3).toInt();
			    sortD<<tmpStrList.at(3).toInt();
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
				sortD<<tmpStrList.at(6).toInt();
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
					tmpStrList.at(6)+","+
					tmpStrList.at(3)+","+
					tmpStrList.at(6);
				if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
					m_ElementSurfmap.insert(strSortD,noSorD);
				}else{
					m_ElementSurfmap.remove(strSortD);
				}

				sortD.clear();
				sortD<<tmpStrList.at(3).toInt();
				sortD<<tmpStrList.at(6).toInt();
				sortD<<tmpStrList.at(4).toInt();
				sortD<<tmpStrList.at(8).toInt();
				sortD<<tmpStrList.at(1).toInt();
				sortD<<tmpStrList.at(3).toInt();
				qSort(sortD.begin(),sortD.end());
				strSortD=QString("%1").arg(sortD.at(0));
				for (int kk=1;kk<sortD.count();kk++){
					strSortD+=","+QString("%1").arg(sortD.at(kk));
				}
				noSorD = tmpStrList.at(0)+",4,"+QString("%1,").arg(iType)+
					tmpStrList.at(3)+","+
					tmpStrList.at(6)+","+
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
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(2).toInt();
                qSort(sortD.begin(), sortD.end());
                QString strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                QString noSorD;
                noSorD =tmpStrList.at(0)+",1,"+QString("%1,").arg(iType)+
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
                sortD<<tmpStrList.at(5).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(5)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }


                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(6).toInt();
                sortD<<tmpStrList.at(1).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(6)+","+
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
                sortD<<tmpStrList.at(6).toInt();
                sortD<<tmpStrList.at(3).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",2,"+QString("%1,").arg(iType)+
                    tmpStrList.at(2)+","+
                    tmpStrList.at(6)+","+
                    tmpStrList.at(3);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }


                sortD.clear();
                sortD<<tmpStrList.at(3).toInt();
                sortD<<tmpStrList.at(7).toInt();
                sortD<<tmpStrList.at(4).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",3,"+QString("%1,").arg(iType)+
                    tmpStrList.at(3)+","+
                    tmpStrList.at(7)+","+
                    tmpStrList.at(4);
                if(m_ElementSurfmap.find(strSortD)==m_ElementSurfmap.end()){
                    m_ElementSurfmap.insert(strSortD,noSorD);
                }else{
                    m_ElementSurfmap.remove(strSortD);
                }

                sortD.clear();
                sortD<<tmpStrList.at(4).toInt();
                sortD<<tmpStrList.at(8).toInt();
                sortD<<tmpStrList.at(1).toInt();
                qSort(sortD.begin(),sortD.end());
                strSortD=QString("%1").arg(sortD.at(0));
                for (int kk=1;kk<sortD.count();kk++){
                    strSortD+=","+QString("%1").arg(sortD.at(kk));
                }
                noSorD = tmpStrList.at(0)+",4,"+QString("%1,").arg(iType)+
                    tmpStrList.at(4)+","+
                    tmpStrList.at(8)+","+
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
