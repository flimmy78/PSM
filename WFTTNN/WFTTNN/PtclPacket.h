/*
���ߣ����(QQ10568617 linan1003@163.com 2009/03)
������������ͨѶ��Լ���Ļ��࣬���ڹ�Լ��չ�����Ľṹ��ͬ�Ĺ�Լ��Ӧһ��CPtclPacket
		�����04��05���û���ӦCPtclPacket_Official �㽭�㶫���û���Լ��ӦCPtclPacket_ZJ
		�洢�������ݣ�packet_buffer����������data_buffer��
		Ϊ�����ṩͳһ�Ĵ���ӿ�
*/
#pragma once

#include "PtclCodec_Global.h"

static const int MAX_PACKET_BUF_SIZE = 2048;
static const int MAX_DATAAREA_BUF_SIZE = 2048;

class PTCLCODEC_EXPORT CPtclPacket  
{
public:
	CPtclPacket();
	virtual ~CPtclPacket();

	//�õ��������ݺͳ���
	unsigned char *getPacketBuf(unsigned char *packetBuf,int &lenPacketBuf);
	//���ñ������ݺͳ���
	void setPacketBuf(unsigned char *packetBuf,int lenPacketBuf);
	
	//�õ����������ݺͳ���
	unsigned char * getDataAreaBuf(unsigned char *dataBuf,int &LenDataBuf);
	//�������������ݺͳ���
	void setDataAreaBuf(unsigned char *dataBuf,int LenDataBuf);

	//�õ����ĳ���
	int getPacketBufLen();
	//�õ������򳤶�
	int getDataAreaBufLen();

	//�õ���������
	unsigned char *getPacketBuf();
	//�õ�����������
	unsigned char *getDataAreaBuf();

	//���ú�packet���������Ժ���װһ������
	virtual void formatPacket() = 0;

	//����һ������
	//���ĳ��Ȳ���:0 
	//���ĸ�ʽ����:<0
	//��ȡ���ĳɹ�:>0
	virtual int parsePacket() = 0;

	//�õ��������
	virtual int getFrameSeq() = 0;
	//�õ��ն��߼���ַ
	virtual unsigned int getDeviceAddr() = 0;
	//�õ���վID
	virtual unsigned char getHostID() = 0;

protected:

	unsigned char m_bufPacket[MAX_PACKET_BUF_SIZE];
	unsigned char m_bufDataArea[MAX_DATAAREA_BUF_SIZE];
	int m_packetBufLen;
	int m_dataAreaBufLen;
	
};

