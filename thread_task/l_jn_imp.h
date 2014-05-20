/*-----------------------------------------------------------------------*\
	file:		l_jn_imp.h
	module:		jobnest for linux
	descript:	��ģ���ṩLinux�µ�����(���߳��������)����,���ļ�������
				ģ���ڲ�ʵ���������ͷ�ŵ�
	by:			Rick Wang
	date:		2002/3
\*-----------------------------------------------------------------------*/
#ifndef L_JN_IMP_H_2E2076BC_B0C1_4a98_889F_8DAABC69D62E
#define L_JN_IMP_H_2E2076BC_B0C1_4a98_889F_8DAABC69D62E
#include "l_jn.h"
#include "rkthread.h"
#include "l_mpm.h"
#include <string.h>
#include "stdio.h"
#include "assert.h"

#define JP_PRI_LEVELCOUNT			5				//�������ȼ���
#define JP_PRI_HEIGHEST				JP_PRI_TOP		//������ȼ�
#define JN_SCH_INVALID				0xffffffff		//��Ч���̵߳������ȼ�

/*����*/
class CJobNest_imp;

class CJobQueue:virtual public CJobPkg{
private:
protected:
	CJobQueue*		m_pNext;						//��һ������
	CJobQueue*		m_pPrev;						//��һ������
public:
	CJobQueue();
	~CJobQueue();

	CJobQueue* GetNext();
	CJobQueue* GetPrev();
	void SetNext(CJobQueue* pNext);
	void SetPrev(CJobQueue* pPrev);

	void InsertBefore(CJobQueue* pjq_after);
	void InsertAfter(CJobQueue* pjq_before);
	void RemoveOut();
};


class CJobPkg_imp:public CJobQueue{
private:
protected:
	CMemPoolInterface*	m_pmpi;						//�ڴ�ع������ӿڶ���ָ��
	HANDLE				m_hPool;					//��Ӧ���ڴ�ض�����
	CJobNest_imp*		m_pjn;						//���������������񳲶���
	UINT				m_uPriroity;				//�������ȼ�
	UINT				m_uSchedule;				//�̵߳������ȼ�
	JOBFUNCTION			m_pExecFunc;				//ִ������Ļص�����
	bool				m_bSubmitted;				//��־�Ƿ��Ѿ��ύ

	PVOID				m_pvParam;					//�û�������Ĳ���

	DWORD				m_dwID;						//�����ID�����û�ָ����
													//��ͬID��������������
													//����ֻ�ܴ���1�������IDֵΪ0
    	
	void Init(CJobNest_imp* pjn);
public:
	CJobPkg_imp();
	CJobPkg_imp(CJobNest_imp* pjn);
	virtual ~CJobPkg_imp();

	//�����ڴ��
	virtual PVOID Alloc(SIZE size);
	virtual PVOID Alloc(SIZE size, UCHAR InitVal);

	//�ͷ��ڴ��
	virtual void Free(PVOID pv);

	//��������ִ��ʱ���̵߳������ȼ�����
	virtual bool SetSchedule(UINT uSch);
	//��ȡ��ǰ������̵߳������ȼ�����
	virtual UINT GetSchedule();
	//������������ȼ�
	virtual bool SetPriroity(UINT uPri);
	//��ȡ��ǰ��������ȼ�
	virtual UINT GetPriroity();


	//��������ִ�к���
	virtual void SetExecFunction(JOBFUNCTION pfun);
	//����������ִ��ʱʹ�õĲ���
	virtual void SetExecParam(PVOID pvParam);
	//���������ID�����Ѿ��ύ�������������ͬID���������
	//�����������ʹ�䱻ɾ����IDΪ0
	//����ԭ����IDֵ
	virtual DWORD SetID(DWORD dwID);
	virtual DWORD GetID();

	virtual void* QueryIF();
	virtual bool IsSubmitted();
	
	void ExecJob();						//ִ������

	void SetJobNest(CJobNest_imp* pjn);
	void SetSubmit(bool bSubmit);
	void* operator new(SIZE size, SIZE initsize, CMemPoolInterface* pmpi);
	void* operator new[](SIZE size){return NULL;};
	void operator delete(void* pv);
	void operator delete[](void* pv){return;};
	void Delete();
};

class CJobNest_imp:virtual public CJobNest{
private:
protected:
	/*�ڴ�ع������ӿڶ���*/
	CMemPoolInterface	m_mpi;		
				
	/*�����������*/
	CJobPkg_imp*		m_pJobPkgs[JP_PRI_LEVELCOUNT];	//���ύ������ж�ͷ����
	CJobPkg_imp*		m_pJobPkgEnd[JP_PRI_LEVELCOUNT];//���ύ������ж�β����
	CJobPkg_imp*		m_pUnSubmitPkgs;				//δ�ύ����

	/*�߳�ͬ����Ա*/
	CEvent				m_evtWakeUp;					//�̻߳����¼�
	CMutex				m_mtxUnSubmitPkgGuard;			//δ�ύ������������

	/*�˳������̱߳�־*/
	bool				m_bExit;

	/*�߳̾��*/
	pthread_t			m_thWorker;

	/*�̴߳��ڱ�־*/
	bool				m_bWorkerExist;			

	/*���ύ���������*/
	UINT				m_uJobCount;
	
	static void* WorkerThread(void* pvParam);			//�̺߳���
	bool WakeUpWorker();								//���ѻ򴴽������߳�
	bool ExitWorker();
														//����true��ʾ�����߳�ԭ������,
														//����false��ʾ�����߳�ԭ��������

	//ȡ��һ����������ϵͳ�Ѿ����������߳��˳��¼���
	//�򷵻�false������ʼ�շ���true��
	//������Ϊ�գ���������״̬��
	//*ppJobPkg����ȡ�õ������
	bool GetNextPkg(CJobPkg_imp** ppJobPkg);
	
	//�����������ȡ�������
	//������Ȳ����ڴ˺�����ʵ��
	CJobPkg_imp* GetPkg();
	
public:
	CJobNest_imp();
	virtual ~CJobNest_imp();

	virtual void Empty();
	virtual bool Suspend();				//�ݲ�֧��
	virtual bool Resume();				//�ݲ�֧��

	virtual CJobPkg* GetJobPkg(SIZE initsize);
	virtual bool SubmitJobPkg(CJobPkg* pjpg);
	virtual void ReleaseJobPkg(CJobPkg* pjpg);
	virtual SIZE GetMemPoolFreeSize();
	virtual SIZE GetTotalMemPoolSize();

	bool SetJobPriroity(CJobPkg_imp* pJobPkg, UINT uPri);
	bool SetJobSchedule(CJobPkg_imp* pJobPkg, UINT uSch);
};

#endif //#ifndef L_JN_H_2E2076BC_B0C1_4a98_889F_8DAABC69D62E
