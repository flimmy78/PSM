#pragma once

#include "ptcl_packet.h"
#include <string>

class  ptcl_packet_dlt : public ptcl_packet
{
public:
	ptcl_packet_dlt(void);
	virtual ~ptcl_packet_dlt(void);

	virtual void reset();

	//��װ���ģ�������set_packet_buffer
	virtual void format_packet();

	//�������� ������set_packet_buffer
	//����ֵ >0:�������ĳɹ� 0:���ĳ��Ȳ��� <0:���ĸ�ʽ����
	virtual int parse_packet();

	// ����Զ����Լӵ�
    int check_packet();

	//����ֵ >0:�������ĳɹ� 0:���ĳ��Ȳ��� <0:���ĸ�ʽ����
	virtual int parse_header();

	//����ֵ >0:�������ĳɹ� 0:���ĳ��Ȳ��� <0:���ĸ�ʽ����
	virtual int parse_header(unsigned char *buffer,int len_buffer);

	//�����ն��߼���ַ ���ձ���˳��
	virtual unsigned __int64 get_device_addr();

	//���ر������к�
	virtual int get_frame_seq();

	//������վID
	virtual unsigned char get_host_id();

	//�����ն��߼���ַ ���ձ���˳��
	unsigned __int64 get_meter_addr();

	//�߼���ַ
	void set_meter_addr(unsigned __int64 addr);

	//����������
	unsigned char get_ctrl_code();
	void set_ctrl_code(unsigned char afn);

	//���ݱ�ʶ
	static short int get_data_addr(unsigned char *buf);

	static unsigned int convert_fn(std::string fn);
 	static std::string convert_fn(unsigned int short fn);
	static std::string convert_meter_addr(unsigned __int64 addr);
	static unsigned __int64 convert_meter_addr(std::string addr);

protected:
	
	//����ͷ�ṹ��
#pragma pack(push, 1)
	struct frame_header_struct
	{
		//unsigned char frame_head[4];	//ǰ��,��������
		unsigned char frame_start;//0x68
		unsigned char device_addr[6];
		unsigned char data_area_start;//0x68
		unsigned char ctrl_code;
		unsigned char len_area;
	};

#pragma pack(pop)

	frame_header_struct *frame_header_;

};