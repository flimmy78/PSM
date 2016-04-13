// DlgReadTermInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WFTTNN.h"
#include "DlgReadTermInfo.h"
#include ".\dlgreadterminfo.h"
#include "ItemData.h"
#include "ptcl_codec/ptcl_module_csg.h"
#include "ptcl_codec/data_parse_csg.h"
#include "MainFrm.h"
#include "LogMgr.h"


// CDlgReadTermInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgReadTermInfo, CDialog)
CDlgReadTermInfo::CDlgReadTermInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReadTermInfo::IDD, pParent)
	, m_strTerminalAddr("")
	, m_strHardVersion(_T(""))
	, m_strSoftVersion(_T(""))
	, m_bHardVer(FALSE)
	, m_bSoftver(FALSE)
	, m_bTime(FALSE)
	, m_bUseSystemTime(TRUE)
	, m_bMeterReadTime(TRUE) //ADD BY WM 20131112
	, m_bDataReset(TRUE)  //ADD BY WM 20131115
	, m_bTerminalReset(FALSE)  //ADD BY WM 20131127
	, m_bAlarmNoShield(TRUE)  //ADD BY WM 20131206  MODIFY BY WM 20140102 (FALSE-->TRUE)
	, m_bEventNoShield(TRUE)  //ADD BY WM 20131206  MODIFY BY WM 20140102 (FALSE-->TRUE)
	, m_bZDSBDisable(TRUE)
	, m_bZDCBCS(TRUE)
{
	for (int i = 0; i < 8; ++i)
		m_bRS485[i] = FALSE;
}

CDlgReadTermInfo::~CDlgReadTermInfo()
{
}

void CDlgReadTermInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_ctrlDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_ctrlTime);
	DDX_Text(pDX, IDC_TXT_VERSION1, m_strHardVersion);
	DDX_Text(pDX, IDC_TXT_VERSION2, m_strSoftVersion);
	DDX_Check(pDX, IDC_CHECK_HARDVER, m_bHardVer);
	DDX_Check(pDX, IDC_CHECK_SOFTVER, m_bSoftver);
	DDX_Check(pDX, IDC_CHECK_TIME, m_bTime);
	DDX_Check(pDX, IDC_CHECK_SYSTEMTIME, m_bUseSystemTime);
	DDX_Check(pDX, IDC_CHECK_METERREADTIME, m_bMeterReadTime);//ADD BY WM 20131112
	DDX_Check(pDX, IDC_CHECK_DATARESET, m_bDataReset);//ADD BY WM 20131115
	DDX_Check(pDX, IDC_CHECK_TERMINALRESET, m_bTerminalReset);//ADD BY WM 20131127
	DDX_Check(pDX, IDC_CHECK_ALARMNOSHIELD, m_bAlarmNoShield);//ADD BY WM 20131206
	DDX_Check(pDX, IDC_CHECK_EVENTNOSHIELD, m_bEventNoShield);//ADD BY WM 20131206
	DDX_Check(pDX, IDC_CHECK_ZDSB_DISABLE, m_bZDSBDisable);
	DDX_Check(pDX, IDC_CHECK_ZDCBCS, m_bZDCBCS);
	DDX_Check(pDX, IDC_CHECK_RS485_1, m_bRS485[0]);
	DDX_Check(pDX, IDC_CHECK_RS485_2, m_bRS485[1]);
	DDX_Check(pDX, IDC_CHECK_RS485_3, m_bRS485[2]);
	DDX_Check(pDX, IDC_CHECK_RS485_4, m_bRS485[3]);
	DDX_Check(pDX, IDC_CHECK_RS485_5, m_bRS485[4]);
	DDX_Check(pDX, IDC_CHECK_RS485_6, m_bRS485[5]);
	DDX_Check(pDX, IDC_CHECK_RS485_7, m_bRS485[6]);
	DDX_Check(pDX, IDC_CHECK_RS485_8, m_bRS485[7]);
}


BEGIN_MESSAGE_MAP(CDlgReadTermInfo, CDialog)
	ON_BN_CLICKED(IDC_BTN_SET, &CDlgReadTermInfo::OnBnClickedBtnSet)
	ON_BN_CLICKED(IDC_BTN_CALL, &CDlgReadTermInfo::OnBnClickedBtnCall)
	ON_BN_CLICKED(IDC_CHECK_TIME, &CDlgReadTermInfo::OnBnClickedCheckTime)
	ON_BN_CLICKED(IDC_CHECK_SYSTEMTIME, &CDlgReadTermInfo::OnBnClickedCheckSystemtime)
END_MESSAGE_MAP()


// CDlgReadTermInfo ��Ϣ�������

void CDlgReadTermInfo::OnEvent(unsigned int nEventID, void *pParam)
{
	if (Event_Type_Item_Double_Click != nEventID || !::IsWindowVisible(this->GetSafeHwnd()))
		return;	
	
	if (ITEM_TYPE_TERMINAL == ((ItemData *)pParam)->m_type)
	{
		TerminalItemData *data = (TerminalItemData *)pParam;
		m_strTerminalAddr = data->m_addr;

		CString strTermNo = "";
		strTermNo.Format("�ն���Ϣ - %d", data->m_id);
		GetDlgItem(IDC_LABEL_TIPS)->SetWindowTextA(strTermNo);
		m_strHardVersion = "";
		m_strSoftVersion = "";
	}
}

void CDlgReadTermInfo::OnBnClickedBtnCall()
{
	CChannel *pChannel = NULL;
	CMainFrame *pMainFrm  = (CMainFrame *)AfxGetMainWnd();
	if (pMainFrm)
		pChannel = pMainFrm->m_pDlgConn->m_pChannel;	
	if (!pChannel)
	{
		MessageBox("δ�����նˣ����Ƚ������ӣ�", "��ʾ", MB_OK|MB_ICONINFORMATION);
		return;
	}

	if (m_strTerminalAddr == "")
	{
		MessageBox("δѡ���նˣ�˫��������ѡ���նˣ�", "��ʾ", MB_OK|MB_ICONINFORMATION);
		return;
	}

	UpdateData(TRUE);
		
	vector<string> vParam;
	if (m_bTime) vParam.push_back("E0000130");
	if (m_bSoftver) vParam.push_back("E0000B01");
	if (m_bHardVer)	vParam.push_back("E0000B02");
	
	if (m_bZDCBCS) 
	{
		vParam.push_back("E0000230");	//��һ·485
		vParam.push_back("E0000231");	//�ڶ�·485
		vParam.push_back("E0000232");	//����·485
		vParam.push_back("E0000233");	//����·485
		vParam.push_back("E0000234");	//����·485
		vParam.push_back("E0000235");	//����·485
		vParam.push_back("E0000236");	//����·485
		vParam.push_back("E0000237");	//�ڰ�·485
	}

	vector<ptcl_module_csg::pnfn_data> vPnFn;
	for (int j = 0; j < vParam.size(); j++)
	{
		ptcl_module_csg::pnfn_data pnfn;
		pnfn.pn = 0;
		pnfn.fn = ptcl_packet_csg::convert_fn(vParam.at(j));;
		vPnFn.push_back(pnfn);
	}

	if (vPnFn.empty())
	{
		MessageBox("��ѡ������", "��ʾ", MB_OK|MB_ICONINFORMATION);
		return;
	}
	
	ptcl_packet_csg packet;
	UINT64 id = ptcl_packet_csg::convert_device_addr(m_strTerminalAddr.GetBuffer(0));
	packet.set_device_addr(id);
	ptcl_module_csg::get_instance()->format_packet_read_parameter(&packet, &vPnFn[0], vPnFn.size());
	pChannel->Write((char *)packet.get_packet_buffer(), packet.get_packet_length(), id);

	CMsgOutput::GetInstance()->ShowpPacketInfo("", 0, TraceBuf(packet.get_packet_buffer(), packet.get_packet_length(), "�·�"));
	CLogMgr::GetInstanse().LogPacket("�·�", packet.get_packet_buffer(), packet.get_packet_length());
}

void CDlgReadTermInfo::OnBnClickedBtnSet()
{
	CChannel *pChannel = NULL;
	CMainFrame *pMainFrm  = (CMainFrame *)AfxGetMainWnd();
	if (pMainFrm)
		pChannel = pMainFrm->m_pDlgConn->m_pChannel;	
	if (!pChannel)
	{
		MessageBox("δ�����նˣ����Ƚ������ӣ�", "��ʾ", MB_OK|MB_ICONINFORMATION);
		return;
	}

	if (m_strTerminalAddr == "")
	{
		MessageBox("δѡ���նˣ�˫��������ѡ���նˣ�", "��ʾ", MB_OK|MB_ICONINFORMATION);
		return;
	}
	
	UpdateData(TRUE);
	//if (!m_bTime && !m_bUseSystemTime && !m_bMeterReadTime && !m_bDataReset && !m_bTerminalReset && !m_bAlarmNoShield && !m_bEventNoShield) //MODIFY BY WM 20131206
	//{
	//	MessageBox("��ѡ�������", "��ʾ", MB_OK|MB_ICONINFORMATION); //MODIFY BY WM 20131112
	//	return;
	//}
		
	vector<ptcl_module_csg::pnfn_data_uint> vDataUnit;
	if (m_bUseSystemTime)
	{
		ptcl_module_csg::pnfn_data_uint dataunit;
		dataunit.pnfn.pn = 0;
		dataunit.pnfn.fn = ptcl_packet_csg::convert_fn("E0000130");
		dataunit.len_data = 6;
		CTime dt(time(NULL));
		dataunit.data_buffer[5] = ptcl_module::bin_to_bcd(dt.GetYear() - 2000);
		dataunit.data_buffer[4] = ptcl_module::bin_to_bcd(dt.GetMonth());
		dataunit.data_buffer[3] = ptcl_module::bin_to_bcd(dt.GetDay());
		dataunit.data_buffer[2] = ptcl_module::bin_to_bcd(dt.GetHour());
		dataunit.data_buffer[1] = ptcl_module::bin_to_bcd(dt.GetMinute());
		dataunit.data_buffer[0] = ptcl_module::bin_to_bcd(dt.GetSecond());
		vDataUnit.push_back(dataunit);
	}
	if (m_bTime)
	{
		ptcl_module_csg::pnfn_data_uint dataunit;
		dataunit.pnfn.pn = 0;
		dataunit.pnfn.fn = ptcl_packet_csg::convert_fn("E0000130");
		dataunit.len_data = 6;
		SYSTEMTIME tDate, tTime;
		m_ctrlDate.GetTime(&tDate);
		m_ctrlTime.GetTime(&tTime);
		dataunit.data_buffer[5] = ptcl_module::bin_to_bcd(tDate.wYear - 2000);
		dataunit.data_buffer[4] = ptcl_module::bin_to_bcd(tDate.wMonth);
		dataunit.data_buffer[3] = ptcl_module::bin_to_bcd(tDate.wDay);
		dataunit.data_buffer[2] = ptcl_module::bin_to_bcd(tTime.wHour);
		dataunit.data_buffer[1] = ptcl_module::bin_to_bcd(tTime.wMinute);
		dataunit.data_buffer[0] = ptcl_module::bin_to_bcd(tTime.wSecond);
		vDataUnit.push_back(dataunit);
	}
	if(m_bMeterReadTime) //MODYFY BY WM 20131112
	{
		UpdateData(TRUE);
		vector<string>vParamID;
		vParamID.push_back("E0000200");	//��һ·485			7bytes
		vParamID.push_back("E0000201");	//�ڶ�·485			7
		vParamID.push_back("E0000202");	//����·485		    7
		vParamID.push_back("E0000203");	//����·485			7
		vParamID.push_back("E0000204");	//����·485		    7
		vParamID.push_back("E0000205");	//����·485			7
		vParamID.push_back("E0000206");	//����·485			7
		vParamID.push_back("E0000207");	//�ڰ�·485			7

		/* ��׼ʱ���Ϊ2000-1-1 00:00
		CTime now(time(NULL));
		CTime temp(now.GetYear(),now.GetMonth(),now.GetDay(),0,0,0);
		for (int i = 0; i < vParamID.size(); i++)
		{
			ptcl_module_csg::pnfn_data_uint dataunit;
			dataunit.pnfn.pn = 0;
			dataunit.pnfn.fn = ptcl_packet_csg::convert_fn(vParamID.at(i));
			dataunit.len_data = 7;
			dataunit.data_buffer[4] = ptcl_module::bin_to_bcd(temp.GetYear() - 2000);
			dataunit.data_buffer[3] = ptcl_module::bin_to_bcd(temp.GetMonth());
			dataunit.data_buffer[2] = ptcl_module::bin_to_bcd(temp.GetDay());
			dataunit.data_buffer[1] = ptcl_module::bin_to_bcd(temp.GetHour());
			dataunit.data_buffer[0] = ptcl_module::bin_to_bcd(temp.GetMinute());
			dataunit.data_buffer[5] = 0;
			dataunit.data_buffer[6] = 3;
			vDataUnit.push_back(dataunit);
		}
		*/
		unsigned char cbcspz[] = { 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x03 };
		for (int i = 0; i < vParamID.size(); i++)
		{
			ptcl_module_csg::pnfn_data_uint dataunit;
			dataunit.pnfn.pn = 0;
			dataunit.pnfn.fn = ptcl_packet_csg::convert_fn(vParamID.at(i));
			dataunit.len_data = sizeof(cbcspz);
			memcpy(dataunit.data_buffer, cbcspz, sizeof(cbcspz));
			vDataUnit.push_back(dataunit);
		}
	}
	if(m_bDataReset == true) //MODIFY BY WM 20131115 
	{
		ptcl_module_csg::pnfn_data_uint dataunit;
		dataunit.pnfn.pn = 0;
		dataunit.pnfn.fn = ptcl_packet_csg::convert_fn("E0000140");
		dataunit.len_data = 1;
		dataunit.data_buffer[0] = 1;
		vDataUnit.push_back(dataunit);
	}
	if(m_bTerminalReset == true) //ADD BY WM 20131127 
	{
		ptcl_module_csg::pnfn_data_uint dataunit;
		dataunit.pnfn.pn = 0;
		dataunit.pnfn.fn = ptcl_packet_csg::convert_fn("E0000140");
		dataunit.len_data = 1;
		dataunit.data_buffer[0] = 0;
		vDataUnit.push_back(dataunit);
	}
	if(m_bAlarmNoShield == true) //ADD BY WM 20131207
	{
		ptcl_module_csg::pnfn_data_uint dataunit;
		dataunit.pnfn.pn = 0;
		dataunit.pnfn.fn = ptcl_packet_csg::convert_fn("E0000151");
		dataunit.len_data = 32;
		for(int i = 0; i < 32; i++ )
		{
			dataunit.data_buffer[i] = 0xFF;
		}
		vDataUnit.push_back(dataunit);
	}
	if(m_bEventNoShield == true) //ADD BY WM 20131207
	{
		ptcl_module_csg::pnfn_data_uint dataunit;
		dataunit.pnfn.pn = 0;
		dataunit.pnfn.fn = ptcl_packet_csg::convert_fn("E0000152");
		dataunit.len_data = 32;
		for(int i = 0; i < 32; i++ )
		{
			dataunit.data_buffer[i] = 0xFF;
		}
		vDataUnit.push_back(dataunit);
	}
	if(m_bZDSBDisable == true)
	{
		ptcl_module_csg::pnfn_data_uint dataunit;
		dataunit.pnfn.pn = 0;
		dataunit.pnfn.fn = ptcl_packet_csg::convert_fn("E0000150");
		dataunit.len_data = 32;
		for(int i = 0; i < 32; i++ )
		{
			dataunit.data_buffer[i] = 0x00;
		}
		vDataUnit.push_back(dataunit);
	}
	if (m_bZDCBCS)
	{
		vector<string>vParamID;
		vParamID.push_back("E0000230");	//��һ·485
		vParamID.push_back("E0000231");	//�ڶ�·485
		vParamID.push_back("E0000232");	//����·485
		vParamID.push_back("E0000233");	//����·485
		vParamID.push_back("E0000234");	//����·485
		vParamID.push_back("E0000235");	//����·485
		vParamID.push_back("E0000236");	//����·485
		vParamID.push_back("E0000237");	//�ڰ�·485
		
		unsigned char cbcspz[] = { 0x01, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
		for (int i = 0; i < vParamID.size(); i++)
		{
			ptcl_module_csg::pnfn_data_uint dataunit;
			dataunit.pnfn.pn = 0;
			dataunit.pnfn.fn = ptcl_packet_csg::convert_fn(vParamID.at(i));
			dataunit.len_data = sizeof(cbcspz);
			memcpy(dataunit.data_buffer, cbcspz, sizeof(cbcspz));
			vDataUnit.push_back(dataunit);
		}
	}

	if (vDataUnit.empty())
	{
		MessageBox("��ѡ�������", "��ʾ", MB_OK|MB_ICONINFORMATION);
		return;
	}

	ptcl_packet_csg packet;
	UINT64 id = ptcl_packet_csg::convert_device_addr(m_strTerminalAddr.GetBuffer(0));
	packet.set_device_addr(id);
	ptcl_module_csg::get_instance()->format_packet_set_param(&packet, (unsigned char *)"", &vDataUnit[0], vDataUnit.size());
	pChannel->Write((char *)packet.get_packet_buffer(), packet.get_packet_length(), id);
	CMsgOutput::GetInstance()->ShowpPacketInfo("", 0, TraceBuf(packet.get_packet_buffer(), packet.get_packet_length(), "�·�"));
	CLogMgr::GetInstanse().LogPacket("�·�", packet.get_packet_buffer(), packet.get_packet_length());

}

void CDlgReadTermInfo::OnReceive(int state, char *buf, int len, u_int64 strTermAddr)
{
	if (g_nModuleControl != Module_RndRead_TerminalInfo)
		return;

	if (len <= 0)
		return;

	CMsgOutput::GetInstance()->ShowpPacketInfo("", 0, TraceBuf((unsigned char *)buf, len, "����"));
	CLogMgr::GetInstanse().LogPacket("����", (unsigned char *)buf, len);

	ptcl_packet_csg packet;
    packet.set_packet_buffer((unsigned char *)buf,len);
    if (packet.parse_packet() <= 0)
		return;
    
	switch (packet.get_afn())
	{
	case 0x0A:
		{
			// ����������
			vector<data_parse_csg_parameter::data_item_value> vdata;
			data_parse_csg_parameter::parse_parameter(packet.get_data_area_buffer(), packet.get_data_area_length(), vdata);
			
			for (int i = 0; i < vdata.size(); i++)
			{
				if (vdata[i].data_item_id == ptcl_packet_csg::convert_fn("E0000130"))
				{
					COleVariant vtime(vdata[i].db_value.c_str());
					vtime.ChangeType(VT_DATE);
					COleDateTime oletime = vtime;
					m_ctrlDate.SetTime(vtime);	
					m_ctrlTime.SetTime(vtime);
					continue;
				}
				if (vdata[i].data_item_id == ptcl_packet_csg::convert_fn("E0000B01"))
				{
					m_strSoftVersion = vdata[i].db_value.c_str();
					continue;
				}
				if (vdata[i].data_item_id == ptcl_packet_csg::convert_fn("E0000B02"))
				{
					m_strHardVersion = vdata[i].db_value.c_str();					
					continue;
				}
				if (vdata[i].data_item_id >= 0xE0000230 && vdata[i].data_item_id <= 0xE0000237)
				{
					m_bRS485[vdata[i].data_item_id - 0xE0000230] = atoi(vdata[i].db_value.c_str());
					continue;
				}
			}

			UpdateData(FALSE);
		}
		break;
	case 0x04:
		{
			ProcessAFN04(packet.get_data_area_buffer(), packet.get_data_area_length());			
		}
		break;
	}
}

static const char *GetErrorInfo(unsigned char errcode)
{
	switch (errcode)
	{
	case 0x00:
		return "��ȷ";		
	case 0x01:
		return "�м�����û�з���";		
	case 0x02:
		return "�������ݷǷ�";		
	case 0x03:
		return "����Ȩ�޲���";		
	case 0x04:
		return "�޴�������";		
	case 0x05:
		return "����ʱ��ʧЧ";		
	case 0x06:
		return "Ŀ���ַ������";		
	case 0x07:
		return "У��ʧ��";		
	}

	return "";
}

void CDlgReadTermInfo::ProcessAFN04(unsigned char *buf, int len)
{
	// д��������
	string result = "д������";
	int nlen = 0;	
	while (len - nlen > 6)
	{
		// pnfn
		ptcl_module_csg::pnfn_data pnfn;
		ptcl_module_csg::parse_pnfn(buf + nlen, &pnfn);
		nlen += 6;

		char temp[256] = {0};		
		sprintf(temp, "�ն˵�ַ%s[%s:%s]; ", m_strTerminalAddr.GetBuffer(0), ptcl_packet_csg::convert_fn(pnfn.fn).c_str(), GetErrorInfo(*(buf + nlen)));
		result.append(temp);

		nlen += 1;
	}			
	ShowOperateLog(result.c_str());
}

void CDlgReadTermInfo::OnBnClickedCheckTime()
{
	UpdateData(TRUE);
	if (m_bTime && m_bUseSystemTime)
		m_bUseSystemTime = false;	
	UpdateData(FALSE);
}


void CDlgReadTermInfo::OnBnClickedCheckSystemtime()
{
	UpdateData(TRUE);
	if (m_bUseSystemTime && m_bTime)
		m_bTime = false;
	UpdateData(FALSE);
}
