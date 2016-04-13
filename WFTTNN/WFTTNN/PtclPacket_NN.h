#pragma once

#include "PtclPacket.h"

static const int START_FLAG_FRAME = 0x68;
static const int END_FLAG_FRAME = 0x16;

class PTCLCODEC_EXPORT CPtclPacket_NN : public CPtclPacket
{
public:
	CPtclPacket_NN();
	virtual ~CPtclPacket_NN();

	//�����û����� 
	//ע�� �˴��û�������ָ�� ������ �� У��� ֮�������

	//���ú�packet�������ԣ����ɱ��ķ���m_bufPacket��
	virtual void formatPacket();
	/*
	�������Զ����㣬��Ҫ��д����������վID���ն˵�ַ��APN��������
	*/


	//���ĳ��Ȳ���:0 
	//���ĸ�ʽ����:<0
	//��ȡ���ĳɹ�:���ĳ���
	virtual int parsePacket();


	//�ն˵�ַ
	virtual unsigned int getDeviceAddr();
	void setDeviceAddr(unsigned int deviceAddr);

	/////////////////////////////////////////////////////////
	//��վ��ַ
	virtual unsigned char getHostID();
	void setHostID(unsigned char ID);

	///////////////////////////////////////////////////////////
	//��������
	unsigned char getCtrlCodeArea();
	void setCtrlCodeArea(unsigned char ctrlCodeArea);
	//��������
	unsigned char getAFN_Ctrl();
	void setAFN_Ctrl(unsigned char AFN_Ctrl);
	//֡������Чλ
	unsigned char getFCV();
	void setFCV(unsigned char FCV);
	//֡����λ ���� �������λ
	unsigned char getFCB_ACD();
	void setFCB_ACD(unsigned char FCB_ACD);
	//������־λ
	unsigned char getPRM();
	void setPRM(unsigned char PRM);
	//���䷽��λ
	unsigned char getDIR();
	void setDIR(unsigned char DIR);

	//Ӧ�ò㹦����
	unsigned char getAFN();
	void setAFN(unsigned char AFN);

	//������
	unsigned char getSeqArea();
	void setSeqArea(unsigned char seqArea);
	//֡���
	virtual int getFrameSeq();
	void setFrameSeq(unsigned char seq);
	//����ȷ�ϱ�־λ
	unsigned char getCON();
	void setCON(unsigned char CON);
	//����֡��־
	unsigned char getFIN();
	void setFIN(unsigned char FIN);
	//��֡��־
	unsigned char getFRI();
	void setFRI(unsigned char FRI);
	//֡ʱ���ǩ��Чλ
	unsigned char getTPV();
	void setTPV(unsigned char TPV);

	int getAUXLen();
protected:
	
#pragma pack(push, 1)
//����ͷ�ṹ��
	struct FrameHeaderStruct
	{
		unsigned char frameStart;//0x68
		unsigned short int lenDataArea;//����L �����򡢵�ַ���û����ݳ��ȵ��ֽ�����
		unsigned short int lenDataArea_;
		unsigned char dataAreaStart;//0x68
		union 
		{
			struct
			{
				unsigned char	AFN_Ctrl : 4 ,//D0--D3����������
                                FCV		 : 1 ,//D4֡������Чλ
                                FCB_ACD	 : 1 ,//D5֡����λ ���� �������λ
                                PRM		 : 1 ,//D6������־λ
                                DIR		 : 1 ;//D7���䷽��λ
			};
			unsigned char ctrlCodeArea;
		};
		unsigned char deviceAddr[6];//�ն��߼���ַ
		unsigned char hostID;
		unsigned char AFN;
		union 
		{
			struct
			{
				unsigned char    frameSeq	:	 4 ,//D0--D3֡���
                                 CON		:	 1 ,//D4����ȷ�ϱ�־λ
                                 FIN		:	 1 ,//D5����֡��־
                                 FRI		:	 1 ,//D6��֡��־
                                 TPV		:	 1 ;//D7֡ʱ���ǩ��Чλ
			};
			unsigned char seqArea;
		};
	};




//�¼��������ṹ
	struct ECStruct
	{
		unsigned char importantEC;
		unsigned char genericEC;
	};

//ʱ���ʶ�ṹ
	struct TPStruct
	{
		unsigned char sec;
		unsigned char min;
		unsigned char hour;
		unsigned char day;
		unsigned char timeOut;
	};


#pragma pack(pop)

	FrameHeaderStruct m_frameHeader;

	unsigned char getCheckSum(unsigned char *dateBuf,int dateBufLen);
};


