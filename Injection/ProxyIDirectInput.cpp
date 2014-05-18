#include "stdafx.h"

#include "tinyconsole.h"
#include "ProxyIDirectInput.h"

#include "Core/RoCodeBind.h"

HRESULT CProxyIDirectInput7::Proxy_CreateDevice(THIS_ REFGUID rguid,LPDIRECTINPUTDEVICEA *lpIDD,LPUNKNOWN pUnkOuter)
{
	kDD_LOGGING(("IDirectInput7::CreateDevice()\n"));

	void *ret_cProxy;
	IDirectInputDevice7* lpDirectInputDevice7;

	HRESULT Result = m_Instance->CreateDevice(rguid, (LPDIRECTINPUTDEVICEA*)&lpDirectInputDevice7,pUnkOuter);

	if( rguid.Data1 == GUID_SysMouse.Data1 
	 && rguid.Data2 == GUID_SysMouse.Data2 
	 && rguid.Data3 == GUID_SysMouse.Data3 
	 && *(UINT64*)rguid.Data4 == *(UINT64*)GUID_SysMouse.Data4
	 ){
		kDD_LOGGING(("IDirectInput7::Hook_CProxyIDirectInputDevice7 0x%0x ",lpDirectInputDevice7));
		ret_cProxy = (void*)(new CProxyIDirectInputDevice7(lpDirectInputDevice7));
		*lpIDD = (LPDIRECTINPUTDEVICEA)ret_cProxy;
	}

	return Result;
}

extern DWORD g_ROmouse;
HWND g_HWND = NULL;
/*
nowait 1000ms = 1970draw
vsync  1000ms =   60draw
*/
extern double g_FrameRate;
extern int g_executecount;
extern double g_VSyncWaitTick;
DWORD oldtime = 0;
HRESULT CProxyIDirectInputDevice7::Proxy_GetDeviceState(THIS_ DWORD cbData,LPVOID lpvData)
{
	HRESULT Result;

	Result = m_Instance->GetDeviceState(cbData, lpvData);

	if( funcRagexe_PlayStream ){
		if( g_pSharedData && g_pSharedData->executeorder == 1){
			g_pSharedData->executeorder = 0;
			char filename[MAX_PATH];
			if( ::WideCharToMultiByte(CP_ACP,0,
				g_pSharedData->musicfilename,wcslen(g_pSharedData->musicfilename)+1,
				filename,sizeof(filename),
				NULL,NULL) ){
				funcRagexe_PlayStream(filename,0);
			}
		}
	}

	if( oldtime ){
		if( (timeGetTime() - oldtime) >= 1000 )
		{
			double _60frametick = g_PerformanceCounter.GetTotalTick();
			//kDD_LOGGING2( ("60frametick = %d:%lf VSyncWaitTick:%lf",g_executecount,_60frametick,g_VSyncWaitTick) );
			g_executecount = 0;
			oldtime = timeGetTime();
		}
	}else{
		oldtime = timeGetTime();
	}

	if( g_ROmouse && g_MouseFreeSw ){
		CMouse *lpCMouse = (CMouse*)g_ROmouse;
		int *pMouseData = (int*)lpvData;

		POINT point;
		::GetCursorPos(&point);
		::ScreenToClient(g_HWND, &point);

		lpCMouse->m_xPos = point.x;
		lpCMouse->m_yPos = point.y;
		if( ::GetActiveWindow() == g_HWND ){
			lpCMouse->m_xPos -= pMouseData[0];
			lpCMouse->m_yPos -= pMouseData[1];
		}
	}
	// cpu cooler
	if( g_pSharedData ){
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

	return Result;
}

HRESULT CProxyIDirectInputDevice7::Proxy_SetCooperativeLevel(HWND hwnd, DWORD dwflags)
{
	HRESULT Result;
	kDD_LOGGING(("lpDI->SetCooperativeLevel\n"));

	g_HWND = hwnd;// Window Handle of RO
	// Mouse Freedom

	if( g_MouseFreeSw )
		dwflags = DISCL_NONEXCLUSIVE | DISCL_BACKGROUND;

	Result = m_Instance->SetCooperativeLevel(hwnd,dwflags);
	return Result;
}
