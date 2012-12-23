// WinSock.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "IOCPUtility.h"
#include <stdio.h>
#include <windows.h>

class CMyServer : public CIOCPUtility
{
public:

	void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
	{
		printf(" ���յ�һ���µ����ӣ�%d���� %s \n", 
			GetCurrentConnection(), ::inet_ntoa(pContext->addrRemote.sin_addr));

		SendText(pContext, pBuffer->buf, pBuffer->nLen);
	}

	void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
	{
		printf(" һ�����ӹرգ� \n" );
	}

	void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError)
	{
		printf(" һ�����ӷ������� %d \n ", nError);
	}

	void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
	{
		SendText(pContext, pBuffer->buf, pBuffer->nLen);
	}

	void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
	{
		printf(" ���ݷ��ͳɹ���\n ");
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	CMyServer *pServer = new CMyServer;

	// ��������
	if(pServer->Start())
	{
		printf(" �����������ɹ�... \n");
	}
	else
	{
		printf(" ����������ʧ�ܣ�\n");
		return -1;
	}

	// �����¼�������ServerShutdown�����ܹ��ر��Լ�
	HANDLE hEvent = ::CreateEventA(NULL, FALSE, FALSE, "ShutdownEvent");
	HANDLE hEvent1 = ::CreateEventA(NULL, FALSE, FALSE, "ShutdownEvent");
	::WaitForSingleObject(hEvent, INFINITE);
	::CloseHandle(hEvent);

	// �رշ���
	pServer->ShutDown();
	delete pServer;

	printf(" �������ر� \n ");
	return 0;
}
