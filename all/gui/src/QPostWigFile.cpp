#include "QPostWigFile.h"
#include <QFileInfo>
#include "Information_Widget.h"

static QString SafeGetOpenFileName(QWidget *parent, const QString &caption, const QString &dir, const QString &filter)
{
    static QFileDialog *dlg = NULL;
    if (!dlg) {
        dlg = new QFileDialog(parent);
        dlg->setOption(QFileDialog::DontUseNativeDialog, true);
        dlg->setFileMode(QFileDialog::ExistingFile);
        dlg->setAcceptMode(QFileDialog::AcceptOpen);
    }
    dlg->setWindowTitle(caption);
    dlg->setDirectory(dir);
    dlg->setNameFilter(filter);
    dlg->selectFile(QString());
    if (dlg->exec() != QDialog::Accepted) {
        return QString();
    }
    QStringList files = dlg->selectedFiles();
    if (files.isEmpty()) {
        return QString();
    }
    return files.first();
}

 int QPostWigFile::m_WigFrdNumOld=0;
QPostWigFile::QPostWigFile(QWidget *parent, VTKColorS* mColor)
	: QWidget(parent)
{
    QGroupBox *gpBox1= new QGroupBox(this);
    m_LabName = new QLabel(tr("文件名"),gpBox1);
    m_ReadEdit= new QLineEdit(gpBox1);
    m_ReadBtn = new QPushButton(tr("导入"),gpBox1);
    QHBoxLayout *layoutH1 = new QHBoxLayout(gpBox1);
    layoutH1->addWidget(m_LabName);
    layoutH1->addWidget(m_ReadEdit);
    layoutH1->addWidget(m_ReadBtn);

    QGroupBox *gpBox2= new QGroupBox(tr("显示选择"), this);
    m_pickActorBtn = new QToolButton(gpBox2);
    m_pickActorBtn->setIcon(QIcon(":/images/arrow.png"));
    m_pickActorBtn->setText(tr("轮廓显示"));
    m_pickActorBtn->setCheckable(true);
    m_pickActorBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_OutPutBtn = new QPushButton(tr("轮廓数据输出"),gpBox2);
    QHBoxLayout *layoutH2 = new QHBoxLayout(gpBox2);
    layoutH2->addWidget(m_pickActorBtn);
    layoutH2->addWidget(m_OutPutBtn);
    
    QGroupBox *gbBox3=new QGroupBox(tr("隐藏/显示"),this) ;
    m_ListActorWiget = new QFrdActorWidge(this);
    QHBoxLayout *layoutH3=new QHBoxLayout(gbBox3);
    layoutH3->addWidget(m_ListActorWiget);

    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addWidget(gpBox1);
    MainLayout->addWidget(gpBox2);
    MainLayout->addWidget(gbBox3);
    MainLayout->addStretch();
    this->setLayout(MainLayout);
    connect(m_ReadBtn,SIGNAL(clicked()),this,SLOT(BtnOpenFrdSlot())); 
    connect(m_OutPutBtn,SIGNAL(clicked()),this,SLOT(BtnOutPutSlot()));
    connect(m_pickActorBtn, SIGNAL(toggled(bool)), this, SLOT(SwitchActorPickSlot(bool)));
    m_WigFrdNum=0;
    init(mColor);
}

QPostWigFile::~QPostWigFile()
{

}

void QPostWigFile::init(VTKColorS* clr)
{
    m_ClrInit.r = clr->r;
    m_ClrInit.g = clr->g;
    m_ClrInit.b = clr->b;
//     QString tmpStr;
//     str.split(",");
//     tmpStr=str.remove("rgba");
//     tmpStr=tmpStr.remove("(");
//     tmpStr=tmpStr.remove(")");
//     QStringList strList =tmpStr.split(",");
//     m_ClrInit.r = strList.at(0).toInt()/255.0;
//     m_ClrInit.g = strList.at(1).toInt()/255.0;
//     m_ClrInit.b = strList.at(2).toInt()/255.0; 
}

void QPostWigFile::PostFrdNew(int index)
{
  
    frdVIS_.InitRenderer(QMyVTK::GetInstance(index)->GetRenderer());
    frdTextVIS_.InitRenderer(QMyVTK::GetInstance(index)->GetRenderer());
    m_WigFrdNum = index;
}
void QPostWigFile::SwitchActorPickSlot(bool b)
{
    frdVIS_.ActorPick(b);
    frdVIS_.AcotrVisibility(b);
}
void QPostWigFile::BtnOpenFrdSlot()
{
    QString fileName = SafeGetOpenFileName(this, "Read Frd/Dat from file", "", "Frd Files (*.Frd *.frd);;Dat Files (*.Dat *.dat)");
    if (fileName.isEmpty()) return;
    m_ReadEdit->setText(fileName);
    QString suffix = QFileInfo(fileName).suffix().toLower();
    if (suffix == "frd") readOpenFrd(fileName);
    else if (suffix == "dat") readOpenDat(fileName);
}
bool QPostWigFile::readOpenFrd(QString fileName)
{
    QFrdDataPro	m_FrdDataPro;
    resultFrdS m_resultFrd;
    bool ret=m_FrdDataPro.ReadFileData(frdVIS_, fileName, m_resultFrd);
    if (!ret) {
        Information_Widget::GetInstance()->ShowInformation("FRD file read failed.");
        return false;
    }
    if (!frdVIS_.Step4_SetupFrd()) {
        Information_Widget::GetInstance()->ShowInformation("FRD data setup failed.");
        return false;
    }
    const map<QString, QStringList>* scaler=frdVIS_.GetScalarInfo();
    if (scaler!=NULL)
    {
        emit emitDataMenu(scaler);
        emit frdDataOk(&frdVIS_);
    }
    const vector<int>* gridIds = frdVIS_.GetGridIds();
    if (gridIds) {
        m_ListActorWiget->upDataListWidget(gridIds);
    }
    if (frdVIS_.GetBindedRenderer()) {
        frdVIS_.ResetCamera();
        frdVIS_.Update();
    }
    return true;
}

bool QPostWigFile::readOpenDat(QString filename)
{
    QFrdDataPro	m_FrdDataPro;
    QFile inFile(filename);
    if(!inFile.exists())
    {
        return false;
    }

    if (!inFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    bool ret=m_FrdDataPro.ReadFileDat(&inFile);
	inFile.close();
    if (!ret) return false;
	emit DatOk(m_FrdDataPro.m_resultDat);
    return true;
}

void QPostWigFile::BtnOutPutSlot()
{
    
    QString fileName=m_ReadEdit->text();
    if (fileName=="") return;
    fileName = QFileDialog::getSaveFileName(this,
        "Choose a file name", ".",
        //tr("HTML (*.html *.htm)"));
        //tr("IGES Files (*.iges *.igs);;STEP Files (*.step *.stp)"));
        ("txt Files (*.txt)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }
    QString data;
    data = "*Node\n";//节点
    file.write(data.toAscii());
    int iSize=frdVIS_.m_ListData.size();
    for (int i=0;i<iSize;i++)
    {
        data=frdVIS_.m_ListData.at(i)+"\n";
        file.write(data.toAscii());
    }
   
    file.close(); 
}
void QPostWigFile::LanguageUpData()
{
    m_ReadBtn->setText(tr("Import")); 
    m_LabName->setText(tr("File Name"));
}
