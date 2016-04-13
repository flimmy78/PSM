#ifndef _MeterOperate_H
#define _MeterOperate_H

#include "jblib.h"
#include "Common.h"
#include "Global.h"
#include "FormProgress.h"
#include "InputBoxPro.h"
#include "DataConvert.h"
#include "DataEncap.h"
#include "DataParse.h"

ListBox ListBox_MeterOperate;

U32 ReadHisDataDate; 

Text ReadDataText_MPNo, ReadDataText_RT, ReadDataText_Res,
  ReadDataText_Data, ReadDataText_MeteAddr, ReadDataText_Port; 
  
unsigned char ReadDataType;
unsigned char *Terminal;
unsigned char *Collect;

void MeterOperateLoad();
void MeterOperateDoLoad(HWND Obj,PMsg Msg);
void ListBox_MeterOperate_OnMsg(HWND Obj, PMsg Msg);
void MeterOperateView(HWND Sender);
void MeterOperateGoto(HWND Sender);
void ReadMPData(HWND Sender);
void ReadMPDataDo(HWND Sender);
void ReadMPDataDoSend(HWND Obj, PMsg Msg);
void ReadMeterData(HWND Sender);
void ReadMeterDataDo(HWND Obj, PMsg Msg);
void MeterOperateHelp(HWND Sender);
void ShowReadDataInfo(HWND Sender);
void ShowReadDataResult(unsigned char fn);
void Form_ReadDataResult_OnKeyPress(HWND Obj, PMsg Msg);
void Form_ReadDataResult_Refresh(HWND Obj);
U32 GetHisDataDate(U32 sourDate);


void MeterOperate(unsigned char *terminal)
{
  Terminal = terminal;
  Window Form_MeterOperate;
  Button Button_Goto, Button_Direct, Button_Data, Button_Info, Button_Help;
  
  MeterOperateLoad();

  CreateListBox(&ListBox_MeterOperate, ListBox_MeterItems, 
    MeterCount, 3, 3, 115, 124);
  ListBox_MeterOperate.OnMsg = ListBox_MeterOperate_OnMsg;  
  ListBox_MeterOperate.OnClick = MeterOperateView;
  CreateButton(&Button_Goto, "��ת1", 119, 3, 38, 20);
  Button_Goto.OnClick = MeterOperateGoto;
  CreateButton(&Button_Data, "����2", 119, 29, 38, 20);
  Button_Data.OnClick = ReadMPData;
  CreateButton(&Button_Direct, "͸��3", 119, 55, 38, 20);
  Button_Direct.OnClick = ReadMeterData;  
  CreateButton(&Button_Info, "���4", 119, 81, 38, 20);
  Button_Info.OnClick = ShowReadDataInfo;
  CreateButton(&Button_Help, "����5", 119, 107, 38, 20);
  Button_Help.OnClick = MeterOperateHelp;

  CreateWindow(&Form_MeterOperate, "", 0, 0, 160, 160);
  sprintf(Form_MeterOperate.text, "�ն�%s�����ݲ�ѯ", terminal);
  AddToWindow(&Form_MeterOperate, &ListBox_MeterOperate);
  AddToWindow(&Form_MeterOperate, &Button_Goto);
  AddToWindow(&Form_MeterOperate, &Button_Data);
  AddToWindow(&Form_MeterOperate, &Button_Direct);
  AddToWindow(&Form_MeterOperate, &Button_Info);  
  AddToWindow(&Form_MeterOperate, &Button_Help);
    
  ShowWindow(&Form_MeterOperate, SS_SHOWMAX);
}


//���������
void MeterOperateLoad()
{
  int count;
  
  use(METE_RECD_GW);
  count = libsumr();
  
  MeterCount = 0;
  memset(ListBox_MeterItems, 0, sizeof(ListBox_MeterItems));
  ShowFormProgress(count, MeterOperateDoLoad);  
}

//��������ش���
void MeterOperateDoLoad(HWND Obj,PMsg Msg)
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
    
    libset(r, FD_METERADDR);
    memset(temp, 0, sizeof(temp));
    libread(temp);
    if (temp[0] == '\0')
    {
    	ProgressStep();
    	continue;
    }
       
    strcpy(ListBox_MeterItems[MeterCount], temp);
    CurrMPAttrib.MPLocate[MeterCount] = r;
    MeterCount++;         
    ProgressStep();
  }
  
  CurrMPAttrib.MPNum = MeterCount;
  
  CloseFormProgress();
  use("");
}


//����б���Ϣ�����¼�
void ListBox_MeterOperate_OnMsg(HWND Obj, PMsg Msg)
{
  if (Msg->Msg == WM_CHAR)
  {
    switch (Msg->WParam)
    {
      case VK_UP:
        if (ListBox_MeterOperate.index == 0)
        {
          ListBox_MeterOperate.index = ListBox_MeterOperate.count - 1;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_DOWN:
        if (ListBox_MeterOperate.index == ListBox_MeterOperate.count - 1)
        {
          ListBox_MeterOperate.index = 0;
          Msg->Result = MSG_OK;
          Refresh(Obj);
        }
        break; 
      case VK_1:
        MeterOperateGoto(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_2:
        ReadMPData(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_3:
        ReadMeterData(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_4:
        ShowReadDataInfo(NULL);
        Msg->Result = MSG_OK;
        break; 
      case VK_5:  
      case VK_0:                
        MeterOperateHelp(NULL);
        Msg->Result = MSG_OK;
        break;         
    }
  }
}


//����б����¼�
void MeterOperateView(HWND Sender)
{

  if (ListBox_MeterOperate.count < 1)
  {
    return;
  }
    
 // MeterRecordInfoLoad(NULL);  
}


//��ת���λ���
void MeterOperateGoto(HWND Sender)
{
  unsigned char meterAddr[MAX_ASSETNO_DIGITS] = {};
  unsigned char meter[MAX_ASSETNO_DIGITS] = {};
  int i;

  if (InputBoxPro("��ת��", meterAddr, sizeof(meterAddr)) == ID_CANCEL)
  {
    return;
  }

  GetAddress(meterAddr, 2);
  
  for (i = 0; i < MeterCount; i++)
  {
    memset(meter, 0, sizeof(meter));
    strncpy(meter, ListBox_MeterItems[i], 12);
    if (strcmp(meterAddr, meter) == 0)
    {
      ListBox_MeterOperate.index = i;
      Refresh(&ListBox_MeterOperate);
      break;
    }
  }
} 
 

//��������������
void ReadMPData(HWND Sender)
{  
  Window Form_ReadData;
	TMenu Menu_ReadData;
	TMenuItem ReadDataItems[] = {
		{NULL, NULL, "1.ʵʱ�����й�"},
		{NULL, NULL, "2.�ն��������й�"},
		//{NULL, NULL, "3.͸�������й�"},
	};
	
	CreateWindow(&Form_ReadData, "", 0,0,160,160);
	sprintf(Form_ReadData.text, "���������ݳ���");
	
	CreateTMenu(&Menu_ReadData, &ReadDataItems,
							sizeof(ReadDataItems)/sizeof(TMenuItem), 10,10,140,110,FONT_CH_12|FONT_EN_12);
	Menu_ReadData.OnClick = ReadMPDataDo;
	
	AddToWindow(&Form_ReadData, &Menu_ReadData);
  	
	ShowWindow(&Form_ReadData, SS_SHOWMAX);	 
}


//��������������
void ReadMPDataDo(HWND Sender)
{

  int r, count;
  char temp[MAX_ASSETNO_DIGITS] = {0};
  
  if (ListBox_MeterOperate.count < 1)
  {
    sprintf(temp, "1");
  }
  else
  {
  	r = CurrMPAttrib.MPLocate[ListBox_MeterOperate.index];
	  use(METE_RECD_GW);
	  libset(r, FD_MPNO);
	  memset(temp, 0, sizeof(temp));
	  libread(temp);
	  use(""); 
  }
  

	if (InputBox("", "������������", temp, 4) == ID_CANCEL)
	{
		return;
	}
		
	CurrMPAttrib.Flag = StrToInt(temp, DEC);
	if (CurrMPAttrib.Flag < 0 || CurrMPAttrib.Flag > MAX_MPNO)
	{
		return;
	}

	memset(GlobalParam, 0, sizeof(GlobalParam)); 
	memset(GlobalParamItem, 0, sizeof(GlobalParamItem));
	switch(((TMenu *)Sender)->index)
	{
		case 0:
		{		
			GlobalParam->AFN = AFN_698RTDATA;
			GlobalParam->Fn = FN_129;
			break;
		}
		case 1:
		{
			GlobalParam->AFN = AFN_698HISDATA;
			GlobalParam->Fn = FN_161;
		  ReadHisDataDate = GetHisDataDate(ReadHisDataDate);
		  if (!ReadHisDataDate)
		  {
		  	return;
		  }
			break;
		}
		default:
			return;	  
	}
	
	if (!GetParamIDFromDBF(GlobalParam, 0, PARAMDBF_GW))
  {
  	return;
  }  		
  GetParamValueFromDBF(GlobalParam, GlobalParamItem, PARAMDBF_GW);
  
	uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150);	
    		      		  
  ShowFormProgress(1, ReadMPDataDoSend);

	uartinit(0, CommParam.Parity, CommParam.Port);
  if (CurrMPAttrib.Flag != 1)
  {
    MessageBox(ConstResStr[CurrMPAttrib.Flag], STR_CUE, MB_OK);
    return;
  }
    
  //CurrMPAttrib.Flag = ListBox_MeterOperate.index;  

	ShowTermParam(GlobalParam, GlobalParamItem);  
}

//�������������ݴ���
void ReadMPDataDoSend(HWND Obj, PMsg Msg)
{
  U16 slen = 0;
  int rlen = 0;
  unsigned char sendbuff[MAXSENDBYTES] = {0};
  unsigned char recvbuff[MAXRECVBYTES] = {0};
  Stru698Data Send698Data;
  
  memset(Send698Data.Saddr, 0, sizeof(Send698Data.Saddr));
  GetAddr(Send698Data.Saddr, CurrMPAttrib.Term);
  
  Send698Data.Ccode = C_698READ;
  Send698Data.AFN = GlobalParam->AFN;
  Send698Data.SEQ = INI_698SEQ;
  
  Send698Data.Fn = GlobalParam->Fn;
  Send698Data.Ulen = 0;
    
  Send698Data.Pn = CurrMPAttrib.Flag;
  
  if (Send698Data.AFN == AFN_698HISDATA)
  {
  	Send698Data.Userbuf[Send698Data.Ulen++] = (unsigned char)ReadHisDataDate;
    Send698Data.Userbuf[Send698Data.Ulen++] = (unsigned char)(ReadHisDataDate >> 8);
    Send698Data.Userbuf[Send698Data.Ulen++] = (unsigned char)(ReadHisDataDate >> 16); 
  }
   
  memset(sendbuff, 0, sizeof(sendbuff));
  slen = Build698SendBuff(sendbuff, &Send698Data);
    
  SendData(sendbuff, slen);
  rlen = RecvChar(recvbuff, sizeof(recvbuff), 2000);
  Beep(100);
    
  CurrMPAttrib.Flag = Parse698Buff(recvbuff, rlen);   
  ProgressStep();    
  CloseFormProgress(); 
}


//͸���������
void ReadMeterData(HWND Sender)
{

  if (ListBox_MeterOperate.count < 1)
  {
    return;
  }
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150);  
   
  CurrMPAttrib.Flag = ListBox_MeterOperate.index;
  
  ShowFormProgress(1, ReadMeterDataDo);

  uartinit(0, CommParam.Parity, CommParam.Port);
  Delay(50);

  if (CurrMPAttrib.Flag != 1)
  {
    MessageBox(ConstResStr[CurrMPAttrib.Flag], STR_CUE, MB_OK);
    return;
  }
    
  CurrMPAttrib.Flag = ListBox_MeterOperate.index;  

  ShowReadDataResult(0);  
}

//͸��������ݴ���
void ReadMeterDataDo(HWND Obj, PMsg Msg)
{

  int r;
  U16 slen = 0;
  int rlen = 0;
  unsigned char sendbuff[MINSENDBYTES] = {0};
  unsigned char recvbuff[MAXRECVBYTES] = {0};
  Stru698Data Send698Data;
  Stru645Data Send645Data;
  
  Send645Data.protype = PROTO_07;
  memcpy(Send645Data.address, ListBox_MeterOperate.text, 12);
  
  memset(Send698Data.Saddr, 0, sizeof(Send698Data.Saddr));
  GetAddr(Send698Data.Saddr, CurrMPAttrib.Term);
  
  Send698Data.Ccode = C_698REBOOT;
  Send698Data.AFN = AFN_698FWDCMD;
  Send698Data.SEQ = INI_698SEQ;
  Send698Data.Pn = 0;
  Send698Data.Fn = 1;

  use(METE_DATA_RCV_GW);

  r = CurrMPAttrib.MPLocate[CurrMPAttrib.Flag];
  memset(sendbuff, 0, sizeof(sendbuff));
  slen = BuildAFN10F01Buff(sendbuff, r);
  
  libset(r, FD_RTDATA);
  libwrit(""); 
    
  use("");
  
  Send698Data.Ulen = 0;
  memset(Send698Data.Userbuf, 0, sizeof(Send698Data.Userbuf));
  memcpy(Send698Data.Userbuf, sendbuff, slen);
  Send698Data.Ulen += slen;
     
  memset(sendbuff, 0, sizeof(sendbuff));
  slen = Build698SendBuff(sendbuff, &Send698Data);
    
  SendData(sendbuff, slen);
  rlen = RecvChar(recvbuff, sizeof(recvbuff), 2000);
  Beep(100);
        
  CurrMPAttrib.Flag = Parse698Buff(recvbuff, rlen);  
  ProgressStep();     
  CloseFormProgress();  
}


//�������ݽ����ʾ
void ShowReadDataInfo(HWND Sender)
{
  int r;
  unsigned char temp[64] = {0};
	
	if (ListBox_MeterOperate.count < 1)
	{
		return;
	}
	
	if (fileexits(METE_DATA_RCV_GW) == FSNULL)
	{
		return;
	}
	
	GetMPLocate(&CurrMPAttrib, METE_DATA_RCV_GW);
	
	if (CurrMPAttrib.MPNum < 1)
	{
		return;
	}
	
  Window Form_ReadDataResult;

  CreateText(&ReadDataText_MPNo, "", 3, 3, 154, 15);
  CreateText(&ReadDataText_Port, "", 3, 21, 154, 15); 
  CreateText(&ReadDataText_MeteAddr, "", 3, 39, 154, 15);  
  CreateText(&ReadDataText_RT, "", 3, 60, 154, 15);
  CreateText(&ReadDataText_Data, "", 3, 88, 154, 15);

  CreateWindow(&Form_ReadDataResult, STR_RES_READ, 0, 0, 160, 160);
 	
 	CurrMPAttrib.Flag = ListBox_MeterOperate.index;
  if (CurrMPAttrib.Flag >= CurrMPAttrib.MPNum)
  {
  	CurrMPAttrib.Flag = 0;
  }
  
  sprintf(Form_ReadDataResult.text, "�������[%d/%d]", CurrMPAttrib.Flag + 1, CurrMPAttrib.MPNum);

  use(METE_DATA_RCV_GW);
  r = CurrMPAttrib.MPLocate[CurrMPAttrib.Flag];

  libset(r, FD_MPNO);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_MPNo.text, "������ţ�%s", temp);
  
  libset(r, FD_PORT);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_Port.text, "�� �ڣ�%s", temp);   
  
  libset(r, FD_METERADDR);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_MeteAddr.text, "���ַ��%s", temp);  

  libset(r, FD_RTDATA);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_RT.text, "ʵʱ���ݣ�\n%s", temp);
  
  libset(r, FD_DAYDATA);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_Data.text, "�ն��᣺\n%s\n", temp);
  
  use("");

  AddToWindow(&Form_ReadDataResult, &ReadDataText_MPNo);
  AddToWindow(&Form_ReadDataResult, &ReadDataText_Port);  
  AddToWindow(&Form_ReadDataResult, &ReadDataText_MeteAddr);  
  
  AddToWindow(&Form_ReadDataResult, &ReadDataText_RT);  
  AddToWindow(&Form_ReadDataResult, &ReadDataText_Data);  
    
  ShowWindow(&Form_ReadDataResult, SS_SHOWMAX);
}

//�������ݽ����ʾ
void ShowReadDataResult(unsigned char fn)
{
  int r;
  unsigned char temp[64] = {0};
	
	if (CurrMPAttrib.MPNum < 1)
	{
		return;
	}
	
  if (fn == 1)      //�鿴�ն�������
  {
  	ReadDataType = FD_DAYDATA;
  }
  else
  {
  	ReadDataType = FD_RTDATA;
  }
  
  Window Form_ReadDataResult;

  CreateText(&ReadDataText_MPNo, "", 3, 3, 154, 15);
  CreateText(&ReadDataText_Port, "", 3, 21, 154, 15); 
  CreateText(&ReadDataText_MeteAddr, "", 3, 39, 154, 15);  
  CreateText(&ReadDataText_RT, "", 3, 60, 154, 15);
  CreateText(&ReadDataText_Data, "", 3, 88, 154, 15);
  CreateText(&ReadDataText_Res, "", 3, 116, 154, 15); 

  CreateWindow(&Form_ReadDataResult, STR_RES_READ, 0, 0, 160, 160);
  Form_ReadDataResult.OnKeyPress = Form_ReadDataResult_OnKeyPress;
 
  if (CurrMPAttrib.Flag >= CurrMPAttrib.MPNum)
  {
  	CurrMPAttrib.Flag = 0;
  }
  
  sprintf(Form_ReadDataResult.text, "�������[%d/%d]", CurrMPAttrib.Flag + 1, CurrMPAttrib.MPNum);

  use(METE_DATA_RCV_GW);
  r = CurrMPAttrib.MPLocate[CurrMPAttrib.Flag];

  libset(r, FD_MPNO);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_MPNo.text, "������ţ�%s", temp);
  
  libset(r, FD_PORT);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_Port.text, "�� �ڣ�%s", temp);   
  
  libset(r, FD_METERADDR);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_MeteAddr.text, "���ַ��%s", temp);  

  libset(r, FD_RESERVE);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_RT.text, "����ʱ�䣺\n%s", temp);
  
  libset(r, ReadDataType);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_Data.text, "�����й���\n%s", temp);

  sprintf(ReadDataText_Res.text, "%s", "���������");
  if (strstr(temp, ".") == NULL)
  {
  	strcat(ReadDataText_Res.text, STR_FAIL);
  }
  else
  {
  	if (temp[0] == 'e')
  	{
  		strcat(ReadDataText_Res.text, STR_NULL);
  	}
  	else
  	{
  		strcat(ReadDataText_Res.text, STR_OK);
  	}
  }
  use("");

  AddToWindow(&Form_ReadDataResult, &ReadDataText_MPNo);
  AddToWindow(&Form_ReadDataResult, &ReadDataText_Port);  
  AddToWindow(&Form_ReadDataResult, &ReadDataText_MeteAddr);  
  
  AddToWindow(&Form_ReadDataResult, &ReadDataText_RT);  
  AddToWindow(&Form_ReadDataResult, &ReadDataText_Data);  
  AddToWindow(&Form_ReadDataResult, &ReadDataText_Res);  
    
  ShowWindow(&Form_ReadDataResult, SS_SHOWMAX);
}

void Form_ReadDataResult_OnKeyPress(HWND Obj, PMsg Msg)
{

  if (Msg->Msg == WM_CHAR)
  {
    switch (Msg->WParam)
    {
      case VK_LEFT: 
      case VK_UP:
      {  
        if (CurrMPAttrib.Flag == 0)
        {
          CurrMPAttrib.Flag = CurrMPAttrib.MPNum - 1;
        }
        else
        {
          CurrMPAttrib.Flag--;
        }       
        Msg->Result = MSG_OK;
        break;
      } 
      case VK_RIGHT:
      case VK_DOWN:
      {  
        if (CurrMPAttrib.Flag == CurrMPAttrib.MPNum - 1)
        {
          CurrMPAttrib.Flag = 0;
        }
        else
        {
          CurrMPAttrib.Flag++;
        }       
        Msg->Result = MSG_OK;
        break; 
      }
      default:
      	break;
    }   
    if (Msg->Result == MSG_OK)
    {
      Form_ReadDataResult_Refresh(Obj);
      Refresh(Obj);
    }                 
  }   
}

//���ݳ������������ʾ
void Form_ReadDataResult_Refresh(HWND Obj)
{
  unsigned char temp[64] = {0};
  int r;    

  sprintf(((Window*)Obj)->text, "�������[%d/%d]", CurrMPAttrib.Flag + 1, CurrMPAttrib.MPNum); 

  use(METE_DATA_RCV_GW);
  r = CurrMPAttrib.MPLocate[CurrMPAttrib.Flag];
  
  libset(r, FD_MPNO);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_MPNo.text, "������ţ�%s", temp);
  
  libset(r, FD_PORT);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_Port.text, "�� �ڣ�%s", temp);   
  
  libset(r, FD_METERADDR);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_MeteAddr.text, "���ַ��%s", temp);  

  libset(r, FD_RESERVE);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_RT.text, "����ʱ�䣺\n%s", temp);
  
  libset(r, ReadDataType);
  memset(temp, 0, sizeof(temp));
  libread(temp);
  sprintf(ReadDataText_Data.text, "�����й���\n%s", temp);
  
  memset(ReadDataText_Res.text, 0, sizeof(ReadDataText_Res.text));
  sprintf(ReadDataText_Res.text, "%s", "���������");
  if (strstr(temp, ".") == NULL)
  {
  	strcat(ReadDataText_Res.text, STR_FAIL);
  }
  else
  {
  	if (temp[0] == 'e')
  	{
  		strcat(ReadDataText_Res.text, STR_NULL);
  	}
  	else
  	{
  		strcat(ReadDataText_Res.text, STR_OK);
  	}
  }

  use("");  
}


void MeterOperateHelp(HWND Sender)
{
  MessageBox("�����λ�ڵ���б�ʱ\nȷ�ϼ���������\n����1����ת\n����2������\n����3��͸��\n����4�����\n����5������", "����", MB_OK); 
}
#endif
