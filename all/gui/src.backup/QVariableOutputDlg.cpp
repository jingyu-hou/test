#include "QVariableOutputDlg.h"

QVariableOutputDlg::QVariableOutputDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("变量输出");
    //QWidget *Hwid1 = new QWidget(this);
    QGroupBox *gbGroup = new QGroupBox(tr("Choose"));
    gbGroup->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    
 
    QHBoxLayout *Hlayout = new QHBoxLayout();
    QVBoxLayout *Vlayout = new QVBoxLayout();

    QGridLayout *gGridlayout1 = new QGridLayout(gbGroup);
    //QHBoxLayout *gMainLayout = new QHBoxLayout(gbGroup);
    
    m_CheckBox[0]= new QCheckBox(tr("应力"),gbGroup);
    m_CheckBox[1]= new QCheckBox(tr("位移"),gbGroup);
    m_CheckBox[2]= new QCheckBox(tr("晶粒度"),gbGroup);
    m_CheckBox[3]= new QCheckBox(tr("应变"),gbGroup);
    m_CheckBox[4]= new QCheckBox(tr("相变"),gbGroup);
    m_CheckBox[5]= new QCheckBox(tr("密度"),gbGroup);
    m_CheckBox[6]= new QCheckBox(tr("温度"),gbGroup);

    m_AllCheckBox = new QCheckBox(tr("全选"),gbGroup);
   
    for (int kk=0;kk<7 ;kk++){
        gGridlayout1->addWidget(m_CheckBox[kk]);
    }
    m_OutputDatCheckBox = new QCheckBox(tr("同时输出Dat文件"));
    m_Combox= new QComboBox(gbGroup);
    m_Combox->addItem("按时间间隔输出");
    m_Combox->addItem("按计算频率输出");
   
    gGridlayout1->addWidget(m_AllCheckBox,0,1,1,1);
    m_OKBtn = new QPushButton(tr("确定"));
    m_DefaultBtn = new QPushButton(tr("默认"));
    m_CancelBtn = new QPushButton(tr("删除"));
    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    m_TimeEdit = new QLineEdit(tr("10"));
    m_TimeFreqStr<<tr("输出时间间隔(S):")<<tr("输出频率间隔(HZ):");
    m_timLabel = new QLabel(m_TimeFreqStr.at(0));
    //m_fLabel = new QLabel(tr("输出频率间隔(HZ):"));
    /*m_ComboxTime = new QComboBox();
    m_ComboxTime->addItem("10s");
    m_ComboxTime->addItem("20s");
    m_ComboxTime->addItem("30s");
    m_ComboxTime->addItem("40s");*/
    Hlayout2->addWidget(m_timLabel);
    Hlayout2->addWidget(m_TimeEdit);
    
    Vlayout->addWidget(gbGroup);
    Vlayout->addWidget(m_OutputDatCheckBox);
    Vlayout->addWidget(m_Combox);
    Hlayout->addWidget(m_OKBtn);
    Hlayout->addWidget(m_DefaultBtn);
    Hlayout->addWidget(m_CancelBtn); 
    //Hlayout->addWidget(m_SectionBtn);
    //Hlayout->addWidget(m_AlloyBtn);
    QVBoxLayout *MainLayout = new QVBoxLayout();
    //MainLayout->addLayout(gMainLayout);
    MainLayout->addLayout(Vlayout);
    MainLayout->addLayout(Hlayout2);
    MainLayout->addLayout(Hlayout);

    setLayout(MainLayout);
    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(OKBtnSlot()));
    connect(m_AllCheckBox,SIGNAL(clicked()),this,SLOT(AllChooseSlot()));
    connect(m_Combox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeTimeFreqSlot(int)));
}

QVariableOutputDlg::~QVariableOutputDlg()
{

}

void QVariableOutputDlg::AllChooseSlot()
{
    for (int kk=0; kk<7 ;kk++)
    {
        m_CheckBox[kk]->setChecked(m_AllCheckBox->isChecked());
    }
   
}
void QVariableOutputDlg::ChangeTimeFreqSlot(int Index)
{
    m_timLabel->setText(m_TimeFreqStr.at(Index));
}

void QVariableOutputDlg::OKBtnSlot()
{
    WriteVarToInp();
}
/*
** 节点变量：U-位移；NT-温度
** 单元变量：S-应力；E-应变；SDV-场变量
*/
void QVariableOutputDlg::WriteVarToInp()
{
    VariableInpS nodeTmp;

    nodeTmp.strTname = m_TimeEdit->text();

    if(!m_OutputDatCheckBox->isChecked()){
        nodeTmp.strNodeFile = "NODE FILE";
        nodeTmp.strELFile = "EL FILE";
    }else{
        nodeTmp.strNodeFile = "NODE PRINT";
        nodeTmp.strELFile = "EL PRINT";
    }

    if (m_timLabel->text()=="输出时间间隔(S):"){
        nodeTmp.strTimeOrFreqName = "TIME POINTS";
    }else if(m_timLabel->text()=="输出频率间隔(HZ):"){
        nodeTmp.strTimeOrFreqName = "FREQUENCY";
    }
    if (m_CheckBox[0]->isChecked()){
        nodeTmp.strELInclude="S";
    }
    if (m_CheckBox[3]->isChecked()){
        nodeTmp.strELInclude=nodeTmp.strELInclude+",E";
    }
    if (m_CheckBox[1]->isChecked()){
        nodeTmp.strNInclude="U";
    }
    if (m_CheckBox[6]->isChecked()){
        nodeTmp.strNInclude=nodeTmp.strNInclude+",NT";
    }

	if (m_CheckBox[2]->isChecked()){
		nodeTmp.strNInclude=",drx";
	}

	if (m_CheckBox[4]->isChecked()){
		nodeTmp.strNInclude=",phas";
	}

	if (m_CheckBox[5]->isChecked()){
		nodeTmp.strNInclude=",peeq";
	}

    //--SDV :*EL FILE

    m_nodeVarList.clear();
    m_nodeVarList.append(nodeTmp);
}
//将读取的INP数据显示在当前的表格中
/*
-S 应力,E-应变,U-位移,NT-温度
*/
void QVariableOutputDlg::SetInpData(ReadInpResultS ReadInpData)
{
  int NVar=ReadInpData.TmpVariableInps.size();
  if(NVar<=0)return;
  QString str=ReadInpData.TmpVariableInps.at(0).strTname;
  m_TimeEdit->setText(str);
  str =ReadInpData.TmpVariableInps.at(0).strTimeOrFreqName.toLower();

  if (str=="frequency"){
    m_timLabel->setText(m_TimeFreqStr.at(1));
    m_Combox->setCurrentIndex(1);
  }else{
    m_timLabel->setText(m_TimeFreqStr.at(0));
    m_Combox->setCurrentIndex(0);
  }
  if(ReadInpData.TmpVariableInps.at(0).strELFile=="*elfile"||
     ReadInpData.TmpVariableInps.at(0).strNodeFile=="*nodefile"){ 
    m_OutputDatCheckBox->setChecked(false);
  }else{
    m_OutputDatCheckBox->setChecked(true);
  }
  QStringList strList;
  for(int JI=0;JI<NVar;JI++){
	  QString Str=ReadInpData.TmpVariableInps.at(JI).strELInclude;
	  if(Str!="")strList<<ReadInpData.TmpVariableInps.at(JI).strELInclude.split(",");//EL File
  }
  for (int i=0;i<strList.size();i++){
      str=strList.at(i);
      strList.replace(i,str.simplified());
  }
   
  if(strList.contains("s",Qt::CaseInsensitive)){
      m_CheckBox[0]->setChecked(true);
  }
  if(strList.contains("e",Qt::CaseInsensitive)){
      m_CheckBox[3]->setChecked(true);
  }

  if(strList.contains("sdv",Qt::CaseInsensitive)){
      m_CheckBox[2]->setChecked(true);
  }
  if(strList.contains("phas",Qt::CaseInsensitive)){
      m_CheckBox[4]->setChecked(true);
  }
  if(strList.contains("phas",Qt::CaseInsensitive)){
      m_CheckBox[5]->setChecked(true);
  }
  //if(strList.contains("rpd",Qt::CaseInsensitive)){
  //    m_CheckBox[7]->setChecked(true);
  //}
  //if(strList.contains("dmg",Qt::CaseInsensitive)){
  //    m_CheckBox[8]->setChecked(true);
  //}

  strList=ReadInpData.TmpVariableInps.at(0).strNInclude.split(",");//node File
  for (int i=0;i<strList.size();i++){
      str=strList.at(i);
      strList.replace(i,str.simplified());
  }
  if (strList.contains("u",Qt::CaseInsensitive)){
       m_CheckBox[1]->setChecked(true);
  }
  if (strList.contains("nt",Qt::CaseInsensitive)){
       m_CheckBox[6]->setChecked(true);
  }

    WriteVarToInp();
  //for (int i=0;i<strList.size();i++)
  //{
  //    strList.at(i);
  //}
}

