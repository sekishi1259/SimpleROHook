#include "stdafx.h"
#include "tinyconsole.h"
#include "ProxyDirectDraw.h"

#include "Core/RoCodeBind.h"

int g_executecount = 0;
double g_VSyncWaitTick = 0.0;
DWORD g_MonitorRefreshRate;

CProxy_IDirectDraw7*    CProxy_IDirectDraw7::lpthis;

CProxy_IDirectDraw7::CProxy_IDirectDraw7(IDirectDraw7* ptr):m_Instance(ptr), CooperativeLevel(0), PrimarySurfaceFlag(0), TargetSurface(NULL)
{
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
	DDSURFACEDESC2 OrgSurfaceDesc2 = *SurfaceDesc;

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

	DWORD RefreshRate = Desc->dwRefreshRate;
	kDD_LOGGING(("    RefreshRate = %d", Desc->dwRefreshRate));
	if( RefreshRate == 0){
		kDD_LOGGING(("    RefreshRate = %d >> dummyset 100", Desc->dwRefreshRate));
		RefreshRate = 100;
	}

	g_MonitorRefreshRate = RefreshRate;
	g_PerformanceCounter.SetMonitorRefreshRate( (int)RefreshRate );

	return Result;
}

HRESULT CProxy_IDirectDraw7::Proxy_SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
{
	kDD_LOGGING(("IDirectDraw7::SetCooperativeLevel()  dwFlags = 0x%x", dwFlags));
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

	g_MonitorRefreshRate = RefreshRate;
	g_PerformanceCounter.SetMonitorRefreshRate( (int)RefreshRate );

	return Result;
}
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
	ReleaseRODraw();
}

HRESULT CProxy_IDirect3DDevice7::Proxy_SetRenderState(THIS_ D3DRENDERSTATETYPE dwRenderStateType,DWORD dwRenderState)
{
	//kDD_LOGGING2(("CProxy_IDirect3D7::Proxy_SetRenderState() type:%08X val:%08X",dwRenderStateType,dwRenderState));
	if( dwRenderStateType == D3DRENDERSTATE_ZENABLE && dwRenderState == 0 ){
		//
		// UI is drawn after the Zbuffer is disabled.
		//
		DrawOn3DMap( m_Instance );
	}
	return m_Instance->SetRenderState(dwRenderStateType,dwRenderState);
}

HRESULT CProxy_IDirect3DDevice7::Proxy_BeginScene(void)
{
	HRESULT result;
	if( m_firstonce ){
		m_firstonce = false;
		InitRODraw(m_Instance);
	}
	m_DrawState = enDRAW3D;
	m_frameonce = TRUE;
	//kDD_LOGGING2(("CProxy_IDirect3D7::Proxy_BeginScene()"));

	result = m_Instance->BeginScene();

	return result;
}

HRESULT CProxy_IDirect3DDevice7::Proxy_EndScene(void)
{
	g_PerformanceCounter.ModifiFrameRate();
	DrawSRHDebug( m_Instance );

	return m_Instance->EndScene();
}

