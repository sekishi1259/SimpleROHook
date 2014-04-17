#include "stdafx.h"
#include "SFastFont.h"
#include "tchar.h"

//#define defDebugLog

#ifndef  defDebugLog
#define DPRINTF(a)
#else
#include <stdio.h>
#define DPRINTF(a) OutputDebugFile a
static void OutputDebugFile(LPCSTR format, ...);
static void OutputDebugFile(LPCSTR format, ...)
{
	char   s[512];
	va_list args;

	va_start(args, format);

	vsprintf(s, format, args);
	
	{
		char logfilename[2048];
		sprintf(logfilename,"SFastFont.log");
		FILE* Stream = fopen(logfilename, "at");
		if(Stream){
			fprintf(Stream, s);
			fclose(Stream);
		}
	}

	va_end(args);
}

#endif

CSFastFont::CSFastFont(void)
{
	m_MaxImageWidth  = 256;
	m_MaxImageHeight = 256;

	m_MaxFontWidth  = 0;
	m_MaxFontHeight = 0;

	m_WidthCount  = 0;
	m_HeightCount = 0;
	m_MaxCacheNums = 0;

	m_HashRootTable = NULL;
	m_pSFontChacheInfo = NULL;

	m_pLastSFontChacheInfo = NULL;
	m_pCreateOrderInfo = NULL;

	m_HashRootTables = 0;

	m_pTex = NULL;
	m_pLastSFontChacheInfoFirstUsed = NULL;
	m_SSF_Vertex_index = 0;

	m_pFastFont = new CFastFont;
	m_ImageMode = 0;
}

CSFastFont::~CSFastFont(void)
{
	if(m_pSFontChacheInfo){
		delete m_pSFontChacheInfo;
		m_pSFontChacheInfo = NULL;
	}
	if(m_HashRootTable){
		delete m_HashRootTable;
		m_HashRootTable = NULL;
	}

	if(m_pFastFont){
		delete m_pFastFont;
		m_pFastFont = NULL;
	}
}


bool CSFastFont::CreateFastFont(LOGFONT *lplf,LPDIRECT3DDEVICE7 pDev,LPDIRECTDRAWSURFACE7 pTex,int imagemode)
{
	m_ImageMode = imagemode;
	if( m_pFastFont->CreateFastFont(lplf) )
	{
	//	m_MaxImageWidth  = 256;
	//	m_MaxImageHeight = 256;
		m_MaxImageWidth  = 512;
		m_MaxImageHeight = 512;
		m_pTex   = pTex;
		m_d3ddev = pDev;
		
		SIZE size;
		m_pFastFont->GetMaxSize(&size);
		m_MaxFontWidth  = size.cx +2 + imagemode;
		m_MaxFontHeight = size.cy +2 + imagemode;
		
		m_WidthCount  = m_MaxImageWidth  / m_MaxFontWidth;
		m_HeightCount = m_MaxImageHeight / m_MaxFontHeight;
		m_MaxCacheNums = m_WidthCount * m_HeightCount;
		//m_MaxCacheNums = 7;
		
		// root
		m_pSFontChacheInfo = new StSFontCacheInfo[m_MaxCacheNums + 1];
		memset( m_pSFontChacheInfo,0,sizeof(StSFontCacheInfo)*(m_MaxCacheNums + 1) );
		// cache root
		m_HashRootTables = 64;
		m_HashRootTable = new StSFontCacheInfo[m_HashRootTables];
		memset( m_HashRootTable,0,sizeof(StSFontCacheInfo)*m_HashRootTables );
		
		// create a root(dummy)
		m_pSFontChacheInfo[0].pNext = &m_pSFontChacheInfo[1];
		for(int ii = 0;ii < m_MaxCacheNums;ii++){
			int index = ii + 1;
			m_pSFontChacheInfo[index].pPrev = &m_pSFontChacheInfo[index-1];
			if( ii < (m_MaxCacheNums-1) )
				m_pSFontChacheInfo[index].pNext = &m_pSFontChacheInfo[index+1];
			//
			m_pSFontChacheInfo[index].u = (ii % m_WidthCount) * m_MaxFontWidth;
			m_pSFontChacheInfo[index].v = (ii / m_WidthCount) * m_MaxFontHeight;
		}
		// m_pSFontChacheInfo[n].pNext = NULL  is disused cache.
		// 
		m_pLastSFontChacheInfo = &m_pSFontChacheInfo[m_MaxCacheNums];

		return true;
	}
	return false;
}

void CSFastFont::DrawChar(int fontcode,int x,int y,DWORD FontColor,SIZE *pSize)
{
	StSFontCacheInfo *pCache;
	DPRINTF( ("%08X:fontcode=%08X\n",this,fontcode) );
	pCache = GetCacheData(fontcode);

	pSize->cx = pCache->w;
	pSize->cy = pCache->h;

	D3DTLVERTEX v[]=
	{
	  { (float)x                    , (float)y                    , 0.0f, 1.0f, FontColor,  0, ((pCache->u            )+0.5f)/m_MaxImageWidth, ((pCache->v            )+0.5f)/m_MaxImageHeight},
	  { (float)(x+(int)pCache->w-1) , (float)y                    , 0.0f, 1.0f, FontColor,  0, ((pCache->u+pCache->w-1)+0.5f)/m_MaxImageWidth, ((pCache->v            )+0.5f)/m_MaxImageHeight}, 
	  { (float)x                    , (float)(y+(int)pCache->h-1) , 0.0f, 1.0f, FontColor,  0, ((pCache->u            )+0.5f)/m_MaxImageWidth, ((pCache->v+pCache->h-1)+0.5f)/m_MaxImageHeight}, 

	  { (float)(x+(int)pCache->w-1) , (float)y                    , 0.0f, 1.0f, FontColor,  0, ((pCache->u+pCache->w-1)+0.5f)/m_MaxImageWidth, ((pCache->v            )+0.5f)/m_MaxImageHeight}, 
	  { (float)(x+(int)pCache->w-1) , (float)(y+(int)pCache->h-1) , 0.0f, 1.0f, FontColor,  0, ((pCache->u+pCache->w-1)+0.5f)/m_MaxImageWidth, ((pCache->v+pCache->h-1)+0.5f)/m_MaxImageHeight},
	  { (float)x                    , (float)(y+(int)pCache->h-1) , 0.0f, 1.0f, FontColor,  0, ((pCache->u            )+0.5f)/m_MaxImageWidth, ((pCache->v+pCache->h-1)+0.5f)/m_MaxImageHeight}, 
	};

	if( defSSF_VERTEX_MAX > m_SSF_Vertex_index ){
		memcpy( &m_SFF_Vertex[m_SSF_Vertex_index],v,6*sizeof(D3DTLVERTEX) );
		m_SSF_Vertex_index+=6;
	}
}

void CSFastFont::DrawText(LPSTR str,int x,int y,DWORD FontColor,int Arrange,SIZE *pSize)
{
	TCHAR *c = str;
	int code;
	int xx;
	int yy;
	SIZE size;

//Arrange
	{
		TCHAR *c = str;
		int w,h;
		w = 0;
		h = 0;
		size.cx = 0;
		size.cy = 0;
		while( c[0]!=_T('\0') ){
			SIZE tempsize;
#if _UNICODE
			code = (int)*c++;
#else
			if(IsDBCSLeadByte(*c)){
				code = (BYTE)c[0]<<8 | (BYTE)c[1];
				c+=2;
			}else{
				code = (BYTE)(*c++);
			}
#endif
			if( code == _T('\n') ){
				w = 0;
			}
			if( m_pFastFont->GetFontData(code,&tempsize) ){
				w += tempsize.cx +1;
				if( size.cx < w )size.cx = w;
				if( size.cy < h+tempsize.cy+1 )
					size.cy = h+tempsize.cy+1;
			}
		}
	}
	switch(Arrange){
	case 0:
		break;
	case 1:
		//Right sort
		x -= size.cx;
		break;
	case 2:
		//Center sort
		x -= size.cx/2;
		break;
	}

	xx = x;
	yy = y;

	size.cx = x;
	size.cy = y;

	c = str;
	while( c[0]!=_T('\0') ){
		SIZE tempsize;
#if _UNICODE
		code = (int)*c++;
#else
		if(IsDBCSLeadByte(*c)){
			code = (BYTE)c[0]<<8 | (BYTE)c[1];
			c+=2;
		}else{
			code = (BYTE)(*c++);
		}
#endif
		if( code == _T('\n') ){
			xx = x;
			yy += m_MaxFontHeight-1;
		}else{
			DrawChar(code,xx,yy,FontColor,&tempsize);
			xx += tempsize.cx-1;
			if( size.cx < xx )size.cx = xx;
			if( size.cy < yy+tempsize.cy-1 )size.cy = yy+tempsize.cy-1;
		}
	}
	size.cx -= x;
	size.cy -= y;

	if(pSize){
		*pSize = size;
	}
}


void CSFastFont::DrawCharSC(int fontcode,int x,int y,float scx,float scy,DWORD FontColor,SIZE *pSize)
{
	StSFontCacheInfo *pCache;
	DPRINTF( ("%08X:fontcode=%08X\n",this,fontcode) );
	pCache = GetCacheData(fontcode);

	float w,h;
	w = (int)pCache->w * scx;
	h = (int)pCache->h * scy;

	pSize->cx = (LONG)w;
	pSize->cy = (LONG)h;

	w = (int)(pCache->w-1) * scx;
	h = (int)(pCache->h-1) * scy;

	D3DTLVERTEX v[]=
	{
	  { (float)x     , (float)y     , 0.0f, 1.0f, FontColor,   0, ((pCache->u            )+0.5f)/m_MaxImageWidth, ((pCache->v            )+0.5f)/m_MaxImageHeight},
	  { (float)(x+w) , (float)y     , 0.0f, 1.0f, FontColor,   0, ((pCache->u+pCache->w-1)+0.5f)/m_MaxImageWidth, ((pCache->v            )+0.5f)/m_MaxImageHeight}, 
	  { (float)x     , (float)(y+h) , 0.0f, 1.0f, FontColor,   0, ((pCache->u            )+0.5f)/m_MaxImageWidth, ((pCache->v+pCache->h-1)+0.5f)/m_MaxImageHeight}, 

	  { (float)(x+w) , (float)y     , 0.0f, 1.0f, FontColor,   0, ((pCache->u+pCache->w-1)+0.5f)/m_MaxImageWidth, ((pCache->v            )+0.5f)/m_MaxImageHeight}, 
	  { (float)(x+w) , (float)(y+h) , 0.0f, 1.0f, FontColor,   0, ((pCache->u+pCache->w-1)+0.5f)/m_MaxImageWidth, ((pCache->v+pCache->h-1)+0.5f)/m_MaxImageHeight},
	  { (float)x     , (float)(y+h) , 0.0f, 1.0f, FontColor,   0, ((pCache->u            )+0.5f)/m_MaxImageWidth, ((pCache->v+pCache->h-1)+0.5f)/m_MaxImageHeight}, 
	};

	if( (defSSF_VERTEX_MAX-1) > m_SSF_Vertex_index ){
		memcpy( &m_SFF_Vertex[m_SSF_Vertex_index],v,6*sizeof(D3DTLVERTEX) );
		m_SSF_Vertex_index+=6;
	}
}
void CSFastFont::GetDrawSize(LPSTR str,float scx,float scy,SIZE *pSize)
{
	TCHAR *c = str;
	int code;
	SIZE size;
	{
		TCHAR *c = str;
		int w,h;
		w = 0;
		h = 0;
		size.cx = 0;
		size.cy = 0;
		while( c[0]!=_T('\0') ){
			SIZE tempsize;
#if _UNICODE
			code = (int)*c++;
#else
			if(IsDBCSLeadByte(*c)){
				code = (BYTE)c[0]<<8 | (BYTE)c[1];
				c+=2;
			}else{
				code = (BYTE)(*c++);
			}
#endif
			if( code == _T('\n') ){
				w = 0;
			}
			if( m_pFastFont->GetFontData(code,&tempsize) ){
				tempsize.cx = (LONG)(tempsize.cx * scx);
				tempsize.cy = (LONG)(tempsize.cy * scy);
				w += tempsize.cx +1;
				if( size.cx < w )size.cx = w;
				if( size.cy < h+tempsize.cy+1 )
					size.cy = h+tempsize.cy+1;
			}
		}
	}
	if(pSize){
		*pSize = size;
	}
}

void CSFastFont::DrawTextSC(LPSTR str,int x,int y,float scx,float scy,DWORD FontColor,int Arrange,SIZE *pSize)
{
	TCHAR *c = str;
	int code;
	int xx;
	int yy;
	SIZE size;

//Arrange
	{
		TCHAR *c = str;
		int w,h;
		w = 0;
		h = 0;
		size.cx = 0;
		size.cy = 0;
		while( c[0]!=_T('\0') ){
			SIZE tempsize;
#if _UNICODE
			code = (int)*c++;
#else
			if(IsDBCSLeadByte(*c)){
				code = (BYTE)c[0]<<8 | (BYTE)c[1];
				c+=2;
			}else{
				code = (BYTE)(*c++);
			}
#endif
			if( code == _T('\n') ){
				w = 0;
			}
			if( m_pFastFont->GetFontData(code,&tempsize) ){
				tempsize.cx = (LONG)(tempsize.cx * scx);
				tempsize.cy = (LONG)(tempsize.cy * scy);
				w += tempsize.cx +1;
				if( size.cx < w )size.cx = w;
				if( size.cy < h+tempsize.cy+1 )
					size.cy = h+tempsize.cy+1;
			}
		}
	}
	switch(Arrange){
	case 0:
		break;
	case 1:
		//Right sort
		x -= size.cx;
		break;
		//Center sort
	case 2:
		x -= size.cx/2;
		break;
	}

	xx = x;
	yy = y;

	size.cx = x;
	size.cy = y;

	c = str;
	while( c[0]!=_T('\0') ){
		SIZE tempsize;
#if _UNICODE
		code = (int)*c++;
#else
		if(IsDBCSLeadByte(*c)){
			code = (BYTE)c[0]<<8 | (BYTE)c[1];
			c+=2;
		}else{
			code = (BYTE)(*c++);
		}
#endif
		if( code == _T('\n') ){
			xx = x;
			yy += m_MaxFontHeight-1;
		}else{
			DrawCharSC(code,xx,yy,scx,scy,FontColor,&tempsize);
			xx += tempsize.cx-1;
			if( size.cx < xx )size.cx = xx;
			if( size.cy < yy+tempsize.cy-1 )size.cy = yy+tempsize.cy-1;
		}
	}
	size.cx -= x;
	size.cy -= y;

	if(pSize){
		*pSize = size;
	}
}




CSFastFont::StSFontCacheInfo *CSFastFont::GetCacheData(int hashkey)
{
	int hash = hashkey % m_HashRootTables;

	StSFontCacheInfo *pCache = m_HashRootTable[hash].pNextHash;

	while(pCache)
	{
		if( pCache->OriginalKey == hashkey ){
			// 参照されたキャッシュは最上位に繰り上がる
			StSFontCacheInfo *pPrev = pCache->pPrev;
			StSFontCacheInfo *pNext = pCache->pNext;
			//
			if(pPrev)pPrev->pNext = pNext;
			if(pNext){
				pNext->pPrev = pPrev;
			}else{
				m_pLastSFontChacheInfo = pPrev;
			}
			if( m_pLastSFontChacheInfoFirstUsed == pCache ){
				// 最も使われていないハッシュキーの処理
				if(&m_pSFontChacheInfo[0] == pPrev){
					m_pLastSFontChacheInfoFirstUsed = pCache;
				}else{
					m_pLastSFontChacheInfoFirstUsed = pPrev;
				}
			}
			//
			pNext = m_pSFontChacheInfo[0].pNext;
			pCache->pPrev     = &m_pSFontChacheInfo[0];
			pCache->pNext     = pNext;
			m_pSFontChacheInfo[0].pNext = pCache;
			if(pNext)pNext->pPrev = pCache;
			return pCache;
		}
		pCache = pCache->pNextHash;
	}
	
	// new data;
	{
//
// 問題
// キャッシュ終端を再利用した場合
// オーダーポインタがリング状態になってしまう
/*
root
DrawFont(x,y,str,color);A -> D
DrawFont(x,y,str,color);D
DrawFont(x,y,str,color);C
DrawFont(x,y,str,color);B -> A

0123456
ABCDEFGAH

0123456
AGFEDCB

*/
		pCache = m_pLastSFontChacheInfo;

		// 最も使われていないハッシュキーの取得
		// …、こうなるとは限らない
		// キャッシュがヒットするとこれは成り立たない
		if(	m_pLastSFontChacheInfoFirstUsed == m_pLastSFontChacheInfo ){
			Flush();
		}
		if(!m_pLastSFontChacheInfoFirstUsed)
			m_pLastSFontChacheInfoFirstUsed = m_pLastSFontChacheInfo;

		m_pLastSFontChacheInfo = pCache->pPrev;// 実質最終端のキャッシュ
		
		StSFontCacheInfo *pPrev = pCache->pPrev;
		StSFontCacheInfo *pNext = pCache->pNext;
		StSFontCacheInfo *pPrevHash = pCache->pPrevHash;
		StSFontCacheInfo *pNextHash = pCache->pNextHash;
		//
		if(pPrev)pPrev->pNext = pNext;
		if(pNext)pNext->pPrev = pPrev;
		if(pPrevHash)pPrevHash->pNextHash = pNextHash;
		if(pNextHash)pNextHash->pPrevHash = pPrevHash;
		
		
		
		
		pNext = m_pSFontChacheInfo[0].pNext;
		pCache->pPrev     = &m_pSFontChacheInfo[0];
		pCache->pNext     = pNext;
		m_pSFontChacheInfo[0].pNext = pCache;
		if(pNext)pNext->pPrev = pCache;
		
		pNextHash = m_HashRootTable[hash].pNextHash;
		pCache->pPrevHash = &m_HashRootTable[hash];
		pCache->pNextHash = pNextHash;
		m_HashRootTable[hash].pNextHash = pCache;
		if(pNextHash)pNextHash->pPrevHash = pCache;
		
		// ----- Create FontImage DIB(Texture)
		CreateFontImageOrder(hashkey,pCache);
	}
	return pCache;
}

bool CSFastFont::CreateFontImageOrder(int fontcode,StSFontCacheInfo *pCache)
{
	SIZE size;
	pCache->OriginalKey = fontcode;
	if( m_pFastFont->GetFontData(fontcode,&size) )
	{
		pCache->w = size.cx+2;
		pCache->h = size.cy+2;

		pCache->pNextOrder = m_pCreateOrderInfo;
		m_pCreateOrderInfo = pCache;
		return true;
	}
	return false;
}

bool CSFastFont::SFontBlt16(void *pData,DWORD alpha)
{
	WORD *pPixel = (WORD*)pData;
	WORD data = 0;
	if(alpha){
		alpha>>=3;
		data = (WORD)((alpha<<10)|(alpha<<5)|alpha|0x8000);
		*pPixel = data;
	}

	return true;
}
bool CSFastFont::SFontBlt16Black(void *pData,DWORD alpha)
{
	WORD *pPixel = (WORD*)pData;
	WORD data = 0;
	if(alpha){
		alpha>>=3;
		alpha>>=3;
		data = (WORD)((alpha<<10)|(alpha<<5)|alpha|0x8000);
		*pPixel = data;
	}

	return true;
}

void CSFastFont::Flush(void)
{
	StSFontCacheInfo *pNextOrder = m_pCreateOrderInfo;

	if(m_pCreateOrderInfo){
		DDSURFACEDESC2 ddsd;
		HRESULT hr;
		hr = m_pTex->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
		if( hr == D3D_OK ){
			while(pNextOrder){
				//
				SIZE tempsize;
				for(int yy = 0;yy < m_MaxFontHeight;yy++){
					memset(
						((BYTE*)ddsd.lpSurface)+ddsd.lPitch*(yy+pNextOrder->v)+pNextOrder->u*2
						,0x00,m_MaxFontWidth*2);
				}
				m_pFastFont->SetBltStatus(ddsd.lpSurface,ddsd.lPitch,16,m_ImageMode, SFontBlt16Black );
				m_pFastFont->BltFontData(
					pNextOrder->OriginalKey,
					pNextOrder->u+1,
					pNextOrder->v,
					&tempsize);
				m_pFastFont->BltFontData(
					pNextOrder->OriginalKey,
					pNextOrder->u,
					pNextOrder->v+1,
					&tempsize);
				m_pFastFont->BltFontData(
					pNextOrder->OriginalKey,
					pNextOrder->u+2,
					pNextOrder->v+1,
					&tempsize);
				m_pFastFont->BltFontData(
					pNextOrder->OriginalKey,
					pNextOrder->u+1,
					pNextOrder->v+2,
					&tempsize);
				m_pFastFont->SetBltStatus(ddsd.lpSurface,ddsd.lPitch,16,m_ImageMode, SFontBlt16 );
				m_pFastFont->BltFontData(
					pNextOrder->OriginalKey,
					pNextOrder->u+1,
					pNextOrder->v+1,
					&tempsize);
				//
				pNextOrder = pNextOrder->pNextOrder;
			}
			m_pCreateOrderInfo = NULL;
			m_pTex->Unlock(NULL);
		}
		//
	}
	if(m_SSF_Vertex_index){
		LPDIRECTDRAWSURFACE7 lpOriginalTexture = NULL;
		DWORD _state_zenable;
		DWORD _state_zwriteenable;
		DWORD _state_zbias;
		DWORD _state_fogenable;
		DWORD _state_specularenable;

		m_d3ddev->GetRenderState(D3DRENDERSTATE_ZENABLE,        &_state_zenable);
		m_d3ddev->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE,   &_state_zwriteenable);
		m_d3ddev->GetRenderState(D3DRENDERSTATE_ZBIAS,          &_state_zbias);
		m_d3ddev->GetRenderState(D3DRENDERSTATE_FOGENABLE,      &_state_fogenable);
		m_d3ddev->GetRenderState(D3DRENDERSTATE_SPECULARENABLE, &_state_specularenable);
	//	m_d3ddev->GetTexture(0, &lpOriginalTexture);

		m_d3ddev->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
		m_d3ddev->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		m_d3ddev->SetRenderState(D3DRENDERSTATE_ZBIAS, 0);
		m_d3ddev->SetRenderState(D3DRENDERSTATE_FOGENABLE    ,FALSE);
		m_d3ddev->SetRenderState(D3DRENDERSTATE_SPECULARENABLE   ,FALSE);

		m_d3ddev->SetTexture(0,m_pTex);
		m_d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST,D3DFVF_TLVERTEX,
			m_SFF_Vertex,m_SSF_Vertex_index,0);

		m_SSF_Vertex_index = 0;

		m_d3ddev->SetRenderState(D3DRENDERSTATE_ZENABLE,        _state_zenable);
		m_d3ddev->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,   _state_zwriteenable);
		m_d3ddev->SetRenderState(D3DRENDERSTATE_ZBIAS,          _state_zbias);
		m_d3ddev->SetRenderState(D3DRENDERSTATE_FOGENABLE,      _state_specularenable);
		m_d3ddev->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, _state_specularenable);
	//	m_d3ddev->SetTexture(0, lpOriginalTexture);

	}

	m_pLastSFontChacheInfoFirstUsed = NULL;
}
