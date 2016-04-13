// ModemCom.h: interface for the CModem class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_MODEMCOM_H__5B029E30_BFC7_4C82_B0E0_3667B3612C2F__INCLUDED_)
#define AFX_MODEMCOM_H__5B029E30_BFC7_4C82_B0E0_3667B3612C2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#pragma pack(1) 
#include "..\Communication\Com.h"

#define COM_EVT_RECV     10
#define COM_EVT_SEND     11
#define COM_EVT_RLSD     12
#define COM_EVT_RING     13

#define CONNECT_STEP_INIT_MODEM             10       //��ʼ��MODEM
#define CONNECT_STEP_DIALING                11       //���ڲ���
#define CONNECT_STEP_DIAL_SUCCESS           12       //���ųɹ�
#define CONNECT_STEP_DIAL_FAIL              13       //����ʧ��

#define CONNECT_STEP_TURNING_ORDER          14       //����ת�����ʽ
#define CONNECT_STEP_HUNGINGUP              15       //���ڹҶ�
#define CONNECT_STEP_HUNGEDUP               16       //�Ҷϳɹ�

#define DIAL_METHOD_TONE                    0		 //��Ƶ����
#define DIAL_METHOD_PLUSE                   1		 //���岦��

#define  CONN_RESULT_RING                 2     //����  
#define  CONN_RESULT_NODIALONE            6     //û�в�����
#define  CONN_RESULT_BUSY                 7     //��·æ
#define  CONN_RESULT_NOANSWER             8     //�޻�Ӧ
#define  CONN_RESULT_NOCARRIER            3     //�ز���ʧ����δ����ز�
#define  CONN_RESULT_ERROR                4     //����
#define  CONN_RESULT_DELAYED              24      //������
#define  CONN_RESULT_BLACKLISTED          32      //���ܲ�
#define  CONN_RESULT_LINE_USER            83      //��·��ʹ��

class CModem : public CCom   
{
public:
	virtual void Close();
	void InitModemInfo(char * sComName,WORD baudRate,BYTE stopBits,BYTE parity,BYTE byteSize,BYTE dialMode,char *initStr,char *szCall);
	virtual int  OnConnecting(DWORD connstate,CString sResult,BYTE nResult);
	virtual int  OnInitResult(DWORD result, CString sResult);
	int m_connect_step;
	int ProcessResult(DWORD connstate, CString sResult, BYTE nResult);
	virtual void ProcessComEventInThread(DWORD dwEvtMask);
	void OnResponse();
	void ProcessData();
	virtual BOOL Connect();
	BOOL Dial();
	BOOL InitModem();
	BYTE m_dialMode;

 


	CString m_initStr;
	CString m_sCallTel;

	BYTE IsConnect;
	CModem();
	virtual ~CModem();
	int CloseModem();
protected:
	virtual void ProcessMsgComEvent(DWORD eventid);
	virtual void GetCommMask(DWORD & mask);
	CString m_replayInfo;
	

private:
	void SeekConnect();
	int JudgeResult();
	
};

#endif // !defined(AFX_MODEMCOM_H__5B029E30_BFC7_4C82_B0E0_3667B3612C2F__INCLUDED_)
