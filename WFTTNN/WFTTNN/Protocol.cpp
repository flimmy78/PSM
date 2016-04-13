#include "stdafx.h"
#include "Protocol.h"


DATAMARKDATA datamarkdta[] = 
{
	{0xE0000130,1,6,0,1},			//�ն�ʱ��					1
	{0x00010000,1,4,2,1},			//(��ǰ)�����й��ܵ���		2
	{0x00010100,1,4,2,1},			//(��ǰ)�����й�����1����	3
	{0x00010200,1,4,2,1},			//(��ǰ)�����й�����2����	4
	{0x00010300,1,4,2,1},			//(��ǰ)�����й�����2����	5
	{0x00010400,1,4,2,1},			//(��ǰ)�����й�����4����	6
	{0x00020000,1,4,2,1},			//(��ǰ)�����й��ܵ���		7
	{0x00020100,1,4,2,1},			//(��ǰ)�����й�����1����	8
	{0x00020200,1,4,2,1},			//(��ǰ)�����й�����2����	9
	{0x00020300,1,4,2,1},			//(��ǰ)�����й�����3����	10
	{0x00020400,1,4,2,1},			//(��ǰ)�����й�����4����	11
	{0x02010100,1,2,1,1},			//A���ѹ					12
	{0x02010200,1,2,1,1},			//B���ѹ					13
	{0x02010300,1,2,1,1},			//C���ѹ					14
	{0x02020100,1,3,3,1},			//A�����					15
	{0x02020200,1,3,3,1},			//B�����					16
	{0x02020300,1,3,3,1},			//C�����					17
	{0x05060100,1,4,2,1},			//�ն��������й��ܵ���		18
	{0x05060101,1,4,2,1},			//�ն��������й�����1����	19
	{0x05060102,1,4,2,1},			//�ն��������й�����2����	20
	{0x05060103,1,4,2,1},			//�ն��������й�����3����	21
	{0x05060104,1,4,2,1},			//�ն��������й�����4����	22
	{0x05060200,1,4,2,1},			//�ն��ᷴ���й��ܵ���		23
	{0x05060201,1,4,2,1},			//�ն��ᷴ���й�����1����	24
	{0x05060202,1,4,2,1},			//�ն��ᷴ���й�����2����	25
	{0x05060203,1,4,2,1},			//�ն��ᷴ���й�����3����	26
	{0x05060204,1,4,2,1}			//�ն��ᷴ���й�����4����	27
	
};

int ParseRetFrame(unsigned char *inbuf,int inlen,unsigned char *outbuf,int *outlen)
{
	int iret=PARSEERROR,posin,posout;
	LPDATAMARKDATA pDATAMARKDATA;
	LPFRAME_HEAD hd=(LPFRAME_HEAD)inbuf;
	LPUPDATAHEAD uphd=(LPUPDATAHEAD)outbuf;
	LPPNFN pPNFN;
	LPUPDATA pUPDATA;
	LPMAXREQTMIN pMAXREQTMIN;
	LPDATATMIN pDATATMIN;
	LPONEBYTE pONEBYTE;

	memset(uphd,0,sizeof(UPDATAHEAD));
	uphd->_afn= hd->_afn;
	memcpy(uphd->_addr,hd->_addr,sizeof(hd->_addr));
	uphd->_ctrl= hd->_ctrl;
	posin = sizeof(FRAME_HEAD);
	posout = sizeof(UPDATAHEAD);

	pPNFN = LPPNFN(inbuf+posin);
		
	posin += sizeof(PNFN);
	switch (hd->_afn) 
	{
		case 0x00://ȷ��/����
			switch(pPNFN->datamark)
			{
				case 0xE0000000://ȫ���϶���
					pONEBYTE=LPONEBYTE(inbuf+posin);
					posin += sizeof(ONEBYTE);
					if(pONEBYTE->value ==0 ) return RTALLCONFIRM;//ȫ��ȷ��
					else return RTALLDENY;//ȫ������						
					break;
				default:
					pONEBYTE=LPONEBYTE(inbuf+posin);
					posin += sizeof(ONEBYTE);
					if(pONEBYTE->value ==0 ) return RTCONFIRM;//ȫ��ȷ��
					else return RTDENY;//ȫ������						
					break;

					
			}				
			break;
		case 0x0A://������
			pDATAMARKDATA= GetDatamarkdata(pPNFN->datamark);
			if(pDATAMARKDATA ==NULL) 
			{
				*outlen = posout;
				return RTDATAMARKERR;
			}
			else
			{
				pUPDATA = LPUPDATA(outbuf+posout);
				memset(pUPDATA,0,sizeof(UPDATA));
				pUPDATA->pn = pPNFN->pn;
				pUPDATA->datamark = pPNFN->datamark;
				GetDataMarkValue(pDATAMARKDATA,inbuf+posin,pUPDATA->sdata);
				
				uphd->size++;
				posin+=pDATAMARKDATA->ilen;
				posout += sizeof(UPDATA);
				iret = RTSUCCESS;
			}				
			break;
		case 0x0C://����ǰ���ݣ�AFN=0CH��
			switch(pPNFN->datamark)
			{
				case 0x01010000://(��ǰ)�����й����������������ʱ��
				case 0x01010100://(��ǰ)�����й�����1�������������ʱ��
				case 0x01010200://(��ǰ)�����й�����2�������������ʱ��
				case 0x01010300://(��ǰ)�����й�����3�������������ʱ��
				case 0x01010400://(��ǰ)�����й�����4�������������ʱ��
				case 0x01020000://(��ǰ)�����й����������������ʱ��
				case 0x01020100://(��ǰ)�����й�����1�������������ʱ��
				case 0x01020200://(��ǰ)�����й�����2�������������ʱ��
				case 0x01020300://(��ǰ)�����й�����3�������������ʱ��
				case 0x01020400://(��ǰ)�����й�����4�������������ʱ��
					 pMAXREQTMIN = LPMAXREQTMIN(inbuf+posin);
					 posin += sizeof(MAXREQTMIN);
					 pUPDATA = LPUPDATA(outbuf+posout);
					 memset(pUPDATA,0,sizeof(UPDATA));
					 pUPDATA->pn = pPNFN->pn;
					 pUPDATA->datamark = pPNFN->datamark;
					 //�õ��������
					 GetBCDValue(pMAXREQTMIN->power,sizeof(pMAXREQTMIN->power),4,pUPDATA->sdata);
					 strcat(pUPDATA->sdata,SEPRATOR);
					 GetBCDValue(pMAXREQTMIN->tm,sizeof(pMAXREQTMIN->tm),0,pUPDATA->sdata);
					 strcat(pUPDATA->sdata,SEPRATOR);
					 posout += sizeof(UPDATA);
					 uphd->size++;
					 iret = RTSUCCESS;
					break;
				default:
					pDATAMARKDATA= GetDatamarkdata(pPNFN->datamark);
					if(pDATAMARKDATA ==NULL) 
					{
						*outlen = posout;
						return RTDATAMARKERR;
					}
					else
					{
						pUPDATA = LPUPDATA(outbuf+posout);
						memset(pUPDATA,0,sizeof(UPDATA));
						pUPDATA->pn = pPNFN->pn;
						pUPDATA->datamark = pPNFN->datamark;
						GetDataMarkValue(pDATAMARKDATA,inbuf+posin,pUPDATA->sdata);
						//strcat(pUPDATA->sdata,SEPRATOR);
						posin+=pDATAMARKDATA->ilen;
						posout += sizeof(UPDATA);
						uphd->size++;
						iret = RTSUCCESS;
						
					}
					break;
			}
			break;
		case 0x0D://����ǰ���ݣ�AFN=0DH��
			switch(pPNFN->datamark)
			{
				case 0x05060900://�ն��������й����������������ʱ��
				case 0x05060901://�ն��������й�����1�������������ʱ��
				case 0x05060902://�ն��������й�����1�������������ʱ��
				case 0x05060903://�ն��������й�����1�������������ʱ��
				case 0x05060904://�ն��������й�����1�������������ʱ��
				case 0x05060A00://�ն��ᷴ���й����������������ʱ��
				case 0x05060A01://�ն��ᷴ���й�����1�������������ʱ��
				case 0x05060A02://�ն��ᷴ���й�����2�������������ʱ��
				case 0x05060A03://�ն��ᷴ���й�����3�������������ʱ��
				case 0x05060A04://�ն��ᷴ���й�����4�������������ʱ��
				case 0x01010001://�¶��������й����������������ʱ��
				case 0x01010101://�¶��������й�����1�������������ʱ��
				case 0x01010201://�¶��������й�����2�������������ʱ��
				case 0x01010301://�¶��������й�����3�������������ʱ��
				case 0x01010401://�¶��������й�����4�������������ʱ��
				case 0x01020001://�¶��ᷴ���й����������������ʱ��
				case 0x01020101://�¶��ᷴ���й�����1�������������ʱ��
				case 0x01020201://�¶��ᷴ���й�����2�������������ʱ��
				case 0x01020301://�¶��ᷴ���й�����3�������������ʱ��
				case 0x01020401://�¶��ᷴ���й�����4�������������ʱ��

					 pMAXREQTMIN = LPMAXREQTMIN(inbuf+posin);
					 posin += sizeof(MAXREQTMIN);
					 pUPDATA = LPUPDATA(outbuf+posout);
					 memset(pUPDATA,0,sizeof(UPDATA));
					 pUPDATA->pn = pPNFN->pn;
					 pUPDATA->datamark = pPNFN->datamark;
					 //�õ��������
					 GetBCDValue(pMAXREQTMIN->power,sizeof(pMAXREQTMIN->power),4,pUPDATA->sdata);
					 strcat(pUPDATA->sdata,SEPRATOR);
					 GetBCDValue(pMAXREQTMIN->tm,sizeof(pMAXREQTMIN->tm),0,pUPDATA->sdata);
					 //strcat(pUPDATA->sdata,SEPRATOR);
					 iret = RTSUCCESS;
					break;
				default:
					pDATAMARKDATA= GetDatamarkdata(pPNFN->datamark);
					if(pDATAMARKDATA ==NULL) 
					{
						*outlen = posout;
						return RTDATAMARKERR;
					}
					else
					{
						pUPDATA = LPUPDATA(outbuf+posout);
						memset(pUPDATA,0,sizeof(UPDATA));
						pUPDATA->pn = pPNFN->pn;
						pUPDATA->datamark = pPNFN->datamark;
						GetDataMarkValue(pDATAMARKDATA,inbuf+posin,pUPDATA->sdata);
						//strcat(pUPDATA->sdata,SEPRATOR);
						posin+=pDATAMARKDATA->ilen;	
						iret = RTSUCCESS;
					}
				break;
			}//end switch(pPNFN->datamark)
			pDATATMIN = LPDATATMIN(inbuf+posin);
			posin += sizeof(DATATMIN);
			//�õ�����ʱ��
			strcat(pUPDATA->sdata,SEPRATOR);

			GetOrderBCDValue(pDATATMIN->tm,sizeof(pDATATMIN->tm),0,pUPDATA->sdata);
			strcat(pUPDATA->sdata,SEPRATOR);
			posout += sizeof(UPDATA);
			uphd->size++;
			break;// end 0D
	}//end switch (hd->_afn) 
	*outlen = posout;
	return iret;
}


//���ز����ṹESAMFILEDATA
//���룺datamark ���ݱ�ʶ
LPDATAMARKDATA GetDatamarkdata(unsigned long datamark)
{
	int i=0,bfind=0;
	LPDATAMARKDATA ret=NULL;
	while((i<MAX_DATAMARK_SIZE) &&(!bfind))
	{
		if(datamarkdta[i].datamark == datamark)
		{
			bfind=1;
			continue;
		}
		i++;
	}
	if(bfind) ret=&datamarkdta[i];
	return ret;
}

//�õ�����ֵ
int GetDataMarkValue(LPDATAMARKDATA pDATAMARKDATA,unsigned char *datavalue,char *outstr)
{
	char tmp[200];
	if(pDATAMARKDATA -> codestyle)//bcd
	{
		memset(tmp,0,200);
		BytetoString(datavalue,pDATAMARKDATA -> ilen,tmp);
		Convertstr(tmp,strlen(tmp));//����
		if(pDATAMARKDATA -> decimnum > 0)
		{
			int k=strlen(tmp);
			memcpy(tmp+k-pDATAMARKDATA -> decimnum+1,tmp+k-pDATAMARKDATA -> decimnum,pDATAMARKDATA -> decimnum);
			*(tmp+k-pDATAMARKDATA -> decimnum) = '.';
		}
	}	
	strcat(outstr,tmp);
	return 1;
}

//�õ�����BCD��ֵ
int GetBCDValue(unsigned char *datavalue,int inlen,int decimnum,char *outstr)
{
	char tmp[200];
	memset(tmp,0,200);
	BytetoString(datavalue,inlen,tmp);
	Convertstr(tmp,strlen(tmp));//����
	if( decimnum > 0)
	{
		int k=strlen(tmp);
		memcpy(tmp+k- decimnum+1,tmp+k- decimnum,decimnum);
		*(tmp+k-decimnum) = '.';
	}
	strcat(outstr,tmp);
	return 1;
}

//�õ�˳��BCD��ֵ
int GetOrderBCDValue(unsigned char *datavalue,int inlen,int decimnum,char *outstr)
{
	char tmp[200];
	memset(tmp,0,200);
	BytetoString(datavalue,inlen,tmp);
	//Convertstr(tmp,strlen(tmp));//����
	if( decimnum > 0)
	{
		int k=strlen(tmp);
		memcpy(tmp+k- decimnum+1,tmp+k- decimnum,decimnum);
		*(tmp+k-decimnum) = '.';
	}
	strcat(outstr,tmp);
	return 1;
}

//���ַ���ת��Ϊ��������
int StringtoByte(char *instr,char *outstr,short int *outlen)
{
	int iret=1,i,ilen;
	ilen = strlen(instr);
	if(ilen % 2 !=0) return -1;//�ַ�����Ϊ����
	for(i=0;i<int(ilen/2);i++)
	{
		*(outstr+i) = HextoValue(*(instr+2*i),*(instr+2*i+1));
	}
	*outlen = int(strlen(instr)/2);
	return 1;
}

//��2���ַ�ת��Ϊһ��ʮ��������
int   HextoValue(char ch1,char ch2)
{
      int tint1,tint2;
	  tint1=chrtobyte(ch1);
	  tint2=chrtobyte(ch2);
	  if((tint1>=0)&&(tint2>=0))
		  return (tint1 << 4) + tint2;
	  else
		  return -1;
}



int BytetoString(unsigned char *instr,short int inlen,char *outstr)
{
	int i;
	char tmp[3];
	for(i=0;i<inlen;i++)
	{
		sprintf(tmp,"%02X",*(instr+i));
		strcat(outstr,tmp);
	}
	return 1;
}

//�ַ�ת��Ϊ����
int chrtobyte(char ch)
{
   if (ch>='0' && ch<='9')
   {
     return ch-'0';
   }
   else if (ch>='a' && ch<='z')
   {
     return ch-'a'+10;
   }
   else if (ch>='A' && ch<='Z')
   {
     return ch-'A'+10;
   }
   else
	   return -1;

}


int Convertstr(char *str,int ilen)
{
	char tmp[3];
	int i; 
	if(ilen % 2 ==1)return -1;
	for(i=0;i<(int)(ilen/4);i++)
	{
		tmp[0] = *(str+2*i);
		tmp[1] = *(str+2*i+1);
		*(str+2*i) = *(str+ilen-2*i-2);
		*(str+2*i+1) = *(str+ilen-2*i-1);
		*(str+ilen-2*i-2) = tmp[0];
		*(str+ilen-2*i-1) = tmp[1];
	}
	return 1;
}


int   TruncNum(char *instr,int ilen)
{
	int inlen = strlen(instr);
	if(inlen > ilen) 
	{
		memcpy(instr,instr+inlen-ilen,ilen);
		memset(instr+ilen,0,inlen-ilen);
	}
	else
	{
		memcpy(instr+ilen-inlen,instr,inlen);
		memset(instr,'0',ilen-inlen);
	}
	return 1;
}