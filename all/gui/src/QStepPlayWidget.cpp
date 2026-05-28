#include "QStepPlayWidget.h"
//-----------------设置-------------------------
//--
QPlaySetDlg* QPlaySetDlg::instance=0;

QPlaySetDlg::QPlaySetDlg( QWidget *parent/*=NULL*/ )
{
	QHBoxLayout *HLayout1 = new QHBoxLayout();
	QLabel *labTime =new QLabel(tr("帧/S"));//
	timeVal_ = new QLineEdit();
	HLayout1->addWidget(labTime);
	HLayout1->addWidget(timeVal_);

	QHBoxLayout *HLayout2 = new QHBoxLayout();
	QGroupBox *gb1=new QGroupBox(tr("动画导出设置"));
    gb1->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
	QGridLayout *VLayout2_1=new QGridLayout(gb1);
	//movieChk_=new QCheckBox(tr("保存avi动画"),gb1);
	QLabel *lab1=new QLabel(tr("动画名称"),gb1);
	movieName_ = new QLineEdit(tr("WelCMEtest.avi"),gb1);
	//QLabel *lab2=new QLabel(tr("帧/s:"),gb1);
	//rateLab_=new QLabel(tr("20"),gb1); 
	//SliderRate_ =new QSlider(gb1);
	//SliderRate_->setOrientation(Qt::Horizontal);
	//SliderRate_->setRange(1,50);
	//SliderRate_->setValue(20);
	//VLayout2_1->addWidget(movieChk_,1,1);
	VLayout2_1->addWidget(lab1,2,1);
	VLayout2_1->addWidget(movieName_,2,2);
	//VLayout2_1->addWidget(lab2,2,1);
	//VLayout2_1->addWidget(SliderRate_,2,2);
	//VLayout2_1->addWidget(rateLab_,2,3);
	HLayout2->addWidget(gb1);
	gb1->setLayout(HLayout2);
	//HLayout2->addWidget(gb1);
	
	QHBoxLayout *Lastlayout = new QHBoxLayout();
	OKSetBtn_ = new QPushButton(tr("确定"));
	Lastlayout->addStretch();
	Lastlayout->addWidget(OKSetBtn_);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addLayout(HLayout1);
	mainLayout->addLayout(HLayout2);
	mainLayout->addLayout(Lastlayout);
	this->setLayout(mainLayout);
	connect(OKSetBtn_,SIGNAL(clicked()),this,SLOT(OKSetBtnSlot()));
	//connect(SliderRate_,SIGNAL(valueChanged(int)),this,SLOT(SliderMovieRateSlot(int)));
	//movieChk_->setChecked(false);

}
QPlaySetDlg* QPlaySetDlg::GetInstance()
{
	if (instance == 0)
	{
		instance = new QPlaySetDlg();
	}
	return instance;
}
void QPlaySetDlg::OKSetBtnSlot()
{
	this->accept();
	this->close();
}
//void QPlaySetDlg::SliderMovieRateSlot(int v)
//{
//	this->rateLab_->setText(QString("%1").arg(v));
//}
//strV[0]->timeVal
//strV[1]->movieName_
//strV[2]->movieNameRate_
void QPlaySetDlg::SetTimeValue(QString strV)
{
	QStringList strList=strV.split(",");
	timeVal_->setText(strList[0]);
	movieName_->setText(strList[1]);
	//SliderRate_->setValue(strList[2].toInt());
}
QString QPlaySetDlg::GetData()
{
	QString str=timeVal_->text();
	if (str.toInt()>=1000){
		str="1000";
	}
	/*QString check="0";
	if(movieChk_->isChecked())
		check="1";*/
		
	str+=+","+movieName_->text()+",15";//+","+check;//+QString("%1").arg(SliderRate_->value());
	return str;
}
//
//-----------------StepPlay---------------------
//
QStepPlayWidget* QStepPlayWidget::instance = 0;
QStepPlayWidget::QStepPlayWidget(QWidget *parent)
	: QWidget(parent)
{
    QHBoxLayout *layout1=new QHBoxLayout(this);
    m_ComBoParam01 = new QComboBox(this);
    m_ComBoParam02 = new QComboBox(this);
    m_ComBoParam03 = new QComboBox(this);
    StepNowBtn = new QPushButton(QIcon(":/images/playSure.png"),"", this);
    m_PlayGroupBtn = new QButtonGroup;
    StepPreBtn = new QPushButton(QIcon(":/images/playpre.png"),"", this);//tr("上一步")
    StepNextBtn = new QPushButton(QIcon(":/images/playnext.png"),"", this);//tr("下一步"),
    Step1stBtn = new QPushButton(QIcon(":/images/playfirst.png"),"", this);//tr("第一步"),
    StepLastBtn = new QPushButton(QIcon(":/images/playlast.png"),"", this);//tr("最后一步"),
    StepPlayBtn  = new QPushButton(QIcon(":/images/playstart.png"),"", this);//tr("播放"), 
    StepPauseBtn = new QPushButton(QIcon(":/images/playstop.png"),"",this);//tr("暂停"),
	SetPlayBtn = new QPushButton(QIcon(":/images/playset.png"),"",this);//tr("设置"),
	MovieBtn = new QPushButton(QIcon(":/images/run.png"),"",this);//tr("播放"),
	
    
    m_PlayGroupBtn->addButton(Step1stBtn,Step_ENUM_1ST);
    m_PlayGroupBtn->addButton(StepPreBtn,Step_ENUM_PRE);
    m_PlayGroupBtn->addButton(StepPlayBtn,Step_ENUM_PLAY);
    m_PlayGroupBtn->addButton(StepPauseBtn,Step_ENUM_PAUSE);
    m_PlayGroupBtn->addButton(StepNextBtn,Step_ENUM_NEXT);
    m_PlayGroupBtn->addButton(StepLastBtn,Step_ENUM_LAST);
	m_PlayGroupBtn->addButton(MovieBtn,Step_ENUM_MOVIE);

    layout1->addWidget(m_ComBoParam01);
    layout1->addWidget(m_ComBoParam02);
    layout1->addWidget(m_ComBoParam03); 
    layout1->addWidget(StepNowBtn);
    layout1->addWidget(Step1stBtn);
    layout1->addWidget(StepPreBtn);
    layout1->addWidget(StepPlayBtn);
    layout1->addWidget(StepPauseBtn);
    layout1->addWidget(StepNextBtn);
    layout1->addWidget(StepLastBtn);
	layout1->addWidget(SetPlayBtn);//setup 
	layout1->addWidget(MovieBtn);
    //layout1->addWidget(m_ComBoParam01);
   // layout1->addWidget(m_ComBoParam01);

    setLayout(layout1);
    StepNowBtn->setFlat(true);StepNowBtn->setToolTip(tr("确定"));
    StepPreBtn->setFlat(true);Step1stBtn->setFlat(true);StepLastBtn->setFlat(true);
    StepNextBtn->setFlat(true);StepPlayBtn->setFlat(true);StepPauseBtn->setFlat(true);
	SetPlayBtn->setFlat(true);MovieBtn->setFlat(true);
    StepPreBtn->setToolTip(tr("上一步"));Step1stBtn->setToolTip(tr("第一步"));
    StepLastBtn->setToolTip(tr("最后一步"));StepNextBtn->setToolTip(tr("下一步"));
    StepPlayBtn->setToolTip(tr("开始"));StepPauseBtn->setToolTip(tr("暂停"));
	SetPlayBtn->setToolTip(tr("设置"));MovieBtn->setToolTip(tr("录制动画"));

    this->setMaximumHeight(35);
    this->m_ComBoParam01->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);//setMaximumWidth(50);
    this->m_ComBoParam02->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);//setMaximumWidth(50);
    this->m_ComBoParam03->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);//setMaximumWidth(50);
    layout1->setSpacing(0);
    layout1->setMargin(0);
    connect(m_PlayGroupBtn,SIGNAL(buttonClicked(int)),this,SLOT(GroupBtnSlot(int)));
    connect(StepNowBtn,SIGNAL(clicked()),this,SLOT(NowOKBtnSlot()));
	connect(SetPlayBtn,SIGNAL(clicked()),this,SLOT(SetUpBtnSlot()));
    
    connect(m_ComBoParam01,SIGNAL(currentIndexChanged(int)),this,SLOT(ComboxParam01Slot(int)));
    connect(m_ComBoParam02,SIGNAL(currentIndexChanged(int)),this,SLOT(ComboxParam02Slot(int)));

	m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(AutoPlaySlot()));

	TValue_ =1000;//初始化时间为1000ms；
    m_ChangingCombos = false;
    connect(m_ComBoParam03,SIGNAL(currentIndexChanged(int)),this,SLOT(ComboxParam03Slot(int)));
}

QStepPlayWidget::~QStepPlayWidget()
{

}
QStepPlayWidget* QStepPlayWidget::GetInstance()
{
    if (instance == 0)
    {
        instance = new QStepPlayWidget;
    }
    return instance;
}
void QStepPlayWidget::NowOKBtnSlot()
{
    SetPlayParam();
}
//0->上一步；1->下一步；2->第一步；
//3->最后一步；4->播放；5->暂停；
void QStepPlayWidget::GroupBtnSlot(int index)
{
    if (m_ResultO.m_Menu.m_MenuName.size()<=0){ 
        return;
    }
    int id_P1=0,id_p2=0,id_p3=0;
    id_p2=m_ComBoParam02->currentIndex();
    id_p3=m_ComBoParam03->currentIndex();
    switch(index){ 
    case Step_ENUM_PRE:
        id_P1 = m_ComBoParam01->currentIndex();
        id_P1=id_P1-1;
        if (id_P1<0)id_P1=0;
        m_ComBoParam01->setCurrentIndex(id_P1);
        break;
    case Step_ENUM_NEXT:
        id_P1 = m_ComBoParam01->currentIndex();
        id_P1=id_P1+1;
        if (id_P1>=m_ComBoParam01->count())id_P1=m_ComBoParam01->count()-1;
        m_ComBoParam01->setCurrentIndex(id_P1);
        break;
    case Step_ENUM_1ST:
        m_ComBoParam01->setCurrentIndex(0);
        break;
    case Step_ENUM_LAST:
        m_ComBoParam01->setCurrentIndex(m_ComBoParam01->count()-1);
        break;
    case Step_ENUM_PLAY://开启Timer定时器
        m_timer->start(TValue_);//(1000);
        StepPlayBtn->setDown(true);
        break;
    case Step_ENUM_PAUSE://关闭Timer定时器
        m_timer->stop();
		m_StepPlayParam.movieSet=9;//end movie
        StepPlayBtn->setDown(false);
        break;
	case Step_ENUM_MOVIE://打开Movie
		//if (m_StepPlayParam.movieSet==0||m_StepPlayParam.movieSet==9){
		m_StepPlayParam.movieSet=1;//start movie
		//	MovieBtn->setIcon(QIcon(":/images/run.png"));
		//}else if (m_StepPlayParam.movieSet==1||m_StepPlayParam.movieSet==2){//start /updata movie
		//	m_StepPlayParam.movieSet=9;//end movie
		//	MovieBtn->setIcon(QIcon(":/images/stop.png"));
		//}
		MovieBtn->setDown(true);
		break;
    default:break;
    } 
    //--
    if (id_p2>=m_ComBoParam02->count()&& id_p2!=0){
        id_p2=m_ComBoParam02->count()-1;
    }
    m_ComBoParam02->setCurrentIndex(id_p2);

    if (id_p3>=m_ComBoParam03->count()&& id_p3!=0){
        id_p3=m_ComBoParam03->count()-1;
    }
    m_ComBoParam03->setCurrentIndex(id_p3);
    SetPlayParam();
}
//--updata Combox1
void QStepPlayWidget::updataCombox(ResultOutputS ResultO)
{
    m_ResultO = ResultO;
    m_ChangingCombos = true;
    m_ComBoParam01 ->clear();
    m_ComBoParam02 ->clear();
    m_ComBoParam03 ->clear();
    m_ComBoParam01->insertItems(0,ResultO.m_Menu.m_MenuName); 
    m_ChangingCombos = false;
    ComboxParam01Slot(m_ComBoParam01->currentIndex());
}
void QStepPlayWidget::ComboxParam01Slot(int id)
{
    if (m_ChangingCombos) return;
    id= m_ComBoParam01->currentIndex();
    QString strT=m_ComBoParam02->currentText();
    if (id>-1 && id<m_ResultO.m_Menu.m_subMenuName.size()){
        QString tmpMenu=m_ResultO.m_Menu.m_subMenuName.at(id).join(",");
        QStringList combList;combList.clear();
        for (int kk=0;kk<m_ComBoParam02->count();kk++){
            combList<<m_ComBoParam02->itemText(kk);
        }
        QString tmpComb=combList.join(",");
        int ret=tmpMenu.compare(tmpComb,Qt::CaseInsensitive);
        if (ret==0){
            ComboxParam02Slot(m_ComBoParam02->currentIndex());
            return;
        }
        m_ChangingCombos = true;
        m_ComBoParam02->clear();
        m_ComBoParam02->addItems(m_ResultO.m_Menu.m_subMenuName.at(id));
        int ind=m_ComBoParam02->findText(strT);
        if (ind!=-1)m_ComBoParam02->setCurrentIndex(ind);
        m_ChangingCombos = false;
        ComboxParam02Slot(m_ComBoParam02->currentIndex());
    }
}
void QStepPlayWidget::ComboxParam02Slot(int id)
{
    if (m_ChangingCombos) return;
    id = m_ComBoParam02->currentIndex();
    QString strT=m_ComBoParam03->currentText();
    int compIndex = componentMenuIndex(m_ComBoParam01->currentIndex(), id);
    if (compIndex>-1 && compIndex<m_ResultO.m_Menu.m_sub2MenuName.size()){
        QString tmpMenu=m_ResultO.m_Menu.m_sub2MenuName.at(compIndex).join(",");
        QStringList combList;combList.clear();
        for (int kk=0;kk<m_ComBoParam03->count();kk++){
            combList<<m_ComBoParam03->itemText(kk);
        }
        QString tmpComb=combList.join(",");
        int ret=tmpMenu.compare(tmpComb,Qt::CaseInsensitive);
        if (ret==0){
            SetPlayParam();
            return;
        }
        m_ChangingCombos = true;
        m_ComBoParam03->clear();
        m_ComBoParam03->addItems(m_ResultO.m_Menu.m_sub2MenuName.at(compIndex));
        int ind=m_ComBoParam03->findText(strT);
        if (ind!=-1)m_ComBoParam03->setCurrentIndex(ind);
        m_ChangingCombos = false;
        SetPlayParam();
    } 
}
void QStepPlayWidget::ComboxParam03Slot(int id)
{
    Q_UNUSED(id);
    if (m_ChangingCombos) return;
    SetPlayParam();
}
//--COMBOXUPDATA
void  QStepPlayWidget::updataParam(StepPlayVisS mParam)
{
    m_StepPlayParam = mParam;

    //--updatacurrentCOMBOX
    QString str1,str2,str3;
    
    str1=m_StepPlayParam.strName.left(m_StepPlayParam.strName.indexOf("-"));// L101;
    str2=m_StepPlayParam.strName.left(m_StepPlayParam.strName.indexOf(":"));
    str3=m_StepPlayParam.strName.remove(str2+":");
    str2=str2.remove(str1+"-");
    int index=0;
    index = m_ComBoParam01->findText(str1);
    if(index!=-1){
        m_ComBoParam01->setCurrentIndex(index);
    }
    index = m_ComBoParam02->findText(str2);
    if(index!=-1){
        m_ComBoParam02->setCurrentIndex(index);
    }
    index = m_ComBoParam03->findText(str3);
    if(index!=-1){
        m_ComBoParam03->setCurrentIndex(index);
    }

}
void QStepPlayWidget::AutoPlaySlot()
{
    if (m_ResultO.m_Menu.m_MenuName.size()<=0){ 
        return;
    }
    //--获取当前str内容
    int id_P1=0,id_p2=0,id_p3=0;
    id_p2=m_ComBoParam02->currentIndex();
    id_p3=m_ComBoParam03->currentIndex();
   
    id_P1 = m_ComBoParam01->currentIndex();
    id_P1=id_P1+1;
    if (id_P1>=m_ResultO.m_Menu.m_MenuName.size()){//停止自动播放功能
		m_timer->stop();
		StepPlayBtn->setDown(false);
		m_StepPlayParam.movieSet=9;
    }
    if (id_P1>=m_ComBoParam01->count())id_P1=m_ComBoParam01->count()-1;
    m_ComBoParam01->setCurrentIndex(id_P1);
    //--
    if (id_p2>=m_ComBoParam02->count()&& id_p2!=0){
        id_p2=m_ComBoParam02->count()-1;
    }
    m_ComBoParam02->setCurrentIndex(id_p2);

    if (id_p3>=m_ComBoParam03->count()&& id_p3!=0){
        id_p3=m_ComBoParam03->count()-1;
    }
    m_ComBoParam03->setCurrentIndex(id_p3);
	if (m_StepPlayParam.movieSet==1){
		m_StepPlayParam.movieSet=2;//updata;
	}
    SetPlayParam();
}
void QStepPlayWidget::SetPlayParam()
{
    if (m_ComBoParam01->currentIndex()<0 ||
        m_ComBoParam02->currentIndex()<0 ||
        m_ComBoParam03->currentIndex()<0) {
        return;
    }
    QString strName = m_ComBoParam01->currentText()+"-"+
        m_ComBoParam02->currentText()+":"+
        m_ComBoParam03->currentText();
    m_StepPlayParam.strName=strName;
    emitPlayStepParam(m_StepPlayParam);
}
int QStepPlayWidget::componentMenuIndex(int stepIndex, int variableIndex) const
{
    if (stepIndex<0 || variableIndex<0 ||
        stepIndex>=m_ResultO.m_Menu.m_subMenuName.size()) {
        return -1;
    }
    int index = variableIndex;
    for (int i=0; i<stepIndex; ++i) {
        index += m_ResultO.m_Menu.m_subMenuName.at(i).size();
    }
    return index;
}
//--setup time
void QStepPlayWidget::SetUpBtnSlot()
{
	QPlaySetDlg::GetInstance()->SetTimeValue(QString("%1").arg((1000.0/(float)TValue_))+","
											 +m_StepPlayParam.movieName+","
											 +QString("%1").arg(m_StepPlayParam.movieNameRate));
	if(QPlaySetDlg::GetInstance()->exec())
	{
		QStringList strL=QPlaySetDlg::GetInstance()->GetData().split(",");
		TValue_=int(1000.0/strL[0].toFloat());
		m_StepPlayParam.movieName = strL[1];
		m_StepPlayParam.movieNameRate=strL[2].toInt();
		/*if (strL[2]=="0"){
			m_StepPlayParam.movieSet=0;
		}else{
			m_StepPlayParam.movieSet=1;
		}*/
	}
}
