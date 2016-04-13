#ifndef _WirelessCollect_H
#define _WirelessCollect_H

#include "jblib.h"
#include "Common.h"
#include "FormProgress.h"
#include "InputBoxPro.h"
#include "WirelessMeter.h"


ListBox ListBox_WirelessCollect;

void WirelessCollectLoad();
void WirelessCollectDoLoad(HWND Obj,PMsg Msg);
void ListBox_WirelessCollect_OnMsg(HWND Obj, PMsg Msg);
void WirelessCollectView(HWND Sender);
void WirelessCollectGoto(HWND Sender);
void WirelessCollectChannel(HWND Sender);
void WirelessCollectDebug(HWND Sender);

void WirelessCollectRead(HWND Sender);

void WirelessCollectHelp(HWND Sender);



void WirelessCollect(unsigned char *terminal)
{

  Window Form_WirelessCollect;
  Button Button_Goto, Button_Channel, Button_Debug, Button_Read, Button_Help;

  WirelessCollectLoad();
  
  CreateListBox(&ListBox_WirelessCollect, ListBox_CollectItems, 
    CollectCount, 3, 3, 115, 124);
  ListBox_WirelessCollect.OnMsg = ListBox_WirelessCollect_OnMsg;  
  ListBox_WirelessCollect.OnClick = WirelessCollectView;
  CreateButton(&Button_Goto, "���1", 119, 3, 38, 20);
  Button_Goto.OnClick = WirelessCollectGoto;
  CreateButton(&Button_Channel, "Ƶ��2", 119, 29, 38, 20);
  Button_Channel.OnClick = WirelessCollectChannel;
  CreateButton(&Button_Debug, "����3", 119, 55, 38, 20);
  Button_Debug.OnClick = WirelessCollectDebug; 
  CreateButton(&Button_Read, "����4", 119, 81, 38, 20);    
  Button_Read.OnClick = WirelessCollectRead; 
  CreateButton(&Button_Help, "����5", 119, 107, 38, 20);
  Button_Help.OnClick = WirelessCollectHelp;
  

  CreateWindow(&Form_WirelessCollect, "", 0, 0, 160, 160);
  sprintf(Form_WirelessCollect.text, "�ն�%s�ɼ���", CurrMPAttrib.Term);
  AddToWindow(&Form_WirelessCollect, &ListBox_WirelessCollect);
  AddToWindow(&Form_WirelessCollect, &Button_Goto);
  AddToWindow(&Form_WirelessCollect, &Button_Channel);  
  AddToWindow(&Form_WirelessCollect, &Button_Debug);
  AddToWindow(&Form_WirelessCollect, &Button_Read);
  AddToWindow(&Form_WirelessCollect, &Button_Help);
    
  ShowWindow(&Form_WirelessCollect, SS_SHOWMAX);
  
  char filename[MAX_FILENAME_LEN] = {0};
  GetRecdFileName(CurrMPAttrib.Term, filename);
  copy(METE_RECD_GW, filename);	  
}


//�ɼ�����������
void WirelessCollectLoad()
{
  int count;
  use(METE_RECD_GW);
  count = libsumr();
  
  CollectCount = 0;
  memset(ListBox_CollectItems, 0, sizeof(ListBox_CollectItems));
  ShowFormProgress(count, WirelessCollectDoLoad);
}

//�ɼ����������ش���
void WirelessCollectDoLoad(HWND Obj,PMsg Msg)
{
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0}; 
  int i, r;
  
  for (r = 0; r < ProgressBar_Proc.max; r++)
  {
    libset(r, FD_FLAG);
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
    for (i = 0; i < CollectCount; i++)
    {
      if (strcmp(temp, ListBox_CollectItems[i]) == 0)
      {
        break;
      }
    }
    if (i == CollectCount)
    {
      strcpy(ListBox_CollectItems[CollectCount], temp);
      CollectCount++; 
    }      
    ProgressStep();
   
  }
  	
  CloseFormProgress();
  use("");
}


//�ɼ����б���Ϣ�����¼�
void ListBox_WirelessCollect_OnMsg(HWND Obj, PMsg Msg)
{
  if (Msg->Msg == WM_CHAR)
  {
    switch (Msg->WParam)
    {
      case VK_UP:
        if (ListBox_WirelessCollect.index == 0)
        {
          ListBox_WirelessCollect.index = ListBox_WirelessCollect.count - 1;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_DOWN:
        if (ListBox_WirelessCollect.index == ListBox_WirelessCollect.count - 1)
        {
          ListBox_WirelessCollect.index = 0;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_1:
        WirelessCollectGoto(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_2:
        WirelessCollectChannel(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_4:
        WirelessCollectRead(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_3:
        WirelessCollectDebug(NULL);
        Msg->Result = MSG_OK;
        break;
      case VK_5:
      case VK_0:
        WirelessCollectHelp(NULL);
        Msg->Result = MSG_OK;
        break; 
    }
  }
}


//�ɼ����б����¼�
void WirelessCollectView(HWND Sender)
{
	int r, count;
	unsigned char collect[MAX_ASSETNO_DIGITS] = {0};
	unsigned char temp[MAX_ASSETNO_DIGITS] = {0};
	
  if (ListBox_WirelessCollect.count < 1)
  {
    return;
  }
  
  memset(CurrMPAttrib.Collect, 0, sizeof(CurrMPAttrib.Collect));
  sprintf(CurrMPAttrib.Collect, "%s", ListBox_WirelessCollect.text);  
    
  WirelessMeter();
}


//��ת��λ����Ӳɼ���
void WirelessCollectGoto(HWND Sender)
{
  unsigned char collectNo[MAX_ASSETNO_DIGITS] = {};
  unsigned char collectAddr[MAX_ASSETNO_DIGITS] = {};
  unsigned char temp[MAX_ASSETNO_DIGITS] = {};
  int r, i;
  
  if (ListBox_WirelessCollect.count >= MAXCOLLECTCOUNT - 1)
  {
  	MessageBox("�ɼ��������Ѵ�����!", STR_CUE, MB_OK);
  	return;
  }
  
  while (1)
  {
    memset(collectNo, 0, sizeof(collectNo));
    memset(collectAddr, 0, sizeof(collectAddr));
    if (ListBox_WirelessCollect.count > 0)
    {
    	sprintf(collectNo, "%s", ListBox_WirelessCollect.text);
    }
    if (InputBoxPro("�ɼ������", collectNo, sizeof(collectNo)) == ID_CANCEL)
    {
      return;
    }
    strcpy(collectAddr, collectNo);    

    i = GetAddress(collectAddr, 4);    
    if(i)
    {
      MessageBox(ConstConfERRStr[i], STR_CUE, MB_OK);
      return;
    }
                            
    for (i = 0; i < CollectCount; i++)
    {
      if (strcmp(collectAddr, ListBox_CollectItems[i]) == 0)
      {
        break;
      }
    }
    if (i < CollectCount)
    {
      MessageBox("�ɼ����Ѵ���", STR_CUE, MB_OK);
      ListBox_WirelessCollect.index = i;
      Refresh(&ListBox_WirelessCollect);
      break; 
    }

		//���ɼ���������������㵵�����ݿ� 
	  use(METE_RECD_GW);	
	  libappend();
	  r = libgetr();
	  libset(r, FD_TERMADDR);
	  libwrit(CurrMPAttrib.Term);     
	  libset(r, FD_COLLNO);
	  libwrit(collectNo);
	  libset(r, FD_COLLADDR);
	  libwrit(collectAddr);	  
	  use("");

    strcpy(ListBox_CollectItems[CollectCount], collectAddr);
    ListBox_WirelessCollect.index = CollectCount;
    CollectCount++;
    ListBox_WirelessCollect.count = CollectCount;
    Refresh(&ListBox_WirelessCollect);
    
    break;
  }
    
}


//��ѯ����С����Ƶ��Ƶ��
void WirelessCollectChannel(HWND Sender)
{
	ChannelManage(ListBox_WirelessCollect.text, 4);
}


//���߲ɼ�������
void WirelessCollectDebug(HWND Sender)
{
	WirelessDebugTools(ListBox_WirelessCollect.text, 4);
}


//ֱ�ӳ���
void WirelessCollectRead(HWND Sender)
{
	char address[13] = {0};
	
	if (InputBox("����ַ", "������", address, 12) == ID_CANCEL)
  {
  	return;
  }

	FormatStr(address, 12, '0', 0); 

	WirelessReadMeter(address, ListBox_WirelessCollect.text, 0);
}


void WirelessCollectHelp(HWND Sender)
{
  MessageBox("���λ�ڲɼ����б�ʱ\nȷ�ϼ��������\n����1�����\n����2��Ƶ��\n����3������\n����4������\n����5������", "����", MB_OK); 
}
#endif
