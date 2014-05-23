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

// Required to create a dll for jRO.
//#define JRO_CLIENT_STRUCTURE
//

#include "ro/system.h"
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

class CRoCodeBind
{
private:
	CRenderer** g_renderer;
	CModeMgr *g_pmodeMgr;
	CMouse* g_mouse;

	struct p_std_map_packetlen
	{
		struct p_std_map_packetlen *left, *parent, *right;
		DWORD key;
		int value;
	};

	#define ROPACKET_MAXLEN 0x2000
	int m_packetLenMap_table[ROPACKET_MAXLEN];
	int m_packetLenMap_table_index;

	#define PACKETQUEUE_BUFFERSIZE 40960
	char m_packetqueuebuffer[PACKETQUEUE_BUFFERSIZE];
	unsigned int m_packetqueue_head;

	int GetTreeData(p_std_map_packetlen* node);

	void LoadIni(void);
	void SearchRagexeMemory(void);

	void ProjectVertex(vector3d& src,matrix& vtm,float *x,float *y,float *oow);
	void ProjectVertex(vector3d& src,matrix& vtm,tlvertex3d *vert);

public:
	CRoCodeBind(IDirect3DDevice7* d3ddevice);
	virtual ~CRoCodeBind();


	void DrawSRHDebug(IDirect3DDevice7* d3ddevice);
	void DrawOn3DMap(IDirect3DDevice7* d3ddevice);
	void DrawM2E(IDirect3DDevice7* d3ddevice);

	int GetPacketLength(int opcode);
	void PacketQueueProc(char *buf,int len);

	void SetMouseCurPos(int x,int y);
};





typedef void (__cdecl *tPlayStream)(const char *streamFileName,int playflag);


extern BOOL g_MouseFreeSw;
extern StSHAREDMEMORY *g_pSharedData;

extern tPlayStream funcRagexe_PlayStream;
extern CMouse* g_mouse;

extern CRoCodeBind* g_pRoCodeBind;

BOOL OpenSharedMemory(void);
BOOL ReleaceSharedMemory(void);


extern CPerformanceCounter g_PerformanceCounter;


void SearchRagexeMemory(void);

