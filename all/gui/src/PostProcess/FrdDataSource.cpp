#include <QSet>
#include <QtAlgorithms>
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
#include "../QFrdDataPro.h"
#include "FrdDataSource.h"

FrdDataSource::FrdDataSource()
{
    frdPoints_ = 0;
    gSource_ = vtkVISUnstructuredGridSource::New();
    this->scalarName_ = 0;
    pointResults_ = 0;
    this->scalarRange_ = 0;
	varHeaderScalarMap_.clear();
}

FrdDataSource::~FrdDataSource()
{
    if (frdPoints_)
    {
        frdPoints_->Delete();
        frdPoints_ = 0;
    }
    gridIdsVec_.clear();
    for (map<int, vtkUnstructuredGrid*>::iterator it = idGridMap_.begin(); it != idGridMap_.end(); ++it)
    {
        it->second->Delete();
    }
    idGridMap_.clear();

    for (int i = 0; i < this->scalarNumber_; ++i)
    {
        if (this->scalarName_)  delete[] this->scalarName_[i];
        if (this->scalarRange_)  delete[] this->scalarRange_[i];
        if (pointResults_)  pointResults_[i]->Delete();
    }
    delete[] pointResults_;
    pointResults_ = 0;
    delete[] this->scalarName_;
    this->scalarName_ = 0;
    delete[] this->scalarRange_;
    this->scalarRange_ = 0;

    gSource_->Delete();
    varHeaderScalarMap_.clear();
    headerTimeStepMap_.clear();
    StepTimeIncMap_.clear();
    varHeaderList_.clear();

    for (map<QString, map<int, vtkUnstructuredGrid*> >::iterator it = dispGridsMap_.begin(); it != dispGridsMap_.end(); ++it)
    {
        for (map<int, vtkUnstructuredGrid*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            it2->second->Delete();
        }
        it->second.clear();
    }
    dispGridsMap_.clear();
}
///////////
bool FrdDataSource::Step1_InitGridPoints(_PointXYZ_S *pStruct)
{
    if (pStruct == 0)  return false;
    int nPoint = pStruct->PointId.size();
    frdPoints_ = vtkPoints::New();
    frdPoints_->SetNumberOfPoints(nPoint);
    for (int i = 0; i < nPoint; ++i)
    {
        frdPoints_->SetPoint(i, pStruct->x.at(i), pStruct->y.at(i), pStruct->z.at(i));
        int id = pStruct->PointId.at(i);
        pointIdMap_[id] = i;
    }
    return true;
}

bool FrdDataSource::Step2_InitGridCells(_RESULT_ELEM_S *cStruct)
{
    if (cStruct == 0)  return false;
    if (frdPoints_ == 0)  return false;
    QSet<int> gridIdSet = cStruct->DataElem1PointMats.toSet();
    for (QSet<int>::iterator it = gridIdSet.begin(); it != gridIdSet.end(); ++it)
    {
        gridIdsVec_.push_back(*it);
        int nCell = 0;
        qCount(cStruct->DataElem1PointMats.begin(), cStruct->DataElem1PointMats.end(), *it, nCell);
        vtkUnstructuredGrid *g = vtkUnstructuredGrid::New();
        g->SetPoints(frdPoints_);
        g->Allocate(nCell);
        idGridMap_[*it] = g;
    }
    frdPoints_->Delete();
    frdPoints_ = 0;

    int nCell = cStruct->nNum;
    for (int i = 0; i < nCell; ++i)
    {
        int frdCellType = cStruct->DataElem1PointStyle.at(i);
        vtkCell *cell = NewVTKCell(frdCellType);
        if (cell)
        {
            int cellType_vtk = cell->GetCellType();
            int np = cell->GetNumberOfPoints();
            vtkIdList *idList = cell->GetPointIds();
            for (int j = 0; j < np; ++j)
            {
                int id = cStruct->DataElem2.at(i)[j];
                idList->SetId(j, pointIdMap_[id]);
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
            int gridId = cStruct->DataElem1PointMats.at(i);
            idGridMap_[gridId]->InsertNextCell(cellType_vtk, idList);
            cell->Delete();
        }
    }
    return true;
}

bool FrdDataSource::Step3_InitResults(_RESULT_STEP_S_ *rStruct)
{
    if (rStruct == 0)  return false;
    if (idGridMap_.size() == 0)  return false;
    /////save scalar name
    QString step(rStruct->nodeResultBlock.strName);  //eg. L101
    QString varName(rStruct->nodeResultBlock.strAttrHeaderName); //eg. DISP
    QString key(step+"-"+varName);
    varHeaderScalarMap_[key] = rStruct->nodeResultBlock.strComptHeaderName; //D1 D2 D3 ALL  
    if (varHeaderList_.indexOf(step)==-1){
        varHeaderList_.append(step);
    }
    
    headerTimeStepMap_[key] = QString::number(rStruct->nodeResultBlock.strTimeVal);
    StepTimeIncMap_[key].dataTime = QString::number(rStruct->nodeResultBlock.strTimeVal);
    StepTimeIncMap_[key].dataStep = QString::number(rStruct->paraHeader.strStep);
    StepTimeIncMap_[key].dataInc = QString::number(rStruct->paraHeader.strInc);
    int pointNum = rStruct->nodeResultBlock.strNum;
    int varCount = rStruct->nodeResultBlock.strAttrNum;
    for (int i = 0; i < varCount; ++i)
    {
        QString scalar(rStruct->nodeResultBlock.strComptHeaderName.at(i));
        QString scalar_name(key+":"+scalar);
        char *p = new char[scalar_name.size()+1];
        strcpy(p, scalar_name.toLatin1().data());
        scalarVec_.push_back(p);

        ////save scalar value
        vtkDoubleArray *darry = vtkDoubleArray::New();
        darry->SetName(scalar_name.toLatin1().data());
        darry->SetNumberOfValues(pointIdMap_.size());//(pointNum);
        if (scalar.compare("all", Qt::CaseInsensitive) != 0)
        {
            for (int j = 0; j < pointNum; ++j)
            {
                int id = rStruct->nodeResultBlock.dataIndex.at(j);
                vtkIdType first1=pointIdMap_[id];
                double second=rStruct->nodeResultBlock.strDataRecord.at(j)[i];       
                darry->SetValue(first1, second);                 
            }
        }
        else
        {
            int s = resultVec_.size();
            vtkDoubleArray *darry1 = resultVec_[s-3];
            vtkDoubleArray *darry2 = resultVec_[s-2];
            vtkDoubleArray *darry3 = resultVec_[s-1];
            for (int j = 0; j < pointNum; ++j)
            {
                int id = pointIdMap_[rStruct->nodeResultBlock.dataIndex.at(j)];
                darry->SetValue(id, darry1->GetValue(id)+darry2->GetValue(id)+darry3->GetValue(id));
            }
        }
        resultVec_.push_back(darry);
    }
    return true;
}

bool FrdDataSource::Step4_SetupFrd()
{
    pointIdMap_.clear();  //clean unused data.
    if (idGridMap_.size() == 0)  return false;
    this->scalarNumber_ = scalarVec_.size();
    if (this->scalarNumber_ < 1)  return false;
    this->scalarName_ = new const char*[this->scalarNumber_];
    pointResults_ = new vtkDoubleArray*[this->scalarNumber_];
    this->scalarRange_ = new double*[this->scalarNumber_];
    for (int i = 0; i < this->scalarNumber_; ++i)
    {
        this->scalarName_[i] = scalarVec_.at(i);
        pointResults_[i] = resultVec_.at(i);
        this->scalarRange_[i] = new double[2];
        pointResults_[i]->GetRange(this->scalarRange_[i]);
    }
    gSource_->SetScalarNumber(this->scalarNumber_);
    gSource_->SetScalarsName(this->scalarName_);
    gSource_->SetScalarsSource(pointResults_);
    gSource_->SetScalarsRange(this->scalarRange_);

    scalarVec_.clear();
    resultVec_.clear();
    return true;
}
///////////

#include <vtkLabeledDataMapper.h>
#include <vtkTextProperty.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkVectorText.h>
vtkCell* FrdDataSource::NewVTKCell(int frdCellType)
{
    vtkCell *cell = 0;
    switch (frdCellType)
    {
        case 1:
        {
            cell = vtkHexahedron::New();
            break;
        }
        case 2:
        {
            cell = vtkWedge::New();
            break;
        }
        case 3:
        {
            cell = vtkTetra::New();
            break;
        }
        case 4:
        {
            cell = vtkQuadraticHexahedron::New();
            break;
        }
        case 5:
        {
            cell = vtkQuadraticWedge::New();
            break;
        }
        case 6:
        {
            cell = vtkQuadraticTetra::New();
            break;
        }
        case 7:
        {
            cell = vtkTriangle::New();
            break;
        }
        case 8:
        {
            cell = vtkQuadraticTriangle::New();
            break;
        }
        case 9:
        {
            cell = vtkQuad::New();
            break;
        }
        case 10:
        {
            cell = vtkQuadraticQuad::New();
            break;
        }
        default:
        {
        }
    }
    return cell;
}

vtkVISUnstructuredGridSource* FrdDataSource::GetSourceGrid()
{
    return gSource_;
}

bool FrdDataSource::SetFrdDataSource(int gridId, const QString &header)
{
    if (header.isEmpty()) //init grid
    {
        if (idGridMap_.find(gridId) == idGridMap_.end())  return false;
        gSource_->SetSourceGrid(idGridMap_[gridId]);
        return true;
    }
    else //displacement grid
    {
        int exist = 1;
        if (dispGridsMap_.find(header) == dispGridsMap_.end())
        {
            exist = 0;
        }
        else
        {
            if (dispGridsMap_[header].find(gridId) == dispGridsMap_[header].end())
            {
                exist = -1;
            }
        }
        if (exist != 1)
        {
            vtkUnstructuredGrid *g = BuildDisplacementGrid(gridId, header);
            if (g)
            {
                if (exist == 0)
                {
                    map<int, vtkUnstructuredGrid*> obj;
                    dispGridsMap_[header] = obj;
                }
                dispGridsMap_[header][gridId] = g;
            }
            else return false;
        }
        gSource_->SetSourceGrid(dispGridsMap_[header][gridId]);
        return true;
    }
}

vtkUnstructuredGrid* FrdDataSource::BuildDisplacementGrid(int gridId, const QString &header)
{
    if (idGridMap_.find(gridId) == idGridMap_.end())  return 0;
    if (!header.endsWith("DISP"))  return 0;
    if (varHeaderScalarMap_.find(header) == varHeaderScalarMap_.end())  //no specified displacement result data.
    {
        return 0;
    }
    if (varHeaderScalarMap_[header].size() < 3)  return 0;
    QString d1(QString("%1:%2").arg(header).arg(varHeaderScalarMap_[header].at(0)));
    int id1 = gSource_->GetScalarIndex(d1.toLatin1().constData());
    if (id1 < 0)  return 0;
    int id2 = id1 + 1;
    int id3 = id1 + 2;
    vtkUnstructuredGrid *ungrid = vtkUnstructuredGrid::New();
    ungrid->DeepCopy(idGridMap_[gridId]);
    vtkPoints *points = ungrid->GetPoints();
    int pointNum = points->GetNumberOfPoints();
    for (int i = 0; i < pointNum; ++i)
    {
        double xyz[3];
        points->GetPoint(i, xyz);
        double dx = pointResults_[id1]->GetValue(i);
        double dy = pointResults_[id2]->GetValue(i);
        double dz = pointResults_[id3]->GetValue(i);
        points->SetPoint(i, xyz[0]+dx, xyz[1]+dy, xyz[2]+dz);
    }
    return ungrid;
}

const map<QString, QStringList>* FrdDataSource::GetScalarInfo()
{
	if (&varHeaderScalarMap_!=0){
		return &varHeaderScalarMap_;
	}else{ 
		return 0;
	}
    
}

const vector<int>* FrdDataSource::GetGridIds()
{
    return &gridIdsVec_;
}

const QVector<QString>* FrdDataSource::GetScalarVectorInfo()
{
    return &varHeaderList_;
}

bool FrdDataSource::IsScalarNameValid(const QString &scalar)
{
    int id = gSource_->GetScalarIndex(scalar.toLatin1().constData());
    return id >= 0;
}

bool FrdDataSource::IsScalarValuesDiff(const QString &scalar)
{
    if (this->scalarRange_ == 0)  return false;
    if (IsScalarNameValid(scalar) == false)  return false;
    double *range = gSource_->GetScalarRange(scalar.toLatin1().data());
    if (range == 0)  return false;
    if (qFuzzyCompare(range[1]-range[0]+1.0, 1.0))  return false;
    return true;
}

map<double, double> FrdDataSource::GetPointScalar_TimeValueMap(int pointId, const QString &scalarName)
{
    map<double, double> resultMap;
    int istyle =scalarName.split("-").size();
    if (istyle>2){//--position
        for (map<QString, QStringList>::const_iterator cit = varHeaderScalarMap_.begin(); cit != varHeaderScalarMap_.end(); ++cit)
        {
            QString header(cit->first);
            QStringList headerListTmp(cit->second);
            QStringList HeaderList;HeaderList.clear();
            for (int kk=0;kk<headerListTmp.size();kk++)
            {
                HeaderList<<header+"-"+headerListTmp.at(kk);
            } 
            if (HeaderList.contains(scalarName)){  
                double time = headerTimeStepMap_[header].toDouble();
                QString scalar(QString("%1:%2").arg(header).arg(scalarName.split("-").at(2)));
                int scalarId = gSource_->GetScalarIndex(scalar.toLatin1().constData());
                double value = pointResults_[scalarId]->GetValue(pointId);
                resultMap[time] = value;
            }
        }
    }else{//--time
        for (map<QString, QStringList>::const_iterator cit = varHeaderScalarMap_.begin(); cit != varHeaderScalarMap_.end(); ++cit)
        {
            QString header(cit->first);
            QStringList varName=cit->second;
            QStringList varTmp;varTmp.clear();
            
            for (int kk=0;kk<varName.size();kk++){
                varTmp<<header.split("-").at(1)+"-"+varName.at(kk);
            }
            
            
            if (varTmp.contains(scalarName)){  
                double time = headerTimeStepMap_[header].toDouble();
                QString scalar(QString("%1:%2").arg(header).arg(scalarName.split("-").at(1)));
                int scalarId = gSource_->GetScalarIndex(scalar.toLatin1().constData());
                double value = pointResults_[scalarId]->GetValue(pointId);
                resultMap[time] = value;
            }
        }
    }
    return resultMap;
}
TextStepIncTimeS FrdDataSource::GetStepTimeIncMap(int pointId, const QString &scalarName)
{
    TextStepIncTimeS ttt;
    QString scalarText;
    scalarText=scalarName.split(":").first();
    for (map<QString, TextStepIncTimeS>::const_iterator cit =StepTimeIncMap_.begin();cit !=StepTimeIncMap_.end();++cit)
    {
        if (cit->first.contains(scalarText))
        {
            return StepTimeIncMap_[scalarText];
        }
    }
    
    return ttt;
}

int FrdDataSource::GetPointID(QString strLabel,double x,double y,double z)
{
    int id=-1; 
    if (strLabel.isEmpty()) return id;
    int gridId = gridIdsVec_[0];//use any grid is ok, all grids share the same points.
    if (strLabel == "undeformed"){//原始网格
        vtkPoints *points = idGridMap_[gridId]->GetPoints();
        int pointNum = points->GetNumberOfPoints();
        for (int i = 0; i < pointNum; ++i)
        {
            double xyz[3];
            points->GetPoint(i, xyz);
            if (fabs(xyz[0]-x)<0.00001 &&fabs(xyz[1]-y)<0.00001 && fabs(xyz[2]-z)<0.00001){
                id = i+1;
                break;
            } 
        }
        return id;
    }else{//变形后网格
        strLabel=strLabel.left(strLabel.indexOf("-"))+"-DISP";
        if (dispGridsMap_.find(strLabel) == dispGridsMap_.end()){
            return   id;
        }
        vtkPoints *points = dispGridsMap_[strLabel][gridId]->GetPoints();
        int pointNum = points->GetNumberOfPoints();
        for (int i = 0; i < pointNum; ++i)
        {
            double xyz[3];
            points->GetPoint(i, xyz);
            if (fabs(xyz[0]-x)<0.00001 &&fabs(xyz[1]-y)<0.00001 && fabs(xyz[2]-z)<0.00001){
                id = i+1;
                break;
            } 
        }
        return id;
    } 

   return id;
}

