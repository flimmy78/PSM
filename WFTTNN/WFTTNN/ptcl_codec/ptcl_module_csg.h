#pragma once
#include<map>

#include "ptcl_module.h"
#include "ptcl_packet_csg.h"


class  ptcl_module_csg : public ptcl_module
{
protected:
	ptcl_module_csg(void);
	virtual ~ptcl_module_csg(void);
public:
	//Ӧ�ò㹦���붨��
	enum AFN_Code
	{
		AFN_ACK						=	0x00, //ȷ�� ����
		AFN_CONNECT_TEST			=	0x02, //��·�ӿڼ��
		AFN_SET_PARAM				=	0x04, //д����
		AFN_ID_PSW					=	0x06, //���ȷ�ϼ���ԿЭ��
		AFN_READ_PARAM				=	0x0A, //������
		AFN_READ_CUR_DATA		    =	0x0C, //����ǰ����
		AFN_READ_DATA_HISTORY		=	0x0D, //����ʷ����
		AFN_READ_EVENT_RECORD		=	0x0E, //���¼���¼
		AFN_FILE_TRANS				=	0x0F, //�ļ�����
		AFN_RELAY_STATION_COMMAND	=	0x10, //�м�ת��
		AFN_READ_DATA_TASK		    =	0x12, //����������
		AFN_READ_DATA_EVENT			=	0x13, //���澯����
		AFN_CASCADE_COMMAND			=	0x14, //��������
		
	};

	//���ݵ�Ԫ
	struct pnfn_data
	{
		int pn;
		int fn;
	};

	//���ݵ�Ԫ�ṹ
	struct pnfn_struct
	{
		unsigned char pn[2];
		unsigned char fn[4];
	};

	//��ʽ����Ϣ������ݱ�ʶ
	static int format_pnfn(pnfn_data *pnfn ,unsigned char *buffer);
	static int parse_pnfn(unsigned char *buffer,pnfn_data *pnfn);
	static int parse_pn(unsigned char *buffer,int &i_pn);
	static int parse_fn(unsigned char *buffer,int &i_fn);




	//ʱ���ʶ
	struct tp_data
	{		
		unsigned char day;//��
		unsigned char hour;//ʱ
		unsigned char min;//��
		unsigned char sec;//��

		unsigned char time_out;//��ʱʱ��
	};
	//ʱ���ǩ
	static int format_tp(tp_data *tp,unsigned char *buffer);
	static int parse_tp(unsigned char *buffer,tp_data *tp);


	//��ʽ����������Ϣ
	static int format_tnm(int* v_tn,int count_tn,unsigned char *buffer);
	//������������Ϣ
	static int parse_tnm(unsigned char *buffer,int* v_tn,int &count_tn);

	//��ʷ���ݡ��¼���¼��ȷ��/�������ݵ�Ԫ
	struct pnfn_data_uint
	{
		pnfn_data pnfn;
		unsigned char data_buffer[1024];
		int len_data;
	};



	static ptcl_module_csg *get_instance();



	////ȷ�ϡ�����//////////////////////////////////////////////////////////////////////////
	void format_packet_confirm_deny_frame( ptcl_packet_csg *packet,
		                              pnfn_data_uint *v_data_uint,
									  int count_data_uint,
									  tp_data *tp = NULL);

	///ȫ��ȷ��//////////////////////////////////////////////////////////////////////////////
	void format_packet_confirm_frame(ptcl_packet_csg *packet);



	////����ǰ����//////////////////////////////////////////////////////////////////////////
	void format_packet_read_realtime_data(	ptcl_packet_csg *packet,
		                                pnfn_data *v_pnfn, int pnfn_count,
										tp_data *tp = NULL);
	/*
	packet: ֻ��Ҫ��д���ֳ�Ա���ն˵�ַ,���ҷ�����װ�õı���
	v_pnfn:��Ϣ���ʶ�����ݱ�ʶ����
	*/


	////����ʷ����//////////////////////////////////////////////////////////////////////////
	void format_packet_read_history_data(	ptcl_packet_csg *packet,
											pnfn_data_uint *v_data_uint,
											int count_data_uint,
											tp_data *tp = NULL);
	/*
	pPacket: ֻ��Ҫ��д���ֳ�Ա�ն˵�ַ
			 ���ҷ�����װ�õı���
	v_data_uint:���ݵ�Ԫ����(data_buffer���ݿ�ʼ������ʱ��)
	*/

	////���¼���¼/////////////////////////////////////////////////////////////////////////
	void format_packet_read_event_data(	ptcl_packet_csg *packet,
											pnfn_data_uint *v_data_uint,
											int count_data_uint,
											tp_data *tp = NULL);

	////���澯����/////////////////////////////////////////////////////////////////////////
	void format_packet_read_alarm_data(	ptcl_packet_csg *packet,
											pnfn_data_uint *v_data_uint,
											int count_data_uint,
											tp_data *tp = NULL);


	////д����////////////////////////////////////////////////////////////////////////////
	void format_packet_set_param(	ptcl_packet_csg * packet,
									unsigned char *password,
									pnfn_data_uint *v_data_unit,
									int count_data_unit,tp_data *tp = NULL);

	////������///////////////////////////////////////////////////////////////////////////
	void format_packet_read_parameter(	ptcl_packet_csg *packet,
		                                pnfn_data *v_pnfn, int pnfn_count,
										tp_data *tp = NULL);

	////�м�ת��////////////////////////////////////////////////////////////////////////////
	void format_packet_transmit(	ptcl_packet_csg * packet,
		                            unsigned char delayType,
									unsigned char port,
									unsigned char btl,
									unsigned char xlfs,
									unsigned char sjw,
									unsigned char tzw,
									unsigned char cssj,
									unsigned char *delayBuf, 
									int lenDelayBuf,
									unsigned char *password,
									tp_data *tp = NULL);
	


protected:
	static ptcl_module_csg *instance_;

	std::map <unsigned long,unsigned char> m_mapSeq;

	unsigned char getFrameSeq(unsigned int nAddr);

};
