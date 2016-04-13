#pragma once

#include "ptcl_module.h"
#include "ptcl_packet_dlt.h"

class  ptcl_module_dlt : public ptcl_module
{
protected:
	ptcl_module_dlt(void);
	virtual ~ptcl_module_dlt(void);

public:
	enum afn_code
	{
		AFN_ALL_TIMING			=	0x08,	//�㲥��ʱ
		AFN_READ_DATA			=	0x01,	//������
		AFN_READ_NEXT_DATA		=	0x12,	//����������
		AFN_READ_DEVICE_ADDR	=	0x13,	//��ͨ�ŵ�ַ
		AFN_WRITE_DATA			=	0x14,	//д����
		AFN_WRITE_DEVICE_ADDR	=	0x15,	//дͨ�ŵ�ַ
		AFN_FREEZE				=	0x16,	//��������
		AFN_UPDATE_COM_BPS		=	0x17,	//����ͨ������
		AFN_UPDATE_PSW			=	0x18,	//�޸�����
		AFN_MAX_DEMAND_ZERO		=	0x19,	//�����������
		AFN_METER_ZERO			=	0x1A,	//�������
		AFN_ALARM_ZERO			=	0x1B	//�¼�����
	};

	static ptcl_module_dlt *get_instance();

	////����ǰ����///////////////////////////////////////////////////////////////////////
	void format_packet_read_data(	ptcl_packet_dlt *packet,unsigned int data_item);
	/*
	packet: ֻ��Ҫ��д���ֳ�Ա ����ַ
			 ���ҷ�����װ�õı���
	data_item:���ݱ�ʾ
	*/

	
	////����������///////////////////////////////////////////////////////////////////////
	void format_packet_read_curve_data(	ptcl_packet_dlt *packet,unsigned int data_item,time_t data_time);
	/*
	packet: ֻ��Ҫ��д���ֳ�Ա ����ַ
			 ���ҷ�����װ�õı���
	data_item:���ݱ�ʾ
	*/

	////�㲥Уʱ///////////////////////////////////////////////////////////////////////
	void format_packet_set_time(	ptcl_packet_dlt *packet,time_t time);
	/*
	packet: ֻ��Ҫ��д���ֳ�Ա ����ַ
			 ���ҷ�����װ�õı���
	data_item:���ݱ�ʾ
	*/



	////������������е�ѹ����������//////////////////////////////////////////////////////////
	void format_packet_write_I_U_data( ptcl_packet_dlt *packet,unsigned int data_item,
		                               short ua, short ub, short uc,
									   double ia,double ib,double ic,
									   unsigned char rs485,unsigned char time);

	////��������������¼���¼����//////////////////////////////////////////////////////////
	void format_packet_write_other_parameter( ptcl_packet_dlt *packet,unsigned int data_item,
		                                      int demandCount, int processCount, int openmeterCount, int dlCount,
											  unsigned char rs485);

	////���������ʱ����ͨ�Ÿ澯��¼ģ��//////////////////////////////////////////////////////////
	void format_packet_start_clock_communicate( ptcl_packet_dlt *packet,unsigned int data_item,
		                                        unsigned char rs485,unsigned char time);

	////���������ֹͣ/��������//////////////////////////////////////////////////////////
	void format_packet_set_stop_start( ptcl_packet_dlt *packet,unsigned int data_item,
		                               unsigned char state,
									   unsigned char rs485,unsigned char time);






	
protected:
	static ptcl_module_dlt *instance_;
};