// Open-source implementation of vtkVISUnContour
#include "vtkVISUnContour.h"
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <vtkGeometryFilter.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkRenderWindowInteractor.h>
#include <string.h>

vtkStandardNewMacro(vtkVISUnContour);
vtkCxxRevisionMacro(vtkVISUnContour, "$Revision: 1.0 $");

vtkVISUnContour::vtkVISUnContour()
    : contourLookupTable_(0), scalarBarWidget_(0), contourScalarIndex_(0), vectorIndex_(0)
    , contourLevel_(20), scaleFactor_(1.0), csdVectorWarp_(0), conFilter_(0)
    , lineContourFilter_(0), contourType_(0), deformation_(0), representFlag_(0)
    , barEnable_(true), _contourActor(0)
{
    scalarRange_[0] = 0;
    scalarRange_[1] = 0;
}

vtkVISUnContour::~vtkVISUnContour()
{
    DeleteObjects();
}

void vtkVISUnContour::CreateContourDisplay(char* scalarName)
{
    CreateContourDisplay(scalarName, (char*)0);
}

void vtkVISUnContour::CreateContourDisplay(char* scalarName, char* vectorName)
{
    if (!_source || !_source->unstruGrid) return;

    contourScalarIndex_ = _source->GetScalarIndex(scalarName);
    if (contourScalarIndex_ < 0) return;

    // Get scalar range
    if (_source->scalarRange && _source->scalarRange[contourScalarIndex_]) {
        scalarRange_[0] = _source->scalarRange[contourScalarIndex_][0];
        scalarRange_[1] = _source->scalarRange[contourScalarIndex_][1];
    }

    // Set scalar data on the grid before building the pipeline
    if (_source->scalarSource && _source->scalarSource[contourScalarIndex_]
        && _source->scalarSource[contourScalarIndex_]->GetDataSize() != 0)
        _source->unstruGrid->GetPointData()->SetScalars(_source->scalarSource[contourScalarIndex_]);
    else if (_source->cellScalarSource_ && _source->cellScalarSource_[contourScalarIndex_]
             && _source->cellScalarSource_[contourScalarIndex_]->GetDataSize() != 0)
        _source->unstruGrid->GetCellData()->SetScalars(_source->cellScalarSource_[contourScalarIndex_]);

    // Build geometry filter
    vtkGeometryFilter* geo = vtkGeometryFilter::New();
    geo->SetInput(_source->unstruGrid);

    // Set up scalar data
    int nObj = 1;
    vtkDataObject* filterInput = geo->GetOutput();

    // Warp vector if specified
    if (vectorName && deformation_ > 0) {
        vectorIndex_ = _source->GetVectorIndex(vectorName);
        if (vectorIndex_ >= 0) {
            csdVectorWarp_ = vtkWarpVector::New();
            csdVectorWarp_->SetInputConnection(geo->GetOutputPort());
            csdVectorWarp_->SetScaleFactor(scaleFactor_);
            nObj++;
        }
    }

    // Contour filter based on type
    vtkDataObject* contourOutput = 0;
    if (contourType_ == 0) {
        // Banded contour
        conFilter_ = vtkBandedPolyDataContourFilter::New();
        if (csdVectorWarp_)
            conFilter_->SetInputConnection(csdVectorWarp_->GetOutputPort());
        else
            conFilter_->SetInputConnection(geo->GetOutputPort());
        conFilter_->GenerateValues(contourLevel_, scalarRange_[0], scalarRange_[1]);
        conFilter_->ClippingOn();
        nObj++;
    } else if (contourType_ == 2) {
        // Line contour
        lineContourFilter_ = vtkContourFilter::New();
        if (csdVectorWarp_)
            lineContourFilter_->SetInputConnection(csdVectorWarp_->GetOutputPort());
        else
            lineContourFilter_->SetInputConnection(geo->GetOutputPort());
        lineContourFilter_->GenerateValues(contourLevel_, scalarRange_[0], scalarRange_[1]);
        nObj++;
    }

    // Lookup table
    contourLookupTable_ = vtkLookupTable::New();
    contourLookupTable_->SetHueRange(0.667, 0.0);
    contourLookupTable_->SetNumberOfColors(contourLevel_);
    contourLookupTable_->SetRange(scalarRange_[0], scalarRange_[1]);
    contourLookupTable_->Build();
    nObj++;

    // Mapper
    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetLookupTable(contourLookupTable_);
    mapper->SetScalarRange(scalarRange_[0], scalarRange_[1]);
    mapper->ScalarVisibilityOn();

    if (contourType_ == 0 && conFilter_)
        mapper->SetInputConnection(conFilter_->GetOutputPort());
    else if (contourType_ == 2 && lineContourFilter_)
        mapper->SetInputConnection(lineContourFilter_->GetOutputPort());
    else if (csdVectorWarp_)
        mapper->SetInputConnection(csdVectorWarp_->GetOutputPort());
    else
        mapper->SetInputConnection(geo->GetOutputPort());

    if (_source->scalarSource && _source->scalarSource[contourScalarIndex_]
        && _source->scalarSource[contourScalarIndex_]->GetDataSize() != 0)
        mapper->SetScalarModeToUsePointData();
    else
        mapper->SetScalarModeToUseCellData();
    nObj++;

    // Actor
    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);
    if (contourType_ == 2) {
        actor->GetProperty()->SetRepresentationToWireframe();
    } else {
        actor->GetProperty()->SetRepresentationToSurface();
    }

    if (_renderer) _renderer->AddActor(actor);
    if (_unActor) { _renderer->RemoveActor(_unActor); _unActor->Delete(); }
    _unActor = actor;
    _contourActor = actor;

    // Store VTK objects for cleanup
    _vtkObjectsNum = nObj;
    _vtkObjects = new vtkObject*[nObj];
    int idx = 0;
    _vtkObjects[idx++] = geo;
    if (csdVectorWarp_) _vtkObjects[idx++] = csdVectorWarp_;
    if (contourType_ == 0 && conFilter_) _vtkObjects[idx++] = conFilter_;
    if (contourType_ == 2 && lineContourFilter_) _vtkObjects[idx++] = lineContourFilter_;
    _vtkObjects[idx++] = contourLookupTable_;
    _vtkObjects[idx++] = mapper;
}

void vtkVISUnContour::ModifyContourDisplay(char* scalarName)
{
    if (!_source || !_unActor || !_source->unstruGrid) return;

    int newIdx = _source->GetScalarIndex(scalarName);
    if (newIdx < 0) return;
    contourScalarIndex_ = newIdx;

    // Update scalar data on the unstruGrid
    if (_source->scalarSource && _source->scalarSource[contourScalarIndex_]) {
        if (_source->scalarSource[contourScalarIndex_]->GetDataSize() != 0)
            _source->unstruGrid->GetPointData()->SetScalars(_source->scalarSource[contourScalarIndex_]);
        else if (_source->cellScalarSource_)
            _source->unstruGrid->GetCellData()->SetScalars(_source->cellScalarSource_[contourScalarIndex_]);
    }

    if (_source->scalarRange && _source->scalarRange[contourScalarIndex_]) {
        scalarRange_[0] = _source->scalarRange[contourScalarIndex_][0];
        scalarRange_[1] = _source->scalarRange[contourScalarIndex_][1];
    }

    if (conFilter_) {
        conFilter_->GenerateValues(contourLevel_, scalarRange_[0], scalarRange_[1]);
        conFilter_->Modified();
    }
    if (lineContourFilter_) {
        lineContourFilter_->GenerateValues(contourLevel_, scalarRange_[0], scalarRange_[1]);
        lineContourFilter_->Modified();
    }

    vtkMapper* m = _unActor->GetMapper();
    if (m) {
        m->SetScalarRange(scalarRange_[0], scalarRange_[1]);
        if (_source->scalarSource && _source->scalarSource[contourScalarIndex_]
            && _source->scalarSource[contourScalarIndex_]->GetDataSize() != 0)
            m->SetScalarModeToUsePointData();
        else
            m->SetScalarModeToUseCellData();
    }

    if (contourLookupTable_) {
        contourLookupTable_->SetRange(scalarRange_[0], scalarRange_[1]);
        contourLookupTable_->Modified();
    }

    if (scalarBarWidget_ && scalarBarWidget_->GetScalarBarActor()) {
        scalarBarWidget_->GetScalarBarActor()->GetLookupTable()->SetRange(scalarRange_[0], scalarRange_[1]);
    }
}

void vtkVISUnContour::CreateScalarBar()
{
    if (!scalarBarWidget_) {
        scalarBarWidget_ = vtkScalarBarWidget::New();
        scalarBarWidget_->SetInteractor(_renWin ? _renWin->GetInteractor() : 0);

        vtkScalarBarActor* bar = vtkScalarBarActor::New();
        if (contourLookupTable_)
            bar->SetLookupTable(contourLookupTable_);
        bar->SetNumberOfLabels(5);
        bar->GetLabelTextProperty()->SetColor(0, 0, 0);
        scalarBarWidget_->SetScalarBarActor(bar);
        bar->Delete();
    }
    if (barEnable_) scalarBarWidget_->EnabledOn();
}

void vtkVISUnContour::ShowOnScalarBar()
{
    barEnable_ = true;
    if (scalarBarWidget_) scalarBarWidget_->EnabledOn();
}

void vtkVISUnContour::ShowOffScalarBar()
{
    barEnable_ = false;
    if (scalarBarWidget_) scalarBarWidget_->EnabledOff();
}

void vtkVISUnContour::DeleteScalarBar()
{
    if (scalarBarWidget_) {
        scalarBarWidget_->EnabledOff();
        scalarBarWidget_->Delete();
        scalarBarWidget_ = 0;
    }
}

void vtkVISUnContour::ModifyScalarBarFont(char* font)
{
    if (scalarBarWidget_ && scalarBarWidget_->GetScalarBarActor()) {
        // VTK 5.4 uses SetFontFamily(int); map common font names to VTK constants
        int family = VTK_ARIAL;
        if (font) {
            string f(font);
            if (f == "Times") family = VTK_TIMES;
            else if (f == "Courier") family = VTK_COURIER;
        }
        scalarBarWidget_->GetScalarBarActor()->GetLabelTextProperty()->SetFontFamily(family);
    }
}

void vtkVISUnContour::ModifyScalarBarFontColor(double r, double g, double b)
{
    if (scalarBarWidget_ && scalarBarWidget_->GetScalarBarActor())
        scalarBarWidget_->GetScalarBarActor()->GetLabelTextProperty()->SetColor(r, g, b);
}

void vtkVISUnContour::ModifyScalarBarTitle(char* title)
{
    if (scalarBarWidget_ && scalarBarWidget_->GetScalarBarActor())
        scalarBarWidget_->GetScalarBarActor()->SetTitle(title);
}

void vtkVISUnContour::ModifyContourLevel(int level)
{
    if (level < 2) level = 2;
    contourLevel_ = level;

    if (contourLookupTable_) contourLookupTable_->SetNumberOfColors(level);

    if (conFilter_) {
        conFilter_->GenerateValues(level, scalarRange_[0], scalarRange_[1]);
        conFilter_->Modified();
    }
    if (lineContourFilter_) {
        lineContourFilter_->GenerateValues(level, scalarRange_[0], scalarRange_[1]);
        lineContourFilter_->Modified();
    }
}

void vtkVISUnContour::SetContourLevel(int level) { contourLevel_ = level; }

void vtkVISUnContour::SetScalarBarEnable(bool enable) { barEnable_ = enable; }

void vtkVISUnContour::SetContourType(int type)
{
    contourType_ = type; // 0=Banded, 1=Continuous, 2=Line
}

void vtkVISUnContour::ModifyContourDisplayType()
{
    // Re-create display with new contour type
    if (_source && _unActor && _source->unstruGrid) {
        // Remove existing pipeline
        if (_unActor) {
            vtkMapper* m = _unActor->GetMapper();
            if (_renderer) _renderer->RemoveActor(_unActor);
            _unActor->Delete();
            _unActor = 0;
            _contourActor = 0;
        }
        // Clean and rebuild
        if (_vtkObjects) {
            for (int i = 0; i < _vtkObjectsNum; ++i)
                if (_vtkObjects[i]) _vtkObjects[i]->Delete();
            delete[] _vtkObjects;
            _vtkObjects = 0;
            _vtkObjectsNum = 0;
        }
        conFilter_ = 0;
        lineContourFilter_ = 0;
        contourLookupTable_ = 0;

        // Rebuild pipeline (scalar data should still be set on the grid)
        // We re-create from scratch using current settings
        if (_source->scalarName && contourScalarIndex_ >= 0) {
            char* sname = const_cast<char*>(_source->scalarName[contourScalarIndex_]);
            CreateContourDisplay(sname);
        }
    }
}

void vtkVISUnContour::SetDeformation(int flag) { deformation_ = flag; }

void vtkVISUnContour::ModifyScaleFactor(double value)
{
    scaleFactor_ = value;
    if (csdVectorWarp_) csdVectorWarp_->SetScaleFactor(value);
}

void vtkVISUnContour::SetWarpScaleFactor(double value)
{
    scaleFactor_ = value;
}

double vtkVISUnContour::GetWarpScaleFactor(char* vectorName)
{
    return scaleFactor_;
}

void vtkVISUnContour::SetContourRepresentationFlag(int flag) { representFlag_ = flag; }

void vtkVISUnContour::ModifyContourRepresentation(int flag)
{
    representFlag_ = flag;
    if (_contourActor) {
        if (flag == 0) _contourActor->GetProperty()->SetRepresentationToSurface();
        else _contourActor->GetProperty()->SetRepresentationToWireframe();
    }
}

void vtkVISUnContour::ClippingOnContourDisplay()
{
    if (conFilter_) {
        conFilter_->ClippingOn();
        conFilter_->Modified();
    }
}

void vtkVISUnContour::ClippingOffContourDisplay()
{
    if (conFilter_) {
        conFilter_->ClippingOff();
        conFilter_->Modified();
    }
}

void vtkVISUnContour::ModifyDisplayScalarRange(double min, double max)
{
    if (min == max) return;
    scalarRange_[0] = min;
    scalarRange_[1] = max;

    if (conFilter_) {
        conFilter_->GenerateValues(contourLevel_, min, max);
        conFilter_->Modified();
    }
    if (lineContourFilter_) {
        lineContourFilter_->GenerateValues(contourLevel_, min, max);
        lineContourFilter_->Modified();
    }
    if (_unActor) {
        vtkMapper* m = _unActor->GetMapper();
        if (m) {
            m->SetScalarRange(min, max);
            if (m->GetLookupTable()) {
                m->GetLookupTable()->SetRange(min, max);
            }
        }
    }
}

void vtkVISUnContour::DeleteObjects()
{
    DeleteScalarBar();
    if (_contourActor) {
        if (_renderer) _renderer->RemoveActor(_contourActor);
        _contourActor->Delete();
        _contourActor = 0;
    }
    if (_unActor) {
        if (_renderer) _renderer->RemoveActor(_unActor);
        _unActor->Delete();
        _unActor = 0;
    }
    if (_vtkObjects) {
        for (int i = 0; i < _vtkObjectsNum; ++i)
            if (_vtkObjects[i]) _vtkObjects[i]->Delete();
        delete[] _vtkObjects;
        _vtkObjects = 0;
        _vtkObjectsNum = 0;
    }
}
