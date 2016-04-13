#ifndef _SystemConf_H
#define _SystemConf_H

#include "jblib.h"
#include "Common.h"
#include "FormSelecte.h"
#include "FormSelCommType.h"
#include "DataConvert.h"
#include "InputBoxPro.h"
#include "FormSeparateAddr.h"
  
  
void DevAddrSeparate(unsigned char type)
{
  unsigned char select = 0;
  unsigned char temp[8] = {0};
  TItem items[] = {"�ֶ�����", "ɨ���ȡ", "ɨ���ѯ",};
  unsigned char f;
  unsigned char caption[20] = {0};
  StruGetAddrRule sRule;
  
  switch(type)
  {
  	case TERM:
  		f = FD_GETTERM;
  		sprintf(caption, "�ն˵�ַ��ȡ��ʽ");
  		break;
  	case METER:
  		f = FD_GETMETER;
  		sprintf(caption, "����ַ��ȡ��ʽ");
  		break;
  	case COLLECT:
  		f = FD_GETCOLLECT;
  		sprintf(caption, "�ɼ�����ַ��ȡ��ʽ");
  		break;
  	default:
  		return;
  }
  
  memset((unsigned char *)(&sRule), 0, sizeof(sRule));
  sRule.type = type;
  
  if (!(ReadGetAddrRule(&sRule)))
  {
  	MessageBox("���ݿ��ʧ��!", STR_CUE, MB_OK);
  	return;
  }

	select = sRule.rule;

  if (FormSelecte(caption, &items, sizeof(items)/sizeof(TItem), 
  	  &select, SS_SHOWCENTER) == ID_CANCEL)
  {
    return;
  }
 
  use(SYSCONFDBF); 
  
  libset(0, f); 
  memset(temp, 0, sizeof(temp));
  sprintf(temp, "%d", select);   
  libwrit(temp);  
   
  use("");  
  
  if (select != 1)
  {
  	return;
  }
  
	memset(caption, 0, sizeof(caption));
  switch(type)
  {
  	case TERM:
  		sprintf(caption, "�ն˵�ַ��ȡ��ʽ");
  		break;
  	case METER:
  		sprintf(caption, "����ַ��ȡ��ʽ");
  		break;
  	case COLLECT:
  		sprintf(caption, "�ɼ�����ַ��ȡ��ʽ");
  		break;
  	default:
  		return;
  } 
	  
  unsigned int res = 0;
  while(1)
  { 	
  	res = ShowFormAddrSeparate(caption, &sRule); 
  	if (res == ID_CANCEL || res == ID_OK)
  	{
  		break;
  	}
  	else if (res == ID_YES)
  	{
  		if (TestAddrSeparate(sRule.type))
  		{
  			continue;
  		}
  		break;
  	}
  }
    	
}


//�����ն˵�ַ��ȡ��ʽ
void TermAddrSeparate(HWND Sender)
{
  DevAddrSeparate(TERM);
}


//���õ���ַ��ȡ��ʽ
void MeterAddrSeparate(HWND Sender)
{
	DevAddrSeparate(METER);
}


//���òɼ�����ַ��ȡ��ʽ
void CollectAddrSeparate(HWND Sender)
{
	DevAddrSeparate(COLLECT);
}


//�����ƻ����ն�ͨ�ŷ�ʽ
void SetTermCommType(HWND Sender)
{
	StruCommParam sCommParam;
	if (GetCommParam(&sCommParam, 0) != 0)
	{
		sCommParam.Port = 0x05;
		sCommParam.Parity = 0x01;
		sCommParam.Baud = 1200;
	}
	
  if (FormSelCommType("�����ն�ͨ�ŷ�ʽ", &sCommParam) == ID_CANCEL)
  {
    return;
  }
	
	FormSelCommTypeSave(&sCommParam, 0);
}


//���õ��ͨѶУ�鷽ʽ
void SetMeterCheck(HWND Sender)
{
  unsigned char select = 0;
  unsigned char temp[8] = {0};
  TItem items[] = {"żУ��", "��У��", "��У��",};
  
  if(use(SYSCONFDBF) != 0)
  {
    MessageBox("���ݿ��ʧ��!", STR_CUE, MB_OK);
    return;
  }
  
  if(libsumr() == 0)
  {
    libappend();
  }
   
  libset(0, FD_METERCHECK);
  memset(temp, 0, sizeof(temp));
  libread(temp); 
  select = StrToInt(temp, HEX);
  switch(select)
  {
  	case 0x09:
  		select = 0;
  		break;
    case 0x01:
    	select = 1;
      break;
    case 0x0D:
    	select = 2;
      break;
    default:
      select = 0;
      break;  		
  }

  if (FormSelecte("���õ��У�鷽ʽ", &items, sizeof(items)/sizeof(TItem),
  	  &select, SS_SHOWCENTER) == ID_CANCEL)
  {
  	use("");
    return;
  }
  
  switch (select)
  {
    case 0:
    default:
      select = 0x09;
      break;
    case 1:
      select = 0x01;
      break;
    case 2:
      select = 0x0D;
      break;
  } 
  
  memset(temp, 0, sizeof(temp));
  sprintf(temp, "%02x", select);
  libset(0, FD_METERCHECK);
  libwrit(temp);
  
  use(""); 
}


//�����ն˵�ַ����
void SelectAddrType(HWND Sender)
{
  unsigned char select = 0;
  unsigned char temp[8] = {0};
  TItem items[] = {"10����", "16����",};
  
  if(use(SYSCONFDBF) != 0)
  {
    MessageBox("���ݿ��ʧ��!", STR_CUE, MB_OK);
    return;
  }
  
  if(libsumr() == 0)
  {
    libappend();
  }
  
  libset(0, FD_ADDRDEC);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  
  if(StrToInt(temp, DEC) == 16)
  {
  	select = 1;
  }
  
  if (FormSelecte("�ն��߼���ַ����", &items, sizeof(items)/sizeof(TItem),
  	  &select, SS_SHOWCENTER) == ID_CANCEL)
  {
  	use("");
    return;
  }
  
  if (select)
  {
  	select = 16;
  }
  else
  {
  	select = 10;
  }
  
  memset(temp, 0, sizeof(temp));
  sprintf(temp, "%d", select);   
  libset(0, FD_ADDRDEC);
  libwrit(temp);
  
  use(""); 
}


//���õ�ѹ�����ܹ�
void SetNetWorkType(HWND Sender)
{
  unsigned char select = 0;
  unsigned char temp[8] = {0};
  TItem items[] = {"˫��RS485�ṹ", "˫�������ز��ṹ", "����RS485�ṹ", "���������ز��ṹ",};
  
  if(use(SYSCONFDBF) != 0)
  {
    MessageBox("���ݿ��ʧ��!", STR_CUE, MB_OK);
    return;
  }
  
  if(libsumr() == 0)
  {
    libappend();
  }
  
  libset(0, FD_NWTYPE);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  
  select = StrToInt(temp, DEC);

  if (FormSelecte("���õ�ѹ�����ܹ�", &items, sizeof(items)/sizeof(TItem),
  	  &select, SS_SHOWCENTER) == ID_CANCEL)
  {
  	use("");
    return;
  }

  memset(temp, 0, sizeof(temp));
  sprintf(temp, "%d", select);
  libset(0, FD_NWTYPE);
  libwrit(temp);
  
  use(""); 
}


//��������ģ������
void SetWlsModuleType(HWND Sender)
{
  unsigned char select = 0;
  unsigned char temp[8] = {0};
  TItem items[] = {"CC1100", "SI1000",};
    
  if(use(SYSCONFDBF) != 0)
  {
    MessageBox("���ݿ��ʧ��!", STR_CUE, MB_OK);
    return;
  }
  
  if(libsumr() == 0)
  {
    libappend();
  }
  
  libset(0, FD_WLSTYPE);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  
  select = StrToInt(temp, DEC);

  if (FormSelecte("��������ģ������", &items, sizeof(items)/sizeof(TItem), 
  		&select, SS_SHOWCENTER) == ID_CANCEL)
  {
  	use("");
    return;
  }

  memset(temp, 0, sizeof(temp));
  sprintf(temp, "%d", select);
  libset(0, FD_WLSTYPE);
  libwrit(temp);
  
  use(""); 
}


//������������������
void SetBatchSendRecordNum(HWND Sender)
{
  unsigned char select = 0;
  unsigned char temp[8] = {0};
  TItem items[] = {"8", "16", "24", "32"};
    
  if(use(SYSCONFDBF) != 0)
  {
    MessageBox("���ݿ��ʧ��!", STR_CUE, MB_OK);
    return;
  }
  
  if(libsumr() == 0)
  {
    libappend();
  }
  
  libset(0, FD_SETRECDNUM);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  
  select = StrToInt(temp, DEC);

  if (FormSelecte("����������������", &items, sizeof(items)/sizeof(TItem), 
  		&select, SS_SHOWCENTER) == ID_CANCEL)
  {
  	use("");
    return;
  }

  memset(temp, 0, sizeof(temp));
  sprintf(temp, "%d", select);
  libset(0, FD_SETRECDNUM);
  libwrit(temp);
  
  use(""); 
}


//��ʾ����汾
void VersionView(HWND Sender)
{   
  if (fileexits(VERSIONMIF) == FSNULL)
	{
		MessageBox("��ʤ������ѹ����ϵͳ\n   �ƻ����Թ���\n  \nWF9\n  -LV376GJD-2.1\n   2013.2.27", "����汾", MB_OK); 
	}
	else
	{
		ShowData("����汾", VERSIONMIF);
	}
}


void SystemConf(HWND Sender)
{
  Window Form_SystemConf;
                              
  TMenu SystemConfMenu;     
                
  TMenuItem  SystemConfItems[]={
    {NULL,SetTermCommType," 1.�ն˱���ͨ�ŷ�ʽ"},
    {NULL,SetMeterCheck," 2.�����У�鷽ʽ"},
    {NULL,SelectAddrType," 3.�ն��߼���ַ����"},
    {NULL,TermAddrSeparate," 4.�ն˵�ַ��ȡ��ʽ"},                
    {NULL,MeterAddrSeparate," 5.����ַ��ȡ��ʽ"},
    {NULL,CollectAddrSeparate," 6.�ɼ���ַ��ȡ��ʽ"},
    {NULL,SetNetWorkType," 7.��ѹ�����ܹ�����"},
    //{NULL,SetWlsModuleType," 8.����ģ����������"},
    {NULL,SetBatchSendRecordNum," 8.��������������"},
    {NULL,VersionView," 9.����汾�鿴"},              
    };                     

  CreateWindow(&Form_SystemConf, "ϵͳ����", 0,0,160,160);
  CreateTMenu(&SystemConfMenu,SystemConfItems,
  sizeof(SystemConfItems)/sizeof(TMenuItem),8,3,144,126,FONT_EN_12|FONT_CH_12);
                               
  AddToWindow(&Form_SystemConf,&SystemConfMenu);
                
  ShowWindow(&Form_SystemConf,SS_SHOWMAX); 
}
#endif
