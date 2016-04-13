#ifndef _WirelessMeter_H
#define _WirelessMeter_H

#include "jblib.h"
#include "Common.h"
#include "FormProgress.h"
#include "FormSelecte.h"
#include "InputBoxPro.h"
#include "AttributeMeter.h"
#include "WirelessDebug.h"

ListBox ListBox_WirelessMeter;

void WirelessMeterLoad();
void WirelessMeterDoLoad(HWND Obj,PMsg Msg);
void ListBox_WirelessMeter_OnMsg(HWND Obj, PMsg Msg);
void WirelessMeterView(HWND Sender);
void WirelessMeterGoto(HWND Sender);
void WirelessMeterChannel(HWND Sender);
void WirelessMeterDebug(HWND Sender);

void WirelessMeterRead(HWND Sender);
void WirelessMeterHelp(HWND Sender);
void WirelessWritMeterRecord(char *meter, char *meterno);


void WirelessMeter(void)
{
  
  Window Form_WirelessMeter;
  Button Button_Goto, Button_Channel, Button_Debug, Button_Read, Button_Help;

  WirelessMeterLoad();
  
  CreateListBox(&ListBox_WirelessMeter, ListBox_MeterItems, 
    MeterCount, 3, 3, 115, 124);
  ListBox_WirelessMeter.OnMsg = ListBox_WirelessMeter_OnMsg;  
  ListBox_WirelessMeter.OnClick = WirelessMeterView;
  CreateButton(&Button_Goto, "���1", 119, 3, 38, 20);
  Button_Goto.OnClick = WirelessMeterGoto;
  CreateButton(&Button_Channel, "Ƶ��2", 119, 29, 38, 20);
  Button_Channel.OnClick = WirelessMeterChannel;
  CreateButton(&Button_Debug, "����3", 119, 55, 38, 20);
  Button_Debug.OnClick = WirelessMeterDebug;
  CreateButton(&Button_Read, "����4", 119, 81, 38, 20);
  Button_Read.OnClick = WirelessMeterRead;
  CreateButton(&Button_Help, "����5", 119, 107, 38, 20);
  Button_Help.OnClick = WirelessMeterHelp;

  CreateWindow(&Form_WirelessMeter, "", 0, 0, 160, 160);
  sprintf(Form_WirelessMeter.text, "�ն�%s�����", CurrMPAttrib.Term);
  AddToWindow(&Form_WirelessMeter, &ListBox_WirelessMeter);
  AddToWindow(&Form_WirelessMeter, &Button_Goto);
  AddToWindow(&Form_WirelessMeter, &Button_Channel);
  AddToWindow(&Form_WirelessMeter, &Button_Debug);
  AddToWindow(&Form_WirelessMeter, &Button_Read);
  AddToWindow(&Form_WirelessMeter, &Button_Help);
      
  ShowWindow(&Form_WirelessMeter, SS_SHOWMAX);
  
  char filename[MAX_FILENAME_LEN] = {0};
  GetRecdFileName(CurrMPAttrib.Term, filename);
  copy(METE_RECD_GW, filename);	 
	
}


//���������
void WirelessMeterLoad()
{
  int count;
  use(METE_RECD_GW);
  count = libsumr();
  
  MeterCount = 0;
  memset(ListBox_MeterItems, 0, sizeof(ListBox_MeterItems));
  ShowFormProgress(count, WirelessMeterDoLoad);
}

//��������ش���
void WirelessMeterDoLoad(HWND Obj,PMsg Msg)
{
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0}; 
  int r;
  
  for (r = 0; r < ProgressBar_Proc.max; r++)
  {
  	
    libset(r, 0);
    memset(temp, 0, sizeof(temp));
    libread(temp);
    if (strcmp(temp, STR_DEL) == 0)
    {
      ProgressStep();
      continue;
    }

    libset(r, FD_COLLADDR);
    memset(temp, 0, sizeof(temp));
    libread(temp);
    if (strcmp(temp, CurrMPAttrib.Collect) != 0)
    {
      ProgressStep();
      continue;     
    }
            
    libset(r, FD_METERADDR);
    memset(temp, 0, sizeof(temp));
    libread(temp);
    if (temp[0] == '\0')
    {
    	ProgressStep();
    	continue;
    }
    strcpy(ListBox_MeterItems[MeterCount], temp);
    MeterCount++;      
    
    ProgressStep();
  }
  
  CloseFormProgress();
  use("");
}


//����б���Ϣ�����¼�
void ListBox_WirelessMeter_OnMsg(HWND Obj, PMsg Msg)
{
  if (Msg->Msg == WM_CHAR)
  {
    switch (Msg->WParam)
    {
      case VK_UP:
        if (ListBox_WirelessMeter.index == 0)
        {
          ListBox_WirelessMeter.index = ListBox_WirelessMeter.count - 1;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_DOWN:
        if (ListBox_WirelessMeter.index == ListBox_WirelessMeter.count - 1)
        {
          ListBox_WirelessMeter.index = 0;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_1:
        WirelessMeterGoto(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_2:
        WirelessMeterChannel(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_3:
        WirelessMeterDebug(NULL);
        Msg->Result = MSG_OK;
        break;        
      case VK_4:
        WirelessMeterRead(NULL);
        Msg->Result = MSG_OK;
        break;
      case VK_5: 
      case VK_0:
        WirelessMeterHelp(NULL);
        Msg->Result = MSG_OK;
        break;         
    }
  }
}


//����б����¼�
void WirelessMeterView(HWND Sender)
{
  if (ListBox_WirelessMeter.count < 1)
  {
    return;
  }

  memset(CurrMPAttrib.Meter, 0, sizeof(CurrMPAttrib.Meter));
  sprintf(CurrMPAttrib.Meter, "%s", ListBox_WirelessMeter.text);

  AttributeMeter();  
}


//��ת��λ����ӵ��
void WirelessMeterGoto(HWND Sender)
{
  unsigned char meterNo[MAX_ASSETNO_DIGITS] = {0};
  unsigned char meterAddr[MAX_ASSETNO_DIGITS] = {0};
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0};
  int r, i;
  
  if (ListBox_WirelessMeter.count >= MAXMETERCOUNT - 1)
  {
  	MessageBox("��������Ѵ�����!", STR_CUE, MB_OK);
  	return;
  }
  
  while (1)
  {
    memset(meterNo, 0, sizeof(meterNo));
    memset(meterAddr, 0, sizeof(meterAddr));
    if (ListBox_WirelessMeter.count > 0)
    {
    	sprintf(meterNo, "%s", ListBox_WirelessMeter.text);
    }
    if (InputBoxPro("�����", meterNo, sizeof(meterNo)) == ID_CANCEL)
    {
      return;
    }
    strcpy(meterAddr, meterNo);    
    
    i = GetAddress(meterAddr, 2);
    
    if (i)
    {
      MessageBox(ConstConfERRStr[i], STR_CUE, MB_OK);
      return;
    }
                            
    /*for (i = 0; i < MeterCount; i++)
    {
      if (strcmp(meterAddr, ListBox_MeterItems[i]) == 0)
      {
        break;
      }
    }
    if (i < MeterCount)
    {
      MessageBox("����Ѵ���", STR_CUE, MB_OK);
      continue; 
    }*/
    
    if (LookUpMeter(meterAddr, METE_RECD_GW))
    {
      MessageBox("����Ѵ���", STR_CUE, MB_OK);
      continue;   		
    }
 
    WirelessWritMeterRecord(meterAddr, meterNo);

    strcpy(ListBox_MeterItems[MeterCount], meterAddr);
    ListBox_WirelessMeter.index = MeterCount;
    MeterCount++;
    ListBox_WirelessMeter.count = MeterCount;
    Refresh(&ListBox_WirelessMeter);

    if (InputBoxProResult == ID_OK)
    {
      break;
    }
  }
}


//��ѯ����С����Ƶ��Ƶ��
void WirelessMeterChannel(HWND Sender)
{
	ChannelManage(ListBox_WirelessMeter.text, 2);
}


//С���ߵ���
void WirelessMeterDebug(HWND Sender)
{
	WirelessDebugTools(ListBox_WirelessMeter.text, 2);
}



//ֱ�ӳ���
void WirelessMeterRead(HWND Sender)
{

	char address[13] = {0};
	char type = 0;
	
	if (ListBox_WirelessMeter.count < 1)
	{
		if (InputBox("����ַ", "������", address, 12) == ID_CANCEL)
	  {
	  	return;
	  }
	
		FormatStr(address, 12, '0', 0); 
	}
	else
	{
		type = GetMeterProto(ListBox_WirelessMeter.text, METE_RECD_GW);
		sprintf(address, "%s", ListBox_WirelessMeter.text);
	}

	WirelessReadMeter(address, CurrMPAttrib.Collect, type);
}


//�����д�����ݿ�
void WirelessWritMeterRecord(char *meter, char *meterno)
{
  char temp[MAX_ASSETNO_DIGITS] = {0};
  int i, j, r;
  int mpNo = 1;
  unsigned char appendFlag = 1;
  
  use(METE_RECD_GW);
  r = libsumr();
  
  for(i = 0; i < r; i++)
  {
    memset(temp, 0, sizeof(temp));
    libset(i, 0);
    libread(temp);
    if (strcmp(temp, STR_DEL) == 0)
    {
      continue;
    }  
       
    memset(temp, 0, sizeof(temp));
    libset(i, FD_COLLADDR);
    libread(temp);
    if (strcmp(CurrMPAttrib.Collect, temp) != 0)
    {
      continue;
    }   
    
    memset(temp, 0, sizeof(temp));
    libset(i, FD_METERADDR);
    libread(temp);   
    if (temp[0] = '\0')
    {
    	appendFlag = 0;
    	break;
    }      
  }
  
  if (appendFlag)
  { 
	  libappend();
	  r = libgetr();  
	}
	else
	{
		r = i;
	} 
	
	if (0 == r)
	{
	  libset(r, FD_TERMADDR);
	  libwrit(CurrMPAttrib.Term);  
	}
	 
  libset(r, FD_COLLADDR);
  libwrit(CurrMPAttrib.Collect);  
  libset(r, FD_METERNO);
  libwrit(meterno);
  libset(r, FD_METERADDR);
  libwrit(meter);
   
  if (CurrMPAttrib.NWType > 1)
  {
  	mpNo = r + 2;  	
  }
  else
  {
	  mpNo = r + 1;
	}
  memset(temp, 0, sizeof(temp));
  sprintf(temp, "%d", mpNo);
  libset(r, FD_DEVNO);
  libwrit(temp);
  libset(r, FD_MPNO);
  libwrit(temp);
  
  libset(r, FD_PORT);
  memset(temp, 0, sizeof(temp));
	CurrMPAttrib.Port = 31;
	sprintf(temp, "%d", CurrMPAttrib.Port);
	libwrit(temp);	
  
  libset(r, FD_RATES);
  libwrit("4");  
  libset(r, FD_SORT);
  libwrit("51");
  
  use("");
}


void WirelessMeterHelp(HWND Sender)
{
  MessageBox("�����λ�ڵ���б�ʱ\nȷ�ϼ������õ������\n����1�����\n����2��Ƶ��\n����3������\n����4������\n����5������", "����", MB_OK); 
}
#endif
