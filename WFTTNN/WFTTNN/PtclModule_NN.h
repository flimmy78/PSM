#pragma once


#include "PtclModule.h"
#include "PtclPacket_NN.h"
#pragma warning (disable:4251)

class PTCLCODEC_EXPORT CPtclModule_NN : public CPtclModule
{
public:
	CPtclModule_NN(void);
	virtual ~CPtclModule_NN(void);

	

	//Ӧ�ò㹦���붨��
	enum AFN_Code
	{
		AFN_ACK						=	0x00, //ȷ�� ����
		AFN_CONNECT_TEST			=	0x02, //��·�ӿڼ��
		AFN_SET_PARAM				=	0x04, //д����
		AFN_ID_PSW					=	0x06, //���ȷ�ϼ���ԿЭ��
		AFN_READ_PARAM				=	0x0A, //������
		AFN_READ_DATA_REALTIME		=	0x0C, //����ǰ����
		AFN_READ_DATA_HISTORY		=	0x0D, //����ʷ����
		AFN_READ_EVENT_RECORD		=	0x0E, //���¼���¼
		AFN_FILE_TRANS				=	0x0F, //�ļ�����
		AFN_RELAY_STATION_COMMAND	=	0x10, //�м�ת��
		AFN_READ_DATA_TASK		    =	0x12, //����������
		AFN_READ_DATA_EVENT			=	0x13, //���澯����
		AFN_CASCADE_COMMAND			=	0x14, //��������
		
	};

	

	//���ݵ�Ԫ�ṹ
	struct PnFnStruct
	{
		unsigned char Pn[2];
		unsigned char Fn[4];
	};

	//���ݵ�Ԫ
	struct FormatPnFnData
	{
		int Pn;
		int Fn;
	};

	//��Ϣ���ʶ
	struct ParsePnFnData
	{
		int vPn[64];
		int nCountPn;
		int vFn[8];
		int nCountFn;
	};

	static int FormatPnFn(CPtclModule_NN::FormatPnFnData *pPnFn ,unsigned char *buffer);
	static int ParsePnFn(unsigned char *buffer,ParsePnFnData *pPnFn);
	static int ParsePn(unsigned char *buffer,int &i_pn);

	//ʱ���ʶ
	struct TPData
	{		
		unsigned char day;//��
		unsigned char hour;//ʱ
		unsigned char min;//��
		unsigned char sec;//��
		unsigned char time_out;//��ʱʱ��
	};
	//ʱ���ǩ
	static int FormatTP(TPData *tp,unsigned char *buffer);
	static int ParseTP(unsigned char *buffer,TPData *tp);



	//�¼�������
	struct ECData
	{
		unsigned char importantEC;//��Ҫ�¼�������
		unsigned char genericEC;//��ͨ�¼�������
	};

	//�¼��������ṹ
	struct ECStruct
	{
		unsigned char importantEC;
		unsigned char genericEC;
	};

	//��ʽ���¼�������
	static int FormatEC(ECData *pEC,unsigned char *pECBuf);
	//�����¼�������
	static int ParseEC(unsigned char *pECBuf,ECData *pEC);


///////////////////////////////////////////////////////////////////////////////
	static CPtclModule_NN *getInstance();

	//���õ��������� //0:��վ,�ɼ�ʹ�� 1:�ն�ʹ��
	void setHostID(unsigned char hostID);

////////////////////////////////////////////////////////////////////////////////////////////////
	

////ȷ�Ϸ���///////////////////////////////////////////////////////////////////////////
	//ȫ��ȷ�� ȫ������
	void FormatPacket_Ack_All(	CPtclPacket_NN *pPacket,
								bool confirm_deny,
								TPData *pTP = NULL,
								ECData *pEC = NULL);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ����Ӧ�����
			 ���ҷ�����װ�õı���
	confirm_deny: 1ȷ�� 0����
	*/

	//����ȷ�� ����
	struct FormatAckDataUnit
	{
		FormatPnFnData PnFn;
		unsigned char error;
	};
	void FormatPacket_Ack_Part(	CPtclPacket_NN *pPacket,
								unsigned char bAckAFN,
								FormatAckDataUnit *vAck,int countAck,
								TPData *pTP = NULL,
								ECData *pEC = NULL);
	/*pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ����Ӧ�����
			 ���ҷ�����װ�õı���
	bAckAFN: ��ȷ��֡��AFN
	vAck:��ȷ�����ݵ�Ԫ����
	*/

	struct ParseAckDataUnit
	{
		ParsePnFnData PnFn;
		unsigned char error;
	};
	//����ȷ�Ϸ��ϱ���
	int ParsePacket_Ack(unsigned char *pDataAreaBuf,int lenDataArea,
						int &Fn,int &AFN,CPtclModule_NN::ParseAckDataUnit *vAck,int &countAck);
	/*pDataAreaBuf ��������  lenDataArea �������ĳ���
	  FN ���ݵ�ԪFn ȫ��ȷ�� 2ȫ������  3���ַ���
	  vAck ���ַ���ʱ��ķ������ݵ�Ԫ����

	*/

////��λ����///////////////////////////////////////////////////////////////////////////
	void FormatPacket_Reset(	CPtclPacket_NN *pPacket,
								int Fn,
								unsigned char *pPSW,
								TPData *pTP = NULL);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ
			 ���ҷ�����װ�õı���
	Fn: 1:Ӳ����ʼ�� 2:��������ʼ�� 3:��������ȫ����������ʼ��
	pPSW:�ն˷�������
	*/

////���� ��½ ע��///////////////////////////////////////////////////////////////////////////
	void FormatPacket_ConnectTest(	CPtclPacket_NN *pPacket,int Fn);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ
			 ��վ->������ ��վIDΪͨѶID �ն˵�ַ��3�ֽ���д��վID 
			 ���ҷ�����װ�õı���
	Fn:1:��½ 2:ע�� 3:����
	*/

////���ò���///////////////////////////////////////////////////////////////////////////
	struct DataUint
	{
		FormatPnFnData PnFn;
		unsigned char pDataBuf[1024];
		int lenDataBuf;
	};
	void FormatPacket_SetParam(	CPtclPacket_NN *pPacket,
								DataUint* pDataUint,int nCountDataUnit,
								unsigned char *pPSW,
								TPData *pTP = NULL);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ
			 ���ҷ�����װ�õı���
	pDataUint:�������ݵ�Ԫ����
	*/

////��������///////////////////////////////////////////////////////////////////////////
	void FormatPacket_Command(	CPtclPacket_NN *pPacket,
								CPtclModule_NN::DataUint* pDataUint,int nCountDataUnit,
								unsigned char *pPSW,
								TPData *pTP = NULL);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ
			 ���ҷ�����װ�õı���
	pDataUint:�����������ݵ�Ԫ����
	*/

////��ѯ����///////////////////////////////////////////////////////////////////////////
	void FormatPacket_ReadParam(	CPtclPacket_NN *pPacket,
									FormatPnFnData *pPnFn,int nCountPnFn,
									TPData *pTP = NULL);

	//��ѯ���������ݵ�Ԫ��
	void FormatPacket_ReadParam_DataUnit( CPtclPacket_NN *pPacket,
										  DataUint* pDataUint,int nCountDataUnit,
										  TPData *pTP = NULL);

	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ
			 ���ҷ�����װ�õı���
	vPnFn:��ѯ������Ϣ�㼯��
	*/

////��ȡ1��2��3������///////////////////////////////////////////////////////////////////////////
	void FormatPacket_ReadData_RealTime(	CPtclPacket_NN *pPacket,
		                                    FormatPnFnData *pPnFn,int nCountPnFn,
		                                    TPData *pTP = NULL);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ
			 ���ҷ�����װ�õı���
	vPnFn:��Ϣ�㼯��
	*/

	void FormatPacket_ReadData_History(	CPtclPacket_NN *pPacket,
										DataUint* pDataUint,int nCountDataUnit,
										TPData *pTP = NULL);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա ��վID���ն˵�ַ
			 ���ҷ�����װ�õı���
	vHistory:���ݵ�Ԫ����
	*/

	void FormatPacket_ReadData_Event(	CPtclPacket_NN *pPacket,
										int Fn,unsigned char startID,unsigned endID,
										TPData *pTP = NULL);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ
			 ���ҷ�����װ�õı���
	Fn:1:��Ҫ�¼� 2:��ͨ�¼�
	startID:�¼���ʼ��ʶ
	endID:�¼�������ʶ
	*/

////����ת��///////////////////////////////////////////////////////////////////////////
	void FormatPacket_Transmit(	CPtclPacket_NN *pPacket,
									unsigned char *pDelayBuf,int lenDelayBuf,
									unsigned char *pPSW,
									TPData *pTP = NULL);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ
			 ���ҷ�����װ�õı���
	vPnFn:��ѯ������Ϣ�㼯��
	*/

	void CRC16( unsigned char *data ,int data_len ,int start_id ,int crc_key , unsigned char *crc_code);
	void GetCRC(unsigned char *ADDR, int frametype, int teamno, unsigned char *buf, unsigned char hostID, int CRCKey , unsigned char *CRC);
	int  GetPW(CPtclPacket_NN *pPacket, unsigned char *UserDataBuf, unsigned char *pPSW, unsigned char *CRC, int isEx = 0);
private:
	static CPtclModule_NN *m_pInstance;
protected:
	unsigned char m_hostID;
	map <unsigned long,unsigned char> m_mapSeq;

	unsigned char getFrameSeq(unsigned int nAddr);
	
	

};
