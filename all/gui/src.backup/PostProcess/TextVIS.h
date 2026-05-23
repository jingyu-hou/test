#ifndef TEXTVIS_H
#define TEXTVIS_H
#include "../ADefineQ.h"
class vtkRenderer;
class vtkRenderWindow;
class vtkTextMapper;
class vtkActor2D;


class TextVIS
{
public:
    TextVIS();
    ~TextVIS();
    void InitRenderer(vtkRenderer *renderer);
    void SetVisible(bool visible);
    void SetText(const char *text);
    void AppendText(const char *text);
    void SetColor(double r, double g, double b);
    void SetFontSize(int size);
    void Update();
    void ShowText(TextStepIncTimeS showT,int);
private:
    vtkRenderer *renderer_;
    vtkTextMapper *textMapper_;
    vtkActor2D *textAct_;
    vtkRenderWindow *renWin_;
};
#endif

