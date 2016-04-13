
#include "jblib.h"
#include "Common.h"
#include "FormProgress.h"
#include "InputBoxPro.h"
#include "FormSelecte.h"
#include "WirelessCollect.h"
#include "DataConvert.h"


ListBox ListBox_WirelessDebug;


void WirelessDebugLoad();
void WirelessDebugDoLoad(HWND Obj,PMsg Msg);
void ListBox_WirelessDebug_OnMsg(HWND Obj, PMsg Msg);
void WirelessDebugView(HWND Sender);
void WirelessDebugGoto(HWND Sender);
void WirelessDebugChannel(HWND Sender);
void WirelessTerminalDebug(HWND Sender);
void WirelessDebugRead(HWND Sender);
void WirelessDebugHelp(HWND Sender);


void WirelessDebug(HWND Sender)
{
  Window Form_WirelessDebug;
  Button Button_Goto, Button_Channel, Button_Debug, Button_Read, Button_Help;
  
  WlsModuleType = GetWlsModuleType();
  WlsSEQ = 0x01;
  
  CommParam.Baud = 9600;
  CommParam.Parity = 0x01;
  CommParam.Port = 0x03; 
      
  WirelessDebugLoad();

  CreateListBox(&ListBox_WirelessDebug, ListBox_TerminalItems, 
    TerminalCount, 3, 3, 110, 124);
  ListBox_WirelessDebug.OnMsg = ListBox_WirelessDebug_OnMsg;  
  ListBox_WirelessDebug.OnClick = WirelessDebugView;
  CreateButton(&Button_Goto, "���1", 115, 3, 40, 20);
  Button_Goto.OnClick = WirelessDebugGoto;
  CreateButton(&Button_Channel, "Ƶ��2", 115, 29, 40, 20);
  Button_Channel.OnClick = WirelessDebugChannel;
  CreateButton(&Button_Debug, "����3", 115, 55, 40, 20);
  Button_Debug.OnClick = WirelessTerminalDebug;
  CreateButton(&Button_Read, "����4", 115, 81, 40, 20);
  Button_Read.OnClick = WirelessDebugRead;
  CreateButton(&Button_Help, "����5", 115, 107, 40, 20);
  Button_Help.OnClick = WirelessDebugHelp;

  CreateWindow(&Form_WirelessDebug, "���ߵ���", 0, 0, 160, 160);
  AddToWindow(&Form_WirelessDebug, &ListBox_WirelessDebug);
  AddToWindow(&Form_WirelessDebug, &Button_Goto);
  AddToWindow(&Form_WirelessDebug, &Button_Channel);
  AddToWindow(&Form_WirelessDebug, &Button_Debug);
  AddToWindow(&Form_WirelessDebug, &Button_Read);
  AddToWindow(&Form_WirelessDebug, &Button_Help);
    
  ShowWindow(&Form_WirelessDebug, SS_SHOWMAX);
  
  if (fileexits(WLSINFOTXT) != FSNULL)
  {
    filedelete(WLSINFOTXT);
  }

}


//�ն��б����
void WirelessDebugLoad()
{
  int count;
  use(TERM_RECD);
  count = libsumr();
  
  TerminalCount = 0;
  memset(ListBox_TerminalItems, 0, sizeof(ListBox_TerminalItems));
  ShowFormProgress(count, WirelessDebugDoLoad);
}

//�ն��б���ش���
void WirelessDebugDoLoad(HWND Obj,PMsg Msg)
{
  int r, i;
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0}; 
  
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
     
    libset(r, FD_TERMADDR);
    memset(temp, 0, sizeof(temp));
    libread(temp);
    /*for (i = 0; i < TerminalCount; i++)
    {
      if (strcmp(temp, ListBox_TerminalItems[i]) == 0)
      {
        break;
      }
    }
    if (i == TerminalCount)
    {
      strcpy(ListBox_TerminalItems[TerminalCount], temp);
      TerminalCount++;
    } */
    strcpy(ListBox_TerminalItems[TerminalCount], temp);
    TerminalCount++;     
    ProgressStep();
  }
  CloseFormProgress();
  use("");
}


//�ն��б���Ϣ�����¼�
void ListBox_WirelessDebug_OnMsg(HWND Obj, PMsg Msg)
{
	
  memset(CurrMPAttrib.Term, 0, sizeof(CurrMPAttrib.Term));
  sprintf(CurrMPAttrib.Term, "%s", ListBox_WirelessDebug.text);
  
  if (Msg->Msg == WM_CHAR)
  {
    switch (Msg->WParam)
    {
      case VK_UP:
        if (ListBox_WirelessDebug.index == 0)
        {
          ListBox_WirelessDebug.index = ListBox_WirelessDebug.count - 1;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_DOWN:
        if (ListBox_WirelessDebug.index == ListBox_WirelessDebug.count - 1)
        {
          ListBox_WirelessDebug.index = 0;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_1:
        WirelessDebugGoto(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_2:
        WirelessDebugChannel(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_3:
        WirelessTerminalDebug(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_4:
        WirelessDebugRead(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_5:          
      case VK_0:        
        WirelessDebugHelp(NULL);
        Msg->Result = MSG_OK;
        break;         
    }
  }
}


//�ն��б����¼�
void WirelessDebugView(HWND Sender)
{
  
 	char filename[MAX_FILENAME_LEN] = {0};
	
  if (ListBox_WirelessDebug.count < 1)
  {
    return;
  }
    
  if (GetRecdFileName(ListBox_WirelessDebug.text, filename) == NULL)
  {
  	return;
  }  
    
  GetMPLocate(&CurrMPAttrib, filename);
  if (CurrMPAttrib.MPNum < 1)
  {
  	return;
  }
  copy(filename, METE_RECD_GW);  
   
  memset(CurrMPAttrib.Term, 0, sizeof(CurrMPAttrib.Term));
  sprintf(CurrMPAttrib.Term, "%s", ListBox_WirelessDebug.text);
   
  WirelessCollect(ListBox_WirelessDebug.text);
  
}


//��ת��λ������ն�
void WirelessDebugGoto(HWND Sender)
{
  unsigned char term[MAX_ASSETNO_DIGITS] = {0};
  unsigned char termno[MAX_ASSETNO_DIGITS] = {0};
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0};
  int i, r;
  
  if (ListBox_WirelessDebug.count >= MAXTERMINALCOUNT - 1)
  {
  	MessageBox("�ն������Ѵ�����,�����ɾ������ղ���!", STR_CUE, MB_OK);
  	return;
  }
 
  while (1)
  {
    memset(term, 0, sizeof(term));
    if (ListBox_WirelessDebug.count > 0)
    {
    	sprintf(term, "%s", ListBox_WirelessDebug.text);
    }
    
    if (InputBoxPro("�ն˱��", term, sizeof(term)) == ID_CANCEL)
    {
      return;
    }
    
    strcpy(termno, term);
    
    i = GetAddress(term, 0);
    if (i)
    {
    	MessageBox(ConstConfERRStr[i], STR_CUE, MB_OK);
    	return;
    }       

    for (i = 0; i < TerminalCount; i++)
    {
      if (strcmp(term, ListBox_TerminalItems[i]) == 0)
      {
        break;
      }
    }
    if (i < TerminalCount)
    {
      MessageBox("�ն��Ѵ���", STR_CUE, MB_OK);
      ListBox_WirelessDebug.index = i;
      Refresh(&ListBox_WirelessDebug);
      break; 
    }

		//�����ն˵������ն˵������ݿ�    
    use(TERM_RECD);    
    libappend();
    r = libgetr();
    libset(r, FD_TERMNO);
    libwrit(termno);
    libset(r, FD_TERMADDR);
    libwrit(term);
    libset(r, FD_ADDRTYPE);
    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%d", CurrMPAttrib.AddrType);
    libwrit(temp);
    use("");
    
    strcpy(ListBox_TerminalItems[TerminalCount], term);
    ListBox_WirelessDebug.index = TerminalCount;
    TerminalCount++;
    ListBox_WirelessDebug.count = TerminalCount;
    Refresh(&ListBox_WirelessDebug);
    
    break;
  }
}


//�л�Ƶ��Ƶ��
void WirelessDebugChannel(HWND Sender)
{
	ChannelManage("", 0);
}


//���ߵ���
void WirelessTerminalDebug(HWND Sender)
{
	WirelessDebugTools("", 0);
}


//ֱ�ӳ������ߵ��
void WirelessDebugRead(HWND Sender)
{
	
	char address[13] = {0};
	
	if (InputBox("����ַ", "������", address, 12) == ID_CANCEL)
  {
  	return;
  }

	FormatStr(address, 12, '0', 0); 

	WirelessReadMeter(address, "", 0);
}


void WirelessDebugHelp(HWND Sender)
{
  MessageBox("�����λ���ն��б�ʱ\nȷ�ϼ����ɼ�������\n����1�����\n����2��Ƶ��\n����3������\n����4������\n����5������", "����", MB_OK); 
}