/*============================================================================
     AESim_FM - Unstructured Grid Clip/Slice Manager for VTK Pipeline

     Convenience wrapper around standard VTK classes (vtkPlane, vtkClipDataSet,
     vtkImplicitPlaneWidget, vtkContourFilter).  Provides cut-plane creation,
     contour/shade/mesh/vector display, and scalar/vector data extraction.

     Copyright: Shenzhen Wedge Central South Research Institute co., Ltd.

     Original author: HUANG Jiaqi, 2010-06-01
     Modifications: see Git history
============================================================================*/
#include "AesVtkUnClip.h"
#include "vtkCellArray.h"
#include "vtkSortDataArray.h"
#include "vtkDataSetSurfaceFilter.h"
#include <algorithm>

vtkStandardNewMacro(AesVtkUnClip);

AesVtkUnClip::AesVtkUnClip()
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

	slicePlane_ = NULL;
	planeWidget_ = NULL;

	scalarRange_[0] = 0;
	scalarRange_[1] = 0;

	bandedContourFilter_ = 0;
	contourLinesFilter_ = 0;
	contourLabelFilter_ = 0;
	scalarSource_ = 0;
}
AesVtkUnClip::~AesVtkUnClip()
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
void AesVtkUnClip::WriteScalarVectorData(const char* FileName)
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
vtkFloatArray* AesVtkUnClip::GetCutScalarData(void)
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
vtkFloatArray* AesVtkUnClip::GetCutPointData(int index)
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
void AesVtkUnClip::ModifySliceSourceScalar(char* scalar)
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
void AesVtkUnClip::ModifySliceSourceVector(char* vector)
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
void AesVtkUnClip::CreateSliceWidget()
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
void AesVtkUnClip::GenerateSliceProData(vtkObject *caller, unsigned long, void* obj, void *)
{
	AesVtkUnClip* usObj = (AesVtkUnClip*)obj;

	if (usObj->planeWidget_ == NULL)
	{
		usObj->ErrorInfo(1,"Plane Widget Not Created");
		return;
	}
	usObj->planeWidget_->GetPlane(usObj->slicePlane_);
}
void AesVtkUnClip::ShowOnSlicePlane()
{
	if (planeWidget_ != NULL)
	{
		planeWidget_->DrawPlaneOn();
		planeWidget_->UpdatePlacement();
	}

}
void AesVtkUnClip::ShowOffSlicePlane()
{
	if (planeWidget_ != NULL)
	{
		planeWidget_->DrawPlaneOff();
		planeWidget_->UpdatePlacement();
	}

}
void AesVtkUnClip::ShowOnSlicePlaneWidget()
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

void AesVtkUnClip::ShowOffSlicePlaneWidget()
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
void AesVtkUnClip::SetSlicePlaneNormal(double nx, double ny, double nz)
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
void AesVtkUnClip::SetSlicePlaneNormalToX()
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
void AesVtkUnClip::SetSlicePlaneNormalToY()
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
void AesVtkUnClip::SetSlicePlaneNormalToZ()
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

double* AesVtkUnClip::GetSlicePlaneNormal()
{
	return this->planeNormal_;
}
void AesVtkUnClip::SetSlicePlaneOrigin(double ox, double oy, double oz)
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
double* AesVtkUnClip::GetSlicePlaneOrigin()
{
	return this->planeOrigin_;
}
void AesVtkUnClip::CreateUnSliceContourDisplay()
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

	vtkGeometryFilter *geoFilter = vtkGeometryFilter::New();
	geoFilter->SetInput(cutSource_->GetOutput());
	geoFilter->Update();

	vtkBandedPolyDataContourFilter *conFilter = vtkBandedPolyDataContourFilter::New();
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

	if (_renderer != NULL)
	{
		_renderer->AddActor(actor);
	}
	sliceActors_[0] = actor;

	sliceTable_[0] = table;

	bandedContourFilter_ = conFilter;
}
void AesVtkUnClip::ShowOnUnSliceContourDisplay()
{
	if (sliceActors_[0] != NULL)
	{
		sliceActors_[0]->VisibilityOn();
	}
}
void AesVtkUnClip::ShowOffUnSliceContourDisplay()
{
	if (sliceActors_[0] != NULL)
	{

		sliceActors_[0]->VisibilityOff();

	}
}
void AesVtkUnClip::DeleteUnSliceContourDisplay()
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
void AesVtkUnClip::CreateUnSliceContourLinesDisplay()
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
	sliceActors_[1] = actor;
	sliceTable_[1] = table;
}
void AesVtkUnClip::ShowOnUnSliceContourLinesDisplay()
{
	if(sliceActors_[1] != NULL)
	{
		sliceActors_[1]->VisibilityOn();
	}
}
void AesVtkUnClip::ShowOffUnSliceContourLinesDisplay()
{
	if(sliceActors_[1] != NULL)
	{
		sliceActors_[1]->VisibilityOff();
	}
}
void AesVtkUnClip::DeleteUnSliceCOntourLinesDisplay()
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
void AesVtkUnClip::CreateUnSliceContourLinesLabelDisplay()
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
	sliceTable_[2] = NULL;

}
void AesVtkUnClip::ShowOnUnSliceContourLinesLabelDisplay()
{
	if(sliceActors_[2] != NULL)
	{
		sliceActors_[2]->VisibilityOn();
	}
}
void AesVtkUnClip::ShowOffUnSliceContourLinesLabelDisplay()
{
	if(sliceActors_[2] != NULL)
	{
		sliceActors_[2]->VisibilityOff();
	}
}
void AesVtkUnClip::DeleteUnSliceContourLinesLabelDisplay()
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

void AesVtkUnClip::CreateUnSliceVectorDisplay()
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
	sliceActors_[3] = actor;
	sliceTable_[3] = table;
	sliceGlyph2D_ = glyph2D;
	sliceGlyph3D_ = glyph3D;

}
void AesVtkUnClip::ShowOnUnSliceVectorDisplay()
{
	if(sliceActors_[3] != NULL)
	{
		sliceActors_[3]->VisibilityOn();
	}
}
void AesVtkUnClip::ShowOffUnSliceVectorDisplay()
{
	if(sliceActors_[3] != NULL)
	{
		sliceActors_[3]->VisibilityOff();
	}
}
void AesVtkUnClip::DeleteUnSliceVectorDisplay()
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
void AesVtkUnClip::CreateUnSliceShadeDisplay()
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
void AesVtkUnClip::ModifyUnSliceShadeColor(double c0, double c1, double c2)
{
	if (sliceActors_[4]!=NULL)
	{
		shadeColor_[0] = c0;
		shadeColor_[1] = c1;
		shadeColor_[2] = c2;

		(sliceActors_[4]->GetProperty())->SetColor(c0, c1, c2);
	}
}
void AesVtkUnClip::ShowOnUnSliceShade()
{
	if(sliceActors_[4] != NULL)
	{
		sliceActors_[4]->VisibilityOn();
	}
}
void AesVtkUnClip::ShowOffUnSliceShade()
{
	if(sliceActors_[4] != NULL)
	{
		sliceActors_[4]->VisibilityOff();
	}
}
void AesVtkUnClip::CreateUnSliceMeshDisplay()
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
void AesVtkUnClip::ModifyUnSliceMeshColor(double c0, double c1, double c2)
{
	if (sliceActors_[5]!=NULL)
	{
		shadeColor_[0] = c0;
		shadeColor_[1] = c1;
		shadeColor_[2] = c2;

		(sliceActors_[5]->GetProperty())->SetColor(c0, c1, c2);
	}
}
void AesVtkUnClip::ShowOnUnSliceMesh()
{
	if(sliceActors_[5] != NULL)
	{
		sliceActors_[5]->VisibilityOn();
	}
}
void AesVtkUnClip::ShowOffUnSliceMesh()
{
	if(sliceActors_[5] != NULL)
	{
		sliceActors_[5]->VisibilityOff();
	}
}
void AesVtkUnClip::SetUnSliceContourOpacity(double value)
{
	if (sliceActors_[0] != NULL)
	{
		(sliceActors_[0]->GetProperty())->SetOpacity(value);
	}
}
double AesVtkUnClip::GetUnSliceContourOpacity()
{
	if (sliceActors_[0] != NULL)
	{
		return (sliceActors_[0]->GetProperty())->GetOpacity();
	}
	return NULL;
}
void AesVtkUnClip::ModifyUnSliceVectorColorMode(char* mode)
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
void AesVtkUnClip::ModifyUnSliceVectorScaleMode(char* mode)
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
void AesVtkUnClip::ModifyUnSliceVectorScaleFactor(double scaleFactor)
{

	if (sliceGlyph3D_ != NULL)
	{
		sliceGlyph3D_->SetScaleFactor(scaleFactor);
	}
}
void AesVtkUnClip::SetUnSliceVectorFilledOn()
{
	if (sliceGlyph2D_ != NULL)
	{
		sliceGlyph2D_->FilledOn();
	}
}
void AesVtkUnClip::SetUnSliceVectorFilledOff()
{
	if (sliceGlyph2D_ != NULL)
	{
		sliceGlyph2D_->FilledOff();
	}
}
void AesVtkUnClip::ModifyUnSliceVectorPosition(double p0, double p1, double p2)
{

	if (sliceGlyph2D_ != NULL)
	{
		sliceGlyph2D_->SetCenter(p0, p1, p2);
	}
}
void AesVtkUnClip::ModifyUnSliceShadeOpacity(double value)
{
	if (sliceActors_[4] != NULL)
	{
		(sliceActors_[4]->GetProperty())->SetOpacity(value);
	}
}

void AesVtkUnClip::ModifyUnSliceContourLevel(int level)
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
void AesVtkUnClip::ModifyUnSliceMeshLineWidth(double lineWidth)
{
	if (sliceActors_[5]!=NULL)
	{
		(sliceActors_[5]->GetProperty())->SetLineWidth(lineWidth);
	}
}
void AesVtkUnClip::ModifyUnSliceContourLinesWidth(double lineWidth)
{
	if (sliceActors_[1]!=NULL)
	{
		(sliceActors_[1]->GetProperty())->SetLineWidth(lineWidth);
	}
}
vtkImplicitPlaneWidget* AesVtkUnClip::GetSlicePlaneWidget()
{
	if (planeWidget_ != NULL)
	{
		return this->planeWidget_;
	}

	return NULL;
}

vtkPlane* AesVtkUnClip::GetSlicePlane()
{
	if(slicePlane_ != NULL)
	{
		return this->slicePlane_;
	}

	return NULL;
}
void AesVtkUnClip::ModifySlicePlaneWidgetHandleSize(double size)
{
	if (planeWidget_ != NULL)
	{
		planeWidget_->SetHandleSize(size);
		planeWidget_->UpdatePlacement();
	}
}
void AesVtkUnClip::ModifyDisplayScalarRange(double min,double max)
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
void AesVtkUnClip::ClippingOnSliceContourDisplay()
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
			}
		}
	}
}
void AesVtkUnClip::ClippingOffSliceContourDisplay()
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

void AesVtkUnClip::SetSliceSource_FD(char* scalar, char* vector, bool bInsideOut)
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

	vtkDataSetSurfaceFilter *surf=vtkDataSetSurfaceFilter::New();
	surf->SetInput(unGrid);
	surf->Update();
	vtkPolyData *m_unGrid_=surf->GetOutput();

	vtkClipDataSet*cutter=vtkClipDataSet::New();
	cutter->SetInput(unGrid);
	cutter->SetClipFunction(plane);
	cutter->GenerateClipScalarsOff();
	cutter->GenerateClippedOutputOn();
	cutter->SetInsideOut(bInsideOut);
	cutter->Update();

	gridSource_ = unGrid;
	cutSource_ = cutter;
	slicePlane_ =  plane;
}

void AesVtkUnClip::CreateSliceWidget_FD()
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
	planeWidget->On();

	planeWidget_ = planeWidget;

	vtkCallbackCommand *cc = vtkCallbackCommand::New();
	cc->SetCallback(GenerateSliceProData_FD);
	cc->SetClientData(this);

	planeWidget->AddObserver(vtkCommand::InteractionEvent, cc);
}
void AesVtkUnClip::GenerateSliceProData_FD(vtkObject *caller, unsigned long, void* obj, void *)
{
	AesVtkUnClip* usObj = (AesVtkUnClip*)obj;
	if (usObj->planeWidget_ == NULL)
	{
		usObj->ErrorInfo(1,"Plane Widget Not Created");
		return;
	}
	usObj->planeWidget_->GetPlane(usObj->slicePlane_);
	double r[2];r[0]=0;r[1]=0;
	vtkFloatArray *arry = usObj->GetCutScalarData();
	if (arry)
		arry->GetRange(r);
	if (r[0]==0&&r[1]==0)
	{
		return;
	}
	usObj->ModifyDisplayScalarRange_FD(r[0], r[1]);
}

void AesVtkUnClip::CreateUnSliceContourDisplay_FD()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	double range[2];
	if (usedScalarIndex_ != -1)
	{
		range[0]  = scalarRange_[0];
		range[1]  = scalarRange_[1];
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

void AesVtkUnClip::CreateUnSliceContourLinesDisplay_FD()
{
	if (cutSource_ == NULL)
	{
		ErrorInfo(1,"Slice Source Not Set");
		return;
	}

	double range[2];
	if (usedScalarIndex_ != -1)
	{
		range[0]  = scalarRange_[0];
		range[1]  = scalarRange_[1];
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
void AesVtkUnClip::ModifyDisplayScalarRange_FD(double min,double max)
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

void AesVtkUnClip::ModifySliceSourceScalar_FD(char* scalar)
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
bool AesVtkUnClip::GetSourceBounds_FD(double *bounds)
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
