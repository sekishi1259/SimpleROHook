#include "stdafx.h"

#include "CacheInfo.h"

//CacheInfo::CacheInfo()
//{
//	m_HashTopTables = 0;
//	m_CacheNums = 0;
//
//	m_pCacheTop = NULL;
//	m_pHashTopTable = NULL;
//}
CacheInfo::CacheInfo(int HashRootTables)
{

	m_HashRootTables = HashRootTables;
	m_CacheNums = 0;

	m_CacheRoot.OriginalKey = 0;
	m_CacheRoot.Value       = 0;
	m_CacheRoot.pData       = NULL;
	m_CacheRoot.pPrev       = NULL;
	m_CacheRoot.pNext       = NULL;
	m_CacheRoot.pPrevHash   = NULL;
	m_CacheRoot.pNextHash   = NULL;


	m_HashRootTable = new StCacheInfo[m_HashRootTables];
	for(int ii = 0; ii < m_HashRootTables;ii++){
		m_HashRootTable[ii].OriginalKey = 0;
		m_HashRootTable[ii].Value       = 0;
		m_HashRootTable[ii].pData       = NULL;
		m_HashRootTable[ii].pPrev       = NULL;
		m_HashRootTable[ii].pNext       = NULL;
		m_HashRootTable[ii].pPrevHash   = NULL;
		m_HashRootTable[ii].pNextHash   = NULL;
	}


}

CacheInfo::~CacheInfo(void)
{
	StCacheInfo *pCache = m_CacheRoot.pNext;

	while(pCache)
	{
		StCacheInfo *pNext = pCache->pNext;
		//
		delete[] pCache->pData;
		delete   pCache;
		pCache = pNext;
	}
	m_CacheRoot.pNext = NULL;
	delete[] m_HashRootTable;
}


void *CacheInfo::CreateData(int hashkey,int datasize)
{
	StCacheInfo *pCache;

	if( m_CacheNums >= 256 )
	{
		StCacheInfo *pNext = 0;
		pCache = m_CacheRoot.pNext;
		while(pCache)
		{
			pNext = pCache->pNext;
			//
			if( !pNext ){
				// もっとも使われていない
				// キャッシュの終端から削除
				StCacheInfo *pPrev = pCache->pPrev;
				if(pPrev)pPrev->pNext = pNext;
				//
				StCacheInfo *pPrevHash = pCache->pPrevHash;
				StCacheInfo *pNextHash = pCache->pNextHash;

				if(pPrevHash)pPrevHash->pNextHash = pNextHash;
				if(pNextHash)pNextHash->pPrevHash = pPrevHash;
				//
				delete[] pCache->pData;
				delete   pCache;
				m_CacheNums--;
				//
			}
			pCache = pNext;
		}
	}

	pCache = new StCacheInfo;
	if(pCache){
		pCache->OriginalKey = hashkey;
		pCache->Value = 0;
		pCache->pData = new u8[datasize];
		if(pCache->pData){
			//
			int hash = hashkey % m_HashRootTables;
			//
			StCacheInfo *pNext     = m_CacheRoot.pNext;
			StCacheInfo *pNextHash = m_HashRootTable[hash].pNextHash;
			//
			m_CacheRoot.pNext               = pCache;
			m_HashRootTable[hash].pNextHash = pCache;
			pCache->pPrev     = &m_CacheRoot;
			pCache->pNext     = pNext;
			pCache->pPrevHash = &m_HashRootTable[hash];
			pCache->pNextHash = pNextHash;

			if(pNext)
				pNext->pPrev = pCache;
			if(pNextHash)
				pNextHash->pPrevHash = pCache;


			m_CacheNums++;
			return pCache->pData;
		}
		delete pCache;
	}
	return NULL;
}

void CacheInfo::ClearCache(void)
{
	StCacheInfo *pCache = m_CacheRoot.pNext;

	while(pCache)
	{
		StCacheInfo *pNext = pCache->pNext;
		//
		delete[] pCache->pData;
		delete   pCache;
		pCache = pNext;
	}
	m_CacheRoot.pNext = NULL;

	for(int ii = 0; ii < m_HashRootTables;ii++){
		m_HashRootTable[ii].OriginalKey = 0;
		m_HashRootTable[ii].Value       = 0;
		m_HashRootTable[ii].pData       = NULL;
		m_HashRootTable[ii].pPrev       = NULL;
		m_HashRootTable[ii].pNext       = NULL;
		m_HashRootTable[ii].pPrevHash   = NULL;
		m_HashRootTable[ii].pNextHash   = NULL;
	}

	m_CacheNums = 0;
}

void *CacheInfo::GetCacheData(int hashkey)
{
	int hash = hashkey % m_HashRootTables;

	StCacheInfo *pCache = m_HashRootTable[hash].pNextHash;

	while(pCache)
	{
		if( pCache->OriginalKey == hashkey ){
			// 参照されたキャッシュは最上位に繰り上がる
			StCacheInfo *pPrev = pCache->pPrev;
			StCacheInfo *pNext = pCache->pNext;
			//
			if(pPrev)pPrev->pNext = pNext;
			if(pNext)pNext->pPrev = pPrev;
			//
			pNext = m_CacheRoot.pNext;
			pCache->pPrev     = &m_CacheRoot;
			pCache->pNext     = pNext;
			m_CacheRoot.pNext = pCache;
			if(pNext)pNext->pPrev = pCache;

			return pCache->pData;
		}
		pCache = pCache->pNextHash;
	}
	return NULL;

}

int  CacheInfo::DebugGetHashEntrys(int hashtableno)
{
	int hashentrys = 0;
	if( hashtableno >= m_HashRootTables)return 0;

	StCacheInfo *pCache = m_HashRootTable[hashtableno].pNextHash;

	while(pCache)
	{
		hashentrys++;
		pCache = pCache->pNextHash;
	}
	return hashentrys;
}

