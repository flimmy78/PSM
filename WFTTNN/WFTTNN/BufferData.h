// BufferData.h: interface for the CBufferData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFERDATA_H__CB971398_AD1F_4262_BE24_F647898D1413__INCLUDED_)
#define AFX_BUFFERDATA_H__CB971398_AD1F_4262_BE24_F647898D1413__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vector"
using namespace std;


typedef vector<BYTE> BUF_VECTOR;
typedef vector<BYTE>::iterator VEC_ITER;

class CBufferData
{
public:
	CBufferData();
	virtual ~CBufferData();
	void Add(BYTE *buf,int nLen);//�ڻ������м���һ������
	int  Find(int nBegin,BYTE byValue);
	BYTE  operator[](int nPos);
	int  GetSize();
	bool GetValue(int nPos,BYTE &byValue);
	BYTE CheckSum(int nBegin,int nEnd);//�õ�У���
	void Remove(int nBegin,int nLength);//ɾ��ָ��λ���ϵ�����
	void GetBuffer(int nBegin,int nLen,BYTE buf[]);
private:
    BUF_VECTOR m_buf;
};

#endif // !defined(AFX_BUFFERDATA_H__CB971398_AD1F_4262_BE24_F647898D1413__INCLUDED_)
