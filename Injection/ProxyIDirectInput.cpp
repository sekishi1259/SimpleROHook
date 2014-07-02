#include "tinyconsole.h"
#include "ProxyIDirectInput.h"

#include "Core/RoCodeBind.h"

HRESULT CProxyIDirectInput7::Proxy_CreateDevice(THIS_ REFGUID rguid,LPDIRECTINPUTDEVICEA *lpIDD,LPUNKNOWN pUnkOuter)
{
	DEBUG_LOGGING_MORE_DETAIL(("IDirectInput7::CreateDevice()\n"));

	void *ret_cProxy;
	IDirectInputDevice7* lpDirectInputDevice7;

	HRESULT Result = m_Instance->CreateDevice(rguid, (LPDIRECTINPUTDEVICEA*)&lpDirectInputDevice7,pUnkOuter);

	if ( IsEqualGUID(rguid, GUID_SysMouse) ){
		DEBUG_LOGGING_MORE_DETAIL(("IDirectInput7::Hook_CProxyIDirectInputDevice7 0x%0x ", lpDirectInputDevice7));
		ret_cProxy = (void*)(new CProxyIDirectInputDevice7(lpDirectInputDevice7));
		*lpIDD = (LPDIRECTINPUTDEVICEA)ret_cProxy;
	}

	return Result;
}

HRESULT CProxyIDirectInputDevice7::Proxy_GetDeviceState(THIS_ DWORD cbData,LPVOID lpvData)
{
	static DWORD oldtime = 0;
	HRESULT Result;

	Result = m_Instance->GetDeviceState(cbData, lpvData);

	if( g_pRoCodeBind )
		g_pRoCodeBind->OneSyncProc(Result, lpvData, g_FreeMouseSw);

	return Result;
}

HRESULT CProxyIDirectInputDevice7::Proxy_SetCooperativeLevel(HWND hwnd, DWORD dwflags)
{
	HRESULT Result;
	DEBUG_LOGGING_MORE_DETAIL(("lpDI->SetCooperativeLevel\n"));

	if( g_FreeMouseSw )
		dwflags = DISCL_NONEXCLUSIVE | DISCL_BACKGROUND;

	Result = m_Instance->SetCooperativeLevel(hwnd,dwflags);

	return Result;
}
