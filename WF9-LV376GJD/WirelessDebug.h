#ifndef _WirelessDebug_H
#define _WirelessDebug_H

#include "jblib.h"
#include "Common.h"
#include "Global.h"
#include "FormSelecte.h"
#include "FormProgress.h"
#include "DataConvert.h"
#include "InputBoxPro.h"
#include "ShowResult.h"
#include "DBFOperate.h"

#define WLSINFOTXT ("wlsinfo.txt")      //������Ϣ��¼��ʱ�ļ�

#define MINNODE 1                       //�Զ��жϰ�װλ�ã���С�ڵ���
#define MINRSSI (-70)                   //�Զ��жϰ�װλ�ã�����ź�ǿ��
#define MAXROUTELEN 6                   //�Զ��жϰ�װλ�ã����·������
#define MAXWLSBYTES 255                 //΢�������߽������֡����
#define MINWLSBYTES 75                 	//΢�������߽�����С֡����
#define MAXWLSDELAYTIME 64              //΢�������߽������ַ����ȴ�����
#define MAXRECVCOUNT 20
#define MINRECVCOUNT 11

//��������֡������ʾ��
const char *ConstMACStr[] = {"��Ӧ��", "����֡", "ȷ��֡", "����֡", "ά��֡", "δ֪֡"};  

//MAC��֡����
enum
{
  MAC_DATA=1,
  MAC_OK,
  MAC_CMD,
  MAC_INFO,
  MAC_UNK,
};

//�������������
enum
{
  NWK_ROUTE_REQ=1,
  NWK_ROUTE_RPY,
  NWK_STATUS,
  NWK_LEAVE,
  NWK_ROUTE_RECD,
  NWK_PING_REQ=0x40,
  NWK_PING_RPY,
  NWK_BRDCAST=0x80,
};

//MAC��֡������
typedef struct _MacCtrl
{
  U16 type:3;
  U16 sec:1;
  U16 pend:1;
  U16 ack:1;
  U16 :4;
  U16 dstMode:2;
  U16 :2;
  U16 srcMode:2;
}MacCtrl;

//�����֡������
typedef struct _NWKCtrl
{
  U16 type:2;
  U16 version:4;
  U16 disRout:2;
  U16 multi:1;
  U16 :1;
  U16 srcRout:1;
  U16 dstIEEE:1;
  U16 srcIEEE:1;
  U16 :3; 
}NWKCtrl;

//��������ݽṹ
typedef struct _NWKData
{
  NWKCtrl sNWKCtrl;
  U16 dstAddr;
  U16 srcAddr;
  U8 radius;  
  U8 seq;
  U8 dstIEEE[16];
  U8 srcIEEE[16];
  U8 multi;
  U8 count;
  U8 index;
  U8 sList[64];
  U8 type;
}NWKData;

//MAC�����ݽṹ
typedef struct _MacData
{
  MacCtrl sMacCtrl;
  U8 seq;
  U16 PANID;
  U8 dstAddr[16];
  U8 srcAddr[16];  
  short rssi;
  NWKData sNWKData;
}MacData;

//����״̬�ֽṹ��MAC��ά����Ϣ֡��
typedef struct _RunStatus
{
	U8 flag;
  U8 seq;
  U16 PANID;
  U16 nAddr;
  U8 nDeep;
  U8 nCost;
  short fRSSI;
  U8 vfMac[10];
  U8 vcMac[10];
  U8 vRoute[20];
  short rssi;
}RunStatus;

U8 WlsSEQ;                              //ͨ�����к�(SI1000)
U8 WlsModuleType;                       //����ģ�����ͣ�0����CC1100��1����SI1000��
U8 WirelessDevType;                     //�����豸���ͣ�������ģ�顢�ɼ�������
char WirelessDevAddr[13];               //�����豸��ַ�����߲ɼ�������
char WirelessCollAddr[13];              //���߲ɼ�����ַ���������߳���


/******************************************************************************
* @fn  convertRssiByte
*
* @brief
*      Converts the RSSI (received signal strength indicator) value,
*      given as a 2's complement number, to dBm value. This requires that a
*      proper RSSI offset value is specified in global variable perRssiOffset
*      before use.
*
* Parameters:
*
* @param  BYTE rssiComp
*                   The RSSI value received from the radio as a 2's complement
*                   number
*
* @return INT16
*           The RSSI value in dBm
*
*Data rate [kBaud] RSSI_offset [dB], 315 MHz RSSI_offset [dB], 433 MHz RSSI_offset [dB], 868 MHz
*   1.2      74      75        73
*  38.4      73      74        73
*   250      74      73        77
******************************************************************************/ 
//#ifdef  FREQ_471MHZ
 #define RSSI_OFFSET  74
//#elif FREQ_922MHZ
// #define RSSI_OFFSET  73
//#else
// #define RSSI_OFFSET  73
//#endif 
short macConvertRssiByte(unsigned char rssiComp)
{
		if (WlsModuleType)
		{
			return -rssiComp;
		}
    // Convert RSSI value from 2's complement to decimal value.
    unsigned short rssiDec = (unsigned short) rssiComp;     
    // Convert to absolute value (RSSI value from radio has resolution of
    // 0.5 dBm) and subtract the radio's appropriate RSSI offset.
    if(rssiDec < 128)
    {
        return (rssiDec/2) - RSSI_OFFSET; //perRssiOffset;
    }
    else
    {
        return ((rssiDec - 256)/2) - RSSI_OFFSET; //perRssiOffset;
    }
    
}


//��ʾ������
int ShowWlsParam(Stru_Param *param, Stru_ParamItem *pItem)
{
  int i = 0;
  char show[NUM_800] = {0};
  char temp[NUM_100] = {0};
  
  if (param->Param_ID != pItem->Param_ID)
  {
    return;
  }
  
  memset(show, 0, sizeof(show));
  for (i = 0; i < param->Param_Num; i++)
  {
    strcat(show, (pItem+i)->Param_Item_Name);
    strcat(show, ":");
    if ((pItem+i)->Param_Item_Mask == 'e')
    {
      memset(temp, 0, sizeof(temp));
      FormatEnumData(pItem+i, temp);
      strcat(show, temp);
    }
    else
    {
      strcat(show, (pItem+i)->Param_Value);
    }
    strcat(show, "\r\n");   
  }
  
  if (fileexits(WLSINFOTXT) != FSNULL)
  {
    filedelete(WLSINFOTXT);
  }
  
  WritDubugInfo(WLSINFOTXT, show, strlen(show), 0xff);
  
  ShowData(STR_RES_READ, WLSINFOTXT);
  
}


//��ȡ�����豸��ַ
char *InputWlsAddr(const char *sAddr, char *address, unsigned char type)
{
  if (!type)
  {
    if (InputBox("�ɼ��������ַ", "������", address, 12) == ID_CANCEL)
    {
      return NULL;
    }
    FormatStr(address, 12, '0', 0); 
  }
  else
  {
    sprintf(address, "%s", sAddr);
  }
  
  return address;   
}


//���������豸��ַ
char *ParseWlsAddr(const unsigned char *sAddr, char *address, char revflag)
{
  
  int i = 0;
  
  for (i = 0; i < 6; i++)
  {
    if (revflag)
    {
      sprintf(address + (2 * i), "%02x", sAddr[6 - i - 1]);
    }
    else
    {
      sprintf(address + (2 * i), "%02x", sAddr[i]);
    }
  }
  
  return address;   
}


//��ȡ�������ַ���
char* BufferToStr(U8 buf[],U8 len)
{
  int i=0;
  static char szBuffer[255];
  
  for (i=0;i<len;i++)
  {
    sprintf(szBuffer+3*i,"%02x ",buf[i]);
  }
  
  return szBuffer;
}


//��ȡ�ַ�����ַ
char* GetMacAddr(U8 buf[], U8 len, U8 *address)
{
  int i=0;
  //static char szBuffer[64]={0};
  
  for (i=0;i<len;i++)
  {
    sprintf(address+2*i,"%02x",buf[len-1-i]);
  }
  
  return address;
}


/******************************************************************************/
/*                      ΢��������Э���ȡMAC�㷵��֡����                     */ 
/*  ����: ��ȡMAC�㷵��֡                                                     */
/*  ����: buff������������֡  len������������֡����                               */
/*  ����: �ɹ�����ʵ��֡���ȣ��쳣����0                                       */
/******************************************************************************/
/*
U8 GetMacBuff(U8 buff[], U16 len)
{
  unsigned short i = 0;
  unsigned char rlen = 0;
  unsigned char macbuf[MAXWLSBYTES] = {0};
  
  for(i = 0; i < len - 5; i++)
  {
    if (buff[i] == 0xAA && buff[i+1] == 0xAA && buff[i+2] == 0xAA)
    {
      if (buff[i+3] != 0xAA)
      {
        break;
      }
      continue;  
    } 
  }
  
  rlen = (unsigned char)(len - i - 3);

  if (rlen < 5)
  {
    return 0;
  }
  else 
  {
    memcpy(macbuf, buff + i + 3, rlen);
  }
  
  for (i = 0; i <= rlen - 3; i++)
  {
    if (macbuf[i] == 0x55 && macbuf[i+1] == 0x55 && macbuf[i+2] == 0x55)
    {
      break;
    }
  }
    
  if (i > rlen - 3)
  {
    return 0;   
  }
  
  rlen = i;
  memcpy(buff, macbuf, i);
        
  return rlen;
}
*/
U8 *GetMacBuff(U8 buff[], U16 len)
{
  unsigned short i = 0;
  unsigned char rlen = 0;
  unsigned char macbuf[MAXWLSBYTES] = {0};

  for(i = 0; i <= len - 3; i++)
  {
    if (buff[i] == 0xAA && buff[i+1] == 0xAA && buff[i+2] == 0xAA)
    {
      rlen = i + 3;            
      continue;
    } 
    
    if (buff[i] == 0x55 && buff[i+1] == 0x55 && buff[i+2] == 0x55)
    {
    	if (rlen)
    	{
	    	break;
	    }
    }
  }
  
  if (!rlen)
  {
  	return 0;
  }
  
  if (i > len - 3)
  {
  	return 0;
  }
   
  memcpy(macbuf, buff + rlen, len - rlen);
  memcpy(buff, macbuf, len - rlen);
  memset(buff+len-rlen, 0, rlen);
  
  rlen = i - rlen;
           
  return rlen;
}

/******************************************************************************/
/*                      ΢��������Э�������֡��������                        */ 
/*  ����: ��������㷵��֡                                                    */
/*  ����: recvbuff������������֡  len������������֡����                           */
/*  ����: �ɹ�����֡���ͣ��쳣����-1                                          */
/******************************************************************************/
char ParseWlsNWKData(U8 recvbuff[], U16 len, NWKData *pNWKData)
{
  
  unsigned char index = 0;
  unsigned char result = 0;

  memcpy(&(pNWKData->sNWKCtrl), recvbuff, sizeof(NWKCtrl));
  index += 2;
    
  if (pNWKData->sNWKCtrl.type > 1)      //������֡������֡
  {
    return -1;
  }
  
  pNWKData->dstAddr = recvbuff[index] + recvbuff[index+1] * 0x100;
  index += 2;
  pNWKData->srcAddr = recvbuff[index] + recvbuff[index+1] * 0x100;
  index += 2;
  pNWKData->radius = recvbuff[index++];
  pNWKData->seq = recvbuff[index++];
  
  if (pNWKData->sNWKCtrl.dstIEEE)       //IEEEĿ�ĵ�ַ��
  {
    memcpy(pNWKData->dstIEEE, recvbuff+index, 8);
    index += 8;
  }
  
  if (pNWKData->sNWKCtrl.srcIEEE)       //IEEEԴ��ַ��
  {
    memcpy(pNWKData->srcIEEE, recvbuff+index, 8);
    index += 8;
  }
  
  if (pNWKData->sNWKCtrl.multi)         //��㴫��
  {
    index++;
  }
  
  if (pNWKData->sNWKCtrl.srcRout)       //Դ·�ɸ�����
  {
    pNWKData->count = recvbuff[index++];
    pNWKData->index = recvbuff[index++];
    memcpy(pNWKData->sList, recvbuff+index, (pNWKData->count + 1)*2);
    index += (pNWKData->count + 1)*2;
  }
    
  
  if (pNWKData->sNWKCtrl.type)          //���������
  {
    pNWKData->type = recvbuff[index++]; //��������ID
    if (pNWKData->type == NWK_BRDCAST)
    {
      if (recvbuff[index] & 0x01)
      {
        index += 2;
        pNWKData->count = recvbuff[index++];
        pNWKData->index = recvbuff[index++];
        if (WlsModuleType)							//SI1000
        {
        	index += 8;
        }
        memcpy(pNWKData->sList, recvbuff+index, (pNWKData->count + 1) * 2);
        index += (pNWKData->count + 1) * 2;       
      }
      else
      {
        index += 2;
        pNWKData->count = recvbuff[index++];
        pNWKData->index = recvbuff[index++];
      }     
    }
  }
  else              //����֡
  {
    pNWKData->type = 0;
  }
    
  result = (U8)(pNWKData->sNWKCtrl.type); 
    
  return result;
}


/******************************************************************************/
/*                           ΢��������Э��֡��������                         */ 
/*  ����: ��������֡                                                          */
/*  ����: recvbuff������������֡  len������������֡����                           */
/*  ����: �ɹ�����MAC��֡���ͱ�ţ��쳣����0                                  */
/******************************************************************************/
U8 ParseWlsBuff(U8 recvbuff[], U16 len, MacData *pMacData)
{
  
  unsigned char index = 0;
  unsigned char result = 0;
  
  result = GetMacBuff(recvbuff, len);
  
  if (!result)
  {
    return result;
  }
      
  memcpy(&(pMacData->sMacCtrl), recvbuff, sizeof(MacCtrl));  
  index += 2;
  
  pMacData->rssi = macConvertRssiByte(recvbuff[result - 2]);
    
  if (pMacData->sMacCtrl.type < MAC_INFO && pMacData->sMacCtrl.type > 0)
  {
    if (pMacData->sMacCtrl.type == MAC_OK)
    {
      return (U8)MAC_OK;
    }
  }
  else
  {
    return (U8)MAC_UNK;
  }
  
  pMacData->seq = recvbuff[index++];
  WlsSEQ = pMacData->seq + 1;
  pMacData->PANID = recvbuff[index] + recvbuff[index+1] * 0x100;
  index += 2;
  
  if (pMacData->sMacCtrl.dstMode == 2)
  {   
    memcpy(pMacData->dstAddr, recvbuff+index, 2);
    index += 2;
  }
  else if (pMacData->sMacCtrl.dstMode == 3)
  {   
    memcpy(pMacData->dstAddr, recvbuff+index, 8);
    index += 8;
  }
  
  if (pMacData->sMacCtrl.srcMode == 2)
  {   
    memcpy(pMacData->srcAddr, recvbuff+index, 2);
    index += 2;
  }
  else if (pMacData->sMacCtrl.srcMode == 3)
  {
    memcpy(pMacData->srcAddr, recvbuff+index, 8);
    index += 8;
  }
    
  if (pMacData->sMacCtrl.type == MAC_DATA)
  {
    char res = ParseWlsNWKData(recvbuff+index, result-index, &(pMacData->sNWKData));
    if (res < 0)
    {
      return (U8)MAC_UNK;
    }   
  }
    
  result = (U8)(pMacData->sMacCtrl.type); 
    
  return result;
}


//��������Ƶ��atָ��
char ReadChannel(void)
{
  
  char channel = 0;
  char buf[64] = {0}; 
  unsigned char len = 0;
  char *cbuf = "+channel:";
  int index = 0;

  sprintf(buf, "at+channel?");
  
  sbuf(buf, strlen(buf), 0);

  memset(buf, 0, sizeof(buf));     
  len = rbuf(buf, sizeof(buf) - 1, 50); 

  index = strlen(cbuf);
  if (len > index)
  {
    cbuf = strstr(buf, cbuf);
    index++;
    if (cbuf != NULL)
    {
      channel = *(cbuf+index+1);    		//SI1000
      if (channel < '0' || channel > '9')
      {
      	channel = *(cbuf+index);				//CC1100
      }
      else
      {
      	WlsModuleType = 1;							//SI1000
      }
    }
  }

  return channel; 
}


//��������Ƶ��atָ��
char SetChannel(char channel)
{
  
  char result = 0;
  char buf[64] = {0};
  int len = 0;
  
  if (channel > '9')
  {
    return 0;
  }
   
  sprintf(buf, "at+channel=%c", channel);
  
  sbuf(buf, strlen(buf), 0);

  memset(buf, 0, sizeof(buf));     
  len = rbuf(buf, sizeof(buf) - 1, 50); 
  
  if (strstr(buf, "OK") != NULL)
  {
    result = 1;
  }

  return result;
}


//��������Ƶ��
void ReadCommChannel(HWND Sender)
{

  char channel = 0;
  char showInfo[NUM_50] = {0};
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
  
  channel = ReadChannel();
  
  if (channel)
  {
    sprintf(showInfo, "��ǰƵ��: %c", channel);   
  }
  else
  {
    sprintf(showInfo, "������");    
  }
  
  MessageBox(showInfo, "��ѯ���", MB_OK);        
}


//��������Ƶ��
void SetCommChannel(HWND Sender)
{
  char value[NUM_8] = {0};
   
  if (InputBox("Ƶ����Χ[0,9]", "�����빤��Ƶ��", value, 1) == ID_CANCEL)
  {
    return;
  }
  
  if (value[0] == '\0')
  {
    value[0] = '0';
  }
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 

  if (SetChannel(value[0]))
  {
    MessageBox("���óɹ�!", STR_CUE, MB_OK);  
  }
  else
  {
    MessageBox("����ʧ��!", STR_CUE, MB_OK);  
  }
  
}


//��ѯ����״̬��
RunStatus *ReadRunStatus(const char *sAddr, RunStatus *sRunStatus)
{
  
  int i = 0;
  char address[13] = {0};
  U8 buf[MAXWLSBYTES] = {0};
  U8 setbuf[21] = {0x04, 0x0C, 0xA9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  								 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01,};
  U8 setbuff[24] = {0x04, 0x8C, 0xA9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  									0xFF, 0xFF, 0xFF, 0xAA, 0xBB, 0x00, 0x00, 0x01, 0x06,};
  U8 nLen = 0;
  
  sprintf(address, "%s", sAddr);
  FormatStr(address, 12, '0', 0); 
  StrToHexStr(address, 6, 1, 0, 0);
	    
  if (WlsModuleType)
  {
  	setbuff[2] += WlsSEQ;
  	WlsSEQ++;
  	memcpy(setbuff+5, address, 6);
  	memcpy(setbuff+17, address, 6);
  	
  	SendData(setbuff, sizeof(setbuff) - 1);
  }
  else
  {
	  memcpy(setbuf+14, address, 6);
	      
	  SendData(setbuf, sizeof(setbuf) - 1);
	}
	
  memset(buf, 0, sizeof(buf));
  nLen = RecvWlsData(buf, sizeof(buf) - 1, MAXWLSDELAYTIME);  
  Beep(100);
  WritDubugInfo(DEBEGINFOTXT, buf, nLen, 12);
      
  nLen = GetMacBuff(buf, nLen);
  	  
  if (nLen > 100)
  {   
    if (!(buf[0] & ((U8)MAC_INFO)))     //MAC��ά��֡
    {
      return NULL;
    }
    
    sRunStatus->flag = 0;
    sRunStatus->seq = buf[2];
    WlsSEQ = sRunStatus->seq + 1;
    memcpy(&(sRunStatus->PANID), buf+3, 2);
	  memcpy(&(sRunStatus->nAddr), buf+18, 2);
	  memcmp(sRunStatus->vcMac, buf+7, 8);
	    
    if (WlsModuleType)
    {	
    	if ((buf[20]) && sRunStatus->nAddr != 0xffff)
    	{
    		sRunStatus->flag = 1;
    	}    	    
	    memcpy(sRunStatus->vfMac, buf+41, 8);
	    sRunStatus->nDeep = buf[21];
	    sRunStatus->nCost = buf[22];
	    sRunStatus->fRSSI = macConvertRssiByte(buf[24]);
	    memcpy(sRunStatus->vRoute, buf + 25, 16);	    
    }
    else
    { 	    	    
    	if (sRunStatus->PANID != 0xffff && sRunStatus->nAddr != 0xffff)
    	{
    		sRunStatus->flag = 1;
    	}
	    memcpy(sRunStatus->vfMac, buf+32, 8);
	    sRunStatus->nDeep = buf[51];
	    sRunStatus->nCost = buf[52];
	    sRunStatus->fRSSI = macConvertRssiByte(buf[54]);
	    memcpy(sRunStatus->vRoute, buf + 57, 16);	    
	  }
	  
	  sRunStatus->rssi = macConvertRssiByte(buf[nLen-2]);
	    
    return sRunStatus;
  }
  
  return NULL;  
} 


//����������������
char AssociationRequest(MacData *sMacData)
{
  int i = 0;
  int count = 0;
  char result = 0;
  U8 setbuf[22] = {0x03, 0xC8, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xAA, 
  								 0xBB, 0x98, 0x99, 0x99, 0x99, 0x99, 0x99, 0x01, 
  								 0x8C, 0xFF, 0xFF, 0xFF, 0xFF}; 
  U8 arrbuf[MAXRECVCOUNT][MAXWLSBYTES] = {0};
  U8 arrbuflen[MAXRECVCOUNT] = {0};
  U8 nLen = 0;
    
  SendData(setbuf, sizeof(setbuf) - 1); 
  
  for (i = 0; i < MAXRECVCOUNT; i++)
  {
    memset(arrbuf[count], 0, sizeof(arrbuf[count]));
    nLen = RecvWlsData(arrbuf[count], sizeof(arrbuf[count]) - 1, MAXWLSDELAYTIME);
    
    if (nLen < 10)
    {
    	break;
    }

    arrbuflen[count] = nLen; 
    count++;      
  }
  
  Beep(100);
  
  for (i = 0; i < count; i++)
  {  
  	nLen = arrbuflen[i];
  	WritDubugInfo(DEBEGINFOTXT, arrbuf[i], nLen, 12);
  	
  	result = ParseWlsBuff(arrbuf[i], nLen, sMacData);
    if (result == MAC_DATA)
    {
      if ((sMacData->sNWKData.type == NWK_BRDCAST))
      {
        if (0 == sMacData->sNWKData.count)
        {
          result = 1;
          break;
        }     
      }
    }
    
    if (nLen > MINWLSBYTES)
    {
	  	result = ParseWlsBuff(arrbuf[i], nLen, sMacData);
	    if (result == MAC_DATA)
	    {
	      if ((sMacData->sNWKData.type == NWK_BRDCAST))
	      {
	        if (0 == sMacData->sNWKData.count)
	        {
	          result = 1;
	          break;
	        }     
	      }
	    }    	
    }
    
    if (result == MAC_OK && nLen > MINWLSBYTES / 2)
    {
	  	result = ParseWlsBuff(arrbuf[i], nLen, sMacData);
	    if (result == MAC_DATA)
	    {
	      if ((sMacData->sNWKData.type == NWK_BRDCAST))
	      {
	        if (0 == sMacData->sNWKData.count)
	        {
	          result = 1;
	          break;
	        }     
	      }
	    }    	
    }
    
  }
  
  return result;    
}


//����·����������
char RouteRequest(MacData *sMacData)
{
  int i = 0;
  int count = 0;
  char flag = 0;  
  char result = 0;
 // char address[13] = {0};
  U8 setbuf[53] = {0x21, 0x88, 0x34, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 
  								 0xFF, 0x01, 0x14, 0x00, 0x00, 0xFF, 0xFF, 0x01, 
  								 0x19, 0xAA, 0xBB, 0x98, 0x99, 0x99, 0x99, 0x99, 
  								 0x99, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x01, 
  								 0x2D, 0x2B, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 
  								 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x06, 0x98, 0x99, 
  								 0x99, 0x99, 0x99, 0x99};
    
  U8 setbuff[62] ={0x21, 0xC8, 0xCD, 0xFF, 0xFF, 0x00, 0x00, 0x98, 
  								 0x99, 0x99, 0x99, 0x99, 0x99, 0xAA, 0xBB, 0x05, 
  								 0x14, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x88, 0x98, 
  								 0x99, 0x99, 0x99, 0x99, 0x99, 0xAA, 0xBB, 0x01, 
  								 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x2F, 0xA0, 
  								 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  								 0xFF, 0xFF, 0xFF, 0x02, 0x00, 0x00, 0x06, 0x98, 
  								 0x99, 0x99, 0x99, 0x99, 0x99};
  
  U8 arrbuf[MAXRECVCOUNT][MAXWLSBYTES] = {0};
  U8 arrbuflen[MAXRECVCOUNT] = {0};
  U8 nLen = 0;
  
  if (WlsModuleType)
  {
  	setbuff[2] = sMacData->seq + 1;
	  setbuff[3] = (unsigned char)(sMacData->PANID);
	  setbuff[4] = (unsigned char)(sMacData->PANID >> 8);
	  setbuff[22] = sMacData->sNWKData.seq + 1;  
	  SendData(setbuff, sizeof(setbuff) - 1); 
  }
  else
  {  
	  setbuf[2] = sMacData->seq + 1;
	  setbuf[3] = (unsigned char)(sMacData->PANID);
	  setbuf[4] = (unsigned char)(sMacData->PANID >> 8);
	  setbuf[16] = sMacData->sNWKData.seq + 1;   
	  SendData(setbuf, sizeof(setbuf) - 1); 
	}
	
	for (i = 0; i < MAXRECVCOUNT; i++)
  {
    memset(arrbuf[count], 0, sizeof(arrbuf[count]));
    nLen = RecvWlsData(arrbuf[count], sizeof(arrbuf[count]) - 1, MAXWLSDELAYTIME);
    //Beep(100);
        
    if (nLen < 12)  //���˵�ȷ��֡
    {
    	flag++; 
    	if (flag > 1)
    	{
    		break;
    	}
    	continue;
    }
    arrbuflen[count] = nLen; 
    flag = 0;
    count++;      
  }
  
  Beep(100);
  
  for (i = 0; i < count; i++)
  {  
  	nLen = arrbuflen[i];
  	WritDubugInfo(DEBEGINFOTXT, arrbuf[i], nLen, 12);
  	
  	result = ParseWlsBuff(arrbuf[i], nLen, sMacData);
    if (result == MAC_DATA)
    {   	
      if (sMacData->sNWKData.type == NWK_LEAVE)
      {
        result = 1;        
        break;
      }     	        	
    }
    
    if (nLen > MINWLSBYTES)
    {
	  	result = ParseWlsBuff(arrbuf[i], nLen, sMacData);
	    if (result == MAC_DATA)
	    {
	      if ((sMacData->sNWKData.type == NWK_LEAVE))
	      {
	        if (0 == sMacData->sNWKData.count)
	        {
	          result = 1;
	          break;
	        }     
	      }
	    }    	
    }
    
    if (result == MAC_OK && nLen > MINWLSBYTES)
    {
	  	result = ParseWlsBuff(arrbuf[i], nLen, sMacData);
	    if (result == MAC_DATA)
	    {
	      if ((sMacData->sNWKData.type == NWK_LEAVE))
	      {
	        if (0 == sMacData->sNWKData.count)
	        {
	          result = 1;
	          break;
	        }     
	      }
	    }    	
    }
        
  }
    
  return result;
}
 

//�Զ���ȡ����Ƶ��������������
void AutoReadTermChannelDoSend(HWND Obj, PMsg Msg)
{
  int i = 0;
  char channel = 0;
  char oldchannel = 0;
  char showInfo[128] = {0};
  char flag = 0;
  MacData rMacData;
  MacData oldMacData;
        
  channel = ReadChannel();
  if (!channel)
  {   
    MessageBox("��ѯʧ��!", STR_CUE, MB_OK);
    CloseFormProgress();  
    return;   
  }

  oldchannel = channel;  
  
  memset((unsigned char*)(&rMacData), 0, sizeof(rMacData));
  memset((unsigned char*)(&oldMacData), 0, sizeof(oldMacData));
  
  for(i = 0; i < ProgressBar_Proc.max; i++)
  {        
    if (AssociationRequest(&rMacData))
    {     
      if (!flag)
      {
        flag = 1;
        oldchannel = channel;
        oldMacData.rssi = rMacData.rssi;
        oldMacData.PANID = rMacData.PANID;
      }
      else
      {
        if (oldMacData.rssi < rMacData.rssi)
        {
          oldchannel = channel;
          oldMacData.rssi = rMacData.rssi;
          oldMacData.PANID = rMacData.PANID;
        }
      }
      
    }  
   
    channel++;
    if(channel > '9')
    {
      channel = '0';
    }
    
    if (!SetChannel(channel))
    {
      Delay(50);
      SetChannel(channel);
    }
		             
    ProgressStep();       
  }
  
  channel = oldchannel;
  if (!SetChannel(channel))
  {
    Delay(50);
    SetChannel(channel);
  }
  	
  if (!flag)
  {
    MessageBox("��ѯʧ��!", STR_CUE, MB_OK);
  }
  else
  {   
    sprintf(showInfo, "��ǰƵ��: %c\n����ID��%04x(H) %05d(D)\n�ź�ǿ�ȣ�%ddBm\n", 
    				channel, oldMacData.PANID, oldMacData.PANID, oldMacData.rssi);   
    MessageBox(showInfo, STR_CUE, MB_OK);  
  }
    
  CloseFormProgress();    
} 


//�Զ���ȡ����Ƶ���������߲ɼ��������
void AutoReadChannelDoSend(HWND Obj, PMsg Msg)
{
  int i = 0;
  char address[13] = {0};
  char channel = 0;
  char oldchannel = 0;
  char showInfo[NUM_50] = {0};
  RunStatus sRunStatus;
      
  channel = ReadChannel();
  if (!channel)
  {   
    MessageBox("��ѯʧ��!", STR_CUE, MB_OK);
    CloseFormProgress();  
    return;   
  }

  oldchannel = channel;   
  memcpy(address, WirelessDevAddr, 12);
  memset((unsigned char *)(&sRunStatus), 0, sizeof(sRunStatus));
   
  for(i = 0; i < ProgressBar_Proc.max; i++)
  {       
    
    if (ReadRunStatus(address, &sRunStatus) != NULL)
    {
      break;
    }
    else
    {     
      channel++;
      if(channel > '9')
      {
        channel = '0';
      }
      
      if (!SetChannel(channel))
      {
        Delay(50);
        SetChannel(channel);
      }

      ProgressStep();
    } 
     
  }
  
  if (i == ProgressBar_Proc.max)
  {
    MessageBox("��ѯʧ��!", STR_CUE, MB_OK);
    channel = oldchannel;
    SetChannel(channel);
  }
  else
  {
    sprintf(showInfo, "��ǰƵ��: %c", channel);   
    MessageBox(showInfo, STR_CUE, MB_OK);  
  }
    
  CloseFormProgress();  
  
} 


//�Զ���ȡ����Ƶ��
void AutoReadChannel(unsigned char type)
{   
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
  
  if (type)
  {	
	 	ShowFormProgress(12, AutoReadChannelDoSend);       
  }
  else
  { 	
    ShowFormProgress(11, AutoReadTermChannelDoSend);    
  }

}


void CheckTermModuleStatusDoSend(HWND Obj, PMsg Msg)
{	
	
	char flag = 0;
	MacData rMacData; 
	
	memset((unsigned char*)(&rMacData), 0, sizeof(rMacData));
	
	if (AssociationRequest(&rMacData))
  {
  	flag++;
  	ProgressStep();
  	
  	if (RouteRequest(&rMacData))
  	{
  		flag++;
  		ProgressStep();
  	}
	}
	
	if (flag < 1)
	{
		MessageBox("����ģ�鹤���쳣!", STR_CUE, MB_OK);		
	}
	else if (flag == 1)
	{
		MessageBox("����ģ�鹤������!\n������·�ɹ����쳣!", STR_CUE, MB_OK);		
	}
	else
	{
		MessageBox("����ģ�鹤������!\n������·�ɹ�������!", STR_CUE, MB_OK);		
	}
	
	CloseFormProgress();		
}

//����ն�����ģ�鹤��״̬
void CheckTermModuleStatus(void)
{   
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
   
	ShowFormProgress(2, CheckTermModuleStatusDoSend);
}

//���ɼ�����������ģ��״̬
void CheckModuleWorkStatus(void)
{ 
  
  char address[13] = {0};    
  RunStatus sRunStatus;

  sprintf(address, "%s", WirelessDevAddr);
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
  
  if (ReadRunStatus(address, &sRunStatus) == NULL)
  {
    MessageBox("����ģ�鹤���쳣!", STR_CUE, MB_OK);
  }
  else
  {
    MessageBox("����ģ�鹤������!", STR_CUE, MB_OK);
  }

}

//��ѯ�����ź�ǿ��
void CheckSignalIntensity(void)
{

  char address[13] = {0};  
  char szResult[128] = {0};   
  RunStatus sRunStatus;
  
  if (InputWlsAddr(WirelessDevAddr, address, WirelessDevType) == NULL)
  {
    return;
  }
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
  
  if (ReadRunStatus(address, &sRunStatus) == NULL)
  {
    MessageBox("����ʧ��!", STR_CUE, MB_OK);
  }
  else
  {
    sprintf(szResult, "�ź�ǿ��:\n  %ddBm", sRunStatus.rssi); 
    MessageBox(szResult, STR_CUE, MB_OK);
  }
    
}

//ɨ�踽�����紦��
void ScanWirelessNetDoSend(HWND Obj, PMsg Msg)
{
  int i = 0;
  int j = 0;
  int k = 0;
  int count = 0;
  char flag = 0;
  int number = 0;
  char channel = 0;
  char oldchannel = 0;
  char address[20] = {0};
  char showInfo[128] = {0};
  U8 setbuf[22] = {0x03, 0xC8, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xAA, 
  								 0xBB, 0x98, 0x99, 0x99, 0x99, 0x99, 0x99, 0x01, 
  								 0x8C, 0xFF, 0xFF, 0xFF, 0xFF}; 
  U8 nLen = 0;
  U8 arrbuf[MAXRECVCOUNT][MAXWLSBYTES] = {0};
  U8 arrbuflen[MAXRECVCOUNT] = {0};
  U16 arrID[MAXRECVCOUNT] = {0};
  MacData rMacData;
        
  channel = ReadChannel();
  if (!channel)
  {   
    MessageBox("��ѯʧ��!", STR_CUE, MB_OK);
    CloseFormProgress();  
    return;   
  }

  oldchannel = channel;  
         
  for(i = 0; i < ProgressBar_Proc.max; i++)
  {   
  	SendData(setbuf, sizeof(setbuf) - 1); 
  	
  	count = 0;  
  	flag = 0; 
  	for (j = 0; j < MAXRECVCOUNT; j++)
	  {
	    memset(arrbuf[count], 0, sizeof(arrbuf[count]));
	    nLen = RecvWlsData(arrbuf[count], sizeof(arrbuf[count]) - 1, MAXWLSDELAYTIME);
	    //Beep(100);
	    
	    if (nLen > 10)
	    {
	    	arrbuflen[count] = nLen;
	    	count++;
	    	flag = 0;
	    }
	    
	    flag++;
	    if (flag > 1)
	    {
	    	break;
	    }
	    if (j > 0 && count < 1)
	    {
	    	break;
	    }	    
	  } 
	  
	  Beep(100); 
	  
	  number = 0;
	  memset((unsigned char *)(&rMacData), 0, sizeof(rMacData));
	  
	  for (j = 0; j < count; j++)
	  {
	  	arrID[j] = 0;
	    nLen = arrbuflen[j];
	    WritDubugInfo(DEBEGINFOTXT, arrbuf[j], nLen, 12);
	    
	    flag = ParseWlsBuff(arrbuf[j], nLen, &rMacData);
	    
	    if (!flag)
	    {
	    	continue;
	    } 
	    
	    if (flag == MAC_DATA && rMacData.sNWKData.type == NWK_BRDCAST)
      { 	        
				for (k = 0; k < number; k++)
				{
					if (rMacData.PANID == arrID[k])
					{
						break;
					}
				}
				
				if (k >= number)
				{				
					arrID[number] = rMacData.PANID;
					number++;
		      memset(showInfo, 0, sizeof(showInfo));
		      sprintf(showInfo, "Ƶ��: %c\r\n����ID%d��%04x(H) %05d(D)\r\n", 
		      				channel, number, rMacData.PANID, rMacData.PANID);    
		      WritDubugInfo(WLSINFOTXT, showInfo, strlen(showInfo), 0xff); 	 
		    }     	        
      }
	    
	    if (nLen > MINWLSBYTES)
	    {
		    flag = ParseWlsBuff(arrbuf[j], nLen, &rMacData);
		    
		    if (!flag)
		    {
		    	continue;
		    } 
		    
		    if (flag == MAC_DATA && rMacData.sNWKData.type == NWK_BRDCAST)
	      { 	        
					for (k = 0; k < number; k++)
					{
						if (rMacData.PANID == arrID[k])
						{
							break;
						}
					}
					
					if (k >= number)
					{				
						arrID[number] = rMacData.PANID;
						number++;
			      memset(showInfo, 0, sizeof(showInfo));
			      sprintf(showInfo, "Ƶ��: %c\r\n����ID%d��%04x(H) %05d(D)\r\n", 
			      				channel, number, rMacData.PANID, rMacData.PANID);    
			      WritDubugInfo(WLSINFOTXT, showInfo, strlen(showInfo), 0xff); 	 
			    }     	        
	      }      		    	    	
	    }	  
	    
	    if (flag == MAC_OK && nLen > MINWLSBYTES / 2)
	    {
		    flag = ParseWlsBuff(arrbuf[j], nLen, &rMacData);
		    
		    if (!flag)
		    {
		    	continue;
		    } 
		    
		    if (flag == MAC_DATA && rMacData.sNWKData.type == NWK_BRDCAST)
	      { 	        
					for (k = 0; k < number; k++)
					{
						if (rMacData.PANID == arrID[k])
						{
							break;
						}
					}
					
					if (k >= number)
					{				
						arrID[number] = rMacData.PANID;
						number++;
			      memset(showInfo, 0, sizeof(showInfo));
			      sprintf(showInfo, "Ƶ��: %c\r\n����ID%d��%04x(H) %05d(D)\r\n", 
			      				channel, number, rMacData.PANID, rMacData.PANID);    
			      WritDubugInfo(WLSINFOTXT, showInfo, strlen(showInfo), 0xff); 	 
			    }     	        
	      }      		    	    	
	    }	 
	    	    	
	  }
    
    channel++;
    if(channel > '9')
    {
      channel = '0';
    }
        
    if (!SetChannel(channel))
    {
      Delay(50);
      SetChannel(channel);
    } 
		              
    ProgressStep();    
  }
  
  channel = oldchannel;
  if (!SetChannel(channel))
  {
    Delay(50);
    SetChannel(channel);
  }
		   
  CloseFormProgress();    
} 

//ɨ�踽����������
void ScanWirelessNet(void)
{
  
  if (fileexits(WLSINFOTXT) != FSNULL)
  {
    filedelete(WLSINFOTXT);
  }
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
  
  ShowFormProgress(10, ScanWirelessNetDoSend);   
  
  if (fileexits(WLSINFOTXT) == FSNULL)
  {
    MessageBox("����û��ɨ�赽\n����������Ϣ!", STR_CUE, MB_OK);
    return;
  }
  
  ShowData("����������Ϣ", WLSINFOTXT);
}


//ɨ�赱ǰƵ��������Ϣ
void ScanChannelNetDoSend(HWND Obj, PMsg Msg)
{
  
  int i = 0;
  int count = 0;
  int number = 0;
  char flag = 0;
  char address[20] = {0};
  char szResult[128] = {0};
  U8 setbuf[22] = {0x03, 0xC8, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xAA, 
  								 0xBB, 0x98, 0x99, 0x99, 0x99, 0x99, 0x99, 0x01, 
  								 0x8C, 0xFF, 0xFF, 0xFF, 0xFF}; 
  U8 nLen = 0;
  U8 arrbuf[MAXRECVCOUNT][MAXWLSBYTES] = {0};
  U8 arrbuflen[MAXRECVCOUNT] = {0};

  MacData rMacData;
   
  SendData(setbuf, sizeof(setbuf) - 1); 

  for (i = 0; i < ProgressBar_Proc.max; i++)
  {
    memset(arrbuf[count], 0, sizeof(arrbuf[count]));
    nLen = RecvWlsData(arrbuf[count], sizeof(arrbuf[count]) - 1, MAXWLSDELAYTIME);
    //Beep(100);
    
    if (nLen < 10)
    {
    	flag++;
    	if (flag > 3)
    	{
    		ProgressBar_Proc.value = ProgressBar_Proc.max - 1;
    		ProgressStep();
    		break;
    	}
    }
    else
    {
    	arrbuflen[count] = nLen;
    	count++;
    	flag = 0;
    }
        
    ProgressStep();
  }
    
  memset((unsigned char *)(&rMacData), 0, sizeof(rMacData));
  
  flag = 0;
  for (i = 0; i < count; i++)
  {
    nLen = arrbuflen[i];
    WritDubugInfo(DEBEGINFOTXT, arrbuf[i], nLen, 12);
      
    flag = ParseWlsBuff(arrbuf[i], nLen, &rMacData);
    if (!flag)
    {
    	continue;
    }  
    
    if (flag == MAC_DATA && rMacData.sNWKData.type == NWK_BRDCAST)
    {     
      memset(szResult, 0, sizeof(szResult));
      memset(address, 0, sizeof(address));
      number++;
      sprintf(szResult, "�ڵ�%d ����ID��%05d(D) %04x(H)\r\nMAC��ַ��%s\r\n·�����ȣ�%d\r\n�ź�ǿ�ȣ�%ddBm\r\n", 
      				number, rMacData.PANID, rMacData.PANID, GetMacAddr(rMacData.sNWKData.srcIEEE, 8, address), 
      				rMacData.sNWKData.count, rMacData.rssi);
      WritDubugInfo(WLSINFOTXT, szResult, strlen(szResult), 0xff);        
    }
    
    if (nLen > MINWLSBYTES)
    {
	    flag = ParseWlsBuff(arrbuf[i], nLen, &rMacData);
	    if (!flag)
	    {
	    	continue;
	    }  
        	
	    if (flag == MAC_DATA && rMacData.sNWKData.type == NWK_BRDCAST)
	    {     
	      memset(szResult, 0, sizeof(szResult));
	      memset(address, 0, sizeof(address));
	      number++;
	      sprintf(szResult, "�ڵ�%d ����ID��%05d(D) %04x(H)\r\nMAC��ַ��%s\r\n·�����ȣ�%d\r\n�ź�ǿ�ȣ�%ddBm\r\n", 
	      				number, rMacData.PANID, rMacData.PANID, GetMacAddr(rMacData.sNWKData.srcIEEE, 8, address), 
	      				rMacData.sNWKData.count, rMacData.rssi);
	      WritDubugInfo(WLSINFOTXT, szResult, strlen(szResult), 0xff);        
	    }    	
    }
    
    if (flag == MAC_OK && nLen > MINWLSBYTES / 2)
    {
	    flag = ParseWlsBuff(arrbuf[i], nLen, &rMacData);
	    if (!flag)
	    {
	    	continue;
	    }  
        	
	    if (flag == MAC_DATA && rMacData.sNWKData.type == NWK_BRDCAST)
	    {     
	      memset(szResult, 0, sizeof(szResult));
	      memset(address, 0, sizeof(address));
	      number++;
	      sprintf(szResult, "�ڵ�%d ����ID��%05d(D) %04x(H)\r\nMAC��ַ��%s\r\n·�����ȣ�%d\r\n�ź�ǿ�ȣ�%ddBm\r\n", 
	      				number, rMacData.PANID, rMacData.PANID, GetMacAddr(rMacData.sNWKData.srcIEEE, 8, address), 
	      				rMacData.sNWKData.count, rMacData.rssi);
	      WritDubugInfo(WLSINFOTXT, szResult, strlen(szResult), 0xff);        
	    }    	
    }    
  
  }
    
  CloseFormProgress();  
}

//ɨ�赱ǰƵ��������Ϣ
void ScanChannelNet(void)
{
  
  if (fileexits(WLSINFOTXT) != FSNULL)
  {
    filedelete(WLSINFOTXT);
  }
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
  
  ShowFormProgress(MAXRECVCOUNT, ScanChannelNetDoSend);   
  
  if (fileexits(WLSINFOTXT) == FSNULL)
  {
    MessageBox("����û��ɨ�赽��ǰƵ��������Ϣ!", STR_CUE, MB_OK);
    return;
  }
  
  ShowData("����������Ϣ", WLSINFOTXT);
}


//����Mac��֡����
char *SortMacInfo(U8 *buff, char *szText)
{		
  char szSrc[32] = {0};
  char szDst[32] = {0};
  char flag = 1;
  unsigned char pos = 0;
	
	MacCtrl *pCtrl=(MacCtrl *)buff;
  if (!(pCtrl->type))
  {
  	return NULL;
  }
  
  pos += 5;  									//2�ֽ�mac������� + 1�ֽ�seq + 2�ֽ�PANID
  if (pCtrl->dstMode == 2)
  {
  	GetMacAddr(buff + pos, 2, szDst);
  	pos += 2;  	 
  }
  else if (pCtrl->dstMode == 3)
  {
  	GetMacAddr(buff + pos, 8, szDst);
  	pos += 8;
  }
  else
  {
  	flag = 0;
  }

	if (pCtrl->srcMode == 2)
	{
		flag = 1;
		GetMacAddr(buff + pos, 2, szSrc);  
		pos += 2;	 		
	}
	else if (pCtrl->srcMode == 3)
	{
		flag = 1;
		GetMacAddr(buff + pos, 8, szSrc); 
		pos += 8;	
	}

	if (flag)
	{
		sprintf(szText, "%s->%s,", szSrc, szDst);
	}
		
	strcat(szText, ConstMACStr[pCtrl->type]);
    
  return szText;
}


//����ģ��ͨ�ű��Ĵ���
void FollowModuleMsgInfo(HWND Obj, PMsg Msg)
{
  char szInfo[255]={0};
  char szResult[255]={0};
  U8 nLen = 0;
  U8 nShowLen;
  U8 buf[MAXWLSBYTES] = {0};
  unsigned char rchar;
  //RunStatus sRunStatus;
  
  /*memset((unsigned char *)(&sRunStatus), 0, sizeof(sRunStatus));
  if (ReadRunStatus(WirelessDevAddr, &sRunStatus) == NULL)
  {
    MessageBox("��ȡ��ַʧ��!", STR_CUE, MB_OK);
    CloseFormResult();
    return;
  }*/
    
  while (1)
  {

    memset(buf, 0, sizeof(buf));            
    nLen = 0;
    
    do
    {
      if (rport(&rchar) == 0)
      {
        buf[nLen++] = rchar;
        continue;
      }       
      break;    
    }while(1);
		
		if (nLen > 0)
		{
			WritDubugInfo(DEBEGINFOTXT, buf, nLen, 12);
		}
		      
    nLen = GetMacBuff(buf, nLen);
		if (nLen)
		{
			memset(szInfo, 0, sizeof(szInfo));			
			if (SortMacInfo(buf, szInfo) == NULL)
			{
				continue;
			}
      nShowLen = nLen > 64 ? 64:nLen;
      sprintf(szResult, "%s;%d:%s", szInfo, nLen, BufferToStr(buf,nShowLen));
      RefleshShowResult(szResult);			
		}
		
	  if (getSystemMessage(MSG_MASK_KEY|MSG_MASK_UART, 1) == CM_REFRESH)
    {
      if(MessageBox("�Ƿ��������?", "", MB_YESNO) == ID_NO)
      {
        break;
      } 
    }
    
  }
  CloseFormResult();
}

//����ģ��ͨ�ű���
void FollowModuleMsg(void)
{
  /*char address[13] = {0}; 

  if (InputWlsAddr(WirelessDevAddr, address, WirelessDevType) == NULL)
  {
    return;
  }*/
  
  //memset(WirelessDevAddr, 0, sizeof(WirelessDevAddr));
  //memcpy(WirelessDevAddr, address, 12);
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 

  ShowResult("��������", "", FollowModuleMsgInfo);
}


//�����������������ط������ģ�ȷ����װλ��
void CheckInstallPosDoSend(HWND Obj, PMsg Msg)
{ 
  int i = 0;
  int j = 0;
  int count = 0;
  int nCount = 0;
  char flag = 0;
  short nAddr = 0;
  char address[20] = {0};
  char szResult[250] = {0};
  U8 setbuf[22] = {0x03, 0xC8, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xAA, 
  								 0xBB, 0x98, 0x99, 0x99, 0x99, 0x99, 0x99, 0x01, 
  								 0x8C, 0xFF, 0xFF, 0xFF, 0xFF}; 
  U8 nLen = 0;
  U8 result = 0;
  U8 arrbuf[MAXRECVCOUNT][MAXWLSBYTES] = {0};
  U8 arrbuflen[MAXRECVCOUNT] = {0};

  MacData arrMacData[MAXRECVCOUNT];
  RunStatus sRunStatus;
     
  memset((unsigned char *)(&sRunStatus), 0, sizeof(sRunStatus));
  if (WirelessDevType)
  {   
    if (ReadRunStatus(WirelessDevAddr, &sRunStatus) != NULL)
    {       
    	memset(szResult, 0, sizeof(szResult));  
    	sprintf(szResult, "�Ѿ�����,ͨ�������ܺ�,�������!\n����ID:%04x(H) %05d(D)�̵�ַ%04x,�ź�ǿ��%ddBm,�븸�ڵ�ͨ���ź�ǿ��%ddBm", 
    	        sRunStatus.PANID, sRunStatus.PANID, sRunStatus.nAddr, sRunStatus.rssi, sRunStatus.fRSSI);  
    	        
    	if ((sRunStatus.flag) && (sRunStatus.fRSSI > MINRSSI))//�ж��븸�ڵ�ͨ���ź�ǿ��
    	{
	      MessageBox(szResult, STR_CUE, MB_OK);
	      CloseFormProgress();  
	      return;    		
    	}  

      if (sRunStatus.PANID != 0xffff)
      {
        flag = 1;
      }     	
    }
    else
    {
    	sRunStatus.PANID = 0xffff;
    }
  }
          
  SendData(setbuf, sizeof(setbuf) - 1); 

  for (i = 0; i < ProgressBar_Proc.max; i++)
  {
    memset(arrbuf[i], 0, sizeof(arrbuf[i]));
    nLen = RecvWlsData(arrbuf[i], sizeof(arrbuf[i]) - 1, MAXWLSDELAYTIME);
    //Beep(100);
    arrbuflen[i] = nLen;
    
    ProgressStep();
  }
  
  memset(arrMacData, 0, sizeof(arrMacData));
  for (i = 0; i < ProgressBar_Proc.max; i++)
  {
    nLen = arrbuflen[i];
    WritDubugInfo(DEBEGINFOTXT, arrbuf[i], nLen, 12);
     
    result = ParseWlsBuff(arrbuf[i], nLen, arrMacData+count);   
    if (!result)
    {
    	continue;
    }
    
    if (result == MAC_DATA && (arrMacData+count)->sNWKData.type == NWK_BRDCAST)	
    { 
      	memset(address, 0, sizeof(address));           	             
        GetMacAddr((arrMacData+count)->sNWKData.srcIEEE, 8, address); 
        
        memset(szResult, 0, sizeof(szResult)); 
        sprintf(szResult, "%d ����ID��%05d(D) %04x(H)\r\nMAC��ַ��%s\r\n·�����ȣ�%d\r\n�ź�ǿ�ȣ�%ddBm\r\n", 
        				count+1, (arrMacData+count)->PANID, (arrMacData+count)->PANID, address, 
        				(arrMacData+count)->sNWKData.count, (arrMacData+count)->rssi);
        				
        WritDubugInfo(WLSINFOTXT, szResult, strlen(szResult), 0xff);
        
        for (j = 0; j < count; j++)
        {
          memset(szResult, 0, sizeof(szResult));           
          if (0 == strcmp(address, GetMacAddr((arrMacData+j)->sNWKData.srcIEEE, 8, szResult)))
          {
            break;
          }
        }     
        if (j >= count)
        {
          count++;
        }
    }  
    
    if (nLen > MINWLSBYTES)
    {
	    result = ParseWlsBuff(arrbuf[i], nLen, arrMacData+count);   
	    if (!result)
	    {
	    	continue;
	    }
	    
	    if (result == MAC_DATA && (arrMacData+count)->sNWKData.type == NWK_BRDCAST)	
	    { 
      	memset(address, 0, sizeof(address));           	             
        GetMacAddr((arrMacData+count)->sNWKData.srcIEEE, 8, address); 
        
        memset(szResult, 0, sizeof(szResult)); 
        sprintf(szResult, "%d ����ID��%05d(D) %04x(H)\r\nMAC��ַ��%s\r\n·�����ȣ�%d\r\n�ź�ǿ�ȣ�%ddBm\r\n", 
        				count+1, (arrMacData+count)->PANID, (arrMacData+count)->PANID, address, 
        				(arrMacData+count)->sNWKData.count, (arrMacData+count)->rssi);
        				
        WritDubugInfo(WLSINFOTXT, szResult, strlen(szResult), 0xff);
        
        for (j = 0; j < count; j++)
        {
          memset(szResult, 0, sizeof(szResult));           
          if (0 == strcmp(address, GetMacAddr((arrMacData+j)->sNWKData.srcIEEE, 8, szResult)))
          {
            break;
          }
        }     
        if (j >= count)
        {
          count++;
        }
    	}     	
    } 
    
    if (result == MAC_OK && nLen > MINWLSBYTES / 2)
    {
	    result = ParseWlsBuff(arrbuf[i], nLen, arrMacData+count);   
	    if (!result)
	    {
	    	continue;
	    }
	    
	    if (result == MAC_DATA && (arrMacData+count)->sNWKData.type == NWK_BRDCAST)	
	    { 
      	memset(address, 0, sizeof(address));           	             
        GetMacAddr((arrMacData+count)->sNWKData.srcIEEE, 8, address); 
        
        memset(szResult, 0, sizeof(szResult)); 
        sprintf(szResult, "%d ����ID��%05d(D) %04x(H)\r\nMAC��ַ��%s\r\n·�����ȣ�%d\r\n�ź�ǿ�ȣ�%ddBm\r\n", 
        				count+1, (arrMacData+count)->PANID, (arrMacData+count)->PANID, address, 
        				(arrMacData+count)->sNWKData.count, (arrMacData+count)->rssi);
        				
        WritDubugInfo(WLSINFOTXT, szResult, strlen(szResult), 0xff);
        
        for (j = 0; j < count; j++)
        {
          memset(szResult, 0, sizeof(szResult));           
          if (0 == strcmp(address, GetMacAddr((arrMacData+j)->sNWKData.srcIEEE, 8, szResult)))
          {
            break;
          }
        }     
        if (j >= count)
        {
          count++;
        }
    	}     	
    }
        
  }
  
  if (count < 1)
  {
    MessageBox("��λ�ò����鰲װ!", STR_CUE, MB_OK);   
    CloseFormProgress();  
    return;
  }
    
  if (!flag)				//δ�����豸����ȡ����ID
  {
  	if (WirelessDevType)
  	{
	    for (i = 0; i < count; i++)
	    {	      
	      if ((arrMacData+i)->sNWKData.count > 0)
	      { 
	      	memset(address, 0, sizeof(address)); 
          GetMacAddr((arrMacData+i)->sNWKData.srcIEEE, 8, address); 
	      	
	      	if (strstr(address, WirelessDevAddr) != NULL)
	      	{
	      		sRunStatus.PANID = (arrMacData+i)->PANID;   
	      		memcpy(&(sRunStatus.nAddr), (arrMacData+i)->srcAddr, 2);
	      		break; 
	      	}

	        //�����豸��ַ�Ƿ�����ͬһ������
	        if ((sRunStatus.PANID == 0xffff) && (FindAddress(METE_RECD_GW, address, 0) == 1))
	        {	        	
	          sRunStatus.PANID = (arrMacData+i)->PANID;  
	          sRunStatus.nAddr = 0xffff;         
	        }
	      }     
	    }
	  }
	  else
	  {
	    for (i = 0; i < count; i++)
	    {
	      if ((arrMacData+i)->sNWKData.count == 0)
	      {     
          sRunStatus.PANID = (arrMacData+i)->PANID;   
          sRunStatus.nAddr = 0xffff;
          break;        	        
	      }     
	    }  	
	  }
  	if (sRunStatus.PANID == 0xffff)
    {
      MessageBox("��ȡIDʧ��!", STR_CUE, MB_OK);
      CloseFormProgress();  
      return;
    }
  }

  for (i = 0; i < count; i++)
  { 

    if ((arrMacData+i)->PANID != sRunStatus.PANID)					//�ж�����ID�Ƿ�һ��
    {
      continue;
    }
    
    if ((arrMacData+i)->rssi < MINRSSI)											//�ж��ź�ǿ��
    {
    	continue;
    }
                    
    if ((arrMacData+i)->sNWKData.count == 0) 								//�������ڵ� 
    {
      nCount++;
      continue;
    }
        
    if ((arrMacData+i)->sNWKData.count >= MAXROUTELEN) 			//·�����ȳ�����Χ
    {
      continue;         
    }
                
    for (j = 1; j <= (arrMacData+i)->sNWKData.count; j++) 	//�ɼ�������ڵ�
    {
      memcpy(&nAddr, ((arrMacData+i)->sNWKData.sList) + 2*j, 2);
      if (nAddr == sRunStatus.nAddr)												//ȥ���Ե�ǰ�ڵ���Ϊ�м̵Ľڵ�
      {
        break;
      }
    }
    
    if (j > (arrMacData+i)->sNWKData.count)
    {
      nCount++;
    }   
  } 
          
  if (nCount < MINNODE)
  {
    MessageBox("��λ�ò����鰲װ\n�ɲ鿴��ϸ��Ϣ", STR_CUE, MB_OK);      
  }
  else
  {   
    MessageBox("��λ���ʺϰ�װ!", STR_CUE, MB_OK);   
  }
  
  CloseFormProgress();  
}


//ȷ���豸��װλ��
void CheckDevInstallPos(void)
{
  
  if (fileexits(WLSINFOTXT) != FSNULL)
  {
    filedelete(WLSINFOTXT);
  }
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
    
  ShowFormProgress(MAXRECVCOUNT, CheckInstallPosDoSend);
  
  if (fileexits(WLSINFOTXT) != FSNULL)
  {
    if (MessageBox("�Ƿ���ʾ��ϸ��Ϣ?", STR_CUE, MB_YESNO) == ID_NO)
    {
      return;
    }
    ShowData(STR_RES_READ, WLSINFOTXT);
  }

}


//��ѯ״̬��
void CheckDevRunStatus(void)
{
  
  int i = 0;
  char address[13] = {0};  
  char temp[64] = {0};
  char szResult[256] = {0};   
  RunStatus sRunStatus;
  
  if (InputWlsAddr(WirelessDevAddr, address, WirelessDevType) == NULL)
  {
    return;
  }
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
  
  memset((unsigned char *)(&sRunStatus), 0, sizeof(sRunStatus));
  if (ReadRunStatus(address, &sRunStatus) == NULL)
  {
    MessageBox("����ʧ��!", STR_CUE, MB_OK);
  }
  else
  {
    sprintf(szResult, "����ID:%04x\r\n����̵�ַ:%d\r\n·�����:%d\r\n·�ɳɱ�:%d\r\n�ź�ǿ��:%ddBm\r\n���ڵ��ź�ǿ��:%ddBm\r\n", 
    				sRunStatus.PANID, sRunStatus.nAddr, sRunStatus.nDeep, sRunStatus.nCost, sRunStatus.rssi, sRunStatus.fRSSI);
    
    strcat(szResult, "���ڵ�MAC��ַ:");   
    memset(temp, 0, sizeof(temp));
    for (i = 0; i < 8; i++)
    {
      sprintf(temp+strlen(temp), "%02x ", sRunStatus.vfMac[8 - i - 1]);
    }
    strcat(szResult, temp);
    strcat(szResult, "\r\n");
    
    strcat(szResult, "Դ·��:");
    memset(temp, 0, sizeof(temp));
    for (i = 0; i < 16; i++)
    {
      sprintf(temp+strlen(temp), "%02x ", sRunStatus.vRoute[i]);
    } 
    strcat(szResult, temp);  
    strcat(szResult, "\r\n");
    
    if (fileexits(WLSINFOTXT) != FSNULL)
    {
      filedelete(WLSINFOTXT);
    }
    
    WritDubugInfo(WLSINFOTXT, szResult, strlen(szResult), 0xff);
  
    ShowData("�豸״̬��", WLSINFOTXT);   
  }

}


//ͨ�ž������-�������ݴ���
void DistanceTestDoSend(HWND Obj, PMsg Msg)
{  

  int i = 0;
  unsigned char buf[5] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5};
  
  for (; i < ProgressBar_Proc.max; i++)
  {   
    sbuf(buf, sizeof(buf), 0);
    ProgressStep();
    Delay(1000);
  }
  
  CloseFormProgress();
}

//ͨ�ž������-��������
void DistanceTestSend(HWND Sender)
{  
  
  char value[NUM_8] = {0};
  unsigned char nCount = 0;
  
  if (InputBox("���ʹ���", "������", value, 1) == ID_CANCEL)
  {
    return;
  }
  
  nCount = StrToInt(value, DEC); 
  nCount = (nCount == 0) ? 10 : nCount; 
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
    
  ShowFormProgress(nCount, DistanceTestDoSend);
}


//ͨ�ž������-�������ݴ���
void DistanceTestDoRecv(HWND Obj, PMsg Msg)
{
  unsigned char rchar;
  unsigned short nRecvLen = 0;
  char szResult[255] = {0};
  char buf[MAXWLSBYTES]={0};
  char setbuf[6] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x00};
  U8 count = 0;

  while (1)
  {
    memset(buf, 0, sizeof(buf));
    nRecvLen = 0;
    
    do
    {
      if (rport(&rchar) == 0)
      {
        buf[nRecvLen++] = rchar;
        continue;
      }       
      break;     
    }while(1);
    
    if (nRecvLen > 0)
    {
    	WritDubugInfo(DEBEGINFOTXT, buf, nRecvLen, 12);
    }
        
    if (nRecvLen >= 5)
    {
      if (strstr(buf, setbuf) != NULL)
      {
        count++;
      }
    }
    
    if (nRecvLen > 0)
    {
      if (nRecvLen > MAXSHOWRESULTNUM / 3)
      {
        nRecvLen = MAXSHOWRESULTNUM / 3; 
      }
      sprintf(szResult,"%02d:%s",nRecvLen,BufferToStr(buf,nRecvLen));
      RefleshShowResult(szResult);
    } 
    
    if (getSystemMessage(MSG_MASK_KEY|MSG_MASK_UART, 1) == CM_REFRESH)
    {
      if (MessageBox("�Ƿ��������?", "", MB_YESNO) == ID_NO)
      {
        break;
      } 
    }   
  }
  
  memset(szResult, 0, sizeof(szResult));
  sprintf(szResult, "���ν��ճɹ� %d ��", count);
  MessageBox(szResult, "ͳ����Ϣ", MB_OK);
  
  CloseFormResult(); 
} 

//ͨ�ž������-��������
void DistanceTestRecv(HWND Sender)
{ 
	uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
  
  ShowResult("������Ϣ", "", DistanceTestDoRecv);
}


//ͨ�ž������
void CommDistanceTest(void)
{
  Window Form_DistanceTest;
                              
  TMenu Menu_DistanceTest; 
                     
  TMenuItem  DistanceTestItems[]={
    {NULL, DistanceTestSend, "   ��������"},
    {NULL, DistanceTestRecv, "   ��������"}
    }; 
                     
  CreateWindow(&Form_DistanceTest,"ͨ�ž������",0,0,160,160);
  CreateTMenu(&Menu_DistanceTest, DistanceTestItems,
  sizeof(DistanceTestItems)/sizeof(TMenuItem),10,5,120,40,FONT_CH_16);
                               
  AddToWindow(&Form_DistanceTest, &Menu_DistanceTest);
                
  ShowWindow(&Form_DistanceTest, SS_SHOWCENTER); 
}


//����Ƶ��Ƶ����ѯ����
void WirelessChannelManage(HWND Sender)
{
  int index = 0;
  
  index = ((TMenu*)Sender)->index;
  
  switch(index)
  {
    case 0:
      ReadCommChannel(NULL);
      break;
    case 1:
      SetCommChannel(NULL);
      break;
    case 2:
      AutoReadChannel(WirelessDevType);
      break;
    default:
      MessageBox("�ù����ݲ�����!", STR_CUE, MB_OK);
      return;
  }
}


//����Ƶ��Ƶ�ι���
void ChannelManage(const char *address, unsigned char type)
{
  Window Form_ChannelManage;
                              
  TMenu Menu_ChannelManage;  
                  
  TMenuItem  ChannelManageItems[] = {
    {NULL, NULL, "1.��ѯ��������Ƶ��"},
    {NULL, NULL, "2.���ñ�������Ƶ��"},
    {NULL, NULL, "3.�Զ���ȡ����Ƶ��"},         
    {NULL, NULL, "4.�Զ���ȡ����Ƶ��"},      
    {NULL, NULL, "5.��ѯ��������Ƶ��"},
    {NULL, NULL, "6.���ñ�������Ƶ��"},
    {NULL, NULL, "7.��ѯ�豸����Ƶ��"},
    {NULL, NULL, "8.�����豸����Ƶ��"},     
    }; 
                          

  CreateWindow(&Form_ChannelManage,"",0,0,160,160);
  
  CreateTMenu(&Menu_ChannelManage, ChannelManageItems,
  sizeof(ChannelManageItems)/sizeof(TMenuItem),8,3,144,126,FONT_EN_12|FONT_CH_12);
  Menu_ChannelManage.OnClick = WirelessChannelManage;                          
  
  AddToWindow(&Form_ChannelManage, &Menu_ChannelManage);

  WirelessDevType = type;
  memset(WirelessDevAddr, 0, sizeof(WirelessDevAddr));
  sprintf(WirelessDevAddr, "%s", address); 
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150);
  
  setStandbyDisable(1800);
                
  ShowWindow(&Form_ChannelManage, SS_SHOWMAX);
  
  uartinit(0, CommParam.Parity, CommParam.Port); 
  
  setStandbyDisable(0);
}


//���ߵ��Թ��ܴ���
WirelessDebugToolsPro(HWND Sender)
{
  int index = 0;
  
  index = ((TMenu*)Sender)->index;
  switch(index)
  {
    case 0:
      if (!WirelessDevType)
      {
        CheckTermModuleStatus();
      }
      else
      {
        CheckModuleWorkStatus();
      }
      break;
    case 1:
      CheckSignalIntensity();
      break;
    case 2:
      ScanWirelessNet();
      break;
    case 3:
      ScanChannelNet();
      break;
    case 4:
      CheckDevInstallPos();
      break;
    case 5:
      CheckDevRunStatus();
      break;
    case 6:
      FollowModuleMsg();
      break;
    case 7:
      CommDistanceTest();
      break;      
    default:
      MessageBox("�ù����ݲ�����!", STR_CUE, MB_OK);
      return;
  }
}


//���ߵ������˵�
void WirelessDebugTools(const char *address, unsigned char type)
{
  Window Form_WirelessDebug;
                              
  TMenu Menu_WirelessDebug;  
              
  TMenuItem  WirelessDebugItems[] = {
    {NULL, NULL, "1.���ģ�鹤��״̬"},
    {NULL, NULL, "2.���Խ����ź�ǿ��"},
    {NULL, NULL, "3.ɨ�踽����������"},         
    {NULL, NULL, "4.ɨ�赱ǰƵ����Ϣ"},      
    {NULL, NULL, "5.�Զ��жϰ�װλ��"},
    {NULL, NULL, "6.��ѯ�豸����״̬"},
    {NULL, NULL, "7.����ģ�鷢����Ϣ"}, 
    {NULL, NULL, "8.����ͨ�ž������"},    
    }; 
                          

  CreateWindow(&Form_WirelessDebug,"���ߵ���",0,0,160,160);
  
  CreateTMenu(&Menu_WirelessDebug, WirelessDebugItems,
  sizeof(WirelessDebugItems)/sizeof(TMenuItem),8,3,144,126,FONT_EN_12|FONT_CH_12);
  Menu_WirelessDebug.OnClick = WirelessDebugToolsPro;                          
  
  AddToWindow(&Form_WirelessDebug, &Menu_WirelessDebug);
    
  WirelessDevType = type;
  memset(WirelessDevAddr, 0, sizeof(WirelessDevAddr));
  sprintf(WirelessDevAddr, "%s", address); 
 
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150);    
  
  setStandbyDisable(1800);
                
  ShowWindow(&Form_WirelessDebug, SS_SHOWMAX); 
  
  uartinit(0, CommParam.Parity, CommParam.Port);
  setStandbyDisable(0);
}


//���߳�����
void WirelessReadMeterSend(HWND Sender)
{
  
  int i = 0;
  int len = 0;
  int paramindex = 0;
  char address[13] = {0};
  U8 buff[MAXWLSBYTES] = {0};
  U8 setbuf[MAXWLSBYTES] = {0x21, 0x88, 0xF9, 0xFF, 0xFF, 0x02, 0x00, 0x00, 
  													0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x01, 
  													0x19, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00};
                
  Stru645Data senddata;
  MacData rMacData; 
  memset((unsigned char *)(&rMacData), 0, sizeof(rMacData));
  RunStatus sRunStatus;
  memset((unsigned char *)(&sRunStatus), 0, sizeof(sRunStatus));
  
  if (WirelessDevType) 				//���߲ɼ���
  {
    sprintf(address, "%s", WirelessCollAddr);
  }
  else
  {
    sprintf(address, "%s", WirelessDevAddr);
  }
  FormatStr(address, 12, '0', 0); 
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
    
  //����״̬�ֲ�ѯ�����ȡ�̵�ַ��֡��š�����ID
  if (ReadRunStatus(address, &sRunStatus) != NULL)
  {
    if (!sRunStatus.flag)
    {
      MessageBox("��ȡ��ַʧ��!", STR_CUE, MB_OK);
      return;
    }
    setbuf[2] = sRunStatus.seq + 1;
    setbuf[3] = (unsigned char)(sRunStatus.PANID);
    setbuf[4] = (unsigned char)(sRunStatus.PANID >> 8);
    memcpy(setbuf+5, (unsigned char*)(&sRunStatus.nAddr), 2);
    memcpy(setbuf+11, (unsigned char*)(&sRunStatus.nAddr), 2);
    setbuf[16] = sRunStatus.seq;
    memcpy(setbuf+19, (unsigned char*)(&sRunStatus.nAddr), 2);    
  }
  else
  {
    MessageBox("��ȡ��ַʧ��!", STR_CUE, MB_OK);
    return; 
  }
  
  memset(address, 0, sizeof(address));
  sprintf(address, "%s", WirelessDevAddr);
  FormatStr(address, 12, '0', 0); 
  
  memset(senddata.address, 0, sizeof(senddata.address));
  sprintf(senddata.address, "%s", address);
        
  paramindex = ((TMenu *)Sender)->index;
  
  senddata.protype = (GlobalParam+paramindex)->Pn; 
  
  memset(GlobalParamItem, 0, sizeof(GlobalParamItem));
  GetParamValueFromDBF(GlobalParam+paramindex, GlobalParamItem, PARAMDBF);
  
  senddata.dataitm = (GlobalParam+paramindex)->Param_ID;  
    
  if (senddata.protype == PROTO_97)
  {
    senddata.ctrcode = C_READ97;  
    senddata.datalen = DI_BYTES;
  }
  else if (senddata.protype == PROTO_SH)
  {
    senddata.ctrcode = (unsigned char)(senddata.dataitm); 
    senddata.datalen = 0;   
  }
  else
  {
    senddata.ctrcode = C_READ07;  
    senddata.datalen = DI07_BYTES;    
  }     

  len = Build645SendBuff(buff, &senddata);
    
  memcpy(setbuf+23, buff, len);
  len += 23;
    
  SendData(setbuf, len);
  
  for (i = 0; i < 5; i++)
  {   
    memset(buff, 0, sizeof(buff));   
    len = RecvWlsData(buff, sizeof(buff) - 1, MAXWLSDELAYTIME);
    //Beep(100);
        
    if (ParseWlsBuff(buff, len, &rMacData) == MAC_DATA)
    {
    	if (rMacData.PANID == sRunStatus.PANID)
    	{
	      break;
	    }
    }
  }
  
  Beep(100);
    
  WritDubugInfo(DEBEGINFOTXT, buff, len, 12);
  
  len = Parse645Buff(buff, len);
  if (0 != len)
  {
    MessageBox("����ʧ��!", STR_CUE, MB_OK);       
    return;  
  } 
  
  ShowWlsParam(GlobalParam+paramindex, GlobalParamItem);    
}


//SI1000���߳�����
void WirelessReadMeterSend_SI1000(HWND Sender)
{
  
  int i = 0;
  int len = 0;
  int paramindex = 0;
  char address[13] = {0};
  U8 buff[MAXWLSBYTES] = {0};
  U8 setbuf[MAXWLSBYTES] = {0x21, 0x8C, 0x59, 0xFF, 0xFF, 0x02, 0x00, 0x00, 
  													0x00, 0x00, 0x00, 0xAA, 0xBB, 0x00, 0x00, 0x00, 
  													0x04, 0x04, 0x00, 0x00, 0x00, 0x01, 0x2E, 0x01, 
  													0x00, 0x04, 0x00, 0x00, 0x00};              
  Stru645Data senddata;
  MacData rMacData; 
  memset((unsigned char *)(&rMacData), 0, sizeof(rMacData));
  
  if (WirelessDevType) 				//���߲ɼ���
  {
    sprintf(address, "%s", WirelessCollAddr);
  }
  else
  {
    sprintf(address, "%s", WirelessDevAddr);
  }
  FormatStr(address, 12, '0', 0); 
	
  setbuf[2] = (unsigned char)(WlsSEQ + 1);
  StrToHexStr(address, 6, 1, 0, 0);
  memcpy(setbuf+5, address, 6);
  setbuf[22] = WlsSEQ;
  WlsSEQ++;
   
  memset(address, 0, sizeof(address));
  sprintf(address, "%s", WirelessDevAddr);
  FormatStr(address, 12, '0', 0); 
  
  memset(senddata.address, 0, sizeof(senddata.address));
  sprintf(senddata.address, "%s", address);
        
  paramindex = ((TMenu *)Sender)->index;
  
  senddata.protype = (GlobalParam+paramindex)->Pn; 
  
  memset(GlobalParamItem, 0, sizeof(GlobalParamItem));
  GetParamValueFromDBF(GlobalParam+paramindex, GlobalParamItem, PARAMDBF);
  
  senddata.dataitm = (GlobalParam+paramindex)->Param_ID;  
    
  if (senddata.protype == PROTO_97)
  {
    senddata.ctrcode = C_READ97;  
    senddata.datalen = DI_BYTES;
  }
  else if (senddata.protype == PROTO_SH)
  {
    senddata.ctrcode = (unsigned char)(senddata.dataitm); 
    senddata.datalen = 0;   
  }
  else
  {
    senddata.ctrcode = C_READ07;  
    senddata.datalen = DI07_BYTES;    
  }     

  len = Build645SendBuff(buff, &senddata);
    
  memcpy(setbuf+29, buff, len);
  len += 29;
  
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
    
  SendData(setbuf, len);
    
  for (i = 0; i < 5; i++)
  {   
    memset(buff, 0, sizeof(buff));   
    len = RecvWlsData(buff, sizeof(buff) - 1, MAXWLSDELAYTIME);
    //Beep(100);
    
    WritDubugInfo(DEBEGINFOTXT, buff, len, 12);
    
		len = Parse645Buff(buff, len);
		if (0 == len)
		{
			break; 
		}
  }
  
  Beep(100);
  
  if (i >= 5)
  {
	  MessageBox("����ʧ��!", STR_CUE, MB_OK);       
	  return;    	
  }
    
  ShowWlsParam(GlobalParam+paramindex, GlobalParamItem);    
}

/******************************************************************************/
/*                      ΢��������Э�鳭��˵�����                            */ 
/*  ����: ���س���˵�����                                                    */
/*  ����: address��������ַ  collect�������߲ɼ�����ַ proto�������Э������      */
/*  ����:                                                                     */
/******************************************************************************/
void WirelessReadMeter(const char *address, const char *collect, char proto)
{
  Window Form_LoadDI;
                              
  TMenu menu_LoadDI; 
  int i = 0;
  int protoType = 0;
  int menuCount = 0; 
  U8 caption[NUM_20] = {0};
                
  TMenuItem  itemsDI[MAX_PARAMITEM_NUM];
   
  if (!proto)
  {
    protoType = TYPE_MP_READ07;
  }
  else
  {
    protoType = TYPE_MP_READ97;
  }
  
  memset(GlobalParam, 0, sizeof(GlobalParam));
  menuCount = GetParamItemFromDBF(GlobalParam, (U8)protoType, PARAMDBF);
  if (menuCount < 1)
  {
    MessageBox("�˵�����ʧ��!", "��ʾ", MB_OK);
    return;
  } 
  
  for (i = 0; i < menuCount; i++)
  {
    (itemsDI[i]).icon = NULL;
    (itemsDI[i]).eventproc = NULL;
    memset((itemsDI[i]).text, 0, sizeof((itemsDI[i]).text));
    sprintf((itemsDI[i]).text, "%d.%s", i + 1, (GlobalParam+i)->Param_Name);
  }                   
  
  CreateWindow(&Form_LoadDI, "���߳���", 0,0,160,160);
  
  CreateTMenu(&menu_LoadDI, itemsDI, menuCount,10,10,140,110,FONT_CH_12|FONT_EN_12);
	
	if (WlsModuleType)
	{
		menu_LoadDI.OnClick = WirelessReadMeterSend_SI1000;
	}
	else
	{
	  menu_LoadDI.OnClick = WirelessReadMeterSend;
	}
	//menu_LoadDI.OnClick = WirelessReadMeterSend;
  menu_LoadDI.style |= WS_PAGEENABLE;

  AddToWindow(&Form_LoadDI, &menu_LoadDI);                             

  memset(WirelessDevAddr, 0, sizeof(WirelessDevAddr));
  sprintf(WirelessDevAddr, "%s", address); 
  memset(WirelessCollAddr, 0, sizeof(WirelessCollAddr));
  sprintf(WirelessCollAddr, "%s", collect); 
  FormatStr(WirelessCollAddr, 12, '0', 0); 
  if (0 != strncmp(WirelessCollAddr, "000000000000", 12))
  {
    WirelessDevType = COLLECT;
  }  
  else
  {
    WirelessDevType = 0;
  }
     
  uartinit(CommParam.Baud, CommParam.Parity, CommParam.Port);
  Delay(150); 
           
  ShowWindow(&Form_LoadDI, SS_SHOWMAX);
  
  uartinit(0, CommParam.Parity, CommParam.Port);
}
#endif
