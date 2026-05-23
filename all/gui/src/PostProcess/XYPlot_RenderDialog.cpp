#include <QIcon>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVTKWidget.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderLargeImage.h>
#include <vtkPNGWriter.h>
#include <QFileDialog>
#include "XYPlot_RenderDialog.h"

XYPlot_RenderDialog* XYPlot_RenderDialog::instance = 0;
XYPlot_RenderDialog* XYPlot_RenderDialog::GetInstance()
{
    if (instance == 0)
    {
        instance = new XYPlot_RenderDialog;
    }
    return instance;
}

XYPlot_RenderDialog::XYPlot_RenderDialog(QWidget *parent) : QDialog(parent)
{
    double rgb1[3] = {1.0, 1.0, 1.0};
    double rgb2[3] = {0.4, 0.74, 1.0};
    QVTKWidget *widget2DPlot = new QVTKWidget;
    vtkRenderWindow *renWin = widget2DPlot->GetRenderWindow();
    renderer2DPlot_ = vtkRenderer::New();
    renderer2DPlot_->SetGradientBackground(1);
    renderer2DPlot_->SetBackground(rgb1);
    renderer2DPlot_->SetBackground2(rgb2);
    renWin->AddRenderer(renderer2DPlot_);

    QPushButton *save = new QPushButton(tr("保存图像"), this);
    m_ExprotDataBtn = new QPushButton(tr("导出数据"),this);
    connect(save, SIGNAL(clicked()), this, SLOT(SavePicture()));
    save->setAutoDefault(false);
    QVBoxLayout *Vlayout=new QVBoxLayout();
    Vlayout->addWidget(widget2DPlot);
    QHBoxLayout *Hlayout=new QHBoxLayout();
    Hlayout->addStretch();
    Hlayout->addWidget(save);
    Hlayout->addWidget(m_ExprotDataBtn);

    this->setWindowIcon(QIcon(":/images/welcme.png"));
    this->setWindowTitle(tr("曲线"));
    QVBoxLayout *mainLayOut= new QVBoxLayout(this);
    mainLayOut->addLayout(Vlayout);
    mainLayOut->addLayout(Hlayout);
    mainLayOut->setMargin(0);
    
    this->setLayout(mainLayOut);
    this->resize(800, 500);
}
XYPlot_RenderDialog::~XYPlot_RenderDialog()
{
    renderer2DPlot_->Delete();
}

vtkRenderer* XYPlot_RenderDialog::GetRenderer()
{
    return renderer2DPlot_;
}

void XYPlot_RenderDialog::CleanRender()
{
    renderer2DPlot_->RemoveAllViewProps();
}

void XYPlot_RenderDialog::SavePicture()
{
    QString file = QFileDialog::getSaveFileName(this, tr("保存文件"), "", "PNG File(*.png)");
    if (file.isEmpty())  return;
    QString ext(".png");
    if (!file.endsWith(ext, Qt::CaseInsensitive)) file.append(ext);
    vtkRenderLargeImage *image = vtkRenderLargeImage::New();
    image->SetInput(renderer2DPlot_);
    image->SetMagnification(1);
    vtkPNGWriter *writer = vtkPNGWriter::New();
    writer->SetInput(image->GetOutput());
    writer->SetFileName(file.toAscii().data());
    writer->Write();   
    writer->Delete();
    image->Delete();
}

void XYPlot_RenderDialog::Show()
{
    this->show();
    this-> raise();//最上层
    this->activateWindow();//激活
}
