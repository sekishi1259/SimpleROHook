#include "stdafx.h"

#include "RoCodeBind.h"

extern HANDLE         g_hMapObject;
extern StSHAREDMEMORY *g_pSharedData;


HINSTANCE g_hTCInstance;

#define MAX_LOADSTRING 100
#define IDC_RICHEDIT (101)

TCHAR strPath[MAX_PATH + 1];
HINSTANCE	hRtLib;
HWND		hRichEdit = NULL;
HANDLE g_hTinyConsole = NULL;
HANDLE g_hDebugConsole = NULL;

UINT __stdcall TinyConsole(LPVOID lpArg);



void DebugLogA(const char* format, ...) 
{
	va_list argptr;
	va_start(argptr, format);

	int length = _vscprintf(format, argptr);
	char* buf = new char [length + 2];
	vsprintf_s(buf,length + 2, format, argptr );
	strcat_s(buf,length + 2,"\n");

	if( hRichEdit ){
		int len = ::GetWindowTextLength(hRichEdit);

		::SendMessage( hRichEdit, EM_SETSEL, (WPARAM)len, (LPARAM)len );
		::SendMessage( hRichEdit, EM_REPLACESEL, 0, (LPARAM)buf );
	}

	delete[] buf;
	va_end(argptr);
}


void CreateTinyConsole(void)
{
	HINSTANCE hinst = ::GetModuleHandle(NULL);

	g_hTinyConsole = ::CreateEvent( NULL,TRUE,TRUE,_T("TinyConsole"));
	if( g_hTinyConsole )
	{
		::ResetEvent( g_hTinyConsole );
		g_hDebugConsole = (HANDLE)_beginthreadex(NULL,0,TinyConsole,(LPVOID)(&hinst),CREATE_SUSPENDED,NULL );
		::ResumeThread( g_hDebugConsole );
		::SetEvent( g_hTinyConsole );
		::WaitForSingleObject( g_hTinyConsole , INFINITE );
	}
}
void ReleaseTinyConsole(void)
{
	if( g_hDebugConsole )
		::CloseHandle( g_hDebugConsole );
	if( g_hTinyConsole )
		::CloseHandle( g_hTinyConsole );

}

LRESULT CALLBACK TinyConsoleWinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch (message)
    {
	case WM_CREATE:
		{
			::GetSystemDirectory(strPath , MAX_PATH + 1);
			wsprintf(strPath, TEXT("%s\\%s"), strPath, _TEXT("RICHED20.DLL"));
			hRtLib = ::LoadLibrary((LPCTSTR)strPath);
			hRichEdit = ::CreateWindowEx(WS_EX_CLIENTEDGE,
					_T("RichEdit20A"),
					_T(""),
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_HSCROLL |
					WS_VSCROLL | ES_AUTOVSCROLL | ES_NOHIDESEL | ES_READONLY,
					0,80,200,200,
					hWnd,
					(HMENU)IDC_RICHEDIT,
					g_hTCInstance,
					NULL);

			DWORD dwLangOptions;
			CHARFORMAT  cfm;    // CHARFORMAT structure

			dwLangOptions = ::SendMessage(hRichEdit, EM_GETLANGOPTIONS, 0, 0);
            dwLangOptions &= ~IMF_DUALFONT;
            ::SendMessage(hRichEdit, EM_SETLANGOPTIONS, 0, (LPARAM)dwLangOptions);

			memset(&cfm, 0, sizeof(CHARFORMAT));
            cfm.cbSize = sizeof(CHARFORMAT);
			cfm.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT |
				CFM_CHARSET | CFM_FACE | CFM_COLOR | CFM_SIZE;

			cfm.yHeight = 14 * 20;
            cfm.bCharSet = SHIFTJIS_CHARSET;  //ÅiShiftJISÅj
			lstrcpy(cfm.szFaceName, _T("FixedSys"));//FixedSys System Terminal

			cfm.crTextColor = RGB(0, 0, 0);
			cfm.dwEffects = 0;

            ::SendMessage(hRichEdit,
                    EM_SETCHARFORMAT,
                    SCF_SELECTION | SCF_WORD,
                    (LPARAM)&cfm);
			::SendMessage( hRichEdit, EM_REPLACESEL, FALSE, (LPARAM)(LPCTSTR)_T("hook enable.\n") );
			{
				char str[256];
				sprintf(str,"g_hMapObject=%08X g_pSharedData=%08X",g_hMapObject,g_pSharedData);
				::SendMessage( hRichEdit, EM_REPLACESEL, FALSE, (LPARAM)(LPCTSTR)str );
			}
			if( g_hTinyConsole ){
				::SetEvent( g_hTinyConsole );
			}
		}
		break;
	case WM_SIZE:
		::MoveWindow(hRichEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		break;
	default:
		return ::DefWindowProc( hWnd, message, wParam, lParam );
		break;
	}
	return 0;
}

#define	TINYCONSOLE_CLASS_NAME	_T("#32769")

UINT __stdcall TinyConsole(LPVOID lpArg)
{
	WNDCLASS	wc;
	HWND		hWnd;
	MSG		 	msg;
	g_hTCInstance = *((HINSTANCE*)lpArg);

	memset( &wc, 0, sizeof(wc) );
	wc.hCursor			= ::LoadCursor( NULL, IDC_ARROW );
	wc.hIcon			= ::LoadIcon( NULL, IDI_WINLOGO );
	wc.hInstance		= g_hTCInstance;
	wc.lpfnWndProc		= TinyConsoleWinProc;
	wc.lpszClassName	= TINYCONSOLE_CLASS_NAME;

	if( !::RegisterClass( &wc ) ) return 0;

	hWnd = ::CreateWindow( TINYCONSOLE_CLASS_NAME, _T("Tiny Console"),
				WS_OVERLAPPEDWINDOW|WS_VISIBLE,
				0, 0, 580, 480,
				NULL, NULL, g_hTCInstance, NULL
				);

	while( ::GetMessage(&msg, NULL, 0, 0 ) )
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	_endthreadex(0);
	return 0;
}
