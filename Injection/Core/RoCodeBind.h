/*
    This file is part of SimpleROHook.

    SimpleROHook is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SimpleROHook is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SimpleROHook.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "shared.h"

#include "PerformanceCounter.h"
#include "SearchCode.h"
#include "FastFont/SFastFont.h"

// Required to create a dll for jRO.
//#define JRO_CLIENT_STRUCTURE
//

#include "ro/system.h"
#include "ro/packet.h"
#include "ro/mouse.h"
#include "ro/unit.h"
#include "ro/res.h"
#include "ro/ui.h"
#include "ro/object.h"
#include "ro/map.h"
#include "ro/task.h"

#define D3DFVF_CPOLVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define D3DCOLOR_ARGB(a,r,g,b) \
	((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

struct CPOLVERTEX {
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};

typedef void (__cdecl *tPlayStream)(const char *streamFileName,int playflag);

typedef void* (__thiscall *tCFileMgr__GetData)(/*CFileMgr*/void *this_pt, const char *name, unsigned int *size);

typedef void* (__thiscall *tCFileMgr__GetPak)(/*CFileMgr*/void *this_pt, const char *name, unsigned int *size);

typedef void* (__cdecl *tCRagConnection__instanceR)(void);

typedef int (__thiscall *tCRagConnection__GetPacketSize)(/*CRagConnection*/void *this_pt, int packetType);


class CRoCodeBind
{
private:
	CRenderer** g_renderer;
	CModeMgr *g_pmodeMgr;
	CMouse* g_mouse;

	void* m_CFileMgr__gfileMgr;
	tCFileMgr__GetData m_functionRagexe_CFileMgr__GetData;
	tCFileMgr__GetPak m_functionRagexe_CFileMgr__GetPak;
	
	tPlayStream m_funcRagexe_PlayStream;


	std::map<int, std::string> m_ItemName;
	void InitItemNameMap();

	tCRagConnection__instanceR m_functionRagexe_CRagConnection__instanceR;
	tCRagConnection__GetPacketSize m_functionRagexe_CRagConnection__GetPacketSize;
	void **m_packetLenMap;
	void *m_packetLenMap_InsertTree;

	HWND m_hWnd;
	int  m_gid;

#define PACKETQUEUE_BUFFERSIZE 40960
	char m_packetqueuebuffer[PACKETQUEUE_BUFFERSIZE];
	unsigned int m_packetqueue_head;

#define ROPACKET_MAXLEN 0x2000
	int m_packetLenMap_table[ROPACKET_MAXLEN];
	int m_packetLenMap_table_index;

#define MAX_FLLORSKILLTYPE 0x100
	DWORD m_M2ESkillColor[MAX_FLLORSKILLTYPE];

	int m_CMode_subMode;
	int m_CMode_old_subMode;

	DWORD _state_zenable;
	DWORD _state_zwriteenable;
	DWORD _state_zbias;
	DWORD _state_fogenable;
	DWORD _state_specularenable;
	DWORD _state_alphafunc;
	DWORD _state_alpharef;
	DWORD _state_srcblend;
	DWORD _state_destblend;
	void BackupRenderState(IDirect3DDevice7* d3ddevice);
	void RestoreRenderState(IDirect3DDevice7* d3ddevice);

	void DrawBBE(IDirect3DDevice7* d3ddevice);
	void DrawM2E(IDirect3DDevice7* d3ddevice);

	void PacketProc(const char *packetdata);

	typedef void (CRoCodeBind::*tPacketHandler)(const char *packetdata);
	std::map<int, tPacketHandler> m_packethandler;

	void InitPacketHandler(void);
	void PacketHandler_Cz_Say_Dialog(const char *packetdata);
	void PacketHandler_Cz_Menu_List(const char *packetdata);

	void SendMessageToNPCLogger(const char *src, int size);

	struct p_std_map_packetlen
	{
		struct p_std_map_packetlen *left, *parent, *right;
		DWORD key;
		int value;
	};
	int GetTreeData(p_std_map_packetlen* node);

	void ProjectVertex(vector3d& src,matrix& vtm,float *x,float *y,float *oow);
	void ProjectVertex(vector3d& src,matrix& vtm,tlvertex3d *vert);
	void ProjectVertexEx(vector3d& src, vector3d& pointvector, matrix& vtm, float *x, float *y, float *oow);
	void ProjectVertexEx(vector3d& src, vector3d& pointvector, matrix& vtm, tlvertex3d *vert);

	void LoadIni(void);
	void SearchRagexeMemory(void);

	LPDIRECTDRAWSURFACE7 m_pddsFontTexture;
	CSFastFont *m_pSFastFont;

	struct MouseDataStructure
	{
		int x_axis,y_axis,wheel;
		char l_button,r_button,wheel_button,pad;
	};

	void DrawGage(LPDIRECT3DDEVICE7 device, int x, int y, int w, int h, unsigned long value, DWORD color, int alpha, int type);
	void DrawHPSPGage(IDirect3DDevice7 *d3ddev, int x, int y, int hp, int sp);

public:
	CRoCodeBind() :
		m_hWnd(NULL),m_funcRagexe_PlayStream(NULL),
		m_CFileMgr__gfileMgr(NULL),
		m_functionRagexe_CFileMgr__GetPak(NULL),
		m_packetLenMap_table_index(0),m_packetqueue_head(0),
		m_pSFastFont(NULL),m_pddsFontTexture(NULL),
		g_renderer(NULL), g_pmodeMgr(NULL), g_mouse(NULL),
		m_packetLenMap(NULL), m_packetLenMap_InsertTree(NULL),
		m_functionRagexe_CRagConnection__GetPacketSize(NULL),
		m_functionRagexe_CRagConnection__instanceR(NULL),
		m_CMode_old_subMode(-1), m_CMode_subMode(-1),
		m_gid(0)
	{
		ZeroMemory(m_packetLenMap_table, sizeof(m_packetLenMap_table));
	};
	virtual ~CRoCodeBind();


	void Init(IDirect3DDevice7* d3ddevice);

	void DrawSRHDebug(IDirect3DDevice7* d3ddevice);
	void DrawOn3DMap(IDirect3DDevice7* d3ddevice);

	int GetPacketLength(int opcode);
	void PacketQueueProc(char *buf,int len);

	void InitWindowHandle(HWND hWnd){m_hWnd = hWnd;};

	const char *GetItemNameByID(int id);

	void *GetPak(const char *name, unsigned int *size);
	void ReleasePak(void *handle);


	void OneSyncProc(HRESULT Result, LPVOID lpvData, BOOL FreeMouse);
	void SetMouseCurPos(int x,int y);
};


extern BOOL g_FreeMouseSw;
extern StSHAREDMEMORY *g_pSharedData;

extern CRoCodeBind* g_pRoCodeBind;

BOOL OpenSharedMemory(void);
BOOL ReleaseSharedMemory(void);


extern CPerformanceCounter g_PerformanceCounter;



