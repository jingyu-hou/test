// Open-source stub for vtkVISUnGlyph (not directly used by consumers)
#ifndef VTK_VIS_UN_GLYPH_OS_H
#define VTK_VIS_UN_GLYPH_OS_H

#include "Win32Header.h"
#include "vtkVISUnstructuredGridManager.h"
#include <vtkGlyph3D.h>
#include <string>

class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnGlyph : public vtkVISUnstructuredGridManager
{
public:
    static vtkVISUnGlyph *New();
    vtkTypeRevisionMacro(vtkVISUnGlyph, vtkVISUnstructuredGridManager);
    vtkVISUnGlyph();
    ~vtkVISUnGlyph();

    void CreateGlyphDisplay(bool flag, char* scalarName, char* vectorName);
    void ModifyGlyphScalar(char* scalarName);
    void ModifyGlyphVector(char* vectorName);
    void SetScalarVisibilityOn();
    void SetScalarVisibilityOff();
    void SetGlyphFilledOn();
    void SetGlyphFilledOff();
    void SetGlyphPosition(double p1, double p2, double p3);
    void SetGlyphScaleMode(char *scaleMode);
    void SetGlyphScaleFactor(double factor);
    void SetGlyphColorMode(char *colorMode);
    void SetGlyphVectorMode(char *vectorMode);
    void SetGlyphIndexMode(char *indexMode);
    void SetGlyph2DType(char *glyphType);
    void ChangeGlyphLineThickness(double value);
    double GetGlyphLineThickness();
    void SetGlyphClampingOn();
    void SetGlyphClampingOff();
    void SetGlyphOrientOn();
    void SetGlyphOrientOff();
    void SetGlyphScalingOn();
    void SetGlyphScalingOff();
    double GetScaleFactor(char* vectorName);
    void ModifyDisplayScalarRange(double minScalar, double maxScalar);

protected:
    bool vectorFlag_;
    double scaleFactor_;
    double glyphPos_[3];
    double lineThickness_;
    int glyphTypeIndex_;
    vtkGlyph3D* glyphVtkObjects_;
    std::string glyph2DType[13];
    std::string scaleMode[4];
    std::string colorMode[3];
    std::string indexMode[3];
    std::string vectorMode[3];

    int GetScaleModeIndex(std::string Mode);
    int GetColorModeIndex(std::string Mode);
    int GetVectorModeIndex(std::string Mode);
    int GetIndexModeIndex(std::string Mode);
    int GetGlyph2DTypeIndex(std::string type);
};

#endif
