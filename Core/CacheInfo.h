#pragma once

typedef long            s32;
typedef unsigned long   u32;
typedef short           s16;
typedef unsigned short  u16;
typedef char            s8;
typedef unsigned char   u8;

#ifndef NULL
#define NULL 0
#endif

class CacheInfo
{
	typedef struct StCacheInfo{
		u32   OriginalKey;
		u32   Value;
		void  *pData;
		//
		StCacheInfo *pPrev;
		StCacheInfo *pNext;
		//
		StCacheInfo *pPrevHash;
		StCacheInfo *pNextHash;
	}StCacheInfo;
public:
	//CacheInfo();
	CacheInfo(int HashTopTables);
	virtual ~CacheInfo(void);

	void ClearCache(void);
	void *CreateData(int hashkey,int datasize);
	void *GetCacheData(int hashkey);

	int  DebugGetHashEntrys(int hashtableno);

private:
	int         m_CacheNums;
	int         m_HashRootTables;

	StCacheInfo m_CacheRoot;
	StCacheInfo *m_HashRootTable;

};
