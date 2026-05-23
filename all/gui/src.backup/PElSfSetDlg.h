#ifndef _PELSFSET_H
#define _PELSFSET_H

class vtkObject;
class vtkCallbackCommand;

#include <QObject>
#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include "QMyCombox.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCallbackCommand.h"
#include "InpDataVIS.h"


#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkInteractorStyleRubberBandPick.h"
#include "vtkExtractPolyDataGeometry.h"
#include "vtkPlane.h"
#include "vtkPlanes.h"
#include "vtkAreaPicker.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkIdFilter.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkProperty.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkVersion.h"
#include "vtkVertexGlyphFilter.h"
#include "vtkActor.h"
#include "vtkRendererCollection.h"
#include "vtkObjectFactory.h"
#include "vtkExtractGeometry.h"
#include "vtkIdTypeArray.h"
#include "vtkVISUnstructuredGridSource.h"
#include "vtkSelectionNode.h"
#include "vtkSelection.h"
#include "vtkExtractSelection.h"
#include "vtkPointPicker.h"
#include "vtkCellPicker.h"
#include "Information_Widget.h"
#include "vtkCell.h"
#include "vtkExtractSelectedFrustum.h"
#include "vtkCellCenters.h"

//r->selected, p->clear
//单元集合
class ElInteractorStyle :public vtkInteractorStyleRubberBandPick
{
public: 
    static ElInteractorStyle* New();

    vtkTypeMacro(ElInteractorStyle,vtkInteractorStyleRubberBandPick)
    ElInteractorStyle()
    {
        this->SelectedMapper = vtkDataSetMapper::New();
        this->OrigActor = vtkActor::New();
        this->OrigActor->SetMapper(SelectedMapper);
        m_EditActor=0;
        m_ElPointSelectMap.clear();
    }
    ~ElInteractorStyle()
    {
        ClearCurrentSelectedMap();
    }
   
    virtual void OnLeftButtonUp()
    {
        vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
        vtkRenderWindowInteractor*rwi = this->GetInteractor();
        if (!this->m_LeftButtonUp)return;
        //if (!this->CurrentMode) return;
        
        //if (this->CurrentMode == 1 && rwi->GetControlKey()){//VTKISRBP_SELECT
        if (rwi->GetControlKey()){//VTKISRBP_SELECT
            vtkPlanes *frustum =static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();
            vtkExtractPolyDataGeometry *extractPolyDataGeometry=vtkExtractPolyDataGeometry::New();
            extractPolyDataGeometry->SetImplicitFunction(frustum);
            extractPolyDataGeometry->SetInput(this->PolyDataS);
            extractPolyDataGeometry->Update();

            vtkPolyData *selected =extractPolyDataGeometry->GetOutput();

            this->SelectedMapper->SetInput(selected);
            this->SelectedMapper->ScalarVisibilityOff();

            vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetCellData()->GetArray("WelCME"));
            vtkIdType TotalNum=ids->GetNumberOfTuples();
            if(TotalNum==0){//点选
                int pos[3]; 
                rwi->GetEventPosition(pos);
                vtkCellPicker *picker=vtkCellPicker::New();
                picker->SetTolerance(0.005);

                picker->Pick(pos[0],pos[1],0,this->GetDefaultRenderer());
                int CellId=picker->GetCellId();        

                //vtkActor* pA=(vtkActor*)picker->GetActor();
                //vtkCell* pCell = pA->GetMapper()->GetInput()->GetCell(CellId);
                //vtkPoints *mypoints=pCell->GetPoints();
                //int ind=mypoints->GetNumberOfPoints();//h获取单元拥有的点数
                //for (int i=0;i<ind;i++)
                //{
                //    double m[4];m[3]=1.0;
                //    mypoints->GetPoint(i,m);//获得Cell指定索引点坐标
                //    this->CurrentRenderer->SetWorldPoint(m[0],m[1],m[2],m[3]);
                //    this->CurrentRenderer->WorldToDisplay();
                //    double ss[3];
                //    this->CurrentRenderer->GetDisplayPoint(ss);
                //}         
                QString info = "-----------Cell Selected--------";  
                int showId=0;
                if (CellId ==-1)return; 
                if (CellId>this->PolyDataS->GetNumberOfPoints())return;
                Information_Widget::GetInstance()->ShowInformation(QString(info));
                info ="Picked Cellorg Id:"+QString("%1").arg(CellId+1);
                //info ="Picked Id:"+QString("%1").arg(showId+1);
                Information_Widget::GetInstance()->ShowInformation(QString(info));

                QString str0;
                str0=QString("%1").arg(CellId+1);

                if (m_ElPointSelectMap.find(str0)!=m_ElPointSelectMap.end()){//存在
                    ;
                }else{
                    vtkIdTypeArray *ids=vtkIdTypeArray::New();
                    ids->SetNumberOfComponents(1);
                    ids->InsertNextValue(CellId);//InsertNextce(CellId);

                    vtkSelectionNode *selectionNode=vtkSelectionNode::New();
                    selectionNode->SetFieldType(vtkSelectionNode::CELL);
                    selectionNode->SetContentType(vtkSelectionNode::INDICES);
                    selectionNode->SetSelectionList(ids);

                    vtkSelection *selection= vtkSelection::New();
                    selection->AddNode(selectionNode);

                    vtkExtractSelection *extractSelection= vtkExtractSelection::New();
                    extractSelection->SetInput(0,this->PickUnPolyData);//this->PolyData);PickUnPolyData
                    extractSelection->SetInput(1,selection);
                    extractSelection->Update();

                    vtkUnstructuredGrid *selected=vtkUnstructuredGrid::New();
                    selected->ShallowCopy(extractSelection->GetOutput());

                    vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
                    TmpSelectdMapper->SetInputConnection(selected->GetProducerPort());
                    TmpSelectdMapper->ScalarVisibilityOff();
                    vtkActor *TmpActor=vtkActor::New();
                    TmpActor->SetMapper(TmpSelectdMapper);

                    TmpActor->VisibilityOn();
                    TmpActor->GetProperty()->SetColor(1,0,0);
                    TmpActor->GetProperty()->SetLineWidth(3);
                    TmpActor->GetProperty()->SetRepresentationToWireframe();
                    render_->AddActor(TmpActor);

                    m_ElPointSelectMap.insert(str0, TmpActor);

                    TmpSelectdMapper->Delete();TmpSelectdMapper=NULL; 
                    selected->Delete();selected=NULL;
                    extractSelection->Delete();extractSelection=NULL;
                    selectionNode->Delete();selectionNode=NULL;
                    ids->Delete();ids=NULL;
                }
                //this->GetDefaultRenderer()->AddActor(TmpActor);
                render_->Render();
                this->HighlightProp(NULL);  
            }else{//-1
                vtkSelectionNode *selectionNode=vtkSelectionNode::New();
                selectionNode->SetFieldType(vtkSelectionNode::CELL);
                selectionNode->SetContentType(vtkSelectionNode::INDICES);
                selectionNode->SetSelectionList(ids);

                vtkSelection *selection= vtkSelection::New();
                selection->AddNode(selectionNode);

                vtkExtractSelection *extractSelection= vtkExtractSelection::New();
                extractSelection->SetInput(0,this->PickUnPolyData);
                extractSelection->SetInput(1,selection);
                extractSelection->Update();

                vtkUnstructuredGrid *selectedd=vtkUnstructuredGrid::New();
                selectedd->ShallowCopy(extractSelection->GetOutput());

                vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
                TmpSelectdMapper->SetInputConnection(selectedd->GetProducerPort());
                TmpSelectdMapper->ScalarVisibilityOff();

                //don't delete;
                QString info = "-----------Cell Selected--------";
                Information_Widget::GetInstance()->ShowInformation(QString(info));
                info="Cell Selected Numbers:"+QString("%1").arg(TotalNum);
                Information_Widget::GetInstance()->ShowInformation(QString(info));
                int td=this->OrigActor->GetMapper()->GetInput()->GetNumberOfCells();
                QString str0;

   
                for (int nn=0;nn<td;nn++)
                {
                    if (nn==0){ 
                        str0=QString("%1").arg(ids->GetValue(nn)+1);
                    }else{
                        str0+=","+QString("%1").arg(ids->GetValue(nn)+1);
                    }
                    double m[3];
                    vtkCell *c=this->OrigActor->GetMapper()->GetInput()->GetCell(nn);
                    vtkPoints *p=c->GetPoints();
                    info ="Cell Id :"+QString("%1").arg(ids->GetValue(nn)+1);
                    Information_Widget::GetInstance()->ShowInformation(QString(info)); 
                    
                //   /* for (int kk=0;kk<c->GetNumberOfPoints();kk++)
                //    {
                //        double m[3];
                //        p->GetPoint(kk,m);
                //        info ="m:"+QString("%1,%2,%3").arg(m[0]).arg(m[1]).arg(m[2]);
                //        Information_Widget::GetInstance()->ShowInformation(QString(info));
                //    }*/
                }
                vtkActor *TmpActor=vtkActor::New();
                TmpActor->SetMapper(TmpSelectdMapper);
                if (m_ElPointSelectMap.find(str0)!=m_ElPointSelectMap.end()){//存在
                    ;
                }else{
                    TmpActor->VisibilityOn();
                    TmpActor->GetProperty()->SetColor(1.0,0.0,0.0);
                    TmpActor->GetProperty()->SetLineWidth(3);
                    TmpActor->GetProperty()->SetRepresentationToWireframe();
                    this->GetDefaultRenderer()->AddActor(TmpActor);
                    m_ElPointSelectMap.insert(str0, TmpActor);
                }

                TmpSelectdMapper->Delete();TmpSelectdMapper=NULL;  
                selectedd->Delete();selectedd=NULL;
                extractSelection->Delete();extractSelection=NULL;
                selectionNode->Delete();selectionNode=NULL;
                ids->Delete();ids=NULL;

                this->GetInteractor()->GetRenderWindow()->Render(); 
                this->HighlightProp(NULL); 
            }
        }else if (rwi->GetShiftKey()){//
            vtkPlanes *frustum =static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();
            vtkExtractPolyDataGeometry *extractPolyDataGeometry=vtkExtractPolyDataGeometry::New();
            extractPolyDataGeometry->SetImplicitFunction(frustum);
            extractPolyDataGeometry->SetInput(this->PolyDataS);
            extractPolyDataGeometry->Update();


            vtkPolyData *selected =extractPolyDataGeometry->GetOutput();
            this->SelectedMapper->SetInput(selected);
            this->SelectedMapper->ScalarVisibilityOff();

            vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetCellData()->GetArray("WelCME"));
            vtkIdType TotalNum=ids->GetNumberOfTuples();
            if (TotalNum==0)
            {
                int pos[3]; 
                rwi->GetEventPosition(pos);
                vtkCellPicker *picker=vtkCellPicker::New();
                picker->SetTolerance(0.005);

                picker->Pick(pos[0],pos[1],0,this->GetDefaultRenderer());
                int CellId=picker->GetCellId();  
             
                QString info = "-----------Cell Selected--------";  
                int showId=0;
                if (CellId ==-1)return; 
                if (CellId>this->PolyDataS->GetNumberOfPoints())return;
              
                //if(m_ElPointMap.find(CellId+1)!=m_ElPointMap.end())//have
                //{
                //    m_ElPointMap[CellId+1]->GetProperty()->EdgeVisibilityOff();
                //    m_ElPointMap[CellId+1]->VisibilityOff();
                //    this->DefaultRenderer->RemoveActor(m_ElPointMap[CellId+1]);
                //    m_ElPointMap[CellId+1]->Delete();m_ElPointMap[CellId+1]=NULL;
                //    m_ElPointMap.remove(CellId+1);
                //    this->DefaultRenderer->Render();
                //    this->HighlightProp(NULL);
                //}
                Information_Widget::GetInstance()->ShowInformation(QString(info));
                info ="Picked Cellorg Id:"+QString("%1").arg(CellId+1);
                //info ="Picked Id:"+QString("%1").arg(showId+1);
                Information_Widget::GetInstance()->ShowInformation(QString(info));

                QString str0=QString("%1").arg(CellId+1);
                if (m_ElPointDelMap.find(str0)!=m_ElPointDelMap.end()){//存在
                    ;
                }else{

                    vtkIdTypeArray *ids=vtkIdTypeArray::New();
                    ids->SetNumberOfComponents(1);
                    ids->InsertNextValue(CellId);//InsertNextce(CellId);

                    vtkSelectionNode *selectionNode=vtkSelectionNode::New();
                    selectionNode->SetFieldType(vtkSelectionNode::CELL);
                    selectionNode->SetContentType(vtkSelectionNode::INDICES);
                    selectionNode->SetSelectionList(ids);

                    vtkSelection *selection= vtkSelection::New();
                    selection->AddNode(selectionNode);

                    vtkExtractSelection *extractSelection= vtkExtractSelection::New();
                    extractSelection->SetInput(0,this->PickUnPolyData);
                    extractSelection->SetInput(1,selection);
                    extractSelection->Update();

                    vtkUnstructuredGrid *selected=vtkUnstructuredGrid::New();
                    selected->ShallowCopy(extractSelection->GetOutput());

                    vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
                    TmpSelectdMapper->SetInputConnection(selected->GetProducerPort());
                    TmpSelectdMapper->ScalarVisibilityOff();

                    vtkActor *TmpActor=vtkActor::New();
                    TmpActor->SetMapper(TmpSelectdMapper);
                    TmpActor->VisibilityOn();
                    TmpActor->SetProperty(this->OrigActor->GetProperty());
                  /*  TmpActor->GetProperty()->SetColor(1.0,1.0,1.0);
                    TmpActor->GetProperty()->SetEdgeColor(1,1,1);*/
                    TmpActor->GetProperty()->SetLineWidth(3);

                    TmpActor->GetProperty()->SetRepresentationToWireframe();

                    render_->AddActor(TmpActor);
                    m_ElPointDelMap.insert(str0, TmpActor);

                    TmpSelectdMapper->Delete();TmpSelectdMapper=NULL; 
                    selected->Delete();selected=NULL;
                    extractSelection->Delete();extractSelection=NULL;
                    selectionNode->Delete();selectionNode=NULL;
                    ids->Delete();ids=NULL;
                }
                render_->Render();
                this->HighlightProp(NULL);  
            }else{
                vtkSelectionNode *selectionNode=vtkSelectionNode::New();
                selectionNode->SetFieldType(vtkSelectionNode::CELL);
                selectionNode->SetContentType(vtkSelectionNode::INDICES);
                selectionNode->SetSelectionList(ids);

                vtkSelection *selection= vtkSelection::New();
                selection->AddNode(selectionNode);

                vtkExtractSelection *extractSelection= vtkExtractSelection::New();
                extractSelection->SetInput(0,this->PickUnPolyData);
                extractSelection->SetInput(1,selection);
                extractSelection->Update();

                vtkUnstructuredGrid *selectedd=vtkUnstructuredGrid::New();
                selectedd->ShallowCopy(extractSelection->GetOutput());
     
                vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
                TmpSelectdMapper->SetInputConnection(selectedd->GetProducerPort());
                TmpSelectdMapper->ScalarVisibilityOff();
            //don't delete;
                QString info = "-----------Cell Selected--------";
                Information_Widget::GetInstance()->ShowInformation(QString(info));
                info="Cell Selected Numbers:"+QString("%1").arg(TotalNum);
                Information_Widget::GetInstance()->ShowInformation(QString(info));
                int td=this->OrigActor->GetMapper()->GetInput()->GetNumberOfCells();
                QString str0;
                for (int nn=0;nn<td;nn++)
                {
                    double m[3];
                    vtkCell *c=this->OrigActor->GetMapper()->GetInput()->GetCell(nn);
                    vtkPoints *p=c->GetPoints();
                    info ="Cell Id :"+QString("%1").arg(ids->GetValue(nn)+1);
                    if (str0=="")str0=QString("%1").arg(ids->GetValue(nn)+1);
                    else str0+=","+QString("%1").arg(ids->GetValue(nn)+1);
                    Information_Widget::GetInstance()->ShowInformation(QString(info));  
                //   /* for (int kk=0;kk<c->GetNumberOfPoints();kk++)
                //    {
                //        double m[3];
                //        p->GetPoint(kk,m);
                //        info ="m:"+QString("%1,%2,%3").arg(m[0]).arg(m[1]).arg(m[2]);
                //        Information_Widget::GetInstance()->ShowInformation(QString(info));
                //    }*/
                }
            
                vtkActor *TmpActor=vtkActor::New();
                TmpActor->SetMapper(TmpSelectdMapper);
                if (m_ElPointDelMap.find(str0)!=m_ElPointDelMap.end()){//存在
                       ;
                }else{
                    TmpActor->VisibilityOn();
                    TmpActor->SetProperty(this->OrigActor->GetProperty());
                    TmpActor->GetProperty()->SetLineWidth(3);
                    TmpActor->GetProperty()->SetRepresentationToWireframe();
                    this->GetDefaultRenderer()->AddActor(TmpActor);
                    m_ElPointDelMap.insert(str0,TmpActor);

                }   
                TmpSelectdMapper->Delete();TmpSelectdMapper=NULL;  
                selectedd->Delete();selectedd=NULL;
                extractSelection->Delete();extractSelection=NULL;        
                selectionNode->Delete();selectionNode=NULL;
                ids->Delete();ids=NULL;
                this->GetInteractor()->GetRenderWindow()->Render(); 
                this->HighlightProp(NULL); 
            }
         }
    }

    void ClearCurrentSelectedMap()
    {

       QMapIterator<QString,vtkActor*>itSelect=(this->m_ElPointSelectMap);

       while(itSelect.hasNext()) 
       {
           itSelect.next();
           itSelect.value()->GetProperty()->EdgeVisibilityOff();
           itSelect.value()->VisibilityOff();
           m_ElPointSelectMap.remove(itSelect.key());
           render_->RemoveActor(itSelect.value());
       }

       QMapIterator<QString,vtkActor*>itDel=(this->m_ElPointDelMap);
       while(itDel.hasNext()) 
       {
           itDel.next();
           itDel.value()->GetProperty()->EdgeVisibilityOff();
           itDel.value()->VisibilityOff();
           m_ElPointDelMap.remove(itDel.key());
           render_->RemoveActor(itDel.value());
       }


        if (m_EditActor!=0)
        {
            m_EditActor->GetProperty()->EdgeVisibilityOff();
            m_EditActor->VisibilityOff(); 
			m_EditActor=0;
            render_->RemoveActor(m_EditActor); 
        }

        renWin_->Render(); 
        this->HighlightProp(NULL); 
    }


    void SetPolyData(vtkPolyData *PolyData)
    {
        this->PolyDataS = PolyData;       
    }
    void SetPickType(int type)//wire->surf
    {
        this->m_pickType=type;
    }
    void SetPolyDataOld(vtkUnstructuredGrid *unpoints)
    {
        this->PickUnPolyData = unpoints;
    } 
    void SetLeftButtonUp(bool f)
    {
        this->m_LeftButtonUp=f;
    }

    void setMapData(QStringList DataList)
    {
        SetFilterActor(DataList);
    }
    void InitSetRender(vtkRenderer *R)
    {
        render_=R;
        renWin_=render_->GetRenderWindow();
    } 
    //-----------
    QStringList GetFilterId()
    {
        QMapIterator<QString,vtkActor*>it=(this->m_ElPointSelectMap);
        QString str0,str1;str0.clear();
        QStringList strL;

        while(it.hasNext()) //(it;it!=m_ElPointSelectMap.end();it++)
        {
            it.next();
            if (str0==""){
                str0=it.key();
                it.value()->GetProperty()->EdgeVisibilityOff();
                it.value()->VisibilityOff();
                m_ElPointSelectMap.remove(it.key());
                render_->RemoveActor(it.value());
                continue;
            }
            str1=it.key();strL=str1.split(",");
            for (int kk=0;kk<strL.size();kk++)
            {
                if(str0.contains(strL[kk]))continue;
                else
                    str0+=","+strL[kk];
            }
            it.value()->GetProperty()->EdgeVisibilityOff();
            it.value()->VisibilityOff();
            m_ElPointSelectMap.remove(it.key());
            render_->RemoveActor(it.value());
        }

        QMapIterator<QString,vtkActor*>itDel=(this->m_ElPointDelMap);
        QString strDel,strDel1;
        while(itDel.hasNext()) //(it;it!=m_ElPointSelectMap.end();it++)
        {
            itDel.next();
            if (strDel==""){
                strDel=itDel.key();
                itDel.value()->GetProperty()->EdgeVisibilityOff();
                itDel.value()->VisibilityOff();
                m_ElPointDelMap.remove(itDel.key());
                render_->RemoveActor(itDel.value());
                continue;;
            }

            strDel1=itDel.key();
            strL=strDel1.split(",");
            for (int kk=0;kk<strL.size();kk++)
            {
                if(strDel.contains(strL[kk]))continue;
                else
                    strDel+=","+strL[kk];
            }
            itDel.value()->GetProperty()->EdgeVisibilityOff();
            itDel.value()->VisibilityOff();
            m_ElPointDelMap.remove(itDel.key());
            render_->RemoveActor(itDel.value());
        }


        QStringList strResult=str0.split(",",QString::SkipEmptyParts);
        strL=strDel.split(",");
        for(int kk=0;kk<strL.size();kk++){
            strResult.removeAt(strResult.indexOf(strL[kk]));// remove(strL[kk],Qt::CaseSensitive);
        }
        renWin_->Render(); 
        this->HighlightProp(NULL);

       /* vtkIdTypeArray *ids=vtkIdTypeArray::New();
        ids->SetNumberOfComponents(1);

        for (int kk=0;kk<strResult.size();kk++ ){
            ids->InsertNextValue(strResult[kk].toInt());
        }*/

        return strResult;
    }

///--------------------------
    void SetFilterActor(QStringList dataList)
    {
        if (m_ElPointSelectMap.find(dataList.join(",")) != m_ElPointSelectMap.end()){//存在
            ;
        }else{
            vtkIdTypeArray *ids=vtkIdTypeArray::New();
            ids->SetNumberOfComponents(1);
            for (int kk=0;kk<dataList.size();kk++){
                ids->InsertNextValue(dataList[kk].toInt()-1);//InsertNextce(CellId);
            }

            vtkSelectionNode *selectionNode=vtkSelectionNode::New();
            selectionNode->SetFieldType(vtkSelectionNode::CELL);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);

            vtkSelection *selection= vtkSelection::New();
            selection->AddNode(selectionNode);

            vtkExtractSelection *extractSelection= vtkExtractSelection::New();
            extractSelection->SetInput(0,this->PickUnPolyData);//this->PolyData);PickUnPolyData
            extractSelection->SetInput(1,selection);
            extractSelection->Update();

            vtkUnstructuredGrid *selected=vtkUnstructuredGrid::New();
            selected->ShallowCopy(extractSelection->GetOutput());

            vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
            TmpSelectdMapper->SetInputConnection(selected->GetProducerPort());
            TmpSelectdMapper->ScalarVisibilityOff();

            m_EditActor=vtkActor::New();
            m_EditActor->SetMapper(TmpSelectdMapper);

            m_EditActor->VisibilityOn();
            m_EditActor->GetProperty()->SetColor(1,0,0);
            m_EditActor->GetProperty()->SetLineWidth(3);
            m_EditActor->GetProperty()->SetRepresentationToWireframe();

            render_->AddActor(m_EditActor);

            m_ElPointSelectMap.insert(dataList.join(","),m_EditActor);
            TmpSelectdMapper->Delete();TmpSelectdMapper=NULL; 
            selected->Delete();selected=NULL;
            extractSelection->Delete();extractSelection=NULL;
            selectionNode->Delete();selectionNode=NULL;
            ids->Delete();ids=NULL;
        }

        render_->Render();
        this->HighlightProp(NULL);  

    }
private:
    //--获取所选Actor中（增加的Id并--删除的Id并集）剩余值(删除临时Actor，保存所选id)
    //str0:增加的id  strDel:删除的Id；
  
  
private:
    vtkPolyData *PolyDataS;
    vtkUnstructuredGrid *PickUnPolyData;//pick use
    
    bool m_LeftButtonUp;
    QMap<QString,vtkActor*>m_ElPointSelectMap;
    vtkActor* m_EditActor;
    QMap<QString,vtkActor*> m_ElPointDelMap;
    int m_pickType;

    vtkActor *OrigActor;
    vtkDataSetMapper *SelectedMapper;

    //QList<int*> m_areaIds;
    vtkRenderer *render_;
    vtkRenderWindow *renWin_; 
};

//点集合
class PointInteractorStyle :public vtkInteractorStyleRubberBandPick
{
public:
    static PointInteractorStyle* New();
    vtkTypeMacro(PointInteractorStyle,vtkInteractorStyleRubberBandPick);
    PointInteractorStyle()
    {
        this->SelectedMapper = vtkDataSetMapper::New();
        this->OrigActor = vtkActor::New();
        this->OrigActor->SetMapper(SelectedMapper);
        m_EditActor=0;
    }
    ~PointInteractorStyle()
    {
        ClearCurrentSelectedMap();
    }

    virtual void OnLeftButtonUp()
    {
        vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
        vtkRenderWindowInteractor*rwi = this->GetInteractor();
        if (!this->m_LeftButtonUp)return;
        
        if (rwi->GetControlKey()){//VTKISRBP_SELECT
            vtkPlanes *frustum =static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();
            vtkExtractGeometry *extractGeometry=vtkExtractGeometry::New();
            extractGeometry->SetImplicitFunction(frustum);
            extractGeometry->SetInput(this->Points); 
            extractGeometry->Update();

            vtkVertexGlyphFilter *glyphFilter = vtkVertexGlyphFilter::New();
            glyphFilter->SetInputConnection(extractGeometry->GetOutputPort());
            glyphFilter->Update();
            
            vtkPolyData *selected = glyphFilter->GetOutput();
            this->SelectedMapper->SetInput(selected);
            this->SelectedMapper->ScalarVisibilityOff();

            vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetPointData()->GetAbstractArray("WelCME"));
            vtkIdType TotalNum=ids->GetNumberOfTuples();
            QString info = "-----------Point Selected--------";  
            if (TotalNum==0){//点选(ok)
                int pos[3]; 
                rwi->GetEventPosition(pos);
                vtkPointPicker *picker=vtkPointPicker::New();
                picker->SetTolerance(0.005);

                picker->Pick(pos[0],pos[1],0,this->GetDefaultRenderer());
                int PointId=picker->GetPointId();
                double *worldPosition=picker->GetPickPosition();
                int showId=0;
                if (PointId ==-1) return;
                int pointNum=PickUnPoints->GetNumberOfPoints();
                vtkPoints *tmpP=PickUnPoints->GetPoints();
                for (int i = 0; i < pointNum; ++i){
                    double xyz[3];
                    tmpP->GetPoint(i, xyz);
                    if (fabs(xyz[0]-worldPosition[0])<0.000001 &&fabs(xyz[1]-worldPosition[1])<0.000001 && fabs(xyz[2]-worldPosition[2])<0.000001){
                        showId = i;
                        QString info ="World Position:"+QString("%1,%2,%3").arg(worldPosition[0]).arg(worldPosition[1]).arg(worldPosition[2]);
                        Information_Widget::GetInstance()->ShowInformation(QString(info));
                        info ="Pick Point Id:"+QString("%1").arg(showId+1);
                        Information_Widget::GetInstance()->ShowInformation(QString(info));
                        info="org PId:"+QString("%1").arg(PointId);
                        Information_Widget::GetInstance()->ShowInformation(QString(info));
                        QString str0=QString("%1").arg(showId+1);

                        if(m_ElPointSelectMap.find(str0)!=m_ElPointSelectMap.end()){//have
                            ;
                        }else{
                            vtkIdTypeArray *ids=vtkIdTypeArray::New();
                            ids->SetNumberOfComponents(1);
                            ids->InsertNextValue(showId);//(PointId);

                            vtkSelectionNode *selectionNode=vtkSelectionNode::New();
                            selectionNode->SetFieldType(vtkSelectionNode::POINT);
                            selectionNode->SetContentType(vtkSelectionNode::INDICES);
                            selectionNode->SetSelectionList(ids);

                            vtkSelection *selection= vtkSelection::New();
                            selection->AddNode(selectionNode);

                            vtkExtractSelection *extractSelection= vtkExtractSelection::New();
                            extractSelection->SetInput(0,this->PickUnPoints);//this->Points);
                            extractSelection->SetInput(1,selection);
                            extractSelection->Update();

                            vtkUnstructuredGrid *selected=vtkUnstructuredGrid::New();
                            selected->ShallowCopy(extractSelection->GetOutput());

                            vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
                            TmpSelectdMapper->SetInputConnection(selected->GetProducerPort());

                            this->SelectedMapper->SetInput(selected);
                            this->SelectedMapper->ScalarVisibilityOff();

                            vtkActor *TmpActor=vtkActor::New();
                            TmpActor->SetMapper(TmpSelectdMapper);

                            TmpActor->GetProperty()->EdgeVisibilityOn();
                            TmpActor->VisibilityOn();
                            TmpActor->GetProperty()->SetEdgeColor(1.0,0.0,0.0);
                            TmpActor->GetProperty()->SetPointSize(5);

                            this->GetDefaultRenderer()->AddActor(TmpActor);                         
                            m_ElPointSelectMap.insert(str0,TmpActor);
                            TmpSelectdMapper->Delete();TmpSelectdMapper=NULL;
                            selected->Delete();selected=NULL;
   
                        }
                        rwi->GetRenderWindow()->Render();
                        this->HighlightProp(NULL);
                    } 

                }//-1
            }else{
                QString str0;
                for (vtkIdType i=0; i<TotalNum;i++)
                {
                    int iPData=ids->GetValue(i)+1;
                    info ="Point Id:"+QString("%1").arg(iPData);
                    Information_Widget::GetInstance()->ShowInformation(QString(info)); 
                    if (i==0){
                        str0=QString("%1").arg(ids->GetValue(i)+1);
                    }else{
                        str0+=","+QString("%1").arg(ids->GetValue(i)+1);
                    }   
                }
                vtkSelectionNode *selectionNode=vtkSelectionNode::New();
                selectionNode->SetFieldType(vtkSelectionNode::POINT);
                selectionNode->SetContentType(vtkSelectionNode::INDICES);
                selectionNode->SetSelectionList(ids);

                vtkSelection *selection= vtkSelection::New();
                selection->AddNode(selectionNode);

                vtkExtractSelection *extractSelection= vtkExtractSelection::New();
                extractSelection->SetInput(0,this->PickUnPoints);
                extractSelection->SetInput(1,selection);
                extractSelection->Update();

                vtkUnstructuredGrid *selecteds=vtkUnstructuredGrid::New();
                selecteds->ShallowCopy(extractSelection->GetOutput());

                vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
                TmpSelectdMapper->SetInputConnection(selecteds->GetProducerPort());
                TmpSelectdMapper->ScalarVisibilityOff();

                vtkActor *TmpActor=vtkActor::New();
                TmpActor->SetMapper(TmpSelectdMapper);
                if (m_ElPointSelectMap.find(str0)!=m_ElPointSelectMap.end()){
                    ;
                }else{
                    TmpActor->GetProperty()->EdgeVisibilityOn();
                    TmpActor->VisibilityOn();
                    TmpActor->GetProperty()->SetEdgeColor(1.0,0.0,0.0);
                    TmpActor->GetProperty()->SetPointSize(5);
                    this->GetDefaultRenderer()->AddActor(TmpActor);
                    m_ElPointSelectMap.insert(str0,TmpActor); 
                }
                TmpSelectdMapper->Delete();TmpSelectdMapper=NULL;
                selecteds->Delete();selecteds=NULL;
                extractSelection->Delete();extractSelection=NULL;
                this->HighlightProp(NULL); 
          }
        }else if (rwi->GetShiftKey()){
            vtkPlanes *frustum =static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();
            vtkExtractGeometry *extractGeometry=vtkExtractGeometry::New();
            extractGeometry->SetImplicitFunction(frustum);
            extractGeometry->SetInput(this->Points); 
            extractGeometry->Update();

            vtkVertexGlyphFilter *glyphFilter = vtkVertexGlyphFilter::New();
            glyphFilter->SetInputConnection(extractGeometry->GetOutputPort());
            glyphFilter->Update();

            vtkPolyData *selected = glyphFilter->GetOutput();
            this->SelectedMapper->SetInput(selected);
            this->SelectedMapper->ScalarVisibilityOff();

            vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetPointData()->GetAbstractArray("WelCME"));
            vtkIdType TotalNum=ids->GetNumberOfTuples();
            QString info = "-----------Point Selected--------";  
            if (TotalNum==0){//点选(ok)
                int pos[3]; 
                rwi->GetEventPosition(pos);
                vtkPointPicker *picker=vtkPointPicker::New();
                picker->SetTolerance(0.005);

                picker->Pick(pos[0],pos[1],0,this->GetDefaultRenderer());
                int PointId=picker->GetPointId();
                double *worldPosition=picker->GetPickPosition();
                int showId=0;
                if (PointId ==-1) return;
                int pointNum=PickUnPoints->GetNumberOfPoints();
                vtkPoints *tmpP=PickUnPoints->GetPoints();
                for (int i = 0; i < pointNum; ++i){
                    double xyz[3];
                    tmpP->GetPoint(i, xyz);
                    if (fabs(xyz[0]-worldPosition[0])<0.000001 &&fabs(xyz[1]-worldPosition[1])<0.000001 && fabs(xyz[2]-worldPosition[2])<0.000001){
                        showId = i;
                        QString info ="World Position:"+QString("%1,%2,%3").arg(worldPosition[0]).arg(worldPosition[1]).arg(worldPosition[2]);
                        Information_Widget::GetInstance()->ShowInformation(QString(info));
                        info ="Pick Point Id:"+QString("%1").arg(showId+1);
                        Information_Widget::GetInstance()->ShowInformation(QString(info));
                        info="org PId:"+QString("%1").arg(PointId);
                        Information_Widget::GetInstance()->ShowInformation(QString(info));
                        QString str0=QString("%1").arg(showId+1);

                        if(m_ElPointDelMap.find(str0)!=m_ElPointDelMap.end()){//have
                            ;
                        }else{
                            vtkIdTypeArray *ids=vtkIdTypeArray::New();
                            ids->SetNumberOfComponents(1);
                            ids->InsertNextValue(showId);//(PointId);

                            vtkSelectionNode *selectionNode=vtkSelectionNode::New();
                            selectionNode->SetFieldType(vtkSelectionNode::POINT);
                            selectionNode->SetContentType(vtkSelectionNode::INDICES);
                            selectionNode->SetSelectionList(ids);

                            vtkSelection *selection= vtkSelection::New();
                            selection->AddNode(selectionNode);

                            vtkExtractSelection *extractSelection= vtkExtractSelection::New();
                            extractSelection->SetInput(0,this->PickUnPoints);//this->Points);
                            extractSelection->SetInput(1,selection);
                            extractSelection->Update();

                            vtkUnstructuredGrid *selected=vtkUnstructuredGrid::New();
                            selected->ShallowCopy(extractSelection->GetOutput());

                            vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
                            TmpSelectdMapper->SetInputConnection(selected->GetProducerPort());

                            this->SelectedMapper->SetInput(selected);
                            this->SelectedMapper->ScalarVisibilityOff();

                            vtkActor *TmpActor=vtkActor::New();
                            TmpActor->SetMapper(TmpSelectdMapper);

                            TmpActor->GetProperty()->EdgeVisibilityOn();
                            TmpActor->VisibilityOn();
                            TmpActor->GetProperty()->SetEdgeColor(1.0,0.0,0.0);
                            TmpActor->GetProperty()->SetPointSize(5);

                            this->GetDefaultRenderer()->AddActor(TmpActor);                         
                            m_ElPointDelMap.insert(str0,TmpActor);
                            TmpSelectdMapper->Delete();TmpSelectdMapper=NULL;
                            selected->Delete();selected=NULL;
                        }
                        rwi->GetRenderWindow()->Render();
                        this->HighlightProp(NULL);
                    } 

                }//-1
            }else{
                QString str0;
                for (vtkIdType i=0; i<TotalNum;i++)
                {
                    int iPData=ids->GetValue(i)+1;
                    info ="Point Id:"+QString("%1").arg(iPData);
                    Information_Widget::GetInstance()->ShowInformation(QString(info)); 
                    if (i==0){
                        str0=QString("%1").arg(ids->GetValue(i)+1);
                    }else{
                        str0+=","+QString("%1").arg(ids->GetValue(i)+1);
                    }   
                }
                vtkSelectionNode *selectionNode=vtkSelectionNode::New();
                selectionNode->SetFieldType(vtkSelectionNode::POINT);
                selectionNode->SetContentType(vtkSelectionNode::INDICES);
                selectionNode->SetSelectionList(ids);

                vtkSelection *selection= vtkSelection::New();
                selection->AddNode(selectionNode);

                vtkExtractSelection *extractSelection= vtkExtractSelection::New();
                extractSelection->SetInput(0,this->PickUnPoints);
                extractSelection->SetInput(1,selection);
                extractSelection->Update();

                vtkUnstructuredGrid *selecteds=vtkUnstructuredGrid::New();
                selecteds->ShallowCopy(extractSelection->GetOutput());

                vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
                TmpSelectdMapper->SetInputConnection(selecteds->GetProducerPort());
                TmpSelectdMapper->ScalarVisibilityOff();

                vtkActor *TmpActor=vtkActor::New();
                TmpActor->SetMapper(TmpSelectdMapper);
                if (m_ElPointDelMap.find(str0)!=m_ElPointDelMap.end()){
                    ;
                }else{ 
                    TmpActor->VisibilityOn();
                    TmpActor->SetProperty(this->OrigActor->GetProperty());
                    TmpActor->GetProperty()->SetPointSize(5);
                    this->GetDefaultRenderer()->AddActor(TmpActor);
                    m_ElPointDelMap.insert(str0,TmpActor); 
                }
                TmpSelectdMapper->Delete();TmpSelectdMapper=NULL;
                selecteds->Delete();selecteds=NULL;
                extractSelection->Delete();extractSelection=NULL;
                this->HighlightProp(NULL); 
            }
        }

    }


    void SetPoints(vtkPolyData *points)
    {
        this->Points = points;       
    }

    void SetPointsOld(vtkUnstructuredGrid *unpoints)
    {
        this->PickUnPoints = unpoints;
    }

    void SetLeftButtonUp(bool f)
    {
        this->m_LeftButtonUp=f;
    }

    void ClearCurrentSelectedMap()
    {
  
        QMapIterator<QString,vtkActor*>itSelect=(this->m_ElPointSelectMap);

        while(itSelect.hasNext()) //(it;it!=m_ElPointSelectMap.end();it++)
        {
            itSelect.next();
            itSelect.value()->GetProperty()->EdgeVisibilityOff();
            itSelect.value()->VisibilityOff();
            m_ElPointSelectMap.remove(itSelect.key());
            render_->RemoveActor(itSelect.value());
        }

        QMapIterator<QString,vtkActor*>itDel=(this->m_ElPointDelMap);
        while(itDel.hasNext()) //(it;it!=m_ElPointSelectMap.end();it++)
        {
            itDel.next();
            itDel.value()->GetProperty()->EdgeVisibilityOff();
            itDel.value()->VisibilityOff();
            m_ElPointDelMap.remove(itDel.key());
            render_->RemoveActor(itDel.value());
        }


        if (m_EditActor!=0)
        {
            m_EditActor->GetProperty()->EdgeVisibilityOff();
            m_EditActor->VisibilityOff(); 
            render_->RemoveActor(m_EditActor); 
        }

        renWin_->Render(); 
        this->HighlightProp(NULL); 
    }

    void setMapData(QStringList DataList)
    {
        SetFilterActor(DataList);
    }
    void InitSetRender(vtkRenderer *R)
    {
        render_ = R;
        renWin_ = render_->GetRenderWindow();
    }
    //-----------
    QStringList GetFilterId()
    {
        QMapIterator<QString,vtkActor*>it=(this->m_ElPointSelectMap);
        QString str0,str1;str0.clear();
        QStringList strL;

        while(it.hasNext()) //(it;it!=m_ElPointSelectMap.end();it++)
        {
            it.next();
            if (str0==""){
                str0=it.key();
                it.value()->GetProperty()->EdgeVisibilityOff();
                it.value()->VisibilityOff();
                m_ElPointSelectMap.remove(it.key());
                render_->RemoveActor(it.value());
                continue;
            }
            str1=it.key();strL=str1.split(",");
            for (int kk=0;kk<strL.size();kk++)
            {
                if(str0.contains(strL[kk]))continue;
                else
                    str0+=","+strL[kk];
            }
            it.value()->GetProperty()->EdgeVisibilityOff();
            it.value()->VisibilityOff();
            m_ElPointSelectMap.remove(it.key());
            render_->RemoveActor(it.value());
        }

        QMapIterator<QString,vtkActor*>itDel=(this->m_ElPointDelMap);
        QString strDel,strDel1;
        while(itDel.hasNext()) //(it;it!=m_ElPointSelectMap.end();it++)
        {
            itDel.next();
            if (strDel==""){
                strDel=itDel.key();
                itDel.value()->GetProperty()->EdgeVisibilityOff();
                itDel.value()->VisibilityOff();
                m_ElPointDelMap.remove(itDel.key());
                render_->RemoveActor(itDel.value());
                continue;;
            }

            strDel1=itDel.key();
            strL=strDel1.split(",");
            for (int kk=0;kk<strL.size();kk++)
            {
                if(strDel.contains(strL[kk]))continue;
                else
                    strDel+=","+strL[kk];
            }
            itDel.value()->GetProperty()->EdgeVisibilityOff();
            itDel.value()->VisibilityOff();
            m_ElPointDelMap.remove(itDel.key());
            render_->RemoveActor(itDel.value());
        }


        QStringList strResult=str0.split(",",QString::SkipEmptyParts);
        strL=strDel.split(",");
        for(int kk=0;kk<strL.size();kk++){
            strResult.removeAt(strResult.indexOf(strL[kk]));// remove(strL[kk],Qt::CaseSensitive);
        }
        renWin_->Render(); 
        this->HighlightProp(NULL);

       /* vtkIdTypeArray *ids=vtkIdTypeArray::New();
        ids->SetNumberOfComponents(1);

        for (int kk=0;kk<strResult.size();kk++ ){
            ids->InsertNextValue(strResult[kk].toInt());
        }*/

        return strResult;
    }
    void SetFilterActor(QStringList dataList)
    {
        if (m_ElPointSelectMap.find(dataList.join(",")) != m_ElPointSelectMap.end()){//存在
            ;
        }else{
            vtkIdTypeArray *ids=vtkIdTypeArray::New();
            ids->SetNumberOfComponents(1);
            for (int kk=0;kk<dataList.size();kk++){
                ids->InsertNextValue(dataList[kk].toInt()-1);//InsertNextce(CellId);
            }

            vtkSelectionNode *selectionNode=vtkSelectionNode::New();
            selectionNode->SetFieldType(vtkSelectionNode::POINT);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);

            vtkSelection *selection= vtkSelection::New();
            selection->AddNode(selectionNode);

            vtkExtractSelection *extractSelection= vtkExtractSelection::New();
            extractSelection->SetInput(0,this->PickUnPoints);//this->PolyData);PickUnPolyData
            extractSelection->SetInput(1,selection);
            extractSelection->Update();

            vtkUnstructuredGrid *selected=vtkUnstructuredGrid::New();
            selected->ShallowCopy(extractSelection->GetOutput());

            vtkDataSetMapper *TmpSelectdMapper=vtkDataSetMapper::New();
            TmpSelectdMapper->SetInputConnection(selected->GetProducerPort());
            //TmpSelectdMapper->ScalarVisibilityOff();
            this->SelectedMapper->SetInput(selected);
            this->SelectedMapper->ScalarVisibilityOff();

            m_EditActor=vtkActor::New();
            m_EditActor->SetMapper(TmpSelectdMapper);

            m_EditActor->VisibilityOn();
            m_EditActor->GetProperty()->SetColor(1,0,0);
            m_EditActor->GetProperty()->SetPointSize(5);
            m_EditActor->GetProperty()->SetRepresentationToPoints();

            render_->AddActor(m_EditActor);

            m_ElPointSelectMap.insert(dataList.join(","),m_EditActor);
            TmpSelectdMapper->Delete();TmpSelectdMapper=NULL; 
            selected->Delete();selected=NULL;
            extractSelection->Delete();extractSelection=NULL;
            selectionNode->Delete();selectionNode=NULL;
            ids->Delete();ids=NULL;
        }

        render_->Render();
        this->HighlightProp(NULL);  

    }
private:
    vtkPolyData *Points;
    vtkUnstructuredGrid *PickUnPoints;//pick use

    bool m_LeftButtonUp;
    QMap<QString,vtkActor*> m_ElPointSelectMap;
    QMap<QString,vtkActor*> m_ElPointDelMap;
    vtkActor* m_EditActor;

    vtkRenderer *render_;
    vtkRenderWindow *renWin_;
    
    
    vtkActor *OrigActor;
    vtkDataSetMapper *SelectedMapper;
};


//#include "vtkInteractorStyleTrackballActor.h"
//
//class MouseInteractorStyle5:public vtkInteractorStyleTrackballActor
//{
//public:
//    static MouseInteractorStyle5*New();
//    vtkTypeMacro(MouseInteractorStyle5,vtkInteractorStyleTrackballActor);
//
//    virtual void OnLeftButtonDown()
//    {
//        vtkInteractorStyleTrackballActor::OnLeftButtonDown();
//        if(this->InteractionProp == this->ActorA)
//        {
//            ;
//        }
//        else if()
//        {
//
//        }
//
//    }
//    vtkActor* ActorA,*ActorB;
//};

class PElSfSetDlg : public QDialog
{
    Q_OBJECT

public:
    PElSfSetDlg(QWidget *parent = NULL);
    ~PElSfSetDlg();
    
private:
    QGroupBox *m_gpbox1,*m_gpbox2,*m_gpbox3,*m_gpbox4;
    QRadioButton *m_PSetBtn,*m_ElSetBtn,*m_SurfSetBtn;
    QCheckBox *m_checkSurf;//仅取表面
    //QCheckBox *m_fineSelect;//精选可编辑
    QGroupBox *m_gbWigPset,*m_gbWigElset,*m_gbWigSfset;
    QLabel *m_lab1,*m_lab1_1,*m_lab2,*m_lab2_1,*m_lab3,*m_lab3_1;
    QLineEdit *m_editPsetName,*m_editElsetName,*m_editSurfsetName;
    MyCombox *m_PsetComb,*m_ElSetComb,*m_SurfSetComb;

    //--
    QPushButton *m_pickPointSetBtn,*m_pickElSetBtn,*m_pickSurfSetBtn;
    QPushButton *m_PointSetClrBtn,*m_ElSetClrBtn,*m_SurfSetClrBtn;//清除
    QPushButton *m_PointSetEditBtn,*m_ElSetEditBtn,*m_SurfSetEditBtn;//编辑按钮
    QPushButton *m_PointSetOkBtn,*m_ElSetOkBtn,*m_SurfSetOkBtn;//确定按钮
    QRadioButton *m_SurfPickerRBtn,*m_SurfSelectRBtn;   
  
    //--
    vtkCallbackCommand *pointPickCall_;
    ElInteractorStyle *m_hightLightSurfPickerStyle;
    ElInteractorStyle *m_hightLightElStyle;
    PointInteractorStyle *m_hightLightPointStyle;

	QMap<QString,ElInteractorStyle*> SurfPickerMap_;
	QMap<QString,ElInteractorStyle*> ElPickerMap_;
	QMap<QString,PointInteractorStyle*> PointStyleMap_;

private:
    InpDataVIS *inpObj_;  

    QMap<QString, QStringList> m_AllPointSetSaveMap;

    QMap<QString, QStringList> m_AllElSetSaveMap;

    QMap<QString, QStringList> m_AllSurfSetSaveMap;

    vtkPolyData *m_surfVTKPolyData;

    vtkRenderWindow *m_renWin;
    vtkRenderWindowInteractor *m_ir;

    vtkPolyData *m_PolyDataInput;
    vtkPolyData *m_OrgWholePointDataInput;//*m_OrgWholeCellDataInput,*m_OrgWholeSurfDataInput;
    vtkPolyData  *m_WholePointDataInput,*m_WholeCellDataInput,*m_WholeSurfDataInput;

    //---Actor
    vtkActor *m_ActorPElSet, *m_ActorSurf;
    vtkAreaPicker *m_areaPicker;
	QMap<QString ,vtkActor*> ActorPElSetMap_, ActorSurfSetMap_;
	QMap<QString, vtkUnstructuredGrid*>disElSetMap_;
    //---
    //--
    bool m_bfine;
    
    void PointSetSelectedPick();//(bool);
    void PointSetOk();
    void PointSetEdit();
    void PointSetClear();
    
    void ElSetSelectedPick();
    void ElSetClear();
    void ElSetEdit();
    void ElSetOK();

    void SurfSetSelectedPick();
    void SurfSetClear();
    void SurfSetEdit();
    void SurfSetOK();

    void clean();

	void CreateFilterData(QStringList ListData);
 private slots:
    void StyleSlot(int Id);
    void PointGroupBtnSlot(int btnId);
    void ElGroupBtnSlot(int BtnId);
    void SurfGroupBtnSlot(int BtnId);
    void SurfGroupRBtnSlot(int btnId);
    void PointcomBoxSlot();
    void ElcomBoxSlot();
    void SurfcomBoxSlot(); 
    void ChangePElIncludeSlot();

   // void ChangeFineorNotSlot();
public slots:
	void UpDataInpElSetDataSlot(QStringList strActorName)/* */;
    void InitInpDataSlot(InpDataVIS *InpObj);
signals:
    void emitPsetHighLight(NElSurfChsS);
    void emitCurrentPSet(QString);
    void emitCurrentElSet(QString);
    void emitCurrentSurfSet(QString);
    void emitCurrentSetsData(QString,int,DecodeGenerateS);
	

};

#endif // PELSFSET_H
