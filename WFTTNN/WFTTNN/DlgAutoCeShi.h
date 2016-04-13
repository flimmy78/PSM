#if !defined(AFX_DLGAUTOCESHI_H__899DB0D3_130F_4471_8449_5CB4AC3A9697__INCLUDED_)
#define AFX_DLGAUTOCESHI_H__899DB0D3_130F_4471_8449_5CB4AC3A9697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoCeShi.h : header file
//
//#include "PtclModule_NN.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAutoCeShi dialog
//------------------------------
#pragma pack (1)
typedef	struct 
{
	unsigned char buf[256];//֡����
	short int ilen;//֡����
	char sterminalno[20];//�ն˱��
	char saddr[20];//�ն˵�ַ
	long pn;//�������
	char sparamid[10];//���ݱ�ʶ
	char soper[50];//��������
	char sparam[20];//����	
}TASKNODE,	*LPTASKNODE; 
typedef	struct 
{
	LPTASKNODE pTASKNODE;
	long maxcount;//�ڵ������
	long maxcurcount;//�������ڵ���
	long curpos;//��ǰִ��λ�� -1��ʾû��������ִ��
	short int flag;//ִ��״̬ 0-δִ�У�1-��ִ�У�2-ִ�����
	short int execout;//ִ�д���
	time_t tm;//��ʼʱ��
	unsigned char buf[1024];//�յ�����֡
	short int ilen;//֡����
	
}TASK,	*LPTASK;

#pragma pack ()
//------------------------------
typedef struct 
{
	long ParamID;
	CString strParmName;
}TASK_TYPE;
class CDlgAutoCeShi : public CDialog
{
// Construction
public:
	CDlgAutoCeShi(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAutoCeShi)
	enum { IDD = IDD_DIALOG_AUTOTEST };
	CListCtrl	m_ShowTerminalInfoListCtrl;
	CListCtrl	m_ShowTerminalInfoListCtrl1;
	CListCtrl	m_Result;

	CComboBox	m_cbDataType;

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	int m_nTerminalNum;

	void InitTerminalInfoList();
	void SetParam(CString strTerminalNo,CString strTerminalAddr);
	void SetTime(CString strTerminalNo,CString strTerminalAddr);
	void ReadHis(CString strTerminalNo,CString strTerminalAddr);
	void ReadReal(CString strTerminalNo,CString strTerminalAddr);
	CString GetTestId();
	CString g_testid;
	///BYTE sTime[5],eTime[5];

	char g_st[20],g_et[20];//��ʼʱ��
	HANDLE g_EventWaite;


	
	//int SendFrame(CPtclPacket_NN &packet_NN,unsigned char *outdata,int *outlen);
	int SendFrame(unsigned char *inbuf,int inlen);


	

	void AddResultOneLine(char *stime,char *terminaladdr,long pn,char *stype,char *paramid,char *sresult,char *svalue);
	void GetCurTimeFmt(char *sfmttm);

	void DoMessage();


	//------------------
	TASK m_TASK;//�������
	int InitTask(long maxcount);
	int ResetTask();
	LPTASKNODE  GetCurTask();
	LPTASKNODE  GetNextTask();
	void FreeTask();
	int  AddTaskNode(LPTASKNODE pTASKNODE);
	void ShowAndToDB(LPTASKNODE pTASKNODE,char *sresult,char *svalue);
	int bbusy;

	void InitialTask();

	void EexeTask(CString strTerminalNo,CString strTerminalAddr,long TaskID);//ִ������
	int VarianttoStr(_variant_t val,char *str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoCeShi)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnAutoCeShiPacketEventNotify(WPARAM wParam,LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoCeShi)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnStart();
	afx_msg void OnButton5();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnViewtask();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOCESHI_H__899DB0D3_130F_4471_8449_5CB4AC3A9697__INCLUDED_)
