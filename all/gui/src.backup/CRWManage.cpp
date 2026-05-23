#include "CRWManage.h"
#include "Information_Widget.h"
#include <QDateTime>
#include <QCoreApplication>

CRWManage::CRWManage(void)
{

}

CRWManage::~CRWManage(void)
{

}
//------------------------------------------------------------
//--Write Inp
//------------------------------------------------------------
//--��������
bool CRWManage::WriteUniformLoadInpFile(QFile *file,Gravity_Inp m_Gravity,QList<SectionInpS> m_nodeList)
{
	QString data;
    if (m_Gravity.Value==""||m_Gravity.direation==""){
        return false;
    }
	int IJ=m_nodeList.size();
	if(IJ<=0)return false;

    data = "*DLOAD\n";
    file->write(data.toAscii());

    for (int i=0; i<IJ; i++)
    {
        data =m_nodeList.at(i).strelset+",GRAV,"+m_Gravity.Value+m_Gravity.direation+"\n";
        file->write(data.toAscii());
    }
     file->write("** \n");
     return true;
}
//--�ڵ㡢��Ԫ�����ϣ��ڵ㡢��Ԫ�����棩
bool CRWManage::WritePartFile(QFile *file, NodeELSetInps m_NElData,QList<SectionInpS> m_nodeList)
{
	QString data;
	int nSize;
	int nElenemtType=0;
	int nSection=m_nodeList.size();
	//int EleNumber1=0;
	if(nSection<=0)return true;
	QStringList NodeInfor2,NodeSetInf2,ElenentSetInf2;
	int nSizess = m_NElData.NSetInpData.strNSetName.size();
	for(int j=0;j<nSizess;j++){
		for(int i=0;i<nSection;i++){
			int NodeStar,NodeEnd,NodeInc;
			QString NseltStyle;
			QStringList NodeNum;
			QString SectionName=m_nodeList.at(i).strelset;
			//nSize = m_NElData.NSetInpData.strNSetName.size();
			QString NsetName=m_NElData.NSetInpData.strNSetName.at(j);
			NseltStyle=m_NElData.NSetInpData.strNSetStyle.at(j);
			if(SectionName.toLower()!=NsetName.toLower())continue;
			//**************Node
			//�����в��ҵ㼯�����ƺ�����
			/*for(j=0;j<nSize;j++){
				QString NsetName=m_NElData.NSetInpData.strNSetName.at(j);
				NseltStyle=m_NElData.NSetInpData.strNSetStyle.at(j);
				if(SectionName.toLower()==NsetName.toLower())break;
			}*/
			//���Ҳ����нڵ���
			if(NseltStyle.simplified().toLower()=="generate"){
				QString dvsd=m_NElData.NSetInpData.strData.at(j).at(0);
				NodeStar=m_NElData.NSetInpData.strData.at(j).at(0).split(",",QString::SkipEmptyParts).at(0).toInt();
				NodeEnd=m_NElData.NSetInpData.strData.at(j).at(0).split(",",QString::SkipEmptyParts).at(1).toInt();
				NodeInc=m_NElData.NSetInpData.strData.at(j).at(0).split(",",QString::SkipEmptyParts).at(2).toInt();
			}else{
				int k=m_NElData.NSetInpData.strData.at(j).size();
				for(int L=0;L<k;L++){
					int Nn=m_NElData.NSetInpData.strData.at(j).at(L).split(",").size();
					NodeNum<<m_NElData.NSetInpData.strData.at(j).at(L).split(",",QString::SkipEmptyParts);
				}
			}
			data = "*Node\n";//�ڵ�
			file->write(data.toAscii());
			//generate
			nSize= m_NElData.NodeInpData.strData.size();
			for (int ii=0; ii<nSize; ii++){
				if(NodeNum.size()>0)break;
				int NmuberNode=m_NElData.NodeInpData.strData.at(ii).split(",",QString::SkipEmptyParts).at(0).toInt();
				if(NmuberNode<=NodeEnd&&NmuberNode>=NodeStar){
					data=m_NElData.NodeInpData.strData.at(ii)+"\n";
					file->write(data.toAscii());
					NodeInfor2<<m_NElData.NodeInpData.strData.at(ii);
				}else if(NmuberNode>NodeEnd){
					break;
				}
			}
			//��������Nset����generate���
			for (int ij=0; ij<nSize; ij++){
				int Num=NodeNum.size();
				if(Num<=0)break;
				for(int j=0;j<Num;j++){
					QString NodeStr=NodeNum.at(j).simplified();
					if(NodeStr=="")continue;
					QString NodeStr0=m_NElData.NodeInpData.strData.at(ij).split(",",QString::SkipEmptyParts).at(0);
					if(NodeStr0==NodeStr){
						data=m_NElData.NodeInpData.strData.at(ij)+"\n";
						file->write(data.toAscii());
						NodeInfor2<<m_NElData.NodeInpData.strData.at(ij);
					}
				}
			}
			//********************************************************************Element
			nSize = m_NElData.ElSetInpData.strElSetName.size();
			int startnode02=0;
			int endnode02=0;
			for (int ic=0; ic<nSize; ic++){
				if(m_NElData.ElSetInpData.strElSetName.at(ic).toLower()==SectionName.toLower())ElenentSetInf2<<m_NElData.ElSetInpData.strElSetName.at(ic);
				else continue;

				if (m_NElData.ElSetInpData.strElSetStyle.at(ic)!=""){
					QStringList strList=m_NElData.ElSetInpData.strData.at(ic).split(",",QString::SkipEmptyParts);
					startnode02=strList.at(0).toInt();
					endnode02=strList.at(1).toInt();
				}else{
					QStringList strList=m_NElData.ElSetInpData.strData.at(ic).split(",",QString::SkipEmptyParts);
					QStringList strList01;
					for (int kk=0;kk<strList.size();kk+=16){//������������ݲ�����16��
						for (int tt=kk;tt<kk+16;tt++){
							if (tt<strList.size()){
								int Number=strList.at(tt).toInt();
								if(Number<startnode02&&startnode02!=0)startnode02=Number;
								else if(startnode02==0)startnode02=Number;

								if(Number>endnode02&&endnode02!=0)endnode02=Number;
								else if(endnode02==0)endnode02=Number;
							}
						}
					}
				}
			}
			startnode02=startnode02-1;
			endnode02=endnode02-1;
			data="*Element,type="+m_NElData.ELInpData.ElementType.at(nElenemtType)+"\n";//��Ԫ
			file->write(data.toAscii());
			int ENumber=m_NElData.ELInpData.NumberE.at(nElenemtType);
			int ElementTy=m_NElData.ELInpData.ElementType.size();
			int Nums=0;
			for(int j=startnode02;j<=endnode02;j++){
				if(Nums==ENumber-1&&nElenemtType<ElementTy){
					nElenemtType++;
				}
				if(Nums==ENumber&&nElenemtType<ElementTy){
					data="*Element,type="+m_NElData.ELInpData.ElementType.at(nElenemtType)+"\n";
					file->write(data.toAscii());
					ENumber=m_NElData.ELInpData.NumberE.at(nElenemtType);
					Nums=0;
				}
				data=m_NElData.ELInpData.strData.at(j)+"\n";
				file->write(data.toAscii());
				Nums++;
			}
			/*for(int j=0;j<ENumber;j++){
			data=m_NElData.ELInpData.strData.at(EleNumber1)+"\n";
			file->write(data.toAscii());
			EleNumber1++;
		}*/
			//***************Nset
			nSize = m_NElData.NSetInpData.strNSetName.size();
			for(int ib=0; ib<nSize; ib++){
				if(m_NElData.NSetInpData.strNSetName.at(ib).toLower()==SectionName.toLower())NodeSetInf2<<m_NElData.NSetInpData.strNSetName.at(ib);
				else continue;

				if (NseltStyle!=""){
					data="*Nset,nset="+m_NElData.NSetInpData.strNSetName.at(ib)+",generate\n";
				}else{
					data="*Nset,nset="+m_NElData.NSetInpData.strNSetName.at(ib)+"\n";
				}

				file->write(data.toAscii());
				int nDataSize=m_NElData.NSetInpData.strData.at(ib).size();
				for (int j=0; j<nDataSize; j++){
					QStringList strL=m_NElData.NSetInpData.strData.at(ib).at(j).split(",",QString::SkipEmptyParts);
					QStringList strL01;
					for (int kk=0;kk<strL.size();kk+=16){
						strL01.clear();
						for (int tt=kk;tt<kk+16;tt++){
							if(tt<strL.size()){
								strL01<<strL.at(tt);
							}
						}
						data=strL01.join(",")+"\n";
						file->write(data.toAscii());
					}
				}
			}
			//***************Elset
			//--��Ԫ����  
			nSize = m_NElData.ElSetInpData.strElSetName.size();
			for (int ic=0; ic<nSize; ic++){
				if(m_NElData.ElSetInpData.strElSetName.at(ic).toLower()==SectionName.toLower())ElenentSetInf2<<m_NElData.ElSetInpData.strElSetName.at(ic);
				else continue;

				if (m_NElData.ElSetInpData.strElSetStyle.at(ic)!=""){
					data="*Elset,elset="+m_NElData.ElSetInpData.strElSetName.at(ic)+",generate\n";	
				}else{
					data="*Elset,elset="+m_NElData.ElSetInpData.strElSetName.at(ic)+"\n";
				}
				file->write(data.toAscii());

				QStringList strList=m_NElData.ElSetInpData.strData.at(ic).split(",",QString::SkipEmptyParts);
				QStringList strList01;
				for (int kk=0;kk<strList.size();kk+=16){//������������ݲ�����16��
					strList01.clear();
					for (int tt=kk;tt<kk+16;tt++){
						if (tt<strList.size()){
							strList01<<strList.at(tt);
						}	
					}
					data=strList01.join(",")+"\n";
					file->write(data.toAscii());
				}	
			}
			file->write("** \n");
			//***************Section
			data = "** Section: " + m_nodeList.at(i).strCommentaryName + "\n"+
				"*" + m_nodeList.at(i).strSectionStyle+",elset="+m_nodeList.at(i).strelset+",material="+m_nodeList.at(i).StrMaterial+"\n"+
				", \n";
			file->write(data.toAscii());
			file->write("** \n");
		}
	}
//*************************************************************************************************
//                     ʣ��ڵ㡢�ڵ㼯�ϡ���Ԫ���ϡ����漯�ϵ����
//*************************************************************************************************
	NodeInfor2.removeDuplicates();NodeSetInf2.removeDuplicates();ElenentSetInf2.removeDuplicates();//ɾ���ڵ㡢�ڵ㼯�ϡ���Ԫ���ϵ��ظ���Ϣ��
	data = "*Node\n";//�ڵ�
	nSize =NodeInfor2.size();
	int nSize2= m_NElData.NodeInpData.strData.size();
	if(nSize<nSize2){
		file->write(data.toAscii());
		for (int i=0; i<nSize2; i++){
			QString data1;
			bool b=false;
			data=m_NElData.NodeInpData.strData.at(i);
			nSize =NodeInfor2.size();
			for(int j=0; j<nSize; j++){
				data1=m_NElData.NodeInpData.strData.at(i).split(",",QString::SkipEmptyParts).at(0);
				if(data1==NodeInfor2.at(j).split(",",QString::SkipEmptyParts).at(0)){
					b=true;
					NodeInfor2.removeAt(j);
					break;
				}
			}
			if(true){
				continue;
			}else{
				data=data+"\n";
				file->write(data.toAscii());
				break;
			}
		}
	}
    //��Ԫ�ż���ڵ��ŵ����;
	//int ElementTy=m_NElData.ELInpData.ElementType.size();
	//int EleNumber=0;
	//if(nSection!=ElementTy){
	//	for(int i=nSection; i<ElementTy; i++){
	//		data="*Element,type="+m_NElData.ELInpData.ElementType.at(i)+"\n";//��Ԫ
	//		file->write(data.toAscii());
	//		int ENumber=m_NElData.ELInpData.NumberE.at(i);
	//		for(int j=0;j<ENumber;j++){
	//			data=m_NElData.ELInpData.strData.at(EleNumber)+"\n";
	//			file->write(data.toAscii());
	//			EleNumber++;
	//		}
	//	}
	//}
	//--�ڵ㼯��
	nSize = m_NElData.NSetInpData.strNSetName.size();
	nSize2=NodeSetInf2.size();
	if(nSize>nSize2){
		for(int i=0; i<nSize; i++){
			bool cycl=false;
			for(int j=0;j<nSize2;j++){
				if(m_NElData.NSetInpData.strNSetName.at(i).simplified()==NodeSetInf2.at(j).simplified()){
					cycl=true;
					break;
				}
			}
			if(cycl)continue;
			if (m_NElData.NSetInpData.strNSetStyle.at(i)!=""){
				data="*Nset,nset="+m_NElData.NSetInpData.strNSetName.at(i)+",generate\n";
			}else{
				data="*Nset,nset="+m_NElData.NSetInpData.strNSetName.at(i)+"\n";
			}

			file->write(data.toAscii());
			int nDataSize=m_NElData.NSetInpData.strData.at(i).size();
			for (int j=0; j<nDataSize; j++){
				/*data=m_NElData.NSetInpData.strData.at(i).at(j)+"\n";
				file->write(data.toAscii());*/
				QStringList strL=m_NElData.NSetInpData.strData.at(i).at(j).split(",",QString::SkipEmptyParts);
				QStringList strL01;
				for (int kk=0;kk<strL.size();kk+=16){
					strL01.clear();
					for (int tt=kk;tt<kk+16;tt++){
						if(tt<strL.size()){
							strL01<<strL.at(tt);
						}
					}
					data=strL01.join(",")+"\n";
					file->write(data.toAscii());
				}
			}
		}
	}
	//--��Ԫ����  
	nSize=m_NElData.ElSetInpData.strElSetName.size();
	nSize2=ElenentSetInf2.size();
	if(nSize>nSize2){
		//for(int j=0; j<nSize2; j++){
			for (int i=0; i<nSize; i++){
				bool cycl=false;
				for(int j=0; j<nSize2; j++){
					if(m_NElData.ElSetInpData.strElSetName.at(i).simplified()==ElenentSetInf2.at(j).simplified()){
						cycl=true;
						break;
					}
				}
				if(cycl)continue;
				if (m_NElData.ElSetInpData.strElSetStyle.at(i)!=""){
					data="*Elset,elset="+m_NElData.ElSetInpData.strElSetName.at(i)+",generate\n";	
				}else{
					data="*Elset,elset="+m_NElData.ElSetInpData.strElSetName.at(i)+"\n";
				}
				file->write(data.toAscii());

				QStringList strList=m_NElData.ElSetInpData.strData.at(i).split(",");
				QStringList strList01;
				for (int kk=0;kk<strList.size();kk+=16){//������������ݲ�����16��
					strList01.clear();
					for (int tt=kk;tt<kk+16;tt++){
						if (tt<strList.size()){
							if(strList.at(tt)!="")strList01<<strList.at(tt);
						}	
					}
					data=strList01.join(",")+"\n";
					file->write(data.toAscii());
				}	
			}
		//}
	}
	//--���漯��
	nSize = m_NElData.SurfaceInpData.strSurfaceName.size();
	int iBaseNum=0;
	for (int i=0;i<nSize;i++)
	{
		int iTotalDataCnt = m_NElData.SurfaceInpData.strData.size();
		data="*Surface,type=ELEMENT,name="+m_NElData.SurfaceInpData.strSurfaceName.at(i)+"\n";
		file->write(data.toAscii());
		int iSurfaceNum=m_NElData.SurfaceInpData.strSurfaceDataNum.at(i).toInt();
		for (int j=0;j<iSurfaceNum;j++){
			data = m_NElData.SurfaceInpData.strData.at(iBaseNum+j)+"\n";
			file->write(data.toAscii());
		}
		iBaseNum+=iSurfaceNum;
	}
	/* nSize = m_NElData.SurfaceInpData.strData.size();
	data="*Surface,type=ELEMENT,name="+m_NElData.SurfaceInpData.strSurfaceName+"\n";
	file->write(data.toAscii());
	for (int i=0; i<nSize; i++){
	data = m_NElData.SurfaceInpData.strData.at(i)+"\n";
	file->write(data.toAscii());
	}*/
	file->write("** \n");
	////--������Ϣ
	//for (int i=0;i<m_nodeList.size();i++)
	//{
	//	data = "** Section: " + m_nodeList.at(i).strCommentaryName + "\n"+
	//		"*" + m_nodeList.at(i).strSectionStyle+",elset="+m_nodeList.at(i).strelset+",material="+m_nodeList.at(i).StrMaterial+"\n"+
	//		", \n";
	//	file->write(data.toAscii());
	//}
	//file->write("** \n");

	return true;
}
bool CRWManage::WriteUnitElInpFile(QFile *file, NodeELSetInps m_NElData,QList<SectionInpS> m_nodeList)
{
    QString data;
	//int nSection=m_nodeList.size();
	//if(nSection>0)return true;
    data = "*Node\n";//�ڵ�
    file->write(data.toAscii());
    int nSize = m_NElData.NodeInpData.strData.size();
    for (int i=0; i<nSize; i++){
        data=m_NElData.NodeInpData.strData.at(i)+"\n";
        file->write(data.toAscii());
    }
  
	int ElementTy=m_NElData.ELInpData.ElementType.size();
	int EleNumber=0;
	for(int i=0; i<ElementTy; i++){
		data="*Element,type="+m_NElData.ELInpData.ElementType.at(i)+"\n";//��Ԫ
		file->write(data.toAscii());
		int ENumber=m_NElData.ELInpData.NumberE.at(i);
		for(int j=0;j<ENumber;j++){
			data=m_NElData.ELInpData.strData.at(EleNumber)+"\n";
			file->write(data.toAscii());
			EleNumber++;
		}
	}
    ////--�ڵ㼯��
    nSize = m_NElData.NSetInpData.strNSetName.size();
    for(int i=0; i<nSize; i++){
        if (m_NElData.NSetInpData.strNSetStyle.at(i)!=""){
           data="*Nset,nset="+m_NElData.NSetInpData.strNSetName.at(i)+",generate\n";
        }else{
           data="*Nset,nset="+m_NElData.NSetInpData.strNSetName.at(i)+"\n";
        }
        
        file->write(data.toAscii());
        int nDataSize=m_NElData.NSetInpData.strData.at(i).size();
        for (int j=0; j<nDataSize; j++){
            /*data=m_NElData.NSetInpData.strData.at(i).at(j)+"\n";
            file->write(data.toAscii());*/
            QStringList strL=m_NElData.NSetInpData.strData.at(i).at(j).split(",",QString::SkipEmptyParts);
            QStringList strL01;
            for (int kk=0;kk<strL.size();kk+=16){
                strL01.clear();
                for (int tt=kk;tt<kk+16;tt++){
                    if(tt<strL.size()){
                        strL01<<strL.at(tt);
                    }
                }
                data=strL01.join(",")+"\n";
                file->write(data.toAscii());
            }
        }
    }
    //--��Ԫ����  
    nSize = m_NElData.ElSetInpData.strElSetName.size();
    for (int i=0; i<nSize; i++){
        if (m_NElData.ElSetInpData.strElSetStyle.at(i)!=""){
            data="*Elset,elset="+m_NElData.ElSetInpData.strElSetName.at(i)+",generate\n";	
        }else{
            data="*Elset,elset="+m_NElData.ElSetInpData.strElSetName.at(i)+"\n";
        }
		file->write(data.toAscii());

		QStringList strList=m_NElData.ElSetInpData.strData.at(i).split(",");
		QStringList strList01;
		for (int kk=0;kk<strList.size();kk+=16){//������������ݲ�����16��
			strList01.clear();
			for (int tt=kk;tt<kk+16;tt++){
				if (tt<strList.size()){
					if(strList.at(tt)!="")strList01<<strList.at(tt);
				}	
			}
			data=strList01.join(",")+"\n";
			file->write(data.toAscii());
		}	
    }

    //--���漯��
    nSize = m_NElData.SurfaceInpData.strSurfaceName.size();
    int iBaseNum=0;
    for (int i=0;i<nSize;i++)
    {
        int iTotalDataCnt = m_NElData.SurfaceInpData.strData.size();
        data="*Surface,type=ELEMENT,name="+m_NElData.SurfaceInpData.strSurfaceName.at(i)+"\n";
        file->write(data.toAscii());
        int iSurfaceNum=m_NElData.SurfaceInpData.strSurfaceDataNum.at(i).toInt();
        for (int j=0;j<iSurfaceNum;j++){
            data = m_NElData.SurfaceInpData.strData.at(iBaseNum+j)+"\n";
            file->write(data.toAscii());
        }
        iBaseNum+=iSurfaceNum;

    }

   /* nSize = m_NElData.SurfaceInpData.strData.size();
    data="*Surface,type=ELEMENT,name="+m_NElData.SurfaceInpData.strSurfaceName+"\n";
    file->write(data.toAscii());
    for (int i=0; i<nSize; i++){
        data = m_NElData.SurfaceInpData.strData.at(i)+"\n";
        file->write(data.toAscii());
    }*/
    file->write("** \n");
	/*for (int i=0;i<m_nodeList.size();i++)
	{
		data = "** Section: " + m_nodeList.at(i).strCommentaryName + "\n"+
			"*" + m_nodeList.at(i).strSectionStyle+",elset="+m_nodeList.at(i).strelset+",material="+m_nodeList.at(i).StrMaterial+"\n"+
			", \n";
		file->write(data.toAscii());
	}
	file->write("** \n");*/
    return true;
}

//д�뵥��M_SectionInp��
bool CRWManage::WriteSectionInpFile(QFile *file, QList<SectionInpS> m_nodeList)
{
    QString data;//="*Node,Material";
    for (int i=0;i<m_nodeList.size();i++)
    {
        data = "** Section: " + m_nodeList.at(i).strCommentaryName + "\n"+
            "*" + m_nodeList.at(i).strSectionStyle+",elset="+m_nodeList.at(i).strelset+",material="+m_nodeList.at(i).StrMaterial+"\n"+
            ", \n";
        file->write(data.toAscii());
    }
    file->write("** \n");
    return true;
}

//д��M_MaterialInp��
bool CRWManage::WriteMaterialInpFile(QFile *file, QList<MaterialInpS> m_nodeList, QList<SectionInpS> m_nodeList1,QList<HPPhyInpS> m_nodeHPPhyList)
{
    QString data;//="*Node,Material";
	QString PartType;
    int nCnt = m_nodeList.size();
    if (nCnt>0){
        data = "** \n ** "+
               m_nodeList.at(0).strCommentary + "\n ** \n";
        file->write(data.toAscii()); 
    }

    for (int i=0; i<nCnt; i++)
    {
        data ="*Material, name=" + m_nodeList.at(i).strMaterialName+"\n";
		PartType=m_nodeList1.at(i).strPartType;
        
        file->write(data.toAscii());
        int DateVectorNum = m_nodeList.at(i).DateVectorNum;
        for (int jj=0; jj<DateVectorNum; jj++)
        {
			QString strName=m_nodeList.at(i).strMaterialStyleName.at(jj).toLower();
			if (strName=="specificheat")
				data="*specific heat";//+"\n";
			else if (strName=="metalpowder")
				data="*metal powder";//+"\n";
			else
				data="*"+strName;//+"\n";

			if(strName=="plastic"&&PartType=="����"||strName=="plastic"&&PartType=="����")
				continue;
			
            if (m_nodeList.at(i).strMaterialStyleParam.size()>0&&strName!="ratedependent"&&strName!="damage"){
                if (m_nodeList.at(i).strMaterialStyleParam.at(jj)!=""){
                    data+= ","+m_nodeList.at(i).strMaterialStyleParam.at(jj);
                }
            }
			if(data=="*ratedependent")data="*rate-dependentplastic";
            data+="\n";
            file->write(data.toAscii());
            
			if (strName=="metal powder"|| strName=="metalpowder"){//������ĩ
				data = m_nodeList.at(i).strMetalPowerData.join(",")+"\n";
				file->write(data.toAscii());
				data="*powder hardening\n";
				file->write(data.toAscii());
				QStringList *tt= m_nodeList.at(i).strData[jj].at(0);
				for (int kk=0;kk<m_nodeList.at(i).strData[jj].count();kk++){
					data=(tt++)->join(",")+"\n";
					file->write(data.toAscii());
				 } 
			}else if(strName=="ratedependent"){
				if(m_nodeList.at(i).strMaterialStyleParam.at(jj).toLower()=="arrhenius")data="2,200e9,0.3,";
				else data="1,200e9,0.3,";
				QStringList *tt= m_nodeList.at(i).strData[jj].at(0);
				for (int kk=0;kk<m_nodeList.at(i).strData[jj].count();kk++){
					if(kk!=m_nodeList.at(i).strData[jj].count()-1)data+=(tt++)->at(1)+",";
					else data+=(tt++)->at(1);
				} 
				int JJ;
                JJ=m_nodeList.at(i).strMaterialStyleParam2.size();
                if(JJ!=0)data+=","+m_nodeList.at(i).strMaterialStyleParam2.at(0);
				data+=+"\n";
				file->write(data.toAscii());
			}else if(strName=="dynamicrecrystallization"){
				QStringList *tt= m_nodeList.at(i).strData[jj].at(0);
				data="";
				for (int kk=0;kk<m_nodeList.at(i).strData[jj].count();kk++){
					if(kk==5)data+="0,0,";
					if(kk!=m_nodeList.at(i).strData[jj].count()-1)data+=(tt++)->at(1)+",";
					else data+=(tt++)->at(1);
					if(kk==5)data+="\n";
					if(kk==13)data+="\n";
				} 
				data+=+"\n";
				file->write(data.toAscii());
			}else if(strName=="damage"){
				data="";
				if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="NormalizedCL")data="1,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="CockroftLatham")data="2,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="McClintock")data="3,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="Freudenthal")data="4,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="RiceTracy")data="5,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="Oyane")data="6,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="Oyanenegative")data="7,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="Ayada")data="8,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="Ayadanegative")data="9,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="Osakada")data="10,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="Brozzo")data="11,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="ZhaKuhn")data="12,";
				else if(m_nodeList.at(i).strMaterialStyleParam.at(jj)=="MPSUTS")data="13,";
				QStringList *tt= m_nodeList.at(i).strData[jj].at(0);
				for (int kk=0;kk<m_nodeList.at(i).strData[jj].count();kk++){
					if(kk!=m_nodeList.at(i).strData[jj].count()-1)data+=(tt++)->at(1)+",";
					else data+=(tt++)->at(1);
				} 
				data+=+"\n";
				file->write(data.toAscii());
			}else{//
				 // file->write(data.toAscii());
				  QStringList *tt= m_nodeList.at(i).strData[jj].at(0);
				  for (int kk=0;kk<m_nodeList.at(i).strData[jj].count();kk++){
					  data=(tt++)->join(",")+"\n";
					  if(m_nodeHPPhyList.size()>0){
						  QString stMCF=m_nodeHPPhyList.at(0).strMassScalingFactor;
						  if(strName.toLower()=="density"&&stMCF!="1"&&stMCF!="1.0"&&stMCF!=""){
							  float Original;
							  QString Current,Original1; 
							  QStringList Original2;
							  Original2=data.split(",");
                              Original1=Original2.at(0);
							  Original=Original1.toFloat();
							  Original=Original*stMCF.toFloat();
							  Current=QString("%1").arg(Original);
							  /*if(Original2.size()>1)data=Current+","+Original2.at(1)+"\n";
							  else if(Original2.size()==1) data=Current+"\n";*/
							  if(Original2.size()>1)data=Current+","+Original2.at(1);
							  else if(Original2.size()==1) data=Current;
						  }
						  if(strName.toLower()=="specificheat"&&stMCF!="1"&&stMCF!="1.0"&&stMCF!=""){
							  float Original;
							  QString Current,Original1; 
							  QStringList Original2;
							  Original2=data.split(",");
                              Original1=Original2.at(0);
							  Original=Original1.toFloat();
							  Original=Original/(stMCF.toFloat());
							  Current=QString("%1").arg(Original);
							  /*if(Original2.size()>1)data=Current+","+Original2.at(1)+"\n";
							  else if(Original2.size()==1) data=Current+"\n";*/
							  if(Original2.size()>1)data=Current+","+Original2.at(1);
							  else if(Original2.size()==1) data=Current;

						  }
					  }
					  file->write(data.toAscii());
				  } 
			}
       }
        file->write("** \n");
     }
    file->write("** \n");
	
    return true;
}

bool CRWManage::WriteContactProperties(QFile *file,QList<OCIInpS> m_OPCS,QList<InpCurveInpS> m_nodeList)							  
{
	QString data;
	int nCnt =m_OPCS.size();
	if (nCnt<=0){
        return false;
    }
	data ="** �Ӵ���Ϣ\n";
    file->write(data.toAscii());
	for (int i=0; i<nCnt; i++){
		QString j33=QString::number(i,10);//������תΪQString
		if(!m_OPCS.at(i).Binding){ 
             data ="*Surface Interaction,name=IntProp-"+j33+"\n";
			 file->write(data.toAscii());
			 data ="*Friction,slip tolerance=0.005\n"+
			 m_OPCS.at(i).FrictionCoefficient+","+m_OPCS.at(i).TangentialStiffness+"\n";
             file->write(data.toAscii());
		     if(m_OPCS.at(i).NormalStiffness!=""){
			    data="*Surface Behavior,pressure-overclosure=LINEAR\n"+m_OPCS.at(i).NormalStiffness+"\n";
			    file->write(data.toAscii());
		     }
		     if(m_OPCS.at(i).HeatExchangeType=="����"&&m_OPCS.at(i).HeatExchangeParameter!=""){
			    data="*Gap Conductance\n"+m_OPCS.at(i).HeatExchangeParameter+"\n";//+",0\n"+"0.,1e-5"+"\n";
			    file->write(data.toAscii());
			 }else if(m_OPCS.at(i).HeatExchangeType=="��ѹ���仯"&&m_OPCS.at(i).AmplitudeName!=""){
			    data="*Gap Conductance\n";
			    file->write(data.toAscii());
				for (int ikl=0;ikl<m_nodeList.size();ikl++)
				{
					QString CurName=m_nodeList.at(ikl).strName;
					if(CurName==m_OPCS.at(i).AmplitudeName){
						int nRowData=m_nodeList.at(ikl).strDataX.size();
						for (int kk=0; kk<nRowData; kk++){
							data = m_nodeList.at(ikl).strDataY.at(kk)+","+
								m_nodeList.at(ikl).strDataX.at(kk)+"\n";
							file->write(data.toAscii());
						}
						break;
					}
				}
			 }else if(m_OPCS.at(i).HeatExchangeType=="���¶ȱ仯"&&m_OPCS.at(i).AmplitudeName!=""){
			    data="*Gap Conductance\n";
			    file->write(data.toAscii());
				for (int ikl=0;ikl<m_nodeList.size();ikl++)
				{
					QString CurName=m_nodeList.at(ikl).strName;
					if(CurName==m_OPCS.at(i).AmplitudeName){
						int nRowData=m_nodeList.at(ikl).strDataX.size();
						for (int kk=0; kk<nRowData; kk++){
							data = m_nodeList.at(ikl).strDataY.at(kk)+",,"+
								m_nodeList.at(ikl).strDataX.at(kk)+"\n";
							file->write(data.toAscii());
						}
						break;
					}
				}
			 }

		     if(m_OPCS.at(i).FrictionGeneratesHeat!=""){
			    float j=1.0-m_OPCS.at(i).FrictionGeneratesHeat.toFloat(); 
				QString data1=QString("%1").arg(j);//������תΪQString
			    data="*Gap Heat Generation\n"+m_OPCS.at(i).FrictionGeneratesHeat+","+data1+"\n";
			    file->write(data.toAscii());
			 } 
		}
	}
	//bool Blot=true;
	//for(int j=0;j<NNU;j++){
	//	if(m_Solvelist.at(j).strNlgeomSTR=="Dynamic,Direct,Explicit")Blot=false;
	//}
	for (int i=0; i<nCnt; i++){
		QString j33=QString::number(i,10);//������תΪQString
		if(!m_OPCS.at(i).Binding){
			data="*contact pair,interaction=IntProp-"+j33+","+m_OPCS.at(i).m_ContactType+"\n"+
				m_OPCS.at(i).Part+","+m_OPCS.at(i).MJ+"\n";
			QString CT;
			if(m_OPCS.at(i).m_ContactType=="����Ӵ�")CT="type=SURFACE TO SURFACE";
			else CT="type=NODE TO SURFACE";
			data="*contact pair,interaction=IntProp-"+j33+","+CT+"\n"+
			         m_OPCS.at(i).Part+","+m_OPCS.at(i).MJ+"\n";
			//if(Blot){
			//	 data="*contact pair,interaction=IntProp-"+j33+","+CT+"\n"+
			//         m_OPCS.at(i).Part+","+m_OPCS.at(i).MJ+"\n";
			//	/* data="*contact pair,interaction=IntProp-"+j33+",type=NODE TO SURFACE\n"+
			//	m_OPCS.at(i).Part+","+m_OPCS.at(i).MJ+"\n";*/
			//}else{
			//	data="*contact pair,interaction=IntProp-"+j33+",type=SURFACE TO SURFACE\n"+
			//		m_OPCS.at(i).Part+","+m_OPCS.at(i).MJ+"\n";
			//}
			file->write(data.toAscii());
		}else if(m_OPCS.at(i).Binding){
			data="*Tie,name=Constraint-"+j33+",adjust=yes\n"+
			m_OPCS.at(i).Part+","+m_OPCS.at(i).MJ+"\n";
			file->write(data.toAscii());
		}
	}
    return true;
}


//д��M_HIPSOLVEInp��
bool CRWManage::WriteHIPSolveInpFile(QFile *file, QList<HIPSolveInpS> m_nodeList)
{
    QString data;//="*Node,Material";
    for (int i=0;i<m_nodeList.size();i++)
    {
		if(m_nodeList.at(i).strNlgeomSTR!="Dynamic,Direct,Explicit"){
        data = "** " + m_nodeList.at(i).strCommentaryName + "\n"+
            "*Step, inc=" + m_nodeList.at(i).strStepInc+", nlgeom="+m_nodeList.at(i).strNlgeom+"\n"+
             "*"+m_nodeList.at(i).strNlgeomSTR+",deltmx= "+m_nodeList.at(i).strDelMX+"\n"+
             m_nodeList.at(i).strInitStep+","+m_nodeList.at(i).strTotalTime+","+m_nodeList.at(i).strMinStep+","+m_nodeList.at(i).strMaxStep+"\n";
		}else if(m_nodeList.at(i).strNlgeomSTR=="Dynamic,Direct,Explicit"){
			data = "** " + m_nodeList.at(i).strCommentaryName + "\n"+
			"*Step, inc=" + m_nodeList.at(i).strStepInc+", nlgeom="+m_nodeList.at(i).strNlgeom+"\n"+
			"*"+m_nodeList.at(i).strNlgeomSTR+"\n"+
				m_nodeList.at(i).strInitStep+","+m_nodeList.at(i).strTotalTime+"\n";
	/*data = "** " + m_nodeList.at(i).strCommentaryName + "\n"+
			"*Step, inc=" + m_nodeList.at(i).strStepInc+", nlgeom="+m_nodeList.at(i).strNlgeom+"\n"+
			"*"+m_nodeList.at(i).strNlgeomSTR+"\n"+
			m_nodeList.at(i).strInitStep+","+m_nodeList.at(i).strTotalTime+","+m_nodeList.at(i).strMinStep+","+m_nodeList.at(i).strMaxStep+"\n";*/

		}
        file->write(data.toAscii());
    }
    file->write("** \n");
 return true;
}
//--����
bool CRWManage::WriteHIPCurveInpFile(QFile *file, QList<InpCurveInpS> m_nodeList)
{
    QString data;
    for (int i=0;i<m_nodeList.size();i++)
    {
        if (m_nodeList.at(i).ThirdStrTime == 1)//����ʱ��д��
        {
            data = "** "+m_nodeList.at(i).strCommentaryName + "\n"+
                "*"+m_nodeList.at(i).strCurveStyle+", name="+m_nodeList.at(i).strName+"\n";
               /*data = "** "+m_nodeList.at(i).strCommentaryName + "\n"+
                "*"+m_nodeList.at(i).strCurveStyle+", name="+m_nodeList.at(i).strName+", time ="+m_nodeList.at(i).strTime+"\n";*/
            file->write(data.toAscii());
            int nRowData=m_nodeList.at(i).strDataX.size();
            for (int kk=0; kk<nRowData; kk++)
            {
             data = m_nodeList.at(i).strDataX.at(kk)+","+
                    m_nodeList.at(i).strDataY.at(kk)+","+"\n";
             file->write(data.toAscii());
            }
        }else if (m_nodeList.at(i).ThirdStrTime == 0){//������ʱ��д��
            data = "** "+m_nodeList.at(i).strCommentaryName + "\n"+
                "*"+m_nodeList.at(i).strCurveStyle+", name="+m_nodeList.at(i).strName+"\n";
            file->write(data.toAscii());
            file->write(data.toAscii());
            int nRowData=m_nodeList.at(i).strDataX.size();
            for (int kk=0; kk<nRowData; kk++){
                data = m_nodeList.at(i).strDataX.at(kk)+","+
                       m_nodeList.at(i).strDataY.at(kk)+","+"\n";
                file->write(data.toAscii());
            }
        } 
    }
    file->write("** \n");
    return true;
}
//--�ȵȾ�ѹ�ƶ�
bool CRWManage::WriteHIPSystemInpFile(QFile *file, QList<HIPSystemInpS> m_nodeList)
{
    QString data;
    for (int i=0;i<m_nodeList.size();i++)
    {
        QString strTmp=m_nodeList.at(i).strStyle;
        if (strTmp == "boundary"){
            data = "** "+m_nodeList.at(i).strCommentaryName + "\n"+
                "*"+strTmp+", amplitude="+m_nodeList.at(i).strAmplitudeName+"\n"+
                m_nodeList.at(i).strPsetName+", 11, 11, "+m_nodeList.at(i).strCofficient+"\n";
            file->write(data.toAscii());       
        }else if(strTmp == "dsload"){
            data = "** "+m_nodeList.at(i).strCommentaryName + "\n"+
                "*"+strTmp+", amplitude="+m_nodeList.at(i).strAmplitudeName+"\n"+
                m_nodeList.at(i).strPsetName+", P, "+m_nodeList.at(i).strCofficient+"\n";
            file->write(data.toAscii());
        }
         
    }
    file->write("** \n");
    return true;

}
//--�ȵȾ�ѹ��ʼ������
bool CRWManage::WriteInitInpFile(QFile *file, QList<InitInpS> m_nodeList)
{
    QString data;
    int nCnt = m_nodeList.size();
    if (nCnt>0)
    {
        data = "** " + m_nodeList.at(0).strCommentaryName + "\n";
        file->write(data.toAscii());
    }
   
    for (int i=0;i<nCnt;i++)
    {
		if(m_nodeList.at(i).strVal!=""){
        data ="*" + m_nodeList.at(i).strInitCondition+", type="+m_nodeList.at(i).strInitTypeName+"\n"+
              m_nodeList.at(i).strPSetName+", "+m_nodeList.at(i).strVal+"\n";
		}else if(m_nodeList.at(i).IncludeName!=""){
			data ="*" + m_nodeList.at(i).strInitCondition+", type="+m_nodeList.at(i).strInitTypeName+"\n"+
              "*INCLUDE,INPUT="+m_nodeList.at(i).IncludeName+"\n";
		}
        file->write(data.toAscii());
    }
    file->write("** \n");
    return true;
}
//--�������
bool CRWManage::WriteVariableInpFile(QFile *file, QList<VariableInpS> m_nodeList)
{
    QString data;
    int nCnt = m_nodeList.size();
  /*  if (nCnt<=0){
        return false;
    }*/
    if (nCnt>0)
    {
        for (int i=0; i<nCnt; i++)
        {
            if (m_nodeList.at(i).strTimeOrFreqName.toLower()=="frequency")
            {
                data = "** " + m_nodeList.at(i).strCommentaryName + "\n"+
                    "*"+ m_nodeList.at(i).strNodeFile+","+ m_nodeList.at(i).strTimeOrFreqName+"="+ m_nodeList.at(i).strTname+"\n"+
                    m_nodeList.at(i).strNInclude+"\n"+
                    "*"+ m_nodeList.at(i).strELFile+","+ m_nodeList.at(i).strTimeOrFreqName+"="+ m_nodeList.at(i).strTname+"\n"+
                    m_nodeList.at(i).strELInclude+"\n";

            }else{
                data = "** " + m_nodeList.at(i).strCommentaryName + "\n"+
                    "*"+ m_nodeList.at(i).strNodeFile+","+ m_nodeList.at(i).strTimeOrFreqName+"=T"+ m_nodeList.at(i).strTname+"\n"+
                    m_nodeList.at(i).strNInclude+"\n"+
                    "*"+ m_nodeList.at(i).strELFile+","+ m_nodeList.at(i).strTimeOrFreqName+"=T"+ m_nodeList.at(i).strTname+"\n"+
                    m_nodeList.at(i).strELInclude+"\n";

            }
            file->write(data.toAscii());
        }
     }
     file->write("*End Step \n");
     return true;
}
//--�߽�����
bool CRWManage::WriteBCInpFile(QFile *file, QList<BCInpS> m_nodeList)
{
    QString data;
    int nCnt = m_nodeList.size();
    if (nCnt<=0){
        return false;
    }
    data = "** " + m_nodeList.at(0).strCommentaryName + "\n";
    file->write(data.toAscii());
    for (int i=0; i<nCnt; i++)
    {
        data ="*" + m_nodeList.at(i).strBoundaryName+"\n"+
            m_nodeList.at(i).strPSetName+", "+m_nodeList.at(i).strURstyle+"\n";
        file->write(data.toAscii());
    }
     file->write("** \n");
     return true;
}
//--���������
bool  CRWManage::WriteResultOIntervalInpFile(QFile *file, QList<VariableInpS> m_VarList,QList<HIPSolveInpS> m_Solvelist)
{
    QString data;
    if (m_Solvelist.size()<=0 ||m_VarList.size()<=0)
    {
        return false;
    }
    data="** ���������\n";
    file->write(data.toAscii());
    //data = "*"+ m_VarList.at(0).strTimeOrFreqName+"= T"+ m_VarList.at(0).strTname+",GENERATE\n"+
    //        "0,"+m_Solvelist.at(0).strTotalTime+","+m_VarList.at(0).strTname+"000\n";//("000-->(s->ms����һǧ��)")
    data = "*TIME POINTS,NAME=T"+ m_VarList.at(0).strTname+",GENERATE\n"+
        "0,"+m_Solvelist.at(0).strTotalTime+","+m_VarList.at(0).strTname+"\n";//+"000\n";//("000-->(s->ms����һǧ��)")  
    file->write(data.toAscii());
    file->write("** \n");
    return true;
}


bool CRWManage::WriteHPTTTCCT(QFile *file, HPTTTInps m_nodeData)
{
    QString data;  int nCnt=0;
    //--д����������
    nCnt=m_nodeData.PhaseCurveInps.strHeadParam.size();
    if (nCnt>0){
        data="*"+m_nodeData.PhaseCurveInps.strName+",type="+m_nodeData.PhaseCurveInps.strPhaseCurveType+"\n";
        file->write(data.toAscii());
    }
    for (int i=0,j=0; i<nCnt; i++) {
        data=m_nodeData.PhaseCurveInps.strHeadParam.at(i)+"\n";
        file->write(data.toAscii());
        if (data.split(",").size()<3) break;
        if (data.split(",").at(2).toInt()!=0){//���ݸ�����Ϊ0
           if (m_nodeData.PhaseCurveInps.strData.size()<=0)continue;
           QStringList strList= m_nodeData.PhaseCurveInps.strData.at(j).split(";");
           j++;
           for (int kk=0; kk<strList.size(); kk++){
               data=strList.at(kk)+"\n";
               file->write(data.toAscii());
           }
        }
    }
    
    //--д����ƽ��
     nCnt=m_nodeData.PhaseBalanceInps.strHeadParam.size();
     if (nCnt>0){
         data="*"+m_nodeData.PhaseBalanceInps.strName+"\n";
         file->write(data.toAscii());
     }
     for (int i=0,j=0; i<nCnt; i++){
         data=m_nodeData.PhaseBalanceInps.strHeadParam.at(i)+"\n";
         file->write(data.toAscii());
         if (data.split(",").size()<2) break;
         if (data.split(",").at(1).toInt()!=0){//���ݸ�����Ϊ0
             if (m_nodeData.PhaseBalanceInps.strData.size()<=0)continue;
             QStringList strList= m_nodeData.PhaseBalanceInps.strData.at(j).split(";");
             j++;
             for (int kk=0; kk<strList.size(); kk++){
                 data=strList.at(kk)+"\n";
                 file->write(data.toAscii());
             }
         }
     }

     //--д���ȴ�����ʽ����
     int strCntrAnalyStyle=0,
         strCntrLatentheat=0,
         strCntrProp=0,
         strCntrPeriod=0,
         strCntrTTT=0;//StrTTT,Ĭ������Ϊ1

     if (m_nodeData.PhaseCtrolInps.strHeadParam!="" && nCnt>0) //��ƽ��������    
     {
         data="*"+m_nodeData.PhaseCtrolInps.strName+"\n";
         file->write(data.toAscii());
         data=m_nodeData.PhaseCtrolInps.strHeadParam+"\n";
         file->write(data.toAscii());
         QStringList strPhaseControl=m_nodeData.PhaseCtrolInps.strHeadParam.split(",");
         if (strPhaseControl.size()==5)
         {
             strCntrAnalyStyle=strPhaseControl.at(0).toInt();
             strCntrLatentheat=strPhaseControl.at(1).toInt();
             strCntrProp=strPhaseControl.at(2).toInt();
             strCntrPeriod=strPhaseControl.at(3).toInt();
             strCntrTTT=strPhaseControl.at(4).toInt();
         }
     }
     
    //--д�뵼�ȡ����ȡ�
    nCnt=m_nodeData.PhasePropCondInps.strHeadParam.size();
    if (nCnt>0){
        data="*"+m_nodeData.PhasePropCondInps.strName+",type="+m_nodeData.PhasePropCondInps.strType+"\n";
        file->write(data.toAscii());
    }

    for (int i=0,j=0; i<nCnt; i++){
        data=m_nodeData.PhasePropCondInps.strHeadParam.at(i)+"\n";
        file->write(data.toAscii());
        if (data.split(",").size()<2) break;
        if (data.split(",").at(1).toInt()!=0){//���ݸ�����Ϊ0
            QStringList strList= m_nodeData.PhasePropCondInps.strData.at(j).split(";");
            j++;
            if (m_nodeData.PhasePropCondInps.strData.size()<=0)continue;
            for (int kk=0; kk<strList.size(); kk++){
                data=strList.at(kk)+"\n";
                file->write(data.toAscii());
            }
        }
    }
    nCnt=m_nodeData.PhasePropSpecInps.strHeadParam.size();
    if (nCnt>0){
        data="*"+m_nodeData.PhasePropSpecInps.strName+",type="+m_nodeData.PhasePropSpecInps.strType+"\n";
        file->write(data.toAscii());
    }

    for (int i=0,j=0; i<nCnt; i++){
        data=m_nodeData.PhasePropSpecInps.strHeadParam.at(i)+"\n";
        file->write(data.toAscii());
        if (data.split(",").size()<2) break;
        if (data.split(",").at(1).toInt()!=0){//���ݸ�����Ϊ0
            if (m_nodeData.PhasePropSpecInps.strData.size()<=0)continue;
            QStringList strList= m_nodeData.PhasePropSpecInps.strData.at(j).split(";");
            j++;
            for (int kk=0; kk<strList.size(); kk++){
                data=strList.at(kk)+"\n";
                file->write(data.toAscii());
            }
        }
    }
    //--д��Ǳ��
    nCnt=m_nodeData.PhaseElateHeatInps.strHeadParam.size();
    if (nCnt>0){
        data="*"+m_nodeData.PhaseElateHeatInps.strName+"\n";
        file->write(data.toAscii());
    }
    for (int i=0;i<nCnt;i++){
        //data=m_nodeData.PhaseElateHeatInps.strHeadParam.at(i)+"\n";
        QStringList dataList=m_nodeData.PhaseElateHeatInps.strHeadParam;
        data=dataList.at(i)+"\n";
        file->write(data.toAscii());
    }
    //--д��Ӳ��
    nCnt=m_nodeData.PhaseHardInps.strHeadParam.size();
    if (nCnt>0){
        data="*"+m_nodeData.PhaseHardInps.strName+"\n";
        file->write(data.toAscii());
    }
    for (int i=0;i<nCnt;i++){
        //data=m_nodeData.PhaseElateHeatInps.strHeadParam.at(i)+"\n";
        QStringList dataList=m_nodeData.PhaseHardInps.strHeadParam;
        data=dataList.at(i)+"\n";
        file->write(data.toAscii());
    }


    //--д������
    nCnt=m_nodeData.IncubatePeriodInps.strHeadParam.size();
    if (nCnt>0){
        data="*"+m_nodeData.IncubatePeriodInps.strName+"\n";
        file->write(data.toAscii());
    }
    for (int i=0,j=0;i<m_nodeData.IncubatePeriodInps.strHeadParam.size();i++){
        data=m_nodeData.IncubatePeriodInps.strHeadParam.at(i)+"\n";
        file->write(data.toAscii());
        if (data.split(",").size()<2) break;
        if (data.split(",").at(1).toInt()!=0){//���ݸ�����Ϊ0
            if (m_nodeData.IncubatePeriodInps.strData.size()<=0)continue;
            QStringList strList= m_nodeData.IncubatePeriodInps.strData.at(j).split(";");
            j++;
            for (int kk=0; kk<strList.size(); kk++){
                data=strList.at(kk)+"\n";
                file->write(data.toAscii());
            }
        }
    }

  
    //--д������ǿ�Ȳ���
    if (m_nodeData.PhaseYSInps.strHeadParam!="")
    {
        data="*"+m_nodeData.PhaseYSInps.strName+"\n";
        file->write(data.toAscii());
        data=m_nodeData.PhaseYSInps.strHeadParam+"\n";
        file->write(data.toAscii());
    }
   
    //--�����ȳߴ�
    if (m_nodeData.PhaseGSInps.strHeadParam!="")
    {
        data="*"+m_nodeData.PhaseGSInps.strName+"\n";
        file->write(data.toAscii());
        data=m_nodeData.PhaseGSInps.strHeadParam+"\n";
        file->write(data.toAscii());
    }
    

    //--д������
    nCnt=m_nodeData.PhaseZBFInps.strHeadParam.size();
    if (nCnt>0){
        data="*"+m_nodeData.PhaseZBFInps.strName+"\n"; 
        file->write(data.toAscii());
    }

    for (int i=0,j=0; i<nCnt; i++){
        data=m_nodeData.PhaseZBFInps.strHeadParam.at(i)+"\n";
        file->write(data.toAscii());
        if (data.split(",").size()<2) break;
        if (data.split(",").at(1)!="0"){//���ݸ�����Ϊ0
            if (m_nodeData.PhaseZBFInps.strData.size()<=0)continue;
            QStringList strList= m_nodeData.PhaseZBFInps.strData.at(j).split(";");
            j++;
            for (int kk=0; kk<strList.size(); kk++){
                data=strList.at(kk)+"\n";
                file->write(data.toAscii());
            }
        }
    }
    
    /*data="*"+m_nodeList.at(i).strName+",type="+m_nodeList.at(i).strPhaseCuraveType+"\n";
    file->write(data.toAscii());*/
    
    
    file->write("** \n");
    return true;
}

//--�ȴ����ƶ�
bool CRWManage::WriteHPSolveVarInpFile(QFile *file, QList<VariableInpS> m_VarList,QList<HPSolveInpS> m_Solvelist,
	QList<HPPhyInpS> m_nodeHPPhyList,QList<HPSystemRInpS> m_nodeRList,QList<HPSystemFInpS> m_nodeFList,
	QList<HIPSystemInpS> m_nodeHIPsysList,QList<FSystem> m_OPFS,QList<BCInpS> m_nodeList,QList<HPSystemFInpS> m_nodeFList2,QList<HPSystemRInpS> m_nodeRList2)
{
    QString data;
    for (int i=0;i<m_nodeHPPhyList.size();i++){
        data = "*"+m_nodeHPPhyList.at(i).strStyle+",absolute zero="+m_nodeHPPhyList.at(i).strAbsZero+",stefan boltzmann="+
            m_nodeHPPhyList.at(i).strStefanBoltzman+"\n";
        file->write(data.toAscii());
    }
    int iStepSize=m_Solvelist.size();
    double dTotalTime=0.0;
	for (int kk=0; kk<iStepSize; kk++){
		dTotalTime=m_Solvelist.at(kk).strTotalTime.simplified().toDouble();
		if (iStepSize>0 ||m_VarList.size()>0) 
		{
			data="** ���������\n";
			file->write(data.toAscii());
			data = "*TIME POINTS,NAME=T"+QString("%1").arg(kk)+",GENERATE\n"+
				"0,"+QString("%1").arg(dTotalTime)+","+ m_Solvelist.at(kk).TimeFrequencyInc+"\n";
			file->write(data.toAscii());
		}
	}

	if(m_OPFS.size()>0){
		int Number=m_OPFS.size();
		int FNum=m_nodeFList2.size();
		int RNum=m_nodeRList2.size();
		int FNum2=m_nodeFList.size();
		int RNum2=m_nodeRList.size();
		m_nodeFList.clear();
		m_nodeRList.clear();
		for(int i=0;i<Number;i++){
			QString Str=m_OPFS.at(i).TBName;
			if(Str=="��")continue;
			for(int j=0;j<FNum;j++){
				if(Str==m_nodeFList2.at(j).strCommentaryName){
					QString stb="Step"+QString::number(i+1,10);
					HPSystemFInpS FInps;
					FInps=m_nodeFList2.at(j);
					if(i!=0)FInps.strOP="new";
					FInps.strStep=stb;
					m_nodeFList2.replace(j,FInps);
					m_nodeFList.append(FInps);
				}
			}
			for(int j1=0;j1<RNum;j1++){
				if(Str==m_nodeRList2.at(j1).strCommentaryName){
					QString stb="Step"+QString::number(i+1,10);
					HPSystemRInpS RInps;
					RInps=m_nodeRList2.at(j1);
					if(i!=0)RInps.strOP="new";
					RInps.strStep=stb;
					m_nodeRList2.replace(j1,RInps);
					m_nodeRList.append(RInps);
				}
			}
		}
	}
	for(int i=0;i<m_nodeFList.size();i++){
		if(m_nodeFList.at(i).strOP=="new"&&m_nodeFList.at(i).strEnvirTemprat=="")m_nodeFList.takeAt(i);
	}
	for(int i=0;i<m_nodeRList.size();i++){
		if(m_nodeRList.at(i).strOP=="new"&&m_nodeRList.at(i).strEnvirTemprat=="")m_nodeRList.takeAt(i);
	}
    //--����step���б��棻
    int iStep=0;
    for (iStep;iStep<iStepSize;iStep++)
    {
        //--1.���������
        data="**--------------------------------------------------\n";
        file->write(data.toAscii());
		if(m_Solvelist.at(iStep).strNlgeomSTR!="Dynamic,Direct,Explicit"){
        data = "** Step - "+QString("%1").arg(iStep+1) + m_Solvelist.at(iStep).strCommentaryName + "\n"+
            "*Step, inc=" + m_Solvelist.at(iStep).strStepInc+", nlgeom="+m_Solvelist.at(iStep).strNlgeom+"\n"+
            "*"+m_Solvelist.at(iStep).strNlgeomSTR+",deltmx= "+m_Solvelist.at(iStep).strDelMX+"\n"+
            m_Solvelist.at(iStep).strInitStep+","+m_Solvelist.at(iStep).strTotalTime+","+m_Solvelist.at(iStep).strMinStep+","+m_Solvelist.at(iStep).strMaxStep+"\n";
		}else if(m_Solvelist.at(iStep).strNlgeomSTR=="Dynamic,Direct,Explicit"){
			data ="*Step, inc=" + m_Solvelist.at(iStep).strStepInc+", nlgeom="+m_Solvelist.at(iStep).strNlgeom+"\n"+
				"*"+m_Solvelist.at(iStep).strNlgeomSTR+"\n"+
				m_Solvelist.at(iStep).strInitStep+","+m_Solvelist.at(iStep).strTotalTime+"\n";
/*data = "** Step - "+QString("%1").arg(iStep+1) + m_Solvelist.at(iStep).strCommentaryName + "\n"+
				"*Step, inc=" + m_Solvelist.at(iStep).strStepInc+", nlgeom="+m_Solvelist.at(iStep).strNlgeom+"\n"+
				"*"+m_Solvelist.at(iStep).strNlgeomSTR+"\n"+
				m_Solvelist.at(iStep).strInitStep+","+m_Solvelist.at(iStep).strTotalTime+","+m_Solvelist.at(iStep).strMinStep+","+m_Solvelist.at(iStep).strMaxStep+"\n";*/

		}
        file->write(data.toAscii());
        
        file->write("** \n");
        ///*****luo
		/*data="*"+m_nodeList.at(0).strBoundaryName+",op=new\n";
		file->write(data.toAscii());
		for (int j=0;j<m_nodeList.size();j++){
			data=m_nodeList.at(j).strPSetName+","+m_nodeList.at(j).strURstyle+"\n";
			file->write(data.toAscii());
		}*/

		
		for(int j1=0;j1<m_OPFS.size();j1++){
			int JJ=m_OPFS.at(j1).ForgingSystemName.toInt();//���ַ���ת��Ϊ����
			if(JJ==iStep&&m_OPFS.at(j1).NodeSetName.at(j1)!="�½�"&&m_OPFS.at(j1).MotionParameterType!="��"&&
				m_OPFS.at(j1).Direction!="No"&&m_OPFS.at(j1).SportType!="No"){
                //data="*Boundary,op=new";
				data="*Boundary";
				if(m_OPFS.at(j1).MotionParameterType=="��ʱ��仯"){
					data+=",amplitude="+m_OPFS.at(j1).CurveName;
				}
				if(m_OPFS.at(j1).SportType=="�ٶ�"){
					data+=",type=velocity";
				}
				QString stD;
				if(m_OPFS.at(j1).Direction=="X"){
					stD=",1,1,";
				}else if(m_OPFS.at(j1).Direction=="Y"){
					stD=",2,2,";
				}else if(m_OPFS.at(j1).Direction=="Z"){
					stD=",3,3,";
				}
				if(m_OPFS.at(j1).MotionParameterType=="��ʱ��仯"){
					data+="\n"+m_OPFS.at(j1).NodeSetName.at(j1)+stD+m_OPFS.at(j1).ScalingFactor+"\n";
				}else{
				    data+="\n"+m_OPFS.at(j1).NodeSetName.at(j1)+stD+m_OPFS.at(j1).MotionParameter+"\n";
				}
				file->write(data.toAscii());
			}
		}
        //--2(or1)�ȵȾ�ѹ�ƶ����ѹ�����غ�
        for (int i=0;i<m_nodeHIPsysList.size();i++)
        {
            QString strTmp=m_nodeHIPsysList.at(i).strStyle;
            if (strTmp == "Boundary"){
                data = "** "+m_nodeHIPsysList.at(i).strCommentaryName + "\n"+
                    "*"+strTmp+", amplitude="+m_nodeHIPsysList.at(i).strAmplitudeName+"\n"+
                    m_nodeHIPsysList.at(i).strPsetName+", 11, 11, "+m_nodeHIPsysList.at(i).strCofficient+"\n";
                if (m_nodeHIPsysList.at(i).strAmplitudeName.toLower()!="�½�"){//����û���½�������Ϊû������
                    file->write(data.toAscii());
                }                    
            }else if(strTmp == "Dsload"){
                data = "** "+m_nodeHIPsysList.at(i).strCommentaryName + "\n"+
                    "*"+strTmp+", amplitude="+m_nodeHIPsysList.at(i).strAmplitudeName+"\n"+
                    m_nodeHIPsysList.at(i).strPsetName+", P, "+m_nodeHIPsysList.at(i).strCofficient+"\n";
                if (m_nodeHIPsysList.at(i).strAmplitudeName.toLower()!="�½�"){//����û���½�������Ϊû������
                    file->write(data.toAscii());
                } 
            }
        }
        file->write("** \n");
        ////--2(or2)film & radiate--�ȴ������
        int nCnt = m_nodeFList.size();
        if(nCnt>0&&iStep!=0){
			data="*film,op=new";
			data+="\n";
			file->write(data.toAscii());
		}
		if (nCnt>0){ 
            for (int kk=0;kk<nCnt;kk++){ 
                QString strTmp=  m_nodeFList.at(kk).strStep;//.remove("step", Qt::CaseInsensitive);//.toInt();
                int iCurStep = strTmp.remove("step",Qt::CaseInsensitive).toInt()-1;
                if (iStep==iCurStep) {
                    data = "*film";
                    QString strTmp=m_nodeFList.at(kk).strAmplitudeName;
                    if (strTmp != ""){data+=",amplitude="+strTmp;}
                    strTmp=m_nodeFList.at(kk).strFilmRadiateAmpName;
                    if (strTmp!=""){data+=",film amplitude="+strTmp;}
                    strTmp=m_nodeFList.at(kk).strOP;
                    if (strTmp!=""){
                        strTmp="new";
                        data+=",op="+strTmp;
                        }
                    data+="\n";
                    file->write(data.toAscii());

					//data="**THName="+m_nodeFList.at(kk).strCommentaryName+"\n";
					//file->write(data.toAscii());

                    QStringList strListName =m_nodeFList.at(kk).strElsetName.split(",",QString::SkipEmptyParts);
                    QStringList strListSurfaceNum=m_nodeFList.at(kk).strSurfaceNum.split(",",QString::SkipEmptyParts);
                    for (int tt=0;tt<strListName.size();tt++){
                        data = strListName.at(tt)+",F"+strListSurfaceNum.at(tt).simplified()+","+
                            m_nodeFList.at(kk).strEnvirTemprat+","+m_nodeFList.at(kk).strCofficient+"\n";
                        file->write(data.toAscii());  
                    }
                }
            }
        }

        nCnt = m_nodeRList.size();
    	if(nCnt>0&&iStep!=0){
			data="*radiate,op=new";
			data+="\n";
			file->write(data.toAscii());
		}
		//if (nCnt>0 && iStep<nCnt){
		if (nCnt>0){
            for (int kk=0;kk<nCnt;kk++){ 
				

                QString strTmp=  m_nodeRList.at(kk).strStep;//.remove("step", Qt::CaseInsensitive);//.toInt();
                int iCurStep = strTmp.remove("step",Qt::CaseInsensitive).toInt()-1;
                if (iStep==iCurStep){
                    data = "*radiate";
                    QString strTmp=m_nodeRList.at(kk).strAmplitudeName;
                    if (strTmp != ""){data+=",amplitude="+strTmp;}
                    strTmp=m_nodeRList.at(kk).strFilmRadiateAmpName;
                    if (strTmp!=""){data+=",radiate amplitude="+strTmp;}
                    strTmp=m_nodeRList.at(kk).strOP;
                    if (strTmp!=""){
                        strTmp="new";
                        data+=",op="+strTmp;
                        }
                    data+="\n";
                    file->write(data.toAscii());

					//data="**THName="+m_nodeRList.at(kk).strCommentaryName+"\n";
					//file->write(data.toAscii());

                    QStringList strListName =m_nodeRList.at(kk).strElsetName.split(",",QString::SkipEmptyParts);
                    QStringList strListSurfaceNum=m_nodeRList.at(kk).strSurfaceNum.split(",",QString::SkipEmptyParts);
                    for (int tt=0;tt<strListName.size();tt++){
                        data = strListName.at(tt)+",R"+strListSurfaceNum.at(tt).simplified()+","+
                            m_nodeRList.at(kk).strEnvirTemprat+","+m_nodeRList.at(kk).strCofficient+"\n";
                        file->write(data.toAscii());  
                    } 
              }
           }
        }
        file->write("** \n");
        
        //3.�������
		nCnt = m_VarList.size();
		if (nCnt>0 ){//���step�£�ÿ��step�����ͬ ����
			if (m_VarList.at(iStep).strTimeOrFreqName.toLower()=="frequency"){
				data = "** " + m_VarList.at(iStep).strCommentaryName + "\n"+
					"*"+ m_VarList.at(iStep).strNodeFile+","+ m_VarList.at(iStep).strTimeOrFreqName+"="+m_Solvelist.at(iStep).TimeFrequencyInc+"\n"+
					m_VarList.at(iStep).strNInclude+"\n";
				file->write(data.toAscii());
				if (m_VarList.at(iStep).bELFileShow){
					data=  "*"+ m_VarList.at(iStep).strELFile+","+ m_VarList.at(iStep).strTimeOrFreqName+"="+m_Solvelist.at(iStep).TimeFrequencyInc+"\n"+
						m_VarList.at(iStep).strELInclude+"\n";
					file->write(data.toAscii());
				}
				if(m_VarList.at(iStep).Igforce=="1"){
                    int IJK=m_VarList.at(iStep).NodeSet1.size();
					for(int i=0;i<IJK;i++){
						data=  "*NODE PRINT,NSET="+ m_VarList.at(iStep).NodeSet1.at(i)+","+ m_VarList.at(iStep).strTimeOrFreqName+"="+m_Solvelist.at(iStep).TimeFrequencyInc+","+
							"TOTALS=ONLY\n";
						file->write(data.toAscii());
					}
				}
			}else{
				data = "** " + m_VarList.at(iStep).strCommentaryName + "\n"+
					"*"+ m_VarList.at(iStep).strNodeFile+","+ m_VarList.at(iStep).strTimeOrFreqName+"=T"+ QString("%1").arg(iStep)+"\n"+
					m_VarList.at(iStep).strNInclude+"\n";
				file->write(data.toAscii());
				if (m_VarList.at(iStep).bELFileShow){
					data="*"+ m_VarList.at(iStep).strELFile+","+ m_VarList.at(iStep).strTimeOrFreqName+"=T"+ QString("%1").arg(iStep)+"\n"+
						m_VarList.at(iStep).strELInclude+"\n";
					file->write(data.toAscii());
				}
				if(m_VarList.at(iStep).Igforce=="1"){
					int IJK=m_VarList.at(iStep).NodeSet1.size();
					for(int i=0;i<IJK;i++){
						data=  "*NODE PRINT,NSET="+ m_VarList.at(iStep).NodeSet1.at(i)+","+ m_VarList.at(iStep).strTimeOrFreqName+"=T"+ QString("%1").arg(iStep)+","+
							"TOTALS=ONLY\n";
						file->write(data.toAscii());
					}
				}
			}
		}
    file->write("*End Step \n"); 
   }
   return true;
}
bool CRWManage::WriteHPSolveVarInpFile(QFile *file, QList<VariableInpS> m_VarList,QList<HPSolveInpS> m_Solvelist, QList<HPPhyInpS> m_nodeHPPhyList)
{
    QString data;
    for (int i=0;i<m_nodeHPPhyList.size();i++){
        data = "*"+m_nodeHPPhyList.at(i).strStyle+",absolute zero="+m_nodeHPPhyList.at(i).strAbsZero+",stefan boltzmann="+
                m_nodeHPPhyList.at(i).strStefanBoltzman+"\n";
         file->write(data.toAscii());
    }

    if (m_Solvelist.size()<=0 ||m_VarList.size()<=0) return false;
    data="** ���������\n";
    file->write(data.toAscii());
    //data = "*"+ m_VarList.at(0).strTimeOrFreqName+"= T"+ m_VarList.at(0).strTname+",GENERATE\n"+
    //        "0,"+m_Solvelist.at(0).strTotalTime+","+m_VarList.at(0).strTname+"000\n";//("000-->(s->ms����һǧ��)")
    //data = "*TIME POINTS,NAME=T"+ m_VarList.at(0).strTname+",GENERATE\n"+
    //    "0,"+m_Solvelist.at(0).strTotalTime+","+m_VarList.at(0).strTname+"000\n";//("000-->(s->ms����һǧ��)")  
    //file->write(data.toAscii());
    data = "*TIME POINTS,NAME=T"+ m_VarList.at(0).strTname+",GENERATE\n"+
        "0,"+m_Solvelist.at(0).strTotalTime+","+m_VarList.at(0).strTname+"\n";//+"000\n";//("000-->(s->ms����һǧ��)")  
    file->write(data.toAscii());


    for (int i=0;i<m_Solvelist.size();i++)
    {
		if(m_Solvelist.at(i).strNlgeomSTR!="Dynamic,Direct,Explicit"){
        data = "** " + m_Solvelist.at(i).strCommentaryName + "\n"+
            "*Step, inc=" + m_Solvelist.at(i).strStepInc+", nlgeom="+m_Solvelist.at(i).strNlgeom+"\n"+
            "*"+m_Solvelist.at(i).strNlgeomSTR+",deltmx= "+m_Solvelist.at(i).strDelMX+"\n"+
            m_Solvelist.at(i).strInitStep+","+m_Solvelist.at(i).strTotalTime+","+m_Solvelist.at(i).strMinStep+","+m_Solvelist.at(i).strMaxStep+"\n";
		}else if(m_Solvelist.at(i).strNlgeomSTR=="Dynamic,Direct,Explicit"){
			data ="*Step, inc=" + m_Solvelist.at(i).strStepInc+", nlgeom="+m_Solvelist.at(i).strNlgeom+"\n"+
				"*"+m_Solvelist.at(i).strNlgeomSTR+"\n"+
				m_Solvelist.at(i).strInitStep+","+m_Solvelist.at(i).strTotalTime+"\n";
               /*data = "** " + m_Solvelist.at(i).strCommentaryName + "\n"+
				"*Step, inc=" + m_Solvelist.at(i).strStepInc+", nlgeom="+m_Solvelist.at(i).strNlgeom+"\n"+
				"*"+m_Solvelist.at(i).strNlgeomSTR+"\n"+
				m_Solvelist.at(i).strInitStep+","+m_Solvelist.at(i).strTotalTime+","+m_Solvelist.at(i).strMinStep+","+m_Solvelist.at(i).strMaxStep+"\n";*/

		}
        file->write(data.toAscii());
    }
    file->write("** \n");

    int nCnt = m_VarList.size();
    if (nCnt<=0) return false;
   
    for (int i=0; i<nCnt; i++)
    {
        data = "** " + m_VarList.at(i).strCommentaryName + "\n"+
            "*"+ m_VarList.at(i).strNodeFile+","+ m_VarList.at(i).strTimeOrFreqName+"="+ m_VarList.at(i).strTname+"\n"+
            m_VarList.at(i).strNInclude+"\n"+
            "*"+ m_VarList.at(i).strELFile+","+ m_VarList.at(i).strTimeOrFreqName+"="+ m_VarList.at(i).strTname+"\n"+
            m_VarList.at(i).strELInclude+"\n";
        file->write(data.toAscii());
    }
    file->write("*End Step \n");

    return true;
}
//------------------------------------------------------------
//--Read Inp From .INP file
//------------------------------------------------------------

bool CRWManage::ReadSectionInpFile(QFile *file,QString FileName)
{
    //QStringList lists = fileNamePath.split(".");
    //QString fileFormat = lists.back();
    int ret;
   
    QString line,strTmp,PartName;
    PartName="";
    QTextStream inText(file);
    
    NodeInpS TmpNodeInpS;
    ELInpS TmpElInpS;
    NSetInpS TmpNsetInps;
    ElSetInpS TmpElSetInps;
    SurfaceInpS TmpSurfaceInps;
    QList<NodeELSetInps> TmpMeshInps;
    GmshInpS TmpGmshInps;
    QList<SectionInpS> TmpSectionInps;
    QList<MaterialInpS> TmpMaterialInps;
    QList<HIPSolveInpS> TmpSolveInps;
    QList<InpCurveInpS> TmpCurveInps;
    QList<HIPSystemInpS> TmpHIPSystemInps;
    QList<BCInpS> TmpBCInps;
    QList<InitInpS> TmpInitInps;
    QList<VariableInpS> TmpVariableInps;

    //--�ȴ���
    //QList<HPSystemInpS> TmpHPSystemInps;
    QList<HPSystemFInpS> TmpHPSystemFInps;
    QList<HPSystemRInpS> TmpHPSystemRInps;
	//QList<HPTTTInps> TmpHPTTTInps;

	QList<HPphaseCurveInps> TmpHPphaseCurveInps;
	QList<HPphaseBalanceInps> TmpHPphaseBalanceInps;
	QList<HPphaseCtrolInps>	TmpHPphaseCtrolInps;
	QList<HPincubatePeriodInps> TmpHPincubatePeriodInps;
	QList<HPphaseElateHeatInps> TmpHPphaseElateHeatInps;
    QList<HPphaseHardInps> TmpHPphaseHardInps;
	QList<HPPhasePropSpecInps> TmpHPPhasePropSpecInps;
	QList<HPPhasePropCondInps> TmpHPPhasePropCondInps;
	QList<HPPhaseYSInps> TmpHPPhaseYSInps;
	QList<HPPhaseGSInps> TmpHPPhaseGSInps;
	QList<HPPhaseZBFInps> TmpHPPhaseZBFInps;

	//�Ӵ�
	QList<CPPInInp> TmpCPPInInp;
	QList<CPInInp> TmpCPInInp;
	QList<CPTieInp> TmpCPTieInp;
	//�����ƶ�
	QList<FSystem> TmpFSystem;
	QList<QString> IncOutput;
	//����
	Gravity_Inp TmpGravityInps;

    int NSetDataCnt=0;
    //SetionInpS TmpSectionInps;
    int InpStyle=0; //0->inital
                    //1->gmsh inp
                    //2->normal inp
    Information_Widget *infoW = Information_Widget::GetInstance();
    QDateTime date = QDateTime::currentDateTime();
    QString strTime;
    strTime.append(date.toString("hh:mm:ss yy.MM.dd"));
    infoW->ShowInformation("Start Time: "+strTime);
    infoW->ShowInformation("Read inp file... ");
	while(!inText.atEnd()) {
   
       //ret = ReadNextLine(file,line);
       ret=ReadNextLine(inText, line); 
       if(ret==1&&line=="*heading")//--gmsh data -->*Heading 
       {
		   PartName=FileName;
           InpStyle=1;
           ret=ReadNextLine(inText, line);
           while(ret!=-1){//Ѱ��*node���б���
               QStringList strList = line.split(",");
               if (ret!=1){
                   ret=ReadNextLine(inText, line);
                   strList = line.split(",");
               }
               if(strList.at(0)=="*node"){//*Node Read
				   TmpGmshInps.NodeInpData.PartName=PartName;
                   ret = ReadNextLine(inText, line);
                   while(0==ret){
                   TmpGmshInps.NodeInpData.strData.push_back(line);
                   ret = ReadNextLine(inText, line); 
               }    
            }
            strList=line.split(",");
            if (strList.at(0)=="*element"){//Element
                ELInpS tmpEl;
                QString strTmp01;
                for (int ttt=1;ttt<strList.size();ttt++){
                    strTmp =strList.at(ttt).simplified();
                    if (strTmp.contains("type=",Qt::CaseInsensitive) ) {
                        strTmp01=strTmp.remove("type=", Qt::CaseInsensitive).simplified();
                        tmpEl.strELType=strTmp01;
                    }else if(strTmp.contains("elset=",Qt::CaseInsensitive)){
                        tmpEl.strELSET=strTmp.remove("elset=", Qt::CaseInsensitive).simplified();
                    }
                }
                ret=ReadNextLine(inText, line);
                while(0==ret||ret==-1){
                    tmpEl.strData.push_back(line);
					if(ret==-1)break;
                    ret = ReadNextLine(inText, line); 
                }
				tmpEl.PartName=PartName;
                TmpGmshInps.gmshELData.append(tmpEl);
            }else if(strList.at(0)=="*elset"){//ELSET
                int nElsetSize = strList.size();
                strTmp = strList.at(1);
                TmpGmshInps.gmshELSetData.strElSetName<<strTmp.remove("elset=", Qt::CaseInsensitive).simplified();
                if (nElsetSize == 2){TmpGmshInps.gmshELSetData.strElSetStyle<<"";
                }else if (nElsetSize == 3){TmpGmshInps.gmshELSetData.strElSetStyle<<"generate";
                }

                ret=ReadNextLine(inText, line);
                QString curElData;curElData.clear();
                while(0==ret){
                    if (curElData ==""){//����ֱ�Ӹ�ֵ
                        curElData=line;
                    }else{//�����н�������
                        curElData +=","+line;
                    }
                    ret = ReadNextLine(inText ,line); 
                }
				TmpGmshInps.gmshELSetData.PartName=PartName;
                TmpGmshInps.gmshELSetData.strData<<curElData;
            }else if (strList.at(0)=="*nset"){ //NSET
                int nNsetSize = strList.size();
                strTmp = strList.at(1);
                TmpGmshInps.gmshNSetData.strNSetName<<strTmp.remove("nset=", Qt::CaseInsensitive).simplified();
                if (nNsetSize==2){TmpGmshInps.gmshNSetData.strNSetStyle<<"";
                }else if (nNsetSize == 3){TmpGmshInps.gmshNSetData.strNSetStyle<<"generate";
                }
                ret=ReadNextLine(inText, line);
                QStringList tmpNsetData;
                while(0==ret){
                    tmpNsetData<<line;
                    ret = ReadNextLine(inText ,line); 
                }
                if (ret==-1){
                    tmpNsetData<<line;
                }
				TmpGmshInps.gmshNSetData.PartName=PartName;
                TmpGmshInps.gmshNSetData.strData.push_back(tmpNsetData);
            }else{//����ʶ�Ĺؼ���
                ret=ReadNextLine(inText, line);
            }//strList.at(0).toLower()=="*element"
            
          }//heading
            ret=ReadNextLine(inText, line);
       }

       //--�����ʶ���Inp data��
       while (ret!=-1){//*Element
           QStringList strList=line.split(",");
		   if (strList.at(0)=="**partstart"){
			   int nSize = strList.size();
			   for(int i=1;i<nSize;i++){
				   strTmp = strList.at(i);
				   if(strTmp.contains("name=")){
					   PartName=strTmp.remove("name=", Qt::CaseInsensitive).simplified();
				   }
			   }
               ret = ReadNextLine(inText, line);
		   }else if (strList.at(0)=="*node"){//*Node Read
               if (!TmpNodeInpS.strData.empty()&&TmpElInpS.strELType==""){//element��û�����ݣ�˵��Ϊ��һ�β����б���
                   NodeELSetInps tmpD;
                   tmpD.NodeInpData=TmpNodeInpS;
                   tmpD.ELInpData=TmpElInpS;
                   tmpD.NSetInpData=TmpNsetInps;
                   tmpD.ElSetInpData=TmpElSetInps;
                   tmpD.SurfaceInpData=TmpSurfaceInps;
                   TmpMeshInps.append(tmpD);
                   TmpNodeInpS.clear();TmpElInpS.clear();TmpNsetInps.clear();TmpElSetInps.clear();TmpSurfaceInps.clear();
               }
               ret = ReadNextLine(inText, line);
			   TmpNodeInpS.PartName=PartName;
               while(0==ret){
                   TmpNodeInpS.strData.push_back(line);
                   ret = ReadNextLine(inText, line); 
               }
               
		   }else if (strList.at(0)=="*element"){//Element
			   int NumberEl=0;
			   int nNsetSize = strList.size();
			   TmpElInpS.PartName=PartName;
			   for(int i=1;i<nNsetSize;i++){	
				   strTmp = strList.at(i);
				   if(strTmp.contains("type="))TmpElInpS.strELType=strTmp.remove("type=", Qt::CaseInsensitive).simplified();
			   }
               ret=ReadNextLine(inText, line);
               while(0==ret){
                   NumberEl=NumberEl+1;
                   TmpElInpS.strData.push_back(line);
                   ret = ReadNextLine(inText, line); 
               }
			   
			   if(NumberEl!=0){
				   TmpElInpS.NumberE.push_back(NumberEl);
                   TmpElInpS.ElementType.push_back( TmpElInpS.strELType);
			   }
		   }else if (strList.at(0)== "*nset"){//*Nset�ڵ㼯��
              int nNsetSize = strList.size();
			  TmpNsetInps.PartName=PartName;
			  for(int i=1;i<nNsetSize;i++){
				  QString Str;
				  Str=strList.at(i);
				  if(Str.contains("nset")){
					  TmpNsetInps.strNSetName<<Str.remove("nset=", Qt::CaseInsensitive).simplified();
				  }else if(Str.contains("generate")){
					  TmpNsetInps.strNSetStyle<<"generate";
              }
			  }
			   if(!line.contains("generate"))TmpNsetInps.strNSetStyle<<"";
              ret=ReadNextLine(inText, line);
              QStringList tmpNsetData;
              while(0==ret){
                  tmpNsetData<<line;
                  ret = ReadNextLine(inText ,line); 
              }
               TmpNsetInps.strData.push_back(tmpNsetData);
		   }else if (strList.at(0)== "*elset"){//Elset��Ԫ����
              int nElsetSize = strList.size();
			  QString Str;
			  TmpElSetInps.PartName=PartName;
			  for(int i=1;i<nElsetSize;i++){
				  Str=strList.at(i);
				  if(Str.contains("elset")){
					  TmpElSetInps.strElSetName<<Str.remove("elset=", Qt::CaseInsensitive).simplified();
				  }else if(Str.contains("generate")){
					  TmpElSetInps.strElSetStyle<<"generate";
              }
			  }
			  if(!line.contains("generate"))TmpElSetInps.strElSetStyle<<"";

              ret=ReadNextLine(inText, line);
              QString curElData;curElData.clear();
              while(0==ret){
                  if (curElData !=""){//����ֱ�Ӹ�ֵ
					  curElData =curElData+","+line;
                  }else{//�����н�������
                      curElData=line;
                  }
                ret = ReadNextLine(inText ,line); 
              }
              TmpElSetInps.strData<<curElData;
		   }else if (strList.at(0)== "*surface"){//Surface���漯��
			   TmpSurfaceInps.PartName=PartName;
               for (int kk=1;kk<strList.size();kk++){
                   strTmp = strList.at(kk);
                   if (strTmp.contains("type",Qt::CaseInsensitive)){
                     TmpSurfaceInps.strSurfaceType=strTmp.remove("type=").simplified();
                   }else if(strTmp.contains("name",Qt::CaseInsensitive)){
                     TmpSurfaceInps.strSurfaceName<<strTmp.remove("name=").simplified();
                   }   
               }
               ret=ReadNextLine(inText, line);
               int iTempCnt=0;
               while(0==ret){
                   iTempCnt++;
                   TmpSurfaceInps.strData<<line;
                   ret = ReadNextLine(inText ,line); 
               }
               TmpSurfaceInps.strSurfaceDataNum<<QString("%1").arg(iTempCnt);
   
           }else if (strList.at(0)=="*solidsection"){
                SectionInpS TmpS;
				TmpS.PartName=PartName;
                TmpS.strSectionStyle ="solidsection";
                strTmp = strList.at(1);
				int nSolid=strList.size();
				for(int i=1;i<nSolid;i++){
					QString Str=strList.at(i);
					if(Str.contains("elset"))TmpS.strelset = Str.remove("elset=").simplified();
					else if(Str.contains("material"))TmpS.StrMaterial=Str.remove("material=").simplified();//material
				}
                TmpSectionInps.append(TmpS);
                ret = ReadNextLine(inText ,line); 
              while (0==ret){
                  ret = ReadNextLine(inText ,line); 
              }
           }else if (strList.at(0)=="*amplitude"){//����
               InpCurveInpS TmpCurve;QStringList strDataX,strDataY,strCuvTmp;

               TmpCurve.strCurveStyle="amplitude";
			   int Namplit;
			   Namplit=strList.size();
			   for(int i=1;i<Namplit;i++){
				   strTmp = strList.at(i);   
				   if(strTmp.contains("name="))TmpCurve.strName=strTmp.remove("name=", Qt::CaseInsensitive).simplified();//����
			   }        
               ret=ReadNextLine(inText, line);
               while (0==ret){
                 strCuvTmp=line.split(",");
                 for (int kk=0;kk<strCuvTmp.size()-1;kk=kk+2){
                     strDataX<<strCuvTmp.at(kk).simplified();
                     strDataY<<strCuvTmp.at(kk+1).simplified();
                 }
                 TmpCurve.strDataX = strDataX;
                 TmpCurve.strDataY = strDataY;
                 ret=ReadNextLine(inText, line);
               }
               TmpCurveInps.append(TmpCurve);
//************************************************************************************************************************
		   }else if (strList.at(0)=="*boundary"){//�߽�����
                  BCInpS TmpBC;
			   if(strList.size()==1)TmpBC.strBoundaryName="boundary";
			   int Nstep=TmpSolveInps.size();
			   if(Nstep>0){
				   TmpBC.StepNumber=QString::number(Nstep-1,10);
				   TmpBC.SolutionName=TmpSolveInps.at(Nstep-1).strProcess;
			   }
			   TmpBC.SportType="λ��";
			   for(int i=1;i<strList.size();i++){
				   QString str;
				   str=strList.at(i);
				   if(str.contains("type=velocity")){
					   TmpBC.SportType="�ٶ�";
				   }else if(str.contains("op=")){
				   }else if(str.contains("amplitude=")){
					   TmpBC.strAmplitudeName=str.remove("amplitude=",Qt::CaseInsensitive).simplified();
					   TmpBC.ParametreType="��ʱ��仯";
				   }
			   }
                   ret=ReadNextLine(inText, line);
			   QStringList BoundaryList0=line.split(",");
			   int Nsss=BoundaryList0.size();
			   if(Nsss<=3||BoundaryList0.at(Nsss-1)=="0")TmpBC.SportType="��";
			   while(ret==0){
                           //TmpSurfaceInps.strData<<line;
                           QStringList BoundaryList=line.split(",");
                           TmpBC.strPSetName=BoundaryList.at(0);
                           if (BoundaryList.size() < 3) { ret=ReadNextLine(inText, line); continue; }
				   TmpBC.strURstyle=BoundaryList.at(1)+","+BoundaryList.at(2);//"1,1;2,2;3,3,11,11"
				   if(BoundaryList.size()>3)TmpBC.Parametre=BoundaryList.at(3);
                           TmpBCInps.append(TmpBC);
						   ret=ReadNextLine(inText, line);
					   }
		   }else if (strList.at(0)=="*dsload" ){
                       HIPSystemInpS TmpSys;
                       TmpSys.strStyle=strList.at(0);
			   for(int i=1;i<strList.size();i++){
				   QString str;
				   str=strList.at(i);
				   if(str.contains("amplitude="))TmpSys.strAmplitudeName=str.remove("amplitude=", Qt::CaseInsensitive).simplified();
			   }
			   int Nstep=TmpSolveInps.size();
			   if(Nstep>0){
				   TmpSys.StepNumber=QString ::number(Nstep-1,10);
				   TmpSys.SolutionName=TmpSolveInps.at(Nstep-1).strProcess;
			   }
			   ret=ReadNextLine(inText, line);
                       while (0==ret){
                           QStringList SysList=line.split(",");
                           TmpSys.strPsetName=SysList.at(0);
                           TmpSys.strCofficient=SysList.at(SysList.size()-1);//���һ��
                           ret=ReadNextLine(inText, line);
                           TmpHIPSystemInps.append(TmpSys);
					   }
		   }else if (strList.at(0)=="*initialconditions"){//��ʼ��
               InitInpS TmpInit;
			   int Ninit;
			   Ninit=strList.size();
			   for(int i=1;i<Ninit;i++){
               strTmp=strList.at(1);
				   if(strTmp.contains("type="))TmpInit.strInitTypeName=strTmp.remove("type=").simplified();
			   }
               ret=ReadNextLine(inText, line);
               while (ret==0){
                       QStringList InitList = line.split(",");   
                       if (InitList.size() < 2) { ret=ReadNextLine(inText, line); continue; }
                       TmpInit.strPSetName=InitList.at(0);
                       TmpInit.strVal = InitList.at(1).simplified();
					   TmpInitInps.append(TmpInit);
                     ret=ReadNextLine(inText, line);
               }
               //TmpInitInps.append(TmpInit); 
		   }else if (strList.at(0)=="*step"){//���������
               HIPSolveInpS TmpSlove;
               for (int kk=1;kk<strList.size();kk++)
               {
                   int strTmpId=strList.at(kk).indexOf("=");
                   strTmp=strList.at(kk).left(strTmpId);  
                   if (strTmp<0)continue;
                   if (strTmp=="inc"){
                       strTmp=strList.at(kk);
                       TmpSlove.strStepInc=strTmp.remove("inc=", Qt::CaseInsensitive).simplified();
                   }else if (strTmp=="nlgeom"){
                       strTmp=strList.at(kk).simplified();
                       TmpSlove.strNlgeom=strTmp.remove("nlgeom=", Qt::CaseInsensitive).simplified();
                   }
               }
               //strTmp=strList.at(1);
               //TmpSlove.strStepInc=strTmp.remove("inc=", Qt::CaseInsensitive).simplified();//������沽��
               //strTmp=strList.at(2);
               //TmpSlove.strNlgeom=strTmp.remove("nlgeom=", Qt::CaseInsensitive).simplified();
               ret=ReadNextLine(inText, line);
			   QString SolverType="";
			   if(ret==4){
				   TmpSlove.strProcess="4";
				   ret=ReadNextLine(inText, line);
				   SolverType=line.split(",").at(0);
			   }else if(ret==5){
				   TmpSlove.strProcess="5";
				   ret=ReadNextLine(inText, line);
				   SolverType=line.split(",").at(0);
			   }else if(ret==6){
				   TmpSlove.strProcess="6";
				   ret=ReadNextLine(inText, line);
				   SolverType=line.split(",").at(0);
			   }else if(ret==7){
				   TmpSlove.strProcess="7";
				   ret=ReadNextLine(inText, line);
				   SolverType=line.split(",").at(0);
			   }else{
				   TmpSlove.strProcess="101";
				   SolverType=line.split(",").at(0);
			   }
			  
               while (ret==1){
                QStringList SloveList = line.split(","); 
                strTmp=SloveList.at(0);
                TmpSlove.strNlgeomSTR=strTmp;//
                for (int ttt=1;ttt<SloveList.size();ttt++){
                    strTmp =SloveList.at(ttt).simplified();
                    if (strTmp.contains("deltmx=",Qt::CaseInsensitive) ) {
                        TmpSlove.strDelMX=strTmp.remove("deltmx=", Qt::CaseInsensitive).simplified();
                    }
                }
                ret=ReadNextLine(inText, line); //ret==0
                SloveList = line.split(",");
                if (SloveList.size() < 4) { ret=ReadNextLine(inText, line); continue; }
                TmpSlove.strInitStep=SloveList.at(0);
                TmpSlove.strTotalTime=SloveList.at(1).simplified();
				if(SolverType=="*dynamic"){
					TmpSlove.strMinStep=1e-8;//min
					TmpSlove.strMaxStep=10000000;//max
				}else{
					TmpSlove.strMinStep=SloveList.at(2).simplified();//min
					TmpSlove.strMaxStep=SloveList.at(3).simplified();//max
				}
				int ijk=TmpSolveInps.size();
				TmpSlove.TimeFrequencyInc=IncOutput.at(ijk);
                TmpSolveInps.append(TmpSlove);
                break;
               }
		   }else if (strList.at(0)=="*timepoints"){//���������
			   //TmpVariableInps
               ret=ReadNextLine(inText, line);
			   QStringList strll=line.split(",");
			   int Ntime=0;
			   for(int i=0;i<strll.size();i++){
				   QString str=strll.at(i);
				   if(str!="")Ntime++;
				   if(Ntime==3)IncOutput<<strll.at(i);
			   }
               while (ret==0){
                ret=ReadNextLine(inText, line);
              }
		   }else if (strList.at(0)=="*nodefile"||strList.at(0)=="*nodeprint"){//�ļ�������
               VariableInpS VariabList;
			   if(strList.size()>=3&&strList.at(strList.size()-1)=="totals=only"){
				   VariabList.Igforce="1";
                   QStringList str=strList.at(1).split("=");
                   QString str2 = (str.size() >= 2) ? str.at(1) : "";
				   VariabList.NodeSet=str2;
				   //VariabList.NodeSet1<<str2;
				   strTmp=strList.at(2);
				   QStringList  TimeFreqList=strTmp.split("=");
				   VariabList.strTimeOrFreqName=TimeFreqList.at(0);
				   strTmp = (TimeFreqList.size() >= 2) ? TimeFreqList.at(1) : "";
				   VariabList.strTname=strTmp.remove("t",Qt::CaseInsensitive).simplified();
			   }else{
				   strTmp=strList.at(strList.size()-1);
				   //VariabList.strTimeOrFreqName=strTmp.remove("TIME POINTS", Qt::CaseInsensitive);//("FREQUENCY",Qt::CaseInsensitive);
				   //VariabList.strTimeOrFreqName=strTmp.remove("FREQUENCY", Qt::CaseInsensitive);
				   QStringList  TimeFreqList=strTmp.split("=");
				   VariabList.strTimeOrFreqName=TimeFreqList.at(0);
				   strTmp = (TimeFreqList.size() >= 2) ? TimeFreqList.at(1) : "";
				   VariabList.strTname=strTmp.remove("t",Qt::CaseInsensitive).simplified();
			   }
               ret=ReadNextLine(inText, line);//ret==0
			   QString line0="";
			   while(ret==0){
				   if(line0=="")line0+=line;
				   else line0+=","+line;
				   ret=ReadNextLine(inText,line);
			   }
			   VariabList.strNInclude=line0;

                    TmpVariableInps.append(VariabList);
		   }else if (strList.at(0)=="*elfile"||strList.at(0)=="*elprint"){//
			   VariableInpS VariabList;
			   if(strList.size()>=3&&strList.at(strList.size()-1)=="totals=only"){
				   VariabList.Igforce="1";
				   QStringList str=strList.at(1).split("=");
                   QString str2 = (str.size() >= 2) ? str.at(1) : "";
				   strTmp=strList.at(2);
				   QStringList  TimeFreqList=strTmp.split("=");
				   VariabList.strTimeOrFreqName=TimeFreqList.at(0);
				   strTmp = (TimeFreqList.size() >= 2) ? TimeFreqList.at(1) : "";
				   VariabList.strTname=strTmp.remove("t",Qt::CaseInsensitive).simplified();
			   }else{
				   strTmp=strList.at(strList.size()-1);
				   QStringList  TimeFreqList=strTmp.split("=");
				   VariabList.strTimeOrFreqName=TimeFreqList.at(0);
				   VariabList.strELFile=strList.at(0);
				   strTmp = (TimeFreqList.size() >= 2) ? TimeFreqList.at(1) : "";
				   VariabList.strTname=strTmp.remove("t",Qt::CaseInsensitive).simplified();
                   }
                   ret=ReadNextLine(inText,line);
			   QString line0="";
			   while(ret==0){
				   if(line0=="")line0+=line;
				   else line0+=","+line;
                   //TmpVariableInps.append(VariabList);
                   ret=ReadNextLine(inText,line);
               } 
			   VariabList.strELInclude=line0;
			   TmpVariableInps.append(VariabList);
		   }else if (strList.at(0)=="*material"){
               MaterialInpS MaterialList;
			   for(int i=1;i<strList.size();i++){
				   strTmp=strList.at(i);
				   if(strTmp.contains("name="))MaterialList.strMaterialName=strTmp.remove("name=").simplified();
               }
               ret = ReadNextLine(inText,line);
               
               while(1==ret){
                   QStringList TMaterial=line.split(",");
                   if (strTmp.size()>1){
					   if(TMaterial.at(0)=="*material"||TMaterial.at(0)=="*boundary"||
						   TMaterial.at(0)=="*physicalconstants"||TMaterial.at(0)=="*initialconditions"||
						   TMaterial.at(0)=="*amplitude"||TMaterial.at(0)=="*surfaceinteraction"){
                        break;
                     }                       
                   }
				   QString str=line.remove("*").simplified();
                   MaterialList.strMaterialStyleName<<str;
				   QStringList strKeyList;QString strKeyStyle="",strKeyStyleType="";
				   strKeyList = str.split("," , QString::SkipEmptyParts);
				   strKeyStyle = strKeyList.at(0);
				   for(int i=1;i<strKeyList.size();i++){
					   strTmp=strKeyList.at(i);
					   if(strTmp. contains("type="))strKeyStyleType=strTmp.remove("type=", Qt::CaseInsensitive).simplified();
				   }

				   if (strKeyStyle== "phasecurve"){ 
					   HPphaseCurveInps HPphaseCurveList;
				       HPphaseCurveList.strName=strKeyStyle;
					   HPphaseCurveList.strPhaseCurveType=strKeyStyleType;
					   ret = ReadNextLine(inText,line); 
					   while (ret==0){
						   if (line.split(",").size()>3){
						      HPphaseCurveList.strHeadParam<<line;
						   }else{
							  HPphaseCurveList.strData<<line;
						   }
						  ret = ReadNextLine(inText,line); 
					   }
					   TmpHPphaseCurveInps.append(HPphaseCurveList);
				   }else if (strKeyStyle=="phaseequilibrium"){
						HPphaseBalanceInps HPphaseBalanceList;
						HPphaseBalanceList.strName=strKeyStyle;
						ret=ReadNextLine(inText,line);
						while (ret==0){
							int iDataNum=0;
							if (ret==0){
								HPphaseBalanceList.strHeadParam<<line;
								iDataNum=line.split(",",QString::SkipEmptyParts).at(1).toInt();
							}
							ret=ReadNextLine(inText,line);

							for (int kk=0;kk<iDataNum;kk++){
								HPphaseBalanceList.strData<<line;
								ret=ReadNextLine(inText,line);
								if (ret==1)break;
							}
						}
						TmpHPphaseBalanceInps.append(HPphaseBalanceList);
				   }else if(strKeyStyle=="incubationperiod"){
					   HPincubatePeriodInps HPincubatePeriodList;
					   HPincubatePeriodList.strName=strKeyStyle;
					   ret=ReadNextLine(inText,line);
					   while (ret==0){
						   int iDataNum=0;
						   if (ret==0){
							   HPincubatePeriodList.strHeadParam<<line;
							   iDataNum=line.split(",",QString::SkipEmptyParts).at(1).toInt();
						   }
						   ret=ReadNextLine(inText,line);

						   for (int kk=0;kk<iDataNum;kk++){
							   HPincubatePeriodList.strData<<line;
							   ret=ReadNextLine(inText,line);
							   if (ret==1)break;
						   }
					   }
					   
					  TmpHPincubatePeriodInps.append(HPincubatePeriodList);
				   }else if(strKeyStyle=="phaselatentheat"){//Ǳ��
					  HPphaseElateHeatInps  TmpHPphaseElateHeatList;
					  TmpHPphaseElateHeatList.strName=strKeyStyle;
					  ret=ReadNextLine(inText,line);
					  while(ret==0){
						  TmpHPphaseElateHeatList.strHeadParam<<line;
						  ret=ReadNextLine(inText,line);
					  }
					  TmpHPphaseElateHeatInps.append(TmpHPphaseElateHeatList);
                   }else if(strKeyStyle=="phasehardness"){
                       HPphaseHardInps  TmpHPphaseHardList;
                       TmpHPphaseHardList.strName=strKeyStyle;
                       ret=ReadNextLine(inText,line);
                       while(ret==0){
                           TmpHPphaseHardList.strHeadParam<<line;
                           ret=ReadNextLine(inText,line);
                       }
                       TmpHPphaseHardInps.append(TmpHPphaseHardList);
                   }else if(strKeyStyle=="phasectrol"){
					   HPphaseCtrolInps HPphaseCtrolList;
					   HPphaseCtrolList.strName=strKeyStyle;
					   ret=ReadNextLine(inText,line);
					   if (ret==0){
						   HPphaseCtrolList.strHeadParam=line;
					   }
					    ret=ReadNextLine(inText,line);
						TmpHPphaseCtrolInps.append(HPphaseCtrolList);
				   }else if(strKeyStyle=="phaseprop"){
					   if (strKeyStyleType=="cond"){
						   HPPhasePropCondInps HPPhasePropCondList;
						   HPPhasePropCondList.strName=strKeyStyle;
						   HPPhasePropCondList.strType=strKeyStyleType;
                           ret=ReadNextLine(inText,line);
						   while (ret==0){
							   int iDataNum=0;
							   if (ret==0){
								   HPPhasePropCondList.strHeadParam<<line;
								   iDataNum=line.split(",",QString::SkipEmptyParts).at(1).toInt();
							   }
							   ret=ReadNextLine(inText,line);

							   for (int kk=0;kk<iDataNum;kk++){
								   HPPhasePropCondList.strData<<line;
								   ret=ReadNextLine(inText,line);
								   if (ret==1)break;
							   }
						   }
						   TmpHPPhasePropCondInps.append(HPPhasePropCondList);
					   }else if (strKeyStyleType=="spec"){
						   HPPhasePropSpecInps HPPhasePropSpecList;
						   HPPhasePropSpecList.strName=strKeyStyle;
						   HPPhasePropSpecList.strType=strKeyStyleType;
                           ret=ReadNextLine(inText,line);
						   while (ret==0){
							   int iDataNum=0;
							   if (ret==0){
								   HPPhasePropSpecList.strHeadParam<<line;
								   iDataNum=line.split(",",QString::SkipEmptyParts).at(1).toInt();
							   }
							   ret=ReadNextLine(inText,line);

							   for (int kk=0;kk<iDataNum;kk++){
								   HPPhasePropSpecList.strData<<line;
								   ret=ReadNextLine(inText,line);
								   if (ret==1)break;
							   }
						   }
						   TmpHPPhasePropSpecInps.append(HPPhasePropSpecList);
					   }
				   }else if(strKeyStyle=="phaseys"){//����ǿ��
					   HPPhaseYSInps HPPPhaseYSList;
					   HPPPhaseYSList.strName=strKeyStyle;
					   ret=ReadNextLine(inText,line);
					   if (ret==0){
						   HPPPhaseYSList.strHeadParam=line;
					   }
					   ret=ReadNextLine(inText,line);
					   TmpHPPhaseYSInps.append(HPPPhaseYSList);

				   }else if(strKeyStyle=="phasegs"){//�����ߴ�
					   HPPhaseGSInps HPPPhaseGSList;
					   HPPPhaseGSList.strName=strKeyStyle;
					   ret=ReadNextLine(inText,line);
					   if (ret==0){
						   HPPPhaseGSList.strHeadParam=line;
					   }
					   ret=ReadNextLine(inText,line);
					   TmpHPPhaseGSInps.append(HPPPhaseGSList);
				   }else if(strKeyStyle=="phasezbf"){//����
					   HPPhaseZBFInps HPPhaseZBFList;
					   HPPhaseZBFList.strName=strKeyStyle;
					   ret = ReadNextLine(inText,line); 
					   while (ret==0){
						   if (line.split(",").size()>3){
							   HPPhaseZBFList.strHeadParam<<line;
						   }else{
							   HPPhaseZBFList.strData<<line;
						   }
						   ret = ReadNextLine(inText,line); 
					   }
					   TmpHPPhaseZBFInps.append(HPPhaseZBFList);
                     }else if(strKeyStyle=="rate-dependentplastic"||strKeyStyle=="damage"||
						 strKeyStyle=="dynamicrecrystallization"){
						   int strDataCnt=0;
						   QStringList *tmpData = new QStringList;
						   ret = ReadNextLine(inText,line); 
						   QStringList tmpL=line.split(",",QString::SkipEmptyParts);
						   QString keyword=strKeyStyle.remove(QRegExp("\\s"));
						   if(keyword=="rate-dependentplastic"){
							   QString strTmp=tmpL.at(0);
							   if(strTmp=="2")strKeyStyleType="Arrhenius";
							   else strKeyStyleType="HansenSpittel";
						   }else if(keyword=="damage"){
							   QString strTmp=tmpL.at(0);
							   if(strTmp=="1")strKeyStyleType="NormalizedCL";
							   else if(strTmp=="2")strKeyStyleType="CockroftLatham";
							   else if(strTmp=="3")strKeyStyleType="McClintock";
							   else if(strTmp=="4")strKeyStyleType="Freudenthal";
							   else if(strTmp=="5")strKeyStyleType="RiceTracy";
							   else if(strTmp=="6")strKeyStyleType="Oyane";
							   else if(strTmp=="7")strKeyStyleType="Oyanenegative";
							   else if(strTmp=="8")strKeyStyleType="Ayada";
							   else if(strTmp=="9")strKeyStyleType="Ayadanegative";
							   else if(strTmp=="10")strKeyStyleType="Osakada";
							   else if(strTmp=="11")strKeyStyleType="Brozzo";
							   else if(strTmp=="12")strKeyStyleType="ZhaoKuhn";
							   else if(strTmp=="13")strKeyStyleType="MPSUTS";
						   }
						   QString tmpStrM(QString("%1").arg(tmpL.size()));
						   MaterialList.strSubMCol<< tmpStrM; //--�õ���ǰ������������һ���ǿյġ�,����ȥ����
						   QString StrLP="";
						   while(0==ret){
							   QStringList tmpL1;
							   tmpL=line.split(",",QString::SkipEmptyParts);
							   tmpL1=tmpL;
							   if(tmpL1.size()>8&&strKeyStyle=="rate-dependentplastic"){
								   tmpL.clear();
								   StrLP=tmpL1.at(8);
								   for(int i=0;i<tmpL1.size()-1;i++){
									   tmpL<<tmpL1.at(i);
								   }
								   tmpL1.clear();
							   }
							   *tmpData<<tmpL;
							   ret = ReadNextLine(inText,line);
						   }
						   MaterialList.strData[strDataCnt].push_back(tmpData);
						  if(StrLP!="")MaterialList.strMaterialStyleParam2<<StrLP;
						   strDataCnt++;
				   }else{
					   int strDataCnt=0;
					   QStringList *tmpData = new QStringList;
					   ret = ReadNextLine(inText,line); 
					   QStringList tmpL=line.split(",",QString::SkipEmptyParts);
	  
					   QString tmpStrM(QString("%1").arg(tmpL.size()));
					  // MaterialList.strSubMCol<< tmpStrM; //--�õ���ǰ������������һ���ǿյġ�,����ȥ����
					   if(tmpStrM.toInt()>2)MaterialList.strSubMCol<<"3";
					   else MaterialList.strSubMCol<<"2";
					   while(0==ret){
						   tmpL=line.split(",",QString::SkipEmptyParts);
						   if(tmpL.size()==1)tmpL<<" ";
						   *tmpData<<tmpL;   
						   ret = ReadNextLine(inText,line);
					   }
					   MaterialList.strData[strDataCnt].push_back(tmpData);
					   strDataCnt++;
				   }
               }
              TmpMaterialInps.append(MaterialList);
           //}else if (strList.at(0)=="*Physical Constants"){
           }else if (strList.at(0)=="*film"){
               ret = ReadNextLine(inText,line);
               HPSystemFInpS HPSystemFList;
               HPSystemFList.strStyle="film";
			   int TSi=TmpSolveInps.size();
               HPSystemFList.strStep=QString("%1").arg(TSi);
			   HPSystemFList.strProcess=TmpSolveInps.at(TSi-1).strProcess;
               for (int kk=1;kk<strList.size();kk++)
               {
                   int strTmpId=strList.at(kk).indexOf("=");
                   strTmp=strList.at(kk).left(strTmpId).simplified();  
                   if (strTmp<0)continue;
                   if (strTmp=="amplitude"){
                       strTmp=strList.at(kk);
                       HPSystemFList.strAmplitudeName=strTmp.remove("amplitude=", Qt::CaseInsensitive).simplified();
                   }else if (strTmp=="film amplitude"){
                       strTmp=strList.at(kk).simplified();
                       HPSystemFList.strFilmRadiateAmpName=strTmp.remove("film amplitude=", Qt::CaseInsensitive).simplified();
                   }else if (strTmp=="op"){
                       strTmp=strList.at(kk).simplified();
                       HPSystemFList.strOP=strTmp.remove("op=", Qt::CaseInsensitive).simplified();
                   }
               }
               int iIncludeCnt=0;
               while(ret==0){
                QStringList InitList = line.split(",");
                iIncludeCnt++;
                HPSystemFList.strInclude=QString("%1").arg(iIncludeCnt);
                HPSystemFList.strElsetName = InitList.at(0);
                HPSystemFList.strSurfaceNum = InitList.at(1).simplified();
                HPSystemFList.strEnvirTemprat = InitList.at(2).simplified();
                HPSystemFList.strCofficient = InitList.at(3).simplified();
                ret = ReadNextLine(inText,line); 
                TmpHPSystemFInps.append(HPSystemFList);
               }
               if(HPSystemFList.strElsetName==""){//δ���롰while��ret =0����film���޲�����
                    TmpHPSystemFInps.append(HPSystemFList);
               }
		   }else if (strList.at(0)=="*radiate"){
               ret = ReadNextLine(inText,line);
               HPSystemRInpS HPSystemRList;
               HPSystemRList.strStyle="radiate";
			   int TSi=TmpSolveInps.size();
               HPSystemRList.strStep=QString("%1").arg(TSi);
			   HPSystemRList.strProcess=TmpSolveInps.at(TSi-1).strProcess;
               for (int kk=1;kk<strList.size();kk++)
               {
                   int strTmpId=strList.at(kk).indexOf("=");
                   strTmp=strList.at(kk).left(strTmpId).simplified();  
                   if (strTmp<0)continue;
                   if (strTmp=="amplitude"){
                       strTmp=strList.at(kk).simplified();
                       HPSystemRList.strAmplitudeName=strTmp.remove("amplitude=", Qt::CaseInsensitive).simplified();
                   }else if (strTmp=="radiate amplitude"){
                       strTmp=strList.at(kk).simplified();
                       HPSystemRList.strFilmRadiateAmpName=strTmp.remove("radiate amplitude=", Qt::CaseInsensitive).simplified();
                   }else if (strTmp=="op"){
                       strTmp=strList.at(kk);
                       HPSystemRList.strOP=strTmp.remove("op=", Qt::CaseInsensitive).simplified();
                   }
               }
               int iIncludeCnt=0;
			   while(ret==0){
                   QStringList InitList = line.split(",");   
                   iIncludeCnt++;
                   HPSystemRList.strInclude=QString("%1").arg(iIncludeCnt);
                   HPSystemRList.strElsetName = InitList.at(0);
                   HPSystemRList.strSurfaceNum = InitList.at(1).simplified();
                   HPSystemRList.strEnvirTemprat = InitList.at(2).simplified();
                   HPSystemRList.strCofficient = InitList.at(3).simplified();
                   ret = ReadNextLine(inText,line); 
                   TmpHPSystemRInps.append(HPSystemRList); 
               }
               if(HPSystemRList.strElsetName==""){//δ���롰while��ret =0����radiation���޲�����
                    TmpHPSystemRInps.append(HPSystemRList);
               }
		   }else if (strList.at(0)=="*surfaceinteraction"){
			   CPPInInp RECPPInInp;
				for(int i=1;i<strList.size();i++){
                    strTmp=strList.at(i);
					if(strTmp.contains("name="))RECPPInInp.CPPName=strTmp.remove("name=").simplified();
					else if(strTmp.contains("slaver="))RECPPInInp.SlaveS=strTmp.remove("slaver=").simplified();
					else if(strTmp.contains("master="))RECPPInInp.MasterS=strTmp.remove("master=").simplified();
				}
			   if (strList.size()<=1)continue;
               ret = ReadNextLine(inText,line);
			   while(ret==1){
				   QStringList Tcontactpart=line.split(",");
				   if (Tcontactpart.size()>1){
                      if(Tcontactpart.at(0)!="*friction"&&Tcontactpart.at(0)!="*surfacebehavior"&&
                          Tcontactpart.at(0)!="*gapconductance"&&Tcontactpart.at(0)!="*gapheatgeneration"){
                        break;
					  }
				   }
				   Tcontactpart=line.remove("*").split(",");
			       QStringList strKeyList;QString strKeyStyle="",strKeyStyleType="";
			       strKeyList =Tcontactpart;
			       strKeyStyle = strKeyList.at(0);
				   if(strKeyStyle=="friction"){
					   QStringList readline;
				       RECPPInInp.SlipTolerance="0.005";
				       ret = ReadNextLine(inText,line);
				       readline=line.split(",");
					   if(ret==0){
					       if(readline.size()==1){
				     	      RECPPInInp.FrictionCoefficient=readline.at(0);
						   }else if(readline.size()>1){
						      RECPPInInp.FrictionCoefficient=readline.at(0);
						      RECPPInInp.TangentialStiffness=readline.at(1);
						   }
					   }
				    ret = ReadNextLine(inText,line);
					}else if(strKeyStyle=="surfacebehavior"){
					   QStringList readline;
				       RECPPInInp.NormalType="linear";
				       ret = ReadNextLine(inText,line);
                       readline=line.split(",");
					   if(ret==0&&readline.size()>0)RECPPInInp.NormalStiffness=readline.at(0);
				       ret = ReadNextLine(inText,line);
					}else if(strKeyStyle=="gapconductance"){
						QStringList readline;
						ret = ReadNextLine(inText,line);
						readline=line.split(",");
						if(readline.size()==1){
							RECPPInInp.HeatExchangeType="����";
							RECPPInInp.HEParam=readline.at(0).simplified();
							ret = ReadNextLine(inText,line);
						}else if(readline.size()==3&&readline.at(1).simplified()==""){
							RECPPInInp.HeatExchangeType="���¶ȱ仯";
							RECPPInInp.strCurveName="heatexchange"+QString::number(TmpCPPInInp.size(),10);//������תΪQString
							while(ret==0){
								readline=line.split(",");
								RECPPInInp.strDataY<<readline.at(0);
								RECPPInInp.strDataX<<readline.at(2);
								ret = ReadNextLine(inText,line);
							}
						}else if(readline.size()==2&&readline.at(1).simplified()!=""){
							RECPPInInp.HeatExchangeType="��ѹ���仯";
							RECPPInInp.strCurveName="heatexchange"+QString::number(TmpCPPInInp.size(),10);//������תΪQString
							while(ret==0){
								readline=line.split(",");
								RECPPInInp.strDataY<<readline.at(0);
								RECPPInInp.strDataX<<readline.at(1);
								ret = ReadNextLine(inText,line);
							}
						}
					}else if(strKeyStyle=="gapheatgeneration"){
						QStringList readline;
				        ret = ReadNextLine(inText,line);
						readline=line.split(",");
						if(ret==0&&readline.size()>0)RECPPInInp.FGenerHeat=readline.at(0).simplified();
						ret = ReadNextLine(inText,line);
					}	
			   }
			   TmpCPPInInp.append(RECPPInInp);
		   }else if (strList.at(0)=="*contactpair"||strList.at(0)=="*tie"){
			   QStringList readline;
			   line=line.remove("*").simplified();
			   line=line.remove("interaction=",Qt::CaseInsensitive).simplified();
			   readline=line.remove("type=",Qt::CaseInsensitive).simplified().split(",");
                           if (readline.size() < 3) { ret=ReadNextLine(inText, line); continue; }
			   if(readline.at(0)=="contactpair"){
				   CPInInp RECPInInp;
				   RECPInInp.CCPName=readline.at(1);
				   RECPInInp.SurfaceType=readline.at(2);
				   ret = ReadNextLine(inText,line);
			       readline=line.split(",");
			       RECPInInp.MainSurface=readline.at(1).simplified();
			       RECPInInp.SlaveSurface=readline.at(0).simplified();
				   TmpCPInInp.append(RECPInInp);
			   }else if(readline.at(0)=="tie"){
                  CPTieInp RECPTieInp;
				  RECPTieInp.TieName=readline.at(1);
				  RECPTieInp.Adjust="yes";
				  ret = ReadNextLine(inText,line);
			      readline=line.split(",");
				  RECPTieInp.MainSurface=readline.at(1).simplified();
			      RECPTieInp.SlaveSurface=readline.at(0).simplified();
				  TmpCPTieInp.append(RECPTieInp);
			   }	   
			   ret = ReadNextLine(inText,line);
		   }else if (strList.at(0)=="*dload"){
			   QStringList readline;
			    ret=ReadNextLine(inText, line);
				while(ret==0){
				   if(line.contains("grav")){
						readline = line.split(",");
						if (readline.size() < 6) { ret=ReadNextLine(inText, line); continue; }
						TmpGravityInps.Value=readline.at(2);
						TmpGravityInps.direation=readline.at(3)+","+readline.at(4)+","+readline.at(5);
					}
					ret=ReadNextLine(inText, line);
				}
		   }else{//not record "*" KeyWord;
              ret=ReadNextLine(inText, line);
           }
      }//while(ret != 1)
    }//while(!inText.atEnd())
    if (InpStyle ==1){//gmsh style
        NodeELSetInps tmpReadNELSufInpS;
        if(changeGmshToNormal(tmpReadNELSufInpS,TmpGmshInps)){
            m_ReadInpResult.TmpNodeInpS=tmpReadNELSufInpS.NodeInpData;
            m_ReadInpResult.TmpElInpS=tmpReadNELSufInpS.ELInpData;
            m_ReadInpResult.TmpNsetInps=tmpReadNELSufInpS.NSetInpData;
            m_ReadInpResult.TmpElSetInps=tmpReadNELSufInpS.ElSetInpData;
            m_ReadInpResult.TmpSurfaceInps=tmpReadNELSufInpS.SurfaceInpData;  
        }
    }else{
        m_ReadInpResult.TmpNodeInpS=TmpNodeInpS;
        m_ReadInpResult.TmpElInpS=TmpElInpS;
        m_ReadInpResult.TmpNsetInps=TmpNsetInps;
        m_ReadInpResult.TmpElSetInps=TmpElSetInps;
        m_ReadInpResult.TmpSurfaceInps=TmpSurfaceInps;
    }
    m_ReadInpResult.TmpSectionInps=TmpSectionInps;
    m_ReadInpResult.TmpMaterialInps=TmpMaterialInps;
    m_ReadInpResult.TmpSolveInps=TmpSolveInps;
    m_ReadInpResult.TmpCurveInps=TmpCurveInps;
    m_ReadInpResult.TmpHIPSystemInps=TmpHIPSystemInps;
    m_ReadInpResult.TmpBCInps=TmpBCInps;
    m_ReadInpResult.TmpInitInps=TmpInitInps;
    m_ReadInpResult.TmpVariableInps=TmpVariableInps;

    //--�ȴ�����
    //HPSystemInpS HPSystemList;
    //for (int i=0;i<TmpHPSystemFInps.size();i++)
    //{
    //    HPSystemList.strElsetName=TmpHPSystemFInps.at(i).strElsetName;
    //    HPSystemList.strSurfaceNum=TmpHPSystemFInps.at(i).strSurfaceNum;
    //    HPSystemList.strEnvirTemprat=TmpHPSystemFInps.at(i).strEnvirTemprat;
    //    HPSystemList.strCofficientF=TmpHPSystemFInps.at(i).strCofficient;
    //    if (i<TmpHPSystemRInps.size()){  
    //         HPSystemList.strCofficientR=TmpHPSystemRInps.at(i).strCofficient;
    //    }   
    //    TmpHPSystemInps.push_back(HPSystemList);
    //}
     m_ReadInpResult.TmpHPSystemFInps=TmpHPSystemFInps;
     m_ReadInpResult.TmpHPSystemRInps=TmpHPSystemRInps;
	 m_ReadInpResult.TmpCPPInInp=TmpCPPInInp;
	 m_ReadInpResult.TmpCPInInp=TmpCPInInp;
	 m_ReadInpResult.TmpCPTieInp=TmpCPTieInp;
	 m_ReadInpResult.TmpFSystem=TmpFSystem;

     //m_ReadInpResult.TmpHPSystemInps=TmpHPSystemInps;
     m_ReadInpResult.TmpHPphaseCurveInps=TmpHPphaseCurveInps;
	 m_ReadInpResult.TmpHPphaseBalanceInps=TmpHPphaseBalanceInps;
	 m_ReadInpResult.TmpHPphaseCtrolInps=TmpHPphaseCtrolInps;
 	 m_ReadInpResult.TmpHPincubatePeriodInps=TmpHPincubatePeriodInps;
 	 m_ReadInpResult.TmpHPphaseElateHeatInps=TmpHPphaseElateHeatInps;
     m_ReadInpResult.TmpHPphaseHardInps=TmpHPphaseHardInps;
 	 m_ReadInpResult.TmpHPPhasePropSpecInps=TmpHPPhasePropSpecInps;
 	 m_ReadInpResult.TmpHPPhasePropCondInps=TmpHPPhasePropCondInps;
	 m_ReadInpResult.TmpHPPhaseYSInps=TmpHPPhaseYSInps;
	 m_ReadInpResult.TmpHPPhaseGSInps=TmpHPPhaseGSInps;
	 m_ReadInpResult.TmpHPPhaseZBFInps=TmpHPPhaseZBFInps;
	 m_ReadInpResult.TmpGravityInps=TmpGravityInps;
    
     date = QDateTime::currentDateTime();
     strTime=date.toString("hh:mm:ss yy.MM.dd");
     infoW->ShowInformation("End Time: "+strTime);
    return true;
}
//------------------------------------------------------------
//--Read Inp From .INP file  02(���ںϲ�inp�ļ��Ķ�ȡ)
//------------------------------------------------------------
bool CRWManage::ReadSectionInpFile02(QFile *file,int NodeNumber,int ElementNumber,QString FileName)
{
    int ret;
    QString line,strTmp,PartName;
    QTextStream inText(file);
    PartName="";
    NodeInpS TmpNodeInpS;//*node
    ELInpS TmpElInpS;//*element
    NSetInpS TmpNsetInps;//*nset
    ElSetInpS TmpElSetInps;//*elset
    SurfaceInpS TmpSurfaceInps;//*surface
    QList<NodeELSetInps> TmpMeshInps;//*node
    GmshInpS TmpGmshInps;
    int InpStyle=0; //0->inital
                    //1->gmsh inp
                    //2->normal inp
	//m_ReadInpResult.TmpNodeInpS;

	while(!inText.atEnd()) {
		//ret = ReadNextLine(file,line);
		ret=ReadNextLine(inText, line); 
		if(ret==1&&line=="*heading")//--gmsh data -->*Heading 
		{
			PartName=FileName;
			InpStyle=1;
			ret=ReadNextLine(inText, line);
			while(ret!=-1){//Ѱ��*node���б���
				QStringList strList = line.split(",");
				if (ret!=1){
					ret=ReadNextLine(inText, line);
					strList = line.split(",");
				}
				if(strList.at(0)=="*node"){//*Node Read
					TmpGmshInps.NodeInpData.PartName=PartName;
					ret = ReadNextLine(inText, line);
					while(0==ret){
						TmpGmshInps.NodeInpData.strData.push_back(line);
						ret = ReadNextLine(inText, line); 
					}    
				}
				strList=line.split(",");
				if (strList.at(0)=="*element"){//Element
					ELInpS tmpEl;
					QString strTmp01;
					for (int ttt=1;ttt<strList.size();ttt++){
						strTmp =strList.at(ttt).simplified();
						if (strTmp.contains("type=",Qt::CaseInsensitive) ) {
							strTmp01=strTmp.remove("type=", Qt::CaseInsensitive).simplified();
							tmpEl.strELType=strTmp01;
						}else if(strTmp.contains("elset=",Qt::CaseInsensitive)){
							tmpEl.strELSET=strTmp.remove("elset=", Qt::CaseInsensitive).simplified();
						}
					}
					ret=ReadNextLine(inText, line);
					while(0==ret||ret==-1){
						tmpEl.strData.push_back(line);
						if(ret==-1)break;
						ret = ReadNextLine(inText, line); 
					}
					tmpEl.PartName=PartName;
					TmpGmshInps.gmshELData.append(tmpEl);
				}else if(strList.at(0)=="*elset"){//ELSET
					int nElsetSize = strList.size();
					strTmp = strList.at(1);
					TmpGmshInps.gmshELSetData.strElSetName<<strTmp.remove("elset=", Qt::CaseInsensitive).simplified();
					if (nElsetSize == 2){TmpGmshInps.gmshELSetData.strElSetStyle<<"";
					}else if (nElsetSize == 3){TmpGmshInps.gmshELSetData.strElSetStyle<<"generate";
					}

					ret=ReadNextLine(inText, line);
					QString curElData;curElData.clear();
					while(0==ret){
						if (curElData ==""){//����ֱ�Ӹ�ֵ
							curElData=line;
						}else{//�����н�������
							curElData +=","+line;
						}
						ret = ReadNextLine(inText ,line); 
					}
					TmpGmshInps.gmshELSetData.PartName=PartName;
					TmpGmshInps.gmshELSetData.strData<<curElData;
				}else if (strList.at(0)=="*nset"){ //NSET
					int nNsetSize = strList.size();
					strTmp = strList.at(1);
					TmpGmshInps.gmshNSetData.strNSetName<<strTmp.remove("nset=", Qt::CaseInsensitive).simplified();
					if (nNsetSize==2){TmpGmshInps.gmshNSetData.strNSetStyle<<"";
					}else if (nNsetSize == 3){TmpGmshInps.gmshNSetData.strNSetStyle<<"generate";
					}
					ret=ReadNextLine(inText, line);
					QStringList tmpNsetData;
					while(0==ret){
						tmpNsetData<<line;
						ret = ReadNextLine(inText ,line); 
					}
					if (ret==-1){
						tmpNsetData<<line;
					}
					TmpGmshInps.gmshNSetData.PartName=PartName;
					TmpGmshInps.gmshNSetData.strData.push_back(tmpNsetData);
				}else{//����ʶ�Ĺؼ���
					ret=ReadNextLine(inText, line);
				}//strList.at(0).toLower()=="*element"

			}//heading
			ret=ReadNextLine(inText, line);
		}
       //--�����ʶ���Inp data��

	   while (ret!=-1){//*Element
           QStringList strList=line.split(",");
		   if (strList.at(0)=="**partstart"){
			   int nSize = strList.size();
			   for(int i=1;i<nSize;i++){
				   strTmp = strList.at(i);
				   if(strTmp.contains("name=")){
					   PartName=strTmp.remove("name=", Qt::CaseInsensitive).simplified();
				   }
			   }
			   ret = ReadNextLine(inText, line);
		   }else if (strList.at(0)=="*node"){//*Node Read
               if (!TmpNodeInpS.strData.empty()&&TmpElInpS.strELType==""){//element��û�����ݣ�˵��Ϊ��һ�β����б���
                   NodeELSetInps tmpD;
                   tmpD.NodeInpData=TmpNodeInpS;
                   tmpD.ELInpData=TmpElInpS;
                   tmpD.NSetInpData=TmpNsetInps;
                   tmpD.ElSetInpData=TmpElSetInps;
                   tmpD.SurfaceInpData=TmpSurfaceInps;
                   TmpMeshInps.append(tmpD);
                   TmpNodeInpS.clear();TmpElInpS.clear();TmpNsetInps.clear();TmpElSetInps.clear();TmpSurfaceInps.clear();
               }
               ret = ReadNextLine(inText, line);
               TmpNodeInpS.PartName=PartName;
               while(0==ret){
				   QStringList line2=line.split(",");
				   int NodeID=line2.at(0).toInt()+NodeNumber;
				   QString j33=QString::number(NodeID,10);
				   QString line3=j33+",";
				   for(int IJ=1;IJ<line2.size();IJ++){
					   if(line2.at(IJ)=="")continue;
					   line3+=line2.at(IJ);
					   if(IJ!=line2.size()-1)line3+=",";
				   }
                   TmpNodeInpS.strData.push_back(line3);
                   ret = ReadNextLine(inText, line); 
               }  
            }else if (strList.at(0)=="*element"){//Element
               int NumberEl=0;
			   int nNsetSize = strList.size();
			   TmpElInpS.PartName=PartName;
				for(int i=1;i<nNsetSize;i++){	
				   strTmp = strList.at(i);
				   if(strTmp.contains("type="))TmpElInpS.strELType=strTmp.remove("type=").simplified();
			   }
               ret=ReadNextLine(inText, line);
			   while(0==ret){
				    QStringList line2=line.split(",");
					int ElementID=line2.at(0).toInt()+ElementNumber;
					QString j33=QString::number(ElementID,10);
					QString line3=j33+",";
				    for(int IJ=1;IJ<line2.size();IJ++){
						if(line2.at(IJ)=="")continue;
						int NodeID=line2.at(IJ).toInt()+NodeNumber;
				        QString j3=QString::number(NodeID,10);
					    line3+=j3;
					    if(IJ!=line2.size()-1)line3+=",";
					}
                   NumberEl=NumberEl+1;
                   TmpElInpS.strData.push_back(line3);
                   ret = ReadNextLine(inText, line); 
               }
			   if(NumberEl!=0){
				   TmpElInpS.NumberE.push_back(NumberEl);
                   TmpElInpS.ElementType.push_back( TmpElInpS.strELType);
			   }
           }else if (strList.at(0)== "*nset"){//*Nset�ڵ㼯��
              int nNsetSize = strList.size();
			  TmpNsetInps.PartName=PartName;
				for(int i=1;i<nNsetSize;i++){
					QString Str;
					Str=strList.at(i);
					if(Str.contains("nset")){
						TmpNsetInps.strNSetName<<Str.remove("nset=", Qt::CaseInsensitive).simplified();
					}else if(Str.contains("generate")){
						TmpNsetInps.strNSetStyle<<"generate";
              }
				}
				if(!line.contains("generate"))TmpNsetInps.strNSetStyle<<"";
              ret=ReadNextLine(inText, line);
              QStringList tmpNsetData;
              while(0==ret){
				   QStringList line2=line.split(",");
				   QString j31,j32;
				   QString line3="";
				   if(nNsetSize == 3){
					   int NodeID1=line2.at(0).toInt()+NodeNumber;
					   int NodeID2=line2.at(1).toInt()+NodeNumber;
					   j31=QString::number(NodeID1,10);
					   j32=QString::number(NodeID2,10);
					   line3=j31+","+j32+","+line2.at(2);
				   }else{
					   for(int IJ=0;IJ<line2.size();IJ++){
						   if(line2.at(IJ)=="")continue;
						   int NodeID1=line2.at(IJ).toInt()+NodeNumber;
						   j31=QString::number(NodeID1,10);
						   if(IJ!=line2.size()-1)line3+=j31+",";
						   else line3+=j31;
					   } 
				   }
				   tmpNsetData<<line3;
                  ret = ReadNextLine(inText ,line);			  
              }
               TmpNsetInps.strData.push_back(tmpNsetData);
			   tmpNsetData.clear();
			}else if (strList.at(0)== "*elset"){//Elset��Ԫ����
              int nElsetSize = strList.size();
			  TmpElSetInps.PartName=PartName;
			  for(int i=1;i<nElsetSize;i++){
				  strTmp = strList.at(i);
				  if(strTmp.contains("elset")){
              TmpElSetInps.strElSetName<<strTmp.remove("elset=", Qt::CaseInsensitive).simplified();
				  }else if(strTmp.contains("generate")){
					  TmpElSetInps.strElSetStyle<<"generate";
              }
			  }
			  if(!line.contains("generate"))TmpElSetInps.strElSetStyle<<"";

              ret=ReadNextLine(inText, line);
              QString curElData;curElData.clear();
              while(0==ret){
				  QStringList line2=line.split(",");
				  QString j31,j32;
				  QString line3;
				  if(nElsetSize == 3){
					  int ElementID1=line2.at(0).toInt()+ElementNumber;
					  int ElementID2=line2.at(1).toInt()+ElementNumber;
					  j31=QString::number(ElementID1,10);
					  j32=QString::number(ElementID2,10);
					  line3=j31+","+j32+","+line2.at(2);
					  curElData =curElData+","+line3;
				  }else{
					  for(int IJ=0;IJ<line2.size();IJ++){
						  if(line2.at(IJ)=="")continue;
						  int ElementID1=line2.at(IJ).toInt()+ElementNumber;
						  j31=QString::number(ElementID1,10);
						  line3=j31+",";
						  curElData =curElData+line3;
					   } 
				  }
                ret = ReadNextLine(inText ,line); 
              }
              TmpElSetInps.strData<<curElData;
		   }else if (strList.at(0)== "*surface"){//Surface���漯��
			   TmpSurfaceInps.PartName=PartName;
               for (int kk=1;kk<strList.size();kk++){
                   strTmp = strList.at(kk);
                   if (strTmp.contains("type",Qt::CaseInsensitive)){
                     TmpSurfaceInps.strSurfaceType=strTmp.remove("type=", Qt::CaseInsensitive).simplified();
                   }else if(strTmp.contains("name",Qt::CaseInsensitive)){
                     TmpSurfaceInps.strSurfaceName<<strTmp.remove("name=", Qt::CaseInsensitive).simplified();
                   }   
               }
               ret=ReadNextLine(inText, line);
               int iTempCnt=0;
               while(0==ret){
                   iTempCnt++;
                   TmpSurfaceInps.strData<<line;
                   ret = ReadNextLine(inText ,line); 
               }
               TmpSurfaceInps.strSurfaceDataNum<<QString("%1").arg(iTempCnt);
			}else{//not record "*" KeyWord;
              ret=ReadNextLine(inText, line);
		   }
	  }//while(ret != 1)
	}//while(!inText.atEnd())

	if (InpStyle ==1){//gmsh style
		NodeELSetInps tmpReadNELSufInpS;
		if(changeGmshToNormal(tmpReadNELSufInpS,TmpGmshInps)){
			//*Node���±��Žڵ���
			QList<QString>Information;
			int j=tmpReadNELSufInpS.NodeInpData.strData.size();
			for(int i=0;i<j;i++){
				QStringList line2=tmpReadNELSufInpS.NodeInpData.strData.at(i).split(",");
				int StNumber=line2.at(0).toInt();
				int StrSise=line2.at(0).size();
				QString OldNodeNumber=line2.at(0);
				QString NodeInformation=tmpReadNELSufInpS.NodeInpData.strData.at(i);
				QString NewNodeNumber=QString::number(StNumber+NodeNumber,10);
				NodeInformation.replace(0,StrSise,NewNodeNumber);
				Information.append(NodeInformation);
				//tmpReadNELSufInpS.NodeInpData.strData.at(i).replace(0,OldNodeNumber,NewNodeNumber);
			}
			tmpReadNELSufInpS.NodeInpData.strData.clear();
			tmpReadNELSufInpS.NodeInpData.strData=Information;
			Information.clear();
			//*Element��Ϣ���±��
			j=tmpReadNELSufInpS.ELInpData.strData.size();
			for(int i=0;i<j;i++){
				QStringList line2=tmpReadNELSufInpS.ELInpData.strData.at(i).split(",");
				QString ElementINF;
				int StNumber=line2.at(0).toInt();
				ElementINF=QString::number(StNumber+ElementNumber,10);
				for(int IJ=1;IJ<line2.size();IJ++){
					QString StrL=QString::number(line2.at(IJ).toInt()+NodeNumber,10);
					ElementINF+=","+StrL;
				}
				Information.append(ElementINF);
			}
			tmpReadNELSufInpS.ELInpData.strData.clear(); 
			tmpReadNELSufInpS.ELInpData.strData=Information;
			Information.clear();
			//*Nset��Ϣ���±��
			j=tmpReadNELSufInpS.NSetInpData.strData.size();
			QList<QStringList>NsetInformation;
			for(int i=0;i<j;i++){
				QStringList line2=tmpReadNELSufInpS.NSetInpData.strData.at(i);
				QString NsetINF="";
				QList<QString>Ns;
				for(int IJ=0;IJ<line2.size();IJ++){
					QString NsetINF1="";
					QStringList Line=line2.at(IJ).split(",");
					for(int JK=0;JK<Line.size();JK++){
						if(Line.at(JK)=="")continue;
						int NodeNum=Line.at(JK).toInt()+NodeNumber;
						NsetINF1+=","+QString::number(NodeNum,10);
					}
					Ns.append(NsetINF1);
				}
				NsetInformation<<Ns;
				Ns.clear();
			}
			tmpReadNELSufInpS.NSetInpData.strData.clear();
			tmpReadNELSufInpS.NSetInpData.strData<<NsetInformation;
			NsetInformation.clear();
			//*ElSet��Ϣ���±��
			Information.clear();
			j=tmpReadNELSufInpS.ElSetInpData.strData.size();
			for(int i=0;i<j;i++){
				QString Str="";
				QStringList line2=tmpReadNELSufInpS.ElSetInpData.strData.at(i).split(",");
				for(int IJ=0;IJ<line2.size();IJ++){
					if(line2.at(IJ)=="")continue;
					int EleNu=line2.at(IJ).toInt()+ElementNumber;
					if(IJ<line2.size()-1)Str+=QString::number(EleNu,10)+",";
					else if(IJ==line2.size()-1) Str+=QString::number(EleNu,10);
				}
				Information.append(Str);
			}
			tmpReadNELSufInpS.ElSetInpData.strData.clear();
			tmpReadNELSufInpS.ElSetInpData.strData=Information;
			Information.clear();
            m_ReadInpResult.TmpNodeInpS=tmpReadNELSufInpS.NodeInpData;
            m_ReadInpResult.TmpElInpS=tmpReadNELSufInpS.ELInpData;
            m_ReadInpResult.TmpNsetInps=tmpReadNELSufInpS.NSetInpData;
            m_ReadInpResult.TmpElSetInps=tmpReadNELSufInpS.ElSetInpData;
            m_ReadInpResult.TmpSurfaceInps=tmpReadNELSufInpS.SurfaceInpData;  
        }
    }else{
        m_ReadInpResult.TmpNodeInpS=TmpNodeInpS;
        m_ReadInpResult.TmpElInpS=TmpElInpS;
        m_ReadInpResult.TmpNsetInps=TmpNsetInps;
        m_ReadInpResult.TmpElSetInps=TmpElSetInps;
        m_ReadInpResult.TmpSurfaceInps=TmpSurfaceInps;
    }
    return true;
}

//��"**"�����ж�ȡ
/*
    return: int    
            -1->��β
            0->normal data
            10->section Name
            1->one "*";
            1000->Error
*/
int CRWManage::ReadNextLine(QTextStream &TextStreamData,QString &DataLine)
{
	int Process=0;
    do{ 
        DataLine= TextStreamData.readLine().simplified();//
    } while (DataLine==""&&!TextStreamData.atEnd());
    
    if (TextStreamData.atEnd()){
        return -1;//
    }
	DataLine.remove(QRegExp("\\s"));//ɾ���ո�
	if(DataLine.right(1)==",")DataLine.chop(1);//ȥ��β���Ķ���
	DataLine=DataLine.toLower();//��ΪСд
    int iContains=DataLine.indexOf("**");
	if(DataLine.contains("**casting")){
		Process=4;
	}else if(DataLine.contains("**hip")){
		Process=5;
	}else if(DataLine.contains("**foring")){
		Process=6;
	}else if(DataLine.contains("**ht")){
		Process=7;
	}else if(DataLine.contains("**partstar")){
       Process=88;
	}

	if(Process!=0){
		return Process;
	}
	while (iContains>=0){//�ڱ���5��֮ǰ�а���2����**��
		DataLine=TextStreamData.readLine().simplified();
		iContains=DataLine.indexOf("**");
		if (TextStreamData.atEnd())return -1;
	}
	DataLine.remove(QRegExp("\\s"));//ɾ���ո�
	if(DataLine.right(1)==",")DataLine.chop(1);//ȥ��β���Ķ���
	DataLine=DataLine.toLower();//��ΪСд
	/*int iContains1=DataLine.indexOf("**thname=");
	if(iContains1>=0){
	return 3;
	}*/
	
    iContains=DataLine.indexOf("*");
    if (iContains>=0)return 1;
    return 0;
}
//gmsh->welcme inp;
bool CRWManage::changeGmshToNormal(NodeELSetInps &saveData,GmshInpS &curData)
{
/*QTime t120;
	t120.start();*/
	QMap<QString,int>NodeNu,ElementEu;
	NodeNu.clear();
	ELInpS tmpELdata;

    int styleType=0;//��
    tmpELdata.strELTitle="element";
    int nSize=curData.gmshELData.size();
    tmpELdata.strELType = curData.gmshELData.at(nSize-1).strELType;
	styleType = getCellType(tmpELdata.strELType);

	//NodeELSetInps saveData1;
    //1.--node the same
	saveData.NodeInpData=curData.NodeInpData;
	saveData.NodeInpData.strData.clear();
	//���¼���ʹ*NODE�ڵ��Ŵ�1��ʼ
	for(int i=0;i<curData.NodeInpData.strData.size();i++){
        QStringList str=curData.NodeInpData.strData.at(i).split(",");
		QString str01=str.at(0);
		int Nu=str01.size()+1;
		QString wef="";
		if(styleType==1&&str.at(str.size()-1)!=""){
			QString str02=str.at(str.size()-1);
			int Num0=str02.size()+1;
			int Num1=curData.NodeInpData.strData.at(i).size()-Num0-Nu;
			wef=curData.NodeInpData.strData.at(i).mid(Nu,Num1);
		}else{
			wef=curData.NodeInpData.strData.at(i).mid(Nu);
		}
		saveData.NodeInpData.strData.append(QString::number(i+1,10)+","+wef);
		//saveData.NodeInpData.strData.append(QString::number(i+1,10)+","+curData.NodeInpData.strData.at(i).mid(Nu));
        NodeNu.insert(str01,i+1);
	}
	//���ϼ���ʹ*NODE�ڵ��Ŵ�1��ʼ

    //2.--nset the same
	saveData.NSetInpData=curData.gmshNSetData;
	saveData.NSetInpData.strData.clear();
	for(int i=0;i<curData.gmshNSetData.strData.size();i++){
        QStringList BCD2;
		BCD2.clear();
		QStringList BCD=curData.gmshNSetData.strData.at(i);
		for(int j=0;j<BCD.size();j++){
			QStringList fs=BCD.at(j).split(",",QString::SkipEmptyParts);
			QString NStr="";
			for(int k=0;k<fs.size();k++){
				QString st02=fs.at(k).simplified();
				int afwsw=NodeNu.value(st02);//ʹ*NSET�нڵ�����*NODE�нڵ����ͬ
                if(k!=fs.size()-1)NStr+=QString::number(afwsw,10)+",";
				else NStr+=QString::number(afwsw,10);
			}
			BCD2.append(NStr);
		}
		saveData.NSetInpData.strData.append(BCD2);
	}
    //3.--element (saveData.ELInpData = tmpELdata)
    QMap<QString, QString> ElementCmp; 
	QMap<QString, QString> SurfElement;
	
	if(styleType==1||styleType==2){
		for (int kk=0;kk<nSize;kk++){
			QList<QString> ElementN;
			ElementN.clear();
			int tmpType = getCellType(curData.gmshELData.at(kk).strELType);
			//���¼���ʵ��*ELEMENT��Ԫ��Ϣ�нڵ�����*NODE�нڵ���ͬ��
			int NumberE1=curData.gmshELData.at(kk).strData.size();
			for(int ijk=0;ijk<NumberE1;ijk++){
				QString wva=curData.gmshELData.at(kk).strData.at(ijk);
				QStringList stl23=wva.split(",",QString::SkipEmptyParts);
				QString strFirst=stl23.at(0).simplified();
				QString NStr="";
				for(int ijk2=1;ijk2<stl23.size();ijk2++){
					QString stll=stl23.at(ijk2).simplified();
					int afwsw=NodeNu.value(stll);//ʹ*ELEMENT�нڵ�����*NODE�нڵ���ͬ��
					if(ijk2==1)NStr+=QString::number(afwsw,10);
					else NStr+=","+QString::number(afwsw,10);
				}
				NStr=strFirst+","+NStr;
				if(tmpType==1&&styleType==1||tmpType==2&&styleType==2){
					ElementN.append(NStr);
				}else if(tmpType==0&&styleType==1||tmpType==1&&styleType==2){
					NStr=sort(NStr.split(",").size(),NStr);
					ElementCmp.insert(strFirst,NStr);//������浥Ԫ��Ϣ
				}
				//tmpELdata32.strData.append (NStr);
			}
			//���ϼ���ʵ��*ELEMENT��Ԫ��Ϣ�нڵ�����*NODE�нڵ���ͬ��

			if(tmpType==1&&styleType==1||tmpType==2&&styleType==2){
				ELInpS tmpELdata32;
				tmpELdata32.clear();
				tmpELdata.strELSET = curData.gmshELData.at(0).strELSET;
				tmpELdata.strData<<ElementN;
				tmpELdata.ElementType.append(curData.gmshELData.at(kk).strELType);//luo
				int NumberE2=curData.gmshELData.at(kk).strData.size();
				tmpELdata.NumberE.append(NumberE2);//luo
			}         
		}
	} 
	//4.---bellow set index from 1,2,3...
	int Nsize;
	int IJKL=0;
	Nsize=tmpELdata.strData.size();
	ELInpS tmpELdata1=tmpELdata;
	tmpELdata1.strData.clear();
	//int IJKL0=tmpELdata1.NumberE.size();
	int suM=tmpELdata1.NumberE.at(0)-1;
	for(int kk=0;kk<Nsize;kk++)
	{
		int ElementNumber;
		if(kk>suM&&(IJKL+1)<tmpELdata1.ElementType.size()){
			IJKL+=1;
			suM=suM+tmpELdata1.NumberE.at(IJKL);	
		}
		QString ElementNumber2,Str2="",Str3="";
		QStringList Str;
		Str=tmpELdata.strData.at(kk).split(",",QString::SkipEmptyParts);
		ElementNumber=kk+1;
		ElementEu.insert(Str.at(0),ElementNumber);//���±��ǰ���ӳ���ϵ
		QString j33=QString::number(ElementNumber,10);//������ת����QString
		for(int j=1;j<Str.size();j++){
			if (j==1)Str2+=j33+","+Str.at(j);
			else Str2+=","+Str.at(j);
		}
		QString StrLL=FaceNumber(tmpELdata1.ElementType.at(IJKL),j33,Str);//�ҳ�ÿ����Ԫ�����ҽ���������ʾ
		//QString StrLL=FaceNumber(tmpELdata1.strELType,j33,Str);//�ҳ�ÿ����Ԫ�����ҽ���������ʾ
		Str=StrLL.split("==");
		for(int ij=0;ij<Str.size();ij++){
			QStringList Str21=Str.at(ij).split("=",QString::SkipEmptyParts);
			if(SurfElement.contains(Str21.at(1)))SurfElement.remove(Str21.at(1));
			else SurfElement.insert(Str21.at(1),Str21.at(0));//����ÿ����Ԫ������
		}
		tmpELdata1.strData.push_back(Str2);
	}
	saveData.ELInpData = tmpELdata1; 
	tmpELdata1.clear();

	if (styleType == 2||styleType == 1){//2d/3d
        ElSetInpS gElSetInpSData;
        gElSetInpSData=curData.gmshELSetData;
		for (int kk=0; kk<gElSetInpSData.strData.size(); kk++){//��Ԫ�����м���������ѭ�� 
			QString STL2_0;
            QStringList strListGElSet=gElSetInpSData.strData.at(kk).split(",", QString::SkipEmptyParts);
            int iDataNum=0;bool bFlag=false;
            QList<QString> strListData;
            QString strDataSaveOneName;
			for (int mm=0; mm<strListGElSet.size(); mm++){ //��Ԫ�����е�Ԫ������ѭ�� 
                QString strId = strListGElSet.at(mm).simplified(); 
                int iSn=0;
                QCoreApplication::processEvents();
				if(ElementCmp.find(strId)!= ElementCmp.end()){//����ElementCmp.contains(strId)
					QString SNode=ElementCmp.value(strId);
					QString SElement=SurfElement[SNode];
					QStringList STL2=SElement.split("_");
					STL2_0=STL2.at(0);
					strDataSaveOneName=gElSetInpSData.strElSetName.at(kk)+"_s"+STL2.at(1);
					int idSurf=saveData.ElSetInpData.strElSetName.indexOf(strDataSaveOneName);
					if (idSurf!=-1){
						QString strLast= saveData.ElSetInpData.strData.at(idSurf)+",";
						strLast+= STL2.at(0);
						saveData.ElSetInpData.strData[idSurf]=strLast;//.replace(idSurf,strLast);
					}else{
						saveData.ElSetInpData.strElSetStyle<<gElSetInpSData.strElSetStyle;
						saveData.ElSetInpData.strElSetName<<strDataSaveOneName;
						saveData.ElSetInpData.strData<< STL2.at(0);
						strListData<<strDataSaveOneName+",s"+ STL2.at(1);
						iDataNum++;
					}
					bFlag=true;
				}else{//������
					int tmpELsize=tmpELdata.strData.size();	
					QStringList STlo=curData.gmshELSetData.strData.at(kk).split(",");
					int siz=STlo.size();
					QString BD="";
					for (int nn=0;nn<tmpELsize;nn++){
						if (strId==tmpELdata.strData.at(nn).split(",",QString::SkipEmptyParts).at(0).simplified()){
							saveData.ElSetInpData.strElSetStyle<<curData.gmshELSetData.strElSetStyle;
							saveData.ElSetInpData.strElSetName<<curData.gmshELSetData.strElSetName.at(kk);
							for(int ijk=0;ijk<siz;ijk++){
								int LIU=ElementEu.value(STlo.at(ijk));
								if(LIU<=0)continue;
								if(ijk!=siz)BD+=QString::number(LIU,10)+",";
								else BD+=QString::number(LIU,10);
							}
							saveData.ElSetInpData.strData<<BD;
							break;
						} 
					}
					break;
				}
			}
            if ( bFlag==true){ 
                saveData.SurfaceInpData.strSurfaceType="element";
                saveData.SurfaceInpData.strSurfaceName<<curData.gmshELSetData.strElSetName.at(kk); 
                saveData.SurfaceInpData.strSurfaceDataNum<<QString::number(iDataNum,10);
                saveData.SurfaceInpData.strData<<strListData;
            }
		}
	}
	//���¼�������ɾ�����漯�Ͻ������ɵĽڵ㼯��
	for(int i=0;i<saveData.SurfaceInpData.strSurfaceName.size();i++){
		QString surfName=saveData.SurfaceInpData.strSurfaceName.at(i);
		for(int j=0;j<saveData.NSetInpData.strNSetName.size();j++){
			QString NsetName=saveData.NSetInpData.strNSetName.at(j);
			if(surfName==NsetName){
				saveData.NSetInpData.strNSetName.removeAt(j);
				saveData.NSetInpData.strData.removeAt(j);
				saveData.NSetInpData.strNSetStyle.removeAt(j);
			}
		}
		
	}
	//���ϼ�������ɾ�����漯�Ͻ������ɵĽڵ㼯��

	//���¼�������ɾ�����漯�Ͻ������ɵĵ�Ԫ����
	/*for(int i=0;i<saveData.SurfaceInpData.strData.size();i++){
		QString surfName=saveData.SurfaceInpData.strData.at(i).split(",",QString::SkipEmptyParts).at(0);
		for(int j=0;j<saveData.ElSetInpData.strElSetName.size();j++){
			QString ELsetName=saveData.ElSetInpData.strElSetName.at(j);
			if(surfName==ELsetName){
				saveData.ElSetInpData.strElSetName.removeAt(j);
				saveData.ElSetInpData.strData.removeAt(j);
				saveData.ElSetInpData.strElSetStyle.removeAt(j);
			}
		}
	}*/
	//���ϼ�������ɾ�����漯�Ͻ������ɵĵ�Ԫ����

	//float time=t120.elapsed()/1000.0;
    return true;
}
//gmsh->get type;
//intcelltype = 0 �� 
//            = 1 ��
//            = 2 ��
int CRWManage::getCellType(QString strType)
{
    int iCellType=0;
    if (strType=="c3d6"){iCellType = 2;
    }else if(strType=="c3d15"){iCellType = 2;
    }else if(strType=="c3d8"||strType=="c3d8r"){iCellType = 2;
    }else if(strType=="c3d20"||strType=="c3d20r"){iCellType = 2;
    }else if(strType=="cps3"||strType=="cpe3"||strType=="cax3"){iCellType = 1;
    }else if(strType=="cps6"||strType=="cpe6"||strType=="cax6"){iCellType = 1;
    }else if(strType=="cps4"||strType=="cps4r"||strType=="cpe4"||strType=="cpe4r"||
        strType=="cax4"||strType=="cax4r"){iCellType = 1;
    }else if (strType=="cps8"||strType=="cps8r"||strType=="cpe8"||strType=="cpe8r"||
        strType=="cax8"||strType=="cax8r"){iCellType = 1;
    }else if(strType=="c3d4"||strType=="c3d4t"){iCellType = 2;
    }else if(strType=="c3d10"||strType=="c3d10t"){iCellType = 2;
    }
    return iCellType;
}
//--�Ƿ���ڣ������ĸ���
//--��ԭstr��ȥ��index��
//--surf,�����ĸ���S1��S2��S3��S4...
bool CRWManage::ELinclude(QString strType, QString strSrc, QString strDis, int &iSurf)
{
    QStringList strSrcList = strSrc.split(",", QString::SkipEmptyParts);
    QStringList strDisList = strDis.split(",", QString::SkipEmptyParts);
    int nSrc=strSrcList.size()-1;
    int nDis=strDisList.size();
    if (nDis > nSrc) return false;
    QList<int> indX;
    for (int kk=0; kk<nDis; kk++){
        int id=strSrcList.indexOf(strDisList.at(kk));
        if (id==-1)return false;//û���ҵ��������ڸ�element����;
        else if (id==0)continue;//����,��Ϊ��һ����ţ������Ѱ��;
        indX.append(id);
    }
    qSort(indX.begin(),indX.end());
    int iBaseNum=0;
    if (strType=="c3d6"){iBaseNum = 4;//3������4��
    }else if(strType=="c3d15"){iBaseNum = 8;//6����8��
    }else if(strType=="c3d8"||strType=="c3d8r"){//4����
        iBaseNum = 4;
        iSurf=CalcSn(indX,iBaseNum,strType);
    }else if(strType=="c3d20"||strType=="c3d20r"){iBaseNum = 8;
    }else if(strType=="cps3"||strType=="cpe3"||strType=="cax3"){//�����Σ������㣬ÿ����2���㣩
        iBaseNum = 2;
        iSurf=CalcSn(indX,iBaseNum,strType);
    }else if(strType=="cps6"||strType=="cpe6"||strType=="cax6"){iBaseNum = 3;
    }else if(strType=="cpe4"||strType=="cps4r"||strType=="cpe4"||strType=="cpe4r"||strType=="cax4"||strType=="cax4r"){//4����
        iBaseNum = 2;
        iSurf=CalcSn(indX,iBaseNum,strType);
    }else if (strType=="cpe8"||strType=="cps8r"||strType=="cpe8"||strType=="cpe8r"||
        strType=="cax8"||strType=="cax8r"){iBaseNum = 3;
    }else if(strType=="c3d4"||strType=="c3d4t"){
        iBaseNum = 3;
        iSurf=CalcSn(indX,iBaseNum,strType);
    }else if(strType=="c3d10"||strType=="c3d10t"){iBaseNum = 6;
    } 
    if (iSurf==0){
        return false;
    }
    return true;
}
//--������ж������ĸ��棻
int CRWManage::CalcSn(QList<int> indX,int Num,QString strType)
{
    int iSurf=0;
    if(indX.size()!=Num)return false;
    switch(Num){
    case 2:{//cps4,cps3 -> �����Ρ��ı���
        if (strType=="cps4"){
            if (indX[0]==1&& indX[1]==2){
                iSurf=1;
            }else if (indX[0]==2&& indX[1]==3){
                iSurf=2;
            }else if (indX[0]==3&& indX[1]==4){
                iSurf=3;
            }else if (indX[0]==1&& indX[1]==4){
                iSurf=4;
            }
        }else if (strType =="cps3"){
            /*if (indX.contains(1) && indX.contains(2)){iSurf=1; 
            }else if (indX.contains(2) && indX.contains(3)){iSurf=2;
            }else if (indX.contains(3) && indX.contains(1)){iSurf=3;
            }   */ 
            if (indX[0]==1&& indX[1]==2){
                iSurf=1;
            }else if (indX[0]==2&& indX[1]==3){
                iSurf=2;
            }else if (indX[0]==1&& indX[1]==3){
                iSurf=3;
            }
        }
      }break;
    case 4: {//c3d8->������
       /* if (indX.contains(1) && indX.contains(2)&& indX.contains(3)&& indX.contains(4)){iSurf=1; 
        }else if (indX.contains(5) && indX.contains(6)&& indX.contains(7)&& indX.contains(8)){iSurf=2;
        }else if (indX.contains(1) && indX.contains(5)&& indX.contains(6)&& indX.contains(2)){iSurf=3;
        }else if (indX.contains(2) && indX.contains(6)&& indX.contains(7)&& indX.contains(3)){iSurf=4;     
        }else if (indX.contains(3) && indX.contains(7)&& indX.contains(8)&& indX.contains(4)){iSurf=5;     
        }else if (indX.contains(4) && indX.contains(8)&& indX.contains(5)&& indX.contains(1)){iSurf=6;     
        }   */
        if (indX[0]==1&& indX[1]==2&& indX[2]==3&& indX[3]==4){
            iSurf=1;
        }else if (indX[0]==1&& indX[1]==4&& indX[2]==5&& indX[3]==8){
            iSurf=6;
        }else if (indX[0]==1&& indX[1]==2&& indX[2]==5&& indX[3]==6){
            iSurf=3;
        }else if (indX[0]==2&& indX[1]==3&& indX[2]==6&& indX[3]==7){
            iSurf=4;
        }else if (indX[0]==3&& indX[1]==4&& indX[2]==7&& indX[3]==8){
            iSurf=5;
        }else if (indX[0]==5&& indX[1]==6&& indX[2]==7&& indX[3]==8){
            iSurf=2;
        }
       /* if (indX.contains(1) && indX.contains(2)&& indX.contains(3)&& indX.contains(4)){iSurf=1; 
        }else if (indX.contains(5) && indX.contains(6)&& indX.contains(7)&& indX.contains(8)){iSurf=2;
        }else if (indX.contains(1) && indX.contains(5)&& indX.contains(6)&& indX.contains(2)){iSurf=3;
        }else if (indX.contains(2) && indX.contains(6)&& indX.contains(7)&& indX.contains(3)){iSurf=4;     
        }else if (indX.contains(3) && indX.contains(7)&& indX.contains(8)&& indX.contains(4)){iSurf=5;     
        }else if (indX.contains(4) && indX.contains(8)&& indX.contains(5)&& indX.contains(1)){iSurf=6;     
        }   */
      }break;
    case 3:{//c3d4->������
            if (indX.contains(1) && indX.contains(2)&& indX.contains(3)){iSurf=1; 
            }else if (indX.contains(1) && indX.contains(4)&& indX.contains(2)){iSurf=2;
            }else if (indX.contains(2) && indX.contains(4)&& indX.contains(3)){iSurf=3;
            }else if (indX.contains(3) && indX.contains(4)&& indX.contains(1)){iSurf=4; 
            }
      }break;
    case 6:
        break;
    case 8:
        break;
    default:break;
    }
     
    return iSurf;
}


QString CRWManage::sort(int number,QString Str)
{
	if (number > 25) number = 25;
	int numberL[25];
	QString Str1="";
	int temp;
	QStringList parts = Str.split(",");
	int safeNum = qMin(number, parts.size());
	for(int i=0;i<safeNum;i++){
		numberL[i]=parts.at(i).toInt();
	}
	while (safeNum < number) {
		numberL[safeNum] = 0;
		safeNum++;
	}

	for(int JJ=1;JJ<number-1;JJ++){
		for(int JJI=JJ+1;JJI<number;JJI++){
			QString BUS;
			if(numberL[JJ]>numberL[JJI]){
				temp=numberL[JJ];
				numberL[JJ]=numberL[JJI];
				numberL[JJI]=temp;
			}
		}
	}

	for(int i=1;i<number;i++){
		if(i!=number-1)Str1+=QString::number(numberL[i],10)+",";
		else Str1+=QString::number(numberL[i],10);
	}
	return Str1;
}

QString CRWManage::FaceNumber(QString EType,QString ENumber,QStringList NnodeNumber)
{
	QString SurfElement;
    int iBaseNum=0;
    if (EType=="c3d6"){iBaseNum = 4;//3������4��
    }else if(EType=="c3d15"){iBaseNum = 8;//6����8
    }else if(EType=="c3d8"||EType=="c3d8r"){//4����
        iBaseNum = 4;
       SurfElement=CalcSn2(iBaseNum,EType,ENumber,NnodeNumber);
    }else if(EType=="c3d20"||EType=="c3d20r"){iBaseNum = 8;
    }else if(EType=="cps3"||EType=="cpe3"||EType=="cax3"){//�����Σ������㣬ÿ����2���㣩
        iBaseNum = 2;
       SurfElement=CalcSn2(iBaseNum,EType,ENumber,NnodeNumber);
    }else if(EType=="cps6"||EType=="cpe6"||EType=="cax6"){iBaseNum = 3;
    }else if(EType=="cps4"||EType=="cps4r"||EType=="cpe4"||EType=="cpe4r"||EType=="cax4"||EType=="cax4r"){//4����
        iBaseNum = 2;
        SurfElement=CalcSn2(iBaseNum,EType,ENumber,NnodeNumber);
    }else if (EType=="cps8"||EType=="cps8r"||EType=="cpe8"||EType=="cpe8r"||
        EType=="cax8"||EType=="cax8r"){iBaseNum = 3;
    }else if(EType=="c3d4"||EType=="c3d4T"||EType=="c3d10"){
        iBaseNum = 3;
        SurfElement=CalcSn2(iBaseNum,EType,ENumber,NnodeNumber);
    }else if(EType=="c3d10"||EType=="c3d10t"){iBaseNum = 6;
	} 
	return SurfElement;
}	

QString CRWManage::CalcSn2(int Num,QString strType,QString ENumber,QStringList NnodeNumber)
{
	QString SurfElement="";
	QString  Str3="",Str="";
	switch(Num){
	case 2:{//cps4,cps3 -> �����Ρ��ı���
		if (strType=="cps4"||strType=="cps4r"||strType=="cpe4"||strType=="cpe4r"||strType=="cax4"||strType=="cax4r"){
			//1��
			Str3=ENumber+","+NnodeNumber.at(1)+","+NnodeNumber.at(2);
			Str3=sort(Str3.split(",").size(),Str3);
			Str=ENumber+"_1";
			SurfElement=Str+"="+Str3;
			//2��
			Str3=ENumber+","+NnodeNumber.at(2)+","+NnodeNumber.at(3);
			Str3=sort(Str3.split(",").size(),Str3);
			Str=ENumber+"_2";
			SurfElement+="=="+Str+"="+Str3;
			//3��
			Str3=ENumber+","+NnodeNumber.at(3)+","+NnodeNumber.at(4);
			Str3=sort(Str3.split(",").size(),Str3);
			Str=ENumber+"_3";
			SurfElement+="=="+Str+"="+Str3;
			//4��
			Str3=ENumber+","+NnodeNumber.at(4)+","+NnodeNumber.at(1);
			Str3=sort(Str3.split(",").size(),Str3);
			Str=ENumber+"_4";
			SurfElement+="=="+Str+"="+Str3;

		}else if (strType =="cps3"||strType=="cpe3"||strType=="cax3"){
			//1��
			Str3=ENumber+","+NnodeNumber.at(1)+","+NnodeNumber.at(2);
			Str3=sort(Str3.split(",").size(),Str3);
			Str=ENumber+"_1";
			SurfElement=Str+"="+Str3;
			//2��
			Str3=ENumber+","+NnodeNumber.at(2)+","+NnodeNumber.at(3);
			Str3=sort(Str3.split(",").size(),Str3);
			Str=ENumber+"_2";
			SurfElement+="=="+Str+"="+Str3;
			//3��
			Str3=ENumber+","+NnodeNumber.at(3)+","+NnodeNumber.at(1);
			Str3=sort(Str3.split(",").size(),Str3);
			Str=ENumber+"_3";
			SurfElement+="=="+Str+"="+Str3;
		}
		   }break;
	case 3: {//c3d4->������
		//1��
		Str3=ENumber+","+NnodeNumber.at(1)+","+NnodeNumber.at(2)+","+NnodeNumber.at(3);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_1";
		SurfElement=Str+"="+Str3;
		//2��
		Str3=ENumber+","+NnodeNumber.at(1)+","+NnodeNumber.at(4)+","+NnodeNumber.at(2);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_2";
		SurfElement+="=="+Str+"="+Str3;
		//3��
		Str3=ENumber+","+NnodeNumber.at(2)+","+NnodeNumber.at(4)+","+NnodeNumber.at(3);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_3";
		SurfElement+="=="+Str+"="+Str3;
		//4��
		Str3=ENumber+","+NnodeNumber.at(3)+","+NnodeNumber.at(4)+","+NnodeNumber.at(1);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_4";
		SurfElement+="=="+Str+"="+Str3;
			}break;
	case 4: {//c3d8->������
		//1��
		Str3=ENumber+","+NnodeNumber.at(1)+","+NnodeNumber.at(2)+","+NnodeNumber.at(3)+","+NnodeNumber.at(4);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_1";
		SurfElement=Str+"="+Str3;
		//2��
		Str3=ENumber+","+NnodeNumber.at(5)+","+NnodeNumber.at(8)+","+NnodeNumber.at(7)+","+NnodeNumber.at(6);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_2";
		SurfElement+="=="+Str+"="+Str3;
		//3��
		Str3=ENumber+","+NnodeNumber.at(1)+","+NnodeNumber.at(5)+","+NnodeNumber.at(6)+","+NnodeNumber.at(2);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_3";
		SurfElement+="=="+Str+"="+Str3;
		//4��
		Str3=ENumber+","+NnodeNumber.at(2)+","+NnodeNumber.at(6)+","+NnodeNumber.at(7)+","+NnodeNumber.at(3);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_4";
		SurfElement+="=="+Str+"="+Str3;
		//5��
		Str3=ENumber+","+NnodeNumber.at(3)+","+NnodeNumber.at(7)+","+NnodeNumber.at(8)+","+NnodeNumber.at(4);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_5";
		SurfElement+="=="+Str+"="+Str3;
		//6��
		Str3=ENumber+","+NnodeNumber.at(4)+","+NnodeNumber.at(8)+","+NnodeNumber.at(5)+","+NnodeNumber.at(1);
		Str3=sort(Str3.split(",").size(),Str3);
		Str=ENumber+"_6";
		SurfElement+="=="+Str+"="+Str3;
			}break;
	default:break;
	}
	return SurfElement;
}
