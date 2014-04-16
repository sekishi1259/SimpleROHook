#include "stdafx.h"
#include "FastFont.h"

CFastFont::CFastFont()
{
	m_hFont = NULL;
	m_pCacheInfo = NULL;
	m_HashDivideNums = 0;
	m_pDefaultBuffer = NULL;
	m_DefaultBufferSize = 0;
	m_OutLineFormat = GGO_BITMAP;

	m_Data2PicFunc = NULL;
	m_DistPtr   = NULL;
	m_DistPitch = 0;
	m_DistBits  = 0;
	m_BltAAMode = 0;
}

CFastFont::~CFastFont(void)
{
	if(m_pDefaultBuffer){
		delete[] m_pDefaultBuffer;
		m_pDefaultBuffer = NULL;
	}

	if(m_pCacheInfo){
		delete m_pCacheInfo;
		m_pCacheInfo = NULL;
	}
	if(m_hDC){
		//SelectObject(m_hDC, m_hOldFont);
		DeleteDC( m_hDC );
		m_hDC = NULL;
	}

	if(m_hFont)
		DeleteObject(m_hFont);
}

void CFastFont::GetMaxSize(SIZE *pSize)
{
	pSize->cx = m_TM.tmMaxCharWidth;
	pSize->cy = m_TM.tmHeight;
}

bool CFastFont::CreateFastFont(LOGFONT *lplf,int OutLineFormat,int HashDivideNums)
{
	m_HashDivideNums = HashDivideNums;
	m_OutLineFormat  = OutLineFormat;

	m_hFont = CreateFontIndirect(lplf);
	if(m_hFont){
		// 高速化のため固定展開バッファを作成
		m_hDC     = CreateCompatibleDC(NULL);
		if( !m_hDC ){
			DeleteObject(m_hFont);
			return false;
		}
		m_hOldFont = (HFONT)SelectObject(m_hDC, m_hFont);
		GetTextMetrics( m_hDC, &m_TM );

		m_DefaultBufferSize = m_TM.tmHeight * ((m_TM.tmMaxCharWidth+3)&0xfffffffc);//最大サイズ
		m_pDefaultBuffer = new BYTE[m_DefaultBufferSize];
		if(!m_pDefaultBuffer){
			DeleteObject(m_hFont);
			return false;
		}

		m_pCacheInfo = new CacheInfo(m_HashDivideNums);
		if(!m_pCacheInfo){
			delete[] m_pDefaultBuffer;
			DeleteObject(m_hFont);
			return false;
		}
		return true;
	}
	return false;
}


int  CFastFont::DebugGetHashEntrys(int hashtableno)
{
	int nums = 0;
	if( m_pCacheInfo ){
		nums = m_pCacheInfo->DebugGetHashEntrys(hashtableno);
	}
	return nums;
}


void CFastFont::ClearCache(void)
{
	m_pCacheInfo->ClearCache();
}

#if 0
void CFastFont::GetStringImage(LPTSTR str)
{
	SIZE TSize;
	int code;

	TCHAR *c;
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
			code = *c++;
		}
#endif
		//
		m_pFastFont->GetFontData(code,&tempsize);
		TSize.cx += tempsize.cx;
		TSize.cy  = tempsize.cy;
	}
	TSize.cx+=2;
	TSize.cy+=2;
}
#endif							   

CFastFont::StOutlineFontData *CFastFont::GetFontData(int code,SIZE *pSize)
{
	StOutlineFontData *pData;

	pData = (StOutlineFontData*)m_pCacheInfo->GetCacheData(code);

	if( !pData ){
		//
		int size;
		DWORD imagesize;
		CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};
		GLYPHMETRICS GM;

		imagesize = GetGlyphOutline(m_hDC, code, m_OutLineFormat,
				&GM, m_DefaultBufferSize,
				m_pDefaultBuffer, &Mat);
		if(imagesize == GDI_ERROR){
			GLYPHMETRICS tempGM;
			imagesize = 0;
			if( GetGlyphOutline(m_hDC, code, GGO_METRICS,&tempGM, 0,NULL, &Mat) != GDI_ERROR ){
				// 2Bit以上のスペースはアウトラインは取れないがGLYPHMETRICSだけなら取得可能
				GM.gmBlackBoxX = 0;
				GM.gmBlackBoxY = 0;
				GM.gmCellIncX = tempGM.gmCellIncX;
				GM.gmCellIncY = 0;
				GM.gmptGlyphOrigin.x = 0;
				GM.gmptGlyphOrigin.y = m_TM.tmAscent;
			}else{
				// 未知のコード
				GM.gmBlackBoxX = 0;
				GM.gmBlackBoxY = 0;
				GM.gmCellIncX = (short)m_TM.tmMaxCharWidth;
				GM.gmCellIncY = 0;
				GM.gmptGlyphOrigin.x = 0;
				GM.gmptGlyphOrigin.y = m_TM.tmAscent;
			}
		}
		size = sizeof(StOutlineFontData)+(imagesize-1);
		pData = (StOutlineFontData*)m_pCacheInfo->CreateData(code,size);
		if(pData){
			memcpy(&pData->GM,&GM,sizeof(GLYPHMETRICS));
			if(imagesize)
				memcpy(pData->Image,m_pDefaultBuffer,imagesize);
		}
	}
	if(pData && pSize){
		pSize->cx = pData->GM.gmCellIncX;
		pSize->cy = m_TM.tmHeight;
	}

	return pData;
}

void CFastFont::SetBltStatus(void *dist,DWORD pitch,DWORD bits,int mode,funcFFDATA2PIC func)
{
	m_Data2PicFunc = func;
	m_DistPtr   = (BYTE*)dist;
	m_DistPitch = pitch;
	m_DistBits  = bits;
	m_BltAAMode = mode;
}


void CFastFont::BltFontData(int code,int x,int y,SIZE *pSize)
{
	StOutlineFontData *pData;

	pData = GetFontData(code,pSize);
	if( pData ){
		int iOfs_x = pData->GM.gmptGlyphOrigin.x;
		int iOfs_y = m_TM.tmAscent - pData->GM.gmptGlyphOrigin.y;
		int iBmp_w = (pData->GM.gmBlackBoxX + 3)&0xfffffffc;
		int iBmp_h = pData->GM.gmBlackBoxY;
		int iUseBYTEparLine = ((iBmp_w+31)/32)*4 ;
		int xx, yy;
		DWORD Alpha;
		BYTE  *ptr = pData->Image;
		int Level = 0;
		switch(m_OutLineFormat){
		case GGO_BITMAP:
			Level = 0;
			break;
		case GGO_GRAY2_BITMAP:
			Level = 5;
			break;
		case GGO_GRAY4_BITMAP:
			Level = 17;
			break;
		case GGO_GRAY8_BITMAP:
			Level = 65;
			break;
		}

		if(m_BltAAMode){
			DWORD *AlphaTable = new DWORD[iBmp_w+1];
			memset(AlphaTable,0,sizeof(DWORD)*(iBmp_w+1));
			for(yy = 0; yy<iBmp_h+1; yy++){
				DWORD old_Alpha = 0;
				Alpha = 0;
				for(xx = 0; xx<iBmp_w+1; xx++){
					old_Alpha = Alpha;
					if( xx == iBmp_w || yy == iBmp_h){
						Alpha = 0;
					}else{
						if(Level){//
							Alpha = (448 * ptr[xx + iBmp_w*yy]) / (Level-1);
						}else{
							BYTE Cur =  (*((BYTE*)ptr + iUseBYTEparLine*yy + (xx / 8) ) >> (7 - xx % 8)) & 0x1;
							Alpha = Cur * 448;
						}
					}
					Alpha = (Alpha + old_Alpha + AlphaTable[xx])/3;
					if(Alpha>255)Alpha=255;
					if(Alpha<64)Alpha=0;
					AlphaTable[xx] = Alpha;

					if(m_Data2PicFunc){
						m_Data2PicFunc(  
							m_DistPtr + m_DistPitch  *(y+yy+iOfs_y) +
									   (m_DistBits/8)*(x+xx+iOfs_x),
							Alpha );
					}
				}
			}
			delete[] AlphaTable;
		}else{
			for(yy = 0; yy<iBmp_h; yy++){
				for(xx = 0; xx<iBmp_w; xx++){
					if(Level){
						Alpha = (255 * ptr[xx + iBmp_w*yy]) / (Level-1);
					}else{
						BYTE Cur =  (*((BYTE*)ptr + iUseBYTEparLine*yy + (xx / 8) ) >> (7 - xx % 8)) & 0x1;
						Alpha = Cur * 255;
					}
					if(m_Data2PicFunc){
						m_Data2PicFunc(  
							m_DistPtr + m_DistPitch  *(y+yy+iOfs_y) +
									   (m_DistBits/8)*(x+xx+iOfs_x),
							Alpha );
					}
				}
			}
		}

	}
}

#include <tchar.h>

void CFastFont::test(int mode)
{
	CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};
	GLYPHMETRICS GM;
	int code;
	DWORD imagesize;
	code = (int)_T("あ");
	switch(mode){
	case 0:
		imagesize = GetGlyphOutline(m_hDC, code, m_OutLineFormat, &GM, 0, NULL, &Mat);
		break;
	case 1:
		{
			// バッファサイズが大きいほど時間が掛かる
			// 
			BYTE g_data[64*64];
			GetGlyphOutline(m_hDC, code, m_OutLineFormat,&GM, 64*64,g_data, &Mat);
		}
		break;
	case 2:
		{
			// NULLによるバッファサイズ取得は以外に負荷が高い
			BYTE *pdata;
			imagesize = GetGlyphOutline(m_hDC, code, m_OutLineFormat, &GM, 0, NULL, &Mat);
			pdata = new BYTE[imagesize];
			GetGlyphOutline(m_hDC, code, m_OutLineFormat,&GM, imagesize,pdata, &Mat);
			delete[] pdata;
		}
		break;
	case 3:
		{
			// フォントサイズがある程度把握できるならば
			// 取得バッファを前もって確保、取得後バッファを作成してコピーが最速
			BYTE g_data[64*64];
			BYTE *pdata;
			imagesize = GetGlyphOutline(m_hDC, code, m_OutLineFormat,&GM, 64*64,g_data, &Mat);
			pdata = new BYTE[imagesize];
			memcpy(g_data,pdata,imagesize);
			delete[] pdata;
		}
		break;
	default:
		break;
	}
}
