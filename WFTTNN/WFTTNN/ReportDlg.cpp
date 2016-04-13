// ReportView.cpp : implementation file
//

#include "stdafx.h"
#include "ReportDlg.h"
#include "shape_mgr.h"
#include "WFTTNN.h"
#include "MainFrm.h"

// CReportDlg dialog

IMPLEMENT_DYNAMIC(CReportDlg, CDialog)

CReportDlg::CReportDlg(int termId, int batchId, CWnd* pParent /*=NULL*/)
	: CDialog(CReportDlg::IDD, pParent)
	, m_pWorkView(NULL)
	, m_layoutCtrl(this)
{
	m_termId = termId;
	m_batchId = batchId;
}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CReportDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CReportDlg message handlers


BOOL CReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CCreateContext cx;     
	cx.m_pNewViewClass = RUNTIME_CLASS( CWorkView ) ;     
	cx.m_pCurrentDoc = NULL;      
	CFrameWnd* pfWnd = (CFrameWnd*)this;      
	m_pWorkView = (CWorkView*)pfWnd->CreateView( &cx );  
	m_pWorkView->SetScrollSizes( MM_TEXT, CSize(600, 1300) );
	m_pWorkView->SetParent( this );  
	CRect rect;
	this->GetClientRect(&rect);
	m_layoutCtrl.SetOldRect(rect);
	rect.bottom -= 30;
	m_pWorkView->MoveWindow(&rect);
	
	TitleReport(&rect);
	ConfigurationReport(&rect);
	PowerCompatibilityReport(&rect);
	CommunicationReport(&rect);

	this->CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CReportDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_pWorkView == NULL)
		return;
	m_pWorkView->MoveWindow(0, 0, cx, cy-30);
	m_layoutCtrl.SetPos(IDC_BUTTON1, LT_NULL, RB_RIGHT_BOTTOM);
	m_layoutCtrl.SetOldRect(CRect(0, 0, cx, cy));
}

void CReportDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_pWorkView->Print();
}

void CReportDlg::TitleReport(CRect *rect)
{
	shp_text *pText = new shp_text(CRect(50,10,850,50),"��վ�������ɼ��ն˼���¼", 200, RGB(0,0,0), DT_CENTER | DT_SINGLELINE);
	m_pWorkView->m_shapeMgr.shapes_.push_back(pText);
	std::vector<int> vec_x, vec_y;
	vec_x.push_back(200);
	vec_x.push_back(300);
	vec_x.push_back(300);
	vec_y.push_back(29);
	vec_y.push_back(29);
	shp_table *pTable = new shp_table(vec_x, vec_y, 50 , 50);
	m_pWorkView->m_shapeMgr.shapes_.push_back(pTable);
	char strTermId[32] = {0}, strSendTime[32] = {0}, strTestTime[32] = {0}, 
		strTestType[32] = {0}, strSendCompany[32] = {0}, strCompany[32] = {0}, 
		strYear[32] = {0}, strSendTestTime[32] = {0}, strTermType[32] = {0};
	CTime now(time(NULL));
	CString csSendTime,csTestTime;
	csSendTime = now.Format("%Y%m%d");
	csTestTime = now.Format("%Y-%m-%d");

	CADODataBase *pConnect = CDBAccess::GetInstance()->GetDefaultConnect();
	if (pConnect == NULL)
		return ;

	char strSQL[300] = {0};
	int nRow = 0;
	sprintf(strSQL,"SELECT * FROM BATCH_INFO WHERE BatchNo=%d", m_batchId);

	CADORecordSet record;
	if (!pConnect->ExecSQL(strSQL,record))
		return ;

	while (!record.IsEndEOF())
	{
		_variant_t val;

		sprintf(strSendTime,"�ͼ����Σ�%d",m_batchId);
		sprintf(strTestTime,"�������ڣ�%s",csTestTime.GetBuffer(0));

		SYSTEMTIME sys;
		record.GetFieldValue("InspectDataTime",sys);
		csTestTime = CTime(sys).Format("%Y-%m-%d");
		sprintf(strSendTestTime,"�ͼ����ڣ�%s",csTestTime.GetBuffer(0));

		record.GetFieldValue("CheckType",val);
		CString testType = val.bstrVal;
		sprintf(strTestType,"������%s", testType);

		record.GetFieldValue("SupplyYear", val);
		CString testYear = val.bstrVal;
		sprintf(strYear,"������ȣ�%s", testYear);

		record.GetFieldValue("InspectUnit", val);
		CString testUint = val.bstrVal;
		sprintf(strSendCompany,"�ͼ쵥λ��%s", testUint);

		record.RecordMoveNext();
		nRow++;
	}

	nRow = 0;
	sprintf(strSQL,"SELECT * FROM RTU_ARCH WHERE RTU_ID=%d", m_termId);
	record.Close();
	if (!pConnect->ExecSQL(strSQL,record))
		return ;

	while (!record.IsEndEOF())
	{
		_variant_t val;
	
		record.GetFieldValue("ASSERTNO", val);
		CString assertNo = val.bstrVal;
		sprintf(strTermId,"������ţ�%s", assertNo);
		
		record.GetFieldValue("RTU_FACT", val);
		CString rtuFact = val.bstrVal;
		sprintf(strCompany,"�������ң�%s", rtuFact);

		int rtuType = 0;
		record.GetFieldValue("RTU_TYPE", rtuType);
		sprintf(strTermType,"�ն����ͣ�%d", rtuType);

		record.RecordMoveNext();
		nRow++;
	}

	pTable->set_text(0, 0, strSendTime);
	pTable->set_text(0, 1, strSendCompany);
	pTable->set_text(0, 2, strSendTestTime);
	pTable->set_text(1, 0, strTestType);
	pTable->set_text(1, 1, strTermId);
	pTable->set_text(1, 2, strCompany);
	m_pWorkView->m_shapeMgr.get_rect(*rect);
	MoveWindow(0, 0, rect->Width()+130, 600);

	vec_x.clear();
	vec_y.clear();
	vec_x.push_back(200);
	vec_x.push_back(600);
	vec_y.push_back(29);
	pTable = new shp_table(vec_x, vec_y, 50 , 108);
	m_pWorkView->m_shapeMgr.shapes_.push_back(pTable);

	nRow = 0;
	record.Close();
	sprintf(strSQL,"SELECT * FROM AUTOTEST_TOTAL_RESULT WHERE TERM_ID=%d AND BANTCH_ID=%d", m_termId,m_batchId);
	if (!pConnect->ExecSQL(strSQL,record))
		return ;

	CString strRflag;
	while (!record.IsEndEOF())
	{
		_variant_t val;
		record.GetFieldValue("RESULT", val);
		int flag = val.boolVal;
		
		if (flag == -1)
			strRflag = "�ϸ�";
		else
			strRflag = "���ϸ�";

		record.RecordMoveNext();
		nRow++;
	}

	pTable->set_text(0, 0, "�����ۣ�");
	pTable->set_text(0, 1, strRflag.GetBuffer(0));
	m_pWorkView->m_shapeMgr.get_rect(*rect);
	MoveWindow(0, 0, rect->Width()+130, 600);
}

void CReportDlg::ConfigurationReport(CRect *rect)
{
	CADODataBase *pConnect = CDBAccess::GetInstance()->GetDefaultConnect();
	if (pConnect == NULL)
		return ;

	char strSQL[300] = {0};
	int nRow = 0;
	sprintf(strSQL,"SELECT * FROM RTU_OTHER_TEST_SHAPE WHERE RTU_ID=%d", m_termId);

	CADORecordSet record;
	if (!pConnect->ExecSQL(strSQL,record))
		return ;

	int isq1,isq2,isq3,isq4,isq5,isq6,isq7 = 0;
	CString desc1,desc2,desc3,desc4,desc5,desc6,desc7 = _T("");

	while (!record.IsEndEOF())
	{
		_variant_t val;

		record.GetFieldValue("ISQ_SSC",val);
		isq1 = val.boolVal;
		record.GetFieldValue("ISQ_LAMP",val);
		isq2 = val.boolVal;
		record.GetFieldValue("ISQ_BARCODE",val);
		isq3 = val.boolVal;
		record.GetFieldValue("ISQ_LID",val);
		isq4 = val.boolVal;
		record.GetFieldValue("ISQ_PEDESTAL",val);
		isq5 = val.boolVal;
		record.GetFieldValue("ISQ_NAMEPLATE",val);
		isq6 = val.boolVal;
		record.GetFieldValue("ISQ_SCREW",val);
		isq7 = val.boolVal;

		record.GetFieldValue("DESC_SSC",val);
		desc1 = val.bstrVal;
		record.GetFieldValue("DESC_LAMP",val);
		desc2 = val.bstrVal;
		record.GetFieldValue("DESC_BARCODE",val);
		desc3 = val.bstrVal;
		record.GetFieldValue("DESC_LID",val);
		desc4 = val.bstrVal;
		record.GetFieldValue("DESC_PEDESTAL",val);
		desc5 = val.bstrVal;
		record.GetFieldValue("DESC_NAMEPLATE",val);
		desc6 = val.bstrVal;
		record.GetFieldValue("DESC_SCREW",val);
		desc7 = val.bstrVal;

		record.RecordMoveNext();
		nRow++;
	}

	std::vector<int> vec_x, vec_y;
	shp_text *pText = new shp_text(CRect(50,150,400,180),"���νṹ���", 120, RGB(0,0,0), DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	m_pWorkView->m_shapeMgr.shapes_.push_back(pText);
	vec_x.push_back(300);
	vec_x.push_back(100);
	vec_x.push_back(400);
	for (int i = 0; i < 8; i++)
	{
		vec_y.push_back(29);
	}	
	shp_table *pTable = new shp_table(vec_x, vec_y, 50 , 180);
	m_pWorkView->m_shapeMgr.shapes_.push_back(pTable);

	if (isq1 == -1)
		pTable->set_text(1,1,"�ϸ�");
	else 
		pTable->set_text(1,1,"���ϸ�");
	if (isq2 == -1)
		pTable->set_text(2,1,"�ϸ�");
	else 
		pTable->set_text(2,1,"���ϸ�");
	if (isq3 == -1)
		pTable->set_text(3,1,"�ϸ�");
	else 
		pTable->set_text(3,1,"���ϸ�");
	if (isq4 == -1)
		pTable->set_text(4,1,"�ϸ�");
	else 
		pTable->set_text(4,1,"���ϸ�");
	if (isq5 == -1)
		pTable->set_text(5,1,"�ϸ�");
	else 
		pTable->set_text(5,1,"���ϸ�");
	if (isq6 == -1)
		pTable->set_text(6,1,"�ϸ�");
	else 
		pTable->set_text(6,1,"���ϸ�");
	if (isq7 == -1)
		pTable->set_text(7,1,"�ϸ�");
	else 
		pTable->set_text(7,1,"���ϸ�");

	pTable->set_text(1, 2, desc1.GetBuffer(0));
	pTable->set_text(2, 2, desc2.GetBuffer(0));
	pTable->set_text(3, 2, desc3.GetBuffer(0));
	pTable->set_text(4, 2, desc4.GetBuffer(0));
	pTable->set_text(5, 2, desc5.GetBuffer(0));
	pTable->set_text(6, 2, desc6.GetBuffer(0));
	pTable->set_text(7, 2, desc7.GetBuffer(0));

	pTable->set_text(0, 0, "�����Ŀ", 0, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pTable->set_text(0, 1, "�Ƿ�ϸ�", 0, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pTable->set_text(0, 2, "���ϸ�ԭ��", 0, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pTable->set_text(1, 0, "���νṹ����װ�ߴ缰��ɫ");
	pTable->set_text(2, 0, "ָʾ��");
	pTable->set_text(3, 0, "������");
	pTable->set_text(4, 0, "���Ӹ�");
	pTable->set_text(5, 0, "������");
	pTable->set_text(6, 0, "����");
	pTable->set_text(7, 0, "��˿");
	m_pWorkView->m_shapeMgr.get_rect(*rect);
	MoveWindow(0, 0, rect->Width()+130, 600);
}

void CReportDlg::PowerCompatibilityReport(CRect *rect)
{
	CADODataBase *pConnect = CDBAccess::GetInstance()->GetDefaultConnect();
	if (pConnect == NULL)
		return ;

	char strSQL[300] = {0};
	int nRow = 0;
	sprintf(strSQL,"SELECT * FROM RTU_OTHER_TEST_EMC WHERE RTU_ID=%d", m_termId);//��վ�ɼ����Է�������

	CADORecordSet record;
	if (!pConnect->ExecSQL(strSQL,record))
		return ;

	int isq1,isq2,isq3,isq4,isq5,isq7,isq8,isq9,isq10,isq11,isq12,isq13,isq14,isq15,isq16 = 0;
	CString desc1,desc2,desc3,desc4,desc5,desc7,desc8,desc9,desc10,desc11,desc12,desc13,desc14,desc15,desc16 = _T("");
	while (!record.IsEndEOF())
	{
		_variant_t val;

		record.GetFieldValue("ISQ1",val);
		isq1 = val.boolVal;
		record.GetFieldValue("ISQ2",val);
		isq2 = val.boolVal;
		record.GetFieldValue("ISQ3",val);
		isq3 = val.boolVal;
		record.GetFieldValue("ISQ4",val);
		isq4 = val.boolVal;
		record.GetFieldValue("ISQ5",val);
		isq5 = val.boolVal;
		record.GetFieldValue("ISQ7",val);
		isq7 = val.boolVal;
		record.GetFieldValue("ISQ8",val);
		isq8 = val.boolVal;
		record.GetFieldValue("ISQ9",val);
		isq9 = val.boolVal;
		record.GetFieldValue("ISQ10",val);
		isq10 = val.boolVal;
		record.GetFieldValue("ISQ11",val);
		isq11 = val.boolVal;
		record.GetFieldValue("ISQ12",val);
		isq12 = val.boolVal;
		record.GetFieldValue("ISQ13",val);
		isq13 = val.boolVal;
		record.GetFieldValue("ISQ14",val);
		isq14 = val.boolVal;
		record.GetFieldValue("ISQ15",val);
		isq15 = val.boolVal;
		record.GetFieldValue("ISQ16",val);
		isq16 = val.boolVal;


		record.GetFieldValue("DESC1",val);
		desc1 = val.bstrVal;
		record.GetFieldValue("DESC2",val);
		desc2 = val.bstrVal;
		record.GetFieldValue("DESC3",val);
		desc3 = val.bstrVal;
		record.GetFieldValue("DESC4",val);
		desc4 = val.bstrVal;
		record.GetFieldValue("DESC5",val);
		desc5 = val.bstrVal;
		record.GetFieldValue("DESC7",val);
		desc7 = val.bstrVal;
		record.GetFieldValue("DESC8",val);
		desc8 = val.bstrVal;
		record.GetFieldValue("DESC9",val);
		desc9 = val.bstrVal;
		record.GetFieldValue("DESC10",val);
		desc10 = val.bstrVal;
		record.GetFieldValue("DESC11",val);
		desc11 = val.bstrVal;
		record.GetFieldValue("DESC12",val);
		desc12 = val.bstrVal;
		record.GetFieldValue("DESC13",val);
		desc13 = val.bstrVal;
		record.GetFieldValue("DESC14",val);
		desc14 = val.bstrVal;
		record.GetFieldValue("DESC15",val);
		desc15 = val.bstrVal;
		record.GetFieldValue("DESC16",val);
		desc16 = val.bstrVal;

		record.RecordMoveNext();
		nRow++;
	}
	std::vector<int> vec_x, vec_y;
	shp_text *pText = new shp_text(CRect(50,420,450,450),"��ż����Լ��", 120, RGB(0,0,0), DT_LEFT | DT_SINGLELINE|DT_VCENTER); //MODIFY BY WM 20140102 ���-->���
	m_pWorkView->m_shapeMgr.shapes_.push_back(pText);
	vec_x.clear();
	vec_y.clear();
	vec_x.push_back(300);
	vec_x.push_back(100);
	vec_x.push_back(400);
	for (int i = 0; i < 16; i++)
	{
		vec_y.push_back(29);
	}	
	shp_table *pTable = NULL;
	pTable = new shp_table(vec_x, vec_y, 50 , 450);
	m_pWorkView->m_shapeMgr.shapes_.push_back(pTable);
	if (isq1 == -1)
		pTable->set_text(1,1,"�ϸ�");
	else 
		pTable->set_text(1,1,"���ϸ�");
	if (isq2 == -1)
		pTable->set_text(2,1,"�ϸ�");
	else 
		pTable->set_text(2,1,"���ϸ�");
	if (isq3 == -1)
		pTable->set_text(3,1,"�ϸ�");
	else 
		pTable->set_text(3,1,"���ϸ�");
	if (isq4 == -1)
		pTable->set_text(4,1,"�ϸ�");
	else 
		pTable->set_text(4,1,"���ϸ�");
	if (isq5 == -1)
		pTable->set_text(5,1,"�ϸ�");
	else 
		pTable->set_text(5,1,"���ϸ�");
	if (isq7 == -1)
		pTable->set_text(6,1,"�ϸ�");
	else 
		pTable->set_text(6,1,"���ϸ�");
	if (isq8 == -1)
		pTable->set_text(7,1,"�ϸ�");
	else 
		pTable->set_text(7,1,"���ϸ�");
	if (isq9 == -1)
		pTable->set_text(8,1,"�ϸ�");
	else 
		pTable->set_text(8,1,"���ϸ�");
	if (isq10 == -1)
		pTable->set_text(9,1,"�ϸ�");
	else 
		pTable->set_text(9,1,"���ϸ�");
	if (isq11 == -1)
		pTable->set_text(10,1,"�ϸ�");
	else 
		pTable->set_text(10,1,"���ϸ�");
	if (isq12 == -1)
		pTable->set_text(11,1,"�ϸ�");
	else 
		pTable->set_text(11,1,"���ϸ�");
	if (isq13 == -1)
		pTable->set_text(12,1,"�ϸ�");
	else 
		pTable->set_text(12,1,"���ϸ�");
	if (isq14 == -1)
		pTable->set_text(13,1,"�ϸ�");
	else 
		pTable->set_text(13,1,"���ϸ�");
	if (isq15 == -1)
		pTable->set_text(14,1,"�ϸ�");
	else 
		pTable->set_text(14,1,"���ϸ�");
	if (isq16 == -1)
		pTable->set_text(15,1,"�ϸ�");
	else 
		pTable->set_text(15,1,"���ϸ�");

	pTable->set_text(0, 0, "�����Ŀ", 0, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pTable->set_text(0, 1, "�Ƿ�ϸ�", 0, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pTable->set_text(0, 2, "���ϸ�ԭ��", 0, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pTable->set_text(1, 0, "��Ե����");
	pTable->set_text(2, 0, "��Եǿ��");
	pTable->set_text(3, 0, "�����ѹ");
	pTable->set_text(4, 0, "��Դ��ѹ�仯����");
	pTable->set_text(5, 0, "������������");
	pTable->set_text(6, 0, "���ݺ�ʱ�ӱ�������");
	pTable->set_text(7, 0, "��ѹ�ݽ��Ͷ�ʱ�ж�����");
	pTable->set_text(8, 0, "��Ƶ�ų����Ŷ�����");
	pTable->set_text(9, 0, "��Ƶ��ų����俹�Ŷ�����");
	pTable->set_text(10, 0, "��Ƶ����Ӧ�Ĵ���ɧ�ſ��Ŷ�����");
	pTable->set_text(11, 0, "����ŵ翹�Ŷ�����");
	pTable->set_text(12, 0, "�����˲������Ⱥ���Ŷ�����");
	pTable->set_text(13, 0, "�����񵴲����Ŷ�����");
	pTable->set_text(14, 0, "��ӿ���Ŷ�����");
	pTable->set_text(15, 0, "R-485������380V�������");

	pTable->set_text(1, 2, desc1.GetBuffer(0));
	pTable->set_text(2, 2, desc2.GetBuffer(0));
	pTable->set_text(3, 2, desc3.GetBuffer(0));
	pTable->set_text(4, 2, desc4.GetBuffer(0));
	pTable->set_text(5, 2, desc5.GetBuffer(0));
	pTable->set_text(6, 2, desc7.GetBuffer(0));
	pTable->set_text(7, 2, desc8.GetBuffer(0));
	pTable->set_text(8, 2, desc9.GetBuffer(0));
	pTable->set_text(9, 2, desc10.GetBuffer(0));
	pTable->set_text(10, 2, desc11.GetBuffer(0));
	pTable->set_text(11, 2, desc12.GetBuffer(0));
	pTable->set_text(12, 2, desc13.GetBuffer(0));
	pTable->set_text(13, 2, desc14.GetBuffer(0));
	pTable->set_text(14, 2, desc15.GetBuffer(0));
	pTable->set_text(15, 2, desc16.GetBuffer(0));

	m_pWorkView->m_shapeMgr.get_rect(*rect);
	MoveWindow(0, 0, rect->Width()+130, 600);

}

void CReportDlg::CommunicationReport(CRect *rect)
{
	int isq1, isq2, isq3, isq4, isq5, isq6, isq7, isq8, isq9;
	CString desc1, desc2, desc3, desc4, desc5, desc6, desc7, desc8, desc9;
	isq1 = isq2 = isq3 = isq4 = isq5 = isq6 = isq7 = isq8 = isq9 = 0;
	desc1 = desc2 = desc3 = desc4 = desc5 = desc6 = desc7 = desc8 = desc9 = "";
	//
	CADODataBase *pConnect = CDBAccess::GetInstance()->GetDefaultConnect();
	CADORecordSet record;
	char strSQL[300] = {0};	
	sprintf(strSQL,"SELECT * FROM RTU_OTHER_TEST_FUNCTION WHERE RTU_ID=%d", m_termId);
	if (pConnect != NULL && pConnect->ExecSQL(strSQL,record) && !record.IsEndEOF())
	{
		_variant_t val;	
	
		record.GetFieldValue("ISQ_SJZC", val);
		isq1 = !val.boolVal;
		record.GetFieldValue("DESC_SJZC", val);
		desc1 = val.bstrVal;
	
		record.GetFieldValue("ISQ_CSSZ", val);
		isq2 = !val.boolVal;
		record.GetFieldValue("DESC_CSSZ", val);
		desc2 = val.bstrVal;

		record.GetFieldValue("ISQ_QXSJ", val);
		isq3 = !val.boolVal;
		record.GetFieldValue("DESC_QXSJ", val);
		desc3 = val.bstrVal;

		record.GetFieldValue("ISQ_RSJ", val);
		isq4 = !val.boolVal;
		record.GetFieldValue("DESC_RSJ", val);
		desc4 = val.bstrVal;

		record.GetFieldValue("ISQ_YSJ", val);
		isq5 = !val.boolVal;
		record.GetFieldValue("DESC_YSJ", val);
		desc5 = val.bstrVal;

		record.GetFieldValue("ISQ_SJJL", val);
		isq6 = !val.boolVal;
		record.GetFieldValue("DESC_SJJL", val);
		desc6 = val.bstrVal;

		record.GetFieldValue("ISQ_GJJL", val);
		isq7 = !val.boolVal;
		record.GetFieldValue("DESC_GJJL", val);
		desc7 = val.bstrVal;

		record.GetFieldValue("ISQ_ZDDS", val);
		isq8 = !val.boolVal;
		record.GetFieldValue("DESC_ZDDS", val);
		desc8 = val.bstrVal;

		record.GetFieldValue("ISQ_SJTC", val);
		isq9 = !val.boolVal;
		record.GetFieldValue("DESC_SJTC", val);
		desc9 = val.bstrVal;
	}

	std::vector<int> vec_x, vec_y;
	shp_text *pText = new shp_text(CRect(50,920,450,950),"���ܲ���", 120, RGB(0,0,0), DT_LEFT | DT_SINGLELINE|DT_VCENTER);
	m_pWorkView->m_shapeMgr.shapes_.push_back(pText);
	vec_x.push_back(300);
	vec_x.push_back(100);
	vec_x.push_back(400);
	for (int i = 0; i < 10; i++)
	{
		vec_y.push_back(29);
	}
	shp_table *pTable = new shp_table(vec_x, vec_y, 50 , 950);
	m_pWorkView->m_shapeMgr.shapes_.push_back(pTable);

	pTable->set_text(0, 0, "�����Ŀ", 0, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pTable->set_text(1, 0, "����ٲ⹦��");
	pTable->set_text(2, 0, "�������ù���");
	pTable->set_text(3, 0, "�������ݹ���");
	pTable->set_text(4, 0, "�����ݹ���");
	pTable->set_text(5, 0, "�����ݹ���");
	pTable->set_text(6, 0, "�¼���¼����");
	pTable->set_text(7, 0, "�澯��¼����");
	pTable->set_text(8, 0, "�ն˶�ʱ����");
	pTable->set_text(9, 0, "����͸������");

	pTable->set_text(0, 1, "�Ƿ�ϸ�", 0, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pTable->set_text(1, 1, isq1 > 0 ? "���ϸ�" : "�ϸ�");
	pTable->set_text(2, 1, isq2 > 0 ? "���ϸ�" : "�ϸ�");
	pTable->set_text(3, 1, isq3 > 0 ? "���ϸ�" : "�ϸ�");
	pTable->set_text(4, 1, isq4 > 0 ? "���ϸ�" : "�ϸ�");
	pTable->set_text(5, 1, isq5 > 0 ? "���ϸ�" : "�ϸ�");
	pTable->set_text(6, 1, isq6 > 0 ? "���ϸ�" : "�ϸ�");
	pTable->set_text(7, 1, isq7 > 0 ? "���ϸ�" : "�ϸ�");
	pTable->set_text(8, 1, isq8 > 0 ? "���ϸ�" : "�ϸ�");
	pTable->set_text(9, 1, isq9 > 0 ? "���ϸ�" : "�ϸ�");

	pTable->set_text(0, 2, "���ϸ�ԭ��", 0, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	pTable->set_text(1, 2, desc1.GetBuffer(0));
	pTable->set_text(2, 2, desc2.GetBuffer(0));
	pTable->set_text(3, 2, desc3.GetBuffer(0));
	pTable->set_text(4, 2, desc4.GetBuffer(0));
	pTable->set_text(5, 2, desc5.GetBuffer(0));
	pTable->set_text(6, 2, desc6.GetBuffer(0));
	pTable->set_text(7, 2, desc7.GetBuffer(0));
	pTable->set_text(8, 2, desc8.GetBuffer(0));
	pTable->set_text(9, 2, desc9.GetBuffer(0));	
	
	m_pWorkView->m_shapeMgr.get_rect(*rect);
	MoveWindow(0, 0, rect->Width()+130, 600);
}