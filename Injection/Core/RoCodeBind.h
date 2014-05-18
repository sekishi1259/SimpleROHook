#pragma once

#include "shared.h"

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

void InitRODraw(IDirect3DDevice7* d3ddevice);
void ReleaseRODraw(void);

void LoadIni(void);
void DrawM2E(IDirect3DDevice7* d3ddevice);

void DrawOn3DMap(IDirect3DDevice7* d3ddevice);
void DrawSRHDebug(IDirect3DDevice7* d3ddevice);

void PacketQueueProc(char *buf,int len);


struct VERTEX {
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};


typedef void (__cdecl *tPlayStream)(const char *streamFileName,int playflag);

void SetScreenSize(int width,int height);

void Transform2screen(struct vector3d& i,vector3d& o,float& rhw);

extern BOOL g_MouseFreeSw;
extern StSHAREDMEMORY *g_pSharedData;
extern DWORD g_MonitorRefreshRate;
extern tPlayStream funcRagexe_PlayStream;


extern DWORD g_CModeMgr__GetGameMode;
extern CModeMgr *g_pmodeMgr;

BOOL OpenSharedMemory(void);
BOOL ReleaceSharedMemory(void);


class CPerformanceCounter
{
private:
	LONGLONG m_OldCounter;
	LARGE_INTEGER m_Counter;
	double m_dFreq;

	LONGLONG m_OldInstantCounter;


	double *m_dticklist;
	int     m_ticklist_index;
	int     m_ticklist_size;

	int		m_FrameCounter;
	int		m_FrameCount;
	int		m_SampleTerm;
	int		m_FrameList[1024];
	int		m_MonitorRefreshRate;
	double  m_FrameRate;

public:
	CPerformanceCounter(int step) :  m_OldCounter(0),m_ticklist_index(0),m_FrameCounter(0),m_FrameCount(0),m_SampleTerm(1000),m_MonitorRefreshRate(60),m_FrameRate(0.0f)
	{
		if( QueryPerformanceCounter( &m_Counter ) != 0){
			m_OldCounter = m_Counter.QuadPart;
			LARGE_INTEGER Freq;
			QueryPerformanceFrequency( &Freq );
			m_dFreq = (double)Freq.QuadPart;
		}else{
			m_OldCounter = (LONGLONG)::timeGetTime();
		}
		m_ticklist_size = step;
		m_dticklist = new double[m_ticklist_size];
		for(int ii = 0;ii < m_ticklist_size;ii++)
			m_dticklist[ ii ] = 0.0;
	}
	virtual ~CPerformanceCounter()
	{
		delete[] m_dticklist;
	}
	void InitInstaltPerformance(void)
	{
		if( QueryPerformanceCounter( &m_Counter ) != 0){
			m_OldInstantCounter = m_Counter.QuadPart;
		}else{
			m_OldInstantCounter = (LONGLONG)::timeGetTime();;
		}
	}
	void SetMonitorRefreshRate(int value)
	{
		m_MonitorRefreshRate = value;
	}
	int GetMonitorRefreshRate(void)
	{
		return m_MonitorRefreshRate;
	}

	int GetFrameRate(void)
	{
		return (int)(m_FrameRate + 0.9);
	}
	void ModifiFrameRate(void)
	{
		int Tick = timeGetTime();
		for(int index = m_FrameCount - 1; index >= 0; index --){
			if(m_FrameList[index] + m_SampleTerm <= Tick){
				m_FrameCount = index;
			}else{
				break;
			}
		}
		memmove(m_FrameList + 1, m_FrameList, sizeof(m_FrameList[0]) * m_FrameCount);
		m_FrameList[0] = Tick;
		m_FrameCount ++;
		if(m_FrameCount > sizeof(m_FrameList) / sizeof(m_FrameList[0]) - 10){
			m_FrameCount = sizeof(m_FrameList) / sizeof(m_FrameList[0]) - 10;
			m_FrameRate = -1.0;
			return;
		}else{
			int Time = Tick - m_FrameList[m_FrameCount - 1];
			if(Time > 0 && m_FrameCount >= 2){
				m_FrameRate = (double)(m_FrameCount - 1) * (double)m_SampleTerm / (double)Time;

				if(m_FrameRate > (double)m_MonitorRefreshRate)
					m_FrameRate = (double)m_MonitorRefreshRate;
			}else{
				m_FrameRate = -1.0;
			}
		}
	}
	double CalcInstaltPerformance(void)
	{
		double tick;
		if( QueryPerformanceCounter( &m_Counter ) != 0){
			LONGLONG m_temptick = m_Counter.QuadPart;
			tick = ((double)(m_temptick - m_OldInstantCounter))*1000 / m_dFreq;
			m_OldInstantCounter = m_temptick;
		}else{
			LONGLONG m_temptick = (LONGLONG)::timeGetTime();
			tick = (double)(m_temptick - m_OldCounter);
			m_OldInstantCounter = m_temptick;
		}
		return tick;
	}

	void SetCounter(double tick)
	{
		m_dticklist[ m_ticklist_index ] = tick;
		m_ticklist_index = (m_ticklist_index+1) % m_ticklist_size;
	}
	void ModifiCounter(void)
	{
		if( QueryPerformanceCounter( &m_Counter ) != 0){
			LONGLONG m_temptick = m_Counter.QuadPart;
			double tick = ((double)(m_temptick - m_OldCounter))*1000 / m_dFreq;
			m_OldCounter = m_temptick;
			SetCounter(tick);
		}else{
			LONGLONG m_temptick = (LONGLONG)::timeGetTime();
			double tick = (double)(m_temptick - m_OldCounter);
			m_OldCounter = m_temptick;
			SetCounter(tick);
		}
	}
	double GetTotalTick(void)
	{
		double tick = 0.0;
		for(int ii = 0;ii < m_ticklist_size;ii++)
			tick += m_dticklist[ ii ];
		tick /= (double)m_ticklist_size;
		return tick;
	}
};
extern CPerformanceCounter g_PerformanceCounter;


void SearchRagexeMemory(void);

typedef struct StFindMemInfo{
	unsigned char x;
	char          flag;
}StFindMemInfo;

class CSearchCode
{
private:
	std::vector<StFindMemInfo> m_FindInfo;
	std::map<char,int> m_MakerIndex;

	char ahex2i(char code)
	{
		if( code >= '0' && code <= '9' ){
			return (code - '0');
		}else
		if( code >= 'a' && code <= 'z' ){
			return (code - 'a' + 0x0a );
		}else
		if( code >= 'A' && code <= 'Z' ){
			return (code - 'A' + 0x0a );
		}
		return -1;
	}

public:
	CSearchCode(char *pattern)
	{
		while( *pattern != 0 )
		{
			StFindMemInfo tempInfo;
			char h1,h2;
			h1 = *pattern++;
			h2 = *pattern++;
			if(!h2)break;

			if( h1 == '*' ){
				// wildcard
				tempInfo.x    = 0x00;
				tempInfo.flag = 0;
				m_FindInfo.push_back( tempInfo );
				if( h2 != '*' )
					m_MakerIndex[ h2 ] = m_FindInfo.size() - 1;
			}else{
				tempInfo.x = (ahex2i(h1)<<4) | ahex2i(h2);
				tempInfo.flag = 1;
				m_FindInfo.push_back( tempInfo );
			}
		}
	}
	~CSearchCode(){}

	int GetMakerIndex(char code)
	{
		return m_MakerIndex[ code ];
	}
	BOOL PatternMatcher(LPBYTE address)
	{
		int nums = m_FindInfo.size();
		for(int ii = 0;ii < nums;ii ++)
		{
			if( m_FindInfo[ii].flag && ( m_FindInfo[ii].x != address[ii] ) )
				return FALSE;
		}
		return TRUE;
	}
	DWORD GetImmediateDWORD(LPBYTE address,char code)
	{
		return *(DWORD*)(address + GetMakerIndex( code ));
	}
	DWORD Get4BIndexDWORD(LPBYTE address,char code)
	{
		LPBYTE targetaddress = address + GetMakerIndex( code ); 
		return (*(DWORD*)targetaddress) + (DWORD)(targetaddress + 4);
	}
	DWORD GetNearJmpAddress(LPBYTE address,char code)
	{
		DWORD im_calladdress = *(DWORD*)(address + GetMakerIndex( code ));
		// convert to immediate address 
		im_calladdress += (DWORD)address + GetMakerIndex( code ) + 4;

		return im_calladdress;
	}

	BOOL NearJmpAddressMatcher(LPBYTE address,char code,DWORD calladdress)
	{
		DWORD im_calladdress = *(DWORD*)(address + GetMakerIndex( code ));
		// convert to immediate address 
		im_calladdress += (DWORD)address + GetMakerIndex( code ) + 4;

		if( im_calladdress == calladdress )
			return TRUE;

		return FALSE;
	}

	int GetSize(){ return m_FindInfo.size(); }
};
