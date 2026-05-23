#include "InpDataVIS.h"
#include <vtkRenderer.h>
#include <vtkVISUnShadeMesh.h>
#include <vtkVISUnEdge.h>
#include <vtkVISUnContour.h>
InpDataVIS::InpDataVIS()
{
    renderer_ = 0;
    renWin_ = 0;
    InpSource_ = 0;
    mesh_ = 0;
    BCmesh_ =0;
	factor_=0.002;
	BCmeshMap_.clear();
    //scalarResultLoaded_ = false;
    //contour_ = 0;
}

InpDataVIS::~InpDataVIS()
{
    Clean();
}
void InpDataVIS::InitRenderer(vtkRenderer *renderer)
{
    renderer_ = renderer;
    renWin_ = renderer_->GetRenderWindow();
}

void InpDataVIS::Update()
{
    if (renWin_)  renWin_->Render();
}
void InpDataVIS::Clean()
{
    if (InpSource_)
    {
        delete InpSource_;
        InpSource_ = 0;
    }

    if (mesh_)
    {
        mesh_->DeleteObjects();
        mesh_->Delete();
        mesh_ = 0;
    }
   
   if (BCmesh_)
    {
        BCmesh_->DeleteObjects();
        BCmesh_->Delete();
        BCmesh_=0;
    }
	for (QMap<QString, vtkVISUnShadeMesh*>::iterator it = BCmeshMap_.begin(); it != BCmeshMap_.end(); ++it)
	{
		it.value()->DeleteObjects();
		it.value()->Delete();
	}
	BCmeshMap_.clear();
   /*scalarResultLoaded_ = false;
    if (contour_)
    {
        contour_->DeleteObjects();
        contour_->Delete();
        contour_ = 0;
    }
    for (map<QString, vtkVISUnShadeMesh*>::iterator it = headerDisplacementMap_.begin(); it != headerDisplacementMap_.end(); ++it)
    {
        it->second->DeleteObjects();
        it->second->Delete();
    }
    headerDisplacementMap_.clear();*/
}

bool InpDataVIS::LoadInpData(ReadInpResultS *Inp)
{
    Clean();
    InpSource_ = new InpDataSource;
    bool f = InpSource_->InitGridFromFrd(Inp);
    if (f){
         bool f2 = InpSource_->InitBCFromFrd(Inp);//点,单元，表面集合数据初始化
         bool f = InpSource_->InpRowDataToSurf(Inp);
    }else{
        delete InpSource_;
        InpSource_ = 0;
    }
    return f;
}
vtkRenderer* InpDataVIS::GetBindedRenderer()
{
    return renderer_;
}

void InpDataVIS::ShallowCopyFrdData(InpDataVIS *source)
{
    this->InpSource_ = source->InpSource_;
}
vtkVISUnShadeMesh* InpDataVIS::CreateShadeMeshObject(int type)
{
    if (InpSource_ == 0)  return 0;
    InpSource_->SetInpDataSource("");
    vtkVISUnShadeMesh *visObj = vtkVISUnShadeMesh::New();
    visObj->SetRenderTo(renderer_, renWin_);
    visObj->SetDataSource(InpSource_->GetSourceGrid());
    if (type == 0)  visObj->CreateShadeMeshDisplay(1);
    else if (type == 1)  visObj->CreateShadeMeshDisplay(0);
    else if (type == 2)  visObj->CreateShadeMeshSurfaceDisplay(0);
    return visObj;
}
void InpDataVIS::SetMeshVisible(bool visible,VTKColorS tmpClr)
{
    if (mesh_ == 0)
    {
        mesh_ = CreateShadeMeshObject(1);//0
        if (mesh_ == 0)  return;
    }
    if (visible)
    {
        //mesh_->SetColor(0,0,0);//(0, 1, 0); qy modify 2020-3-31
        mesh_->SetColor(tmpClr.r,tmpClr.g,tmpClr.b);
        mesh_->ShowOn();
        renderer_->ResetCamera();
    }
    else  mesh_->ShowOff();
}

//--BC高亮显示
void InpDataVIS::SetBCCheckVisible(bool visible, NElSurfChsS Settype)//QString str)
{
	if (InpSource_ == 0) return ;
	if (BCmeshMap_.find(Settype.strName)==BCmeshMap_.end())
	{
		vtkVISUnShadeMesh *tmpMesh=CreateBCShadeMeshObject(Settype.iType,Settype.strName);
		if (tmpMesh==0)return;
		BCmeshMap_[Settype.strName]=tmpMesh;
	}
	

	if (visible){
		if (Settype.iType == 0){//--设置点显示
			BCmeshMap_[Settype.strName]->ChangeRepresentation("Points");
			vtkActor* vtkActor_ = BCmeshMap_[Settype.strName]->GetActor();
			vtkActor_->GetProperty()->SetPointSize(5.0);
		}else{
			BCmeshMap_[Settype.strName]->SetMeshLineWidth(1.0);
		}
		BCmeshMap_[Settype.strName]->SetColor(0,1,0);
		BCmeshMap_[Settype.strName]->ShowOn();
		renderer_->ResetCamera();
	}
	else  BCmeshMap_[Settype.strName]->ShowOff();
}
//----BC高亮显示
void InpDataVIS::SetBCVisible(bool visible, NElSurfChsS Settype)//QString str)
{
    if (InpSource_ == 0) return ;
	 int type = Settype.iType;
    if (BCmesh_)
    {
        BCmesh_->DeleteObjects();
        BCmesh_->Delete();           
        BCmesh_ = 0;
    }
    BCmesh_ = CreateBCShadeMeshObject(type, Settype.strName);
    if (BCmesh_ == 0) return;

    if (visible){
        if (type == 0){//--设置点显示
            BCmesh_->ChangeRepresentation("Points");
            vtkActor* vtkActor_ = BCmesh_->GetActor();
            vtkActor_->GetProperty()->SetPointSize(5.0);
        }else{
            BCmesh_->SetMeshLineWidth(2.0);
        }
        BCmesh_->SetColor(1,0,0);
        BCmesh_->ShowOn();
        renderer_->ResetCamera();
    }
    else  BCmesh_->ShowOff();	
}

vtkVISUnShadeMesh* InpDataVIS::CreateBCShadeMeshObject(int type, QString str)
{
    if (InpSource_ == 0)  return 0;
     vtkVISUnShadeMesh *visObj = vtkVISUnShadeMesh::New();
    if (type ==0){//点显示
        InpSource_->SetInpNSetDataSource(str);
        visObj->SetRenderTo(renderer_, renWin_);
        visObj->SetDataSource(InpSource_->GetNSetSourceGrid());
        visObj->CreateShadeMeshSurfaceDisplay(1);
    }else if(type == 1){//单元显示
        InpSource_->SetInpElSetDataSource(str);
        visObj->SetRenderTo(renderer_, renWin_);
        visObj->SetDataSource(InpSource_->GetElSourceGrid());
        visObj->CreateShadeMeshSurfaceDisplay(1);
        //visObj->CreateShadeMeshDisplay(0);
    }else if (type==2){//表面显示//--线或者实体显示
        InpSource_->SetInpSurfDataSource(str);
        visObj->SetRenderTo(renderer_, renWin_);
        visObj->SetDataSource(InpSource_->GetSurfaceSourceGrid());
        visObj->CreateShadeMeshSurfaceDisplay(1);
    } 
    return visObj;
}

vtkVISUnstructuredGridSource* InpDataVIS::GetSourceData()
{
    //return InpSource_->GetMapPointSource();
    return InpSource_->GetSourceGrid();
}

vtkUnstructuredGrid* InpDataVIS::GetWholeData(QString str)
{
    //return InpSource_->GetMapPointSource();
    return InpSource_->GetWholeData(str);
}
vtkUnstructuredGrid* InpDataVIS::GetSurfGridData()
{
    return InpSource_->GetSurfGridData();
}
void InpDataVIS::SetNset(QString str,DecodeGenerateS Datas)
{
    this->InpSource_->disBCSourceMap_.remove(str);
    this->InpSource_->varNSetMap_.insert(str,Datas);
}
void InpDataVIS::SetElset(QString str,DecodeGenerateS Datas)
{
    this->InpSource_->disELSourceMap_.remove(str);
    this->InpSource_->varElSetMap_.insert(str,Datas);
}
void InpDataVIS::SetSurfset(QString str,QStringList Datas)
{
    this->InpSource_->disSurfSourceMap_.remove(str);
    this->InpSource_->varSurfaceSetMap_.insert(str,Datas);
}
//获取经过滤波后表面集合的数据
const QMap<QString, QString>* InpDataVIS::GetElementSurfFilterMap()
{
    if (InpSource_)return InpSource_->GetElementSurfFilterData();
    else return 0;
}
void InpDataVIS::SetMeshActorShow(bool bShow)
{
    if (mesh_ == 0)
    {
        mesh_ = CreateShadeMeshObject(1);//0
        if (mesh_ == 0)  return;
    }
    if (bShow){
        mesh_->GetActor()->VisibilityOn();
    }else{
        mesh_->GetActor()->VisibilityOff();
    }

}
//element not start Id=1;
int InpDataVIS::GetCellBaseId()
{
	return this->InpSource_->CellBaseId_;
}

bool InpDataVIS::SetHisPointVtkShow(int PointId,double x, double y, double z)
{  
	 
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
    actor->GetProperty()->SetColor(1.0,0.,0.);
    actor->SetMapper(pointMaper);

    vtkPolyDataMapper *textmaperL= vtkPolyDataMapper::New();
    textmaperL->ImmediateModeRenderingOn();
    textmaperL->SetInputConnection(tf->GetOutputPort());

    vtkActor *textActorL=vtkActor::New();
    textActorL->GetProperty()->SetColor(1.0,0.,0.);
    textActorL->SetMapper(textmaperL); 
    

    //renderer_->AddActor(textActorL);
    renderer_->AddActor(actor);
    if (bPointLab_){
        textActorL->VisibilityOn();
    }else{
        textActorL->VisibilityOff();
    }

    HisPointS1 tmpDataP;
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
}

void InpDataVIS::PointVisibility(bool f)
{
    QMapIterator<int,HisPointS1> it(m_PointProp);
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
            m_PointProp[it.key()].MapActor->VisibilityOff();//取消节点高亮显示
           // m_PointProp[it.key()].LabActor->VisibilityOff();//节点编号不显示
        }
    }
}

void InpDataVIS::ClearHisPointVtkShow()
{
    PointVisibility(false);
    QMapIterator<int,HisPointS1> it(m_PointProp);

    while (it.hasNext()){
        it.next();
        //if(m_PointProp[it.key()].MapActor==actor)
        //{
            m_PointProp.remove(it.key());
        //}
    }
}