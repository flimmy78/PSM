#ifndef TASK_H_INC
#define TASK_H_INC

#include <string>

enum TASK_TYPE {
	TASK_READ_REALTIME,
	TASK_READ_HISTORY,
	TASK_READ_EVENT,
};

struct AutoTask {
	int channelId;
	int taskId;
	int type;	
	int desity;
	int step;
	int status;
	int pn;
	int protocol;
	int port;
	int baud;
	int check;
	int databit;
	int stopbit;
	vector<double> realdata;
	vector<double> virtualdata;
	time_t starttime;
	time_t endtime;
	time_t waittime;
	time_t terminaltime;
	std::string rtuAddr;
	std::string meterAddr;
	std::string dataitem;
	std::string strdataitem;//ADD BY WM 20140114 ���������Զ������е�ǰ����������������ͬ��������
	CString realcurstring; //ADD BY WM 20140108 �����������ݲ���ʱ�洢ʵ��ֵ���ĸ����ֵ
	CString virtualcurstring; //ADD BY WM 20140108 �����������ݲ���ʱ�洢�ο�ֵ���ĸ����ֵ
	bool isfirst;
	bool bdataitemE0000130;//MODIFY BY WM 20131116 ���ڶ�ȡ�ն�ʱ��ʱ����E0000130��ʱ��
	bool isfinish; //ADD BY WM 20140108 �������������ж��ĸ����Ƿ�������
	
	AutoTask(int nChannelId, int nTaskId, int nType, int nPn, int nDesity, std::string sRtuAddr, std::string sMeterAddr, 
		std::string sDataitem, std::string sStrdataitem,time_t tBegin, time_t tEnd,bool bIsFirst = true)
	{
		channelId = nChannelId;
		taskId = nTaskId;
		type = nType;
		pn = nPn;
		desity = nDesity;
		step = 0;
		status = 0;				
		starttime = tBegin;
		endtime = tEnd;
		terminaltime = 0;
		waittime = 0;
		rtuAddr = sRtuAddr;
		meterAddr = sMeterAddr;
		dataitem = sDataitem;
		strdataitem = sStrdataitem; //ADD BY WM 20140114
		protocol = port = baud = check = databit = stopbit = 0;
		isfirst = bIsFirst;
		realcurstring = ""; //ADD BY WM 20140108
		virtualcurstring ="";//ADD BY WM 20140108
		bdataitemE0000130 = false; //ADD BY WM 20131116 
		isfinish = true; //ADD BY WM 20140108
	}

	void SetMeterInfo(int nProtocol, int nPort, int nBaud, int nCheck, int nDatabit, int nStopbit)
	{
		protocol = nProtocol;
		port = nPort;
		baud = nBaud;
		check = nCheck;
		databit = nDatabit;
		stopbit = nStopbit;
	}
};

struct Task {
	int type;
	std::string addr;
	unsigned int pn;
	std::string dataitem;
	time_t starttime;
	time_t endtime;
	int densy;
	
	Task(int nType, std::string sAddr, unsigned int nPn, std::string sDataitem, time_t tBegin, time_t tEnd, int nDensy)
	{
		type = nType;
		addr = sAddr;
		pn = nPn;
		dataitem = sDataitem;
		starttime = tBegin;
		endtime = tEnd;
		densy = nDensy;
	}
};

#endif