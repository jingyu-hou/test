#include <vtkRenderer.h>
#include <vtkVISUnShadeMesh.h>
#include <vtkVISUnEdge.h>
#include <vtkVISUnContour.h>
//#include <QVTKUnClip.h>
//#include <vtkVISUnSlice.h>
#include "FrdDataSource.h"
#include "FrdDataVIS.h"
#include "Information_Widget.h"
#include <vtkCallbackCommand.h>

FrdDataVIS::FrdDataVIS()
{
    renderer_ = 0;
    renWin_ = 0;
    frdSource_ = 0;
    scalarResultLoaded_ = false;
    m_OutLineS.clear();
	vtkWriter_=0;
	ImageFilter_=0;
	cutZoneHeaderMap_.clear();
}

FrdDataVIS::~FrdDataVIS()
{
    Clean();
}

void FrdDataVIS::InitRenderer(vtkRenderer *renderer)
{
    renderer_ = renderer;
    renWin_ = renderer_->GetRenderWindow();
}

vtkRenderer* FrdDataVIS::GetBindedRenderer()
{
    return renderer_;
}

void FrdDataVIS::Update()
{
    if (renWin_)  renWin_->Render();
}

void FrdDataVIS::Clean()
{
    if (frdSource_)
    {
        delete frdSource_;
        frdSource_ = 0;
    }
    for (map<int, vtkVISUnShadeMesh*>::iterator it = shadeMap_.begin(); it != shadeMap_.end(); ++it)
    {
        it->second->DeleteObjects();
        it->second->Delete();
    }
    shadeMap_.clear();

    for (map<int, vtkVISUnShadeMesh*>::iterator it = meshMap_.begin(); it != meshMap_.end(); ++it)
    {
        it->second->DeleteObjects();
        it->second->Delete();
    }
    meshMap_.clear();

    for (map<int, vtkVISUnShadeMesh*>::iterator it = allEdgeMeshMap_.begin(); it != allEdgeMeshMap_.end(); ++it)
    {
        it->second->DeleteObjects();
        it->second->Delete();
    }
    allEdgeMeshMap_.clear();

    for (map<int, vtkVISUnEdge*>::iterator it = outlineMap_.begin(); it != outlineMap_.end(); ++it)
    {
        it->second->DeleteObjects();
        it->second->Delete();
    }
    outlineMap_.clear();

    scalarResultLoaded_ = false;
    for (map<QString, map<int, vtkVISUnShadeMesh*> >::iterator it = headerDisplacementsMap_.begin(); it != headerDisplacementsMap_.end(); ++it)
    {
        for (map<int, vtkVISUnShadeMesh*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            it2->second->DeleteObjects();
            it2->second->Delete();
        }
        it->second.clear();
    }
    headerDisplacementsMap_.clear();

    for (map<QString, map<int, vtkVISUnContour*> >::iterator it = headerContoursMap_.begin(); it != headerContoursMap_.end(); ++it)
    {
        for (map<int, vtkVISUnContour*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            it2->second->DeleteObjects();
            it2->second->Delete();
        }
        it->second.clear();
    }
    headerContoursMap_.clear();

    PointVisibility(false);
    QMapIterator<int,HisPointS> it(m_PointProp);
    while (it.hasNext()){
        it.next();      
        m_PointProp.remove(it.key());
    }
    m_PointProp.clear();

	ImageFilter_=0;
	vtkWriter_=0;
	cutZoneHeaderMap_.clear();
}

//////////////////////////
bool FrdDataVIS::Step1_LoadFrdPoints(_PointXYZ_S *pStruct)  //run 1 time
{
    Clean();
    frdSource_ = new FrdDataSource;
    return frdSource_->Step1_InitGridPoints(pStruct);
}

bool FrdDataVIS::Step2_LoadFrdCells(_RESULT_ELEM_S *cStruct)  //run 1 time
{
    if (frdSource_ == 0)  return false;
    return frdSource_->Step2_InitGridCells(cStruct);
}

bool FrdDataVIS::Step3_LoadFrdResults(_RESULT_STEP_S_ *rStruct)  //run in loops
{
    if (frdSource_ == 0)  return false;
    return frdSource_->Step3_InitResults(rStruct);
}

bool FrdDataVIS::Step4_SetupFrd()   //run 1 time
{
    if (frdSource_ == 0)  return false;
    scalarResultLoaded_ = frdSource_->Step4_SetupFrd(); 
    return scalarResultLoaded_;
}
//////////////////////////

void FrdDataVIS::ShallowCopyFrdData(FrdDataVIS *source)
{
    this->frdSource_ = source->frdSource_;
}
void FrdDataVIS::SetShadeVisible(int gridId, bool visible, VTKColorS tmpClr)
{
    if (shadeMap_.find(gridId) == shadeMap_.end())
    {
        vtkVISUnShadeMesh *shade = CreateShadeMeshObject(gridId, 1);
		// vtkVISUnShadeMesh *shade = CreateShadeMeshObject(gridId, 2);//20210910
        if (shade == 0)  return;
        shadeMap_[gridId] = shade;
    }
    if (visible)
    {
        shadeMap_[gridId]->SetColor(tmpClr.r,tmpClr.g,tmpClr.b);
        //shadeMap_[gridId]->SetColor(0.8, 0.8, 0.8);//qy modify 2020-3-31
        shadeMap_[gridId]->ShowOn();
        //renderer_->ResetCamera();qy 2021-3-1
    }
    else  shadeMap_[gridId]->ShowOff();
}
//--reset camera
void FrdDataVIS::ResetCamera()
{
    renderer_->ResetCamera();
}
//--add AVI begin
int FrdDataVIS::StartAVI(QString name,int rate)
{
	//if (frdSource_ == 0)  return;
	if (ImageFilter_==0){
		ImageFilter_=vtkWindowToImageFilter::New(); 
	}
	
	ImageFilter_->SetInput(renderer_->GetRenderWindow());
	//ImageFilter_->SetInputBufferTypeToRGBA();
	//ImageFilter_->ReadFrontBufferOff();
	ImageFilter_->Update();
 
	if (vtkWriter_==0){
		//vtkWriter_= QVTKAVI::New();//vtkAVIWriter::New();
		#ifdef _MSC_VER
			vtkWriter_= vtkAVIWriter::New();
		#else
			vtkWriter_= vtkFFMPEGWriter::New();
		#endif
	
	}

	vtkWriter_->SetInput(ImageFilter_->GetOutput());
	QString nowName = vtkWriter_->GetFileName();

	if (nowName!=""){
		Information_Widget::GetInstance()->ShowInformation("movie is recording...");
		//int rate=vtkWriter_->GetRate();
		//vtkWriter_->SetRate(2000);
		return 1;//已经启动
	}else{
		if (name=="")
			name ="WelCMEtest.avi";
		if (!name.contains(".avi",Qt::CaseSensitive))
			name+=".avi";
		vtkWriter_->SetFileName(name.toAscii().data());
		vtkWriter_->Start();//第一次启动
		//vtkWriter_->SetRate(2000);
		//vtkWriter_->SetRate(5000);//(rate);
		Information_Widget::GetInstance()->ShowInformation("movie is ready");
		return 2;
	}
}
void FrdDataVIS::ModifiedAVI()
{
	if (frdSource_ == 0)  return;
	if(vtkWriter_)
	{
		//for (int kk=0;kk<5;kk++)
		//{
			ImageFilter_->Modified();
			
			vtkWriter_->Write();
		//}
		
	}
}
void FrdDataVIS::EndAVI()
{
	if (frdSource_ == 0)  return;
	if(vtkWriter_)
	{
		vtkWriter_->End();
		Information_Widget::GetInstance()->ShowInformation("movie record ok");
		vtkWriter_=0;
	}
}
//--add avi end

vtkVISUnShadeMesh* FrdDataVIS::CreateShadeMeshObject(int gridId, int type)
{
    if (frdSource_ == 0)  return 0;
    if (frdSource_->SetFrdDataSource(gridId) == false)  return 0;
    vtkVISUnShadeMesh *visObj = vtkVISUnShadeMesh::New();
    visObj->SetRenderTo(renderer_, renWin_);
    visObj->SetDataSource(frdSource_->GetSourceGrid());
    if (type == 0)       visObj->CreateShadeMeshSurfaceDisplay(0);  //mesh
    else if (type == 1)  visObj->CreateShadeMeshSurfaceDisplay(1);  //shade
    else if (type == 2)  visObj->CreateShadeMeshDisplay(0);  //all edges
    return visObj;
}

void FrdDataVIS::SetMeshVisible(int gridId, bool visible, VTKColorS tmpClr)
{
    if (meshMap_.find(gridId) == meshMap_.end())
    {
        vtkVISUnShadeMesh *mesh = CreateShadeMeshObject(gridId, 0);
        if (mesh == 0)  return;
        meshMap_[gridId] = mesh;
    }
    if (visible)
    {
        //meshMap_[gridId]->SetColor(0,0,0);//(0, 1, 0); qy modify 2020-3-31
        meshMap_[gridId]->SetColor(tmpClr.r,tmpClr.g,tmpClr.b);
        meshMap_[gridId]->ShowOn();
        //renderer_->ResetCamera();qy 2021-3-1
    }
    else  meshMap_[gridId]->ShowOff(); 
}

void FrdDataVIS::SetAllEdgeMeshVisible(int gridId, bool visible, VTKColorS tmpClr)
{
    if (allEdgeMeshMap_.find(gridId) == allEdgeMeshMap_.end())
    {
        vtkVISUnShadeMesh *mesh = CreateShadeMeshObject(gridId, 2);
        if (mesh == 0)  return;
        allEdgeMeshMap_[gridId] = mesh;
    }
    if (visible)
    {
        //allEdgeMeshMap_[gridId]->SetColor(0, 1, 0);//qy modify 2020-3-31
        allEdgeMeshMap_[gridId]->SetColor(tmpClr.r,tmpClr.g,tmpClr.b);
        allEdgeMeshMap_[gridId]->ShowOn();
        //renderer_->ResetCamera();qy 2021-3-1
    }
    else  allEdgeMeshMap_[gridId]->ShowOff();
}

void FrdDataVIS::SetOutlineVisible(int gridId, bool visible, VTKColorS tmpClr)
{
    if (outlineMap_.find(gridId) == outlineMap_.end())
    {
        vtkVISUnEdge *outline = CreateOutlineObject(gridId);
        if (outline == 0)  return;
        outlineMap_[gridId] = outline;
    }
    if (visible)
    {
        //outlineMap_[gridId]->SetColor(0, 1, 0);//qy modify 2020-3-31
        outlineMap_[gridId]->SetColor(tmpClr.r,tmpClr.g,tmpClr.b);
        outlineMap_[gridId]->ShowOn();
        //renderer_->ResetCamera();qy 2021-3-1
    }
    else  outlineMap_[gridId]->ShowOff();
}

vtkVISUnEdge* FrdDataVIS::CreateOutlineObject(int gridId)
{
    if (frdSource_ == 0)  return 0;
    if (frdSource_->SetFrdDataSource(gridId) == false)  return 0;
    //createActor();
    vtkVISUnEdge *visObj = vtkVISUnEdge::New();
    visObj->SetRenderTo(renderer_, renWin_);
    visObj->SetDataSource(frdSource_->GetSourceGrid());
    visObj->CreateEdgeDisplay();
    return visObj;
}

const map<QString, QStringList>* FrdDataVIS::GetScalarInfo()
{
	if (frdSource_){ 
		return frdSource_->GetScalarInfo();
	}else{ 
		return 0;
	}
}

const vector<int>* FrdDataVIS::GetGridIds()
{
    if (frdSource_)  return frdSource_->GetGridIds();
    else return 0;
}

const QVector<QString>* FrdDataVIS::GetScalarVectorInfo()
{
    QVector<QString> *TMP = 0;
    if (frdSource_)  return frdSource_->GetScalarVectorInfo();
    else return TMP;
}

void FrdDataVIS::SetContourVisible(int gridId, const QString &scalar, bool visible, const QString &header)
{
    //scalar = Lnnn-xxx:xxx  eg. L101-STRESS:YY
    //header = Lnnn-DISP     eg. L101-DISP
    int exist = 1;
    if (headerContoursMap_.find(header) == headerContoursMap_.end())
    {
        exist = 0;
    }
    else
    {
        if (headerContoursMap_[header].find(gridId) == headerContoursMap_[header].end())
        {
            exist = -1;
        }
    }
    if (exist != 1)
    {
        vtkVISUnContour *contourObj = CreateContourObject(gridId, scalar, header);
        if (contourObj == 0){
            contourObj=CreateContourObject(gridId, scalar, "original");
            if (contourObj==0) return; 
        }
        if (exist == 0)
        {
            map<int, vtkVISUnContour*> obj;
            headerContoursMap_[header] = obj;
        }
        headerContoursMap_[header][gridId] = contourObj;
    }
    if (visible)
    {
        headerContoursMap_[header][gridId]->ShowOn();
        headerContoursMap_[header][gridId]->ShowOnScalarBar();
        //renderer_->ResetCamera();qy 2021-3-1
    }
    else
    {
        headerContoursMap_[header][gridId]->ShowOff();
        headerContoursMap_[header][gridId]->ShowOffScalarBar();
    }
}

vtkVISUnContour* FrdDataVIS::CreateContourObject(int gridId, const QString &scalar, const QString &header)
{
    if (!scalarResultLoaded_)  return 0;
    if (frdSource_->IsScalarNameValid(scalar) == false)  return 0;
    if (header == "original")
    {
        if (frdSource_->SetFrdDataSource(gridId) == false)  return 0;
    }
    else
    {
        if (frdSource_->SetFrdDataSource(gridId, header) == false)  return 0;
    }
    vtkVISUnContour *visObj = vtkVISUnContour::New();
    visObj->SetRenderTo(renderer_, renWin_);
    visObj->SetDataSource(frdSource_->GetSourceGrid());
    visObj->SetContourType(0);  //banded contour, can change level.
    visObj->CreateContourDisplay(scalar.toAscii().data());
    visObj->ClippingOffContourDisplay();
    visObj->CreateScalarBar();
    visObj->ModifyScalarBarTitle(scalar.toAscii().data());
    visObj->ModifyScalarBarFontColor(0, 0, 0);
    visObj->ShowOnScalarBar();
    return visObj;
}
//L101-DISP:D1
void FrdDataVIS::SetContourVariable(int gridId, const QString &scalar, const QString &header)
{
    if (headerContoursMap_.find(header) == headerContoursMap_.end())
    {
        return;
    }
	if (headerContoursMap_[header].find(gridId) == headerContoursMap_[header].end())
	{
		return;
	}
    if (frdSource_->IsScalarNameValid(scalar) == false)  return;
    headerContoursMap_[header][gridId]->ModifyContourDisplay(scalar.toAscii().data());
    headerContoursMap_[header][gridId]->ModifyScalarBarTitle(scalar.toAscii().data());
}

void FrdDataVIS::SetContourType(int gridId, const QString &scalar, int type, const QString &header)
{
    if (headerContoursMap_.find(header) == headerContoursMap_.end())
    {
        return;
    }
    if (headerContoursMap_[header].find(gridId) == headerContoursMap_[header].end())
    {
        return;
    }
    if (frdSource_->IsScalarValuesDiff(scalar))
    {
        headerContoursMap_[header][gridId]->SetContourType(type);
        headerContoursMap_[header][gridId]->ModifyContourDisplayType();
    }
}

void FrdDataVIS::SetContourLevel(int gridId, int level, const QString &header)
{
    if (headerContoursMap_.find(header) == headerContoursMap_.end())
    {
        return;
    }
    if (headerContoursMap_[header].find(gridId) == headerContoursMap_[header].end())
    {
        return;
    }
    headerContoursMap_[header][gridId]->ModifyContourLevel(level);
}

void FrdDataVIS::SetContourOpacity(int gridId, double opacity, const QString &header)
{
    if (headerContoursMap_.find(header) == headerContoursMap_.end())
    {
        return;
    }
    if (headerContoursMap_[header].find(gridId) == headerContoursMap_[header].end())
    {
        return;
    }
    headerContoursMap_[header][gridId]->SetOpacity(opacity);
}

void FrdDataVIS::SetContourRange(int gridId, double minV, double maxV, const QString &header)
{
    if (headerContoursMap_.find(header) == headerContoursMap_.end())
    {
        return;
    }
    if (headerContoursMap_[header].find(gridId) == headerContoursMap_[header].end())
    {
        return;
    }
    if (minV<maxV)
    {
        headerContoursMap_[header][gridId]->ModifyDisplayScalarRange(minV, maxV);
    }
    else
    {
        headerContoursMap_[header][gridId]->ModifyDisplayScalarRange(maxV, minV);
    }
}

void FrdDataVIS::SetContourScalarbarVisible(int gridId, bool visible, const QString &header)
{
    if (headerContoursMap_.find(header) == headerContoursMap_.end())
    {
        return;
    }
    if (headerContoursMap_[header].find(gridId) == headerContoursMap_[header].end())
    {
        return;
    }
    if (visible)
    {
        headerContoursMap_[header][gridId]->ShowOnScalarBar();
    }
    else
    {
        headerContoursMap_[header][gridId]->ShowOffScalarBar();
    }
}

void FrdDataVIS::SetDisplacementVisible(int gridId, const QString &header, bool visible)
{
    int exist = 1;
    if (headerDisplacementsMap_.find(header) == headerDisplacementsMap_.end())
    {
        exist = 0;
    }
    else
    {
        if (headerDisplacementsMap_[header].find(gridId) == headerDisplacementsMap_[header].end())
        {
            exist = -1;
        }
    }
    if (exist != 1)
    {
        if (!scalarResultLoaded_)  return;
        if (exist == 0)
        {
            map<int, vtkVISUnShadeMesh*> obj;
            headerDisplacementsMap_[header] = obj;
        }
        if (frdSource_->SetFrdDataSource(gridId, header) == false)  return;
        vtkVISUnShadeMesh *visObj = vtkVISUnShadeMesh::New();
        visObj->SetRenderTo(renderer_, renWin_);
        visObj->SetDataSource(frdSource_->GetSourceGrid());
        visObj->CreateShadeMeshDisplay(0);
        headerDisplacementsMap_[header][gridId] = visObj;
    }
    if (visible)
    {
        headerDisplacementsMap_[header][gridId]->SetColor(m_gridShowColor.r, m_gridShowColor.g, m_gridShowColor.b);
        headerDisplacementsMap_[header][gridId]->ShowOn();
        //renderer_->ResetCamera();qy 2021-3-1
    }
    else  headerDisplacementsMap_[header][gridId]->ShowOff();
}
void FrdDataVIS::SetDisplacementColor(VTKColorS Color)
{
    m_gridShowColor = Color;
}

void FrdDataVIS::SetShadeOpacity(double factor, const QString &header)
{
    if (header == "original")
    {
        for (map<int, vtkVISUnShadeMesh*>::iterator it = shadeMap_.begin(); it != shadeMap_.end(); ++it)
        {
            it->second->SetOpacity(factor);
        }
    }
    else
    {
        if (headerDisplacementsMap_.find(header) != headerDisplacementsMap_.end())
        {
            for (map<int, vtkVISUnShadeMesh*>::iterator it = headerDisplacementsMap_[header].begin(); it != headerDisplacementsMap_[header].end(); ++it)
            {
                it->second->SetOpacity(factor);
            }
        }
    }
}
void FrdDataVIS::createActor()
{
	if (frdSource_ == 0)  return;
    vtkVISUnstructuredGridSource *vbj=frdSource_->GetSourceGrid();
    //--
    m_geoFilter=vtkGeometryFilter::New();
    m_geoFilter->SetInput(vbj->unstruGrid);
    m_geoFilter->ReleaseDataFlagOn();
    vtkFeatureEdges *edges =vtkFeatureEdges::New();
    edges->SetInput(m_geoFilter->GetOutput());
    edges->ReleaseDataFlagOn();
    edges->BoundaryEdgesOn();
    edges->ColoringOff();
    vtkPolyDataMapper *mapper=vtkPolyDataMapper::New();
    mapper->SetInputConnection(edges->GetOutputPort());
    mapper->ScalarVisibilityOff();

    vtkActor *actor = vtkActor::New();
    actor->SetMapper(mapper);
    (actor->GetProperty())->SetColor(1.0, 0.0, 0.0);
    (actor->GetProperty())->SetLineWidth(10);
    actor->VisibilityOn();

    OutLineS tmpOBJ;
    tmpOBJ.SourceData=vbj;
    tmpOBJ.vActor=actor;
    tmpOBJ.edges=edges;
    m_OutLineS.insert(m_OutLineS.size(), tmpOBJ);
    renderer_->AddActor(actor);  
}
void FrdDataVIS::ActorPick(bool flag)
{
    pointPickCallback_=0;
    pointPickCallback_ = vtkCallbackCommand::New();
    pointPickCallback_->SetCallback(Callback_PickPoint);
    pointPickCallback_->SetClientData(this);
    vtkRenderWindowInteractor *ir = renWin_->GetInteractor();
    ir->AddObserver(vtkCommand::LeftButtonPressEvent, pointPickCallback_);
    if(!flag ){
        ir->RemoveObservers(vtkCommand::LeftButtonPressEvent, pointPickCallback_);
    }else{
        ir->AddObserver(vtkCommand::LeftButtonPressEvent, pointPickCallback_);
    }
}

#include <vtkPropPicker.h>
void FrdDataVIS::Callback_PickPoint(vtkObject *caller, unsigned long, void *clientdata, void*)
{
    vtkRenderWindowInteractor *ir = vtkRenderWindowInteractor::SafeDownCast(caller);
    vtkRenderWindow *renWin = ir->GetRenderWindow();
    FrdDataVIS *w = static_cast<FrdDataVIS*>(clientdata);
    //if (ir == 0 || w == 0)  return;
    int winx, winy;
    ir->GetEventPosition(winx, winy);
    vtkPointPicker *picker = vtkPointPicker::New();
    //picker->SetTolerance(0.01);
    vtkRenderer *renderer = renWin->GetRenderers()->GetFirstRenderer();
    picker->Pick(winx, winy, 0, renderer);
    int pointId=0; //= picker->GetPointId();

    double xyz[3];
    picker->GetPickPosition(xyz);
    if (pointId<0){
        
        return;
    }
    //QString str=picker->GetMapper()->GetClassName();
    w->UpdataSelectActor(picker->GetActor());
    //cout << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << endl;
    picker->GetActor();
}
void FrdDataVIS::UpdataSelectActor(vtkActor *actor)
{
    QMapIterator<int,OutLineS> it(m_OutLineS);

    while (it.hasNext()){
        it.next();
        if(m_OutLineS[it.key()].vActor==actor)
        {
           //vtkPolyData *data1 = m_geoFilter->GetOutput();
           //int num1=data1->GetNumberOfPoints();
           //vtkPointData *PointD1=data1->GetPointData();
          

           vtkPolyData *data2 =m_OutLineS[it.key()].edges->GetOutput();
           int num2=data2->GetNumberOfPoints();
           //vtkPointData *PointD2=data2->GetPointData();
           m_ListData.clear();
           for (int i=0;i<num2;i++){
               double p[3];
               data2->GetPoint(i,p);
               QString str=QString("%1,%2,%3").arg(p[0]).arg(p[1]).arg(p[2]);
               m_ListData.append(str);
           }    
        }
    }
}
//--actor show or hide
void FrdDataVIS::AcotrVisibility(bool f)
{
    createActor();
    QMapIterator<int,OutLineS> it(m_OutLineS);
    if (f){
        while (it.hasNext()){
            it.next();
            m_OutLineS[it.key()].vActor->VisibilityOn();  
        }
    }else{
        while (it.hasNext()){
            it.next();
             m_OutLineS[it.key()].vActor->VisibilityOff();   
        }
    }
    Update();
}
//属性线宽设置
void FrdDataVIS::SetWidth(int LineW)
{
    if (LineW <=0 )LineW = 1.0;
    if(LineW >20)LineW = 20;
    
    for (map<int, vtkVISUnShadeMesh*>::iterator it = meshMap_.begin(); it != meshMap_.end(); ++it)
    {
        it->second->SetMeshLineWidth(LineW);
    }
    for (map<int, vtkVISUnShadeMesh*>::iterator it = shadeMap_.begin(); it != shadeMap_.end(); ++it)
    {
        it->second->SetMeshLineWidth(LineW);
    }
    for (map<int, vtkVISUnShadeMesh*>::iterator it = allEdgeMeshMap_.begin(); it != allEdgeMeshMap_.end(); ++it)
    {
        it->second->SetMeshLineWidth(LineW);
    }
    for (map<int, vtkVISUnEdge*>::iterator it = outlineMap_.begin(); it != outlineMap_.end(); ++it)
    {
        it->second->ChangeLineWidth(LineW);
    }
}


//get point variable's timestep-value//position-value map for xyplot
map<double, double> FrdDataVIS::GetPointScalar_TimeValueMap(int pointId, const QString &scalarName)
{
    if (frdSource_)  return frdSource_->GetPointScalar_TimeValueMap(pointId, scalarName);
    else return map<double, double>();
}

TextStepIncTimeS FrdDataVIS::GetText(const QString &scalar, const QString &header)
{
   TextStepIncTimeS ttt;
    if (frdSource_)  return frdSource_->GetStepTimeIncMap(1, scalar);
    else return ttt;//map<double, double>();
    //headerContourMap_[header]->ModifyContourDisplay(scalar.toAscii().data());
    //headerContourMap_[header]->ModifyScalarBarTitle(scalar.toAscii().data());
}
bool FrdDataVIS::SetHisPointVtkShow(int PointId,double x, double y, double z)
{   
    //m_tmpActor=mesh_->GetActor();
    //m_tmpActor->GetProperty()->SetRepresentationToPoints();

    PointVisibility(true);
    vtkSphereSource* sphSource=vtkSphereSource::New(); 
     sphSource->SetCenter(x,y,z);
     sphSource->SetRadius(factor_);
     sphSource->Update();


    vtkVectorText *text=vtkVectorText::New();
    text->SetText(QString("%1").arg(PointId).toAscii().data());
   
    
    //--缩放
    vtkTransform *t=vtkTransform::New();
    t->Translate(x+factor_,y+factor_,z+factor_);
    t->Scale(factor_,factor_,factor_);
  
  
    vtkTransformFilter *tf=vtkTransformFilter::New();
    tf->SetTransform(t);
    tf->SetInput(text->GetOutput());
    tf->Update();

    vtkPolyDataMapper *pointMaper =vtkPolyDataMapper::New();
    pointMaper->SetInputConnection(sphSource->GetOutputPort());

    vtkActor *actor=vtkActor::New();
    actor->GetProperty()->SetColor(PointColor_.r,PointColor_.g,PointColor_.b);
    actor->SetMapper(pointMaper);
    //actor->SetScale(factor_);
    //actor->SetPosition(x,y,z);
    //actor->SetScale(1.0,1,1);
   
    vtkPolyDataMapper *textmaperL= vtkPolyDataMapper::New();
    textmaperL->ImmediateModeRenderingOn();
    textmaperL->SetInputConnection(tf->GetOutputPort());

    vtkActor *textActorL=vtkActor::New();
    textActorL->GetProperty()->SetColor(PointColor_.r,PointColor_.g,PointColor_.b);
    textActorL->SetMapper(textmaperL); 
    

    renderer_->AddActor(textActorL);
    renderer_->AddActor(actor);
    if (bPointLab_){
        textActorL->VisibilityOn();
    }else{
        textActorL->VisibilityOff();
    }

    HisPointS tmpDataP;
    tmpDataP.sphere=sphSource;
    tmpDataP.pointMapMaper=pointMaper;
    tmpDataP.MapActor=actor;
    tmpDataP.LabActor=textActorL;
    tmpDataP.t=t;
    tmpDataP.tf=tf;
    tmpDataP.text=text;
    tmpDataP.textMaper=textmaperL;
    tmpDataP.x=x;tmpDataP.y=y;tmpDataP.z=z;
    m_PointProp.insert(PointId,tmpDataP);

 
    actor->Delete();actor=NULL;
    textActorL->Delete();textActorL=NULL;
    sphSource->Delete();sphSource=NULL;
    text->Delete();text=NULL;
    t->Delete();t=NULL;
    tf->Delete();tf=NULL;

    return true;
   
  /*  static int ttt=1;
    vtkPoints *points = vtkPoints::New();
    vtkCellArray *verts= vtkCellArray::New();
    vtkAppendPolyData *appendPolyData = vtkAppendPolyData::New();
    
    points->InsertNextPoint(x,y,z);
    verts->InsertNextCell(1);
    verts->InsertCellPoint(1);

    vtkVectorText *text=vtkVectorText::New();
    text->SetText(QString("%1").arg(PointId).toAscii().data());
  
    //--缩放
    vtkTransform *t=vtkTransform::New();
    t->Translate(x,y,z);
    t->Scale(0.04,0.04,0.04);

    vtkTransformFilter *tf=vtkTransformFilter::New();
    tf->SetTransform(t);
    tf->SetInput(text->GetOutput());
    appendPolyData->AddInputConnection(tf->GetOutputPort());
    
    vtkPolyData *polyData=vtkPolyData::New();
    polyData->SetPoints(points);
    polyData->SetVerts(verts);

    vtkPolyDataMapper *mapper=vtkPolyDataMapper::New();
    mapper->SetInput(polyData);

    vtkActor *vertsActor=vtkActor::New();
    vertsActor->SetMapper(mapper);
    vertsActor->GetProperty()->SetPointSize(10);
    vertsActor->GetProperty()->SetColor(1,0,0);
    renderer_->AddActor(vertsActor);

    vtkPolyDataMapper *textMapper=vtkPolyDataMapper::New();
    textMapper->ImmediateModeRenderingOn();
    textMapper->SetInputConnection(appendPolyData->GetOutputPort());
  
    vtkActor *TextActor2=vtkActor::New();
    TextActor2->SetMapper(textMapper);
    TextActor2->GetProperty()->SetColor(1,0,0);
    renderer_->AddActor(TextActor2);
    */
}
bool FrdDataVIS::RemoveHisPointVtkShow(vtkActor *actor)
{
    QMapIterator<int,HisPointS> it(m_PointProp);
  
    while (it.hasNext()){
        it.next();
        if(m_PointProp[it.key()].MapActor==actor)
        {
          m_PointProp.remove(it.key());
        }
    }
    return true;
       
}
void FrdDataVIS::ClearHisPointVtkShow()
{
    PointVisibility(false);
    QMapIterator<int,HisPointS> it(m_PointProp);

    while (it.hasNext()){
        it.next();
        //if(m_PointProp[it.key()].MapActor==actor)
        //{
            m_PointProp.remove(it.key());
        //}
    }
}
void FrdDataVIS::PointVisibility(bool f)
{
    QMapIterator<int,HisPointS> it(m_PointProp);
    if (f){
       while (it.hasNext()){
           it.next();
           m_PointProp[it.key()].MapActor->VisibilityOn();
           if (bPointLab_){
                m_PointProp[it.key()].LabActor->VisibilityOn();
           }else{
                m_PointProp[it.key()].LabActor->VisibilityOff();
           }
       }
    }else{
        while (it.hasNext()){
            it.next();
            m_PointProp[it.key()].MapActor->VisibilityOff();
            m_PointProp[it.key()].LabActor->VisibilityOff();
        }
    }
}
void FrdDataVIS::SetColorSize(double factor,VTKColorS tmpClr)
{
    factor_=factor;
    PointColor_=tmpClr;
    QMapIterator<int,HisPointS> it(m_PointProp);

    while (it.hasNext()){
        it.next();
        m_PointProp[it.key()].sphere->SetRadius(factor);
        m_PointProp[it.key()].MapActor->GetProperty()->SetColor(tmpClr.r,tmpClr.g,tmpClr.b);
        //m_PointProp[it.key()].t->Translate(m_PointProp[it.key()].x+factor_,m_PointProp[it.key()].y+factor_,m_PointProp[it.key()].z+factor_);
        //m_PointProp[it.key()].t->Scale(factor,factor,factor);
        //m_PointProp[it.key()].tf->SetTransform(m_PointProp[it.key()].t);
        //m_PointProp[it.key()].tf->SetInput(m_PointProp[it.key()].text->GetOutput());
        //m_PointProp[it.key()].tf->Update();
        //m_PointProp[it.key()].textMaper->ImmediateModeRenderingOn();
        m_PointProp[it.key()].textMaper->SetInputConnection(m_PointProp[it.key()].tf->GetOutputPort());
        m_PointProp[it.key()].LabActor->GetProperty()->SetColor(tmpClr.r,tmpClr.g,tmpClr.b);
    } 
    Update();
}
//设置拾取点的lab visible
void FrdDataVIS::SetPickPointLabVisible(bool f)
{
    bPointLab_=f;
    QMapIterator<int,HisPointS> it(m_PointProp);
    if (f){
        while (it.hasNext()){
            it.next();
            m_PointProp[it.key()].LabActor->VisibilityOn();
        }
    }else{
        while (it.hasNext()){
            it.next();
            m_PointProp[it.key()].LabActor->VisibilityOff();
        }
    }
     Update();

}
//--获取id
int FrdDataVIS::GetPointId(QString sLabel, double x, double y, double z)
{
    int id=-1;
    if (frdSource_)  return frdSource_->GetPointID(sLabel,x,y,z);
    return id;
}

//cut api begin
void FrdDataVIS::CreateCutObjects(int gridId,int cutId, bool bInsideOut,QString &scalar, const QString &header)
{
    if (scalar.isEmpty()) return;
	if (!scalarResultLoaded_)  return;
	if (frdSource_->IsScalarNameValid(scalar) == false)  return;
	if (header == "original"){
		if (frdSource_->SetFrdDataSource(gridId) == false)  return;
	}else{
		if (frdSource_->SetFrdDataSource(gridId, header) == false)  return;
	}

	//const vector<int>* idS=GetGridIds();
	//vector<int>::const_iterator it1=idS->begin();//IdList
    if (header == "original")
    {
        VTKColorS color;
        color.r = color.g = color.b = 0.5;
		SetShadeVisible(gridId,true, color);
		  //set right frd data sources.(默认只有一块材料)
        ////SetShadeVisible(true, color);  //set right frd data sources.
    }else{
		SetDisplacementVisible(gridId,header, true);
    }
    SetShadeOpacity(0.5, header);
		
	//vtkVISUnSlice *obj2 = vtkVISUnSlice::New();
	QVTKUnClip *obj2=QVTKUnClip::New();//new QVTKUnClip();
	obj2->SetRenderTo(renderer_, renWin_);
	obj2->SetDataSource(frdSource_->GetSourceGrid());
	obj2->SetSliceSource_FD(scalar.toAscii().data(), 0,bInsideOut);
	obj2->CreateSliceWidget_FD();

	obj2->CreateUnSliceContourDisplay_FD();
	obj2->CreateUnSliceContourLinesDisplay_FD();

	//obj2->ShowOnUnSliceContourDisplay();
	obj2->ShowOffUnSliceContourLinesDisplay();
	obj2->ModifyUnSliceContourLevel(20);
	obj2->ClippingOffSliceContourDisplay();
    renderer_->ResetCamera();
    //cutMap_[cutId] = obj2;
	GridcutIdMap_[gridId][cutId]=obj2;
    cutZoneHeaderMap_[cutId] = header;
}

void FrdDataVIS::DeleteCut(int gridId,int cutId)
{
    if (cutZoneHeaderMap_.find(cutId) != cutZoneHeaderMap_.end())
    {
        cutZoneHeaderMap_.erase(cutId);
        GridcutIdMap_[gridId][cutId]->ShowOffSlicePlaneWidget();
        GridcutIdMap_[gridId][cutId]->Delete();
        GridcutIdMap_[gridId].erase(cutId);
    }
}

void FrdDataVIS::AddCutCallBack(int gridId,int cutId, vtkCallbackCommand *callback)
{
	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return ;
	}
    GridcutIdMap_[gridId][cutId]->GetSlicePlaneWidget()->AddObserver(vtkCommand::InteractionEvent, callback);
}

void FrdDataVIS::SetCutVTKWidgetVisible(int gridId,int cutId, bool visible)
{

	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return ;
	}

   
    if (visible)
    {
        GridcutIdMap_[gridId][cutId]->ShowOnSlicePlaneWidget();
    }
    else
    {
        GridcutIdMap_[gridId][cutId]->ShowOffSlicePlaneWidget();
    }
}

void FrdDataVIS::SetCutZoneVisible(int gridId,int cutId, bool visible)
{
    if (cutZoneHeaderMap_.find(cutId) == cutZoneHeaderMap_.end())
    {
        return;
    }
    QString header(cutZoneHeaderMap_[cutId]);

	//const vector<int>* idS=GetGridIds();
	//vector<int>::const_iterator it1=idS->begin();//IdList

	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return ;
	}
    if (header == "original")
    {
        VTKColorS color;
        color.r = color.g = color.b = 0.5;
		
		//for (it1;it1!=idS->end();++it1){ 
			//SetShadeVisible((*it1),true, color);
		//}
		SetShadeVisible(gridId,visible,color);
		//SetShadeVisible(1,visible, color); //set right frd data sources.
        ////SetShadeVisible(visible, color); //set right frd data sources.
    }
    else
    {
		//for (it1;it1!=idS->end();++it1){ 
		//	SetDisplacementVisible((*it1),header, true); //set right frd data sources.
		//}
		SetDisplacementVisible(gridId,header, visible); //set right frd data sources.
		//SetDisplacementVisible(1,header, visible); //set right frd data sources.
        ////SetDisplacementVisible(header, visible); //set right frd data sources.
    }
    if (visible)  SetShadeOpacity(0.5, header);
}
void FrdDataVIS::SetCutPlaneNormal(int gridId,int cutId, double x, double y, double z)
{
	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return ;
	}
    
    GridcutIdMap_[gridId][cutId]->SetSlicePlaneNormal(x, y, z);
}

void FrdDataVIS::SetCutPlanePosition(int gridId,int cutId, double x, double y, double z)
{
	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return ;
	}
    GridcutIdMap_[gridId][cutId]->SetSlicePlaneOrigin(x, y, z);
    GridcutIdMap_[gridId][cutId]->GetSlicePlaneWidget()->InvokeEvent(vtkCommand::InteractionEvent);  //for calling callback-method.  when x, y, z out of range, reset it by callback.
}

bool FrdDataVIS::GetCutPlanePosition(int gridId,int cutId, double *xyz)
{
	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return false;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return false;
	}
    GridcutIdMap_[gridId][cutId]->GetSlicePlaneWidget()->GetOrigin(xyz);
    return true;
}

void FrdDataVIS::SetCutContourState(int gridId,int cutId, bool visible, int type)
{

	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return;
	}
	
    if (visible)
    {
		//cutMap_[cutId]->ClippingOffSliceContourDisplay();
        if (type == 0)
        {
			
            GridcutIdMap_[gridId][cutId]->ShowOnUnSliceContourDisplay();
            GridcutIdMap_[gridId][cutId]->ShowOffUnSliceContourLinesDisplay();
        }
        else
        {
            GridcutIdMap_[gridId][cutId]->ShowOffUnSliceContourDisplay();
            GridcutIdMap_[gridId][cutId]->ShowOnUnSliceContourLinesDisplay();
        }
    }
    else
    {
        GridcutIdMap_[gridId][cutId]->ShowOffUnSliceContourDisplay();
        GridcutIdMap_[gridId][cutId]->ShowOffUnSliceContourLinesDisplay();
    }
}

void FrdDataVIS::SetCutContourLevel(int gridId,int cutId, int level)
{
	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return;
	}
	GridcutIdMap_[gridId][cutId]->ModifyUnSliceContourLevel(level);
}

void FrdDataVIS::SetCutContourVariable(int gridId, int cutId, const QString &scalar)
{

	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return;
	}
    GridcutIdMap_[gridId][cutId]->ModifySliceSourceScalar_FD(scalar.toAscii().data());
}

bool FrdDataVIS::GetCutZoneBounds(int gridId,int cutId, double *bounds)
{
	if (GridcutIdMap_.find(gridId) == GridcutIdMap_.end())
	{
		return false;
	}
	if (GridcutIdMap_[gridId].find(cutId) == GridcutIdMap_[gridId].end())
	{
		return false;
	}
    
    GridcutIdMap_[gridId][cutId]->GetSourceBounds_FD(bounds);
    return true;
}

//cut api end
