#ifndef	_FormMain_H
#define _FormMain_H

#include "jblib.h"
#include "SystemConf.h"
#include "ArchivesTerminal.h"
#include "Check485Terminal.h"
#include "BatchSendRecord.h"   
#include "TermOperate.h"
//#include "WirelessDebug.c"
#include "ShowData.h"
#include "Register.h"


void ShowForm_Main(void)
{
 
 	if (!Register())
 	{
 		return;
 	}
 
  Window Form_Main;
  GMenu GMenu_Desktop;
  TMenuItem pGMenu_DesktopItems[] = {
    {getSysIcon(4), SystemConf, "ϵͳ����"}, 
    {getSysIcon(2), ArchivesTerminal, "����¼��"}, 
    {getSysIcon(6), Check485Terminal, "485���"}, 
    {getSysIcon(6), BatchSendRecord, "��������"},    
    {getSysIcon(6), TermOperate, "�ն˵���"},
    //{getSysIcon(18), WirelessDebug, "���ߵ���"}, 
    {getSysIcon(5), ShowMsg, "���Ĳ鿴"}, 
  };

	DelFile(DEBEGINFOTXT, MAX_DEBGFILE_LEN);
	
  CreateGMenu(&GMenu_Desktop, pGMenu_DesktopItems,  
    sizeof(pGMenu_DesktopItems) / sizeof(TMenuItem), 0, 3, 160, 130);

  CreateWindow(&Form_Main, MAINFORMNAME, 0, 0, 160, 160);  
  AddToWindow(&Form_Main, &GMenu_Desktop);

  ShowWindow(&Form_Main, SS_SHOWMAX);

  if (fileexits(METE_RECD_RCV_GW) != FSNULL)
  {
    filedelete(METE_RECD_RCV_GW);
  }   
  
  if (fileexits(METE_DATA_RCV_GW) != FSNULL)
  {
  	filedelete(METE_DATA_RCV_GW);
  }
  
  if (fileexits(TEMPPARAMTXT) != FSNULL)
  {
  	filedelete(TEMPPARAMTXT);
  }
}


#endif
