#ifndef _DATAPRO_H
#define _DATAPRO_H

#include "jblib.h"
#include "Global.h"
#include "DataConvert.h"

#define MAXCOMMSTATUSINDEX 21
const char *CommStatusStr[] = {"ע��ɹ�", 
	"��������վͨ��", 
	"��վ��Ӧ��������", 
	"��Ӧ��վ����",  
	"ͨ��ģ��δӦ��",  
	"connect ʧ��",  
	"ע��ʧ��",  
	"��ͨ��ģ��",  
	"connect �ɹ�",  
	"OK->��·����",  
	"����ע��...",  
	"���� connect...",  
	"���²���",  
	"��·���ж�",  
	"����վ��·�ж�",  
	"���ڹһ�",  
	"SIM������ʧ��",  
	"׼����λGPRSģ��",  
	"ͨѶģ�鸴λ",  
	"��վ��Ӧ����ʧ��",        
	"��վ��Ӧ������ʱ", 
	"δ����״̬", 
};


/******************************************************************************/
/*                           ��ʽ��ʱ�亯��                                   */ 
/*	����: ��ʽ��ʱ��                                                          */
/*	����: format����ʱ���ʽ, timestr������ʽ��ʱ��                               */
/*	����: ��ַ                                                                */
/******************************************************************************/
U8 *FormatTimeStr(const char *format, unsigned char *timestr, int tlen)
{

	U8 temp[NUM_10] = {0};
	U8 deststr[NUM_20] = {0};
	U8 index = 0;
	U8 len = 0;
	U8 week = 0;
	
	if (timestr[0] == NULLCHAR)
	{
		memset(timestr, 0, tlen);
		sprintf(timestr, "----");		
		return timestr;
	}

	len = strlen(format) / 2;
		
	if (tlen != len) 
	{
		return NULL;
	}
	
	memset(deststr, 0, sizeof(deststr));	
	while(index < tlen)
	{
		memset(temp, 0, sizeof(temp));
		switch(format[index*2])
		{
			case 'Y':
			case 'y':
				sprintf(temp, "%02x-", timestr[index]);
				break;
			case 'M':      //698.41/376.1Э��Format01
				if (format[index*2+1] == 'W')
				{
					week = timestr[index];
					sprintf(temp, "%02x-", week&0x1f);
					week = (week >> NUM_5) & 0x07;
				}
				else
				{
					sprintf(temp, "%02x-", timestr[index]);
				}
				break;
			case 'D':
			case 'd':
			case 's':
				sprintf(temp, "%02x ", timestr[index]);
				break;
			case 'h':
			case 'm':
				sprintf(temp, "%02x:", timestr[index]);
				break;
			case 'W':
			{
				switch(timestr[index])
				{
					case 0:
					case 7:
						sprintf(temp, "���� ");
						break;
					case 1:
						sprintf(temp, "��һ ");
						break;	
					case 2:
						sprintf(temp, "�ܶ� ");
						break;							
					case 3:
						sprintf(temp, "���� ");
						break;							
					case 4:
						sprintf(temp, "���� ");
						break;							
					case 5:
						sprintf(temp, "���� ");
						break;							
					case 6:
						sprintf(temp, "���� ");
						break;							
					default:
						break;											
				}
				break;
			}
			default:
				break;
		}
		strcat(deststr, temp);
		index++;
	}
	
	if (week > 0)     //698.41/376.1Э��Format01
	{
		memset(temp, 0, sizeof(temp));
		switch(week)
		{
			case 7:
				sprintf(temp, "���� ");
				break;
			case 1:
				sprintf(temp, "��һ ");
				break;	
			case 2:
				sprintf(temp, "�ܶ� ");
				break;							
			case 3:
				sprintf(temp, "���� ");
				break;							
			case 4:
				sprintf(temp, "���� ");
				break;							
			case 5:
				sprintf(temp, "���� ");
				break;							
			case 6:
				sprintf(temp, "���� ");
				break;							
			default:
				break;											
		}
		strcat(deststr, temp);

	}


	len = strlen(deststr);
	if(len < 1)
	{
		return NULL;
	}
	deststr[len - 1] = '\0';
	memset(timestr, 0, tlen);
	memcpy(timestr, deststr, len - 1);	
	
	return timestr;
}


/******************************************************************************/
/*                           ��ʽ���ַ���                                     */ 
/*	����: ��ʽ���ַ���                                                        */
/*	����: format������ʽ, str������ʽ������                                       */
/*	����: ��ַ                                                                */
/******************************************************************************/
U8 *FormatString(const char *format, unsigned char *str)
{

  U8 sFormat[NUM_30] = {0};
	U8 sourstr[NUM_100] = {0};	
	U8 temp[NUM_100] = {0};
	char sUnit[NUM_10] = {0};

	int i = 0;
	int fpp = 0;
	int fsLen = 0;
	U8 len = 0;
	
	if (str[0] == NULLCHAR)
	{
		memset(str, 0, strlen(str));
		sprintf(str, "----");
		return str;
	}
		
  len = strlen(format);

  memset(sFormat, 0, sizeof(sFormat));
  memcpy(sFormat, format, len);
	for (i = 0; i < len; i++)
	{
		if (format[i] == ' ')
		{
			memset(sFormat, 0, sizeof(sFormat));
			memcpy(sFormat, format, i);
			
			memcpy(sUnit, format+i, len - i);
			break;
		}
	}
	
  fsLen = strlen(sFormat);	
  fpp = fsLen;
	for (i = 0; i < fsLen; i++)
	{
		if (format[i] == '.')
		{
			fpp = i;
			break;
		}
	}
	
	if (fpp == fsLen)
	{
		strcat(str, sUnit);
		return str;
	}
	
	len = strlen(str);	
	memset(sourstr, 0, sizeof(sourstr));
	memcpy(sourstr, str, len);

	if (fpp == 1 && fsLen % 2 == 0)
	{
		memset(temp, 0, sizeof(temp));
		temp[0] = format[0];
		memcpy(temp+1, sourstr, len);
		memset(sourstr, 0, sizeof(sourstr));
		len++;
		memcpy(sourstr, temp, len);				
	}
	
  memset(temp, 0, sizeof(temp));
  memcpy(temp, sourstr+fpp, len - fpp);
  sourstr[fpp] = '.';
  sourstr[fpp+1] = '\0';  
  strcat(sourstr, temp);
  strcat(sourstr, sUnit);
  
  len = strlen(sourstr);
  
  for (i = 0; i < fpp; i++)
  {
  	if (sourstr[i] != '0')
  	{
  		break;
  	}
  }
  
  if (i == fpp)
  {
  	i--;
  }
  
  memset(str, 0, strlen(str));
  memcpy(str, sourstr+i, len - i);
  			
	return str;
}


/******************************************************************************/
/*                          �����б����ȡ����                                */ 
/*	����: ��ȡ�����б���б���                                                */
/*	����: pItem��������������  items�����б���  index�����б���Ĭ������             */
/*	����: �ɹ������б���������쳣����0                                       */
/******************************************************************************/
unsigned char GetComboTItem(Stru_ParamItem *pItem, TItem *items, int *index)
{
	int i = 0;
	int j = 0;
	int len = 0;
	int pos = 0;
	int flag = 0;
	int count = 0;
	char tmpstr[NUM_10] = {0};
	char temp[NUM_255] = {0};
	
	len = strlen(pItem->Param_Item_Value);
	if (len == 0)
	{
		return 0;
	}
	
	for (i = 0; i < len; i++)
  {
    if (pItem->Param_Item_Value[i] == '|')
    {     
    	memset(temp, 0, sizeof(temp));
    	memset(tmpstr, 0, sizeof(tmpstr));
    	memcpy(temp, pItem->Param_Item_Value + pos, i - pos);
    	for (j = 0; j < i - pos; j++)
    	{
    		if (temp[j] == ':')
    		{
    			memcpy(items + count, temp + j + 1, i - pos - j - 1);
    			memcpy(tmpstr, temp, j);
    			flag = 1;
    			break;
    		}
    	}     
      if (!flag)
      {
      	memcpy(items + count, pItem->Param_Item_Value + pos, i - pos);   
      	if (count == StrToInt(pItem->Param_Value, HEX))
      	{
      		*index = count;
      	}
      } 
      else
      {
      	if (StrToInt(tmpstr, HEX) == StrToInt(pItem->Param_Value, HEX))
      	{
      		*index = count;
      	}
      } 
      pos = i + 1;
      flag = 0;
      count++;
    }
    
  }
  memset(temp, 0, sizeof(temp));
  memset(tmpstr, 0, sizeof(tmpstr));
  memcpy(temp, pItem->Param_Item_Value + pos, i - pos);
  for (j = 0; j < i - pos; j++)
	{
		if (temp[j] == ':')
		{
			memcpy(items + count, temp + j + 1, i - pos - j - 1);
			memcpy(tmpstr, temp, j);
			flag = 1;
			break;
		}
	}
	if (!flag)
	{
		memcpy(items + count, pItem->Param_Item_Value + pos, i - pos); 
    if (count == StrToInt(pItem->Param_Value, HEX))
  	{
  		*index = count;
  	}
	} 
	else
	{
		if (StrToInt(tmpstr, HEX) == StrToInt(pItem->Param_Value, HEX))
  	{
  		*index = count;
  	}
	}   
  count++;
  
  return count;	
}


/******************************************************************************/
/*                          ��ʽ��ö��������                                  */ 
/*	����: ��ʽ��ö�������ݣ�����ö��ֵ��ȡö��������                          */
/*	����: pItem��������������  value����ö��������                                */
/*	����: �ɹ�����ö�������ݣ��쳣����NULL                                    */
/******************************************************************************/
char *FormatEnumData(Stru_ParamItem *pItem, char *value)
{
	int i = 0;
	int j = 0;
	int len = 0;
	int pos = 0;	
	int flag = 0;
	int index = 0;
	char tmpstr[NUM_10] = {0};
	char temp[NUM_255] = {0};
	
	if (pItem->Param_Item_Mask != 'e')
	{
		return NULL;
	}
	
	len = strlen(pItem->Param_Item_Value);
	if (len == 0)
	{
		return NULL;
	}
	
	if (NULLCHAR == StrToInt(pItem->Param_Value, HEX))
	{
		sprintf(value, STR_NULL);
		return value;
	}
	
	if (strstr(pItem->Param_Item_Value, CommStatusStr[1]) != NULL)
	{
		flag = StrToInt(pItem->Param_Value, HEX);
		if (flag >= MAXCOMMSTATUSINDEX)
		{
			flag = MAXCOMMSTATUSINDEX;
			sprintf(value, "%s%d", CommStatusStr[flag], flag);
		}
		else
		{
			sprintf(value, CommStatusStr[flag]);
		}
		return value;		
	}
	
	//index--;
	for (i = 0; i < len; i++)
  {
    if (pItem->Param_Item_Value[i] == '|')
    {     
    	//index++;
    	memset(temp, 0, sizeof(temp));
    	memcpy(temp, (pItem->Param_Item_Value) + pos, i - pos);

    	for (j = 0; j < i - pos; j++)
    	{
    		if (temp[j] == ':')
    		{
    			flag = 1;
    			break;
    		}
    	} 
    	    	
    	if (!flag)
    	{
    		if (index == StrToInt(pItem->Param_Value, HEX))
    		{
    			memcpy(value, temp, j);
    			flag = 1;
    			break;
    		}
    		//pos = i + 1; 
    		//continue;
    	}
    	else
    	{	    
		    memset(tmpstr, 0, sizeof(tmpstr));
				memcpy(tmpstr, temp, j);
				if (StrToInt(tmpstr, HEX) == StrToInt(pItem->Param_Value, HEX))
				{
					memcpy(value, temp + j + 1, i - pos - j - 1);
					flag = 1;
					break;
				}
			}
			flag = 0;
			index++;
    	pos = i + 1;            
    }    
  }
  
  if (flag)
  {
  	return value;
  }
  
  index++;
  memset(temp, 0, sizeof(temp));
  memcpy(temp, (pItem->Param_Item_Value) + pos, i - pos);
  for (j = 0; j < i - pos; j++)
	{
		if (temp[j] == ':')
		{
			memcpy(value, temp + j + 1, i - pos - j - 1);
			break;
		}
	}
	if (j == i - pos)
	{
		memcpy(value, temp, j);
	}
  
  return value;	
}


/******************************************************************************/
/*                          ��ȡö��ֵ����                                    */ 
/*	����: ����ö�����ݻ�ȡö��ֵ              		                            */
/*	����: pItem��������������  value����ö��������                                */
/*	����: �ɹ�����ö��ֵ���쳣����-1		                                      */
/******************************************************************************/
int GetEnumValue(Stru_ParamItem *pItem, const char *value)
{
	int i = 0;
	int j = 0;
	int len = 0;
	int pos = 0;
	int flag = 0;
	int index = 0;
	char temp[NUM_255] = {0};
	char tmpstr[NUM_32] = {0};
	
	if (pItem->Param_Item_Mask != 'e')
	{
		return ERROR;
	}
	
	len = strlen(pItem->Param_Item_Value);
	if (len == 0)
	{
		return ERROR;
	}
	
	index--;
	for (i = 0; i < len; i++)
  {
    if (pItem->Param_Item_Value[i] == '|')
    { 
    	index++;    
    	memset(temp, 0, sizeof(temp));
    	memcpy(temp, (pItem->Param_Item_Value) + pos, i - pos);
    	for (j = 0; j < i - pos; j++)
    	{
    		if (temp[j] == ':')
    		{
    			flag = 1;
    			break;    			
    		}
    	} 
    	
    	memset(tmpstr, 0, sizeof(tmpstr));
    	if (!flag)
    	{
    		memcpy(tmpstr, temp, j);
    		if (strcmp(value, tmpstr) == 0)
    		{
    			return index;
    		}  
    		 
    		pos = i + 1; 
    		continue;		
    	}   
    	 
	    memcpy(tmpstr, temp + j + 1, i - pos - j - 1);
	    if (strcmp(value, tmpstr) == 0)
	    {
	    	memset(tmpstr, 0, sizeof(tmpstr));
	    	memcpy(tmpstr, temp, j);
	    	return (StrToInt(tmpstr, HEX));
	    }
			flag = 0;
      pos = i + 1;      
    }    
  }
  
  index++;
  memset(temp, 0, sizeof(temp));
  memcpy(temp, (pItem->Param_Item_Value) + pos, i - pos);
  for (j = 0; j < i - pos; j++)
	{
		if (temp[j] == ':')
		{
			memset(tmpstr, 0, sizeof(tmpstr));
			memcpy(tmpstr, temp, j);
			break;
		}
	}
	if (j == i - pos)
	{
		return index;
	}

	return (StrToInt(tmpstr, HEX));
}


/******************************************************************************/
/*                          ��ʽ��λ�ṹ������������                          */ 
/*	����: ��ʽ��λ�ṹ�����������ݣ�������ʾ����                              */
/*	����: pItem��������������  value����ö��������                                */
/*	����: �ɹ�����ö�������ݣ��쳣����NULL                                    */
/******************************************************************************/
char *FormatBSData(Stru_Param *param, Stru_ParamItem *pItem, char *value)
{
	int i = 0;
	int j = 0;
	int len = 0;
	int pos = 0;	
	int flag = 0;
	int index = 0;
	U32 tmpint = 0;
	char tmpstr[NUM_10] = {0};
	char temp[NUM_255] = {0};
	
	if (pItem->Param_Item_Mask != 's')
	{
		return NULL;
	}
	
	if ((param->AFN == AFN_698SET || param->AFN == AFN_698READ) && (param->Fn == FN_1))
	{
		if (pItem->Param_Item_No == 2)
		{
			tmpint = StrToInt(pItem->Param_Value, HEX);
			sprintf(value, "%d;%d", tmpint & 0x0fff, (tmpint >> 12) & 0x03);				
		}
		else if (pItem->Param_Item_No == 3)
		{
			tmpint = StrToInt(pItem->Param_Value, HEX);
			if (tmpint & 0x01)
			{
				strcat(value, "1�������Զ��ϱ�;");
			}
			if (tmpint & 0x02)
			{
				strcat(value, "2�������Զ��ϱ�;");
			}
			if (tmpint & 0x04)
			{
				strcat(value, "3�������Զ��ϱ�;");
			}					
		}		
	}
	else if ((param->AFN == AFN_698SET || param->AFN == AFN_698READ) && (param->Fn == FN_8))
	{
		
		if (pItem->Param_Item_No == 0)
		{
			tmpint = (U8)(StrToInt(pItem->Param_Value, HEX));
			if (tmpint & 0x80)
			{
				strcat(value, "UDP;");					
			}
			else
			{
				strcat(value, "TCP;");		
			}
			flag = (tmpint & 0x30) >> 4;
			if (flag == 0x00)
			{
				strcat(value, "���ģʽ;");
			}
			else if (flag == 0x02)
			{
				strcat(value, "������ģʽ;");
			} 
			else if (flag == 0x01)
			{
				flag = tmpint & 0x03;
				if (flag == 1)
				{
					strcat(value, "�ͻ���;��������ģʽ");
				}
				else if (flag == 2)
				{
					strcat(value, "�ͻ���;��������ģʽ");
				}
				else if (flag == 3)
				{
					strcat(value, "�ͻ���;ʱ������ģʽ");
				}
				else
				{
					strcat(value, "�ͻ���;δ֪ģʽ");
				}
			}
			else
			{
				strcat(value, "δ֪ģʽ;");
			}				
		}	
		if (pItem->Param_Item_No == 4)
		{
			tmpint = StrToInt(pItem->Param_Value, DEC);
			if (tmpint == 0)
			{
				return value;
			}
			if (tmpint & 0xffffff == 0xffffff)
			{
				strcat(value, "ʱ�Σ�00:00-23:00");	
				return value;
			}
			index = 1;
			flag = 0;		
			for (i = 0; i < 24; i++)
			{
				if ((tmpint >> i) & 0x01)
				{
					if (flag)
					{
						continue;
					}
					memset(temp, 0, sizeof(temp));
					sprintf(temp, "ʱ��%d��", index);
					strcat(value, temp);	
					memset(temp, 0, sizeof(temp));					
					sprintf(temp, "%02d:00", i);
					strcat(value, temp);
					flag = index;
					continue;
				}
				if (flag == index)
				{
					memset(temp, 0, sizeof(temp));					
					sprintf(temp, "-%02d:00;", i);
					strcat(value, temp);		
					index++;			
				}
				flag = 0;
			}
			if (flag == index)
			{
				memset(temp, 0, sizeof(temp));					
				sprintf(temp, "-%02d:00;", i);
				strcat(value, temp);		
			}
		}			
	}
	
	else if (param->AFN == AFN_698CONF && param->Fn == FN_9)
	{
		
	}
	else if ((param->AFN == AFN_698READ) && (param->Fn == FN_33))
	{
		sprintf(temp, "%s", pItem->Param_Value);
		len = strlen(temp) / 8;
		sprintf(value, "\r\n"); 
		for (i = 0; i < len; i++)
		{
			memcpy(tmpstr, temp+i*8, 8);
			sprintf(value+strlen(value), "%c%c:%c%c-%c%c:%c%c%s", tmpstr[2], tmpstr[3], 
							tmpstr[0], tmpstr[1], tmpstr[6], tmpstr[7], tmpstr[4], tmpstr[5], "\r\n");
		}
	}
	else if (param->AFN == AFN_698RTDATA)
	{
	}
	else
	{
		strcpy(value,pItem->Param_Value);	
	}
	 
  return value;	
}


/******************************************************************************/
/*                          ��ȡip��ʽ����֡                                  */ 
/*	����: ip��ַ��֡                          		                            */
/*	����: strIP��������ip�ַ���  buff�������ip֡ revflag�����Ƿ�ת               */
/*	����: �ɹ�����ip����4���쳣����0		                                      */
/******************************************************************************/
unsigned char SeparateIP(const U8 *strIP, U8 *buff, U8 revflag)
{
	int i = 0;
	int len = 0;
	int pos = 0;
	U8 count = 0;
	U8 temp[NUM_5] = {0};
	
	len = strlen(strIP);
	if (len == 0)
	{
		return len;
	}

	for (i = 0; i < len; i++)
  {
    if (strIP[i] == '.')
    {
      memset(temp, 0, sizeof(temp));
      memcpy(temp, strIP + pos, i - pos);     
      buff[count++] = (unsigned char)StrToInt(temp, DEC);
      pos = i + 1;
    }
  }
  if (count != NUM_3)
  {
  	return 0;
  }
  memset(temp, 0, sizeof(temp));
  memcpy(temp, strIP + pos, i - pos);  
  buff[count++] = (unsigned char)StrToInt(temp, DEC);    
  
  if (revflag)
  {
  	memset(temp, 0, sizeof(temp));
  	memcpy(temp, buff, count);
  	for (i = 0; i < count; i++)
  	{
  		buff[i] = temp[count - i - 1];  		
  	}
  }
 
  return count;	
}


unsigned char SepPhoneNum(U8 *buff, U8 clen)
{
	int i = 0;
	int len = 0;
	unsigned char temp[NUM_20] = {0};
	
	len = strlen(buff);
	if (len == 0);
	{
		while (len < clen)
		{
			buff[len++] = 0xAA;
		}
		return clen;
	}
	
	for (i = 0; i < len; i++)
	{
		if (buff[i] < 0x30 || buff[i] > 0x39)
		{
			return 0;
		}
	}
  			
	memcpy(temp, buff, len);	
	memset(buff, 0, len);
	len = len / 2 + (len % 2);	
	StrToHexStr(temp, len, 1, 1, 0);
	memcpy(buff, temp, len);
	
	while (len < clen)
	{
		buff[len++] = 0xAA;
	}		
	
	return clen;
}
#endif