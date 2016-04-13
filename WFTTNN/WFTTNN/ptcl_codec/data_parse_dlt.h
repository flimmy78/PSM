#pragma once
#include<vector>
#include<string>
#include"ptcl_module_dlt.h"

class  parse_read_data
{
public:
	struct data_item_value
	{
		data_item_value ()
		{
			data_item_id = 0;
			db_value = 0;
			value_time = 0;
		};

		unsigned short int data_item_id;
		double db_value;
		unsigned int value_time;
	};

public:
	parse_read_data(void);
	~parse_read_data(void);

	//��������
	static int parse_read_data_current(unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//������������
	static int parse_read_data_curve(unsigned char *buf,int len,std::vector <data_item_value> & v_values);

protected:

	static int parse_fn(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��ǰ�����й�����
	static int parse_901f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��ǰ�����й�����
	static int parse_902f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��ǰ�����޹�����
	static int parse_911f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��ǰ�����޹�����
	static int parse_912f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��ǰһ�����޹�����
	static int parse_913f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��ǰ�������޹�����
	static int parse_914f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��ǰ�������޹�����
	static int parse_915f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��ǰ�������޹�����
	static int parse_916f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������й�����
	static int parse_941f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����й�����
	static int parse_942f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������޹�����
	static int parse_951f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����޹�����
	static int parse_952f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//����һ�����޹�����
	static int parse_953f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//�����������޹�����
	static int parse_954f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���¶������޹�����
	static int parse_955f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//�����������޹�����
	static int parse_956f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������й�����
	static int parse_9c1f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���շ����й�����
	static int parse_9c2f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������޹�����
	static int parse_9c3f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���շ����޹�����
	static int parse_9c4f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//����һ�����޹�����
	static int parse_9c5f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//�����������޹�����
	static int parse_9c6f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���ն������޹�����
	static int parse_9c7f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//�����������޹�����
	static int parse_9c8f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��ѹ����
	static int parse_b61f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//��������
	static int parse_b62f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//�й���������
	static int parse_b63f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//�޹���������
	static int parse_b64f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//������������
	static int parse_b65f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������й������������
	static int parse_a01f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����й������������
	static int parse_a02f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������޹������������
	static int parse_a11f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����޹������������
	static int parse_a12f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������й������������ʱ������
	static int parse_b01f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����й������������ʱ������
	static int parse_b02f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������޹������������ʱ������
	static int parse_b11f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����޹������������ʱ������
	static int parse_b12f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������й������������
	static int parse_a41f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����й������������
	static int parse_a42f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������޹������������
	static int parse_a51f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����޹������������
	static int parse_a52f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������й������������ʱ������
	static int parse_b41f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����й������������ʱ������
	static int parse_b42f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���������޹������������ʱ������
	static int parse_b51f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);
	//���·����޹������������ʱ������
	static int parse_b52f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values);

	//////////////////////////��������/////////////////////////////////////////////////////////////////////////////////////////
	static int parse_fn_curve(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values,unsigned int value_time);
	//��ʱ������������
	static int parse_9b1f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values,unsigned int value_time);
	//�����޹�������������
	static int parse_9b5f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values,unsigned int value_time);
	//���������������
	static int parse_aa1f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values,unsigned int value_time);
	//˲ʱ����������
	static int parse_bb1f(short int fn,unsigned char *buf,int len,std::vector <data_item_value> & v_values,unsigned int value_time);
};


