#pragma once

#define kSHAREDMEMORY_OBJECTNAME _T("SimpleROHook1006")

typedef struct _StSHAREDMEMORY{
//	DWORD	used_flag;
	HWND	g_hROWindow;

	DWORD	executeorder;

	BOOL	freemouse; 
	BOOL	m2e; 
	BOOL	fix_windowmode_vsyncwait;
	BOOL	show_framerate;
	BOOL	objectinformation;
	BOOL	_44khz_audiomode;
	int		cpucoolerlevel;

	WCHAR	configfilepath[MAX_PATH];
	WCHAR	musicfilename[MAX_PATH];

}StSHAREDMEMORY;
