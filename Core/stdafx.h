// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>
#include <WinSock2.h>
#include <Shlwapi.h>
#include <mmsystem.h>
#include <Richedit.h>
#include <tchar.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dinput.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


#define DIRECTINPUT_VERSION  0x0700
#include <ddraw.h>
#include <d3d.h>
#include <dinput.h>
/*
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 一部の CString コンストラクターは明示的です。
#include <atlbase.h>
#include <atlstr.h>
#include <atlsimpcoll.h>
#include <atlcoll.h>
*/

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#include <process.h>

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>

#include <fstream>

#include <cwchar>
#include <regex>
