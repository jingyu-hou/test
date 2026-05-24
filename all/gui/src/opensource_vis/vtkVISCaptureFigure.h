// Open-source stub for vtkVISCaptureFigure (not directly used by consumers)
#ifndef VTK_VIS_CAPTURE_FIGURE_OS_H
#define VTK_VIS_CAPTURE_FIGURE_OS_H

#include "Win32Header.h"
#include "vtkVISManager.h"
#include <vtkRenderLargeImage.h>

class VTK_VIS_COMMON_EXPORT vtkVISCaptureFigure : public vtkVISManager
{
public:
    static vtkVISCaptureFigure *New();
    vtkTypeRevisionMacro(vtkVISCaptureFigure, vtkVISManager);
    vtkVISCaptureFigure();
    ~vtkVISCaptureFigure();

    void SetCaptureFigureSource(vtkRenderer *render);
    void ModifyMagnificationFactor(int factor);
    int GetMagnificationFactor();
    void CaptureRenderFigureAsJPEG(char* imageName);
    void CaptureRenderFigureAsBMP(char* imageName);
    void CaptureRenderFigureAsPNG(char* imageName);
    void CaptureRenderFigureAsTIFF(char* imageName);
    void CaptureRenderFigureAsPNM(char* imageName);

protected:
    vtkRenderLargeImage *renderImage_;
    int magFactor_;
};

#endif
