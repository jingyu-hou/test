#ifndef XYPLOT_RENDERDIALOG_H
#define XYPLOT_RENDERDIALOG_H
/**
* @file
*/

#include <QDialog>
class vtkRenderer;
class QComboBox;

class XYPlot_RenderDialog : public QDialog
{
    Q_OBJECT

public:
    static XYPlot_RenderDialog* GetInstance();
    ~XYPlot_RenderDialog();
    vtkRenderer* GetRenderer();
    void CleanRender();
    void Show();
    QPushButton *m_ExprotDataBtn;
private:
    static XYPlot_RenderDialog *instance;
    XYPlot_RenderDialog(QWidget *parent = 0);

private slots:
    void SavePicture();
private:
    vtkRenderer *renderer2DPlot_;
};
#endif
