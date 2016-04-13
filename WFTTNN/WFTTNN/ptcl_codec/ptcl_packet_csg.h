#pragma once

#include "ptcl_packet.h"
#include <string>

class  ptcl_packet_csg : public ptcl_packet
{
public:
	ptcl_packet_csg(void);
	virtual ~ptcl_packet_csg(void);

	virtual void reset();

	//��װ���ģ�������set_packet_buffer
	virtual void format_packet();

	//�������� ������set_packet_buffer
	//����ֵ >0:�������ĳɹ� 0:���ĳ��Ȳ��� <0:���ĸ�ʽ����
	virtual int parse_packet();
	
	//����ֵ >0:�������ĳɹ� 0:���ĳ��Ȳ��� <0:���ĸ�ʽ����
	virtual int parse_header();

	//����ֵ >0:�������ĳɹ� 0:���ĳ��Ȳ��� <0:���ĸ�ʽ����
	virtual int parse_header(unsigned char *buffer,int len_buffer);

	//�ն��߼���ַ
	virtual unsigned __int64 get_device_addr();
	void set_device_addr(unsigned __int64 addr);

	//��վ��ַ
	virtual unsigned char get_host_id();
	void set_host_id(unsigned char id);

	//����������
	unsigned char get_afn_ctrl();
	void set_afn_ctrl(unsigned char afn);

	//֡������Чλ
	unsigned char get_fcv();
	void set_fcv(unsigned char fcv);

	//֡����λ
	unsigned char get_fcv_acd();
	void set_fcv_acd(unsigned char fcv_acd);

	//������־λ
	unsigned char get_frm();
	void set_frm(unsigned char frm);

	//���䷽��
	unsigned char get_dir();
	void set_dir(unsigned char dir);

	//��������
	unsigned char get_ctrl_code_area();
	void set_ctrl_code_area(unsigned char area);
	
	//Ӧ�ò㹦����
	unsigned char get_afn();
	void set_afn(unsigned char afn);

	//֡���
	virtual int get_frame_seq();
	void set_frame_seq(unsigned char seq);
	
	//����ȷ�ϱ�־λ
	unsigned char get_con();
	void set_con(unsigned char con);

	//����֡��־
	unsigned char get_fin();
	void set_fin(unsigned char fin);

	//��֡��־
	unsigned char get_fri();
	void set_fri(unsigned char fri);

	//֡ʱ���ǩ��Чλ
	unsigned char get_tpv();
	void set_tpv(unsigned char tpv);

	//�������ֶ�
	unsigned char get_seq_area();
	void set_seq_area(unsigned char area);

	static unsigned __int64 convert_device_addr(std::string addr);
	static std::string convert_device_addr(unsigned __int64);
    static unsigned int convert_fn(std::string fn);
 	static std::string convert_fn(unsigned int);

protected:
	//����ͷ�ṹ��
#pragma pack(push, 1)
	struct frame_header_struct
	{
		unsigned char frame_start;//0x68
		unsigned short int len_data_area;//����L �����򡢵�ַ���û����ݳ��ȵ��ֽ�����
		unsigned short int len_area_ex;
		unsigned char data_area_start;//0x68
		union 
		{
			struct
			{
				unsigned char	afn_ctrl : 4 ,//D0--D3����������
								fcv		 : 1 ,//D4֡������Чλ
								fcb_acd	 : 1 ,//D5֡����λ ���� �������λ
								frm		 : 1 ,//D6������־λ
								dir		 : 1 ;//D7���䷽��λ
			};
			unsigned char ctrl_code_area;
		};
		unsigned char device_addr[6];//�ն��߼���ַ
		unsigned char host_id;
		unsigned char afn;
		union 
		{
			struct
			{
				unsigned char    frame_seq	:	 4 ,//D0--D3֡���
								 con		:	 1 ,//D4����ȷ�ϱ�־λ
								 fin		:	 1 ,//D5����֡��־
								 fri		:	 1 ,//D6��֡��־
								 tpv		:	 1 ;//D7֡ʱ���ǩ��Чλ
			};
			unsigned char seq_area;
		};
	};

#pragma pack(pop)

	frame_header_struct *frame_header_;
};
