#include "../tinyconsole.h"
#include "RoCodeBind.h"

HANDLE         g_hMapObject = 0;
StSHAREDMEMORY *g_pSharedData = 0;


typedef int (WSAAPI *tWS2_32_send)(SOCKET s, char *buf, int len, int flags);
typedef int (WSAAPI *tWS2_32_recv)(SOCKET s, char *buf, int len, int flags);

tWS2_32_send *pCConnection_s_wsSend = NULL;
tWS2_32_recv *pCConnection_s_wsRecv = NULL;

extern tWS2_32_recv OrigWS2_32_recv;
int WSAAPI ProxyWS2_32_recv(SOCKET s, char *buf, int len, int flags);

/*
	Create a shared memory.
*/
BOOL OpenSharedMemory(void)
{
	g_hMapObject = ::OpenFileMapping( FILE_MAP_ALL_ACCESS , FALSE, SHAREDMEMORY_OBJECTNAME );
	if( !g_hMapObject ){
		DEBUG_LOGGING_NORMAL( ("shared memory:Initialize Failed.") );
		return FALSE;
	}
	g_pSharedData = (StSHAREDMEMORY*)::MapViewOfFile(g_hMapObject,
		FILE_MAP_ALL_ACCESS,0,0,0);
	if(!g_pSharedData){
		DEBUG_LOGGING_NORMAL( ("shared memory:DataMap Failed.") );
		::CloseHandle( g_hMapObject);
		g_hMapObject = NULL;
		return FALSE;
	}
	return TRUE;
}
/*
	Release shared memory
*/
BOOL ReleaseSharedMemory(void)
{
	if(g_pSharedData){
		g_pSharedData->g_hROWindow = 0;
		::UnmapViewOfFile( g_pSharedData );
		g_pSharedData = NULL;
	}
	if(g_hMapObject){
		::CloseHandle( g_hMapObject);
		g_hMapObject = NULL;
	}
	return TRUE;
}

CPerformanceCounter g_PerformanceCounter(60);

CRoCodeBind* g_pRoCodeBind = NULL;

BOOL g_FreeMouseSw = FALSE;


static HRESULT CALLBACK TextureSearchCallback( DDPIXELFORMAT* pddpf,
                                               VOID* param )
{
    if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
        return DDENUMRET_OK;
    if( pddpf->dwFourCC != 0 )
        return DDENUMRET_OK;
    if( pddpf->dwRGBBitCount != 16 )
        return DDENUMRET_OK;
    if(!(pddpf->dwFlags&DDPF_ALPHAPIXELS) )
        return DDENUMRET_OK;
	// get 16 bit with alphapixel format
    memcpy( (DDPIXELFORMAT*)param, pddpf, sizeof(DDPIXELFORMAT) );
    return DDENUMRET_CANCEL;
}

void CRoCodeBind::Init(IDirect3DDevice7* d3ddevice)
{
	SearchRagexeMemory();
	InitItemNameMap();
	InitPacketHandler();
	LoadIni();

	D3DDEVICEDESC7 ddDesc;
	d3ddevice->GetCaps( &ddDesc );

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize          = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|
							DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
	ddsd.dwWidth         = 512;
	ddsd.dwHeight        = 512;

	if( ddDesc.deviceGUID == IID_IDirect3DHALDevice ){
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	} else if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice ){
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	} else {
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}

	d3ddevice->EnumTextureFormats( TextureSearchCallback, &ddsd.ddpfPixelFormat );
	if( ddsd.ddpfPixelFormat.dwRGBBitCount ){
		LPDIRECTDRAWSURFACE7 pddsRender = NULL;
		LPDIRECTDRAW7        pDD = NULL;

		d3ddevice->GetRenderTarget( &pddsRender );
		if( pddsRender ){
			pddsRender->GetDDInterface( (VOID**)&pDD );
			pddsRender->Release();
		}
		if( pDD ){
			if( SUCCEEDED( pDD->CreateSurface( &ddsd, &m_pddsFontTexture, NULL )) ){
				DEBUG_LOGGING_NORMAL(( "font texture created." ));
			} else {
				DEBUG_LOGGING_NORMAL(( "failed create a font texture." ));
			}
			pDD->Release();
		}
		if( m_pddsFontTexture ){
			LOGFONT logfont;
			logfont.lfHeight         = -12;
			logfont.lfWidth          = 0;
			logfont.lfEscapement     = 0;
			logfont.lfOrientation    = 0;
			logfont.lfWeight         = FW_REGULAR;
			//
			logfont.lfItalic         = FALSE;
			logfont.lfUnderline      = FALSE;
			logfont.lfStrikeOut      = FALSE;
			logfont.lfCharSet        = DEFAULT_CHARSET;
			logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS; 
			logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS; 
			logfont.lfQuality        = NONANTIALIASED_QUALITY; 
			logfont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE; 
			_tcscpy_s(logfont.lfFaceName,_T("‚l‚r ‚oƒSƒVƒbƒN"));

			m_pSFastFont = new CSFastFont;
			m_pSFastFont->CreateFastFont(&logfont,d3ddevice,m_pddsFontTexture,0);
		}
	}

}

CRoCodeBind::~CRoCodeBind(void)
{
	if( m_pSFastFont )
		delete m_pSFastFont;
	if( m_pddsFontTexture ){
		m_pddsFontTexture->Release();
		m_pddsFontTexture = NULL;
	}
}

void *CRoCodeBind::GetPak(const char *name, unsigned int *size)
{
	if (m_CFileMgr__gfileMgr && m_functionRagexe_CFileMgr__GetPak)
	{
		void *address;
		address = m_functionRagexe_CFileMgr__GetPak(m_CFileMgr__gfileMgr,name, size);
		return address;
	}
	return NULL;
}
void CRoCodeBind::ReleasePak(void *handle)
{
	::VirtualFree(handle, 0, MEM_RELEASE);
}

void CRoCodeBind::InitItemNameMap()
{
	char *buf = NULL;
	char *p, *ptoken;

	unsigned int size;
	buf = (char*)GetPak("data\\idnum2itemdisplaynametable.txt",&size);

	if (!buf)return;
	DEBUG_LOGGING_NORMAL(("load data\\idnum2itemdisplaynametable.txt from grf."));

	p = buf;
	while (*p != '\0'){
		ptoken = p;
		while (*p != '\0' && *p != '\r' && *p != '\n'){
			p++;
		}
		if (*p == '\r')p++;
		if (*p == '\n')p++;

		if (*ptoken == '/' || *ptoken == '\0' || *ptoken == ' ' || *ptoken == '\r' || *ptoken == '\n')
			continue;

#if 1
		int itemid = 0;
		while (*ptoken != '#'){
			int num;
			num = *ptoken++;
			if( num >= '0' && num <= '9' ){
				itemid = (itemid*10)+(num - '0');
			}
		}
#else
		// case of bRO( brasil ragnarok )
		// injection.dll is dead if use the win32api(StrToInt and atoi and etc..) on nprotect gameguard.
		char numstr[10], *pname;
		pname = numstr;
		while (*ptoken != '#')*pname++ = *ptoken++;
		*pname++ = '\0';
		int itemid = atoi(numstr);
#endif
		//DEBUG_LOGGING_NORMAL(("item id: %d",itemid));

		char tempstr[256];
		char *pdname = tempstr;
		*ptoken++;
		while (*ptoken != '#'){
			if (*ptoken != '_')
				*pdname = *ptoken;
			else
				*pdname = ' ';
			pdname++;
			ptoken++;
		}
		*pdname++ = '\0';

		if (itemid >= 0 )
			m_ItemName[itemid] = tempstr;
	}
	ReleasePak(buf);
	DEBUG_LOGGING_NORMAL(("release data\\idnum2itemdisplaynametable.txt"));
}

const char *CRoCodeBind::GetItemNameByID(int id)
{
	static const char* pUnknownItem = "Unknown Item";

	if (m_ItemName[id].empty()){
		return pUnknownItem;
	}
	else{
		return m_ItemName[id].c_str();
	}
}


void CRoCodeBind::OneSyncProc(HRESULT Result,LPVOID lpvData,BOOL FreeMouse)
{
#ifndef USE_WS2_32DLLINJECTION
	if (pCConnection_s_wsRecv){
		if (*pCConnection_s_wsRecv && OrigWS2_32_recv == 0){
			OrigWS2_32_recv = *pCConnection_s_wsRecv;
			*pCConnection_s_wsRecv = &ProxyWS2_32_recv;
			DEBUG_LOGGING_NORMAL(("Hook CConnection_s_wsRecv(%08X) = %08X old %08X",
				pCConnection_s_wsRecv, ProxyWS2_32_recv,OrigWS2_32_recv ));
		}
	}
#endif

	if( Result == DI_OK ){
		//
		//  FreeMouse
		//
		if( FreeMouse ){
			MouseDataStructure *p = (MouseDataStructure*)lpvData;

			POINT point;
			::GetCursorPos(&point);
			::ScreenToClient(m_hWnd, &point);

			if( ::GetActiveWindow() == m_hWnd ){
				point.x -= p->x_axis;
				point.y -= p->y_axis;
			}
			SetMouseCurPos((int)point.x, (int)point.y);
		}
	}


	// proc on frame
	if( g_pSharedData ){
		if( g_pSharedData->executeorder == 1){
			g_pSharedData->executeorder = 0;
			char filename[MAX_PATH];
			if( ::WideCharToMultiByte(CP_ACP,0,
				g_pSharedData->musicfilename,wcslen(g_pSharedData->musicfilename)+1,
				filename,sizeof(filename),
				NULL,NULL) ){
				//
				// Play Sound File
				//
				if( m_funcRagexe_PlayStream )
					m_funcRagexe_PlayStream(filename,0);
			}
		}

		// cpu cooler
		if( g_pSharedData->cpucoolerlevel ){
			int level = g_pSharedData->cpucoolerlevel;
			int CoolerLevelTable[4]={1,1,3,10};
			int ref = g_PerformanceCounter.GetMonitorRefreshRate();

			if( level < 0 )level = 0;
			else if( level > 3 )level = 3;

			ref /= CoolerLevelTable[level];
			if( ref ){
				::Sleep( 1000/ref );
			}
		}
	}

}

void CRoCodeBind::SetMouseCurPos(int x,int y)
{
	if (g_mouse){
		g_mouse->m_xPos = x;
		g_mouse->m_yPos = y;
	}
}

void vector3d::MatrixMult(struct vector3d& v, struct matrix& m)
{
	x = v.x * m.v11 + v.y * m.v21 + v.z * m.v31 + m.v41;
	y = v.x * m.v12 + v.y * m.v22 + v.z * m.v32 + m.v42;
	z = v.x * m.v13 + v.y * m.v23 + v.z * m.v33 + m.v43;
}

// CRenderer::ProjectVertex
void CRoCodeBind::ProjectVertex(vector3d& src,matrix& vtm,float *x,float *y,float *oow)
{
	if( !g_renderer && !*g_renderer )return;

	vector3d viewvect;
	viewvect.MatrixMult( src , vtm );

	float w = 1.0f / viewvect.z;

	*x = viewvect.x * w * (*g_renderer)->m_hpc + (*g_renderer)->m_xoffset;
	*y = viewvect.y * w * (*g_renderer)->m_vpc + (*g_renderer)->m_yoffset;
	*oow = w;
}

// CRenderer::ProjectVertex
void CRoCodeBind::ProjectVertex(vector3d& src,matrix& vtm,tlvertex3d *vert)
{
	if( !g_renderer && !*g_renderer )return;

	vector3d viewvect;
	viewvect.MatrixMult( src , vtm );

	float w = 1.0f / viewvect.z;

	vert->x = viewvect.x * w * (*g_renderer)->m_hpc + (*g_renderer)->m_halfWidth;
	vert->y = viewvect.y * w * (*g_renderer)->m_vpc + (*g_renderer)->m_halfHeight;
	vert->z = (1500 / (1500.0f - 10.0f)) * (( 1.0f / w ) - 10.0f) * w;
	vert->oow = w;
}

// ProjectVertexEx
// to move pointvector on camera view
void CRoCodeBind::ProjectVertexEx(vector3d& src, vector3d& pointvector, matrix& vtm, float *x, float *y, float *oow)
{
	if (!g_renderer && !*g_renderer)return;

	vector3d viewvect;
	viewvect.MatrixMult(src, vtm);
	viewvect += pointvector;

	float w = 1.0f / viewvect.z;

	*x = viewvect.x * w * (*g_renderer)->m_hpc + (*g_renderer)->m_xoffset;
	*y = viewvect.y * w * (*g_renderer)->m_vpc + (*g_renderer)->m_yoffset;
	*oow = w;
}

void CRoCodeBind::ProjectVertexEx(vector3d& src,vector3d& pointvector, matrix& vtm, tlvertex3d *vert)
{
	if (!g_renderer && !*g_renderer)return;

	vector3d viewvect;
	viewvect.MatrixMult(src, vtm);
	viewvect += pointvector;

	float w = 1.0f / viewvect.z;

	vert->x = viewvect.x * w * (*g_renderer)->m_hpc + (*g_renderer)->m_halfWidth;
	vert->y = viewvect.y * w * (*g_renderer)->m_vpc + (*g_renderer)->m_halfHeight;
	vert->z = (1500 / (1500.0f - 10.0f)) * ((1.0f / w) - 10.0f) * w;
	vert->oow = w;
}


void CRoCodeBind::LoadIni(void)
{
	if( g_pSharedData ){

		int sectionsize;
		char Sectionbuf[32768];
		char *pkey;
		char filename[MAX_PATH];

		if( ::WideCharToMultiByte(CP_ACP,0,
			g_pSharedData->configfilepath,wcslen(g_pSharedData->configfilepath)+1,
			filename,sizeof(filename),
			NULL,NULL) ){

		DEBUG_LOGGING_NORMAL( ("LoadIni startup") );
		DEBUG_LOGGING_NORMAL( ("%s",filename) );

			sectionsize = GetPrivateProfileSection(_T("M2E"),Sectionbuf,32768,filename);
			pkey = Sectionbuf;

			for(int ii = 0;ii < MAX_FLLORSKILLTYPE;ii++)
				m_M2ESkillColor[ii]=0;

			
			while(*pkey!='\0'){
				int index;
				DWORD color;

				char *ptemp;
				ptemp = pkey;

				std::string linestring(ptemp);

				pkey += linestring.length();

				sscanf_s(linestring.c_str(),"Skill%x=%x",&index,&color);
				m_M2ESkillColor[index] = color;
				pkey++;
			}
		}
	}
}

/*
   drawgage sample
   (x,y,  w,h) screenposition size
   value : 0 - 1000
   type != 0 : use sub bg
 */
void CRoCodeBind::DrawGage(LPDIRECT3DDEVICE7 device, int x, int y, int w, int h, unsigned long value, DWORD color, int alpha,int type)
{
	int gage_range;
	DWORD gage_color;
	D3DTLVERTEX vp[6];

	for (int ii = 0; ii < 5; ii++){
		vp[ii].sx = 0.0f;
		vp[ii].sy = 0.0f;
		vp[ii].sz = 0.5f;
		vp[ii].rhw = 1.0f;
		vp[ii].tu = 0.0f;
		vp[ii].tv = 0.0f;
	}

	if (type){
		gage_color = D3DCOLOR_ARGB(0x80, 1, 1, 1);
		for (int ii = 0; ii < 4; ii++){
			vp[ii].color = gage_color;
		}
		vp[0].sx = (D3DVALUE)(x);
		vp[0].sy = (D3DVALUE)(y);
		vp[1].sx = (D3DVALUE)(x + w);
		vp[1].sy = (D3DVALUE)(y);
		vp[2].sx = (D3DVALUE)(x);
		vp[2].sy = (D3DVALUE)(y - 12 - 1);
		vp[3].sx = (D3DVALUE)(x + w);
		vp[3].sy = (D3DVALUE)(y - 12 - 1);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, vp, 4, 0);
	}

	gage_color = D3DCOLOR_ARGB(0x00, 64, 64, 64);
	gage_color = gage_color | (alpha << 24);
	for (int ii = 0; ii < 4; ii++){
		vp[ii].color = gage_color;
	}
	vp[0].sx = (D3DVALUE)(x);
	vp[0].sy = (D3DVALUE)(y);
	vp[1].sx = (D3DVALUE)(x + w - 1);
	vp[1].sy = (D3DVALUE)(y);
	vp[2].sx = (D3DVALUE)(x);
	vp[2].sy = (D3DVALUE)(y + h - 1);
	vp[3].sx = (D3DVALUE)(x + w - 1);
	vp[3].sy = (D3DVALUE)(y + h - 1);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, vp, 4, 0);

	gage_color = D3DCOLOR_ARGB(0x00, 16, 24, 152);
	gage_color = gage_color | (alpha << 24);
	for (int ii = 0; ii < 5; ii++){
		vp[ii].color = gage_color;
	}
	vp[0].sx = (D3DVALUE)(x);
	vp[0].sy = (D3DVALUE)(y);
	vp[1].sx = (D3DVALUE)(x + w - 1);
	vp[1].sy = (D3DVALUE)(y);
	vp[2].sx = (D3DVALUE)(x + w - 1);
	vp[2].sy = (D3DVALUE)(y + h - 1);
	vp[3].sx = (D3DVALUE)(x);
	vp[3].sy = (D3DVALUE)(y + h - 1);
	vp[4].sx = vp[0].sx;
	vp[4].sy = vp[0].sy;
	device->DrawPrimitive(D3DPT_LINESTRIP, D3DFVF_TLVERTEX, vp, 5, 0);

	gage_color = color | (alpha << 24);

	gage_range = (int)((value*(w - 2)) / 1000);
	for (int ii = 0; ii < 4; ii++){
		vp[ii].color = gage_color;
	}
	vp[0].sx = (D3DVALUE)(x + 1);
	vp[0].sy = (D3DVALUE)(y + 1);
	vp[1].sx = (D3DVALUE)(x + 1 + gage_range);
	vp[1].sy = (D3DVALUE)(y + 1);
	vp[2].sx = (D3DVALUE)(x + 1);
	vp[2].sy = (D3DVALUE)(y + h - 1);
	vp[3].sx = (D3DVALUE)(x + 1 + gage_range);
	vp[3].sy = (D3DVALUE)(y + h - 1);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, vp, 4, 0);
}
/*
 gage draw sample
 hp: 0 - 1000
 sp: 0 - 1000
 */
void CRoCodeBind::DrawHPSPGage(IDirect3DDevice7 *d3ddev, int x, int y, int hp, int sp)
{
	D3DTLVERTEX vp[6];
	int gage_range;

	vp[0].rhw =
	vp[1].rhw =
	vp[2].rhw =
	vp[3].rhw = 1.0f;

	vp[0].color =
	vp[1].color =
	vp[2].color =
	vp[3].color = D3DCOLOR_ARGB(255, 16, 24, 152);
	//
	vp[0].sz =
	vp[1].sz =
	vp[2].sz =
	vp[3].sz = 0.5f;

	vp[0].sx = (float)(x);
	vp[0].sy = (float)(y);
	vp[0].tu = 0.0f;
	vp[0].tv = 0.0f;
	//
	vp[1].sx = (float)(x + 60);
	vp[1].sy = vp[0].sy;
	vp[1].tu = 0.0f;
	vp[1].tv = 0.0f;
	//
	vp[2].sx = vp[0].sx;
	vp[2].sy = (float)(y + 9);
	vp[2].tu = 0.0f;
	vp[2].tv = 0.0f;
	//
	vp[3].sx = vp[1].sx;
	vp[3].sy = vp[2].sy;
	vp[3].tu = 0.0f;
	vp[3].tv = 0.0f;
	//
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, vp, 4, 0);
	//
	for (int ii = 0; ii<2; ii++){
		int value;
		// HP SP
		vp[0].color =
		vp[1].color =
		vp[2].color =
		vp[3].color = D3DCOLOR_ARGB(255, 64, 64, 64);
		//
		vp[0].sx = (float)(x + 1);
		vp[0].sy = (float)(y + 1 + ii * 4);
		vp[1].sx = (float)(x + 1 + 58);
		vp[1].sy = vp[0].sy;
		vp[2].sx = vp[0].sx;
		vp[2].sy = (float)(y + 1 + 3 + ii * 4);
		vp[3].sx = vp[1].sx;
		vp[3].sy = vp[2].sy;
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, vp, 4, 0);

		// normal color

		if (ii == 0){
			value = hp;
			if (hp > (1000 / 4) ) {
				// dead zone color
				vp[0].color =
				vp[1].color =
				vp[2].color =
				vp[3].color = D3DCOLOR_ARGB(255, 24, 96, 216);
			} else {
				vp[0].color =
				vp[1].color =
				vp[2].color =
				vp[3].color = D3DCOLOR_ARGB(255, 232, 16, 16);
			}
		} else {
			vp[0].color =
			vp[1].color =
			vp[2].color =
			vp[3].color = D3DCOLOR_ARGB(255, 38, 236, 32);
			value = sp;
		}

		gage_range = (value * 58) / 1000;

		// HP SP
		vp[0].sx = (float)(x + 1);
		vp[0].sy = (float)(y + 1 + ii * 4);
		vp[1].sx = (float)(x + 1 + gage_range);
		vp[1].sy = vp[0].sy;
		vp[2].sx = vp[0].sx;
		vp[2].sy = (float)(y + 1 + 3 + ii * 4);
		vp[3].sx = vp[1].sx;
		vp[3].sy = vp[2].sy;
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, vp, 4, 0);
	}
}


void CRoCodeBind::DrawSRHDebug(IDirect3DDevice7* d3ddevice)
{
	if (!g_pSharedData) return;
	if (!g_pmodeMgr) return;
	if (!g_renderer) return;
	if (!*g_renderer) return;

	if( g_pSharedData->show_framerate ){
		std::stringstream str;
		str << g_PerformanceCounter.GetFrameRate() << "fps : "<<(int)g_PerformanceCounter.GetTotalTick() << std::endl;

		m_pSFastFont->DrawText((LPSTR)str.str().c_str(), 0, 0,D3DCOLOR_ARGB(255,255,255,255),0,NULL);
	}

	if( g_pSharedData->objectinformation ){
		std::stringstream str;
		CModeMgr *pcmode = g_pmodeMgr;
		//str.str("");
		CGameMode *p_gamemode = (CGameMode*)pcmode->m_curMode;

		BackupRenderState(d3ddevice);
		d3ddevice->SetTexture(0, NULL);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, FALSE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, 0x00);
		d3ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		d3ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);


		if( p_gamemode && pcmode->m_curModeType == 1 
		 && p_gamemode->m_world && p_gamemode->m_view && p_gamemode->m_world->m_attr ){
			CView *pView = p_gamemode->m_view;

			C3dAttr *pAttr = p_gamemode->m_world->m_attr;

			if( p_gamemode->m_world->m_player ){
				std::stringstream putinfostr;
				CPlayer *pPlayer = (CPlayer*)p_gamemode->m_world->m_player;

				// CRenderObject
				str << "m_BodyAni = "      << (int)pPlayer->m_BodyAni << "\n";
				str << "m_BodyAct = "      << (int)pPlayer->m_BodyAct << "\n";
				str << "m_BodyAniFrame = " << (int)pPlayer->m_BodyAniFrame << "\n";
				str << "m_sprRes = "       << std::hex << pPlayer->m_sprRes << "\n";
				str << "m_actRes = "       << std::hex << pPlayer->m_actRes << "\n\n";
				// CAbleToMakeEffect
				str << "m_efId = "         << pPlayer->m_efId << "\n";
				str << "m_Sk_Level = "     << pPlayer->m_Sk_Level << "\n\n";

				// CGameActor
				str << "m_moveDestX/Y = " << std::dec 
					<< pPlayer->m_moveDestX << ","
					<< pPlayer->m_moveDestY << "\n\n";
				str << "m_speed = " << pPlayer->m_speed << "\n\n";

				str << "m_clevel = " << pPlayer->m_clevel << "\n";
				str << "m_gid = " << std::hex << (unsigned long)pPlayer->m_gid << "\n";
				str << "m_job = " << std::hex << (unsigned long)pPlayer->m_job << "\n";
				str << "m_sex = " << std::hex << (unsigned long)pPlayer->m_sex << "\n\n";

				unsigned char *pdump = (unsigned char*)&pPlayer->m_efId;
				for(int ii = 0;ii < 64 ;ii++){
					str << std::setfill('0') << std::setw(2) << std::hex << (int)pdump[ii] << ",";
					if( (ii % 0x10)==0x0f ){
						str << std::endl;
					}
				}
				str << std::endl;

				matrix *pm = &p_gamemode->m_view->m_viewMatrix;
				str << "m_viewMatrix\n"
					<< pm->v11 << " , "
					<< pm->v12 << " , "
					<< pm->v13 << "\n"
					<< pm->v21 << " , "
					<< pm->v22 << " , "
					<< pm->v23 << "\n"
					<< pm->v31 << " , "
					<< pm->v32 << " , "
					<< pm->v33 << "\n"
					<< pm->v41 << " , "
					<< pm->v42 << " , "
					<< pm->v43 << std::endl;

				str << "CRenderer = " << *g_renderer << "\n"
					<< "m_hpc = " << (*g_renderer)->m_hpc << "\n"
					<< "m_vpc = " << (*g_renderer)->m_vpc << "\n"
					<< "m_hratio = " << (*g_renderer)->m_hratio << "\n"
					<< "m_vratio = " << (*g_renderer)->m_vratio << "\n"
					<< "m_aspectRatio = " << (*g_renderer)->m_aspectRatio << "\n"
					<< "m_screenXFactor = " << (*g_renderer)->m_screenXFactor << "\n"
					<< "m_screenYFactor = " << (*g_renderer)->m_screenYFactor << "\n"
					<< "m_xoffset = " << (*g_renderer)->m_xoffset << "\n"
					<< "m_yoffset = " << (*g_renderer)->m_yoffset << "\n"
					<< "m_width = " << (*g_renderer)->m_width << "\n"
					<< "m_height = " << (*g_renderer)->m_height << "\n"
					<< "m_halfWidth = " << (*g_renderer)->m_halfWidth << "\n"
					<< "m_halfHeight = " << (*g_renderer)->m_halfHeight << "\n"
					<< "m_curFrame = " << (*g_renderer)->m_curFrame << "\n"
					<< "m_bRGBBitCount = " << (*g_renderer)->m_bRGBBitCount << "\n"
					<< "m_fpsFrameCount = " << (*g_renderer)->m_fpsFrameCount << "\n"
					<< "m_dwScreenWidth = " << (*g_renderer)->m_dwScreenWidth << "\n"
					<< "m_dwScreenHeight = " << (*g_renderer)->m_dwScreenHeight << "\n"
					<< std::endl;

				//
				//  world position to cell position
				//
				long cx,cy;
				pAttr->ConvertToCellCoor(pPlayer->m_pos.x,pPlayer->m_pos.z,cx,cy);
				//
				//
				//
				putinfostr << "(" << cx << "," << cy << ")" << std::endl;

				int sx,sy;
				float fx,fy,oow;
				ProjectVertex( pPlayer->m_pos,pView->m_viewMatrix,&fx,&fy,&oow);
				sx = (int)fx;
				sy = (int)fy;


				m_pSFastFont->DrawText((LPSTR)putinfostr.str().c_str(), sx, sy,D3DCOLOR_ARGB(255,255,255,255),2,NULL);

				// fake cast bar
				vector3d pointvecter(0, -13.5f, 0);
				ProjectVertexEx(pPlayer->m_pos, pointvecter, pView->m_viewMatrix, &fx, &fy, &oow);
				{
					static int sx = 0, sy = 0;
					DrawGage(d3ddevice, sx, sy, 60, 6, 800, D3DCOLOR_ARGB(0x00, 180, 0, 0), 0xff, 0);
					sx = (int)fx - 30 + 60; // +60 side of the true cast bar
					sy = (int)fy - 3;
				}
				// fake hp sp bar
				pointvecter.Set(0, 2.5, 0);
				ProjectVertexEx(pPlayer->m_pos, pointvecter, pView->m_viewMatrix, &fx, &fy, &oow);
				{
					static int sx = 0, sy = 0;
					DrawHPSPGage(d3ddevice, sx, sy, 200, 900);
					sx = (int)fx - 30 + 60; // +60 side of the true hpsp bar
					sy = (int)fy - 6;
				}
			}

			int skillnums = p_gamemode->m_world->m_skillList.size();
			str << " m_skillList size =" << skillnums << "\n";

			int itemnums = p_gamemode->m_world->m_itemList.size();
			str << " m_itemList size =" << itemnums << "\n";

			std::list<CItem*> itemList = p_gamemode->m_world->m_itemList;
			for( std::list<CItem*>::iterator it = itemList.begin() ; it != itemList.end() ; it++ )
			{
				CItem *pItem = *it;
				if( pItem ){
					long cx,cy;
					pAttr->ConvertToCellCoor(pItem->m_pos.x,pItem->m_pos.z,cx,cy);

					std::stringstream putinfostr;
					putinfostr << "(" << cx << "," << cy << ")" << std::endl;
					//putinfostr << pItem->m_itemName << std::endl;
				//	putinfostr << "aid = " << pItem->m_aid << std::endl;
					putinfostr << "itemid = " << pItem->m_itemid2 << std::endl;
#if 0
					unsigned char *pdump = (unsigned char*)&pItem->m_itemName;
					for (int ii = 0; ii < 64; ii++){
						putinfostr << std::setfill('0') << std::setw(2) << std::hex << (int)pdump[ii] << ",";
						if ((ii % 0x10) == 0x0f){
							putinfostr << std::endl;
						}
					}
					putinfostr << std::endl;
#endif

					int sx,sy;
					float fx,fy,oow;
					ProjectVertex( pItem->m_pos,pView->m_viewMatrix,&fx,&fy,&oow);
					sx = (int)fx;
					sy = (int)fy;

					m_pSFastFont->DrawText((LPSTR)putinfostr.str().c_str(), sx, sy,D3DCOLOR_ARGB(255,255,255,255),2,NULL);
				}
			}


			int actornums = p_gamemode->m_world->m_actorList.size();
			str << " m_actorList size =" << actornums << "\n";

			std::list<CGameActor*> actorList = p_gamemode->m_world->m_actorList;
			for( std::list<CGameActor*>::iterator it = actorList.begin() ; it != actorList.end() ; it++ )
			{
				CGameActor *pGameActor = *it;
				if( pGameActor && !pGameActor->m_isPc){
					long cx,cy;
					pAttr->ConvertToCellCoor(pGameActor->m_pos.x,pGameActor->m_pos.z,cx,cy);

					std::stringstream putinfostr;
					putinfostr << "(" << cx << "," << cy << ")" << std::endl;
				//	putinfostr << "dest(" << pGameActor->m_moveDestX << "," << pGameActor->m_moveDestY << ")" << std::endl;
					putinfostr << "lv = " << pGameActor->m_clevel << std::endl;
					putinfostr << "job = " << pGameActor->m_job << std::endl;
					putinfostr << "m_npcId = " << pGameActor->m_npcId << std::endl;
#if 0
					putinfostr << "m_bodyPaletteName = " << pGameActor->m_bodyPaletteName << std::endl;
					unsigned char *pdump = (unsigned char*)&pGameActor->m_pos.x;
					for (int ii = 0; ii < 64; ii++){
						putinfostr << std::setfill('0') << std::setw(2) << std::hex << (int)pdump[ii] << ",";
						if ((ii % 0x10) == 0x0f){
							putinfostr << std::endl;
						}
					}
					putinfostr << std::endl;
#endif
					int sx,sy;
					float fx,fy,oow;
					ProjectVertex( pGameActor->m_pos,pView->m_viewMatrix,&fx,&fy,&oow);
					sx = (int)fx;
					sy = (int)fy;

					m_pSFastFont->DrawText((LPSTR)putinfostr.str().c_str(), sx, sy,D3DCOLOR_ARGB(255,255,255,255),2,NULL);
				}
			}
			
		}
		str << std::endl;
		m_pSFastFont->DrawText((LPSTR)str.str().c_str(), 0, 16,D3DCOLOR_ARGB(255,255,255,255),0,NULL);

		RestoreRenderState(d3ddevice);
	}
	m_pSFastFont->Flush();

}

void CRoCodeBind::BackupRenderState(IDirect3DDevice7* d3ddevice)
{
	d3ddevice->GetRenderState(D3DRENDERSTATE_ZENABLE, &_state_zenable);
	d3ddevice->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &_state_zwriteenable);
	d3ddevice->GetRenderState(D3DRENDERSTATE_ZBIAS, &_state_zbias);
	d3ddevice->GetRenderState(D3DRENDERSTATE_FOGENABLE, &_state_fogenable);
	d3ddevice->GetRenderState(D3DRENDERSTATE_SPECULARENABLE, &_state_specularenable);
	d3ddevice->GetRenderState(D3DRENDERSTATE_ALPHAFUNC, &_state_alphafunc);
	d3ddevice->GetRenderState(D3DRENDERSTATE_ALPHAREF, &_state_alpharef);
	d3ddevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &_state_srcblend);
	d3ddevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &_state_destblend);
}

void CRoCodeBind::RestoreRenderState(IDirect3DDevice7* d3ddevice)
{
	d3ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, _state_zenable);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, _state_zwriteenable);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ZBIAS, _state_zbias);
	d3ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, _state_specularenable);
	d3ddevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, _state_specularenable);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, _state_alphafunc);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, _state_alpharef);
	d3ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, _state_srcblend);
	d3ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, _state_destblend);
}

void CRoCodeBind::DrawOn3DMap(IDirect3DDevice7* d3ddevice)
{
	if (!g_pSharedData)return;
	if (!g_pmodeMgr)return;

	if (g_pmodeMgr->m_curMode && g_pmodeMgr->m_curModeType == 1){
		CGameMode *pGamemode = (CGameMode*)g_pmodeMgr->m_curMode;
		if (!pGamemode->m_world || !pGamemode->m_view)return;
		if (!pGamemode->m_world->m_player)return;

		BackupRenderState(d3ddevice);

		DrawM2E(d3ddevice);
		DrawBBE(d3ddevice);

		RestoreRenderState(d3ddevice);
	}
}

void CRoCodeBind::DrawM2E(IDirect3DDevice7* d3ddevice)
{
	if( g_pSharedData->m2e == FALSE )return;

	CGameMode *pGamemode = (CGameMode*)g_pmodeMgr->m_curMode;
	CWorld *pWorld = pGamemode->m_world;
	CView *pView = pGamemode->m_view;

	int zbias = g_pSharedData->ground_zbias;

	d3ddevice->SetTexture(0, NULL);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ZBIAS, zbias);
	d3ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE    ,FALSE);
	d3ddevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE   ,FALSE);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC,D3DCMP_GREATER);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAREF,0x00);
	d3ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND ,D3DBLEND_SRCALPHA );
	d3ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA);

	if( pWorld && pView && pWorld->m_attr ){
		C3dAttr *pAttr = pWorld->m_attr;

		int skillnums = pWorld->m_skillList.size();
		std::list<CSkill*> skillList = pWorld->m_skillList;

		for( std::list<CSkill*>::iterator it = skillList.begin() ; it != skillList.end() ; it++ )
		{
			CSkill *pSkill = *it;

			if( pSkill && pSkill->m_job < 0x100 && m_M2ESkillColor[pSkill->m_job] ){
				DWORD color = m_M2ESkillColor[pSkill->m_job];
				CPOLVERTEX vertex[4] =
				{
					{   0.0,  0.0,   0.0f,  1.0f, color },
					{ 100.0,  0.0,   0.0f,  1.0f, color },
					{   0.0,100.0,   0.0f,  1.0f, color },
					{ 100.0,100.0,   0.0f,  1.0f, color }
				};

				long cx,cy;
				CAttrCell *pCell;
				vector3d centerpos(pSkill->m_pos),polvect[4];

				pAttr->ConvertToCellCoor(centerpos.x,centerpos.z,cx,cy);
				pCell = pAttr->GetCell(cx, cy);

				polvect[0].Set(centerpos.x -2.4f, pCell->h1 ,centerpos.z -2.4f);
				polvect[1].Set(centerpos.x +2.4f, pCell->h2 ,centerpos.z -2.4f);
				polvect[2].Set(centerpos.x -2.4f, pCell->h3 ,centerpos.z +2.4f);
				polvect[3].Set(centerpos.x +2.4f, pCell->h4 ,centerpos.z +2.4f);

				for(int ii = 0; ii < 4; ii ++){
					tlvertex3d tlv3d;
					ProjectVertex( polvect[ii] , pView->m_viewMatrix,&tlv3d );

					vertex[ii].x = tlv3d.x;
					vertex[ii].y = tlv3d.y;
					vertex[ii].z = tlv3d.z;
					vertex[ii].rhw = tlv3d.oow;
				}
				d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_CPOLVERTEX, vertex, 4, 0);
			}
		}
	}
}

void CRoCodeBind::DrawBBE(IDirect3DDevice7* d3ddevice)
{
	CGameMode *pGamemode = (CGameMode*)g_pmodeMgr->m_curMode;
	CWorld *pWorld = pGamemode->m_world;
	CView *pView = pGamemode->m_view;

	int zbias = g_pSharedData->ground_zbias;
	int alphalevel = g_pSharedData->alphalevel;
	BOOL bbe = g_pSharedData->bbe;
	BOOL deadcell = g_pSharedData->deadcell;
	BOOL chatscope = g_pSharedData->chatscope;

	d3ddevice->SetTexture(0, NULL);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ZBIAS, zbias);
	d3ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
	d3ddevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, FALSE);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
	d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, 0x00);
	d3ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	d3ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (pWorld && pView && pWorld->m_attr){
		C3dAttr *pAttr = pWorld->m_attr;
		CPlayer *pPlayer = pGamemode->m_world->m_player;
		CPOLVERTEX vertex[4] =
		{
			{ 0.0, 0.0, 0.0f, 1.0f, 0 },
			{ 100.0, 0.0, 0.0f, 1.0f, 0 },
			{ 0.0, 100.0, 0.0f, 1.0f, 0 },
			{ 100.0, 100.0, 0.0f, 1.0f, 0 }
		};
		long cx, cy,wx,wy;

		pAttr->ConvertToCellCoor(
			pPlayer->m_pos.x,pPlayer->m_pos.z,
			cx, cy);
		wx = 20;
		wy = 20;

		for (int yy = cy - wy; yy <= cy + wy; yy++) {
			for (int xx = cx - wx; xx <= cx + wx; xx++) {
				if (xx >= 0 && yy >= 0 && xx < pAttr->m_width && yy < pAttr->m_height){
					DWORD color = 0;
					CAttrCell *pCell = pAttr->GetCell(xx, yy);

					if (!pCell->flag && bbe)
					{
						if ((xx % 40 == 0) || (yy % 40 == 0)) {
							color = 0x00FF0000;
						}
						else if ((xx % 40 < 5) || (yy % 40 < 5)) {
							color = 0x000000FF;
						}
					}
					if (pCell->flag){
						if (deadcell){
							color = 0x00ff00ff;
						}
					}

					if (chatscope){
						if ((xx - cx) >= -9 && (xx - cx) <= +9 && ((yy - cy) == -9 || (yy - cy) == +9)
							|| (yy - cy) >= -9 && (yy - cy) <= +9 && ((xx - cx) == -9 || (xx - cx) == +9)){
							color = 0x0000ff00;
						}
					}

					if (color) {
						vector3d polvect[4];
						vector2d wpos;

						color += alphalevel << 24;
						pAttr->GetWorldPos((float)xx, (float)yy, wpos);

						polvect[0].Set(wpos.x - 2.5f, pCell->h1, wpos.y - 2.5f);
						polvect[1].Set(wpos.x + 2.5f, pCell->h2, wpos.y - 2.5f);
						polvect[2].Set(wpos.x - 2.5f, pCell->h3, wpos.y + 2.5f);
						polvect[3].Set(wpos.x + 2.5f, pCell->h4, wpos.y + 2.5f);

						for (int ii = 0; ii < 4; ii++){
							tlvertex3d tlv3d;
							ProjectVertex(polvect[ii], pView->m_viewMatrix, &tlv3d);

							vertex[ii].x = tlv3d.x;
							vertex[ii].y = tlv3d.y;
							vertex[ii].z = tlv3d.z;
							vertex[ii].rhw = tlv3d.oow;
							vertex[ii].color = color;
						}
						d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_CPOLVERTEX, vertex, 4, 0);
					}
				}
			}
		}
	}
}

 int CRoCodeBind::GetPacketLength(int opcode)
{
	int result = -1;

	if( opcode < m_packetLenMap_table_index )
	{
		result = m_packetLenMap_table[ opcode ];
	}
	return result;
}

int CRoCodeBind::GetTreeData(p_std_map_packetlen* node)
{
	if ( node->parent == NULL || node->key >= ROPACKET_MAXLEN || node->key == 0)
		return m_packetLenMap_table_index;

	GetTreeData( node->left );

	if( m_packetLenMap_table_index < (int)node->key )
		 m_packetLenMap_table_index = node->key;

	m_packetLenMap_table[ node->key ] = node->value;
	//DEBUG_LOGGING_DETAIL( ("[ %08X ] = %d",node->key,node->value) );

	GetTreeData( node->right );

	return m_packetLenMap_table_index;
}

void CRoCodeBind::InitPacketHandler(void)
{
	m_packethandler[HEADER_ZC_SAY_DIALOG] = &CRoCodeBind::PacketHandler_Cz_Say_Dialog;
	m_packethandler[HEADER_ZC_MENU_LIST] = &CRoCodeBind::PacketHandler_Cz_Menu_List;
}

void CRoCodeBind::PacketProc(const char *packetdata)
{
	unsigned short opcode = *(unsigned short*)packetdata;
	unsigned int packetlength;
	packetlength = GetPacketLength(opcode);
	if (m_packetqueue_head < 4)return; // packet is illegal
	if (packetlength == -1){
		packetlength = *(unsigned int*)packetdata;
		packetlength >>= 16;
	}

	// switch packet handler
	if (m_packethandler[opcode])
		// call to packetproc function.( CRoCodeBind::PacketHandler_...(const char *packetdata) )
		(this->*m_packethandler[opcode])(packetdata);

	// output packet log
	if (g_pSharedData && g_pSharedData->write_packetlog){
		std::stringstream str;
		str << "[" << std::setfill('0') << std::setw(8) << timeGetTime() << "] R ";
		for (int ii = 0; ii < (int)packetlength; ii++)
			str << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (int)(unsigned char)packetdata[ii] << " ";
		str << std::flush;
		DEBUG_LOGGING_NORMAL((str.str().c_str()));
	}

}

void CRoCodeBind::SendMessageToNPCLogger(const char *src, int size)
{
	char buffer[512];
	char *dst = buffer;
	while (size){
		if (*src == '^'){
			//
			if (src[1] == 'n'){
				if (src[2] == 'I'
				 && src[3] == 't'
				 && src[4] == 'e'
				 && src[5] == 'm'
				 && src[6] == 'I'
				 && src[7] == 'D'
				 && src[8] == '^'){
					//
					src += 9;
					size -= 9;
					//
					char numstr[10];
					unsigned char *pname;
					pname = (unsigned char *)numstr;
					while (*src >= '0' && *src <= '9'){
						*pname++ = *src++;
						size--;
					}
					*pname++ = '\0';
					int itemid;
					itemid = atoi(numstr);
					const char *itemname = GetItemNameByID(itemid);
					while (*itemname != '\0')*dst++ = *itemname++;
				}
			}
			else{
				*dst++ = *src++;
				size--;
			}
		}
		else{
			int c;
			int mbcode;
			c = (unsigned char)*src++;
			mbcode = c << 8 | (unsigned char)*src;
			if (_ismbclegal(mbcode)){
				*dst++ = c;
				c = *src++;
				size--;
			}
			*dst++ = c;
			size--;
		}
	}
	*dst++ = '\0';

	WCHAR wbuffer[512];
	int wsize = 0;
	if ((wsize = ::MultiByteToWideChar(CP_ACP, 0, buffer, -1, wbuffer, sizeof(wbuffer) / sizeof(WCHAR))) != NULL){
		//DEBUG_LOGGING_NORMAL(("sizeof wbuffer = %d wsize = %d", sizeof(wbuffer),wsize));
		HWND hNPCLoggerWnd;
		hNPCLoggerWnd = ::FindWindow(NULL, "NPCLogger");
		if (hNPCLoggerWnd){
			COPYDATASTRUCT data;
			data.dwData = COPYDATA_NPCLogger;
			data.cbData = wsize * sizeof(WCHAR);
			data.lpData = wbuffer;
			::SendMessage(hNPCLoggerWnd, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)&data);
		}
	}
}

void CRoCodeBind::PacketHandler_Cz_Say_Dialog(const char *packetdata)
{
	PACKET_CZ_SAY_DIALOG *p = (PACKET_CZ_SAY_DIALOG*)packetdata;
	int size = p->PacketLength - sizeof(PACKET_CZ_SAY_DIALOG);

	SendMessageToNPCLogger((const char*)p->Data, size);
}
void CRoCodeBind::PacketHandler_Cz_Menu_List(const char *packetdata)
{
	PACKET_CZ_MENU_LIST *p = (PACKET_CZ_MENU_LIST*)packetdata;
	int size = p->PacketLength - sizeof(PACKET_CZ_SAY_DIALOG);
	char buffer[512];
	const char *src = (const char*)p->Data;
	int index = 0;

	while (size){
		int mbcode;
		int dlength;
		char *dst = buffer;

		std::stringstream answerhead;
		answerhead << '[' << (1 + index) << "] " << std::flush;

		dlength = answerhead.str().length();

		memcpy(dst, answerhead.str().c_str(), dlength);
		dst += dlength;


		while (size && *src != ':' && *src != '\0'){
			int c;
			c = (unsigned char)*src++;
			// mbcode
			mbcode = c << 8 | (unsigned char)*src;
			if (_ismbclegal(mbcode)){
				*dst++ = c;
				c = *src++;
				size--;
				dlength++;
			}
			*dst++ = c;
			size--;
			dlength++;
		}
		if (*src == ':'){
			src++;
			size--;
		}
		*dst++ = '\0';
		if (dlength <= 5)break;


		SendMessageToNPCLogger((const char*)buffer, dlength);
		index++;
	}

}


void CRoCodeBind::PacketQueueProc(char *buf,int len)
{
	if( len > 0 ){
		int now_subMode = -1;
		if( m_packetqueue_head + len >= PACKETQUEUE_BUFFERSIZE ){
			DEBUG_LOGGING_NORMAL( ("packet buffer has overflowed.\n") );
			return;
		}

		memcpy( &m_packetqueuebuffer[m_packetqueue_head] , buf,
			len);
		m_packetqueue_head += len;
		while( m_packetqueue_head > 1 ){
			unsigned short opcode = *(unsigned short*)m_packetqueuebuffer;
			unsigned int packetlength;
			BOOL isReceivedGID = FALSE;

			if (g_pmodeMgr){
				CMode *p_mode = g_pmodeMgr->m_curMode;
				if (p_mode){
					m_CMode_old_subMode = m_CMode_subMode;
					m_CMode_subMode = p_mode->m_subMode;
					DEBUG_LOGGING_DETAIL(("CMode.m_subMode = %08X->%08X m_subModeCnt = %08X\n",
						m_CMode_old_subMode, m_CMode_subMode, p_mode->m_subModeCnt));
				}
			}

			if (m_CMode_subMode != m_CMode_old_subMode && m_CMode_subMode == LSM_WAITRESP_FROM_CHSVR ){
				packetlength = 4;
				isReceivedGID = TRUE;
			}else{
				packetlength = GetPacketLength( opcode );
				if( packetlength == -1 ){
					if( m_packetqueue_head < 4 )break;
					packetlength = *(unsigned int*)m_packetqueuebuffer;
					packetlength >>= 16;
				}
			}
			if( m_packetqueue_head >= packetlength ){
				if (isReceivedGID){
					m_gid = *(int*)m_packetqueuebuffer;
					DEBUG_LOGGING_NORMAL(("GID = %08X", m_gid));
				}else{
					DEBUG_LOGGING_DETAIL(("Opcode %04X size = %d / %d\n", opcode, packetlength, m_packetqueue_head));
					PacketProc(m_packetqueuebuffer);
				}

				m_packetqueue_head -= packetlength;
				if( m_packetqueue_head ){
					memmove( m_packetqueuebuffer, &m_packetqueuebuffer[packetlength],
						m_packetqueue_head );
				}
			}else{
				break;
			}
		}
	}
}

typedef int (__stdcall *Func_CRagConnection__GetPacketSize)(DWORD opcode);

void CRoCodeBind::SearchRagexeMemory(void)
{
	// CZ_UIYourItemWnd::SendMsg CZ_REQ_WEAR_EQUIP handler
	// Marker '1' CModeMgr g_modeMgr (C++ Class Instance)
	// Marker '2' CModeMgr::GetGameMode
	// Marker '3' CRagConnection::instanceR
	// Marker '4' CRagConnection::GetPacketSize
	// Marker '5' CRagConnection::SendPacket
	// Marker '6' UIWindowMgr g_windowMgr (C++ Class Instance)
	// Marker '7' UIWindowMgr::DeleteWindow
	// based RagFree.exe iRO vc6
	CSearchCode UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeA(
		"b9*1******"		// mov     ecx,dword L008208d0 ; CModeMgr__g_modeMgr
		"e8*2******"		// call    near F0052c0e0 ; CModeMgr__GetGameMode
		"668b536c"			// mov     dx,[ebx+06ch]
		"668b4370"			// mov     ax,[ebx+070h]
		"8d4dec"			// lea     ecx,[ebp-014h]
		"66c745eca900"		// mov     word [ebp-014h],word 000a9h
		"51"				// push    ecx
		"68a9000000"		// push    dword 0000000a9h
		"668955ee"			// mov     [ebp-012h],dx
		"668945f0"			// mov     [ebp-010h],ax
		"e8*3******"		// call    near F004190f0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*4******"		// call    near F00419030 ; CRagConnection__GetPacketSize
		"50"				// push    eax
		"e8********"		// call    near F004190f0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*5******"		// call    near F00418f00 ; CRagConnection__SendPacket
		"68**000000"		// push    dword 00000008ah
		"b9*6******"		// mov     ecx,dword L007e7220 ; UIWindowMgr__g_windowMgr
		"e8*7******"		// call    near F00502390 ; UIWindowMgr__DeleteWindow
		);
	// F2P_Ragexe.exe iRO vc9
	CSearchCode UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeB(
		"b9*1******"		// mov     ecx,dword L00812088 ; CModeMgr__g_modeMgr
		"e8*2******"		// call    near F0051eeb0 ; CModeMgr__GetGameMode
		"668b4e**"			// mov     cx,[esi+06ch]
		"668b56**"			// mov     dx,[esi+070h]
		"b8a9000000"		// mov     eax,dword 0000000a9h
		"6689442408"		// mov     [esp+008h],ax
		"8d442408"			// lea     eax,[esp+008h]
		"50"				// push    eax
		"68a9000000"		// push    dword 0000000a9h
		"66894c2412"		// mov     [esp+012h],cx
		"6689542414"		// mov     [esp+014h],dx
		"e8*3******"		// call    near F006335a0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*4******"		// call    near F006336d0 ; CRagConnection__GetPacketSize
		"50"				// push    eax
		"e8********"		// call    near F006335a0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*5******"		// call    near F00633550 ; CRagConnection__SendPacket
		"688a000000"		// push    dword 00000008ah
		"b9*6******"		// mov     ecx,dword L0083df40 ; UIWindowMgr__g_windowMgr
		"e8*7******"		// call    near F004f8270 ; UIWindowMgr__DeleteWindow
		);
	// based 2011-12-01aRagexe.exe iRO vc9
	CSearchCode UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeC(
		"b9*1******"		// mov     ecx,dword L008371f8 ; CModeMgr__g_modeMgr
		"e8*2******"		// call    near F0054a7a0 ; CModeMgr__GetGameMode
		"668b4e**"			// mov     cx,[esi+07ch]
		"668b96**000000"	// mov     dx,[esi+000000080h]
		"b8a9000000"		// mov     eax,dword 0000000a9h
		"6689442408"		// mov     [esp+008h],ax
		"8d442408"			// lea     eax,[esp+008h]
		"50"				// push    eax
		"68a9000000"		// push    dword 0000000a9h
		"66894c2412"		// mov     [esp+012h],cx
		"6689542414"		// mov     [esp+014h],dx
		"e8*3******"		// call    near F0065d4d0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*4******"		// call    near F0065cf90 ; CRagConnection__GetPacketSize
		"50"				// push    eax
		"e8********"		// call    near F0065d4d0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*5******"		// call    near F0065d3f0 ; CRagConnection__SendPacket
		"688a000000"		// push    dword 00000008ah
		"b9*6******"		// mov     ecx,dword L008626b8 ; UIWindowMgr__g_windowMgr
		"e8*7******"		// call    near F00523770 ; UIWindowMgr__DeleteWindow
		);
	// 2014-03-14aRagexe.exe jRO
	CSearchCode UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeD(
		"b9*1******"		// mov     ecx,dword L00812088 ; CModeMgr__g_modeMgr
		"e8*2******"		// call    near F0051eeb0 ; CModeMgr__GetGameMode
		"668b****"			// mov     cx,[esi+06ch]
		"668b****"			// mov     dx,[esi+070h]
		"b9a9000000"		// mov     eax,dword 0000000a9h
		"6689****"			// mov     [esp+008h],ax
		"8d****"			// lea     eax,[esp+008h]
		"51"				// push    eax
		"68a9000000"		// push    dword 0000000a9h
		"668955**"			// mov     [ebp-012h],dx
		"668945**"			// mov     [ebp-010h],ax
		"e8*3******"		// call    near F006335a0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*4******"		// call    near F006336d0 ; CRagConnection__GetPacketSize
		"50"				// push    eax
		"e8********"		// call    near F006335a0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*5******"		// call    near F00633550 ; CRagConnection__SendPacket
		"688a000000"		// push    dword 00000008ah
		"b9*6******"		// mov     ecx,dword L0083df40 ; UIWindowMgr__g_windowMgr
		"e8*7******"		// call    near F004f8270 ; UIWindowMgr__DeleteWindow
		);
	// 2014-03-18 Ragexe.exe iRO link time 20140226 155100
	CSearchCode UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeE(
		"b9*1******"        //         mov     ecx,dword L0099f930 ; CModeMgr__g_modeMgr
		"e8*2******"        //         call    near F005a5eb0 ; CModeMgr__GetGameMode
		"668b96********"    //         mov     dx,[esi+00000008ch]
		"668b86********"    //         mov     ax,[esi+000000090h]
		"b9a9000000"        //         mov     ecx,dword 0000000a9h
		"66894d**"          //         mov     [ebp-034h],cx
		"8d4d**"            //         lea     ecx,[ebp-034h]
		"51"                //         push    ecx
		"68a9000000"        //         push    dword 0000000a9h
		"668955**"          //         mov     [ebp-032h],dx
		"668945**"          //         mov     [ebp-030h],ax
		"e8*3******"        //         call    near F00716fc0 ; CRagConnection__instanceR
		"8bc8"              //         mov     ecx,eax
		"e8*4******"        //         call    near F007168e0 ; CRagConnection__GetPacketSize
		"50"                //         push    eax
		"e8********"        //         call    near F00716fc0 ; CRagConnection__instanceR
		"8bc8"              //         mov     ecx,eax
		"e8*5******"        //         call    near F00716ee0 ; CRagConnection__SendPacket
		"688a000000"        //         push    dword 00000008ah
		"b9*6******"        //         mov     ecx,dword L009d21c0 ; UIWindowMgr__g_windowMgr
		"e8*7******"        //         call    near F00573a40 ; UIWindowMgr__DeleteWindow
		);

	// 2014-08-20data-gm Ragexe.exe iRO
	CSearchCode UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeF(
		"b9*1******"        //   mov     ecx, dword L009b74f8
		"e8*2******"        //   call    near F005ac940
		"668b96********"    //   mov     dx, [esi + 00000008ch]
		"8b86********"      //   mov     eax, dword[esi + 000000090h]
		"b998090000"        //   mov     ecx, dword 000000998h
		"66894d**"          //   mov[ebp - 034h], cx
		"8d4d**"            //   lea     ecx, [ebp - 034h]
		"51"                //   push    ecx
		"6898090000"        //   push    dword 000000998h
		"668955**"          //   mov[ebp - 032h], dx
		"8945**"            //   mov     dword[ebp - 030h], eax
		"e8*3******"        //   call    near F0071fb40
		"8bc8"              //   mov     ecx, eax
		"e8*4******"        //   call    near F0071f460
		"50"                //   push    eax
		"e8********"        //   call    near F0071fb40
		"8bc8"              //   mov     ecx, eax
		"e8*5******"        //   call    near F0071fa60
		"688a000000"        //   push    dword 00000008ah
		"b9*6******"        //   mov     ecx, dword L009ebe90
		"e8*7******"        //   call    near F00579fc0
		);

	CSearchCode CMouse_Init_vc6(
		"a1********"		// mov     eax,[ g_hInstance ]
		"53"				// push    ebx
		"56"				// push    esi
		"33db"				// xor     ebx,ebx
		"57"				// push    edi
		"8bf1"				// mov     esi,ecx
		"53"				// push    ebx
		"56"				// push    esi
		"6800070000"		// push    dword 000000700h
		"50"				// push    eax
		);
	CSearchCode CMouse_Init_vc9(
		"a1********"		// mov     eax,[ g_hInstance ]
		"53"				// push    ebx
		"56"				// push    esi
		"33db"				// xor     ebx,ebx
		"53"				// push    ebx
		"8bf1"				// mov     esi,ecx
		"56"				// push    esi
		"6800070000"		// push    dword 000000700h
		"50"				// push    eax
		);
	CSearchCode winmain_init_CMouse_Init_call(
		"b9*1******"		// mov     ecx,g_mouse
		"e8*2******"		// call    near CMouse__Init
		"a1*3******"		// mov     eax, g_renderer__CRenderer
		);

	CSearchCode funcPlayStrem_based_HighPrest_exe(
		"55"                //   push    ebp
		"8bec"              //   mov     ebp,esp
		"a1*1******"        //   mov     eax,[L006f2534] ; g_soundMode
		"53"                //   push    ebx
		"85c0"              //   test    eax,eax
		"56"                //   push    esi
		"0f84c5000000"      //   jz      near C0040afa7
		"8b5d08"            //   mov     ebx,dword [ebp+008h] ;+streamFileName
		"be********"        //   mov     esi,dword L0074e7c4
		"8bc3"              //   mov     eax,ebx
		"8a10"              //   mov     dl,[eax]
		"8aca"              //   mov     cl,dl
		"3a16"              //   cmp     dl,[esi]
		"751c"              //   jnz     C0040af10                            
		"84c9"              //   test    cl,cl
		);
	CSearchCode funcPlayStrem_based_RagFree_exe(
		"55"                //   push    ebp
		"8bec"              //   mov     ebp,esp
		"81ec00010000"      //   sub     esp,dword 000000100h
		"a1*1******"        //   mov     eax,[L00775d44] ; g_soundMode
		"53"                //   push    ebx
		"85c0"              //   test    eax,eax
		"56"                //   push    esi
		"0f8417010000"      //   jz      near C0041b6bf
		"8b5d08"            //   mov     ebx,dword [ebp+008h] ;+streamFileName
		"be********"        //   mov     esi,dword L007cd9b4
		"8bc3"              //   mov     eax,ebx
		"8a10"              //   mov     dl,[eax]
		"8aca"              //   mov     cl,dl
		"3a16"              //   cmp     dl,[esi]
		"751c"              //   jnz     C0041b5d6                            
		"84c9"              //   test    cl,cl
		);
	CSearchCode funcPlayStrem_based_2011111201aRagexe_exe(
		"81ec04010000"      //   sub     esp,dword 000000104h
		"a1*1******"        //   mov     eax,[L00845990] ; g_soundMode
		"33c4"              //   xor     eax,esp
		"89842400010000"    //   mov     dword [esp+000000100h],eax
		"833d********00"    //   cmp     dword [L0084459c],byte +000h
		"56"                //   push    esi
		"8bb4240c010000"    //   mov     esi,dword [esp+00000010ch] ;+streamFileName
		"0f8406010000"      //   jz      near C0065fd9f
		"b9********"        //   mov     ecx,dword L008e348c
		"8bc6"              //   mov     eax,esi
		"8a10"              //   mov     dl,[eax]
		"3a11"              //   cmp     dl,[ecx]
		"751a"              //   jnz     C0065fcc0                                  
		"84d2"              //   test    dl,dl
		);
	CSearchCode funcPlayStrem_based_20140226_155100iRagexe_exe(
		// F00719260
		"55"                //   push    ebp
		"8bec"              //   mov     ebp,esp
		"81ec********"      //   sub     esp,dword 000000104h
		"a1*1******"        //   mov     eax,[L009b63d0] ; g_soundMode
		"33c5"              //   xor     eax,ebp
		"8945**"            //   mov     dword [ebp-004h],eax
		"833d********00"    //   cmp     dword [L00aa1610],byte +000h
		"56"                //   push    esi
		"8b75**"            //   mov     esi,dword [ebp+008h]
		"0f84********"      //   jz      near C00719393
		);

	CSearchCode initCConnection_20140318iRagexe( //  winmain...
//		"56"                 //  push    esi
//		"68********"         //  push    dword S00920704; "ws2_32.dll"
//		"ff15********"       //  call    dword[L008e619c]; ds:LoadLibraryA
//		"8b35********"       //  mov     esi, dword[L008e6194]; ds:GetProcAddress
//		"68********"         //  push    dword L009206e8 ; "send"
//		"50"                 //  push    eax
//		"a3********"         //  mov[L00a68674], eax; CConnection_s_wsmodule
//		"ffd6"               //  call    esi; GetProcAddress
//		"a3********"         //  mov[L00a68678], eax; CConnection_s_wsSend
//		"a1********"         //  mov     eax, [L00a68674]; CConnection_s_wsmodule
//		"68********"         //  push    dword S0092078c; "recv"
//		"50"                 //  push    eax; hModule
		"ffd6"               //  call    esi; GetProcAddress
		"833d*1******00"     //  cmp     dword[L00a68678], byte + 000h; CConnection_s_wsSend
		"8b35********"       //  mov     esi, dword[L008e66e8]; ds:MessageBoxA
		"a3*2******"         //  mov[L00a68670], eax; CConnection_s_wsRecv
		"751c"               //  jnz     C007170db
		"8b0d********"       //  mov     ecx, dword[WS2_32.dll_13]; ds:send
		"6a00"               //  push    byte + 000h; uType
	);

	CSearchCode strings_event_grf(0, "event.grf");
	LPBYTE strings_event_grf_address = NULL;
	CSearchCode addPak_event_grf(
		"68*1******"         //  push    dword *"event.grf"
		"b9*2******"         //  mov     ecx, dword CFileMgr::g_fileMgr
		"e8*3******"         //  call    near CFileMgr::AddPak
	);

	CSearchCode strings_readfolder(0, "readfolder");
	LPBYTE strings_readfolder_address = NULL;
	CSearchCode set_g_readFolderFirst(
		"68*1******"         //  push    dword *"readfolder"
		"8bce"               //  mov     ecx, esi
		"e8********"         //  call    near XMLElement::FindChild(char const *)
		"85c0"               //  test    eax, eax
		"7407"               //  jz      C005a43ce
		"c605*2******01"     //  mov     byte[g_readFolderFirst], byte 001h // bool g_readFolderFirst
	);
	PBOOL pg_readFolderFirst = NULL;

	CSearchCode subfunction_CFileMgr__Open(
		"803d*1******00"     //  cmp     byte[ g_readFolderFirst ], byte 000h
		"53"                 //  push    ebx
		"8b5d08"             //  mov     ebx, dword[ebp + 008h]
		"57"                 //  push    edi
		"8b7d0c"             //  mov     edi, dword[ebp + 00ch]
		"57"                 //  push    edi
		"53"                 //  push    ebx
		"7419"               //  jz      C005c79ac
		"e8*2******"         //  call    near CFileMgr::GetFile
		"85c0"               //  test    eax, eax
		"7522"               //  jnz     C005c79be;; goto
		"57"                 //  push    edi
		"53"                 //  push    ebx
		"8bce"               //  mov     ecx, esi
		"e8*3******"         //  call    near CFileMgr::GetPak
	);


	LPBYTE pRagexeBase;
	MEMORY_BASIC_INFORMATION mbi,mbi_data;
	DWORD temp_eax, temp_ecx, temp_edx, temp_esp;

	pRagexeBase = (LPBYTE)::GetModuleHandle(NULL);
	pRagexeBase += 0x1000;

	if (::VirtualQuery(pRagexeBase, &mbi, sizeof(mbi)) &&
		::VirtualQuery((LPBYTE)mbi.BaseAddress + mbi.RegionSize, &mbi_data, sizeof(mbi_data)))
	{
		DWORD ptr_CMouse_Init = 0;

		p_std_map_packetlen *packetLenMap = 0;

		DEBUG_LOGGING_NORMAL( ("MEMORY_BASIC_INFORMATION lpAddres:%08X",pRagexeBase) );
		DEBUG_LOGGING_NORMAL( ("mbi.AllocationBase = %08X",mbi.AllocationBase) );
		DEBUG_LOGGING_NORMAL( ("mbi.BaseAddress    = %08X",mbi.BaseAddress) );
		DEBUG_LOGGING_NORMAL( ("mbi.RegionSize     = %08X",mbi.RegionSize) );
		DEBUG_LOGGING_NORMAL(("MEMORY_BASIC_INFORMATION lpAddres:%08X", (LPBYTE)mbi.BaseAddress + mbi.RegionSize));
		DEBUG_LOGGING_NORMAL(("mbi_data.AllocationBase = %08X", mbi_data.AllocationBase));
		DEBUG_LOGGING_NORMAL(("mbi_data.BaseAddress    = %08X", mbi_data.BaseAddress));
		DEBUG_LOGGING_NORMAL(("mbi_data.RegionSize     = %08X", mbi_data.RegionSize));

		mbi.RegionSize += mbi_data.RegionSize;

		// get s_CMouse instance
		for (UINT ii = 0; ii < mbi.RegionSize - 1000; ii++)
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;

			if (initCConnection_20140318iRagexe.PatternMatcher(&pBase[ii]))
			{
				pCConnection_s_wsSend = (tWS2_32_send*)initCConnection_20140318iRagexe.GetImmediateDWORD(&pBase[ii], '1');
				pCConnection_s_wsRecv = (tWS2_32_recv*)initCConnection_20140318iRagexe.GetImmediateDWORD(&pBase[ii], '2');
				DEBUG_LOGGING_NORMAL(("Find s_wsSend,s_wsRecv baseaddress = %08X send = %08X | recv =%08X",
					&pBase[ii], pCConnection_s_wsSend, pCConnection_s_wsRecv));

				break;
			}
		}

		// snatch the packetLenMap
		for( UINT ii = 0; ii < mbi.RegionSize - 1000 ; ii++ )
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;

			if( UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeA.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeA.Get4BIndexDWORD( &pBase[ii] , '4' );
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov temp_esp,esp

				p_std_map_packetlen *plen = (p_std_map_packetlen*)
					*(DWORD*) (*(DWORD*)(temp_esp-19*4) + 4);

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeA
					.GetImmediateDWORD( &pBase[ii], '1' );

				DEBUG_LOGGING_NORMAL( ("TypeA GetPacketSizeAddr %08X",GetPacketSizeAddr) );
				DEBUG_LOGGING_NORMAL( (" esp = %08X",temp_esp) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						DEBUG_LOGGING_NORMAL( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}else
			if( UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeB.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeB.Get4BIndexDWORD( &pBase[ii] , '4' );
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov temp_eax,eax
				__asm mov temp_ecx,ecx
				__asm mov temp_edx,edx
				p_std_map_packetlen *plen;
				if( temp_eax == temp_edx ){
					plen = (p_std_map_packetlen*)temp_ecx;
				}else{
					plen = (p_std_map_packetlen*)temp_edx;
				}

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeB
					.GetImmediateDWORD( &pBase[ii], '1' );

				DEBUG_LOGGING_NORMAL( ("TypeB GetPacketSizeAddr     = %08X eax = %08X ecx = %08X edx =%08X",
					GetPacketSizeAddr,temp_eax,temp_ecx,temp_edx) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						DEBUG_LOGGING_NORMAL( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}else
			if( UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeC.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				Func_CRagConnection__GetPacketSize GetPacketSize;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeC.Get4BIndexDWORD( &pBase[ii] , '4' );
				GetPacketSize = (Func_CRagConnection__GetPacketSize)GetPacketSizeAddr;
				//GetPacketSize(0x0A9);
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov temp_ecx,ecx
				p_std_map_packetlen *plen = (p_std_map_packetlen*)temp_ecx;

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeC
					.GetImmediateDWORD( &pBase[ii], '1' );

				DEBUG_LOGGING_NORMAL( ("TypeC GetPacketSizeAddr     = %08X ecx = %08X",GetPacketSizeAddr,temp_ecx) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						DEBUG_LOGGING_NORMAL( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}else
			if( UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeD.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				Func_CRagConnection__GetPacketSize GetPacketSize;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeD.Get4BIndexDWORD( &pBase[ii] , '4' );
				GetPacketSize = (Func_CRagConnection__GetPacketSize)GetPacketSizeAddr;
				//GetPacketSize(0x0A9);
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov temp_ecx,ecx
				p_std_map_packetlen *plen = (p_std_map_packetlen*)temp_ecx;

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeD
					.GetImmediateDWORD( &pBase[ii], '1' );

				DEBUG_LOGGING_NORMAL( ("TypeD GetPacketSizeAddr     = %08X ecx = %08X",GetPacketSizeAddr,temp_ecx) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						DEBUG_LOGGING_NORMAL( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}else
			if( UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeE.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				Func_CRagConnection__GetPacketSize GetPacketSize;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeE.Get4BIndexDWORD( &pBase[ii] , '4' );
				GetPacketSize = (Func_CRagConnection__GetPacketSize)GetPacketSizeAddr;
				//GetPacketSize(0x0A9);
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov temp_ecx,ecx
				p_std_map_packetlen *plen = (p_std_map_packetlen*)temp_ecx;

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeE
					.GetImmediateDWORD( &pBase[ii], '1' );

				DEBUG_LOGGING_NORMAL( ("TypeE GetPacketSizeAddr     = %08X ecx = %08X",GetPacketSizeAddr,temp_ecx) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						DEBUG_LOGGING_NORMAL( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}else
			if (UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeF.PatternMatcher(&pBase[ii]))
			{
				DWORD GetPacketSizeAddr;
				Func_CRagConnection__GetPacketSize GetPacketSize;
				GetPacketSizeAddr =
					UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeF.Get4BIndexDWORD(&pBase[ii], '4');
				GetPacketSize = (Func_CRagConnection__GetPacketSize)GetPacketSizeAddr;
				//GetPacketSize(0x0998);
				__asm push 0x0998
				__asm call GetPacketSizeAddr
				__asm mov temp_ecx, ecx
				p_std_map_packetlen *plen = (p_std_map_packetlen*)temp_ecx;

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_REQ_WEAR_EQUIP_Handler_TypeF
					.GetImmediateDWORD(&pBase[ii], '1');

				DEBUG_LOGGING_NORMAL(("TypeE GetPacketSizeAddr     = %08X ecx = %08X", GetPacketSizeAddr, temp_ecx));
				while (1)
				{
					if (plen->key > 0xffff || (plen->key == 0 && plen->value == 0)){
						packetLenMap = plen;
						DEBUG_LOGGING_NORMAL(("packetLenMap = %08X", packetLenMap));
						break;
					}
					plen = plen->parent;
				}
				break;
			}
		}

		// get CMouse instance
		for( UINT ii = 0; ii < mbi.RegionSize - CMouse_Init_vc6.GetSize() ; ii++ )
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;
			if( CMouse_Init_vc6.PatternMatcher( &pBase[ii] )
			 || CMouse_Init_vc9.PatternMatcher( &pBase[ii] )
				)
			{
				ptr_CMouse_Init = (DWORD)( &pBase[ii] );
				DEBUG_LOGGING_NORMAL( ("find CMouse::Init = %08X",pBase + ii) );
				break;
			}
		}
		if( ptr_CMouse_Init )
		{
			for( int ii = mbi.RegionSize - winmain_init_CMouse_Init_call.GetSize(); ii >= 0 ; ii-- )
			{
				LPBYTE pBase = (LPBYTE)mbi.BaseAddress;
				if( winmain_init_CMouse_Init_call.PatternMatcher( &pBase[ii] ) )
				{
					DEBUG_LOGGING_NORMAL( ("find CMouse::Init call : %08X",pBase + ii) );
					if( winmain_init_CMouse_Init_call.NearJmpAddressMatcher( &pBase[ii],'2',ptr_CMouse_Init ) )
					{
						g_mouse = (CMouse*)winmain_init_CMouse_Init_call.GetImmediateDWORD(&pBase[ii],'1');
						g_renderer = (CRenderer**)winmain_init_CMouse_Init_call.GetImmediateDWORD(&pBase[ii],'3');
						
						DEBUG_LOGGING_NORMAL( ("find g_mouse = %08X",g_mouse) );
						DEBUG_LOGGING_NORMAL( ("find *g_renderer = %08X",g_renderer) );
						break;
					}
				}
			}
		}

		// get the address of PlayStream function 
		for( UINT ii = 0; ii < mbi.RegionSize - funcPlayStrem_based_HighPrest_exe.GetSize() ; ii++ )
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;

			if( funcPlayStrem_based_20140226_155100iRagexe_exe.PatternMatcher( &pBase[ii] )	)
			{
				m_funcRagexe_PlayStream = (tPlayStream)&pBase[ii];
				DWORD pPlayStream;
				pPlayStream = (DWORD)&pBase[ii];
				DEBUG_LOGGING_NORMAL( ("based_20140226_155100iRagexe : %08X",&pBase[ii]) );
				DEBUG_LOGGING_NORMAL( ("g_soundMode == %08X",(char*)funcPlayStrem_based_20140226_155100iRagexe_exe.GetImmediateDWORD(&pBase[ii],'1')) );
				DEBUG_LOGGING_NORMAL( ("void PlayStream(const char *streamFileName) = %08X",pPlayStream) );
				break;
			}
			if( funcPlayStrem_based_HighPrest_exe.PatternMatcher( &pBase[ii] )	)
			{
				m_funcRagexe_PlayStream = (tPlayStream)&pBase[ii];
				DWORD pPlayStream;
				pPlayStream = (DWORD)&pBase[ii];
				DEBUG_LOGGING_NORMAL( ("based_HighPrest_exe : %08X",&pBase[ii]) );
				DEBUG_LOGGING_NORMAL( ("g_soundMode == %08X",(char*)funcPlayStrem_based_HighPrest_exe.GetImmediateDWORD(&pBase[ii],'1')) );
				DEBUG_LOGGING_NORMAL( ("void PlayStream(const char *streamFileName) = %08X",pPlayStream) );
				break;
			}
			if( funcPlayStrem_based_RagFree_exe.PatternMatcher( &pBase[ii] )	)
			{
				m_funcRagexe_PlayStream = (tPlayStream)&pBase[ii];
				DWORD pPlayStream;
				pPlayStream = (DWORD)&pBase[ii];
				DEBUG_LOGGING_NORMAL( ("based_RagFree_exe : %08X",&pBase[ii]) );
				DEBUG_LOGGING_NORMAL( ("g_soundMode == %08X",(char*)funcPlayStrem_based_RagFree_exe.GetImmediateDWORD(&pBase[ii],'1')) );
				DEBUG_LOGGING_NORMAL( ("void PlayStream(const char *streamFileName,int playflag) = %08X",pPlayStream) );
				break;
			}
			if( funcPlayStrem_based_2011111201aRagexe_exe.PatternMatcher( &pBase[ii] )	)
			{
				m_funcRagexe_PlayStream = (tPlayStream)&pBase[ii];
				DWORD pPlayStream;
				pPlayStream = (DWORD)&pBase[ii];
				DEBUG_LOGGING_NORMAL( ("based_2011111201aRagexe_exe : %08X",&pBase[ii]) );
				DEBUG_LOGGING_NORMAL( ("g_soundMode == %08X",(char*)funcPlayStrem_based_2011111201aRagexe_exe.GetImmediateDWORD(&pBase[ii],'1')) );
				DEBUG_LOGGING_NORMAL( ("void PlayStream(const char *streamFileName,int playflag) = %08X",pPlayStream) );
				break;
			}
		}

		// find strings
		for (UINT ii = 0; ii < mbi.RegionSize - 1000; ii++)
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;

			if ( !strings_event_grf_address && strings_event_grf.PatternMatcher(&pBase[ii]))
			{
				strings_event_grf_address = &pBase[ii];
				DEBUG_LOGGING_NORMAL(("find 'event.grf' : %08X", strings_event_grf_address));
			}
			if (!strings_readfolder_address && strings_readfolder.PatternMatcher(&pBase[ii]))
			{
				strings_readfolder_address = &pBase[ii];
				DEBUG_LOGGING_NORMAL(("find 'readfolderf' : %08X", strings_readfolder_address));
			}
		}

		// get the address of CFileMgr::g_fileMgr
		for (UINT ii = 0; ii < mbi.RegionSize - 1000; ii++)
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;

			if (addPak_event_grf.PatternMatcher(&pBase[ii]) &&
				addPak_event_grf.GetImmediateDWORD(&pBase[ii], '1') == (DWORD)strings_event_grf_address)
			{
				m_CFileMgr__gfileMgr = (void*)addPak_event_grf.GetImmediateDWORD(&pBase[ii], '2');
				DEBUG_LOGGING_NORMAL(("find CFileMgr::g_fileMgr : %08X", m_CFileMgr__gfileMgr));
				break;
			}
		}

		// get the address of g_readFolderFirst
		for (UINT ii = 0; ii < mbi.RegionSize - 1000; ii++)
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;

			if (set_g_readFolderFirst.PatternMatcher(&pBase[ii]) &&
				set_g_readFolderFirst.GetImmediateDWORD(&pBase[ii], '1') == (DWORD)strings_readfolder_address)
			{
				pg_readFolderFirst = (PBOOL)set_g_readFolderFirst.GetImmediateDWORD(&pBase[ii], '2');
				DEBUG_LOGGING_NORMAL(("find g_readFolderFirst : %08X", pg_readFolderFirst));
				break;
			}
		}

		// get the address of g_readFolderFirst
		for (UINT ii = 0; ii < mbi.RegionSize - 1000; ii++)
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;

			if (subfunction_CFileMgr__Open.PatternMatcher(&pBase[ii]) &&
				subfunction_CFileMgr__Open.GetImmediateDWORD(&pBase[ii], '1') == (DWORD)pg_readFolderFirst)
			{
				m_functionRagexe_CFileMgr__GetPak = (tCFileMgr__GetPak)subfunction_CFileMgr__Open.GetNearJmpAddress(&pBase[ii], '3');
				DEBUG_LOGGING_NORMAL(("find CFileMgr::GetFile : %08X", subfunction_CFileMgr__Open.GetNearJmpAddress(&pBase[ii], '2') ));
				DEBUG_LOGGING_NORMAL(("find CFileMgr::GetPak : %08X", m_functionRagexe_CFileMgr__GetPak));
				break;
			}
		}
		{
			void *address = NULL;
			unsigned int size = 0;

			DEBUG_LOGGING_NORMAL(("call CFileMgr::GetPak"));
			address = GetPak("data\\idnum2itemdisplaynametable.txt", &size);

			DEBUG_LOGGING_NORMAL(("load data\\idnum2itemdisplaynametable.txt %08X size of %d",
				address, size));

			DEBUG_LOGGING_NORMAL(("release data\\idnum2itemdisplaynametable.txt"));
			ReleasePak(address);
		}

		// put packetlengthmap
		if( packetLenMap ){
			int packetnums = GetTreeData( packetLenMap->parent );
			if( packetnums ){
				std::stringstream onelinestr;
				for(int ii = 0;ii < packetnums ;ii++){
					if( (ii % 0x40)==0 ){
						onelinestr << "# 0x" << std::setfill('0') << std::setw(4) << std::hex << ii;
						DEBUG_LOGGING_NORMAL(( onelinestr.str().c_str() ));
						onelinestr.str("");
					}
					if( (ii % 0x10)==0 ){
						onelinestr << " ";
					}
					onelinestr << std::setfill(' ') << std::setw(4) << std::dec << m_packetLenMap_table[ii] << ",";
					if( (ii % 0x10)==0x0f ){
						DEBUG_LOGGING_NORMAL(( onelinestr.str().c_str() ));
						onelinestr.str("");
					}
				}
				DEBUG_LOGGING_NORMAL(( onelinestr.str().c_str() ));
				DEBUG_LOGGING_NORMAL(( "" ));
			}
		}

	}
}

