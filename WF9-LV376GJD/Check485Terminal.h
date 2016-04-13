#ifndef	_Check485Terminal_H
#define _Check485Terminal_H

#include "jblib.h"
#include "Common.h"
#include "FormProgress.h"
#include "InputBoxPro.h"
#include "FormSelecte.h"
#include "Check485Meter.h"
#include "DataConvert.h"

ListBox ListBox_Check485Terminal;

unsigned char TestAll = 0;
unsigned char TestPort;
                                                             
void Check485TerminalLoad();
void Check485TerminalDoLoad(HWND Obj,PMsg Msg);
void ListBox_Check485Terminal_OnMsg(HWND Obj, PMsg Msg);
void Check485TerminalView(HWND Sender);
void Check485TerminalGoto(HWND Sender);
void Check485TerminalCheck(HWND Sender);
void Check485TerminalDoCheck(HWND Obj,PMsg Msg);
void Check485TerminalResult(HWND Sender);
void Check485TerminalDoResult(HWND Obj,PMsg Msg);
void Form_Check485TerminalResult_OnKeyPress(HWND Obj, PMsg Msg);
void Check485TerminalReset(HWND Sender);
void Check485TerminalDoReset(HWND Obj,PMsg Msg);
void Check485TerminalHelp(HWND Sender);


//485���ģ��
void Check485Terminal(HWND Sender)
{
  Window Form_Check485Terminal;
  Button Button_Goto, Button_Check, Button_Result, Button_Reset, Button_Help;

  CommParam.Parity = 0x09;		//Ĭ��żУ��
  
  Check485TerminalLoad();

  CreateListBox(&ListBox_Check485Terminal, ListBox_TerminalItems, 
    TerminalCount, 3, 3, 110, 124);
  ListBox_Check485Terminal.OnMsg = ListBox_Check485Terminal_OnMsg;  
  ListBox_Check485Terminal.OnClick = Check485TerminalView;
  CreateButton(&Button_Goto, "��ת1", 115, 3, 40, 20);
  Button_Goto.OnClick = Check485TerminalGoto;
  CreateButton(&Button_Check, "���2", 115, 29, 40, 20);
  Button_Check.OnClick = Check485TerminalCheck;
  CreateButton(&Button_Result, "����3", 115, 55, 40, 20);
  Button_Result.OnClick = Check485TerminalResult;
  CreateButton(&Button_Reset, "����4", 115, 81, 40, 20);
  Button_Reset.OnClick = Check485TerminalReset;
  CreateButton(&Button_Help, "����5", 115, 107, 40, 20);
  Button_Help.OnClick = Check485TerminalHelp;

  CreateWindow(&Form_Check485Terminal, "�������", 0, 0, 160, 160);
  AddToWindow(&Form_Check485Terminal, &ListBox_Check485Terminal);
  AddToWindow(&Form_Check485Terminal, &Button_Goto);
  AddToWindow(&Form_Check485Terminal, &Button_Check);
  AddToWindow(&Form_Check485Terminal, &Button_Result);
  AddToWindow(&Form_Check485Terminal, &Button_Reset);
  AddToWindow(&Form_Check485Terminal, &Button_Help);
    
  ShowWindow(&Form_Check485Terminal, SS_SHOWMAX);
}


//�ն˵����б����
void Check485TerminalLoad()
{
  int count;
  
  use(TERM_RECD);
  count = libsumr();
  
  TerminalCount = 0;
  memset(ListBox_TerminalItems, 0, sizeof(ListBox_TerminalItems));
  ShowFormProgress(count, Check485TerminalDoLoad);
}

//�ն˵����б���ش���
void Check485TerminalDoLoad(HWND Obj,PMsg Msg)
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
    {*/
      strcpy(ListBox_TerminalItems[TerminalCount], temp);
      TerminalCount++;
    //}      
    ProgressStep();
  }
  CloseFormProgress();
  use("");
}

void ListBox_Check485Terminal_OnMsg(HWND Obj, PMsg Msg)
{
  if (Msg->Msg == WM_CHAR)
  {
    switch (Msg->WParam)
    {
      case VK_UP:
        if (ListBox_Check485Terminal.index == 0)
        {
          ListBox_Check485Terminal.index = ListBox_Check485Terminal.count - 1;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_DOWN:
        if (ListBox_Check485Terminal.index == ListBox_Check485Terminal.count - 1)
        {
          ListBox_Check485Terminal.index = 0;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_1:
        Check485TerminalGoto(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_2:
        Check485TerminalCheck(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_3:
        Check485TerminalResult(NULL);
        Msg->Result = MSG_OK;
        break;
      case VK_4:         
      case VK_DEL:
        Check485TerminalReset(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_5:
      case VK_0:      	
        Check485TerminalHelp(NULL);
        Msg->Result = MSG_OK;
        break; 
    }
  }
}

void Check485TerminalView(HWND Sender)
{
	
	char filename[MAX_FILENAME_LEN] = {0};
	 
  if (ListBox_Check485Terminal.count < 1)
  {
    return;
  }
  
	memset(CurrMPAttrib.Term, 0, sizeof(CurrMPAttrib.Term));
  sprintf(CurrMPAttrib.Term, "%s", ListBox_Check485Terminal.text);
  
  if (GetRecdFileName(ListBox_Check485Terminal.text, filename) == NULL)
  {
  	MessageBox("δ�ҵ����ն˵Ĳ����㵵���ļ�!", STR_CUE, MB_OK);
  	return;
  }
  
  copy(filename, METE_RECD_GW);
  
  Check485Meter(ListBox_Check485Terminal.text);
}

void Check485TerminalGoto(HWND Sender)
{
  unsigned char term[MAX_ASSETNO_DIGITS] = {0};
  
  int i;

  if (InputBoxPro("��ת��", term, sizeof(term)) == ID_CANCEL)
  {
    return;
  }
//  GetTerminalAddress(term);
  GetAddress(term, 0);

  for (i = 0; i < TerminalCount; i++)
  {
    if (strcmp(term, ListBox_TerminalItems[i]) == 0)
    {
      ListBox_Check485Terminal.index = i;
      Refresh(&ListBox_Check485Terminal);
      break;
    }
  }
}

void Check485TerminalCheck(HWND Sender)
{
  int count;
  char strPort[NUM_5] = {0};
  char filename[MAX_FILENAME_LEN] = {0};

  if (ListBox_Check485Terminal.index < 0)
  {
    return;
  }
  
  memset(CurrMPAttrib.Term, 0, sizeof(CurrMPAttrib.Term));
  sprintf(CurrMPAttrib.Term, "%s", ListBox_Check485Terminal.text);  
  
  if (GetRecdFileName(ListBox_Check485Terminal.text, filename) == NULL)
  {
  	MessageBox("δ�ҵ����ն˵Ĳ����㵵���ļ�!", STR_CUE, MB_OK);
  	return;
  }
  
  copy(filename, METE_RECD_GW);
 
 	TestPort = 0;  
  TItem portItems[] = {"ȫ���˿�", "�˿�1", "�˿�2", "�˿�3", "�˿�4", "�˿�31",};
  count = sizeof(portItems)/sizeof(TItem);
  if (FormSelecte("��ѡ����˿ں�", &portItems, count, &TestPort, SS_SHOWDEFAULT) == ID_CANCEL)
  {
    return;
  }
  if (TestPort == 5)
  {
  	TestPort = 31;
  }  

  use(METE_RECD_GW);
  count = libsumr();
  TestAll = MessageBox("ѡ���ǡ��������\nѡ���񡱼��ʧ����", STR_CUE, MB_YESNO);  
  ShowFormProgress(count, Check485TerminalDoCheck);
}

void Check485TerminalDoCheck(HWND Obj, PMsg Msg)
{
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0};
  char filename[MAX_FILENAME_LEN] = {0};
  int r, i, j;
  int port = 0;
  int count = 0;
  DLT645INFO di;
  DLT645DATA ddin, ddout;
  unsigned long long dataItem = 0;
  unsigned char data[13] = {};

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
    
    if (TestPort)
    {
	    libset(r, FD_PORT);
	    memset(temp, 0, sizeof(temp));
	    libread(temp);
	    port = StrToInt(temp, DEC);
	    if (port != TestPort)
	    {
	      ProgressStep();
	      continue;    	
	    }
    }
    
    if (TestAll == ID_NO)
    {
      libset(r, FD_CKRES);
      memset(temp, 0, sizeof(temp));
      libread(temp);
      if (strcmp(temp, "07") == 0)
      {
        ProgressStep();
        continue;
      }        
      if (strcmp(temp, "97") == 0)
      {
        ProgressStep();
        continue;
      }    
    }
   
    DLT645_CreateInfo(&di);
    libset(r, FD_METERADDR);
    memset(temp, 0, sizeof(temp));
    libread(temp);
    dataItem = StrToHex(temp, sizeof(temp));
    memcpy(di.addr, &dataItem, di.addrlen);

    int baudRate[] = {2400, 1200, 9600};
    for (i = 0; i < 3; i++)
    { 
      uartinit(baudRate[i], CommParam.Parity, 0x01);
      Delay(150);
      
      if (i == 0)
      {
        ddin.CCode = 0x11;
        ddin.datalen = 0x04;
        dataItem = 0x00010000;
        memcpy(ddin.databuf, &dataItem, ddin.datalen);
        if (DLT645_RequestWS(&di, &ddin, &ddout) == 0)
        {
          libset(r, FD_BAUD);
          memset(temp, 0, sizeof(temp));
          sprintf(temp, "%d", baudRate[i]);
          libwrit(temp);

          libset(r, FD_PROTO);
          memset(temp, 0, sizeof(temp));
          sprintf(temp, "DL/T 645-2007");
          libwrit(temp);

          libset(r, FD_CKRES);
          memset(temp, 0, sizeof(temp));
          sprintf(temp, "07");
          libwrit(temp);
          
          memset(data, 0, sizeof(data));
          memset(temp, 0, sizeof(temp));
          memcpy(data, ddout.databuf+4, 4);
          sprintf(temp, "%02x%02x%02x.%02xkWh", data[3], data[2], data[1], data[0]);
          for(j = 0; j < 5; j++)
          {
            if(temp[j] != '0')
              break;
          }
          memset(data, 0, sizeof(data));
          memcpy(data, temp + j, strlen(temp) - j);
          
          libset(r, FD_ENERGY);
          libwrit(data);
          
          break;
        }
        else
        { 
          libset(r, FD_CKRES);
          memset(temp, 0, sizeof(temp));
          sprintf(temp, STR_FAIL);
          libwrit(temp);
        }
      }
      else
      {
        ddin.CCode = 0x01;
        ddin.datalen = 0x02;
        dataItem = 0x9010;
        memcpy(ddin.databuf, &dataItem, ddin.datalen);
        if (DLT645_RequestWS(&di, &ddin, &ddout) == 0)
        {
          libset(r, FD_BAUD);
          memset(temp, 0, sizeof(temp));
          sprintf(temp, "%d", baudRate[i]);
          libwrit(temp);

          libset(r, FD_PROTO);
          memset(temp, 0, sizeof(temp));
          sprintf(temp, "DL/T 645-1997");
          libwrit(temp);

          libset(r, FD_CKRES);
          memset(temp, 0, sizeof(temp));
          sprintf(temp, "97");
          libwrit(temp);
          
          memset(data, 0, sizeof(data));
          memset(temp, 0, sizeof(temp));
          memcpy(data, ddout.databuf+2, 4);
          sprintf(temp, "%02x%02x%02x.%02xkWh", data[3], data[2], data[1], data[0]);
          for(j = 0; j < 5; j++)
          {
            if(temp[j] != '0')
              break;
          }
          memset(data, 0, sizeof(data));
          memcpy(data, temp + j, strlen(temp) - j);
          
          libset(r, FD_ENERGY);
          libwrit(data);
          
          break;
        }
        else
        {          
          libset(r, FD_CKRES);
          memset(temp, 0, sizeof(temp));
          sprintf(temp, STR_FAIL);
          libwrit(temp);
        }
      }      

      uartinit(0, CommParam.Parity, 0x01);
    }    
    Beep(100);
    ProgressStep();
  }
  CloseFormProgress();
  use("");
 
 
  if (GetRecdFileName(ListBox_Check485Terminal.text, filename) != NULL)
  {
  	copy(METE_RECD_GW ,filename);
  }
     
  Check485TerminalResult(NULL);
  CloseFormProgress();
}

void Check485TerminalResult(HWND Sender)
{
  int count = 0;
  char filename[MAX_FILENAME_LEN] = {0};

  if (ListBox_Check485Terminal.count < 1)
  {
    return;
  }
  
  if (GetRecdFileName(ListBox_Check485Terminal.text, filename) != NULL)
  {
  	copy(METE_RECD_GW, filename);
  }
  else
  {
  	MessageBox("�ļ�������", STR_CUE, MB_OK);
  	return;
  }
  
  use(METE_RECD_GW);
  count = libsumr();
  ShowFormProgress(count, Check485TerminalDoResult);
}

void Check485TerminalDoResult(HWND Obj,PMsg Msg)
{
	int r = 0;
  int count = 0;
  char showInfo[128] = {0};
  unsigned char temp[MAX_ASSETNO_DIGITS] = {};
  Window Form_Check485TerminalResult;
  Text Text_Detail;
  
  int succCount = 0;
  int aborCount = 0;
  int noTestC = 0;
  int port = 0;
  int aborC1 = 0;
  int aborC2 = 0; 
  int aborC3 = 0;
  int aborC4 = 0;
  int aborC31 = 0;

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
    
    libset(r, FD_METERADDR);
    memset(temp, 0, sizeof(temp));
    libread(temp);
    if (temp[0] == '\0')
    {
      ProgressStep();
      continue;
    }
    
    count++;
 	 	
    libset(r, FD_CKRES);
    memset(temp, 0, sizeof(temp));
    libread(temp);
    if (temp[0] == '\0')
    {
    	noTestC++;
    	ProgressStep();
      continue;
    }
    if ((0 == strcmp(temp, "97")) || (0 == strcmp(temp, "07")))
    {
    	succCount++;
      ProgressStep();
      continue;
    } 
    else      	      
    {
      aborCount++;
      libset(r, FD_PORT);
      memset(temp, 0, sizeof(temp));
      libread(temp);
      port = StrToInt(temp, DEC);
      switch(port)
      {
      	case 1:
      		aborC1++;
      		break;
      	case 2:
      		aborC2++;
      		break;
      	case 3:
      		aborC3++;
      		break;
      	case 4:
      		aborC4++;
      		break;
      	case 31:
      		aborC31++;
      		break;
      	default:
      		break;       		
      }    
    }
       
    ProgressStep();
  }
  CloseFormProgress();
  use("");
  
  CreateText(&Text_Detail, "", 3, 3, 154, 150);
  sprintf(showInfo, "�ն˵�ַ��%s\n", ListBox_Check485Terminal.text);
//  CreateText(&Text_Count, "", 3, 21, 154, 15);
	memset(temp, 0, sizeof(temp));
  sprintf(temp, "���������%d\n", count);
  strcat(showInfo, temp);	
//  CreateText(&Text_SuccCount, "", 3, 39, 154, 15);
	memset(temp, 0, sizeof(temp));
  sprintf(temp, "���ɹ���%d\n", succCount);
  strcat(showInfo, temp);	
//  CreateText(&Text_AborCount, "", 3, 57, 154, 15);
	memset(temp, 0, sizeof(temp));
  sprintf(temp, "���ʧ�ܣ�%d\n", aborCount);
  strcat(showInfo, temp);	
//  CreateText(&Text_AborCount, "", 3, 57, 154, 15);
	memset(temp, 0, sizeof(temp));
  sprintf(temp, "δ���������%d\n", noTestC);
  strcat(showInfo, temp);	
//  CreateText(&Text_Detail, "", 3, 75, 154, 45);
  if (aborC1)
  {
  	memset(temp, 0, sizeof(temp));
  	sprintf(temp, "�˿�1ʧ�ܣ�%d ", aborC1);
  	strcat(showInfo, temp);
  }
  if (aborC2)
  {
  	memset(temp, 0, sizeof(temp));
  	sprintf(temp, "�˿�2ʧ�ܣ�%d ", aborC2);  
  	strcat(showInfo, temp);	
  }
  if (aborC3)
  {
  	memset(temp, 0, sizeof(temp));
  	sprintf(temp, "�˿�3ʧ�ܣ�%d ", aborC3); 
  	strcat(showInfo, temp);	 	
  }
  if (aborC4)
  {
  	memset(temp, 0, sizeof(temp));
  	sprintf(temp, "�˿�4ʧ�ܣ�%d ", aborC4); 
  	strcat(showInfo, temp);	 	
  }
  if (aborC31)
  {
  	memset(temp, 0, sizeof(temp));
  	sprintf(temp, "�˿�31ʧ�ܣ�%d", aborC31);  
  	strcat(showInfo, temp);		
  }      
  
  memset(Text_Detail.text, 0, sizeof(Text_Detail.text));
  sprintf(Text_Detail.text, "%s", showInfo);

  CreateWindow(&Form_Check485TerminalResult, "������Ϣ", 0, 0, 160, 160);
  Form_Check485TerminalResult.OnKeyPress = Form_Check485TerminalResult_OnKeyPress;
  //AddToWindow(&Form_Check485TerminalResult, &Text_Term);
  //AddToWindow(&Form_Check485TerminalResult, &Text_Count);
  //AddToWindow(&Form_Check485TerminalResult, &Text_SuccCount);
  //AddToWindow(&Form_Check485TerminalResult, &Text_AborCount);
  AddToWindow(&Form_Check485TerminalResult, &Text_Detail);
    
  if (ShowWindow(&Form_Check485TerminalResult, SS_SHOWMAX) == VK_ENTER)
  {
    Check485TerminalView(ListBox_Check485Terminal.handle);
  }
  CloseFormProgress();
}

void Form_Check485TerminalResult_OnKeyPress(HWND Obj, PMsg Msg)
{
  if (Msg->Msg == WM_CHAR)
  {
    switch (Msg->WParam)
    {
      case VK_ENTER:
        CloseWindow(Obj, VK_ENTER);
        break;
    }
  }        
}

void Check485TerminalReset(HWND Sender)
{
  int count;
  char filename[MAX_FILENAME_LEN] = {0};

  if (ListBox_Check485Terminal.count < 1)
  {
    return;
  }
  
  if (GetRecdFileName(ListBox_Check485Terminal.text, filename) == NULL)
  {
  	MessageBox("δ�ҵ����ն˵Ĳ����㵵���ļ�!", STR_CUE, MB_OK);
  	return;
  }
  copy(filename, METE_RECD_GW);
  
  if (MessageBox("ȷ��Ҫ���ø��ն������е��ļ������", 
    STR_CUE, MB_YESNO) == ID_NO)
  {
    return;
  }
    
  use(METE_RECD_GW);
  count = libsumr();
  ShowFormProgress(count, Check485TerminalDoReset);
}

void Check485TerminalDoReset(HWND Obj,PMsg Msg)
{
  unsigned char temp[MAX_ASSETNO_DIGITS] = {};
  int r, c;
  
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
    for (c = FD_BAUD; c <= FD_ENERGY; c++)
    {
      libset(r, c);
      libwrit("");
    } 
        
    ProgressStep();
  }
  CloseFormProgress();
  use("");
}

void Check485TerminalHelp(HWND Sender)
{
  MessageBox("�����λ���ն��б�ʱ\nȷ�ϼ����������\n����1����ת\n����2�����\n����3������\n����4/ɾ����������\n����5������", "����", MB_OK); 
}
#endif
