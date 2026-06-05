#include "QMyVTK.h"
#include "vtkCommand.h"
//#include "./OCCView/QOCCView.h"
//#include "vtkTextProperty.h"
//#include "vtkTextSource.h"
#include "vtkTextActor.h"
#include "vtkLODActor.h"
#include "vtkObjectBase.h"
#include "vtkAxesActor.h"
#include "vtkCaptionActor2D.h"
#include "vtkTextProperty.h"
#include "vtkVectorText.h"
#include "vtkPolyDataMapper.h"


QMyVTK* QMyVTK::instance = 0;
QMap<int,QMyVTK*> QMyVTK::m_QVTKWidgetObjectMap;

QMyVTK::QMyVTK()
{
    m_VTKWidget = new QSafeVTKWidget();
    render_= vtkRenderer::New();
    m_axisRenderer=0;
    m_axisWidget=0;
    m_axisAssembly=0;
    m_axisActor=0;
    m_axisLabels[0]=0;
    m_axisLabels[1]=0;
    m_axisLabels[2]=0;
    BackColor();

    vtkRenderWindow *renWin = m_VTKWidget->GetRenderWindow();
    renWin->AddRenderer(render_);

}

QMyVTK::~QMyVTK()
{
    if (m_axisWidget) {
        m_axisWidget->SetEnabled(0);
        m_axisWidget->Delete();
        m_axisWidget = 0;
    }
    if (m_axisAssembly) {
        m_axisAssembly->Delete();
        m_axisAssembly = 0;
    }
    for (int i = 0; i < 3; i++) {
        if (m_axisLabels[i]) {
            m_axisLabels[i]->Delete();
            m_axisLabels[i] = 0;
        }
    }
    if (m_axisActor) {
        m_axisActor->Delete();
        m_axisActor = 0;
    }
    render_->Delete();
    instance = 0;
}
QMyVTK* QMyVTK::GetInstance(int id)
{
    if(m_QVTKWidgetObjectMap.find(id) != m_QVTKWidgetObjectMap.end())
    { //存在，则返回当前的

    }else{//不存在，则进行添加
        QMyVTK *tmpVTK = new QMyVTK;
        m_QVTKWidgetObjectMap.insert(id,tmpVTK);
    }
   
   return m_QVTKWidgetObjectMap.value(id);
}
QMyVTK* QMyVTK::GetInstance()
{
   
    if (instance == 0)
    {
        instance = new QMyVTK;
    }
  
    return instance;
}

void QMyVTK::clear()
{
    if (m_axisWidget) {
        m_axisWidget->SetEnabled(0);
        m_axisWidget->Delete();
        m_axisWidget = 0;
    }
    if (m_axisAssembly) {
        m_axisAssembly->Delete();
        m_axisAssembly = 0;
    }
    for (int i = 0; i < 3; i++) {
        if (m_axisLabels[i]) {
            m_axisLabels[i]->Delete();
            m_axisLabels[i] = 0;
        }
    }
    if (m_axisActor) {
        m_axisActor->Delete();
        m_axisActor = 0;
    }
    render_->Delete();
    instance = 0;
}
QVTKWidget* QMyVTK::GetVTKWidget()
{
    return m_VTKWidget;
}
//获取当前Render
vtkRenderer* QMyVTK::GetRenderer()
{
    return render_;
}


void QMyVTK::ChangeViewDirection(const QString &direction)
{
    vtkRenderer *renderer = GetRenderer();
    ModifyVTKRendererCamera(renderer, direction);
}
/*
调整显示界面视角
*/
void QMyVTK::ModifyVTKRendererCamera(vtkRenderer *renderer, const QString &action)
{
    vtkRenderWindow *renWin = renderer->GetRenderWindow();
    vtkCamera *camera = renderer->GetActiveCamera();
    if (action == "+Zoom")  camera->Zoom(1.1);
    else if (action == "-Zoom")  camera->Zoom(0.9);
    else if (action == "Reset")  renderer->ResetCamera();
    else if (action == "+X")
    {
        camera->SetPosition(1,0,0);
        camera->SetViewUp(0,0,1);
    }
    else if (action == "-X")
    {
        camera->SetPosition(-1,0,0);
        camera->SetViewUp(0,0,1);
    }
    else if (action == "+Y")
    {
        camera->SetPosition(0,1,0);
        camera->SetViewUp(1,0,0);
    }
    else if (action == "-Y")
    {
        camera->SetPosition(0,-1,0);
        camera->SetViewUp(1,0,0);
    }
    else if (action == "+Z")
    {
        camera->SetPosition(0,0,1);
        camera->SetViewUp(0,1,0);
    }
    else if (action == "-Z")
    {
        camera->SetPosition(0,0,-1);
        camera->SetViewUp(0,1,0);
    }
    else if (action == "Orthogonal")
    {
        camera->ParallelProjectionOn();
    }
    else if (action == "Perspective")
    {
        camera->ParallelProjectionOff();
    }

    //QRegExp re("^[x-z][+\\-]$");  //x+ x- y+ y- z+ z-
      QRegExp re("^[+\\-][X-Z]$");  //+X -X +Y -Y +Z -Z
    if (re.exactMatch(action))
    {
        camera->SetFocalPoint(0,0,0);
        renderer->ResetCamera();
    }
    renWin->Render();
}
/*
对显示界面进行缩放操作
*/
void QMyVTK::Zoom(double factor)
{
    vtkRenderer *renderer = GetRenderer();
    if (renderer == 0)  //occ view page
    {
        //OCCView_->Zoom(factor);
    }
    else
    {
        if (factor > 1.0)  ModifyVTKRendererCamera(renderer, "+Zoom");
        else  ModifyVTKRendererCamera(renderer, "-Zoom");
    }
}
/*
对显示界面恢复到默认显示大小
*/
void QMyVTK::Reset()
{
    vtkRenderer *renderer = GetRenderer();
    if (renderer == 0)  //occ view page
    {
        //OCCView_->FitAll();
    }
    else
    {
        ModifyVTKRendererCamera(renderer, "Reset");
    }
}
/*
view 
*/
void QMyVTK::ViewChange(int index)
{
    if (index == 1){//bg  
         Dialog_BG::GetInstance()->exec();
    }else if (index ==2){//axis
        Orient();
    }
}
/*
    marker开关
*/
void QMyVTK::Orient()
{
    vtkRenderWindow *renWin = m_VTKWidget ? m_VTKWidget->GetRenderWindow() : 0;
    vtkRenderWindowInteractor *interactor = renWin ? renWin->GetInteractor() : 0;
    if (!renWin || !interactor) return;

    if (m_axisWidget == 0) {
        m_axisAssembly = vtkPropAssembly::New();

        m_axisActor = vtkAxesActor::New();
        m_axisActor->SetShaftTypeToCylinder();
        m_axisActor->SetTipTypeToCone();
        m_axisActor->AxisLabelsOff();
        m_axisActor->SetTotalLength(1.0, 1.0, 1.0);
        m_axisActor->GetXAxisShaftProperty()->SetColor(1.0, 0.0, 0.0);
        m_axisActor->GetXAxisTipProperty()->SetColor(1.0, 0.0, 0.0);
        m_axisActor->GetYAxisShaftProperty()->SetColor(0.0, 0.65, 0.0);
        m_axisActor->GetYAxisTipProperty()->SetColor(0.0, 0.65, 0.0);
        m_axisActor->GetZAxisShaftProperty()->SetColor(0.0, 0.2, 1.0);
        m_axisActor->GetZAxisTipProperty()->SetColor(0.0, 0.2, 1.0);
        m_axisAssembly->AddPart(m_axisActor);

        const char *labels[3] = {"X", "Y", "Z"};
        double colors[3][3] = {{1.0, 0.0, 0.0}, {0.0, 0.65, 0.0}, {0.0, 0.2, 1.0}};
        double pos[3][3] = {{1.15, -0.06, 0.0}, {-0.06, 1.15, 0.0}, {-0.06, 0.0, 1.15}};
        for (int i = 0; i < 3; i++) {
            vtkVectorText *vt = vtkVectorText::New();
            vt->SetText(labels[i]);
            vtkPolyDataMapper *pm = vtkPolyDataMapper::New();
            pm->SetInputConnection(vt->GetOutputPort());
            m_axisLabels[i] = vtkActor::New();
            m_axisLabels[i]->SetMapper(pm);
            m_axisLabels[i]->SetScale(0.18, 0.18, 0.18);
            m_axisLabels[i]->SetPosition(pos[i]);
            m_axisLabels[i]->GetProperty()->SetColor(colors[i]);
            m_axisAssembly->AddPart(m_axisLabels[i]);
            pm->Delete();
            vt->Delete();
        }

        m_axisWidget = vtkOrientationMarkerWidget::New();
        m_axisWidget->SetOrientationMarker(m_axisAssembly);
        m_axisWidget->SetInteractor(interactor);
        m_axisWidget->SetViewport(0.02, 0.02, 0.16, 0.16);
        m_axisWidget->SetOutlineColor(0.6, 0.6, 0.6);
        m_axisWidget->SetEnabled(1);
        m_axisWidget->InteractiveOff();
    } else {
        m_axisWidget->SetEnabled(!m_axisWidget->GetEnabled());
    }

    renWin->Render();
}
/*
    backgroundColor
*/
void QMyVTK::BackColor()
{
    double rgb[3] = {1.0, 1.0, 1.0};
    render_->SetGradientBackground(0);
    render_->SetBackground(rgb);
}
//----------------------------------------
//----------------------------------------
//              背景颜色
//----------------------------------------
//----------------------------------------
Dialog_BG* Dialog_BG::instance = 0;
Dialog_BG* Dialog_BG::GetInstance()
{
    if (instance == 0)
    {
        instance = new Dialog_BG;
    }
    return instance;
}

Dialog_BG::Dialog_BG(QWidget *parent) : QDialog(parent)
{
   
    
    this->setWindowTitle(tr("背景颜色"));
    this->setWindowIcon(QIcon(":/icon/appIcon.png"));
   
    QLabel *label = new QLabel(tr("颜色1"));
    QLabel *label_2 = new QLabel(tr("颜色2"));;
    QGroupBox *gbBox = new QGroupBox(tr("背景模式"),this);
    radioButton_singleColor=new QRadioButton(gbBox);
    radioButton_singleColor->setText("单颜色");
    radioButton_gradientColor=new QRadioButton(gbBox);
    radioButton_gradientColor->setText("渐变色");
    QHBoxLayout *Hlayout = new QHBoxLayout(gbBox);
    Hlayout->addWidget(radioButton_singleColor);
    Hlayout->addWidget(radioButton_gradientColor);

    pushButton_color1=new QPushButton(this);
    pushButton_color2=new QPushButton(this);
    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    Hlayout1->addWidget(label);
    Hlayout1->addWidget(pushButton_color1);

    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    Hlayout2->addWidget(label_2);
    Hlayout2->addWidget(pushButton_color2);

  /*  buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Apply|QDialogButtonBox::Close);
    buttonBox->setCenterButtons(true);*/
    OKBtn=new QPushButton(tr("确定"),this);
    ApplyBtn=new QPushButton(tr("应用"),this);
    CloseBtn=new QPushButton(tr("退出"),this);
    QHBoxLayout *HlayoutLast = new QHBoxLayout();
    HlayoutLast->addStretch();
    HlayoutLast->addWidget(OKBtn);
    HlayoutLast->addWidget(ApplyBtn);
    HlayoutLast->addWidget(CloseBtn);
    
    pushButton_color2->setEnabled(false);
    QVBoxLayout *mainlayout=new QVBoxLayout(this);
    mainlayout->addWidget(gbBox);
    mainlayout->addLayout(Hlayout1);
    mainlayout->addLayout(Hlayout2);
    mainlayout->addLayout(HlayoutLast);
    //mainlayout->addWidget(gbBox);
    setLayout(mainlayout);

    connect(radioButton_singleColor, SIGNAL(clicked()), this, SLOT(BGModeChanged()));
    connect(radioButton_gradientColor, SIGNAL(clicked()), this, SLOT(BGModeChanged()));
    connect(pushButton_color1, SIGNAL(clicked()), this, SLOT(ChooseColor()));
    connect(pushButton_color2, SIGNAL(clicked()), this, SLOT(ChooseColor()));
    //connect(this, SIGNAL(accepted()), this, SLOT(UpdateBG()));
    connect(ApplyBtn, SIGNAL(clicked()), this, SLOT(UpdateBG()));
    connect(CloseBtn, SIGNAL(clicked()), this, SLOT(reject()));
    connect(OKBtn, SIGNAL(clicked()), this, SLOT(OKSlot()));
}
Dialog_BG::~Dialog_BG()
{

}
void Dialog_BG::BGModeChanged()
{
    if (radioButton_singleColor->isChecked()){ 
        pushButton_color2->setEnabled(false);
    }else{
        pushButton_color2->setEnabled(true);
    }
}

void Dialog_BG::ChooseColor()
{
    QPushButton *pb = qobject_cast<QPushButton*>(sender());
    if (pb == 0)  return;
    QColor color = QColorDialog::getColor(QColor(Qt::white), this);
    if (!color.isValid())  return;
    pb->setStyleSheet(QString("background:rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
    if (pb == pushButton_color1){
        color1_ = color;
    }else if (pb == pushButton_color2){
        color2_ = color;
    }
}
void Dialog_BG::OKSlot()
{
    UpdateBG();
    this->accept();
}
void Dialog_BG::UpdateBG()
{
    QMap<int,QMyVTK*> tmpMap;
    tmpMap = QMyVTK::GetInstance()->m_QVTKWidgetObjectMap;
    QMapIterator<int,QMyVTK*> it(tmpMap);

    while (it.hasNext()){
        it.next();
        vtkRenderer *renderer = it.value()->GetRenderer();

        if (color1_.isValid() == false)  return;
        double rgb1[3] = {color1_.red()/255.0, color1_.green()/255.0, color1_.blue()/255.0};

        if (radioButton_singleColor->isChecked()){
            ModifyBackground(renderer, false, rgb1, 0);
        }else{
            if (color2_.isValid() == false)  return;
            double rgb2[3] = {color2_.red()/255.0, color2_.green()/255.0, color2_.blue()/255.0};
            ModifyBackground(renderer, true, rgb1, rgb2);
        }
    }

}
void Dialog_BG::ModifyBackground(vtkRenderer *renderer, bool isGradient, double *rgb1, double *rgb2)
{

        vtkRenderWindow *renWin = renderer->GetRenderWindow();
        renderer->SetGradientBackground(isGradient);
        renderer->SetBackground(rgb1);
        if (isGradient)  renderer->SetBackground2(rgb2);
        renWin->Render();
   
}

