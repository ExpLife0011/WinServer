
/**************************************************************************
	created:	2012/11/13	19:27
	filename: 	IOCPUtility.H
	file path:	e:\Documents\Visual Studio 2008\Projects\WinSock\WinSock
	author:		Dailiming, en_name: Dicky

	purpose:		
**************************************************************************/

#pragma once

#include <winsock2.h>
#include <mswsock.h>

#define BUFFER_SIZE 1024*4		// I/O����Ļ�������С
#define MAX_THREAD	2			// I/O�����̵߳�����

typedef struct _CIOCPBuffer
{
	WSAOVERLAPPED ol;				// AcceptEx ���յĿͻ����׽���
	SOCKET sClient;					
	char *buf;
	int nLen;
	ULONG nSequenceNumber;			// I/O�������к�
	int nOperation;					// ��������
#define OP_ACCEPT 1
#define OP_WRITE 2
#define OP_READ 3
	_CIOCPBuffer *pNext;
}CIOCPBuffer;


typedef struct _CIOCPContext
{
	WSAOVERLAPPED ol;				// �׽��־��
	SOCKET s;
	SOCKADDR_IN addrLocal;
	SOCKADDR_IN addrRemote;
	bool bClosing;
	int nOutstandingRecv;			// ���׽����׳����ظ���������
	int nOutstandingSend;
	ULONG nReadSeuence;				// ���Ž��յ���һ�����к�
	ULONG nCurrentReadSequence;		// ��ǰҪ�������к�
	CIOCPBuffer *pOutOfOrderReads;	//��¼û�а�˳����ɵĶ�I/O
	CRITICAL_SECTION lock;
	_CIOCPContext* pNext;
}CIOCPContext;

class CIOCPUtility
{
public:
	CIOCPUtility(void);
	~CIOCPUtility(void);
	bool Start(int nPort=4567, int nMaxConnections=2000, int nMaxFreeBuffer=200, 
		int nMaxFreeContext=100, int nInitialRead=5);
	void ShutDown(void);
	void CloseAConnection(CIOCPContext* pContext);
	void CloseAllConnections(void);

	ULONG GetCurrentConnection(void)
	{
		return m_nCurrentConnection;
	}
	bool SendText(CIOCPContext* pContext, char *pText, int nLen);
protected:
	CIOCPBuffer *AllocateBuffer(int nLen);
	void ReleaseBuffer(CIOCPBuffer *pBuffer);
	CIOCPContext* AllocateContext(SOCKET s);
	void ReleaseContext(CIOCPContext * pContext);
	void FreeBuffers();
	void FreeContexts();
	bool AddAConnection(CIOCPContext* pContext);
	bool InsertPendingAccept(CIOCPBuffer *pBuffer);
	bool RemovePendingAccept(CIOCPBuffer *pBuffer);
	CIOCPBuffer *GetNextReadBuffer(CIOCPContext* pContext, CIOCPBuffer* pBuffer);
	bool PostAccept(CIOCPBuffer *Buffer);
	bool PostSend(CIOCPContext* pContext, CIOCPBuffer*pBuffer);
	bool PostRecv(CIOCPContext* pContext, CIOCPBuffer*pBuffer);
	void HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError);
	virtual void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer* pBuffer);
	virtual void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer* pBuffer);
	virtual void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer* pBuffer, int nError);
	virtual void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer* pBuffer);
	virtual void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer* pBuffer);
protected:
	CIOCPBuffer *m_pFreeBufferList;
	CIOCPContext *m_pFreeContextList;
	int m_nFreeBufferCount;
	int m_nFreeContextCount;
	CRITICAL_SECTION m_FreeBufferListLock;
	CRITICAL_SECTION m_FreeContextListLock;
	CIOCPBuffer *m_pPendingAccepts;
	long m_nPendingAcceptCount;
	CRITICAL_SECTION m_PendingAcceptsLock;
	CIOCPContext* m_pConnectionList;
	int m_nCurrentConnection;
	CRITICAL_SECTION m_ConnectionListLock;
	HANDLE m_hAcceptEvent;
	HANDLE m_hRepostEvent;
	LONG m_nRepostCount;
	int m_nPort;
	int m_nInitialReads;
	int m_nInitialAccepts;
	int m_nMaxAccepts;
	int m_nMaxSends;
	int m_nMaxFreeBuffers;
	int m_nMaxFreeContexts;
	int m_nMaxConnections;
	HANDLE m_hListenThread;
	HANDLE m_hCompletion;
	SOCKET m_sListen;
	LPFN_ACCEPTEX m_lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockAddrs;
	bool m_bShutDown;
	bool m_bServerStarted;
private:
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);
};

