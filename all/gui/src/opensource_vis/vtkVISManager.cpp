// Open-source implementation of vtkVISManager
#include "vtkVISManager.h"
#include <vtkObjectFactory.h>
#include <string.h>

vtkStandardNewMacro(vtkVISManager);
vtkCxxRevisionMacro(vtkVISManager, "$Revision: 1.0 $");

vtkVISManager::vtkVISManager()
    : scalarNumber_(0), vectorNumber_(0), scalarName(0), vectorName(0)
    , scalarNumber(0), vectorNumber(0)
    , _renderer(0), _renWin(0)
{
    const char* palette[20] = {
        "0 0 1", "0 0.5 0", "0 1 1", "0 0.5 1", "1 0 0",
        "0.5 0 1", "1 1 0.5", "0.7 0.7 0.7", "0.5 0.2 0", "1 0.6 0",
        "0 0.4 0.2", "0.5 0 0.5", "0 0 0", "1 1 1", "0.2 0.8 0.2",
        "0.2 0.2 0.7", "0.8 0.2 0.2", "0.2 0.7 0.7", "0.7 0.2 0.7", "1 0.84 0"
    };
    for (int i = 0; i < 20; ++i) {
        colorPalette[i] = palette[i];
        vtkColorPalette[i][0] = 0; vtkColorPalette[i][1] = 0; vtkColorPalette[i][2] = 0;
    }
}

vtkVISManager::~vtkVISManager()
{
}

void vtkVISManager::SetDataType(char* type)
{
    _dataType = type ? type : "";
}

void vtkVISManager::SetRenderTo(vtkRenderer *ren, vtkRenderWindow *renWin)
{
    _renderer = ren;
    _renWin = renWin;
}

void vtkVISManager::Translate_TkColorCode_To_VTKColorCode_All()
{
}

int vtkVISManager::GetScalarNumber() { return scalarNumber_; }
int vtkVISManager::GetVectorNumber() { return vectorNumber_; }

int vtkVISManager::GetScalarIndex(const char* scalar)
{
    if (!scalar || !scalarName) return -1;
    for (int i = 0; i < scalarNumber_; ++i)
        if (scalarName[i] && strcmp(scalarName[i], scalar) == 0) return i;
    return -1;
}

int vtkVISManager::GetVectorIndex(const char* vector)
{
    if (!vector || !vectorName) return -1;
    for (int i = 0; i < vectorNumber_; ++i)
        if (vectorName[i] && strcmp(vectorName[i], vector) == 0) return i;
    return -1;
}

const char* vtkVISManager::GetScalarName(int i)
{
    if (i >= 0 && i < scalarNumber_ && scalarName) return scalarName[i];
    return 0;
}

const char* vtkVISManager::GetVectorName(int i)
{
    if (i >= 0 && i < vectorNumber_ && vectorName) return vectorName[i];
    return 0;
}

int vtkVISManager::GetScalarIndex(int scalarNum)
{
    if (scalarNum >= 0 && scalarNum < scalarNumber_) return scalarNum;
    return -1;
}

int vtkVISManager::GetVectorIndex(int vectorNum)
{
    if (vectorNum >= 0 && vectorNum < vectorNumber_) return vectorNum;
    return -1;
}

