/*-----------------------------------------------------------------------*\
	file:		l_mpm.cpp
	module:		memory pool manager
	descript:	�ڴ�ع��������ṩ���٣�����Ƭ���ڴ�������(ֻ������Linux����)
				���ļ�Ϊ�ڲ�ʵ���ļ�
	by:			Rick Wang
	date:		2002/3
\*-----------------------------------------------------------------------*/
#include "l_mpm_imp.h"
#include "rkthread.h"
DWORD g_dwDataAlign=0;
SIZE g_PageSize=0;				//��g_mpm��ʼ��ʱ��g_PageSize�����������ϵͳ��ֵ
SIZE g_GrainSize=0;				//��g_mpm��ʼ��ʱ��g_GrainSize�����g_PageSize����Բ��

inline DWORD GrainAlign(SIZE size){
	return ((DWORD)((size+g_GrainSize-1)/g_GrainSize));
}
inline DWORD PageAlign(SIZE size){
	return ((DWORD)((size+g_PageSize-1)/g_PageSize));
}

/*
	ȫ��Ψһ���ڴ�ع���������
*/
CMemPoolMgr g_mpm;


/*================================
	�ڴ��ģ��ӿ���ʵ�ִ���
================================*/
CMemPoolInterface::CMemPoolInterface(){
}

CMemPoolInterface::~CMemPoolInterface(){
}

/*
	�ڴ�ز���
*/
HANDLE CMemPoolInterface::CreatePool(SIZE initsize){
	return (HANDLE)g_mpm.CreatePool(NULL,initsize);
}

HANDLE CMemPoolInterface::CreatePool(HANDLE hParentPool, SIZE initsize){
	return (HANDLE)g_mpm.CreatePool((CMemPool*)hParentPool,initsize);
}

void CMemPoolInterface::EmptyPool(HANDLE hPool){
	assert(hPool);
	if(!hPool){
		return;
	}

	((CMemPool*)hPool)->Empty();
}

void CMemPoolInterface::FreePool(HANDLE hPool){
	assert(hPool);
	if(!hPool){
		return;
	}
	g_mpm.FreePool((CMemPool*)hPool);
}

void CMemPoolInterface::FreeAllPools(){
	g_mpm.FreeAllPools();
}

void CMemPoolInterface::DestroyPool(HANDLE hPool){
	assert(hPool);
	if(!hPool){
		return;
	}
	g_mpm.DestroyPool((CMemPool*)hPool);
}

void CMemPoolInterface::DestroyAllPools(){
	g_mpm.DestroyAllPools();
}

/*
	�ڴ����
*/
PVOID CMemPoolInterface::Alloc(HANDLE hPool, SIZE size){
	assert(hPool);
	if(!hPool){
		return NULL;
	}
	return ((CMemPool*)hPool)->Alloc(size);
}

PVOID CMemPoolInterface::AllocInit(HANDLE hPool,SIZE size, UCHAR ucInitVal){
	assert(hPool);
	if(!hPool){
		return NULL;
	}
	return ((CMemPool*)hPool)->Alloc(size,ucInitVal);
}

void CMemPoolInterface::Free(HANDLE hPool, PVOID pv){
	assert(hPool);
	if(!hPool){
		return;
	}
	((CMemPool*)hPool)->Free(pv);
}

void CMemPoolInterface::FreeAllMemory(){
	g_mpm.FreeAllBlks();
}

SIZE CMemPoolInterface::GetFreeMemSizeInPoolManeger(){
	return g_mpm.GetFreeBlkSize();
}

SIZE CMemPoolInterface::GetTotalMemSizeInPoolManeger(){
	return CBlkHdr::GetTotalMemSize();
}

/*=======================
	�ڴ��ͷ��ʵ�ִ���
=======================*/
DWORD CBlkHdr::ms_TotalBlkCount=0;
SIZE CBlkHdr::ms_TotalMemSize=0;
CMutex CBlkHdr::ms_mtxGuard;

CBlkHdr::CBlkHdr(){
	m_blk.pNext=m_blk.pPrev=NULL;
	m_blk.pBigger=NULL;
	m_blk.pNextAvail=m_blk.ucData;
	m_blk.pEnd=(PUCHAR)this + (m_blk.dwGrainCount * g_GrainSize - 1);
//	dbout("****************Next avail address: 0x%08x",m_blk.pNextAvail);

}

CBlkHdr::~CBlkHdr(){
}

/*�����ڴ���䷽���ڴ�ʵ��-------------------------------------------*/
void* CBlkHdr::operator new(SIZE size){
//	dbout("****************new blk: size=%d",size);
	
	assert(size>0);


	//����Բ��Ϊ�ض���������ʱ��Ҫ�ĳߴ�
	SIZE blk_size=g_GrainSize * GrainAlign(size);
	//�����ڴ棬��������ӳ��
	void* pv=mmap(NULL,blk_size, PROT_READ | PROT_WRITE,
				  MAP_ANON | MAP_PRIVATE, -1, 0);
	//������ʧ�ܣ�������ֵ��Ϊ��
	if(-1==(int)pv || !pv){
		pv=NULL;
		throw(ERR_NOT_ENOUGH_MEMORY);
	}
	else{
		((BLKHDR*)pv)->dwGrainCount=GrainAlign(size);
//		dbout("thread[%d]:******lock blk new",pthread_self());
		CBlkHdr::ms_mtxGuard.Lock();

		CBlkHdr::ms_TotalBlkCount++;
		CBlkHdr::ms_TotalMemSize+=blk_size;

		CBlkHdr::ms_mtxGuard.UnLock();
//		dbout("thread[%d]:******unlock blk new--------",pthread_self());
//		dbout("****************new blk size in fact: size=%d",blk_size);
//		dbout("****************blk address: 0x%08x",pv);
	}

	return pv;
}

void* CBlkHdr::operator new(SIZE size, SIZE initsize){
	assert(size>0);
	size+= initsize;
	return (operator new(size));
}

void CBlkHdr::operator delete(void* pblk){
//	dbout("*******************Delete blk: 0x%08x",pblk);
//	dbout("*******************%d byte memory will be free",((BLKHDR*)pblk)->dwGrainCount * g_GrainSize);
//	dbout("thread[%d]:******lock blk delete",pthread_self());
	CBlkHdr::ms_mtxGuard.Lock();

	CBlkHdr::ms_TotalBlkCount--;
	CBlkHdr::ms_TotalMemSize-=((BLKHDR*)pblk)->dwGrainCount * g_GrainSize;

	CBlkHdr::ms_mtxGuard.UnLock();
//	dbout("thread[%d]:******unlock blk delete--------",pthread_self());
	
	munmap(pblk,((BLKHDR*)pblk)->dwGrainCount * g_GrainSize);
}
/*-------------------------------------------------------------------*/

inline PVOID CBlkHdr::AlignAddress(PVOID pv) const{
	//ֻ�е�g_dwDataAlign=0x1��0x10��0x100��0x1000��0x10000...ʱ�����㷨����ȷ
	//��m_dwDataAlign=1��2��4��8��16...2^n
	//��m_dwDataAlign��ʾϵͳ��ݿ�ȣ����ֽ�Ϊ��λ��������g_dwDataAlign��������Ҫ��
	return (PVOID)(((DWORD)pv+(g_dwDataAlign-1)) & ~(g_dwDataAlign-1));
}

inline SIZE CBlkHdr::GetAvailSize() const{
	return (SIZE)(m_blk.pEnd >= m_blk.pNextAvail ? (DWORD)m_blk.pEnd - (DWORD)m_blk.pNextAvail + 1 : 0);
}

inline bool CBlkHdr::IsAddressInBlk(PVOID pv) const{
	return (DWORD)pv>=(DWORD)(m_blk.ucData) && (DWORD)pv<=(DWORD)(m_blk.pEnd);
}

inline bool CBlkHdr::IsFull() const{
	return m_blk.pEnd >= m_blk.pNextAvail;
}

inline bool CBlkHdr::IsFit(SIZE size) const{
	return (GetAvailSize() >= size);
}

inline DWORD CBlkHdr::GetGrainCount() const{
	return m_blk.dwGrainCount;
}

inline CBlkHdr* CBlkHdr::GetBigger() const{
	return m_blk.pBigger;
}

inline CBlkHdr* CBlkHdr::GetNext() const{
	return m_blk.pNext;
}

inline CBlkHdr* CBlkHdr::GetPrev() const{
	return m_blk.pPrev;
}

inline void CBlkHdr::SetBigger(CBlkHdr* pBigger){
	m_blk.pBigger=pBigger;
}

inline void CBlkHdr::SetNext(CBlkHdr* pNext){
	m_blk.pNext=pNext;
}

inline void CBlkHdr::SetPrev(CBlkHdr* pPrev){
	m_blk.pPrev=pPrev;
}

inline void CBlkHdr::InsertAfter(CBlkHdr* pblk_before){
	if(pblk_before){
		//��ǰ��
		m_blk.pNext=pblk_before->GetNext();
		m_blk.pPrev=pblk_before;
		//��ǰ
		pblk_before->SetNext(this);
		//�Ϻ�
		if(m_blk.pNext){
			m_blk.pNext->SetPrev(this);
		}
	}
	else{
		m_blk.pNext=m_blk.pPrev=NULL;
	}
}

inline void CBlkHdr::InsertBefore(CBlkHdr* pblk_after){
	if(pblk_after){
		//��ǰ��
		m_blk.pNext=pblk_after;
		m_blk.pPrev=pblk_after->GetPrev();
		//��ǰ
		if(m_blk.pPrev){
			m_blk.pPrev->SetNext(this);
		}
		//�Ϻ�
		pblk_after->SetPrev(this);
	}
	else{
		m_blk.pNext=m_blk.pPrev=NULL;
	}
}

inline void CBlkHdr::RemoveOut(){
	//��ǰ��
	if(m_blk.pPrev){
		m_blk.pPrev->SetNext(m_blk.pNext);
	}
	if(m_blk.pNext){
		m_blk.pNext->SetPrev(m_blk.pPrev);
	}
	//��ǰ
	m_blk.pPrev=NULL;
	//�Ϻ�
	m_blk.pNext=NULL;
}

inline PVOID CBlkHdr::Alloc(SIZE size){
//	dbout("***************+blk alloc: size = %d",size);
	PVOID pv=NULL;
	if(size && IsFit(size)){
		pv=m_blk.pNextAvail;
		m_blk.pNextAvail=(PUCHAR)AlignAddress((char*)(m_blk.pNextAvail)+size);
		
		assert(m_blk.pNextAvail<=AlignAddress(m_blk.pEnd+1));
	}
//	dbout("***************+Next avail address: 0x%08x",m_blk.pNextAvail);
	return pv;
}

inline void CBlkHdr::Free(PVOID pv){
	if(pv){
		if((DWORD)pv>=(DWORD)(m_blk.ucData) && (DWORD)pv<=(DWORD)(m_blk.pEnd)){
			m_blk.pNextAvail=(PUCHAR)AlignAddress(pv);;
		}
	}
}

inline void CBlkHdr::Empty(){
	m_blk.pNextAvail=(PUCHAR)AlignAddress(m_blk.ucData);
}

inline DWORD CBlkHdr::GetTotalMemBlkCount(){
	return CBlkHdr::ms_TotalBlkCount;
};
inline SIZE CBlkHdr::GetTotalMemSize(){
	return CBlkHdr::ms_TotalMemSize;
};


/*
	�ڴ�ع�������ʵ�ִ���
*/
CMemPoolMgr::CMemPoolMgr(){
	//��ʼȫ�ֱ���
	g_dwDataAlign=sizeof(int);
	if(!g_GrainSize){
		assert(MPM_GRAINSIZE);
		g_PageSize=getpagesize();
		g_GrainSize=g_PageSize * PageAlign(MPM_GRAINSIZE);
	}

	m_pFreeBlks=NULL;
	m_pLastFreeBlk=NULL;
	m_pTopLevelPools=NULL;
	
	m_dwFreeBlkCount=0;
	m_szFreeBlkSize=0;
}

CMemPoolMgr::~CMemPoolMgr(){
	//rick:debug
//	dbout("thread[%d]:CMemPoolMgr destroying...",pthread_self());
	
	//�ͷ����п��п���ڴ�
	FreeAllBlks();
	//�ͷ������ڴ�ص��ڴ�
	FreeAllPools();
	
	//rick:debug
//	dbout("thread[%d]:CMemPoolMgr destroyed",pthread_self());
}

/*���п�����������----------------------------------------------------*/
CBlkHdr* CMemPoolMgr::AllocBlkFromFreeChain(DWORD dwGrainCount){
	m_mtxFreeBlksGuard.Lock();

	//���Һ��ʵĿ�
	CBlkHdr* pblk=m_pFreeBlks;
	CBlkHdr* pblk_smaller=NULL;
	while(pblk && pblk->GetGrainCount()<dwGrainCount){
		pblk_smaller=pblk;
		pblk=pblk->GetBigger();
	}

	//���ҵ�
	if(pblk){
		CBlkHdr* pnext=pblk->GetNext();
		if(pnext){
			//��һ��Ĵ�С�����Ǵ��ڻ���ڱ���
			assert(pnext->GetGrainCount() >= pblk->GetGrainCount());
			//�����С˳����
			if(pnext->GetGrainCount()==pblk->GetGrainCount()){
				pnext->SetBigger(pblk->GetBigger());
			}

		}
		else{
			//û����һ���飬��Ҫ�޸�β��ָ��
			m_pLastFreeBlk=pblk->GetPrev();
		}

		if(pblk_smaller){
			pblk_smaller->SetBigger(pnext);
		}
		else{
			//û�и�С�Ŀ飬�򱾿�����ǵ�ǰ���׿�
			assert(m_pFreeBlks==pblk);
			//�޸��׿�ָ�룬ָ����һ��
			m_pFreeBlks=pnext;
		}

		//�������˳�����
		pblk->RemoveOut();
		pblk->SetBigger(NULL);

		m_dwFreeBlkCount--;
		m_szFreeBlkSize-=pblk->GetGrainCount()*g_GrainSize;
	}
	
//    dbout("...............Alloc -- : Cur block count %d",m_dwFreeBlkCount);
	m_mtxFreeBlksGuard.UnLock();
	
	return pblk;
}

//��һ���ڴ�鷵������п���
//����ǰ�����п鶼����գ��������ͷ��Ѿ��ύ�������ڴ�
//��Щ�齫�������ӵķ���˳��������п���(����ͬ��С�Ŀ����ǽ���󷴻��Ŀ������ǰ��)
//(����������ȷ�������Ϊ������Ŀ������ύ�����ڴ���ܱȽ϶࣬������ڿ�������ǰ���Է���
//�´��û������ʱ�Ƚ����ͳ����Ӷ���������ڴ���ύ��Ҳ����ִ�������ڴ��ύ��ʱ�俪��)
void CMemPoolMgr::ReturnBlksToFreeChain(CBlkHdr* pblks){
	if(!pblks){
		return;
	}
	pblks->SetPrev(NULL);

	CBlkHdr* pblk_cur=NULL;
	CBlkHdr* pblk_end=pblks;
	CBlkHdr* pblk_smaller=NULL;
	CBlkHdr* pblk_after=NULL;

	//�ҵ��������һ��
	while(pblk_end->GetNext()){
//		assert((pblk_end->DebugDump(),true));
		pblk_end=pblk_end->GetNext();
	}

	m_mtxFreeBlksGuard.Lock();
	
	while(pblk_end){
		pblk_cur=pblk_end;
		pblk_end=pblk_end->GetPrev();

		//��տ�
		pblk_cur->Empty();
		
		//Ѱ�Ҳ���λ�ã�����ĳߴ��С������룩
		pblk_after=m_pFreeBlks;
		pblk_smaller=NULL;
		while(pblk_after && pblk_after->GetGrainCount()<pblk_cur->GetGrainCount()){
			pblk_smaller=pblk_after;
			pblk_after=pblk_after->GetBigger();
		}
		
		//��������
		if(pblk_after){
			pblk_cur->InsertBefore(pblk_after);
			if(m_pFreeBlks==pblk_after){
				m_pFreeBlks=pblk_cur;
			}

			if(pblk_after->GetGrainCount()==pblk_cur->GetGrainCount()){
				pblk_cur->SetBigger(pblk_after->GetBigger());
			}
			else{
				pblk_cur->SetBigger(pblk_after);
			}
		}
		else{
			if(m_pLastFreeBlk){
				//û�д��ڵ��ڱ���Ŀ��п飬�򽫱�����뵽ĩβ
				pblk_cur->InsertAfter(m_pLastFreeBlk);
			}
			else{
				//��û��β�飬����п��������ǿյ�
				assert(m_pFreeBlks==NULL);
				//��������Ϊ�׿�
				pblk_cur->SetPrev(NULL);
				pblk_cur->SetNext(NULL);
				m_pFreeBlks=pblk_cur;
			}
			//����϶�Ҳ��β��
			m_pLastFreeBlk=pblk_cur;

			pblk_cur->SetBigger(NULL);
		}//end if(pblk_after)

		if(pblk_smaller){
			pblk_smaller->SetBigger(pblk_cur);
		}
		
		//pblk_cur->DebugDump();
		m_dwFreeBlkCount++;
		m_szFreeBlkSize+=pblk_cur->GetGrainCount()*g_GrainSize;
	}//end of while(pblk_end)
	
//    dbout("...............Return ++ : Cur block count %d",m_dwFreeBlkCount);
	m_mtxFreeBlksGuard.UnLock();

}

void CMemPoolMgr::FreeAllBlks(){
	m_mtxFreeBlksGuard.Lock();
	//rick:debug
//	dbout("thread[%d]:FreeAllBlks...",pthread_self());

	while(m_pFreeBlks && m_pFreeBlks->GetNext()){
		m_pFreeBlks=m_pFreeBlks->GetNext();
		delete (m_pFreeBlks->GetPrev());
	}

	if(m_pFreeBlks){
		delete m_pFreeBlks;
	}

	m_pFreeBlks=m_pLastFreeBlk=NULL;
	m_dwFreeBlkCount=0;
	
	//rick:debug
//	dbout("thread[%d]:FreeAllBlks ok...",pthread_self());
	m_mtxFreeBlksGuard.UnLock();
}
/*--------------------------------------------------------------------*/

/*�ڴ�ز�������------------------------------------------------------*/

//��һ���ؼ��뵽����������
void CMemPoolMgr::AddPool(CMemPool* pPool){
	assert(pPool);
	if(!pPool){
		return;
	}
	m_mtxPoolsGuard.Lock();

	pPool->InsertBefore(m_pTopLevelPools);
	m_pTopLevelPools=pPool;

	m_mtxPoolsGuard.UnLock();
}

void CMemPoolMgr::AddPool(CMemPool* pParent, CMemPool* pPool){
	if(!pParent){
		AddPool(pPool);
		return;
	}

	assert(pPool);
	if(!pPool){
		return;
	}

	m_mtxPoolsGuard.Lock();

	pPool->InsertBefore(pParent->GetChild());
	pParent->SetChild(pPool);
	pPool->SetParent(pParent);

	m_mtxPoolsGuard.UnLock();
}

//���ڴ����������һ���أ�����ص����������Ҳ��һ������
void CMemPoolMgr::RemovePool(CMemPool* pPool){
	assert(pPool);
	if(!pPool){
		return;
	}

	m_mtxPoolsGuard.Lock();

	if(pPool->GetParent()){
		//�����������ֵܳ��еĵ�һ������Ҫ�޸ĸ��ص��ӳ�ָ�룬ʹ��ָ����һ���ֵܳ�
		if(!pPool->GetPrev()){
			pPool->GetParent()->SetChild(pPool->GetNext());
		}

		pPool->SetParent(NULL);
	}

	if(m_pTopLevelPools==pPool){
		m_pTopLevelPools=pPool->GetNext();
	}
	
	pPool->RemoveOut();

	m_mtxPoolsGuard.UnLock();
}

void CMemPoolMgr::DestroyPool(CMemPool* pPool){
	assert(pPool);
	if(!pPool){
		return;
	}

	//���شӳ������Ƴ�
	RemovePool(pPool);
	pPool->Destroy();
}

void CMemPoolMgr::DestroyAllPools(){
	m_mtxPoolsGuard.Lock();
	
	while(m_pTopLevelPools && m_pTopLevelPools->GetNext()){
		m_pTopLevelPools=m_pTopLevelPools->GetNext();
		m_pTopLevelPools->GetPrev()->Destroy();
	}
	
	if(m_pTopLevelPools){
		m_pTopLevelPools->Destroy();
		m_pTopLevelPools=NULL;
	}

	m_mtxPoolsGuard.UnLock();
}

void CMemPoolMgr::FreePool(CMemPool* pPool){
	assert(pPool);
	if(!pPool){
		return;
	}

	//���شӳ������Ƴ�
	RemovePool(pPool);
	pPool->FreePool();
}

void CMemPoolMgr::FreeAllPools(){
	m_mtxPoolsGuard.Lock();
	//rick:debug
//	dbout("thread[%d]:FreeAllPools...",pthread_self());
	
	while(m_pTopLevelPools && m_pTopLevelPools->GetNext()){
		m_pTopLevelPools=m_pTopLevelPools->GetNext();
		m_pTopLevelPools->GetPrev()->FreePool();
	}
	
	if(m_pTopLevelPools){
		m_pTopLevelPools->FreePool();
		m_pTopLevelPools=NULL;
	}

	//rick:debug
//	dbout("thread[%d]:FreeAllPools ok...",pthread_self());
	m_mtxPoolsGuard.UnLock();
}

/*
	�����ڴ�أ����������
*/
CMemPool* CMemPoolMgr::CreatePool(CMemPool* pParent, SIZE initsize){
	
	CMemPool* pPool=new(initsize) CMemPool;
	AddPool(pParent,pPool);

	return pPool;
}
/*--------------------------------------------------------------------*/

/*=====================
	�ڴ����ʵ�ִ���
=====================*/

CMemPool::CMemPool(){
	//��ʼ���ڲ���ݽṹ
	assert(m_pBlks);
	m_pLastBlk=m_pBlks;
	m_pNext=m_pPrev=m_pParent=m_pChild=NULL;
	m_bFree=false;
//	dbout("**************Availsize = %d",m_pLastBlk->GetAvailSize());
}

CMemPool::~CMemPool(){
}

/*�ڴ���������----------------------------------------------------------------*/
/*
	����AllocBlk
	���ڴ�ع���������һ���ڴ棬����ڴ�ع�����û�к��ʵ��ڴ�飬�򴴽�һ���µ�
	�ڴ�飨�����ϵͳ�����ڴ棩
*/
CBlkHdr* CMemPool::AllocBlk(SIZE size){
//	dbout("**************AllocBlk dwGrainCount=%d",dwGrainCount);
//	dbout("--AllocBlk: Free blks count: %d",g_mpm.GetFreeBlkCount());
	//���ڴ�ع���������
	DWORD dwGrain=GrainAlign(size+sizeof(CBlkHdr));
	CBlkHdr* pblk=g_mpm.AllocBlkFromFreeChain(dwGrain);
	//���ڴ�ع�����û�к��ʵĿ����ڴ�飬�򴴽��µ��ڴ��
	if(!pblk){
//		dbout("----No comptible blk, create new blk...%s",NULL);
		pblk=new(size) CBlkHdr;
	}
//	assert((pblk->DebugDump(),true));	
	return pblk;
}
/*
	����ReturnBlks
	��ָ���ڴ�鼰�����齻�ظ��ڴ�ع�����
*/
void CMemPool::ReturnBlks(CBlkHdr* pBlks){
	if(!pBlks){
		return;
	}

	if(pBlks->GetPrev()){
		pBlks->GetPrev()->SetNext(NULL);
	}

	pBlks->SetPrev(NULL);
	g_mpm.ReturnBlksToFreeChain(pBlks);
//	dbout("--ReturnBlk: Free blks count: %d",g_mpm.GetFreeBlkCount());
}
/*
	����FreeBlks
	��ָ���ڴ�鼰�������ͷţ����ز���ϵͳ��
*/
void CMemPool::FreeBlks(CBlkHdr* pBlks){
	if(!pBlks){
		return;
	}

	if(pBlks->GetPrev()){
		pBlks->GetPrev()->SetNext(NULL);
	}

	while(pBlks->GetNext()){
		pBlks=pBlks->GetNext();
		delete pBlks->GetPrev();
	}
	delete pBlks;
}

/*
	����AppendBlk
	�򱾳�׷��ָ����С��һ���ڴ��
*/
CBlkHdr* CMemPool::AppendBlk(SIZE size){
	if(!size){
		return NULL;
	}

	CBlkHdr* pblk=AllocBlk(size);

	if(pblk){
		pblk->InsertAfter(m_pLastBlk);
		m_pLastBlk=pblk;
	}

	return pblk;
}
/*-----------------------------------------------------------------------------*/
/*
	�ڴ�ز���
*/
void* CMemPool::operator new(SIZE size){
	assert(size);
	PVOID pv=NULL;
	CBlkHdr* pblk=AllocBlk(size);
	assert(pblk);
    if(!pblk){
    	throw(ERR_NOT_ENOUGH_MEMORY);
    }
    else{
    	pv=pblk->Alloc(sizeof(CMemPool));
    	assert(pv);
    	((CMemPool*)pv)->m_pBlks=pblk;
    }
	return pv;
}

void* CMemPool::operator new(SIZE size, SIZE initsize){
	assert(size);
	return operator new(size+initsize);
}

void CMemPool::operator delete(void* pv){
	if(!pv){
		return;
	}
	if(((CMemPool*)pv)->m_bFree){
		FreeBlks(((CMemPool*)pv)->m_pBlks);
	}
	else{
		ReturnBlks(((CMemPool*)pv)->m_pBlks);
	}
}

inline CMemPool* CMemPool::GetChild() const{
	return m_pChild;
}

inline CMemPool* CMemPool::GetParent() const{
	return m_pParent;
}

inline CMemPool* CMemPool::GetPrev() const{
	return m_pPrev;
}

inline CMemPool* CMemPool::GetNext() const{
	return m_pNext;
}

inline void CMemPool::SetChild(CMemPool* pChild){
	m_pChild=pChild;
}

inline void CMemPool::SetParent(CMemPool* pParent){
	m_pParent=pParent;
}

inline void CMemPool::SetPrev(CMemPool* pPrev){
	m_pPrev=pPrev;
}

inline void CMemPool::SetNext(CMemPool* pNext){
	m_pNext=pNext;
}

inline void CMemPool::InsertAfter(CMemPool* pBefore){
	if(pBefore){
		m_pPrev=pBefore;
		m_pNext=pBefore->GetNext();

		pBefore->SetNext(this);

		if(m_pNext){
			m_pNext->SetPrev(this);
		}
	}
	else{
		m_pPrev=NULL;
		m_pNext=NULL;
	}
}

inline void CMemPool::InsertBefore(CMemPool* pAfter){
	if(pAfter){
		m_pPrev=pAfter->GetPrev();
		m_pNext=pAfter;

		if(m_pPrev){
			m_pPrev->SetNext(this);
		}

		pAfter->SetPrev(this);
	}
	else{
		m_pPrev=NULL;
		m_pNext=NULL;
	}
}

inline void CMemPool::RemoveOut(){
	if(m_pPrev){
		m_pPrev->SetNext(m_pNext);
	}
	if(m_pNext){
		m_pNext->SetPrev(m_pPrev);
	}

	m_pPrev=m_pNext=NULL;
}

void CMemPool::Empty(){
	CBlkHdr* p=NULL;

	m_mtxBlksGuard.Lock();

	if(p=m_pBlks->GetNext()){
		m_pLastBlk=m_pBlks;
	}
	m_pBlks->Free((char*)this+sizeof(CMemPool));

//	dbout("**************Availsize = %d",m_pLastBlk->GetAvailSize());
	m_mtxBlksGuard.UnLock();

	if(p){
		ReturnBlks(p);
	}
}

void CMemPool::Destroy(){
	m_mtxBlksGuard.Lock();
	
	m_pLastBlk=NULL;

	m_mtxBlksGuard.UnLock();

	//�ڹ�����Գ������������
	if(m_pChild){
		CMemPool* pnext=NULL;
		while(pnext=m_pChild->GetNext()){
			pnext->RemoveOut();
			pnext->Destroy();
		}
		m_pChild->Destroy();
	}

/*
	��delete���������������ջ��ͷ��ڴ����Ϊ�˱�֤
	������ͷ��ڴ��ʱ����������ȷ����
*/
	m_bFree=false;
	delete this;
}

void CMemPool::FreePool(){
	m_mtxBlksGuard.Lock();
	
	m_pLastBlk=NULL;

	m_mtxBlksGuard.UnLock();

	//�ڹ������ɾ�����������
	if(m_pChild){
		CMemPool* pnext=NULL;
		while(pnext=m_pChild->GetNext()){
			pnext->RemoveOut();
			pnext->FreePool();
		}
		m_pChild->FreePool();
	}
/*
	��delete���������������ջ��ͷ��ڴ����Ϊ�˱�֤
	������ͷ��ڴ��ʱ����������ȷ����
*/
	m_bFree=true;
	delete this;
}

PVOID CMemPool::Alloc(SIZE size){
	if(!size){
		return NULL;
	}
	m_mtxBlksGuard.Lock();
	
	PVOID pv=NULL;
	if(m_pLastBlk){
		if(!m_pLastBlk->IsFit(size)){
			AppendBlk(size);
		}
		pv=m_pLastBlk->Alloc(size);
		assert(m_pLastBlk->IsAddressInBlk((char*)pv+size-1));
	}

	m_mtxBlksGuard.UnLock();
//	dbout("**************Alloced pv = 0x%08x",pv);
//	dbout("**************Availsize = %d",m_pLastBlk->GetAvailSize());
	return pv;
}

PVOID CMemPool::Alloc(SIZE size, UCHAR ucInitVal){
	PVOID pv=Alloc(size);
	if(pv){
		memset(pv,ucInitVal,size);
	}
	return pv;
}

void CMemPool::Free(PVOID pv){
	if(!pv){
		return;
	}
	m_mtxBlksGuard.Lock();

	CBlkHdr* pCut=NULL;
	CBlkHdr* pblk=m_pLastBlk;
	while(pblk){
		if(pblk->IsAddressInBlk(pv)){
			//�������ͷŵĵ�ַ���ڴ�ص��׿��ڣ�Ӧ����Ƿ���ƻ��ڴ��
			if(pblk==m_pBlks){
				assert((DWORD)pv>=(DWORD)pblk+sizeof(CMemPool));
				//���ڴ�ر��ƻ����׳��쳣
				if(!((DWORD)pv >= (DWORD)pblk+sizeof(CMemPool))){
					throw(ERR_POOL_DAMAGED);		//�ر��ƻ�
					break;
				}
			}
			
			pblk->Free(pv);							//�����ڴ�

			//�ҵ�ָ�����ڵĿ飬�ÿ���Ϊ�ڴ���ڿ��������һ�飬��������
			//�齫������
			//pCut=pblk->GetNext();
pCut=pblk->GetPrev();
			m_pLastBlk=pblk;
			pblk->SetNext(NULL);

			break;
		}
		pblk=pblk->GetPrev();
	}

//	dbout("**************Availsize = %d",m_pLastBlk->GetAvailSize());
	m_mtxBlksGuard.UnLock();

	if(pCut){
		ReturnBlks(pCut);
	}
}

//end of file
