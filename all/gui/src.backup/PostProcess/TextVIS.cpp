#include <string>
using namespace std;
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkActor2D.h>
#include "TextVIS.h"


TextVIS::TextVIS() : renderer_(0)
{
    renWin_ = 0;
    textMapper_ = vtkTextMapper::New();
    textMapper_->SetInput("");
    vtkTextProperty *tprop = textMapper_->GetTextProperty();
    tprop->SetFontSize(22);
    tprop->SetFontFamilyToArial();
    tprop->BoldOff();
    tprop->ItalicOff();
    tprop->ShadowOff();
    tprop->SetLineSpacing(1.0);
    tprop->SetJustificationToLeft();
    tprop->SetVerticalJustificationToTop();
    tprop->SetColor(0.0, 0.0, 0.0);
    textAct_ = vtkActor2D::New();
    textAct_->SetMapper(textMapper_);
    textAct_->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
    textAct_->GetPositionCoordinate()->SetValue(0.1, 0.25);
}

TextVIS::~TextVIS()
{
    textMapper_->Delete();
    textAct_->Delete();
    
}

void TextVIS::InitRenderer(vtkRenderer *renderer)
{
    renderer_ = renderer;
    renderer_->AddActor2D(textAct_);
    renWin_ = renderer_->GetRenderWindow();
}

void TextVIS::SetVisible(bool visible)
{
    textAct_->SetVisibility(visible);
}

void TextVIS::SetText(const char *text)
{
    textMapper_->SetInput(text);
    textMapper_->Modified();
}

void TextVIS::AppendText(const char *text)
{
    string strText(textMapper_->GetInput());
    if (!strText.empty() && text[0] != '\n')  strText.append("    ");
    strText.append(text);
    textMapper_->SetInput(strText.c_str());
    textMapper_->Modified();
}

void TextVIS::SetColor(double r, double g, double b)
{
    textMapper_->GetTextProperty()->SetColor(r, g, b);
    textMapper_->Modified();
}

void TextVIS::SetFontSize(int size)
{
    textMapper_->GetTextProperty()->SetFontSize(size);
    textMapper_->Modified();
}
void TextVIS::Update()
{
    if (renWin_)  renWin_->Render();
}

//--数据处理与显示
void TextVIS::ShowText(TextStepIncTimeS showT,int iShowFlag)
{
   int tmpFlag=iShowFlag;
   QString str="";
   if ((tmpFlag & 0x01)== 1){
        str = "Step: "+showT.dataStep+"\n";
   }
   if ((tmpFlag & 0x02)==2){
        str += "Inc: "+showT.dataInc+"\n";
   } 
   if ((tmpFlag & 0x04)==4){
       str += "Time: "+showT.dataTime+"\n";
   }
   

    SetText(str.toAscii().data());
}