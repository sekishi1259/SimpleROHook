#include "stdafx.h"
#include "tinyconsole.h"
#include "RoCodeBind.h"
#include "ProxyDirectDraw.h"

#define kFloorSkillTypeMAX 0x100
DWORD g_M2ESkillColor[kFloorSkillTypeMAX];

void LoadIni(void)
{
	if( g_pSharedData){

		int sectionsize;
		char Sectionbuf[32768];
		char *pkey;
		int result_ws2ms;
		char filename[MAX_PATH];

		if( ::WideCharToMultiByte(CP_ACP,0,
			g_pSharedData->configfilepath,wcslen(g_pSharedData->configfilepath)+1,
			filename,sizeof(filename),
			NULL,NULL) ){

			sectionsize = GetPrivateProfileSection(_T("M2E"),Sectionbuf,32768,filename);
			pkey = Sectionbuf;

			for(int ii = 0;ii < kFloorSkillTypeMAX;ii++)
				g_M2ESkillColor[ii]=0;

			while(*pkey!='\0'){
				int index;
				DWORD color;
				char view[64];
				char *ptemp;
				ptemp = pkey;
				strcpy(view, ptemp);
				pkey +=strlen(ptemp);
				sscanf(view,"Skill%x=%x",&index,&color);
				g_M2ESkillColor[index] = color;
				pkey++;
			}
		}
	}
}

CSFastFont *g_pSFastFont = NULL;
LPDIRECTDRAWSURFACE7 g_pddsFontTexture = NULL;

void DrawSRHDebug(IDirect3DDevice7* d3ddevice)
{
	if( !g_pSharedData )return;

	if( g_pSharedData->show_framerate ){
		std::stringstream str;
		str << g_PerformanceCounter.GetFrameRate() << "fps : "<<(int)g_PerformanceCounter.GetTotalTick() << std::endl;
/*
		str << "g_pSharedData = " << g_pSharedData << "\n";
		str << "g_pSharedData->g_hROWindow = " << g_pSharedData->g_hROWindow << "\n";
		str << "g_pSharedData->freemouse = " << g_pSharedData->freemouse << "\n";
		str << "g_pSharedData->m2e = " << g_pSharedData->m2e << "\n";
		str << "g_pSharedData->show_framerate = " << g_pSharedData->show_framerate << "\n";
		str << "g_pSharedData->executeorder = " << g_pSharedData->executeorder << "\n";
		char filename[MAX_PATH];
		if( ::WideCharToMultiByte(CP_ACP,0,
			g_pSharedData->musicfilename,wcslen(g_pSharedData->musicfilename)+1,
			filename,sizeof(filename),
			NULL,NULL) ){
			str << "musicfile = " << filename << "\n";
		}
		str << std::endl;
*/

		g_pSFastFont->DrawText((LPSTR)str.str().c_str(), 0, 0,D3DCOLOR_ARGB(255,255,255,255),0,NULL);
	}

	if( g_pSharedData->objectinformation ){
		std::stringstream str;
		CModeMgr *pcmode = g_pmodeMgr;
		//str.str("");
		CGameMode *p_gamemode = (CGameMode*)pcmode->m_curMode;

		if( p_gamemode && pcmode->m_curModeType == 1 
		 && p_gamemode->m_world && p_gamemode->m_view && p_gamemode->m_world->m_attr ){
			CView *pView = p_gamemode->m_view;

			C3dAttr *pAttr = p_gamemode->m_world->m_attr;

			if( p_gamemode->m_world->m_player ){
				C3dAttr *pattr = p_gamemode->m_world->m_attr;
				ViewInfo3d *pVinfo = &pView->m_cur;
				vector3d pos;
				vector3d tpos,spos;
				float rhw;
				std::stringstream putinfostr;
				CPlayer *pPlayer = (CPlayer*)p_gamemode->m_world->m_player;

				str << "m_clevel = " << pPlayer->m_clevel << "\n";
				str << "m_gid = " << std::hex << (unsigned long)pPlayer->m_gid << "\n";
				str << "m_job = " << std::hex << (unsigned long)pPlayer->m_job << "\n";
				str << "m_sex = " << std::hex << (unsigned long)pPlayer->m_sex << "\n";


				//
				//  world position to cell position
				//
				pos.x = pPlayer->m_pos.x - pVinfo->at.x;
				pos.y = pPlayer->m_pos.y - pVinfo->at.y;
				pos.z = pPlayer->m_pos.z - pVinfo->at.z;
				//
				tpos.MatrixMult(pos,pView->m_viewMatrix);
				tpos.z += pVinfo->distance;
				long cx,cy;
				pattr->ConvertToCellCoor(pPlayer->m_pos.x,pPlayer->m_pos.z,cx,cy);
				//
				//
				//
				putinfostr << "(" << cx << "," << cy << ")" << std::endl;
				int sx,sy;
				Transform2screen(tpos,spos,rhw);
				sx = (int)spos.x;
				sy = (int)spos.y;
				g_pSFastFont->DrawText((LPSTR)putinfostr.str().c_str(), sx, sy,D3DCOLOR_ARGB(255,255,255,255),2,NULL);
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
					ViewInfo3d *pVinfo = &pView->m_cur;
					vector3d pos;
					vector3d tpos,spos;
					float rhw;
					pos.x = pItem->m_pos.x - pVinfo->at.x;
					pos.y = pItem->m_pos.y - pVinfo->at.y;
					pos.z = pItem->m_pos.z - pVinfo->at.z;
					//
					tpos.MatrixMult(pos,pView->m_viewMatrix);
					tpos.z += pVinfo->distance;

					Transform2screen(tpos,spos,rhw);

					long cx,cy;
					pAttr->ConvertToCellCoor(pItem->m_pos.x,pItem->m_pos.z,cx,cy);

					std::stringstream putinfostr;
					putinfostr << "(" << cx << "," << cy << ")" << std::endl;
					//putinfostr << pItem->m_itemName << std::endl;
				//	putinfostr << "aid = " << pItem->m_aid << std::endl;
					putinfostr << "itemid = " << pItem->m_itemid2 << std::endl;

					int sx,sy;
					sx = (int)spos.x;
					sy = (int)spos.y;
					g_pSFastFont->DrawText((LPSTR)putinfostr.str().c_str(), sx, sy,D3DCOLOR_ARGB(255,255,255,255),2,NULL);
				}
			}


			int actornums = p_gamemode->m_world->m_actorList.size();
			str << " m_actorList size =" << actornums << "\n";
			std::list<CGameActor*> actorList = p_gamemode->m_world->m_actorList;
			for( std::list<CGameActor*>::iterator it = actorList.begin() ; it != actorList.end() ; it++ )
			{
				CGameActor *pGameActor = *it;
				if( pGameActor ){
					ViewInfo3d *pVinfo = &pView->m_cur;
					vector3d pos;
					vector3d tpos,spos;
					float rhw;
					pos.x = pGameActor->m_pos.x - pVinfo->at.x;
					pos.y = pGameActor->m_pos.y - pVinfo->at.y;
					pos.z = pGameActor->m_pos.z - pVinfo->at.z;
					//
					tpos.MatrixMult(pos,pView->m_viewMatrix);
					tpos.z += pVinfo->distance;

					Transform2screen(tpos,spos,rhw);

					long cx,cy;
					pAttr->ConvertToCellCoor(pGameActor->m_pos.x,pGameActor->m_pos.z,cx,cy);

					std::stringstream putinfostr;
					putinfostr << "(" << cx << "," << cy << ")" << std::endl;
				//	putinfostr << "dest(" << pGameActor->m_moveDestX << "," << pGameActor->m_moveDestY << ")" << std::endl;
					putinfostr << "lv = " << pGameActor->m_clevel << std::endl;
					putinfostr << "job = " << pGameActor->m_job << std::endl;

					int sx,sy;
					sx = (int)spos.x;
					sy = (int)spos.y;
					g_pSFastFont->DrawText((LPSTR)putinfostr.str().c_str(), sx, sy,D3DCOLOR_ARGB(255,255,255,255),2,NULL);
				}
			}
			
		}
		str << std::endl;
		g_pSFastFont->DrawText((LPSTR)str.str().c_str(), 0, 16,D3DCOLOR_ARGB(255,255,255,255),0,NULL);
	}
	g_pSFastFont->Flush();

}

void DrawM2E(IDirect3DDevice7* d3ddevice)
{
	if( !g_pSharedData )return;
	if( g_pSharedData->m2e == FALSE )return;

	CModeMgr *pcmode = g_pmodeMgr;
	CWorld *pWorld;
	CView *pView;

	if( pcmode->m_curMode && pcmode->m_curModeType == 1 ){
		CGameMode *pGamemode = (CGameMode*)pcmode->m_curMode;
		pWorld = pGamemode->m_world;
		pView  = pGamemode->m_view;

		DWORD _state_zenable;
		DWORD _state_zwriteenable;
		DWORD _state_zbias;
		DWORD _state_fogenable;
		DWORD _state_specularenable;
		LPDIRECTDRAWSURFACE7 lpOriginalTexture = NULL;

		d3ddevice->GetRenderState(D3DRENDERSTATE_ZENABLE,        &_state_zenable);
		d3ddevice->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE,   &_state_zwriteenable);
		d3ddevice->GetRenderState(D3DRENDERSTATE_ZBIAS,          &_state_zbias);
		d3ddevice->GetRenderState(D3DRENDERSTATE_FOGENABLE,      &_state_fogenable);
		d3ddevice->GetRenderState(D3DRENDERSTATE_SPECULARENABLE, &_state_specularenable);
	//	d3ddevice->GetTexture(0, &lpOriginalTexture);

		d3ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZBIAS, 2);
		d3ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE    ,FALSE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE   ,FALSE);
		d3ddevice->SetTexture(0, NULL);

		if( pWorld && pView && pWorld->m_attr ){
			C3dAttr *pAttr = pWorld->m_attr;

			int skillnums = pWorld->m_skillList.size();
			std::list<CSkill*> skillList = pWorld->m_skillList;

			for( std::list<CSkill*>::iterator it = skillList.begin() ; it != skillList.end() ; it++ )
			{
				CSkill *pSkill = *it;
				//
				if( pSkill && pSkill->m_job < 0x100 && g_M2ESkillColor[pSkill->m_job] ){
					DWORD color = g_M2ESkillColor[pSkill->m_job];
					VERTEX vertex[4] =
					{
						{   0.0,  0.0,   0.0f,  1.0f, color },
						{ 100.0,  0.0,   0.0f,  1.0f, color },
						{   0.0,100.0,   0.0f,  1.0f, color },
						{ 100.0,100.0,   0.0f,  1.0f, color }
					};
					ViewInfo3d *pVinfo = &pView->m_cur;
					vector3d pos;
					vector3d tpos,spos;
					float rhw;
					pos.x = pSkill->m_pos.x - pVinfo->at.x;
					pos.y = pSkill->m_pos.y - pVinfo->at.y;
					pos.z = pSkill->m_pos.z - pVinfo->at.z;
					//
					tpos.MatrixMult(pos,pView->m_viewMatrix);
					tpos.z += pVinfo->distance;

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
						pos.x = polvect[ii].x - pVinfo->at.x;
						pos.y = polvect[ii].y - pVinfo->at.y;
						pos.z = polvect[ii].z - pVinfo->at.z;
						tpos.MatrixMult(pos,pView->m_viewMatrix);
						tpos.z += pVinfo->distance;
						Transform2screen(tpos,spos,rhw);
						vertex[ii].x = spos.x;
						vertex[ii].y = spos.y;
						vertex[ii].z = spos.z;
						vertex[ii].rhw = rhw;
					}
					d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_CPOLVERTEX, vertex, 4, 0);
				}
			}
		}
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE,        _state_zenable);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,   _state_zwriteenable);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZBIAS,          _state_zbias);
		d3ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE,      _state_specularenable);
		d3ddevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, _state_specularenable);
	//	d3ddevice->SetTexture(0, lpOriginalTexture);



	}
}

int g_executecount = 0;
double g_VSyncWaitTick = 0.0;
HRESULT CProxy_IDirectDrawSurface7::Proxy_Flip(
			LPDIRECTDRAWSURFACE7 lpDDSurfaceTargetOverride, DWORD dwFlags)
{
//	if(g_pRCXDraw)g_pRCXDraw->DrawSyncFunc();

	//if(BackSurface && g_pRCXDraw){
	//	g_pRCXDraw->Draw(BackSurface);
	//}
	return m_Instance->Flip(lpDDSurfaceTargetOverride, dwFlags);
}

HRESULT CProxy_IDirectDrawSurface7::Proxy_GetAttachedSurface(
				LPDDSCAPS2 ddsCaps, LPDIRECTDRAWSURFACE7 FAR * Surface)
{
	kDD_LOGGING(("IDirectDrawSurface7::GetAttachedSurface()  ddsCaps.dwCaps = 0x%x", ddsCaps->dwCaps));
	HRESULT Result = m_Instance->GetAttachedSurface(ddsCaps, Surface);
	if(FAILED(Result))
		return Result;

	if(ddsCaps->dwCaps == DDSCAPS_BACKBUFFER){
		kDD_LOGGING(("Atached BackBuffer Surface."));
		m_BackSurface = *Surface;
	}
	
	return Result;
}

IDirectDraw7*           CProxy_IDirectDraw7::StaticInstance;
CProxy_IDirectDraw7*    CProxy_IDirectDraw7::lpthis;

CProxy_IDirectDraw7::CProxy_IDirectDraw7(IDirectDraw7* ptr)
{
	m_Instance = ptr;
	StaticInstance = ptr;
	lpthis   = NULL;
	kDD_LOGGING(("IDirectDraw7::Create"));
}

CProxy_IDirectDraw7::~CProxy_IDirectDraw7()
{
	kDD_LOGGING(("IDirectDraw7::Releace"));
}

ULONG CProxy_IDirectDraw7::Proxy_Release(void)
{
	ULONG Count;

	Count = m_Instance->Release();
	kDD_LOGGING(("cProxy_IDirectDraw7::Release()  RefCount = %d", Count));
	delete this;

	return Count;
}

HRESULT CProxy_IDirectDraw7::Proxy_RestoreAllSurfaces(void)
{
	return m_Instance->RestoreAllSurfaces();
}


HRESULT CProxy_IDirectDraw7::Proxy_QueryInterface(THIS_ REFIID riid, LPVOID FAR * ppvObj)
{
	kDD_LOGGING(("CProxy_IDirectDraw7::QueryInterface()"));

	if( riid.Data1 == IID_IDirect3D7.Data1
	 && riid.Data2 == IID_IDirect3D7.Data2
	 && riid.Data3 == IID_IDirect3D7.Data3
	 && *(UINT64*)riid.Data4 == *(UINT64*)IID_IDirect3D7.Data4 )
	{
		kDD_LOGGING(("CProxy_IDirectDraw7::IDirect3D7 create"));
		HRESULT temp_ret = m_Instance->QueryInterface(riid, ppvObj);
		if(temp_ret == S_OK){
			void *ret_cProxy;
			//LPVOID FAR * ppvObj_proxy;
			ret_cProxy = new CProxy_IDirect3D7( (IDirect3D7*) *ppvObj);
			*ppvObj = ret_cProxy;

			return S_OK;
		}else{
			return temp_ret;
		}
	}

	return m_Instance->QueryInterface(riid, ppvObj);
}

HRESULT CProxy_IDirectDraw7::Proxy_CreateSurface(LPDDSURFACEDESC2 SurfaceDesc,
			LPDIRECTDRAWSURFACE7 FAR * CreatedSurface, IUnknown FAR * pUnkOuter)
{
	DDSURFACEDESC2 OrgSurfaceDesc2;// = *SurfaceDesc;
	OrgSurfaceDesc2 = *SurfaceDesc;

	kDD_LOGGING(("IDirectDraw7::CreateSurface()  Desc.dwFlags = 0x%x", SurfaceDesc->dwFlags));
	kDD_LOGGING(("DDSD_BACKBUFFERCOUNT = %x",DDSD_BACKBUFFERCOUNT)); 
	HRESULT Result = m_Instance->CreateSurface(SurfaceDesc, CreatedSurface, pUnkOuter);
	if(FAILED(Result))
		return Result;

	if(SurfaceDesc->dwFlags & DDSD_CAPS){
		DDSCAPS2* Caps = &SurfaceDesc->ddsCaps;
		kDD_LOGGING(("    Desc.ddsCaps.dwCaps = 0x%x", Caps->dwCaps));
		if(Caps->dwCaps == DDSCAPS_BACKBUFFER){
			kDD_LOGGING(("BackBuffer Surface"));
		}
		kDD_LOGGING(("%0x W %d H %d",*CreatedSurface, 
			SurfaceDesc->dwWidth,
			SurfaceDesc->dwHeight
			));
		if( Caps->dwCaps & DDSCAPS_PRIMARYSURFACE ){
			*CreatedSurface = new CProxy_IDirectDrawSurface7(*CreatedSurface);
			kDD_LOGGING(("Primary Surface = %0x",CreatedSurface));
			PrimarySurfaceFlag = 1;
		}else
		if(Caps->dwCaps & DDSCAPS_3DDEVICE){
			if(CooperativeLevel & DDSCL_FULLSCREEN && !PrimarySurfaceFlag){
				kDD_LOGGING(("Hook the 3D Device Rendering Surface."));
				kDD_LOGGING(("  FullScreen Mode"));
				*CreatedSurface = new CProxy_IDirectDrawSurface7(*CreatedSurface);
				g_screen_width  = SurfaceDesc->dwWidth;
				g_screen_height = SurfaceDesc->dwHeight;

			}else{
				kDD_LOGGING(("Hook the 3D Device Rendering Surface."));
				kDD_LOGGING(("  Window Mode"));
				TargetSurface = *CreatedSurface;
				g_screen_width  = SurfaceDesc->dwWidth;
				g_screen_height = SurfaceDesc->dwHeight;
			}
		}else{
			if(CooperativeLevel & DDSCL_FULLSCREEN){
				if(Caps->dwCaps & DDSCAPS_BACKBUFFER){
					kDD_LOGGING(("Hook Rendering Surface without 3D Device."));
					kDD_LOGGING(("  FullScreen Mode"));
					*CreatedSurface = new CProxy_IDirectDrawSurface7(*CreatedSurface);
				}
			}else{
				if(Caps->dwCaps & DDSCAPS_BACKBUFFER){
					kDD_LOGGING(("Hook Rendering Surface without 3D Device."));
					kDD_LOGGING(("  Window Mode"));
				}
			}
		}
	}

	//
	return Result;

}
HRESULT CProxy_IDirectDraw7::Proxy_GetDisplayMode(LPDDSURFACEDESC2 Desc)
{
	kDD_LOGGING(("IDirectDraw7::GetDisplayMode()"));
	HRESULT Result = m_Instance->GetDisplayMode(Desc);
	if(FAILED(Result))
		return Result;
	g_MonitorRefreshRate = Desc->dwRefreshRate;
	kDD_LOGGING(("    RefreshRate = %d", Desc->dwRefreshRate));
	if( g_MonitorRefreshRate == 0){
		kDD_LOGGING(("    RefreshRate = %d >> dummyset 100", Desc->dwRefreshRate));
		g_MonitorRefreshRate = 100;
	}
	//if(g_pRCXDraw)g_pRCXDraw->SetMonitorRefreshRate(RefreshRate);
	return Result;
}

HRESULT CProxy_IDirectDraw7::Proxy_SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
{
	kDD_LOGGING(("IDirectDraw7::SetCooperativeLevel()  dwFlags = 0x%x", dwFlags));
//	if(!g_pRCXDraw){
//		g_pRCXDraw = new CRCXDraw(hWnd);
//	}
	if( g_pSharedData ){
		g_pSharedData->g_hROWindow = hWnd;
	}

	DDSURFACEDESC2 dummy;
	dummy.dwSize = sizeof(DDSURFACEDESC2);
	Proxy_GetDisplayMode(&dummy);
	CooperativeLevel = dwFlags;
	return m_Instance->SetCooperativeLevel(hWnd, dwFlags);
}

HRESULT CProxy_IDirectDraw7::Proxy_SetDisplayMode(DWORD p1, DWORD p2, DWORD p3, DWORD p4, DWORD p5)
{

	kDD_LOGGING(("IDirectDraw7::SetDisplayMode()"));
	HRESULT Result = m_Instance->SetDisplayMode(p1, p2, p3, p4, p5);
	if(FAILED(Result))
		return Result;

	kDD_LOGGING(("    %d  %d  %d  %d  %d", p1, p2, p3, p4, p5));

	DDSURFACEDESC2 Desc;
	::ZeroMemory(&Desc, sizeof(Desc));
	Desc.dwSize = sizeof(Desc);
	m_Instance->GetDisplayMode(&Desc);
	kDD_LOGGING(("    RefreshRate = %d", Desc.dwRefreshRate));
	DWORD RefreshRate = Desc.dwRefreshRate;
	if( RefreshRate == 0){
		kDD_LOGGING(("    RefreshRate = %d >> dummyset 100", Desc.dwRefreshRate));
		RefreshRate = 100;
	}
	g_PerformanceCounter.SetMonitorRefreshRate( (int)RefreshRate );
//	if(g_pRCXDraw)g_pRCXDraw->SetMonitorRefreshRate(RefreshRate);

	return Result;
}
/*
int g_normalcounter = 0;
DWORD g_TimeCounter = 0;
DWORD OldTimeCounter = g_TimeCounter;
g_TimeCounter = ::timeGetTime();
if( OldTimeCounter ){
	if( (OldTimeCounter - g_TimeCounter) > 1000 / 60
}
*/

HRESULT CProxy_IDirectDraw7::Proxy_WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent)
{
//	kDD_LOGGING(("IDirectDraw7::WaitForVerticalBlank()  dwFlags = 0x%x  hEvent = 0x%x", dwFlags, hEvent));
	HRESULT result;

	//kDD_LOGGING2( ("VBlank executecounter:%d :%d",g_executecount,g_normalcounter++) );

	g_PerformanceCounter.ModifiCounter();
	if((CooperativeLevel & DDSCL_FULLSCREEN) == 0){
		if( g_pSharedData && g_pSharedData->fix_windowmode_vsyncwait ){
			Sleep( (DWORD)((g_VSyncWaitTick * 950) / 1000) );
			g_PerformanceCounter.InitInstaltPerformance();
			result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
			g_VSyncWaitTick = g_PerformanceCounter.CalcInstaltPerformance();
			g_executecount++;
			//result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
			result = DD_OK;
		}else{
			result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
		}
	}else{
		result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
	}
#if 0
//	if(g_pRCXDraw)g_pRCXDraw->DrawSyncFunc();
	result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
	if((CooperativeLevel & DDSCL_FULLSCREEN) == 0){
//		if(g_pRCXDraw)g_pRCXDraw->Draw(TargetSurface);
	}
#endif

	return result;
}


HRESULT CProxy_IDirect3D7::Proxy_CreateDevice( REFCLSID rclsid,LPDIRECTDRAWSURFACE7 lpDDS,LPDIRECT3DDEVICE7 *  lplpD3DDevice)
{
	kDD_LOGGING(("CProxy_IDirect3D7::CreateDevice2()"));
	HRESULT temp_ret = m_Instance->CreateDevice(rclsid,lpDDS,lplpD3DDevice);

	if(temp_ret == D3D_OK ){
		void *ret_cProxy;
		ret_cProxy = new CProxy_IDirect3DDevice7( (IDirect3DDevice7*) *lplpD3DDevice,lpDDS);
		*lplpD3DDevice = (LPDIRECT3DDEVICE7)ret_cProxy;
		return D3D_OK ;
	}
	return temp_ret;
}

void CProxy_IDirect3DDevice7::Proxy_Release(void)
{
	if( g_pSFastFont )
		delete g_pSFastFont;
	if( g_pddsFontTexture ){
		g_pddsFontTexture->Release();
		g_pddsFontTexture = NULL;
	}
}


HRESULT CProxy_IDirect3DDevice7::Proxy_SetViewport(THIS_ LPD3DVIEWPORT7 lpViewport)
{
	return m_Instance->SetViewport(lpViewport);
}

HRESULT CProxy_IDirect3DDevice7::Proxy_Clear(DWORD     dwCount,LPD3DRECT lpRects,DWORD     dwFlags,DWORD     dwColor,D3DVALUE  dvZ,DWORD     dwStencil)
{
	return m_Instance->Clear(dwCount,lpRects,dwFlags,dwColor,dvZ,dwStencil);
}

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


LPDIRECTDRAWSURFACE7 g_BackupSurface = NULL;
HRESULT CProxy_IDirect3DDevice7::Proxy_SetTexture(THIS_ DWORD p1,LPDIRECTDRAWSURFACE7 p2)
{
	g_BackupSurface = p2;
	return m_Instance->SetTexture(p1,p2);
}

HRESULT CProxy_IDirect3DDevice7::Proxy_BeginScene(void)
{
	HRESULT result;
	if( m_firstonce ){
		m_firstonce = false;
		LoadIni();
		SearchRagexeMemory();

		D3DDEVICEDESC7 ddDesc;
		m_Instance->GetCaps( &ddDesc );

		DDSURFACEDESC2 ddsd;
		ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize          = sizeof(DDSURFACEDESC2);
		ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|
								DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
		ddsd.dwWidth         = 512;
		ddsd.dwHeight        = 512;

		if( ddDesc.deviceGUID == IID_IDirect3DHALDevice )
			ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
		else if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice )
			ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
		else
			ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

		m_Instance->EnumTextureFormats( TextureSearchCallback, &ddsd.ddpfPixelFormat );
		if( ddsd.ddpfPixelFormat.dwRGBBitCount ){
			LPDIRECTDRAWSURFACE7 pddsRender = NULL;
			LPDIRECTDRAW7        pDD = NULL;
			m_Instance->GetRenderTarget( &pddsRender );
			if( pddsRender ){
				pddsRender->GetDDInterface( (VOID**)&pDD );
				pddsRender->Release();
			}
			if( pDD ){
				//{
				//	std::stringstream str;
				//	str << "pDD = " << pDD;
				//	::MessageBox(NULL,str.str().c_str(),"ROS",MB_OK);
				//}
				if( SUCCEEDED( pDD->CreateSurface( &ddsd, &g_pddsFontTexture, NULL )) ){
				//	::MessageBox(NULL,"font texture created.","ROS",MB_OK);
				} else {
				//	::MessageBox(NULL,"failed create font texture.","ROS",MB_OK);
				}
				pDD->Release();
			}
			if( g_pddsFontTexture ){
				LOGFONT logfont;
				logfont.lfHeight         = -10;       // フォントの高さ
				logfont.lfWidth          = 0;         // 平均文字幅
				logfont.lfEscapement     = 0;         // 文字送り方向の角度
				logfont.lfOrientation    = 0;         // ベースラインの角度 
				logfont.lfWeight         = FW_REGULAR;// フォントの太さ 
				//
				logfont.lfItalic         = FALSE; // 斜体にするかどうか
				logfont.lfUnderline      = FALSE; // 下線を付けるかどうか
				logfont.lfStrikeOut      = FALSE; // 取り消し線を付けるかどうか
				logfont.lfCharSet        = DEFAULT_CHARSET;          // 文字セットの識別子
				logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;       // 出力精度 
				logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;      // クリッピング精度 
				logfont.lfQuality        = NONANTIALIASED_QUALITY;   // 出力品質 
				logfont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;// ピッチとファミリ 
				_tcscpy_s(logfont.lfFaceName,_T("ＭＳ Ｐゴシック"));// フォント名

				g_pSFastFont = new CSFastFont;
				g_pSFastFont->CreateFastFont(&logfont,m_Instance,g_pddsFontTexture,0);
			}
		}
	}
	result = m_Instance->BeginScene();

	return result;
}

HRESULT CProxy_IDirect3DDevice7::Proxy_EndScene(void)
{
	g_PerformanceCounter.ModifiFrameRate();
	DrawSRHDebug( m_Instance );
	DrawM2E( m_Instance );

	return m_Instance->EndScene();
}

HRESULT CProxy_IDirect3DDevice7::Proxy_SetRenderState(THIS_ D3DRENDERSTATETYPE dwRenderStateType,DWORD dwRenderState)
{
	return m_Instance->SetRenderState(dwRenderStateType,dwRenderState);
}
HRESULT CProxy_IDirect3DDevice7::Proxy_DrawPrimitive(D3DPRIMITIVETYPE dptPrimitiveType,DWORD  dwVertexTypeDesc,LPVOID lpvVertices,DWORD  dwVertexCount,DWORD  dwFlags)
{
	return m_Instance->DrawPrimitive(dptPrimitiveType,dwVertexTypeDesc,lpvVertices,dwVertexCount,dwFlags);
}
