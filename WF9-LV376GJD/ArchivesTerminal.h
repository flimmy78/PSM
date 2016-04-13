#ifndef _Archives_H
#define _Archives_H

#include "jblib.h"
#include "Common.h"
#include "FormProgress.h"
#include "InputBoxPro.h"
#include "ArchivesMeter.h"
#include "ArchivesCollect.h"
#include "Set_Addr_Time.h"

Window Form_ArchivesTerminal;
ListBox ListBox_ArchivesTerminal;

void ArchivesTerminalLoad();
void ArchivesTerminalDoLoad(HWND Obj,PMsg Msg);
void ListBox_ArchivesTerminal_OnMsg(HWND Obj, PMsg Msg);
void ArchivesTerminalView(HWND Sender);
void ArchivesTerminalGoto(HWND Sender);
void ArchivesTerminalAdd(HWND Sender);
void ArchivesTerminalDel(HWND Sender);
void ArchivesTerminalDoDel(HWND Obj,PMsg Msg);
void ArchivesTerminalClear(HWND Sender);
void ArchivesTerminalAmend(HWND Sender);
void ArchivesTerminalSetAddr(HWND Sender);
void ArchivesTerminalHelp(HWND Sender);

//�ն˵����������
void ArchivesTerminal(HWND Sender)
{
  Button Button_Goto, Button_Add, Button_Del, Button_Clear, Button_Amend;
  
  CurrMPAttrib.NWType = 0;
  ArchivesTerminalLoad();

  CreateListBox(&ListBox_ArchivesTerminal, ListBox_TerminalItems, 
    TerminalCount, 3, 3, 110, 124);
  ListBox_ArchivesTerminal.OnMsg = ListBox_ArchivesTerminal_OnMsg;  
  ListBox_ArchivesTerminal.OnClick = ArchivesTerminalView;
  CreateButton(&Button_Goto, "��ת1", 115, 3, 40, 20);
  Button_Goto.OnClick = ArchivesTerminalGoto;
  CreateButton(&Button_Add, "���2", 115, 29, 40, 20);
  Button_Add.OnClick = ArchivesTerminalAdd;
  CreateButton(&Button_Amend, "�޸�3", 115, 55, 40, 20);
  Button_Amend.OnClick = ArchivesTerminalAmend;
  CreateButton(&Button_Clear, "���4", 115, 81, 40, 20);
  Button_Clear.OnClick = ArchivesTerminalClear;
  CreateButton(&Button_Del, "ɾ��5", 115, 107, 40, 20);
  Button_Del.OnClick = ArchivesTerminalDel;

  CreateWindow(&Form_ArchivesTerminal, "�ն˵���", 0, 0, 160, 160);
  AddToWindow(&Form_ArchivesTerminal, &ListBox_ArchivesTerminal);
  AddToWindow(&Form_ArchivesTerminal, &Button_Goto);
  AddToWindow(&Form_ArchivesTerminal, &Button_Add);
  AddToWindow(&Form_ArchivesTerminal, &Button_Amend);  
  AddToWindow(&Form_ArchivesTerminal, &Button_Clear);
  AddToWindow(&Form_ArchivesTerminal, &Button_Del);
    
  ShowWindow(&Form_ArchivesTerminal, SS_SHOWMAX);
}


//�ն˵�������
void ArchivesTerminalLoad()
{
  int count = 0;
  char temp[NUM_10] = {0};
  
  if (fileexits(SYSCONFDBF) == FSNULL)
  {
  	MessageBox("�����ļ���ʧ!", STR_CUE, MB_OK);
  	return;
  }
  
  use(SYSCONFDBF);
  count = libsumr();
  if (count)
  {
  	libset(0, FD_ADDRDEC);
	  memset(temp, 0, sizeof(temp));
	  libread(temp);
	  CurrMPAttrib.AddrType = StrToInt(temp, DEC);
 
  	libset(0, FD_NWTYPE);
  	memset(temp, 0, sizeof(temp));
	  libread(temp);
	  CurrMPAttrib.NWType = StrToInt(temp, DEC);
    
  }
  use("");  
  
  use(TERM_RECD);
  count = libsumr();
  
  TerminalCount = 0;
  memset(ListBox_TerminalItems, 0, sizeof(ListBox_TerminalItems));
  ShowFormProgress(count, ArchivesTerminalDoLoad);
}

//�ն˵������ش���
void ArchivesTerminalDoLoad(HWND Obj,PMsg Msg)
{
  int r, i;
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0}; 
  
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
    }*/      
    strcpy(ListBox_TerminalItems[TerminalCount], temp);
    TerminalCount++;
    ProgressStep();
  }
  
  CloseFormProgress();
  use("");
}


//�ն��б���Ϣ�����¼�
void ListBox_ArchivesTerminal_OnMsg(HWND Obj, PMsg Msg)
{
  if (Msg->Msg == WM_CHAR)
  {
    switch (Msg->WParam)
    {
      case VK_UP:
        if (ListBox_ArchivesTerminal.index == 0)
        {
          ListBox_ArchivesTerminal.index = ListBox_ArchivesTerminal.count - 1;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_DOWN:
        if (ListBox_ArchivesTerminal.index == ListBox_ArchivesTerminal.count - 1)
        {
          ListBox_ArchivesTerminal.index = 0;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_1:
        ArchivesTerminalGoto(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_2:
        ArchivesTerminalAdd(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_DEL:
      case VK_5:        
        ArchivesTerminalDel(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_4:
        ArchivesTerminalClear(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_3:
        ArchivesTerminalAmend(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_0:
        ArchivesTerminalHelp(NULL);
        Msg->Result = MSG_OK;
        break;  
      case VK_FUNCTION:
        ArchivesTerminalSetAddr(NULL);
        Msg->Result = MSG_OK;
        break;               
    }
  }
}


//�ն��б����¼�
void ArchivesTerminalView(HWND Sender)
{
  int r, count;
  unsigned char termasset[MAX_ASSETNO_DIGITS] = {0};
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0};
  
  if (ListBox_ArchivesTerminal.count < 1)
  {
    return;
  }
    
  use(TERM_RECD);
  count = libsumr();
  for (r = 0; r < count; r++)
  {  	  
    libset(r, FD_FLAG);
    memset(temp, 0, sizeof(temp));    
    libread(temp);
    if (strcmp(temp, STR_DEL) == 0)
    {
    	continue;
    }

  	libset(r, FD_TERMADDR);    
    memset(temp, 0, sizeof(temp));    
    libread(temp); 
    if (strcmp(temp, ListBox_ArchivesTerminal.text) == 0)
    {
    	break;
    }     	
  }
 
  libset(r, FD_TERMNO);
  libread(termasset);   
  use("");
  
  memset(temp, 0, sizeof(temp));
  if (GetRecdFileName(ListBox_ArchivesTerminal.text, temp) != NULL)
  {
  	copy(temp, METE_RECD_GW);
  }
  else
  {
  	copy(METE_RECD_NEW_GW, METE_RECD_GW);
  }
 
  memset(CurrMPAttrib.Term, 0, sizeof(CurrMPAttrib.Term));
  sprintf(CurrMPAttrib.Term, "%s", ListBox_ArchivesTerminal.text);     

  if (CurrMPAttrib.NWType < 2)
  {
  	memset(CurrMPAttrib.Collect, 0, sizeof(CurrMPAttrib.Collect));
  	sprintf(CurrMPAttrib.Collect, "000000000000");  	  	
    ArchivesMeter(termasset, "");
  }
  else
  {
    ArchivesCollect(termasset);
  }
    
}


//��ת������Ҷ�λ�ն�
void ArchivesTerminalGoto(HWND Sender)
{
  unsigned char term[MAX_ASSETNO_DIGITS] = {0};
  int i;
  
  if (ListBox_ArchivesTerminal.count < 1)
  {
  	return;
  }

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
      ListBox_ArchivesTerminal.index = i;
      Refresh(&ListBox_ArchivesTerminal);
      break;
    }
  }
}


//�ն����
void ArchivesTerminalAdd(HWND Sender)
{
  unsigned char term[MAX_ASSETNO_DIGITS] = {0};
  unsigned char termno[MAX_ASSETNO_DIGITS] = {0};
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0};
  int i, r;
  
  if (ListBox_ArchivesTerminal.count >= MAXTERMINALCOUNT - 1)
  {
  	MessageBox("�ն������Ѵ�����,�����ɾ������ղ���!", STR_CUE, MB_OK);
  	return;
  }
 
  while (1)
  {
    memset(term, 0, sizeof(term));
    if (ListBox_ArchivesTerminal.count > 0)
    {
    	sprintf(term, "%s", ListBox_ArchivesTerminal.text);
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
      continue; 
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
    ListBox_ArchivesTerminal.index = TerminalCount;
    TerminalCount++;
    ListBox_ArchivesTerminal.count = TerminalCount;
    Refresh(&ListBox_ArchivesTerminal);
    
    break;
  }

  if (InputBoxProResult == ID_SCANOK)
  {
    ArchivesTerminalView(ListBox_ArchivesTerminal.handle);
  }
}

//�ն�ɾ��
void ArchivesTerminalDel(HWND Sender)
{

  int count;
	
  if (ListBox_ArchivesTerminal.count < 1)
  {
    return;
  }
  
  if (MessageBox("ɾ���ն˽�����ɾ�����ն��������豸��\nȷ��Ҫɾ����", 
    STR_CUE, MB_YESNO) == ID_NO)
  {
    return;
  }
  
  use(TERM_RECD);
  count = libsumr();
  ShowFormProgress(count, ArchivesTerminalDoDel);
}

//�ն�ɾ������
void ArchivesTerminalDoDel(HWND Obj,PMsg Msg)
{
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0};
  int r, i;
  
  for (r = 0; r < ProgressBar_Proc.max; r++)
  {
  	libset(r, FD_FLAG);
  	memset(temp, 0, sizeof(temp));
  	libread(temp);
  	if (strcmp(temp, STR_DEL) == 0)
  	{
  		continue;
  	}
  	
    libset(r, FD_TERMADDR);
    memset(temp, 0, sizeof(temp));
    libread(temp);
    if (strcmp(temp, ListBox_ArchivesTerminal.text) == 0)
    {
      libset(r, 0);
      libwrit(STR_DEL);
      break;
    }    
    ProgressStep();
  }
  
  CloseFormProgress();
  use(""); 
  
  memset(temp, 0, sizeof(temp));
  if (GetRecdFileName(ListBox_ArchivesTerminal.text, temp) != NULL)
  {
  	filedelete(temp);  		
  }  
  
  for (i = ListBox_ArchivesTerminal.index + 1; i < ListBox_ArchivesTerminal.count; i++)
  {
    strcpy(ListBox_TerminalItems[i - 1], ListBox_TerminalItems[i]);
    if (i == ListBox_ArchivesTerminal.count - 1)
    {
      memset(ListBox_TerminalItems[i], 0, sizeof(TItem));
    }
  }
   
  TerminalCount--;
  ListBox_ArchivesTerminal.count = TerminalCount;
  Refresh(&ListBox_ArchivesTerminal);   
}


//��������豸����(ֻ�����ն˵����������ݲ����㵵��)
void ArchivesTerminalClear(HWND Sender)
{
  int i, count;
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0};

  if (TerminalCount == 0)
  {
    return;
  }

  if (MessageBox("ȷ��Ҫ��������ն��� ��ȷ����Ҫ�����Ѿ�����!", 
    STR_CUE, MB_YESNO) == ID_NO)
  {
    return;
  } 

  copy(TERM_RECD, TERM_RECD_BAK);
  copy(TERM_RECD_NEW, TERM_RECD);
  
  for (i = 0; i < TerminalCount; i++)
  {
  	memset(temp, 0, sizeof(temp));
  	if (GetRecdFileName(ListBox_TerminalItems[i], temp) == NULL)
  	{
  		continue;
  	}
  	filedelete(temp);
  }
  
  memset(ListBox_TerminalItems, 0, sizeof(ListBox_TerminalItems));
  TerminalCount = 0;
  ListBox_ArchivesTerminal.count = TerminalCount;
  Refresh(&Form_ArchivesTerminal);
}


//�ն��޸�
void ArchivesTerminalAmend(HWND Sender)
{
  unsigned char term[MAX_ASSETNO_DIGITS] = {0};
  unsigned char termno[MAX_ASSETNO_DIGITS] = {0};
  unsigned char temp[MAX_ASSETNO_DIGITS] = {0};
  int i, r;
  int index;
  
  if (ListBox_ArchivesTerminal.count < 1)
  {
    return;
  }
  
  index = ListBox_ArchivesTerminal.index;
   
  while (1)
  {
    memset(term, 0, sizeof(term));
    sprintf(term, "%s", ListBox_ArchivesTerminal.text);
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
      if (i == ListBox_ArchivesTerminal.index)
      {
        continue;
      }
      if (strcmp(term, ListBox_TerminalItems[i]) == 0)
      {
        break;
      }
    }
    if (i < TerminalCount && i != ListBox_ArchivesTerminal.index)
    {
      MessageBox("�ն��Ѵ���", STR_CUE, MB_OK);
      continue; 
    }
    
    use(TERM_RECD);
    r = libsumr();
    for(i = 0; i < r; i++)
    {
      libset(i, FD_FLAG);
      memset(temp, 0, sizeof(temp));
      libread(temp);
      if (strcmp(temp, STR_DEL) == 0)
      {
        continue;
      }
      
      libset(i, FD_TERMADDR);
      memset(temp, 0, sizeof(temp));
      libread(temp);
      if (strcmp(temp, ListBox_ArchivesTerminal.text) == 0)
      {
        libset(i, FD_TERMNO);
        libwrit(termno);
        libset(i, FD_TERMADDR);
        libwrit(term);
        break;
      }     
    }       

    use("");
    
    memset(temp, 0, sizeof(temp));
    if (GetRecdFileName(ListBox_ArchivesTerminal.text, temp) != NULL)
    {
    	char filename[MAX_FILENAME_LEN] = {0};
    	GetRecdFileName(term, filename);
    	copy(temp, filename);
    	filedelete(temp);
    }
    
    strcpy(ListBox_ArchivesTerminal.text, term);
    memset(ListBox_TerminalItems[index], 0, sizeof(ListBox_TerminalItems[index]));
    strcpy(ListBox_TerminalItems[index], term);       
    ListBox_ArchivesTerminal.index = index;    
    Refresh(&ListBox_ArchivesTerminal);
    
    break;
  }

}


//�����ܼ� �����ն˵�ַ&ʱ��
void ArchivesTerminalSetAddr(HWND Sender)
{
  int res = 0;
  unsigned char term[MAX_ASSETNO_DIGITS] = {0};
  
  if (ListBox_ArchivesTerminal.index < 0)
  {
    return;
  }    
 
  sprintf(term, "%s", ListBox_ArchivesTerminal.text);     

  if (GetCommParam(&CommParam, 0) == ERROR)
  {
  	MessageBox(ConstConfERRStr[1], STR_CUE, MB_OK);
  	return;
  }
  


  res = SetAddrTime();
  if (res == ID_CANCEL)
  {
  	return;
  }
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150);
  switch(res)
  {
  	case 1:
  		res = SetTermAddr(term,res);
  		break;
  	case 2:
  		res = SetTermAddrWS(term);
  		break;
  	case 3:
  		res = SetTermTime(term);
  		break;
  	case 4:
  		res = SetTermTimeWS(term);
  		break;
  	default:
  		break;
  }

	
  uartinit(0, CommParam.Parity, CommParam.Port);  
  
  if (res == 0)
  {
  	return;
  }
  
  if (res == -1)
  {
  	MessageBox(SEND_FAIL, STR_CUE, MB_OK);
  }
  else if (res == -2)
  {
  	MessageBox(SEND_DENY, STR_CUE, MB_OK);   
  }
  else if (res == 1)
  {
  	MessageBox(SEND_OK, STR_CUE, MB_OK);
  }
    
}


void ArchivesTerminalHelp(HWND Sender)
{

  if (CurrMPAttrib.NWType < 2)
  {
    MessageBox("�����λ���ն��б�ʱ\nȷ�ϼ��������\n����1����ת\n����2�����\n����3���޸�\n����4�����\n����5/ɾ������ɾ��", "����", MB_OK); 
  }
  else
  {
    MessageBox("�����λ���ն��б�ʱ\nȷ�ϼ����ɼ�������\n����1����ת\n����2�����\n����3���޸�\n����4�����\n����5/ɾ������ɾ��", "����", MB_OK); 
  }
  return;
}
#endif
