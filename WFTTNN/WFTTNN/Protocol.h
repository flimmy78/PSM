#pragma once

#define MAX_DATAMARK_SIZE 27
#define SEPRATOR ","
#define RTALLCONFIRM	2
#define RTCONFIRM		3

#define RTSUCCESS	1
#define PARSEERROR -1
#define RTALLDENY		-2
#define RTDENY		-3
#define RTDATAMARKERR -4
#define RTTMOUT		-5	//��ʱ

#pragma pack (1)

typedef	struct 								// ֡ͷ�ṹ
{
	unsigned char	_head;								// 68H
    unsigned short int	_len;							// L
	unsigned short int	_lenex;							// L
	unsigned char	_headEx;							// 68H
    unsigned char   _ctrl;							    // ������
	unsigned char	_addr[7];							// ��ַ
	unsigned char   _afn;								// AFN
	unsigned char   _seq;								// SEQ
} FRAME_HEAD,		*LPFRAME_HEAD;

typedef	struct 								
{
	unsigned short int pn;			//��Ϣ���ʶ					
    unsigned long datamark;			//���ݱ�ʶ	
} PNFN,		*LPPNFN;

//�������ݽṹ
//��������ͷ
typedef	struct 
{
	unsigned char   _afn;								// AFN
	unsigned char   _ctrl;							    // ������
	unsigned char	_addr[7];							// ��ַ
	short int size;										//PNFNEX ����
}UPDATAHEAD,*LPUPDATAHEAD;

typedef	struct 
{
	unsigned short int	pn;			//��Ϣ���ʶ					
    unsigned long		datamark;			//���ݱ�ʶ		
	char				sdata[100];			//���ݣ��ַ���
}UPDATA,*LPUPDATA;

typedef	struct 	
{
	unsigned long datamark;			//���ݱ�ʶ
	short int blocknum;				//���ݿ���
	short int		ilen;			//����
	short int decimnum;				//С��λ��
	short int codestyle;			//����0-16���ƣ�1-BCD	
}DATAMARKDATA,		*LPDATAMARKDATA;

typedef	struct 	
{
	unsigned char value;
}ONEBYTE,		*LPONEBYTE; //1�ֽ�����


//�������������ʱ��
typedef	struct 
{
	unsigned char power[3];
	unsigned char tm[5];
}MAXREQTMIN,	*LPMAXREQTMIN; 

//����ʱ��
typedef	struct 
{
	unsigned char tm[6];
}DATATMIN,	*LPDATATMIN; 


//--------------------645Э��ͷ-------------------------
typedef	struct _FRAME645_HEAD								// ֡ͷ�ṹ
{
	unsigned char	_head;								// 68H
	unsigned char	_addr[6];							// ��ַ
	unsigned char	_headEx;							// 68H
	unsigned char   _ctrl;							    // ������
    unsigned char	_len;							// L
	
} FRAME645_HEAD,		*LPFRAME645_HEAD;

#pragma pack ()



LPDATAMARKDATA GetDatamarkdata(unsigned long datamark);
int GetDataMarkValue(LPDATAMARKDATA pDATAMARKDATA,unsigned char *datavalue,char *outstr);
int ParseRetFrame(unsigned char *inbuf,int inlen,unsigned char *outbuf,int *outlen);

int StringtoByte(char *instr,char *outstr,short int *outlen);
int HextoValue(char ch1,char ch2);
int chrtobyte(char ch);
int Convertstr(char *str,int ilen);
int TruncNum(char *instr,int ilen);
int BytetoString(unsigned char *instr,short int inlen,char *outstr);
int GetBCDValue(unsigned char *datavalue,int inlen,int decimnum,char *outstr);
int GetOrderBCDValue(unsigned char *datavalue,int inlen,int decimnum,char *outstr);