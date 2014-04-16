#pragma once

#include <windows.h>

//#define D3D_OVERLOADS
#include <d3d.h>
#include <d3dtypes.h>


#include "FastFont.h"
//d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,D3DFVF_TLVERTEX,vp,4,0);

class CSFastFont
{
private:
	/// 頂点関係 ///
//	#define SFF_FVF_CUSTOM ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 ) // 座標変換済み頂点
//
//	struct SFF_CUSTOMVERTEX{
//	   float x,y,z; // 頂点座標 
//	   float rhw; // 除算数
//	   DWORD dwColor; // 頂点の色
//	   float u, v; //　テクスチャ座標 
//	};

	typedef struct StSFontCacheInfo{
		DWORD OriginalKey;
		DWORD u,v,w,h;
		//
		StSFontCacheInfo *pPrev;
		StSFontCacheInfo *pNext;
		//
		StSFontCacheInfo *pPrevHash;
		StSFontCacheInfo *pNextHash;
		//
		StSFontCacheInfo *pNextOrder;
	}StSFontCacheInfo;

public:
	CSFastFont(void);
	virtual ~CSFastFont(void);

	bool CreateFastFont(LOGFONT *lplf,LPDIRECT3DDEVICE7 pDev,LPDIRECTDRAWSURFACE7 pTex,int imagemode = 0);

	StSFontCacheInfo *GetCacheData(int hashkey);

	bool CreateFontImageOrder(int fontcode,StSFontCacheInfo *pCache);
	void Flush(void);

	void DrawChar(int fontcode,int x,int y,DWORD FontColor,SIZE *pSize);
	void DrawText(LPSTR str,int x,int y,DWORD FontColor,int Arrange,SIZE *pSize);

	void DrawCharSC(int fontcode,int x,int y,float scx,float scy,DWORD FontColor,SIZE *pSize);
	void DrawTextSC(LPSTR str,int x,int y,float scx,float scy,DWORD FontColor,int Arrange,SIZE *pSize);
	void GetDrawSize(LPSTR str,float scx,float scy,SIZE *pSize);

	static bool SFontBlt16(void *pData,DWORD alpha);
	static bool SFontBlt16Black(void *pData,DWORD alpha);

private:
#define defSSF_VERTEX_MAX (65536*6)
	D3DTLVERTEX m_SFF_Vertex[defSSF_VERTEX_MAX];
	int m_SSF_Vertex_index;

	CFastFont *m_pFastFont;
	int m_HashRootTables;
	StSFontCacheInfo *m_HashRootTable;
	StSFontCacheInfo *m_pSFontChacheInfo;
	//
	StSFontCacheInfo *m_pLastSFontChacheInfo;
	StSFontCacheInfo *m_pLastSFontChacheInfoFirstUsed;
	//
	StSFontCacheInfo *m_pCreateOrderInfo;

	IDirect3DDevice7    *m_d3ddev;
	IDirectDrawSurface7 *m_pTex;
	//LPDIRECT3DTEXTURE9 m_pTex;

	int m_ImageMode;

	int m_MaxImageWidth;
	int m_MaxImageHeight;

	int m_MaxFontWidth;
	int m_MaxFontHeight;

	int m_WidthCount;
	int m_HeightCount;
	int m_MaxCacheNums;

};
