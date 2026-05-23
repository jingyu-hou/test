#include "QCfgFileManage.h"

#include <QXmlStreamReader>
#include <QtXml>
#include <QMessageBox>
#include <QDialog>
#include "ADefineQ.h"


QCfgFileManage::QCfgFileManage(QWidget *parent)
: QWidget(parent)
{

}

QCfgFileManage::~QCfgFileManage()
{

}
/*
加载配置文件
*/
bool QCfgFileManage::LoadConfigFile()
{
    QString	FilePathName=QCoreApplication::applicationDirPath()+PLOTOPTNAME;
    QString fileStart=PLOTOPTNAMESTART;
    LoadOptionFile(FilePathName,fileStart);
    FilePathName=QCoreApplication::applicationDirPath()+READRESULT;
    fileStart=READRESULTSTART;
    LoadOptionFile(FilePathName,fileStart);
    return true;

}
/*----------------------------------
  配置文件
----------------------------------*/
bool QCfgFileManage::LoadOptionFile(QString fileName,QString NameStart)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox::warning(this,tr("Configer File"),NameStart+tr(" No this Configer File"),QMessageBox::Ok,QMessageBox::Ok);
        return false;
    }
    QXmlStreamReader Reader(&file);
    while (!Reader.atEnd())
    {
        //读取下一个元素
        QXmlStreamReader::TokenType nType = Reader.readNext();
        if (nType == QXmlStreamReader::StartElement){
            if (Reader.name() == NameStart){
                if (NameStart==READRESULTSTART){
                 ParseEntryVTKText(Reader);   
                }else if (NameStart==PLOTOPTNAMESTART){
                 ParseEntry(Reader);
                }    
            }
        }
    }
    file.close();

    if (Reader.hasError()){//解析出错时
        QMessageBox::warning(this,tr("Configer File"),NameStart+tr(" Read File Error"),QMessageBox::Ok,QMessageBox::Ok);
    }
    return true;
}

/*----------------------------------
    ParseEntry vtktext配置文件
----------------------------------*/
void QCfgFileManage::ParseEntryVTKText(QXmlStreamReader &reader)
{
    QStringRef strPageTmp;
    QStringList pageList,pageList2,pageList3,pageList4;
    QStringRef strName;
    while (!reader.atEnd())
    {
        strName=reader.name();
        //page=reader.readElementText();
        QXmlStreamReader::TokenType type = reader.readNext();
        if (type == QXmlStreamReader::EndElement){
            if(reader.name() == READRESULTSTART)
                break;
        }

        if (type == QXmlStreamReader::StartElement){
            int NameId =chgTheNameToInt(reader.name());
            switch(NameId){
            case 1:ParseEntry(reader);           
                break;
            case 11:m_readRltDlgs.RadioTextIndex = reader.readElementText().toInt();
                break;
            default:break;
            }
        }
    }
}
/*----------------------------------
    ParseEntry 配置文件
----------------------------------*/
void QCfgFileManage::ParseEntry(QXmlStreamReader &reader)
{
    QStringRef strPageTmp;
    QStringList pageList,pageList2,pageList3,pageList4;
    QStringRef strName;
    while (!reader.atEnd())
    {
        strName=reader.name();
        //page=reader.readElementText();
        QXmlStreamReader::TokenType type = reader.readNext();
        if(type == QXmlStreamReader::EndElement){
            if(reader.name() == PLOTOPTNAMESTART)
                break;
        }

        if (type == QXmlStreamReader::StartElement)
        {
            int NameId =chgTheNameToInt(reader.name());
            switch(NameId){
            case 1:ParseEntry(reader);
                break;
            case 11:
                m_optDlgs.BaseRenderIndex= reader.readElementText().toInt();break;
            case 12:
                m_optDlgs.BaseScaleInder= reader.readElementText().toInt();break;
            case 13:
                m_optDlgs.BaseVisIndex= reader.readElementText().toInt();break;
            case 14:
                m_optDlgs.BaseValue=reader.readElementText().toFloat();break;
            case 20:
                m_optDlgs.ClrEdgesWHPlot=reader.readElementText().toLocal8Bit().constData() ;break;
            case 21:
                m_optDlgs.ClrEdgesFSPlot=reader.readElementText().toLocal8Bit().constData();break;
            case 22:
                m_optDlgs.ClrEdgesCSPLot=reader.readElementText().toLocal8Bit().constData();break;
            case 23:
                m_optDlgs.ClrHisCuvPoint=reader.readElementText().toLocal8Bit().constData();break;
            case 24:
                m_optDlgs.ClrHisPointfactor=reader.readElementText().toLocal8Bit().constData();break;
            case 25:
                m_optDlgs.ClrEdgeAStyIndex=reader.readElementText().toInt();break;
            case 26:
                m_optDlgs.ClrEdgeAThikIndex=reader.readElementText().toInt();break;
            case 30:
                m_optDlgs.LabElemChk=reader.readElementText().toInt();break;
            case 31:
                m_optDlgs.LabFaceChk=reader.readElementText().toInt();break;
            case 32:
                m_optDlgs.LabNodeChk=reader.readElementText().toInt();break;
            case 33:
                m_optDlgs.LabNodSymbolChk=reader.readElementText().toInt();break;
            case 34:
                m_optDlgs.LabClrShowElem=reader.readElementText().toLocal8Bit().constData() ;break;
            case 35:
                m_optDlgs.LabClrShowFace=reader.readElementText().toLocal8Bit().constData() ;break;
            case 36:
                m_optDlgs.LabClrShowNode=reader.readElementText().toLocal8Bit().constData() ;break;
            case 37:
                m_optDlgs.LabClrShowNodeSymb=reader.readElementText().toLocal8Bit().constData() ;break;
            case 38:
                m_optDlgs.LabNodSymbolIndex=reader.readElementText().toInt();break;
            case 39:
                m_optDlgs.LabNodSizeIndex=reader.readElementText().toInt();break;
            case 40:
                m_optDlgs.NormShowChk=reader.readElementText().toInt();break;
            case 41:
                m_optDlgs.NormRadioElemOrSur=reader.readElementText().toInt();break;
            case 42:
                m_optDlgs.NormClrFacePlot=reader.readElementText().toLocal8Bit().constData();break;
            case 43:
                m_optDlgs.NormClrBeamTangPlot=reader.readElementText().toLocal8Bit().constData();break;
            case 44:
                m_optDlgs.NormClrBeamN1Plot=reader.readElementText().toLocal8Bit().constData();break;
            case 45:
                m_optDlgs.NormClrBeamN2Plot=reader.readElementText().toLocal8Bit().constData();break;
            case 46:
                m_optDlgs.NormStyLenIndex=reader.readElementText().toLocal8Bit().toInt();break;
            case 47:
                m_optDlgs.NormStyLinThickIndex=reader.readElementText().toLocal8Bit().toInt();break;
            case 48:
                m_optDlgs.NormStyArrowHeadIndex=reader.readElementText().toLocal8Bit().toInt();break;
                break;
            default:break;   
            }
        }
    }
}
/*
descrition:
change the XML Node to the Int
*/
int QCfgFileManage::chgTheNameToInt(QStringRef strRef)
{
    if (strRef=="Baise" ||strRef=="Color"||strRef=="Label"||strRef=="Normals"){return 1;
    }else if (strRef=="Bs1"){return 11;
    }else if (strRef=="Bs2"){return 12;
    }else if (strRef=="Bs3"){return 13;
    }else if (strRef=="Bs4"){return 14;
    }else if (strRef=="Clr1"){return 20;
    }else if (strRef=="Clr2"){return 21;
    }else if (strRef=="Clr3"){return 22;
    }else if (strRef=="Clr4"){return 23;
    }else if (strRef=="Clr5"){return 24;
    }else if (strRef=="Clr6"){return 25;
    }else if (strRef=="Clr7"){return 26;
    }else if (strRef=="Lab1"){return 30;
    }else if (strRef=="Lab2"){return 31;
    }else if (strRef=="Lab3"){return 32;
    }else if (strRef=="Lab4"){return 33;
    }else if (strRef=="Lab5"){return 34;
    }else if (strRef=="Lab6"){return 35;
    }else if (strRef=="Lab7"){return 36;
    }else if (strRef=="Lab8"){return 37;
    }else if (strRef=="Lab9"){return 38;
    }else if (strRef=="Lab10"){return 39;
    }else if (strRef=="Norm1"){return 40;
    }else if (strRef=="Norm2"){return 41;
    }else if (strRef=="Norm3"){return 42;
    }else if (strRef=="Norm4"){return 43;
    }else if (strRef=="Norm5"){return 44;
    }else if (strRef=="Norm6"){return 45;
    }else if (strRef=="Norm7"){return 46;
    }else if (strRef=="Norm8"){return 47;
    }else if (strRef=="Norm9"){return 48;
    }
    return -1;
}
