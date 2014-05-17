#include "ProxyHelper.h"


#define CLASSNAME "IDirectDraw7"
class CProxy_IDirectDraw7 : public IDirectDraw7{
private:
	IDirectDraw7*			m_Instance;
	DWORD					CooperativeLevel;
	DWORD					PrimarySurfaceFlag;
	IDirectDrawSurface7*	TargetSurface;
public:
	static CProxy_IDirectDraw7*    lpthis;

	CProxy_IDirectDraw7(IDirectDraw7* ptr);
	~CProxy_IDirectDraw7();
	
	static CProxy_IDirectDraw7* getLPProxyIDirectDraw7(void){return lpthis;};
	void   setThis(CProxy_IDirectDraw7* _lpthis){lpthis = _lpthis;};

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj)
	{
		return Proxy_QueryInterface(riid, ppvObj);
	}
	STDMETHOD_(ULONG,AddRef) (THIS)		PROXY0(AddRef)
	STDMETHOD_(ULONG,Release) (THIS)
	{
		return Proxy_Release();
	}

	/*** IDirectDraw methods ***/
	STDMETHOD(Compact)(THIS)			PROXY0(Compact)
	STDMETHOD(CreateClipper)(THIS_ DWORD p1, LPDIRECTDRAWCLIPPER FAR* p2, IUnknown FAR * p3)					PROXY3(CreateClipper)
	STDMETHOD(CreatePalette)(THIS_ DWORD p1, LPPALETTEENTRY p2, LPDIRECTDRAWPALETTE FAR* p3, IUnknown FAR * p4)	PROXY4(CreatePalette)
	STDMETHOD(CreateSurface)(THIS_  LPDDSURFACEDESC2 p1, LPDIRECTDRAWSURFACE7 FAR * p2, IUnknown FAR * p3)
	{
		return Proxy_CreateSurface(p1, p2, p3);
	}
	STDMETHOD(DuplicateSurface)( THIS_ LPDIRECTDRAWSURFACE7 p1, LPDIRECTDRAWSURFACE7 FAR * p2)					PROXY2(DuplicateSurface)
	STDMETHOD(EnumDisplayModes)( THIS_ DWORD p1, LPDDSURFACEDESC2 p2, LPVOID p3, LPDDENUMMODESCALLBACK2 p4)		PROXY4(EnumDisplayModes)
	STDMETHOD(EnumSurfaces)(THIS_ DWORD p1, LPDDSURFACEDESC2 p2, LPVOID p3, LPDDENUMSURFACESCALLBACK7 p4) 		PROXY4(EnumSurfaces)
	STDMETHOD(FlipToGDISurface)(THIS)								PROXY0(FlipToGDISurface)
	STDMETHOD(GetCaps)( THIS_ LPDDCAPS p1, LPDDCAPS p2)				PROXY2(GetCaps)
	STDMETHOD(GetDisplayMode)( THIS_ LPDDSURFACEDESC2 p1)
	{
		return Proxy_GetDisplayMode(p1);
	}
	STDMETHOD(GetFourCCCodes)(THIS_  LPDWORD p1, LPDWORD p2)		PROXY2(GetFourCCCodes)
	STDMETHOD(GetGDISurface)(THIS_ LPDIRECTDRAWSURFACE7 FAR * p1)	PROXY1(GetGDISurface)
	STDMETHOD(GetMonitorFrequency)(THIS_ LPDWORD p1)				PROXY1(GetMonitorFrequency)
	STDMETHOD(GetScanLine)(THIS_ LPDWORD p1)						PROXY1(GetScanLine)
	STDMETHOD(GetVerticalBlankStatus)(THIS_ LPBOOL p1)				PROXY1(GetVerticalBlankStatus)
	STDMETHOD(Initialize)(THIS_ GUID FAR * p1)						PROXY1(Initialize)
	STDMETHOD(RestoreDisplayMode)(THIS)								PROXY0(RestoreDisplayMode)
	STDMETHOD(SetCooperativeLevel)(THIS_ HWND p1, DWORD p2)
	{
		return Proxy_SetCooperativeLevel(p1, p2);
	}
	STDMETHOD(SetDisplayMode)(THIS_ DWORD p1, DWORD p2, DWORD p3, DWORD p4, DWORD p5)	{ return Proxy_SetDisplayMode(p1, p2, p3, p4, p5); }
	STDMETHOD(WaitForVerticalBlank)(THIS_ DWORD p1, HANDLE p2)
	{
		return Proxy_WaitForVerticalBlank(p1, p2);
	}

	/*** Added in the v2 interface ***/
	STDMETHOD(GetAvailableVidMem)(THIS_ LPDDSCAPS2 p1, LPDWORD p2, LPDWORD p3)	PROXY3(GetAvailableVidMem)
	/*** Added in the V4 Interface ***/
	STDMETHOD(GetSurfaceFromDC) (THIS_ HDC p1, LPDIRECTDRAWSURFACE7 * p2)		PROXY2(GetSurfaceFromDC)
	STDMETHOD(RestoreAllSurfaces)(THIS)
	{
		return Proxy_RestoreAllSurfaces();
	}
	STDMETHOD(TestCooperativeLevel)(THIS)										PROXY0(TestCooperativeLevel)
	STDMETHOD(GetDeviceIdentifier)(THIS_ LPDDDEVICEIDENTIFIER2 p1, DWORD p2) 	PROXY2(GetDeviceIdentifier)
	/*** Added in the V7 Interface ***/
	STDMETHOD(StartModeTest)(THIS_ LPSIZE p1, DWORD p2, DWORD p3)				PROXY3(StartModeTest)
	STDMETHOD(EvaluateMode)(THIS_ DWORD p1, DWORD * p2)							PROXY2(EvaluateMode)


	//
	// Proxy Functions
	//
	ULONG   Proxy_Release(void);
	HRESULT Proxy_CreateSurface(LPDDSURFACEDESC2 p1, LPDIRECTDRAWSURFACE7 FAR * p2, IUnknown FAR * p3);
	HRESULT Proxy_GetDisplayMode(LPDDSURFACEDESC2 p1);
	HRESULT Proxy_SetCooperativeLevel(HWND p1, DWORD p2);
	HRESULT Proxy_SetDisplayMode(DWORD p1, DWORD p2, DWORD p3, DWORD p4, DWORD p5);
	HRESULT Proxy_WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent);
	HRESULT Proxy_QueryInterface(THIS_ REFIID riid, LPVOID FAR * ppvObj);
	HRESULT Proxy_RestoreAllSurfaces(void);
};
#undef CLASSNAME


#define CLASSNAME "IDirectDrawSurface7"
class CProxy_IDirectDrawSurface7 : public IDirectDrawSurface7{
private:
	IDirectDrawSurface7* m_Instance;
public:
	CProxy_IDirectDrawSurface7(IDirectDrawSurface7* ptr) : m_Instance(ptr) {}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (THIS_ REFIID p1, LPVOID FAR * p2)	PROXY2(QueryInterface)

	STDMETHOD_(ULONG,AddRef) (THIS)		PROXY0(AddRef)
	STDMETHOD_(ULONG,Release) (THIS)	PROXY_RELEASE

	/*** IDirectDrawSurface methods ***/
	STDMETHOD(AddAttachedSurface)(THIS_ LPDIRECTDRAWSURFACE7 p1)	PROXY1(AddAttachedSurface)
	STDMETHOD(AddOverlayDirtyRect)(THIS_ LPRECT p1)					PROXY1(AddOverlayDirtyRect)

	STDMETHOD(Blt)(THIS_ LPRECT p1, LPDIRECTDRAWSURFACE7 p2, LPRECT p3, DWORD p4, LPDDBLTFX p5)	PROXY5(Blt)
	STDMETHOD(BltBatch)(THIS_ LPDDBLTBATCH p1, DWORD p2, DWORD p3)								PROXY3(BltBatch)
	STDMETHOD(BltFast)(THIS_ DWORD p1, DWORD p2, LPDIRECTDRAWSURFACE7 p3, LPRECT p4, DWORD p5)	PROXY5(BltFast)

	STDMETHOD(DeleteAttachedSurface)(THIS_ DWORD p1, LPDIRECTDRAWSURFACE7 p2)					PROXY2(DeleteAttachedSurface)
	STDMETHOD(EnumAttachedSurfaces)(THIS_ LPVOID p1, LPDDENUMSURFACESCALLBACK7 p2)				PROXY2(EnumAttachedSurfaces)
	STDMETHOD(EnumOverlayZOrders)(THIS_ DWORD p1, LPVOID p2, LPDDENUMSURFACESCALLBACK7 p3)		PROXY3(EnumOverlayZOrders)
	STDMETHOD(Flip)(THIS_ LPDIRECTDRAWSURFACE7 p1, DWORD p2)									PROXY2(Flip)
	STDMETHOD(GetAttachedSurface)(THIS_ LPDDSCAPS2 p1, LPDIRECTDRAWSURFACE7 FAR * p2)			PROXY2(GetAttachedSurface)
	STDMETHOD(GetBltStatus)(THIS_ DWORD p1)										PROXY1(GetBltStatus)
	STDMETHOD(GetCaps)(THIS_ LPDDSCAPS2 p1)										PROXY1(GetCaps)
	STDMETHOD(GetClipper)(THIS_ LPDIRECTDRAWCLIPPER FAR* p1)					PROXY1(GetClipper)
	STDMETHOD(GetColorKey)(THIS_ DWORD p1, LPDDCOLORKEY p2)						PROXY2(GetColorKey)
	STDMETHOD(GetDC)(THIS_ HDC FAR * p1)										PROXY1(GetDC)

	STDMETHOD(GetFlipStatus)(THIS_ DWORD p1)									PROXY1(GetFlipStatus)

	STDMETHOD(GetOverlayPosition)(THIS_ LPLONG p1, LPLONG p2)					PROXY2(GetOverlayPosition)
	STDMETHOD(GetPalette)(THIS_ LPDIRECTDRAWPALETTE FAR* p1)					PROXY1(GetPalette)
	STDMETHOD(GetPixelFormat)(THIS_ LPDDPIXELFORMAT p1)							PROXY1(GetPixelFormat)
	STDMETHOD(GetSurfaceDesc)(THIS_ LPDDSURFACEDESC2 p1)						PROXY1(GetSurfaceDesc)
	STDMETHOD(Initialize)(THIS_ LPDIRECTDRAW p1, LPDDSURFACEDESC2 p2)			PROXY2(Initialize)
	STDMETHOD(IsLost)(THIS)														PROXY0(IsLost)
	STDMETHOD(Lock)(THIS_ LPRECT p1, LPDDSURFACEDESC2 p2, DWORD p3, HANDLE p4)	PROXY4(Lock)
	STDMETHOD(ReleaseDC)(THIS_ HDC p1)											PROXY1(ReleaseDC)
	STDMETHOD(Restore)(THIS)													PROXY0(Restore)
	STDMETHOD(SetClipper)(THIS_ LPDIRECTDRAWCLIPPER p1)							PROXY1(SetClipper)
	STDMETHOD(SetColorKey)(THIS_ DWORD p1, LPDDCOLORKEY p2)						PROXY2(SetColorKey)
	STDMETHOD(SetOverlayPosition)(THIS_ LONG p1, LONG p2)						PROXY2(SetOverlayPosition)
	STDMETHOD(SetPalette)(THIS_ LPDIRECTDRAWPALETTE p1)							PROXY1(SetPalette)
	STDMETHOD(Unlock)(THIS_ LPRECT p1)											PROXY1(Unlock)
	STDMETHOD(UpdateOverlay)(THIS_ LPRECT p1, LPDIRECTDRAWSURFACE7 p2, LPRECT p3, DWORD p4, LPDDOVERLAYFX p5)	PROXY5(UpdateOverlay)
	STDMETHOD(UpdateOverlayDisplay)(THIS_ DWORD p1)								PROXY1(UpdateOverlayDisplay)
	STDMETHOD(UpdateOverlayZOrder)(THIS_ DWORD p1, LPDIRECTDRAWSURFACE7 p2)		PROXY2(UpdateOverlayZOrder)
	/*** Added in the v2 interface ***/
	STDMETHOD(GetDDInterface)(THIS_ LPVOID FAR * p1)	PROXY1(GetDDInterface)
	STDMETHOD(PageLock)(THIS_ DWORD p1)					PROXY1(PageLock)
	STDMETHOD(PageUnlock)(THIS_ DWORD p1)				PROXY1(PageUnlock)
	/*** Added in the v3 interface ***/
	STDMETHOD(SetSurfaceDesc)(THIS_ LPDDSURFACEDESC2 p1, DWORD p2)	PROXY2(SetSurfaceDesc)
	/*** Added in the v4 interface ***/
	STDMETHOD(SetPrivateData)(THIS_ REFGUID p1, LPVOID p2, DWORD p3, DWORD p4)	PROXY4(SetPrivateData)
	STDMETHOD(GetPrivateData)(THIS_ REFGUID p1, LPVOID p2, LPDWORD p3)			PROXY3(GetPrivateData)
	STDMETHOD(FreePrivateData)(THIS_ REFGUID p1)		PROXY1(FreePrivateData)
	STDMETHOD(GetUniquenessValue)(THIS_ LPDWORD p1)		PROXY1(GetUniquenessValue)
	STDMETHOD(ChangeUniquenessValue)(THIS)				PROXY0(ChangeUniquenessValue)
	/*** Moved Texture7 methods here ***/
	STDMETHOD(SetPriority)(THIS_ DWORD p1)				PROXY1(SetPriority)
	STDMETHOD(GetPriority)(THIS_ LPDWORD p1)			PROXY1(GetPriority)
	STDMETHOD(SetLOD)(THIS_ DWORD p1)					PROXY1(SetLOD)
	STDMETHOD(GetLOD)(THIS_ LPDWORD p1)					PROXY1(GetLOD)
};
#undef CLASSNAME



// D3DDev HOOK
#define CLASSNAME "IDirect3DDevice7"
class CProxy_IDirect3DDevice7 : public IDirect3DDevice7{
private:
	IDirect3DDevice7*			m_Instance;
	IDirectDrawSurface7*	TargetSurface;
	enum enDRAWSTATUS{
		enDRAW3D = 0,
		enDRAWSPR,
		enDRAWGUI
	};
	enDRAWSTATUS			m_DrawState;
	BOOL					m_firstonce;
	BOOL					m_frameonce;
public:

	CProxy_IDirect3DDevice7(IDirect3DDevice7* ptr,IDirectDrawSurface7* psf) : m_Instance(ptr), TargetSurface(psf), m_firstonce(true) {}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID p1, LPVOID * p2) PROXY2(QueryInterface)
	STDMETHOD_(ULONG,AddRef) (THIS)		PROXY0(AddRef)
	STDMETHOD_(ULONG,Release) (THIS)//	PROXY_RELEASE
	{
		Proxy_Release();
		ULONG Count = m_Instance->Release();
		kDD_LOGGING_TEST((CLASSNAME "::Release()  RefCount = %d", Count));
		if(Count == 0)
			delete this;
		return Count;
	}
	void Proxy_Release(void);

	/*** IDirect3DDevice7 methods ***/
	STDMETHOD(GetCaps)(THIS_ LPD3DDEVICEDESC7 p1) PROXY1(GetCaps)
	STDMETHOD(EnumTextureFormats)(THIS_ LPD3DENUMPIXELFORMATSCALLBACK p1,LPVOID p2) PROXY2(EnumTextureFormats)

	//    STDMETHOD(BeginScene)(THIS) PROXY0(BeginScene)
	STDMETHOD(BeginScene)(THIS)	{ return Proxy_BeginScene(); };

	//    STDMETHOD(EndScene)(THIS) PROXY0(EndScene)
	STDMETHOD(EndScene)(THIS) {	return Proxy_EndScene(); };

	STDMETHOD(GetDirect3D)(THIS_ LPDIRECT3D7* p1) PROXY1(GetDirect3D)
	STDMETHOD(SetRenderTarget)(THIS_ LPDIRECTDRAWSURFACE7 p1,DWORD p2) PROXY2(SetRenderTarget)
	STDMETHOD(GetRenderTarget)(THIS_ LPDIRECTDRAWSURFACE7 *p1) PROXY1(GetRenderTarget)
    STDMETHOD(Clear)(THIS_ DWORD p1,LPD3DRECT p2,DWORD p3,D3DCOLOR p4,D3DVALUE p5,DWORD p6) PROXY6(Clear)
    STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE p1,LPD3DMATRIX p2) PROXY2(SetTransform)
	STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE p1,LPD3DMATRIX p2) PROXY2(GetTransform)
	STDMETHOD(SetViewport)(THIS_ LPD3DVIEWPORT7 p1) PROXY1(SetViewport)

	STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE p1,LPD3DMATRIX p2) PROXY2(MultiplyTransform)
	STDMETHOD(GetViewport)(THIS_ LPD3DVIEWPORT7 p1) PROXY1(GetViewport)
	STDMETHOD(SetMaterial)(THIS_ LPD3DMATERIAL7 p1) PROXY1(SetMaterial)
	STDMETHOD(GetMaterial)(THIS_ LPD3DMATERIAL7 p1) PROXY1(GetMaterial)
	STDMETHOD(SetLight)(THIS_ DWORD p1,LPD3DLIGHT7 p2) PROXY2(SetLight)
	STDMETHOD(GetLight)(THIS_ DWORD p1,LPD3DLIGHT7 p2) PROXY2(GetLight)

	//STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE p1,DWORD p2) PROXY2(SetRenderState)
	STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE p1,DWORD p2) {return Proxy_SetRenderState(p1,p2);}

	STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE p1,LPDWORD p2) PROXY2(GetRenderState)
	STDMETHOD(BeginStateBlock)(THIS) PROXY0(BeginStateBlock)
	STDMETHOD(EndStateBlock)(THIS_ LPDWORD p1) PROXY1(EndStateBlock)
	STDMETHOD(PreLoad)(THIS_ LPDIRECTDRAWSURFACE7 p1) PROXY1(PreLoad)
	STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE p1,DWORD p2,LPVOID p3,DWORD p4,DWORD p5) PROXY5(DrawPrimitive)
	//
	STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE p1,DWORD p2,LPVOID p3,DWORD p4,LPWORD p5,DWORD p6,DWORD p7) PROXY7(DrawIndexedPrimitive)
	STDMETHOD(SetClipStatus)(THIS_ LPD3DCLIPSTATUS p1) PROXY1(SetClipStatus)
	STDMETHOD(GetClipStatus)(THIS_ LPD3DCLIPSTATUS p1) PROXY1(GetClipStatus)
	STDMETHOD(DrawPrimitiveStrided)(THIS_ D3DPRIMITIVETYPE p1,DWORD p2,LPD3DDRAWPRIMITIVESTRIDEDDATA p3,DWORD p4,DWORD p5) PROXY5(DrawPrimitiveStrided)
	STDMETHOD(DrawIndexedPrimitiveStrided)(THIS_ D3DPRIMITIVETYPE p1,DWORD p2,LPD3DDRAWPRIMITIVESTRIDEDDATA p3,DWORD p4,LPWORD p5,DWORD p6,DWORD p7) PROXY7(DrawIndexedPrimitiveStrided)
	STDMETHOD(DrawPrimitiveVB)(THIS_ D3DPRIMITIVETYPE p1,LPDIRECT3DVERTEXBUFFER7 p2,DWORD p3,DWORD p4,DWORD p5) PROXY5(DrawPrimitiveVB)
	STDMETHOD(DrawIndexedPrimitiveVB)(THIS_ D3DPRIMITIVETYPE p1,LPDIRECT3DVERTEXBUFFER7 p2,DWORD p3,DWORD p4,LPWORD p5,DWORD p6,DWORD p7) PROXY7(DrawIndexedPrimitiveVB)
	STDMETHOD(ComputeSphereVisibility)(THIS_ LPD3DVECTOR p1,LPD3DVALUE p2,DWORD p3,DWORD p4,LPDWORD p5) PROXY5(ComputeSphereVisibility)
	STDMETHOD(GetTexture)(THIS_ DWORD p1,LPDIRECTDRAWSURFACE7 *p2) PROXY2(GetTexture)
	STDMETHOD(SetTexture)(THIS_ DWORD p1,LPDIRECTDRAWSURFACE7 p2) PROXY2(SetTexture)
	STDMETHOD(GetTextureStageState)(THIS_ DWORD p1,D3DTEXTURESTAGESTATETYPE p2,LPDWORD p3) PROXY3(GetTextureStageState)
	STDMETHOD(SetTextureStageState)(THIS_ DWORD p1,D3DTEXTURESTAGESTATETYPE p2,DWORD p3) PROXY3(SetTextureStageState)
	STDMETHOD(ValidateDevice)(THIS_ LPDWORD p1) PROXY1(ValidateDevice)
	STDMETHOD(ApplyStateBlock)(THIS_ DWORD p1) PROXY1(ApplyStateBlock)
	STDMETHOD(CaptureStateBlock)(THIS_ DWORD p1) PROXY1(CaptureStateBlock)
	STDMETHOD(DeleteStateBlock)(THIS_ DWORD p1) PROXY1(DeleteStateBlock)
	STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE p1,LPDWORD p2) PROXY2(CreateStateBlock)
	STDMETHOD(Load)(THIS_ LPDIRECTDRAWSURFACE7 p1,LPPOINT p2,LPDIRECTDRAWSURFACE7 p3,LPRECT p4,DWORD p5) PROXY5(Load)
	STDMETHOD(LightEnable)(THIS_ DWORD p1,BOOL p2) PROXY2(LightEnable)
	STDMETHOD(GetLightEnable)(THIS_ DWORD p1,BOOL* p2) PROXY2(GetLightEnable)
	STDMETHOD(SetClipPlane)(THIS_ DWORD p1,D3DVALUE* p2) PROXY2(SetClipPlane)
	STDMETHOD(GetClipPlane)(THIS_ DWORD p1,D3DVALUE* p2) PROXY2(GetClipPlane)
	STDMETHOD(GetInfo)(THIS_ DWORD p1,LPVOID p2,DWORD p3) PROXY3(GetInfo)


	//
	// Proxy Functions
	//
	HRESULT Proxy_SetRenderState(THIS_ D3DRENDERSTATETYPE dwRenderStateType,DWORD dwRenderState);
	HRESULT Proxy_BeginScene(void);
	HRESULT Proxy_EndScene(void);
};
#undef CLASSNAME


// D3D HOOK
#define CLASSNAME "IDirect3D7"
class CProxy_IDirect3D7 : public IDirect3D7{
private:
	IDirect3D7*			m_Instance;
public:
	CProxy_IDirect3D7(IDirect3D7* ptr) : m_Instance(ptr) {}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID p1, LPVOID * p2) PROXY2(QueryInterface)
	STDMETHOD_(ULONG,AddRef) (THIS)		PROXY0(AddRef)
	STDMETHOD_(ULONG,Release) (THIS)	PROXY_RELEASE

	/*** IDirect3D7 methods ***/
	STDMETHOD(EnumDevices)(THIS_ LPD3DENUMDEVICESCALLBACK7 p1,LPVOID p2) PROXY2(EnumDevices)
	//STDMETHOD(CreateDevice)(THIS_ REFCLSID p1,LPDIRECTDRAWSURFACE7 p2,LPDIRECT3DDEVICE7* p3) PROXY3(CreateDevice)
	STDMETHOD(CreateDevice)(THIS_ REFCLSID p1,LPDIRECTDRAWSURFACE7 p2,LPDIRECT3DDEVICE7* p3)
	{
		return Proxy_CreateDevice(p1,p2,p3);
	};
	STDMETHOD(CreateVertexBuffer)(THIS_ LPD3DVERTEXBUFFERDESC p1,LPDIRECT3DVERTEXBUFFER7* p2,DWORD p3) PROXY3(CreateVertexBuffer)
	STDMETHOD(EnumZBufferFormats)(THIS_ REFCLSID p1,LPD3DENUMPIXELFORMATSCALLBACK p2,LPVOID p3) PROXY3(EnumZBufferFormats)
	STDMETHOD(EvictManagedTextures)(THIS) PROXY0(EvictManagedTextures)


	HRESULT Proxy_CreateDevice( REFCLSID rclsid,LPDIRECTDRAWSURFACE7 lpDDS,LPDIRECT3DDEVICE7 *  lplpD3DDevice);
};
#undef CLASSNAME
