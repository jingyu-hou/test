// Open-source replacement for vtkVISManager — base class for all VIS managers
#ifndef VTK_VIS_MANAGER_OS_H
#define VTK_VIS_MANAGER_OS_H
#include <iostream>
#include <string>

using namespace std;

#define MAXLEVEL 50

#include "Win32Header.h"
#include <vtkObject.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkDoubleArray.h>
#include <vtkObjectFactory.h>
#include <sstream>

class VTK_VIS_COMMON_EXPORT vtkVISManager : public vtkObject
{
public:
    static vtkVISManager *New();
    vtkTypeRevisionMacro(vtkVISManager, vtkObject);

    vtkVISManager();
    virtual ~vtkVISManager();

    void SetDataType(char* type);
    void SetRenderTo(vtkRenderer *ren, vtkRenderWindow *renWin);
    void Translate_TkColorCode_To_VTKColorCode_All();

    int scalarNumber_;
    int vectorNumber_;

    const char** scalarName;
    const char** vectorName;

    int GetScalarNumber();
    int GetVectorNumber();

    int GetScalarIndex(const char* scalar);
    int GetVectorIndex(const char* vector);

    const char* GetScalarName(int i);
    const char* GetVectorName(int i);

    int* scalarNumber;
    int* vectorNumber;

    int GetScalarIndex(int scalarNum);
    int GetVectorIndex(int vectorNum);

    void ErrorInfo(bool error, string message);

protected:
    string _dataType;
    string colorPalette[20];
    double vtkColorPalette[20][3];
    vtkRenderer *_renderer;
    vtkRenderWindow *_renWin;
};

inline void vtkVISManager::ErrorInfo(bool error, string message)
{
    if (error)
        cerr<<"VISError: "<<this->GetClassName()<<": "<<message<<endl;
    else
        cout<<"VISInfo:  "<<this->GetClassName()<<": "<<message<<endl;
}

#endif
