/*=================================================================================

! Module name = Visualization
!
! File name = QVTKUnClip.cxx
!
! Version: ASTE-P 2.0
!
! Time of release: 
!
!       All rights reserved by Advanced Dynamics Corporation, any person or organization
!       must get written permission from Advanced Dynamics Corporation to copy a part or
!       whole of this code except US Army, all NASA Centers and Governmental Agencies 
!       such as USAF, Navy, and DARPA etc. The code is provided as it is and Advanced 
!       Dynamics Corporation will not be responsible for any liability for usage of the 
!       code and any technical support, including the loss of any kind due to the use of 
!       this code, and the user uses it at your own risk.
!
!                            Copyright is protected from 2009-2030
!--------------------------------------------------------------------------------------
! Modified records:
! Date            reviser 
! YYYY/MM/DD      XXXX
!  
! -------------------------------------------------------------------------------------
! Indentation:
!   tab = 4
!
=====================================================================================*/
#include "QVTKUnClip.h"
//#include "vtkVISUnSlice.h"
//add by HaoJingjing 2015.12.11 begin.
#include "vtkCellArray.h"
#include "vtkSortDataArray.h"
#include "vtkDataSetSurfaceFilter.h"
#include <algorithm>
//----------------------------------------------------------------------------

//vtkCxxRevisionMacro(QVTKUnClip, "$Revision: 1.4 $");
vtkStandardNewMacro(QVTKUnClip);
	
//----------------------------------------------------------------------------
QVTKUnClip::QVTKUnClip()
{

	sliceActors_ = new vtkActor*[6];
	sliceTable_ = new vtkLookupTable*[6];

	for (int i = 0; i<6;i++)
	{
		sliceActors_[i] = vtkActor::New();
		sliceActors_[i] = NULL;

		sliceTable_[i] = vtkLookupTable::New();
		sliceTable_[i] = NULL;
	}

	//sliceMapper_ = new vtkDataSetMapper*[6];
	sliceMapper_ = vtkCollection::New();
	sliceMapper_ = NULL;

	contourLinesFilter_ = vtkContourFilter::New ();
	contourLinesFilter_ = NULL;

	contourLabelFilter_ = vtkContourFilter::New ();
	contourLabelFilter_ = NULL;

	sliceGlyph2D_ = vtkGlyphSource2D::New ();
	sliceGlyph2D_ = NULL;

	sliceGlyph3D_ = vtkGlyph3D::New();
	sliceGlyph3D_ = NULL;

	shadeColor_[0] = 1.0;
	shadeColor_[1] = 0.0;
	shadeColor_[2] = 0.0;

	meshColor_[0] = 0.0;
	meshColor_[1] = 1.0;
	meshColor_[2] = 0.0;


	planeNormal_[0] = 1.0;
	planeNormal_[1] = 0.0;
	planeNormal_[2] = 0.0;

	conLevel_ = 5;
	usedScalarIndex_ = -1;
	usedVectorIndex_ = -1;

	for (int i=0; i<3; i++) 
	{
		planeOrigin_[i] = 0;
	}  

	cutSource_ = NULL;
	cutActor_ = NULL;
	gridSource_ = NULL;

	//cutMapper_ = NULL;

	slicePlane_ = NULL;
	planeWidget_ = NULL;

	scalarRange_[0] = 0;
	scalarRange_[1] = 0;

	bandedContourFilter_ = 0;
	contourLinesFilter_ = 0;
	contourLabelFilter_ = 0;
	scalarSource_ = 0;
}
QVTKUnClip::~QVTKUnClip()
{
	ErrorInfo(0, "destructure func 0");
	if (cutActor_ != NULL) 
	{
		if (_renderer != NULL) 
		{    
			_renderer->RemoveActor(cutActor_);     
		}
		cutActor_->Delete();
		cutActor_ = NULL;
	}
	ErrorInfo(0, "destructure func 1");

	if(planeWidget_ != NULL)
	{
		//if(_renWin != NULL)
		//{
		//    planeWidget->SetInteractor(_renWin->GetInteractor());
		//}
		// planeWidget_->EnabledOff();
		planeWidget_->Delete();
		planeWidget_ = NULL;
	}
	ErrorInfo(0, "destructure func 2");

	if (cutSource_ != NULL) 
	{
		cutSource_->Delete();
		cutSource_ = NULL;
	}	
	ErrorInfo(0, "destructure func 3");
	if (gridSource_ != NULL) 
	{
		gridSource_->Delete();
		gridSource_ = NULL;
	}
	ErrorInfo(0, "destructure func 4");

	if(slicePlane_ != NULL)
	{
		slicePlane_->Delete();
		slicePlane_ = NULL;
	}
	ErrorInfo(0, "destructure func 5");

	/*if(cutMapper_ != NULL)
	{
	cutMapper_->Delete();
	cutMapper_ = NULL;
	}*/
	for (int i=0;i<6;i++)
	{
		if (_renderer != NULL) 
		{    
			if (sliceActors_[i] != NULL)
			{
				_renderer->RemoveActor(sliceActors_[i]); 
				sliceActors_[i]->Delete();
				sliceActors_[i] = NULL;
			}
		}
	}
	delete [] sliceActors_;

	for (int i=0;i<6;i++)
	{   
		if (sliceTable_[i] != NULL)
		{
			sliceTable_[i]->Delete();
			sliceTable_[i] = NULL;
		}	
	}
	delete [] sliceTable_;

	ErrorInfo(0, "destructure func 6");

	if (sliceMapper_!= NULL)
	{
		sliceMapper_->Delete ();
		sliceMapper_ = NULL;
	}
	if (contourLinesFilter_!= NULL)
	{
		contourLinesFilter_->Delete();
		contourLinesFilter_ = NULL;
	}
	if (contourLabelFilter_!= NULL)
	{
		contourLabelFilter_->Delete();
		contourLabelFilter_ = NULL;
	}
	if (sliceGlyph2D_!= NULL)
	{
		sliceGlyph2D_->Delete();
		sliceGlyph2D_ = NULL;
	}
	if (sliceGlyph3D_!= NULL)
	{
		sliceGlyph3D_->Delete();
		sliceGlyph3D_ = NULL;
	}	
	if (scalarSource_ != 0) 
	{
		scalarSource_->Delete();
		scalarSource_ = 0;
	}	
	ErrorInfo(0, "destructure func 7 end.");
}
// --------------------------------------------------------------------------
/// Write point,scalar and vector data to file. 
/// 
/// @param FileName the filename to be wrote.
/// 
void QVTKUnClip::WriteScalarVectorData(const char* FileName)
{    
	cutSource_->Update();

	vtkFloatArray* cutScalar = vtkFloatArray::New();
	cutScalar->SetNumberOfComponents(1);

	if(usedScalarIndex_ != -1)
	{
		if(_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
		{
			cutScalar->DeepCopy((cutSource_->GetOutput())->GetPointData()->GetScalars());
		}
		else
		{
			cutScalar->DeepCopy((cutSource_->GetOutput())->GetCellData()->GetScalars());
		}
	}
	cutScalar->Modified();

	vtkFloatArray* cutVector = vtkFloatArray::New();
	cutVector->SetNumberOfComponents(3);

	if(usedVectorIndex_ != -1)
	{
		if(_source->vectorSource[usedVectorIndex_]->GetDataSize() != 0)
		{
			cutVector->DeepCopy((cutSource_->GetOutput())->GetPointData()->GetVectors());
		}
		else
		{
			cutVector->DeepCopy((cutSource_->GetOutput())->GetCellData()->GetVectors());
		}
	}
	cutVector->Modified();

	double range[2];
	double cutScalarRange_[2];

	(cutSource_->GetOutput())->GetScalarRange(range);
	cutScalarRange_[0] = range[0];
	cutScalarRange_[1] = range[1];


	int pointNum = 0;
	pointNum = (cutSource_->GetOutput())->GetNumberOfPoints();

	if (_source->vectorSource[usedVectorIndex_]->GetDataSize() == 0)
	{
		return;
	}
	double value;
	double* v; 
	double p[3];

	ofstream f(FileName);
	if (!f)
	{
		return;
	}
	f<<std::setiosflags(ios::left)<<std::setw(30)<<"x"
		<<std::setiosflags(ios::left)<<std::setw(30)<<"y"
		<<std::setiosflags(ios::left)<<std::setw(30)<<"z"
		<<std::setiosflags(ios::left)<<std::setw(20)<<"scalar"
		<<std::setiosflags(ios::left)<<std::setw(20)<<"v0"
		<<std::setiosflags(ios::left)<<std::setw(20)<<"v1"
		<<std::setiosflags(ios::left)<<std::setw(20)<<"v2"
		<<endl;

	for(int j=0; j<pointNum; j++)
	{
		(cutSource_->GetOutput())->GetPoint(j, p);

		value = cutScalar->GetValue(j);

		v = cutVector->GetTuple3(j);

		f<<std::setiosflags(ios::left)<<std::setw(20)<<p[0]
		<<std::setiosflags(ios::left)<<std::setw(20)<<p[1]
		<<std::setiosflags(ios::left)<<std::setw(20)<<p[2]
		<<std::setiosflags(ios::left)<<std::setw(20)<<value
			<<std::setiosflags(ios::left)<<std::setw(20)<<v[0]
		<<std::setiosflags(ios::left)<<std::setw(20)<<v[1]
		<<std::setiosflags(ios::left)<<std::setw(20)<<v[2]
		<<endl;
	}
	f.close();
}
// --------------------------------------------------------------------------
/// Get cut scalar data.
/// 
/// @return 
/// 
vtkFloatArray* QVTKUnClip::GetCutScalarData(void)
{    
	if(usedScalarIndex_ == -1)
	{
		return NULL;
	}
	if (cutSource_==NULL)
	{
		return NULL;
	}
	cutSource_->Update();

	vtkFloatArray* cutScalar = vtkFloatArray::New();
	cutScalar->SetNumberOfComponents(1);

	if(usedScalarIndex_ != -1)
	{
		if(_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
		{
			cutScalar->DeepCopy((cutSource_->GetOutput())->GetPointData()->GetScalars());
		}
		else
		{
			cutScalar->DeepCopy((cutSource_->GetOutput())->GetCellData()->GetScalars());
		}
	}
	cutScalar->Modified();

	return cutScalar;
}
// --------------------------------------------------------------------------
/// Get cut point data by index.
/// 
/// @param index Indicate the coordinate.
/// @li 0   x
/// @li 1   y
/// @li 2   z
vtkFloatArray* QVTKUnClip::GetCutPointData(int index)
{
	if (index < 0 || index > 2)
	{
		return NULL;
	}

	if (cutSource_==NULL)
	{
		return NULL;
	}
	cutSource_->Update();

	vtkFloatArray* cutPointData = vtkFloatArray::New();
	cutPointData->SetNumberOfComponents(1);

	int pointNum = (cutSource_->GetOutput())->GetNumberOfPoints();

	double point[3];

	for(int j=0; j<pointNum; j++)
	{
		(cutSource_->GetOutput())->GetPoint(j, point);

		cutPointData->InsertNextValue(point[index]);
	}
	cutPointData->Modified();

	return cutPointData;
}
// --------------------------------------------------------------------------
/// Change slice scalar/vector data source
/// 
/// @param scalar
/// 
/// @return 
/// 
void QVTKUnClip::ModifySliceSourceScalar(char* scalar)
{
	if (gridSource_ == NULL)
	{
		ErrorInfo(1,"Grid Source are not Setted!");
		return;     
	}  

	int index = -1;
	if(scalar != NULL)
	{
		index = _source->GetScalarIndex(scalar);
	}
	if (index == -1)
	{
		ErrorInfo(1,"Scalar data are not existed!");
		return;
	}
	if (_source->scalarSource[index]->GetDataSize() != 0)
	{
		(gridSource_->GetPointData())->SetScalars(_source->scalarSource[index]);
	}
	else 
	{
		(gridSource_->GetCellData())->SetScalars(_source->cellScalarSource_[index]);
	}
	gridSource_->Modified();

	usedScalarIndex_ = index;

	//2011-05-10 zhuqin add begin.
	if(bandedContourFilter_ != NULL)
	{
		bandedContourFilter_->GenerateValues(conLevel_, _source->scalarRange[usedScalarIndex_][0],_source->scalarRange[usedScalarIndex_][1]);
		bandedContourFilter_->ClippingOn();
		bandedContourFilter_->Modified();
	}
	if (contourLinesFilter_ != NULL)
	{
		contourLinesFilter_->GenerateValues(conLevel_, _source->scalarRange[usedScalarIndex_][0],_source->scalarRange[usedScalarIndex_][1]);
		contourLinesFilter_->Modified();
	}
	if (contourLabelFilter_!= NULL)
	{
		contourLabelFilter_->GenerateValues(conLevel_, _source->scalarRange[usedScalarIndex_][0],_source->scalarRange[usedScalarIndex_][1]);
		contourLabelFilter_->Modified();
	}
	//2011-05-10 zhuqin add end.


	for (int i= 0; i<6; i++)
	{
		if (i == 0 || i== 1 || i == 3)
		{
			if(sliceActors_[i] != NULL)
			{
				(sliceActors_[i]->GetMapper())->GetLookupTable()->SetRange(_source->scalarRange[usedScalarIndex_][0],_source->scalarRange[usedScalarIndex_][1]);
				(sliceActors_[i]->GetMapper())->GetLookupTable()->Modified();

				(sliceActors_[i]->GetMapper())->SetScalarRange(_source->scalarRange[usedScalarIndex_][0],_source->scalarRange[usedScalarIndex_][1]);

				if (_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
				{
					(sliceActors_[i]->GetMapper())->SetScalarModeToUsePointData();
				}
				else
				{
					(sliceActors_[i]->GetMapper())->SetScalarModeToUseCellData();
				}

				(sliceActors_[i]->GetMapper())->Modified();
				sliceActors_[i]->Modified();
			}
		}

	}

}
// --------------------------------------------------------------------------
/// Change slice scalar/vector data source
/// 
/// @param vector
/// 
/// @return 
/// 
void QVTKUnClip::ModifySliceSourceVector(char* vector)
{
	if (gridSource_ == NULL)
	{
		ErrorInfo(1,"Grid Source are not Setted!");
		return;     
	} 

	int index = -1;

	if(vector != NULL)
	{
		index = _source->GetVectorIndex(vector);
	}
	if (index == -1)
	{
		ErrorInfo(1,"Vector data are not existed!");
		return;
	}       

	usedVectorIndex_ = index;

	if(_source->vectorSource[index]->GetDataSize() != 0)
	{
		(gridSource_->GetPointData())->SetVectors(_source->vectorSource[index]);
	}
	else 
	{
		(gridSource_->GetCellData())->SetVectors(_source->cellVectorSource_[index]);
	}
	gridSource_->Modified();
}      
// --------------------------------------------------------------------------
/// Create slice plane widget
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::CreateSliceWidget()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}
	double* bounds;
	bounds = _source->GetSourceBounds();

	planeOrigin_[0] = (bounds[0]+bounds[1])/2;
	planeOrigin_[1] = (bounds[2]+bounds[3])/2;
	planeOrigin_[2] = (bounds[4]+bounds[5])/2;

	vtkImplicitPlaneWidget* planeWidget = vtkImplicitPlaneWidget::New();
	planeWidget->SetInteractor(_renWin->GetInteractor());
	planeWidget->SetPlaceFactor(1.0);

	planeWidget->SetHandleSize(0.005);
	planeWidget->SetDiagonalRatio(0.2);

	planeWidget->GetOutlineProperty()->SetOpacity(0.0);
	planeWidget->GetEdgesProperty()->SetOpacity(0.0);

	planeWidget->GetPlane(slicePlane_);      
	planeWidget->SetInput(cutSource_->GetOutput());

	planeWidget->TubingOff();
	//planeWidget->OutsideBoundsOff();
	planeWidget->SetOrigin(planeOrigin_[0], planeOrigin_[1], planeOrigin_[2]);
	planeWidget->PlaceWidget(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]);
	planeWidget->On();

	planeWidget_ = planeWidget;

	vtkCallbackCommand *cc = vtkCallbackCommand::New();
	cc->SetCallback(GenerateSliceProData);
	cc->SetClientData(this);

	planeWidget->AddObserver(vtkCommand::InteractionEvent, cc);

	ShowOffSlicePlane();
}
void QVTKUnClip::GenerateSliceProData(vtkObject *caller, unsigned long, void* obj, void *)
{
	QVTKUnClip* usObj = (QVTKUnClip*)obj;

	if (usObj->planeWidget_ == NULL)
	{
		usObj->ErrorInfo(1,"Plane Widget Not Created");
		return;
	}
	usObj->planeWidget_->GetPlane(usObj->slicePlane_);

	//int num;
	//num = cutSource_->GetOutput()->GetNumberOfPoints();
}
// --------------------------------------------------------------------------
/// Show the slice plane
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::ShowOnSlicePlane()
{
	if (planeWidget_ != NULL)
	{
		planeWidget_->DrawPlaneOn(); 
		planeWidget_->UpdatePlacement(); 
	}

}
// --------------------------------------------------------------------------
/// Hide the slice plane
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::ShowOffSlicePlane()
{
	if (planeWidget_ != NULL)
	{
		planeWidget_->DrawPlaneOff();    
		planeWidget_->UpdatePlacement();
	}

}
// --------------------------------------------------------------------------
/// Show the widget
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::ShowOnSlicePlaneWidget()
{
	if (planeWidget_ != NULL)
	{
		planeWidget_->On();
	}

	if (cutActor_) 
	{
		cutActor_->VisibilityOn();
	}
}

// --------------------------------------------------------------------------
/// Hide the widget
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::ShowOffSlicePlaneWidget()
{
	if (planeWidget_ != NULL)
	{
		planeWidget_->Off();
	}

	if (cutActor_) 
	{
		cutActor_->VisibilityOff();
	}
}
// --------------------------------------------------------------------------
/// Set the normal of the slice plane
/// 
/// @param nx
/// @param ny
/// @param nz
/// 
/// @return 
/// 
void QVTKUnClip::SetSlicePlaneNormal(double nx, double ny, double nz)
{
	if (nx == 0 && ny == 0 && nz == 0) {
		return;
	}

	if (planeWidget_ != NULL)
	{
		planeNormal_[0] = nx;
		planeNormal_[1] = ny;
		planeNormal_[2] = nz;

		slicePlane_->SetNormal(nx, ny, nz);
		planeWidget_->SetNormal(nx, ny, nz);	
	} 
}
// --------------------------------------------------------------------------
/// Set the normal of the slice plane
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::SetSlicePlaneNormalToX()
{
	if (planeWidget_ != NULL)
	{
		slicePlane_->SetNormal(1, 0, 0);
		planeWidget_->SetNormal(1, 0, 0);

		planeNormal_[0] = 1.0;
		planeNormal_[1] = 0;
		planeNormal_[2] = 0;
	} 
}
// --------------------------------------------------------------------------
/// Set the normal of the slice plane
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::SetSlicePlaneNormalToY()
{
	if (planeWidget_ == NULL)
	{
		ErrorInfo(1,"Plane Widget Not Created");
		return;
	}
	slicePlane_->SetNormal(0, 1, 0);
	planeWidget_->SetNormal(0, 1, 0);

	planeNormal_[0] = 0;
	planeNormal_[1] = 1.0;
	planeNormal_[2] = 0;
}
// --------------------------------------------------------------------------
/// Set the normal of the slice plane
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::SetSlicePlaneNormalToZ()
{
	if (planeWidget_ == NULL)
	{
		ErrorInfo(1,"Plane Widget Not Created");
		return;
	}
	slicePlane_->SetNormal(0, 0, 1);
	planeWidget_->SetNormal(0, 0, 1);

	planeNormal_[0] = 0;
	planeNormal_[1] = 0;
	planeNormal_[2] = 1.0;
}

// --------------------------------------------------------------------------
/// Get the normal direction of the slice plane
/// 
/// @param 
///
/// @return double[3] contains the normal direction
/// 

double* QVTKUnClip::GetSlicePlaneNormal()
{
	return this->planeNormal_;
}
// --------------------------------------------------------------------------
/// Set the slice plane origin
/// 
/// @param ox
/// @param oy
/// @param oz
/// 
/// @return 
/// 
void QVTKUnClip::SetSlicePlaneOrigin(double ox, double oy, double oz)
{
	if (planeWidget_ == NULL)
	{
		ErrorInfo(1,"Plane Widget Not Created");
		return;
	}
	slicePlane_->SetOrigin(ox, oy, oz);
	planeWidget_->SetOrigin(ox, oy, oz);

	planeOrigin_[0] = ox;
	planeOrigin_[0] = oy;
	planeOrigin_[0] = oz;

}
// --------------------------------------------------------------------------
/// Get the slice plane origin
/// 
/// @param 
/// 
/// @return double[3] contains the origin
/// 
double* QVTKUnClip::GetSlicePlaneOrigin()
{
	return this->planeOrigin_;
}
// --------------------------------------------------------------------------
/// Create UnSlice display pipeline
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::CreateUnSliceContourDisplay()
{    
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	double range[2];
	if (usedScalarIndex_ != -1)
	{     
		range[0] = _source->scalarRange[usedScalarIndex_][0];
		range[1]  = _source->scalarRange[usedScalarIndex_][1];
	}
	else 
	{
		range[0] = 0;
		range[1] = 0;
	}
	cutSource_->Update();

	//cout<<range[0]<<" "<<range[1]<<endl;

	//cout<<"cut  num:  "<<(cutSource_->GetOutput())->GetNumberOfPoints()<<endl;

	vtkGeometryFilter *geoFilter = vtkGeometryFilter::New();
	geoFilter->SetInput(cutSource_->GetOutput());
	geoFilter->Update();

	//cout<<"geo num: "<<(geoFilter->GetOutput())->GetNumberOfPoints()<<endl;

	vtkBandedPolyDataContourFilter *conFilter = vtkBandedPolyDataContourFilter::New();
	conFilter->SetInput(geoFilter->GetOutput());
	conFilter->GenerateValues(conLevel_, range[0], range[1]);
	conFilter->ClippingOn();
	conFilter->Modified();
	conFilter->Update();

	//cout<<"con num: "<<(conFilter->GetOutput())->GetNumberOfPoints()<<endl;


	vtkDataSetMapper *mapper = vtkDataSetMapper::New();
	mapper->InterpolateScalarsBeforeMappingOn();

	if(_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
	{
		//cout<<"test here 3333"<<endl;

		mapper->SetInputConnection(conFilter->GetOutputPort());
	}
	else
	{
		mapper->SetInputConnection(geoFilter->GetOutputPort());
	}
	mapper->SetScalarRange(range[0],range[1]);

	if (usedScalarIndex_ != -1)
	{
		if (_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
		{
			mapper->SetScalarModeToUsePointData();
		}
		else
		{
			mapper->SetScalarModeToUseCellData();
		}
	}

	vtkLookupTable *table = (vtkLookupTable*)mapper->GetLookupTable(); 
	table->GetHueRange(range);
	table->SetHueRange(range[1],range[0]);
	table->SetNumberOfColors(conLevel_);

	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);    

	if (_renderer != NULL) 
	{    
		_renderer->AddActor(actor);     
	}
	//actor->GetProperty()->SetRepresentationToSurface();
	sliceActors_[0] = actor;

	sliceTable_[0] = table;

	bandedContourFilter_ = conFilter;
}
// --------------------------------------------------------------------------
/// Show On UnSlice Display 
/// 
/// @param 
/// 
/// @return 
void QVTKUnClip::ShowOnUnSliceContourDisplay()
{
	if (sliceActors_[0] != NULL)
	{
		//ClippingOffSliceContourDisplay();
		//sliceActors_[0]->GetProperty()->SetRepresentationToSurface();
		sliceActors_[0]->VisibilityOn();
		//sliceActors_[0]->GetProperty()->SetRepresentationToSurface();
	}      
}
// --------------------------------------------------------------------------
/// Show Off UnSlice Display 
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::ShowOffUnSliceContourDisplay()
{
	if (sliceActors_[0] != NULL)
	{

		sliceActors_[0]->VisibilityOff();

	} 
}
// --------------------------------------------------------------------------
///Delete UnSlice Display 
/// 
/// @param 
/// 
/// @return g
/// 
void QVTKUnClip::DeleteUnSliceContourDisplay()
{
	if (sliceActors_[0] != NULL)
	{
		sliceActors_[0]->VisibilityOff();

		if (_renderer != NULL)
		{
			_renderer->RemoveActor(sliceActors_[0]);
		}

		sliceActors_[0]->Delete();
	} 
}
// --------------------------------------------------------------------------
/// Create UnSlice Contour Lines Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::CreateUnSliceContourLinesDisplay()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	double range[2];
	if (usedScalarIndex_ != -1)
	{     
		range[0] = _source->scalarRange[usedScalarIndex_][0];
		range[1] = _source->scalarRange[usedScalarIndex_][1];
	}
	else 
	{
		range[0] = 0;
		range[1] = 0;
	}

	vtkContourFilter *conFilter = vtkContourFilter::New();
	conFilter->SetInputConnection(cutSource_->GetOutputPort());
	conFilter->GenerateValues(conLevel_, range[0],range[1]);
	conFilter->Update();

	vtkDataSetMapper *mapper = vtkDataSetMapper::New();
	mapper->InterpolateScalarsBeforeMappingOn();
	mapper->SetInputConnection(conFilter->GetOutputPort());
	mapper->SetScalarRange(range[0],range[1]);

	if (usedScalarIndex_ != -1)
	{
		if (_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
		{
			mapper->SetScalarModeToUsePointData();
		}
		else
		{
			mapper->SetScalarModeToUseCellData();
		}
	}

	vtkLookupTable *table = (vtkLookupTable*)mapper->GetLookupTable(); 
	table->GetHueRange(range);
	table->SetHueRange(range[1],range[0]);
	table->SetNumberOfColors(conLevel_);

	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);

	if (_renderer != NULL) 
	{    
		_renderer->AddActor(actor);   
	}

	contourLinesFilter_ = conFilter;
	/*if (sliceMapper_ != NULL)
	{
	sliceMapper_->AddItem (mapper);
	}*/
	sliceActors_[1] = actor;
	sliceTable_[1] = table; 
}
// --------------------------------------------------------------------------
/// Show On UnSlice Contour Lines Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ShowOnUnSliceContourLinesDisplay()
{
	if(sliceActors_[1] != NULL)
	{
		sliceActors_[1]->VisibilityOn();
	}
}
// --------------------------------------------------------------------------
/// Show Off UnSlice Contour Lines Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ShowOffUnSliceContourLinesDisplay()
{
	if(sliceActors_[1] != NULL)
	{
		sliceActors_[1]->VisibilityOff();
	}
}
// --------------------------------------------------------------------------
/// Delete UnSlice Contour Lines Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::DeleteUnSliceCOntourLinesDisplay()
{
	if (sliceActors_[1] != NULL)
	{
		sliceActors_[1]->VisibilityOff();

		if (_renderer != NULL)
		{
			_renderer->RemoveActor(sliceActors_[1]);
		}

		sliceActors_[1]->Delete();
	} 
}
// --------------------------------------------------------------------------
/// Create UnSlice Contour Lines Label Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::CreateUnSliceContourLinesLabelDisplay()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	double range[2];
	if (usedScalarIndex_ != -1)
	{     
		range[0] = _source->scalarRange[usedScalarIndex_][0];
		range[1] = _source->scalarRange[usedScalarIndex_][1];
	}
	else 
	{
		range[0] = 0;
		range[1] = 0;
	}


	vtkContourFilter *labelFilter = vtkContourFilter::New();
	labelFilter->SetInputConnection(cutSource_->GetOutputPort());
	labelFilter->GenerateValues(conLevel_, range[0],range[1]);
	labelFilter->Update();

	int numPoints = labelFilter->GetOutput()->GetNumberOfPoints();
	vtkMaskPoints *mask = vtkMaskPoints::New();
	mask->SetInputConnection(labelFilter->GetOutputPort());
	mask->SetOnRatio((double)numPoints/conLevel_);
	mask->SetMaximumNumberOfPoints(50);
	mask->RandomModeOn();

	vtkSelectVisiblePoints *visPoints = vtkSelectVisiblePoints::New();
	visPoints->SetInputConnection(mask->GetOutputPort());
	visPoints->SetRenderer(_renderer);

	vtkLabeledDataMapper *mapper = vtkLabeledDataMapper::New();
	mapper->GlobalWarningDisplayOff();
	mapper->SetInputConnection(mask->GetOutputPort());
	mapper->SetLabelModeToLabelScalars();
	mapper->GetLabelTextProperty()->SetFontFamilyToArial();
	mapper->GetLabelTextProperty()->SetFontSize(10);
	mapper->GetLabelTextProperty()->SetColor(1,1,1);

	vtkActor2D *actor = vtkActor2D::New();
	actor->SetMapper(mapper);

	if (_renderer != NULL)
	{
		_renderer->AddActor2D(actor);
	}

	contourLabelFilter_ = labelFilter;
	sliceActors_[2] = (vtkActor*) actor;
	/*if (sliceMapper_ != NULL)
	{
	sliceMapper_->AddItem (mapper);
	}*/
	sliceTable_[2] = NULL; 

}
// --------------------------------------------------------------------------
/// Show On UnSlice Contour Lines Label Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ShowOnUnSliceContourLinesLabelDisplay()
{
	if(sliceActors_[2] != NULL)
	{
		sliceActors_[2]->VisibilityOn();
	}
}
// --------------------------------------------------------------------------
/// Show Off UnSlice Contour Lines Display
/// 
/// @param
/// 
/// @return 
void QVTKUnClip::ShowOffUnSliceContourLinesLabelDisplay()
{
	if(sliceActors_[2] != NULL)
	{
		sliceActors_[2]->VisibilityOff();
	}
}
// --------------------------------------------------------------------------
/// Delete UnSlice Contour Lines Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::DeleteUnSliceContourLinesLabelDisplay()
{
	if (sliceActors_[2] != NULL)
	{
		sliceActors_[2]->VisibilityOff();

		if (_renderer != NULL)
		{
			_renderer->RemoveActor(sliceActors_[2]);
		}

		sliceActors_[2]->Delete();
	} 
}

// --------------------------------------------------------------------------
/// Create UnSlice Vector Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::CreateUnSliceVectorDisplay()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	double range[2];
	if (usedScalarIndex_ != -1)
	{     
		range[0] = _source->scalarRange[usedScalarIndex_][0];
		range[1] = _source->scalarRange[usedScalarIndex_][1];
	}
	else 
	{
		range[0] = 0;
		range[1] = 0;
	}

	vtkGlyphSource2D *glyph2D = vtkGlyphSource2D::New();
	glyph2D->SetGlyphTypeToArrow();
	glyph2D->SetScale(1.0);
	glyph2D->SetScale2(1.5);
	glyph2D->SetCenter(0, 0, 0);
	glyph2D->FilledOff();

	vtkGlyph3D *glyph3D = vtkGlyph3D::New();
	glyph3D->SetInputConnection(cutSource_->GetOutputPort());
	glyph3D->SetSource(glyph2D->GetOutput());
	glyph3D->ScalingOn();
	glyph3D->ClampingOn();
	glyph3D->GeneratePointIdsOff();
	glyph3D->SetIndexModeToOff();
	glyph3D->SetVectorModeToUseVector();
	glyph3D->SetScaleFactor(0.2);

	vtkDataSetMapper *mapper = vtkDataSetMapper::New();
	mapper->SetInput(glyph3D->GetOutput());
	mapper->SetScalarRange(range[0], range[1]);
	mapper->InterpolateScalarsBeforeMappingOn();

	if (usedScalarIndex_ != -1)
	{
		if (_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
		{
			mapper->SetScalarModeToUsePointData();
		}
		else
		{
			mapper->SetScalarModeToUseCellData();
		}
	}

	vtkLookupTable *table = (vtkLookupTable*)mapper->GetLookupTable(); 
	table->GetHueRange(range);
	table->SetHueRange(range[1], range[0]);
	table->SetNumberOfColors(conLevel_);

	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);

	if (_renderer != NULL)
	{
		_renderer->AddActor(actor);  
	}
	/*if (sliceMapper_ != NULL)
	{
	sliceMapper_->AddItem (mapper);
	}*/
	sliceActors_[3] = actor;
	sliceTable_[3] = table; 
	sliceGlyph2D_ = glyph2D;
	sliceGlyph3D_ = glyph3D;

}
// --------------------------------------------------------------------------
/// Show On UnSlice Vector Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ShowOnUnSliceVectorDisplay()
{
	if(sliceActors_[3] != NULL)
	{	
		sliceActors_[3]->VisibilityOn();
	}
}
// --------------------------------------------------------------------------
/// Show Off UnSlice Vector Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ShowOffUnSliceVectorDisplay()
{
	if(sliceActors_[3] != NULL)
	{
		sliceActors_[3]->VisibilityOff();	
	}
}
// --------------------------------------------------------------------------
/// Delete UnSlice Vector Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::DeleteUnSliceVectorDisplay()
{
	if (sliceActors_[3] != NULL)
	{
		sliceActors_[3]->VisibilityOff();

		if (_renderer != NULL)
		{
			_renderer->RemoveActor(sliceActors_[3]);
		}

		sliceActors_[3]->Delete();
	} 
}
// --------------------------------------------------------------------------
/// Create UnSlice Shade Display
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::CreateUnSliceShadeDisplay()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	vtkGeometryFilter *geoFilter=vtkGeometryFilter::New();
	geoFilter->SetInput(cutSource_->GetOutput()); 

	vtkDataSetMapper *mapper=vtkDataSetMapper::New();
	mapper->SetInputConnection(geoFilter->GetOutputPort());
	mapper->ScalarVisibilityOff();

	vtkActor *actor=vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(shadeColor_[0], shadeColor_[1], shadeColor_[2]);
	actor->GetProperty()->SetRepresentationToSurface();

	if (_renderer != NULL)
	{
		_renderer->AddActor(actor);
	}

	if (sliceMapper_ != NULL)
	{
		sliceMapper_->AddItem (mapper);
	}
	sliceTable_[4] = NULL;
	sliceActors_[4] = actor;

}
// --------------------------------------------------------------------------
/// Modify UnSlice Shade Color
/// 
/// @param c0 -- color r
/// @param c1 -- color g
/// @param c2 -- color b
/// 
/// @return 
/// 
void QVTKUnClip::ModifyUnSliceShadeColor(double c0, double c1, double c2)
{    
	if (sliceActors_[4]!=NULL)
	{
		shadeColor_[0] = c0;
		shadeColor_[1] = c1;
		shadeColor_[2] = c2;

		(sliceActors_[4]->GetProperty())->SetColor(c0, c1, c2);
	}
}
// --------------------------------------------------------------------------
/// Show On UnSlice Shade Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ShowOnUnSliceShade()
{
	if(sliceActors_[4] != NULL)
	{
		sliceActors_[4]->VisibilityOn();
		//sliceActors_[4]->GetProperty()->SetRepresentationToSurface();
	}
}
// --------------------------------------------------------------------------
/// Show Off UnSlice Shade Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ShowOffUnSliceShade()
{
	if(sliceActors_[4] != NULL)
	{
		sliceActors_[4]->VisibilityOff();
	}
}
// --------------------------------------------------------------------------
/// Create UnSlice Mesh Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::CreateUnSliceMeshDisplay()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	vtkGeometryFilter *geoFilter=vtkGeometryFilter::New();
	geoFilter->SetInput(cutSource_->GetOutput()); 

	vtkDataSetMapper *mapper=vtkDataSetMapper::New();
	mapper->SetInputConnection(geoFilter->GetOutputPort());
	mapper->ScalarVisibilityOff();

	vtkActor *actor=vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(meshColor_[0], meshColor_[1], meshColor_[2]);  
	//(actor->GetProperty())->SetRepresentationToSurface();

	if (_renderer != NULL)
	{
		_renderer->AddActor(actor);
	}

	if (sliceMapper_ != NULL)
	{
		sliceMapper_->AddItem (mapper);
	}
	sliceTable_[5] = NULL;
	sliceActors_[5] = actor;
}
// --------------------------------------------------------------------------
/// Modify UnSlice Mesh Color
/// 
/// @param c0 -- color r
/// @param c1 -- color g
/// @param c2 -- color b
/// 
/// @return 
/// 
void QVTKUnClip::ModifyUnSliceMeshColor(double c0, double c1, double c2)
{    
	if (sliceActors_[5]!=NULL)
	{
		shadeColor_[0] = c0;
		shadeColor_[1] = c1;
		shadeColor_[2] = c2;

		(sliceActors_[5]->GetProperty())->SetColor(c0, c1, c2);
	}
}
// --------------------------------------------------------------------------
/// Show On UnSlice Mesh Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ShowOnUnSliceMesh()
{
	if(sliceActors_[5] != NULL)
	{
		sliceActors_[5]->VisibilityOn();
		//sliceActors_[5]->GetProperty()->SetRepresentationToSurface();
	}
}
// --------------------------------------------------------------------------
/// Show Off UnSlice Mesh Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ShowOffUnSliceMesh()
{
	if(sliceActors_[5] != NULL)
	{
		sliceActors_[5]->VisibilityOff();
	}
}
// --------------------------------------------------------------------------
/// Set UnSlice Contour Opacity
/// 
/// @param value -- opacity value
/// 
/// @return 
/// 
void QVTKUnClip::SetUnSliceContourOpacity(double value)
{
	if (sliceActors_[0] != NULL)
	{
		(sliceActors_[0]->GetProperty())->SetOpacity(value);
	}
}
// --------------------------------------------------------------------------
/// Get UnSlice Contour Opacity
/// 
/// @param blockIndex
/// 
/// @return opacity value
/// 
double QVTKUnClip::GetUnSliceContourOpacity()
{
	if (sliceActors_[0] != NULL)
	{
		return (sliceActors_[0]->GetProperty())->GetOpacity();
	}
	return NULL;
}
// --------------------------------------------------------------------------
/// Modify UnSlice Vector Color Mode
/// 
/// @param mode -- color mode
/// @li "ColorByScale";
/// @li "ColorByScalar";
/// @li "ColorByVector";
///
/// @return 
void QVTKUnClip::ModifyUnSliceVectorColorMode(char* mode)
{

	if (sliceGlyph3D_ != NULL)
	{
		if (strcmp(mode, "ColorByScale") == 0)
		{
			sliceGlyph3D_->SetColorModeToColorByScale();
		}
		else if (strcmp(mode, "ColorByScalar") == 0)
		{
			sliceGlyph3D_->SetColorModeToColorByScalar();
		}
		else if (strcmp(mode, "ColorByVector") == 0)
		{
			sliceGlyph3D_->SetColorModeToColorByVector();
		}
		else 
		{
			sliceGlyph3D_->SetColorModeToColorByScalar();
		}
	}
}
// --------------------------------------------------------------------------
/// Modify UnSlice Vector Scale Mode
/// 
/// @param mode -- Scale mode
/// @li "ScaleByScalar"
/// @li "ScaleByVector"
/// @li "ScaleByVectorComponents"
/// @li "DataScalingOff"
/// 
/// @return 
/// 
void QVTKUnClip::ModifyUnSliceVectorScaleMode(char* mode)
{

	if (sliceGlyph3D_ != NULL)
	{
		if (strcmp(mode, "ScaleByScalar") == 0)
		{
			sliceGlyph3D_->SetScaleModeToScaleByScalar();
		}
		else if (strcmp(mode, "ScaleByVectorComponents") == 0)
		{
			sliceGlyph3D_->SetScaleModeToScaleByVector();
		}
		else if (strcmp(mode, "ColorByVector") == 0)
		{
			sliceGlyph3D_->SetScaleModeToScaleByVectorComponents();
		}
		else if (strcmp(mode, "DataScalingOff") == 0)
		{
			sliceGlyph3D_->SetScaleModeToDataScalingOff ();
		}
		else 
		{
			sliceGlyph3D_->SetScaleModeToScaleByScalar();
		}
	}
}
// --------------------------------------------------------------------------
/// Modify UnSlice Vector Scale factor
/// 
/// @param scale -- Scale factor
/// 
/// @return 
/// 
void QVTKUnClip::ModifyUnSliceVectorScaleFactor(double scaleFactor)
{

	if (sliceGlyph3D_ != NULL)
	{
		sliceGlyph3D_->SetScaleFactor(scaleFactor);
	}
}
// --------------------------------------------------------------------------
/// Set UnSlice Vector Filled On
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::SetUnSliceVectorFilledOn()
{
	if (sliceGlyph2D_ != NULL)
	{
		sliceGlyph2D_->FilledOn();
	}
}
// --------------------------------------------------------------------------
/// Set UnSlice Vector Filled Off
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::SetUnSliceVectorFilledOff()
{   
	if (sliceGlyph2D_ != NULL)
	{
		sliceGlyph2D_->FilledOff();
	}
}
// --------------------------------------------------------------------------
/// Modify UnSlice Vector Position
/// 
/// @param p0 -- position x
/// @param p1 -- position y
/// @param p2 -- position z
/// 
/// @return 
/// 
void QVTKUnClip::ModifyUnSliceVectorPosition(double p0, double p1, double p2)
{

	if (sliceGlyph2D_ != NULL)
	{
		sliceGlyph2D_->SetCenter(p0, p1, p2);
	}    
}
// --------------------------------------------------------------------------
/// Modify UnSlice Shade
/// 
/// @param value -- opacity value
///
/// @return 
/// 
void QVTKUnClip::ModifyUnSliceShadeOpacity(double value)
{
	if (sliceActors_[4] != NULL)
	{
		(sliceActors_[4]->GetProperty())->SetOpacity(value);
	}
}
// --------------------------------------------------------------------------
/// Modify UnSlice Contour Level
/// 
/// @param level 
/// 
/// @return 
/// 

void QVTKUnClip::ModifyUnSliceContourLevel(int level)
{      
	if (level < 2) 
	{
		ErrorInfo(0, "Contour Level must be within range: 2 to 50!, Using the Min Level 2 insteading!");

		level = 2;
	}
	if(level>MAXLEVEL)
	{
		ErrorInfo(0, "Contour Level must be within range: 2 to 50!, Using the Max Level 50 insteading!");

		level = MAXLEVEL;
	}

	conLevel_ = level; 

	if (sliceTable_[0] != NULL)
	{
		sliceTable_[0]->SetNumberOfColors(conLevel_);
	}

	if(bandedContourFilter_ != NULL)
	{
		bandedContourFilter_->GenerateValues(conLevel_, scalarRange_[0], scalarRange_[1]);
		bandedContourFilter_->ClippingOn();
	}

	if (contourLinesFilter_ != NULL)
	{
		contourLinesFilter_->GenerateValues(conLevel_, scalarRange_[0], scalarRange_[1]);
	}

	if (sliceTable_[1] != NULL)
	{
		sliceTable_[1]->SetNumberOfColors(conLevel_);
	}

	if (contourLabelFilter_!= NULL)
	{
		contourLabelFilter_->GenerateValues(conLevel_, scalarRange_[0], scalarRange_[1]);
	}

}
// --------------------------------------------------------------------------
/// Modify UnSlice Vector Position
/// 
/// @param lineWidth -- line width
/// 
/// @return 
/// 
void QVTKUnClip::ModifyUnSliceMeshLineWidth(double lineWidth)
{
	if (sliceActors_[5]!=NULL)
	{
		(sliceActors_[5]->GetProperty())->SetLineWidth(lineWidth);
	}
}
// --------------------------------------------------------------------------
/// Modify UnSlice Contou rLines Width
/// 
/// @param lineWidth
/// 
/// @return 
/// 
void QVTKUnClip::ModifyUnSliceContourLinesWidth(double lineWidth)
{
	if (sliceActors_[1]!=NULL)
	{
		(sliceActors_[1]->GetProperty())->SetLineWidth(lineWidth);
	}
}
// --------------------------------------------------------------------------
/// Get slice plane widget
/// 
/// @param 
/// 1
/// @return vtkImplicitPlaneWidget
vtkImplicitPlaneWidget* QVTKUnClip::GetSlicePlaneWidget()
{
	if (planeWidget_ != NULL)
	{
		return this->planeWidget_;
	}

	return NULL;
}

// --------------------------------------------------------------------------
/// Get slice plane
/// 
/// @param 
/// 
/// @return vtkPlane
/// 
vtkPlane* QVTKUnClip::GetSlicePlane()
{
	if(slicePlane_ != NULL)
	{
		return this->slicePlane_;
	}

	return NULL;
}
// --------------------------------------------------------------------------
/// Set Slice PlaneWidget Handle Size
/// 
/// @param size
/// 
/// @return 
void QVTKUnClip::ModifySlicePlaneWidgetHandleSize(double size)
{
	if (planeWidget_ != NULL)
	{
		planeWidget_->SetHandleSize(size);
		planeWidget_->UpdatePlacement();
	}
}
// --------------------------------------------------------------------------
/// Modify Display Scalar Range 
/// 
/// @param min -- min Scalar
/// @param max -- max Scalar
/// 
/// @return 
/// 
void QVTKUnClip::ModifyDisplayScalarRange(double min,double max)
{
	scalarRange_[0] = min;
	scalarRange_[1] = max;
	if (min == max) {
		return;
	}

	if(bandedContourFilter_ != NULL)
	{
		//cout<<"test here 4444"<<endl;

		bandedContourFilter_->GenerateValues(conLevel_, min, max);
		bandedContourFilter_->Modified();
	}


	for (int i= 0; i<6; i++)
	{
		if (i == 0 || i== 1 || i == 3)
		{

			if(sliceActors_[i] != NULL)
			{
				(sliceActors_[i]->GetMapper())->GetLookupTable()->SetRange(min, max);
				(sliceActors_[i]->GetMapper())->GetLookupTable()->Modified();

				(sliceActors_[i]->GetMapper())->SetScalarRange(min, max);				
				(sliceActors_[i]->GetMapper())->Modified();

				sliceActors_[i]->Modified();
				//sliceActors_[i]->GetProperty()->SetRepresentationToSurface();
			}
		}
	}
}
// --------------------------------------------------------------------------
/// Clipping On Slice Contour Display
/// 
/// @param 
/// 
/// @return 
/// 
void QVTKUnClip::ClippingOnSliceContourDisplay()
{

	if(bandedContourFilter_ != NULL)
	{
		bandedContourFilter_->GenerateValues(conLevel_, scalarRange_[0], scalarRange_[1] );
		bandedContourFilter_->ClippingOn();
		bandedContourFilter_->Modified();
	}


	for (int i= 0; i<6; i++)
	{
		if (i == 0 || i== 1 || i == 3)
		{			
			if(sliceActors_[i] != NULL)
			{
				(sliceActors_[i]->GetMapper())->GetLookupTable()->SetRange(scalarRange_[0], scalarRange_[1]);
				(sliceActors_[i]->GetMapper())->GetLookupTable()->Modified();

				(sliceActors_[i]->GetMapper())->SetScalarRange(scalarRange_[0], scalarRange_[1]);				
				(sliceActors_[i]->GetMapper())->Modified();

				sliceActors_[i]->Modified();
				//sliceActors_[i]->GetProperty()->SetRepresentationToSurface();
			}
		}
	}
}
// --------------------------------------------------------------------------
/// Clipping Off Slice Contour Display
/// 
/// @param
/// 
/// @return 
/// 
void QVTKUnClip::ClippingOffSliceContourDisplay()
{

	if(bandedContourFilter_ != NULL)
	{
		bandedContourFilter_->GenerateValues(conLevel_, scalarRange_[0], scalarRange_[1] );
		bandedContourFilter_->ClippingOff();
		bandedContourFilter_->Modified();
	}

	for (int i= 0; i<6; i++)
	{
		if (i == 0 || i== 1 || i == 3)
		{			
			if(sliceActors_[i] != NULL)
			{
				(sliceActors_[i]->GetMapper())->GetLookupTable()->SetRange(scalarRange_[0], scalarRange_[1]);
				(sliceActors_[i]->GetMapper())->GetLookupTable()->Modified();

				(sliceActors_[i]->GetMapper())->SetScalarRange(scalarRange_[0], scalarRange_[1]);				
				(sliceActors_[i]->GetMapper())->Modified();

				sliceActors_[i]->Modified();
			}
		}
	}
}

//////////////////////////////////////////////////////
//#include "vtkIdFilter.h"
//#include "vtkCellCenters.h"
//#include "vtkVertexGlyphFilter.h"
void QVTKUnClip::SetSliceSource_FD(char* scalar, char* vector, bool bInsideOut)
{
	int index1 = -1;
	int index2 = -1;

	if(scalar != NULL)
	{
		index1 = _source->GetScalarIndex(scalar);
	}
	if(vector != NULL)
	{
		index2 = _source->GetVectorIndex(vector);
	}

	vtkUnstructuredGrid *unGrid = vtkUnstructuredGrid::New();
	//vtkPolyData *unGrid = vtkPolyData::New();
	unGrid->DeepCopy(_source->unstruGrid);

	if (index1 != -1)
	{
		usedScalarIndex_ = index1;

		scalarSource_ = vtkDoubleArray::New();
		if(_source->scalarSource[index1]->GetDataSize() != 0)
		{
			scalarSource_->DeepCopy(_source->scalarSource[index1]);
			(unGrid->GetPointData())->SetScalars(scalarSource_);
		}
		else
		{
			scalarSource_->DeepCopy(_source->cellScalarSource_[index1]);
			(unGrid->GetCellData())->SetScalars(scalarSource_);
		}

	}
	if (index2 != -1)
	{
		usedVectorIndex_ = index2;

		if(_source->vectorSource[index2]->GetDataSize() != 0)
		{
			(unGrid->GetPointData())->SetVectors(_source->vectorSource[index2]);
		}
		else 
		{
			(unGrid->GetCellData())->SetVectors(_source->cellVectorSource_[index2]);
		}
	}
	unGrid->Modified();

	vtkPlane *plane = vtkPlane::New();
	plane->SetNormal(planeNormal_[0], planeNormal_[1], planeNormal_[2]);

/*	vtkCutter *cutter = vtkCutter::New();
	cutter->SetInput(unGrid);
	cutter->SetCutFunction(plane);
	cutter->GenerateCutScalarsOff();
	cutter->SetSortByToSortByCell();
	cutter->Update();*/

	vtkDataSetSurfaceFilter *surf=vtkDataSetSurfaceFilter::New();
	surf->SetInput(unGrid);
	surf->Update();
	/*vtkPolyData *m_unGrid_=surf->GetOutput();
	vtkExtractEdges *surf=vtkExtractEdges::New();
	surf->SetInput(unGrid);
	surf->Update();
	vtkGeometryFilter *surf = vtkGeometryFilter::New();
	surf->SetInput(unGrid);
	surf->Update();
	vtkExtractEdges *surf=vtkExtractEdges::New();
	surf->SetInput(unGrid);
	surf->Update();*/
	/*vtkIdFilter *idFilter= vtkIdFilter::New();
	idFilter->SetInput(unGrid);
	idFilter->PointIdsOn();
	idFilter->FieldDataOn();
	idFilter->CellIdsOn();
	idFilter->Update();
	vtkCellCenters *surf = vtkCellCenters::New();
	 surf->SetInputConnection(idFilter->GetOutputPort());
	 surf->VertexCellsOn();
	 surf->Update();*/
	vtkPolyData *m_unGrid_=surf->GetOutput();
	
	//vtkClipPolyData *cutter=vtkClipPolyData::New();
	vtkClipDataSet*cutter=vtkClipDataSet::New();
	cutter->SetInput(unGrid);//(m_unGrid_);
	cutter->SetClipFunction(plane);//CutFunction(plane);
	cutter->GenerateClipScalarsOff();
	cutter->GenerateClippedOutputOn();
	//cutter->SetGenerateClippedOutput(false);//(true);////
	cutter->SetInsideOut(bInsideOut);
	//cutter->SetValue(0.0);
	//cutter->GenerateClipScalarsOff();//GenerateCutScalarsOff();
	//cutter->SetSortByToSortByCell();
	cutter->Update();

	//cout<<"test here 1111"<<(cutter->GetOutput())->GetNumberOfPoints()<<endl;

	gridSource_ = unGrid;
	//gridSource_ ->DeepCopy(unGrid);
	cutSource_ = cutter;
	slicePlane_ =  plane;    
}

void QVTKUnClip::CreateSliceWidget_FD()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}
	double* bounds;
	bounds = _source->GetSourceBounds();

	planeOrigin_[0] = (bounds[0]+bounds[1])/2;
	planeOrigin_[1] = (bounds[2]+bounds[3])/2;
	planeOrigin_[2] = (bounds[4]+bounds[5])/2;

	vtkImplicitPlaneWidget* planeWidget = vtkImplicitPlaneWidget::New();
	planeWidget->SetInteractor(_renWin->GetInteractor());
	planeWidget->SetPlaceFactor(1.0);

	planeWidget->SetHandleSize(0.005);
	planeWidget->SetDiagonalRatio(0.2);

	planeWidget->GetPlane(slicePlane_);      
	planeWidget->SetInput(cutSource_->GetOutput());

	planeWidget->TubingOff();
	//add begin
	planeWidget->GetEdgesProperty()->SetLineWidth(2.0);
	planeWidget->GetPlaneProperty()->SetColor(0.5, 0.5, 0.5);
	planeWidget->GetPlaneProperty()->SetOpacity(1);
	planeWidget->GetSelectedPlaneProperty()->SetOpacity(0.5);
	planeWidget->OutlineTranslationOff();

	planeWidget->PlaceWidget(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]);
	planeWidget->SetOrigin(planeOrigin_[0], planeOrigin_[1], planeOrigin_[2]);
	slicePlane_->SetOrigin(planeOrigin_[0], planeOrigin_[1], planeOrigin_[2]);
	vtkFloatArray *arry = GetCutScalarData();
	if (arry)  
		arry->GetRange(scalarRange_);
	//add end
	planeWidget->On();

	planeWidget_ = planeWidget;

	vtkCallbackCommand *cc = vtkCallbackCommand::New();
	cc->SetCallback(GenerateSliceProData_FD);
	cc->SetClientData(this);

	planeWidget->AddObserver(vtkCommand::InteractionEvent, cc);
}
//#include "vtkVISUnSlice.h"
void QVTKUnClip::GenerateSliceProData_FD(vtkObject *caller, unsigned long, void* obj, void *)
{
	QVTKUnClip* usObj = (QVTKUnClip*)obj;
	if (usObj->planeWidget_ == NULL)
	{
		usObj->ErrorInfo(1,"Plane Widget Not Created");
		return;
	}
	usObj->planeWidget_->GetPlane(usObj->slicePlane_);
	//add begin. when cut plane moving, update the cut plane's scalar range
	double r[2];r[0]=0;r[1]=0;
	vtkFloatArray *arry = usObj->GetCutScalarData();
	if (arry)  
		arry->GetRange(r);
	if (r[0]==0&&r[1]==0)
	{
		return;
	}
	usObj->ModifyDisplayScalarRange_FD(r[0], r[1]);
	//add end
}

void QVTKUnClip::CreateUnSliceContourDisplay_FD()
{    
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	double range[2];
	if (usedScalarIndex_ != -1)
	{     
		//range[0] = _source->scalarRange[usedScalarIndex_][0];
		//range[1]  = _source->scalarRange[usedScalarIndex_][1];
		range[0]  = scalarRange_[0];   //use current cut plane's scalar range,
		range[1]  = scalarRange_[1];   //instead of the whole input's scalar range.
	}
	else 
	{
		range[0] = 0;
		range[1] = 0;
	}
	cutSource_->Update();

	vtkGeometryFilter *geoFilter = vtkGeometryFilter::New();
	geoFilter->SetInput(cutSource_->GetOutput());
	geoFilter->Update();

	vtkBandedPolyDataContourFilter *conFilter = vtkBandedPolyDataContourFilter::New();
	conFilter->GlobalWarningDisplayOff();
	conFilter->SetInput(geoFilter->GetOutput());
	conFilter->GenerateValues(conLevel_, range[0], range[1]);
	conFilter->ClippingOn();
	conFilter->Modified();
	conFilter->Update();

	vtkDataSetMapper *mapper = vtkDataSetMapper::New();
	mapper->InterpolateScalarsBeforeMappingOn();

	if(_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
	{
		mapper->SetInputConnection(conFilter->GetOutputPort());
	}
	else
	{
		mapper->SetInputConnection(geoFilter->GetOutputPort());
	}
	mapper->SetScalarRange(range[0],range[1]);

	if (usedScalarIndex_ != -1)
	{
		if (_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
		{
			mapper->SetScalarModeToUsePointData();
		}
		else
		{
			mapper->SetScalarModeToUseCellData();
		}
	}

	vtkLookupTable *table = (vtkLookupTable*)mapper->GetLookupTable(); 
	table->GetHueRange(range);
	table->SetHueRange(range[1],range[0]);
	table->SetNumberOfColors(conLevel_);

	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetRepresentationToSurface();

	if (_renderer != NULL) 
	{    
		_renderer->AddActor(actor);     
	}

	sliceActors_[0] = actor;

	sliceTable_[0] = table;

	bandedContourFilter_ = conFilter;
}

void QVTKUnClip::CreateUnSliceContourLinesDisplay_FD()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	double range[2];
	if (usedScalarIndex_ != -1)
	{     
		//range[0] = _source->scalarRange[usedScalarIndex_][0];
		//range[1] = _source->scalarRange[usedScalarIndex_][1];
		range[0]  = scalarRange_[0];   //use current cut plane's scalar range,
		range[1]  = scalarRange_[1];   //instead of the whole input's scalar range.
	}
	else 
	{
		range[0] = 0;
		range[1] = 0;
	}

	vtkContourFilter *conFilter = vtkContourFilter::New();
	conFilter->GlobalWarningDisplayOff();
	conFilter->SetInputConnection(cutSource_->GetOutputPort());
	conFilter->GenerateValues(conLevel_, range[0],range[1]);
	conFilter->Update();

	vtkDataSetMapper *mapper = vtkDataSetMapper::New();
	mapper->InterpolateScalarsBeforeMappingOn();
	mapper->SetInputConnection(conFilter->GetOutputPort());
	mapper->SetScalarRange(range[0],range[1]);

	if (usedScalarIndex_ != -1)
	{
		if (_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
		{
			mapper->SetScalarModeToUsePointData();
		}
		else
		{
			mapper->SetScalarModeToUseCellData();
		}
	}

	vtkLookupTable *table = (vtkLookupTable*)mapper->GetLookupTable(); 
	table->GetHueRange(range);
	table->SetHueRange(range[1],range[0]);
	table->SetNumberOfColors(conLevel_);

	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetRepresentationToSurface();

	if (_renderer != NULL) 
	{    
		_renderer->AddActor(actor);   
	}

	contourLinesFilter_ = conFilter;
	sliceActors_[1] = actor;
	sliceTable_[1] = table; 
}
void QVTKUnClip::ModifyDisplayScalarRange_FD(double min,double max)
{
	scalarRange_[0] = min;
	scalarRange_[1] = max;
	if (min == max) {
		return;
	}

	if(bandedContourFilter_ != NULL)
	{
		bandedContourFilter_->GenerateValues(conLevel_, min, max);
		bandedContourFilter_->Modified();
	}
	//add begin. update contour lines/labels values
	if (contourLinesFilter_ != NULL)
	{
		contourLinesFilter_->GenerateValues(conLevel_, min, max);
		contourLinesFilter_->Modified();
	}
	if (contourLabelFilter_!= NULL)
	{
		contourLabelFilter_->GenerateValues(conLevel_, min, max);
		contourLabelFilter_->Modified();
	}
	//add end

	for (int i= 0; i<6; i++)
	{
		if (i == 0 || i== 1 || i == 3)
		{

			if(sliceActors_[i] != NULL)
			{
				(sliceActors_[i]->GetMapper())->GetLookupTable()->SetRange(min, max);
				(sliceActors_[i]->GetMapper())->GetLookupTable()->Modified();

				(sliceActors_[i]->GetMapper())->SetScalarRange(min, max);				
				(sliceActors_[i]->GetMapper())->Modified();

				sliceActors_[i]->Modified();

			}
		}
	}
}

void QVTKUnClip::ModifySliceSourceScalar_FD(char* scalar)
{
	if (gridSource_ == NULL)
	{
		ErrorInfo(1,"Grid Source are not Setted!");
		return;     
	}  

	int index = -1;
	if(scalar != NULL)
	{
		index = _source->GetScalarIndex(scalar);
	}
	if (index == -1)
	{
		ErrorInfo(1,"Scalar data are not existed!");
		return;
	}
	if (scalarSource_ != 0)
	{
		scalarSource_->Delete();
		scalarSource_ = 0;
	}
	scalarSource_ = vtkDoubleArray::New();
	if (_source->scalarSource[index]->GetDataSize() != 0)
	{
		scalarSource_->DeepCopy(_source->scalarSource[index]);
		(gridSource_->GetPointData())->SetScalars(scalarSource_);
	}
	else 
	{
		scalarSource_->DeepCopy(_source->cellScalarSource_[index]);
		(gridSource_->GetCellData())->SetScalars(scalarSource_);
	}
	gridSource_->Modified();

	usedScalarIndex_ = index;

	vtkFloatArray *arry = GetCutScalarData();
	if (arry)  arry->GetRange(scalarRange_);
	//2011-05-10 zhuqin add begin.
	if(bandedContourFilter_ != NULL)
	{
		bandedContourFilter_->GenerateValues(conLevel_, scalarRange_[0],scalarRange_[1]);
		bandedContourFilter_->ClippingOn();
		bandedContourFilter_->Modified();
	}
	if (contourLinesFilter_ != NULL)
	{
		contourLinesFilter_->GenerateValues(conLevel_, scalarRange_[0],scalarRange_[1]);
		contourLinesFilter_->Modified();
	}
	if (contourLabelFilter_!= NULL)
	{
		contourLabelFilter_->GenerateValues(conLevel_, scalarRange_[0],scalarRange_[1]);
		contourLabelFilter_->Modified();
	}
	//2011-05-10 zhuqin add end.


	for (int i= 0; i<6; i++)
	{
		if (i == 0 || i== 1 || i == 3)
		{
			if(sliceActors_[i] != NULL)
			{
				(sliceActors_[i]->GetMapper())->GetLookupTable()->SetRange(scalarRange_[0],scalarRange_[1]);
				(sliceActors_[i]->GetMapper())->GetLookupTable()->Modified();

				(sliceActors_[i]->GetMapper())->SetScalarRange(scalarRange_[0],scalarRange_[1]);

				if (_source->scalarSource[usedScalarIndex_]->GetDataSize() != 0)
				{
					(sliceActors_[i]->GetMapper())->SetScalarModeToUsePointData();
				}
				else
				{
					(sliceActors_[i]->GetMapper())->SetScalarModeToUseCellData();
				}

				(sliceActors_[i]->GetMapper())->Modified();
				sliceActors_[i]->Modified();
			}
		}

	}

}
bool QVTKUnClip::GetSourceBounds_FD(double *bounds)
{
	if (cutSource_=NULL)
	{
		ErrorInfo(1,"Clip Source Not Set");
		return false;
	}
	double *range = _source->GetSourceBounds();
	for (int i=0;i<6;++i)
	{
		bounds[i]=range[i];
	}
	return true;
}