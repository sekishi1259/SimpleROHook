#include "ProxyHelper.h"

#define CLASSNAME "IDirectInput7"
class CProxyIDirectInput7 : public IDirectInput7
{
private:
	IDirectInput7*			m_Instance;
public:
	CProxyIDirectInput7(IDirectInput7* ptr) : m_Instance(ptr) {}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID p1, LPVOID * p2) PROXY2(QueryInterface)
	STDMETHOD_(ULONG,AddRef) (THIS)		PROXY0(AddRef)
	STDMETHOD_(ULONG,Release) (THIS)	PROXY_RELEASE

	/*** IDirectInput2A methods ***/
	//
	STDMETHOD(CreateDevice)(THIS_ REFGUID rguid,LPDIRECTINPUTDEVICEA *lpIDD,LPUNKNOWN pUnkOuter)
	{
		return Proxy_CreateDevice(rguid, lpIDD,pUnkOuter);
	}
	STDMETHOD(EnumDevices)(THIS_ DWORD p1,LPDIENUMDEVICESCALLBACKA p2,LPVOID p3,DWORD p4) PROXY4(EnumDevices)
	STDMETHOD(GetDeviceStatus)(THIS_ REFGUID p1)                                 PROXY1(GetDeviceStatus)
	STDMETHOD(RunControlPanel)(THIS_ HWND p1,DWORD p2)                           PROXY2(RunControlPanel)
	STDMETHOD(Initialize)(THIS_ HINSTANCE p1,DWORD p2)                           PROXY2(Initialize)
	STDMETHOD(FindDevice)(THIS_ REFGUID p1,LPCSTR p2,LPGUID p3)                  PROXY3(FindDevice)

	/*** IDirectInput7A methods ***/
	STDMETHOD(CreateDeviceEx)(THIS_ REFGUID p1,REFIID p2,LPVOID *p3,LPUNKNOWN p4)        PROXY4(CreateDeviceEx)

	//
	// Proxy Functions
	//
	HRESULT Proxy_CreateDevice(THIS_ REFGUID rguid,LPDIRECTINPUTDEVICEA *lpIDD,LPUNKNOWN pUnkOuter);
};
#undef CLASSNAME

#define CLASSNAME "IDirectInputDevice7"
class CProxyIDirectInputDevice7 : public IDirectInputDevice7{
private:
	IDirectInputDevice7*			m_Instance;
public:
	CProxyIDirectInputDevice7(IDirectInputDevice7* ptr) : m_Instance(ptr) {}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID p1, LPVOID * p2) PROXY2(QueryInterface)
	STDMETHOD_(ULONG,AddRef) (THIS)		PROXY0(AddRef)
	STDMETHOD_(ULONG,Release) (THIS)	PROXY_RELEASE

	/*** IDirectInputDevice2W methods ***/
	STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS p1) PROXY1(GetCapabilities)
	STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKA p1,LPVOID p2,DWORD p3) PROXY3(EnumObjects)
	STDMETHOD(GetProperty)(THIS_ REFGUID p1,LPDIPROPHEADER p2) PROXY2(GetProperty)
	STDMETHOD(SetProperty)(THIS_ REFGUID p1,LPCDIPROPHEADER p2) PROXY2(SetProperty)
	STDMETHOD(Acquire)(THIS) PROXY0(Acquire)
	STDMETHOD(Unacquire)(THIS) PROXY0(Unacquire)
	STDMETHOD(GetDeviceState)(THIS_ DWORD p1,LPVOID p2)
	{
		return Proxy_GetDeviceState(p1,p2);
	}

	STDMETHOD(GetDeviceData)(THIS_ DWORD p1,LPDIDEVICEOBJECTDATA p2,LPDWORD p3,DWORD p4)  PROXY4(GetDeviceData)
	STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT p1) PROXY1(SetDataFormat)
	STDMETHOD(SetCooperativeLevel)(THIS_ HWND p1,DWORD p2)
	{
		return Proxy_SetCooperativeLevel(p1,p2);
	}

	STDMETHOD(SetEventNotification)(THIS_ HANDLE p1) PROXY1(SetEventNotification)

	STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEA p1,DWORD p2,DWORD p3) PROXY3(GetObjectInfo)
	STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEA p1) PROXY1(GetDeviceInfo)
	STDMETHOD(RunControlPanel)(THIS_ HWND p1,DWORD p2) PROXY2(RunControlPanel)
	STDMETHOD(Initialize)(THIS_ HINSTANCE p1,DWORD p2,REFGUID p3) PROXY3(Initialize)
	STDMETHOD(CreateEffect)(THIS_ REFGUID p1,LPCDIEFFECT p2,LPDIRECTINPUTEFFECT *p3,LPUNKNOWN p4) PROXY4(CreateEffect)
	STDMETHOD(EnumEffects)(THIS_ LPDIENUMEFFECTSCALLBACKA p1,LPVOID p2,DWORD p3) PROXY3(EnumEffects)
	STDMETHOD(GetEffectInfo)(THIS_ LPDIEFFECTINFOA p1,REFGUID p2) PROXY2(GetEffectInfo)
	STDMETHOD(GetForceFeedbackState)(THIS_ LPDWORD p1) PROXY1(GetForceFeedbackState)
	STDMETHOD(SendForceFeedbackCommand)(THIS_ DWORD p1) PROXY1(SendForceFeedbackCommand)
	STDMETHOD(EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK p1,LPVOID p2,DWORD p3) PROXY3(EnumCreatedEffectObjects)
	STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE p1) PROXY1(Escape)
	STDMETHOD(Poll)(THIS) PROXY0(Poll)
	STDMETHOD(SendDeviceData)(THIS_ DWORD p1,LPCDIDEVICEOBJECTDATA p2,LPDWORD p3,DWORD p4) PROXY4(SendDeviceData)

	/*** IDirectInputDevice7W methods ***/
	STDMETHOD(EnumEffectsInFile)(THIS_ LPCSTR p1,LPDIENUMEFFECTSINFILECALLBACK p2,LPVOID p3,DWORD p4) PROXY4(EnumEffectsInFile)
	STDMETHOD(WriteEffectToFile)(THIS_ LPCSTR p1,DWORD p2,LPDIFILEEFFECT p3,DWORD p4) PROXY4(WriteEffectToFile)


	//
	// Proxy Functions
	//
	HRESULT Proxy_GetDeviceState(THIS_ DWORD cbData,LPVOID lpvData);
	HRESULT Proxy_SetCooperativeLevel(HWND hwnd, DWORD dwflags);
};
#undef CLASSNAME

