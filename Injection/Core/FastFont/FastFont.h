#pragma once

#include <windows.h>
#include "CacheInfo.h"

typedef bool (*funcFFDATA2PIC)( VOID* , DWORD );

class CFastFont
{
private:
	typedef struct StOutlineFontData{
		GLYPHMETRICS GM;
		BYTE         Image[1];
	}StOutlineFontData;
public:

	CFastFont();
	virtual ~CFastFont(void);
	bool CreateFastFont(LOGFONT *lplf,
		int OutLineFormat = GGO_BITMAP,
		int HashDivideNums = 64);

	void ClearCache(void);
	StOutlineFontData *GetFontData(int code,SIZE *size = NULL);

	void BltFontData(int code,int x,int y,SIZE *size);

	int  DebugGetHashEntrys(int hashtableno);


	//typedef int (WINAPI *PFNMESSAGEBOXA)(HWND, PCSTR, PCSTR, UINT);
//	typedef bool (*funcFFDATA2PIC)( VOID* , DWORD );
	void SetBltStatus(void *dist,DWORD pitch,DWORD bits,int mode,funcFFDATA2PIC func);

	void GetMaxSize(SIZE *size);

	void test(int mode);

private:
	HDC     m_hDC;
	HFONT   m_hOldFont;
	TEXTMETRIC m_TM;

	int m_DefaultBufferSize;
	int m_OutLineFormat;
	int m_HashDivideNums;
	int m_BltAAMode;
	HFONT m_hFont;
	BYTE      *m_pDefaultBuffer;
	CacheInfo *m_pCacheInfo;

	funcFFDATA2PIC m_Data2PicFunc;
	BYTE  *m_DistPtr;
	DWORD m_DistPitch;
	DWORD m_DistBits;
};
