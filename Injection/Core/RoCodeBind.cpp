#include "stdafx.h"
#include "../tinyconsole.h"
#include "RoCodeBind.h"
#include "FastFont/SFastFont.h"

HANDLE         g_hMapObject = 0;
StSHAREDMEMORY *g_pSharedData = 0;
BOOL g_MouseFreeSw = FALSE;

#define kFloorSkillTypeMAX 0x100
DWORD g_M2ESkillColor[kFloorSkillTypeMAX];

CSFastFont *g_pSFastFont = NULL;
LPDIRECTDRAWSURFACE7 g_pddsFontTexture = NULL;

static HRESULT CALLBACK TextureSearchCallback( DDPIXELFORMAT* pddpf,
                                               VOID* param )
{
    if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
        return DDENUMRET_OK;
    if( pddpf->dwFourCC != 0 )
        return DDENUMRET_OK;
    if( pddpf->dwRGBBitCount != 16 )
        return DDENUMRET_OK;
    if(!(pddpf->dwFlags&DDPF_ALPHAPIXELS) )
        return DDENUMRET_OK;
	// get 16 bit with alphapixel format
    memcpy( (DDPIXELFORMAT*)param, pddpf, sizeof(DDPIXELFORMAT) );
    return DDENUMRET_CANCEL;
}

void InitRODraw(IDirect3DDevice7* d3ddevice)
{
	SearchRagexeMemory();
	LoadIni();

	D3DDEVICEDESC7 ddDesc;
	d3ddevice->GetCaps( &ddDesc );

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize          = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|
							DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
	ddsd.dwWidth         = 512;
	ddsd.dwHeight        = 512;

	if( ddDesc.deviceGUID == IID_IDirect3DHALDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	d3ddevice->EnumTextureFormats( TextureSearchCallback, &ddsd.ddpfPixelFormat );
	if( ddsd.ddpfPixelFormat.dwRGBBitCount ){
		LPDIRECTDRAWSURFACE7 pddsRender = NULL;
		LPDIRECTDRAW7        pDD = NULL;

		d3ddevice->GetRenderTarget( &pddsRender );
		if( pddsRender ){
			pddsRender->GetDDInterface( (VOID**)&pDD );
			pddsRender->Release();
		}
		if( pDD ){
			if( SUCCEEDED( pDD->CreateSurface( &ddsd, &g_pddsFontTexture, NULL )) ){
				kDD_LOGGING2(( "font texture created." ));
			} else {
				kDD_LOGGING2(( "failed create a font texture." ));
			}
			pDD->Release();
		}
		if( g_pddsFontTexture ){
			LOGFONT logfont;
			logfont.lfHeight         = -10;
			logfont.lfWidth          = 0;
			logfont.lfEscapement     = 0;
			logfont.lfOrientation    = 0;
			logfont.lfWeight         = FW_REGULAR;
			//
			logfont.lfItalic         = FALSE;
			logfont.lfUnderline      = FALSE;
			logfont.lfStrikeOut      = FALSE;
			logfont.lfCharSet        = DEFAULT_CHARSET;
			logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS; 
			logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS; 
			logfont.lfQuality        = NONANTIALIASED_QUALITY; 
			logfont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE; 
			_tcscpy_s(logfont.lfFaceName,_T("‚l‚r ‚oƒSƒVƒbƒN"));

			g_pSFastFont = new CSFastFont;
			g_pSFastFont->CreateFastFont(&logfont,d3ddevice,g_pddsFontTexture,0);
		}
	}

}

void ReleaseRODraw(void)
{
	if( g_pSFastFont )
		delete g_pSFastFont;
	if( g_pddsFontTexture ){
		g_pddsFontTexture->Release();
		g_pddsFontTexture = NULL;
	}
}



int s_screen_width = 0;
int s_screen_height = 0;

void SetScreenSize(int width,int height)
{
	s_screen_width  = width;
	s_screen_height = height;
}

void vector3d::MatrixMult(struct vector3d& v, struct matrix& m)
{
	x = v.x * m.v11 + v.y * m.v21 + v.z * m.v31;
	y = v.x * m.v12 + v.y * m.v22 + v.z * m.v32;
	z = v.x * m.v13 + v.y * m.v23 + v.z * m.v33;
};

void Transform2screen(struct vector3d& i,vector3d& o,float& rhw)
{
	matrix projmatrix = {
	7.595756f     , 0.0f      , 0.0f ,
		0.0f      , 7.595756f , 0.0f ,
		0.0f      , 0.0f      , 1.006711f ,
		0.0f      , 0.0f      ,-10.067114f
	};

	o.MatrixMult(i,projmatrix);

	o.x = o.x / i.z;
	o.y = o.y / i.z;
	o.z = o.z / i.z + projmatrix.v43 / (i.z-1.0f);

//	o.x = o.x * (s_screen_height * 0.5f) + (s_screen_width  * 0.5f);
//	o.y = o.y * (s_screen_height * 0.5f) + (s_screen_height * 0.5f);

	o.x = (o.x * s_screen_height + s_screen_width  ) * 0.5f;
	o.y = (o.y * s_screen_height + s_screen_height ) * 0.5f;

	rhw = 1.0f / i.z;
}

void LoadIni(void)
{
	if( g_pSharedData ){

		int sectionsize;
		char Sectionbuf[32768];
		char *pkey;
		char filename[MAX_PATH];

		if( ::WideCharToMultiByte(CP_ACP,0,
			g_pSharedData->configfilepath,wcslen(g_pSharedData->configfilepath)+1,
			filename,sizeof(filename),
			NULL,NULL) ){

		kDD_LOGGING2( ("LoadIni startup") );
		kDD_LOGGING2( ("%s",filename) );

			sectionsize = GetPrivateProfileSection(_T("M2E"),Sectionbuf,32768,filename);
			pkey = Sectionbuf;

			for(int ii = 0;ii < kFloorSkillTypeMAX;ii++)
				g_M2ESkillColor[ii]=0;

			
			while(*pkey!='\0'){
				int index;
				DWORD color;

				char *ptemp;
				ptemp = pkey;

				std::string linestring(ptemp);

				pkey += linestring.length();

				sscanf_s(linestring.c_str(),"Skill%x=%x",&index,&color);
				g_M2ESkillColor[index] = color;
				pkey++;
			}
		}
	}
}

void DrawSRHDebug(IDirect3DDevice7* d3ddevice)
{
	if( !g_pSharedData )return;

	if( g_pSharedData->show_framerate ){
		std::stringstream str;
		str << g_PerformanceCounter.GetFrameRate() << "fps : "<<(int)g_PerformanceCounter.GetTotalTick() << std::endl;

		g_pSFastFont->DrawText((LPSTR)str.str().c_str(), 0, 0,D3DCOLOR_ARGB(255,255,255,255),0,NULL);
	}

	if( g_pSharedData->objectinformation ){
		std::stringstream str;
		CModeMgr *pcmode = g_pmodeMgr;
		//str.str("");
		CGameMode *p_gamemode = (CGameMode*)pcmode->m_curMode;

		if( p_gamemode && pcmode->m_curModeType == 1 
		 && p_gamemode->m_world && p_gamemode->m_view && p_gamemode->m_world->m_attr ){
			CView *pView = p_gamemode->m_view;

			C3dAttr *pAttr = p_gamemode->m_world->m_attr;

			if( p_gamemode->m_world->m_player ){
				C3dAttr *pattr = p_gamemode->m_world->m_attr;
				ViewInfo3d *pVinfo = &pView->m_cur;
				vector3d pos;
				vector3d tpos,spos;
				float rhw;
				std::stringstream putinfostr;
				CPlayer *pPlayer = (CPlayer*)p_gamemode->m_world->m_player;

				str << "m_clevel = " << pPlayer->m_clevel << "\n";
				str << "m_gid = " << std::hex << (unsigned long)pPlayer->m_gid << "\n";
				str << "m_job = " << std::hex << (unsigned long)pPlayer->m_job << "\n";
				str << "m_sex = " << std::hex << (unsigned long)pPlayer->m_sex << "\n";


				//
				//  world position to cell position
				//
				pos.x = pPlayer->m_pos.x - pVinfo->at.x;
				pos.y = pPlayer->m_pos.y - pVinfo->at.y;
				pos.z = pPlayer->m_pos.z - pVinfo->at.z;
				//
				tpos.MatrixMult(pos,pView->m_viewMatrix);
				tpos.z += pVinfo->distance;
				tpos += pView->m_up;
				long cx,cy;
				pattr->ConvertToCellCoor(pPlayer->m_pos.x,pPlayer->m_pos.z,cx,cy);
				//
				//
				//
				putinfostr << "(" << cx << "," << cy << ")" << std::endl;
				int sx,sy;
				Transform2screen(tpos,spos,rhw);
				sx = (int)spos.x;
				sy = (int)spos.y;
				g_pSFastFont->DrawText((LPSTR)putinfostr.str().c_str(), sx, sy,D3DCOLOR_ARGB(255,255,255,255),2,NULL);
			}

			int skillnums = p_gamemode->m_world->m_skillList.size();
			str << " m_skillList size =" << skillnums << "\n";

			int itemnums = p_gamemode->m_world->m_itemList.size();
			str << " m_itemList size =" << itemnums << "\n";

			std::list<CItem*> itemList = p_gamemode->m_world->m_itemList;
			for( std::list<CItem*>::iterator it = itemList.begin() ; it != itemList.end() ; it++ )
			{
				CItem *pItem = *it;
				if( pItem ){
					ViewInfo3d *pVinfo = &pView->m_cur;
					vector3d pos;
					vector3d tpos,spos;
					float rhw;
					pos.x = pItem->m_pos.x - pVinfo->at.x;
					pos.y = pItem->m_pos.y - pVinfo->at.y;
					pos.z = pItem->m_pos.z - pVinfo->at.z;
					//
					tpos.MatrixMult(pos,pView->m_viewMatrix);
					tpos.z += pVinfo->distance;

					Transform2screen(tpos,spos,rhw);

					long cx,cy;
					pAttr->ConvertToCellCoor(pItem->m_pos.x,pItem->m_pos.z,cx,cy);

					std::stringstream putinfostr;
					putinfostr << "(" << cx << "," << cy << ")" << std::endl;
					//putinfostr << pItem->m_itemName << std::endl;
				//	putinfostr << "aid = " << pItem->m_aid << std::endl;
					putinfostr << "itemid = " << pItem->m_itemid2 << std::endl;

					int sx,sy;
					sx = (int)spos.x;
					sy = (int)spos.y;
					g_pSFastFont->DrawText((LPSTR)putinfostr.str().c_str(), sx, sy,D3DCOLOR_ARGB(255,255,255,255),2,NULL);
				}
			}


			int actornums = p_gamemode->m_world->m_actorList.size();
			str << " m_actorList size =" << actornums << "\n";
			std::list<CGameActor*> actorList = p_gamemode->m_world->m_actorList;
			for( std::list<CGameActor*>::iterator it = actorList.begin() ; it != actorList.end() ; it++ )
			{
				CGameActor *pGameActor = *it;
				if( pGameActor ){
					ViewInfo3d *pVinfo = &pView->m_cur;
					vector3d pos;
					vector3d tpos,spos;
					float rhw;
					pos.x = pGameActor->m_pos.x - pVinfo->at.x;
					pos.y = pGameActor->m_pos.y - pVinfo->at.y;
					pos.z = pGameActor->m_pos.z - pVinfo->at.z;
					//
					tpos.MatrixMult(pos,pView->m_viewMatrix);
					tpos.z += pVinfo->distance;

					Transform2screen(tpos,spos,rhw);

					long cx,cy;
					pAttr->ConvertToCellCoor(pGameActor->m_pos.x,pGameActor->m_pos.z,cx,cy);

					std::stringstream putinfostr;
					putinfostr << "(" << cx << "," << cy << ")" << std::endl;
				//	putinfostr << "dest(" << pGameActor->m_moveDestX << "," << pGameActor->m_moveDestY << ")" << std::endl;
					putinfostr << "lv = " << pGameActor->m_clevel << std::endl;
					putinfostr << "job = " << pGameActor->m_job << std::endl;

					int sx,sy;
					sx = (int)spos.x;
					sy = (int)spos.y;
					g_pSFastFont->DrawText((LPSTR)putinfostr.str().c_str(), sx, sy,D3DCOLOR_ARGB(255,255,255,255),2,NULL);
				}
			}
			
		}
		str << std::endl;
		g_pSFastFont->DrawText((LPSTR)str.str().c_str(), 0, 16,D3DCOLOR_ARGB(255,255,255,255),0,NULL);
	}
	g_pSFastFont->Flush();

}

void DrawOn3DMap(IDirect3DDevice7* d3ddevice)
{
	DrawM2E(d3ddevice);
}

void DrawM2E(IDirect3DDevice7* d3ddevice)
{
	if( !g_pSharedData )return;
	if( g_pSharedData->m2e == FALSE )return;
	if( !g_pmodeMgr )return;

	CModeMgr *pcmode = g_pmodeMgr;
	CWorld *pWorld;
	CView *pView;

	if( pcmode->m_curMode && pcmode->m_curModeType == 1 ){
		CGameMode *pGamemode = (CGameMode*)pcmode->m_curMode;
		pWorld = pGamemode->m_world;
		pView  = pGamemode->m_view;

		DWORD _state_zenable;
		DWORD _state_zwriteenable;
		DWORD _state_zbias;
		DWORD _state_fogenable;
		DWORD _state_specularenable;
		DWORD _state_alphafunc;
		DWORD _state_alpharef;
		DWORD _state_srcblend;
		DWORD _state_destblend;
		//LPDIRECTDRAWSURFACE7 lpOriginalTexture = NULL;
		//d3ddevice->GetTexture(0, &lpOriginalTexture);
		d3ddevice->GetRenderState(D3DRENDERSTATE_ZENABLE,        &_state_zenable);
		d3ddevice->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE,   &_state_zwriteenable);
		d3ddevice->GetRenderState(D3DRENDERSTATE_ZBIAS,          &_state_zbias);
		d3ddevice->GetRenderState(D3DRENDERSTATE_FOGENABLE,      &_state_fogenable);
		d3ddevice->GetRenderState(D3DRENDERSTATE_SPECULARENABLE, &_state_specularenable);
		d3ddevice->GetRenderState(D3DRENDERSTATE_ALPHAFUNC,      &_state_alphafunc);
		d3ddevice->GetRenderState(D3DRENDERSTATE_ALPHAREF,       &_state_alpharef);
		d3ddevice->GetRenderState(D3DRENDERSTATE_SRCBLEND ,      &_state_srcblend);
		d3ddevice->GetRenderState(D3DRENDERSTATE_DESTBLEND,      &_state_destblend);

		d3ddevice->SetTexture(0, NULL);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZBIAS, 2);
		d3ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE    ,FALSE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE   ,FALSE);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC,D3DCMP_GREATER);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAREF,0x00);
		d3ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND ,D3DBLEND_SRCALPHA );
		d3ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA);

		if( pWorld && pView && pWorld->m_attr ){
			C3dAttr *pAttr = pWorld->m_attr;

			int skillnums = pWorld->m_skillList.size();
			std::list<CSkill*> skillList = pWorld->m_skillList;

			for( std::list<CSkill*>::iterator it = skillList.begin() ; it != skillList.end() ; it++ )
			{
				CSkill *pSkill = *it;
				//
				if( pSkill && pSkill->m_job < 0x100 && g_M2ESkillColor[pSkill->m_job] ){
					DWORD color = g_M2ESkillColor[pSkill->m_job];
					VERTEX vertex[4] =
					{
						{   0.0,  0.0,   0.0f,  1.0f, color },
						{ 100.0,  0.0,   0.0f,  1.0f, color },
						{   0.0,100.0,   0.0f,  1.0f, color },
						{ 100.0,100.0,   0.0f,  1.0f, color }
					};
					ViewInfo3d *pVinfo = &pView->m_cur;
					vector3d pos;
					vector3d tpos,spos;
					float rhw;
					pos.x = pSkill->m_pos.x - pVinfo->at.x;
					pos.y = pSkill->m_pos.y - pVinfo->at.y;
					pos.z = pSkill->m_pos.z - pVinfo->at.z;
					//
					tpos.MatrixMult(pos,pView->m_viewMatrix);
					tpos.z += pVinfo->distance;

					long cx,cy;
					CAttrCell *pCell;
					vector3d centerpos(pSkill->m_pos),polvect[4];

					pAttr->ConvertToCellCoor(centerpos.x,centerpos.z,cx,cy);
					pCell = pAttr->GetCell(cx, cy);

					polvect[0].Set(centerpos.x -2.4f, pCell->h1 ,centerpos.z -2.4f);
					polvect[1].Set(centerpos.x +2.4f, pCell->h2 ,centerpos.z -2.4f);
					polvect[2].Set(centerpos.x -2.4f, pCell->h3 ,centerpos.z +2.4f);
					polvect[3].Set(centerpos.x +2.4f, pCell->h4 ,centerpos.z +2.4f);

					for(int ii = 0; ii < 4; ii ++){
						pos.x = polvect[ii].x - pVinfo->at.x;
						pos.y = polvect[ii].y - pVinfo->at.y;
						pos.z = polvect[ii].z - pVinfo->at.z;
						tpos.MatrixMult(pos,pView->m_viewMatrix);
						tpos.z += pVinfo->distance;
						Transform2screen(tpos,spos,rhw);
						vertex[ii].x = spos.x;
						vertex[ii].y = spos.y;
						vertex[ii].z = spos.z;
						vertex[ii].rhw = rhw;
					}
					d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_CPOLVERTEX, vertex, 4, 0);
				}
			}
		}
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZENABLE,        _state_zenable);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,   _state_zwriteenable);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ZBIAS,          _state_zbias);
		d3ddevice->SetRenderState(D3DRENDERSTATE_FOGENABLE,      _state_specularenable);
		d3ddevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, _state_specularenable);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC,      _state_alphafunc);
		d3ddevice->SetRenderState(D3DRENDERSTATE_ALPHAREF,       _state_alpharef);
		d3ddevice->SetRenderState(D3DRENDERSTATE_SRCBLEND ,      _state_srcblend);
		d3ddevice->SetRenderState(D3DRENDERSTATE_DESTBLEND,      _state_destblend);
		//d3ddevice->SetTexture(0, lpOriginalTexture);
	}
}




CPerformanceCounter g_PerformanceCounter(60);

DWORD g_ROmouse = NULL;
DWORD g_CModeMgr__GetGameMode = NULL;
DWORD g_CViewOffset_CGameMode = 0;

CModeMgr *g_pmodeMgr = NULL;

tPlayStream funcRagexe_PlayStream = NULL;

BOOL OpenSharedMemory(void)
{
	g_hMapObject = ::OpenFileMapping( FILE_MAP_ALL_ACCESS , FALSE, kSHAREDMEMORY_OBJECTNAME );
	if( !g_hMapObject ){
		kDD_LOGGING2( ("shared memory:Initialize Failed.") );
		return FALSE;
	}
	g_pSharedData = (StSHAREDMEMORY*)::MapViewOfFile(g_hMapObject,
		FILE_MAP_ALL_ACCESS,0,0,0);
	if(!g_pSharedData){
		kDD_LOGGING2( ("shared memory:DataMap Failed.") );
		::CloseHandle( g_hMapObject);
		g_hMapObject = NULL;
		return FALSE;
	}
	return TRUE;
}
BOOL ReleaceSharedMemory(void)
{
	if(g_pSharedData){
		g_pSharedData->g_hROWindow = 0;
		::UnmapViewOfFile( g_pSharedData );
		g_pSharedData = NULL;
	}
	if(g_hMapObject){
		::CloseHandle( g_hMapObject);
		g_hMapObject = NULL;
	}
	return TRUE;
}



DWORD g_temp_eax,g_temp_ecx,g_temp_edx,g_temp_ebx,g_temp_esp;

typedef int (__stdcall *Func_CRagConnection__GetPacketSize)(DWORD opcode);

struct p_std_map_packetlen
{
	struct p_std_map_packetlen *left, *parent, *right;
	DWORD key;
	int value;
};


#define ROPACKET_MAXLEN 0x2000

int g_packetLenMap_table[ROPACKET_MAXLEN];
int g_packetLenMap_table_index = 0;
 int GetPacketLength(int opcode)
{
	int result = -1;

	if( g_packetLenMap_table_index >= opcode )
	{
		result = g_packetLenMap_table[ opcode ];
	}
	return result;
}

int GetTreeData(p_std_map_packetlen* node) 
{
	if ( node->parent == NULL || node->key >= ROPACKET_MAXLEN || node->key == 0)
		return g_packetLenMap_table_index;

	GetTreeData( node->left );

	if( g_packetLenMap_table_index < (int)node->key )
		 g_packetLenMap_table_index = node->key;

	g_packetLenMap_table[ node->key ] = node->value;
	//kDD_LOGGING2( ("[ %08X ] = %d",node->key,node->value) );

	GetTreeData( node->right );

	return g_packetLenMap_table_index;
}

#define PACKETQUEUE_BUFFERSIZE 40960
char packetqueuebuffer[PACKETQUEUE_BUFFERSIZE];
unsigned int packetqueue_head = 0;

void PacketQueueProc(char *buf,int len)
{
	if( len > 0 ){
		if( packetqueue_head + len >= PACKETQUEUE_BUFFERSIZE ){
			kDD_LOGGING2( ("packet buffer has overflowed.\n") );
			return;
		}
		memcpy( &packetqueuebuffer[packetqueue_head] , buf,
			len);
		packetqueue_head += len;
		while( packetqueue_head > 1 ){
			unsigned short opcode = *(unsigned short*)packetqueuebuffer;
			unsigned int packetlength;

			if( packetqueue_head == 4 && len == 4 ){
				packetlength = 4;
			}else{
				packetlength = GetPacketLength( opcode );
				if( packetlength == -1 ){
					if( packetqueue_head < 4 )break;
					packetlength = *(unsigned int*)packetqueuebuffer;
					packetlength >>= 16;
				}
			}
			if( packetqueue_head >= packetlength ){
				if( g_pSharedData && g_pSharedData->write_packetlog ){
					std::stringstream str;
					str << "[" << std::setfill('0') << std::setw(8) << timeGetTime() << "] R ";
					for(int ii = 0;ii < (int)packetlength ;ii++)
						str << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (int)(unsigned char)packetqueuebuffer[ii] << " ";
					str << std::flush;
					//kDD_LOGGING2( ("Opcode %04X size = %d / %d\n",opcode,packetlength,packetqueue_head) );
					kDD_LOGGING2(( str.str().c_str() ));
				}

				packetqueue_head -= packetlength;
				if( packetqueue_head ){
					memmove( packetqueuebuffer, &packetqueuebuffer[packetlength],
						packetqueue_head );
				}
			}else{
				break;
			}
		}
	}
}



void SearchRagexeMemory(void)
{
	// CZ_UIYourItemWnd::SendMsg CZ_REQ_WEAR_EQUIP handler
	// Marker '1' CModeMgr g_modeMgr (C++ Class Instance)
	// Marker '2' CModeMgr::GetGameMode
	// Marker '3' CRagConnection::instanceR
	// Marker '4' CRagConnection::GetPacketSize
	// Marker '5' CRagConnection::SendPacket
	// Marker '6' UIWindowMgr g_windowMgr (C++ Class Instance)
	// Marker '7' UIWindowMgr::DeleteWindow
	// based RagFree.exe iRO vc6
	CSearchCode UIYourItemWnd__SendMsg_0A9Handler_TypeA(
		"b9*1******"		// mov     ecx,dword L008208d0 ; CModeMgr__g_modeMgr
		"e8*2******"		// call    near F0052c0e0 ; CModeMgr__GetGameMode
		"668b536c"			// mov     dx,[ebx+06ch]
		"668b4370"			// mov     ax,[ebx+070h]
		"8d4dec"			// lea     ecx,[ebp-014h]
		"66c745eca900"		// mov     word [ebp-014h],word 000a9h
		"51"				// push    ecx
		"68a9000000"		// push    dword 0000000a9h
		"668955ee"			// mov     [ebp-012h],dx
		"668945f0"			// mov     [ebp-010h],ax
		"e8*3******"		// call    near F004190f0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*4******"		// call    near F00419030 ; CRagConnection__GetPacketSize
		"50"				// push    eax
		"e8********"		// call    near F004190f0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*5******"		// call    near F00418f00 ; CRagConnection__SendPacket
		"68**000000"		// push    dword 00000008ah
		"b9*6******"		// mov     ecx,dword L007e7220 ; UIWindowMgr__g_windowMgr
		"e8*7******"		// call    near F00502390 ; UIWindowMgr__DeleteWindow
		);
	// F2P_Ragexe.exe iRO vc9
	CSearchCode UIYourItemWnd__SendMsg_0A9Handler_TypeB(
		"b9*1******"		// mov     ecx,dword L00812088 ; CModeMgr__g_modeMgr
		"e8*2******"		// call    near F0051eeb0 ; CModeMgr__GetGameMode
		"668b4e**"			// mov     cx,[esi+06ch]
		"668b56**"			// mov     dx,[esi+070h]
		"b8a9000000"		// mov     eax,dword 0000000a9h
		"6689442408"		// mov     [esp+008h],ax
		"8d442408"			// lea     eax,[esp+008h]
		"50"				// push    eax
		"68a9000000"		// push    dword 0000000a9h
		"66894c2412"		// mov     [esp+012h],cx
		"6689542414"		// mov     [esp+014h],dx
		"e8*3******"		// call    near F006335a0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*4******"		// call    near F006336d0 ; CRagConnection__GetPacketSize
		"50"				// push    eax
		"e8********"		// call    near F006335a0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*5******"		// call    near F00633550 ; CRagConnection__SendPacket
		"688a000000"		// push    dword 00000008ah
		"b9*6******"		// mov     ecx,dword L0083df40 ; UIWindowMgr__g_windowMgr
		"e8*7******"		// call    near F004f8270 ; UIWindowMgr__DeleteWindow
		);
	// based 2011-12-01aRagexe.exe iRO vc9
	CSearchCode UIYourItemWnd__SendMsg_0A9Handler_TypeC(
		"b9*1******"		// mov     ecx,dword L008371f8 ; CModeMgr__g_modeMgr
		"e8*2******"		// call    near F0054a7a0 ; CModeMgr__GetGameMode
		"668b4e**"			// mov     cx,[esi+07ch]
		"668b96**000000"	// mov     dx,[esi+000000080h]
		"b8a9000000"		// mov     eax,dword 0000000a9h
		"6689442408"		// mov     [esp+008h],ax
		"8d442408"			// lea     eax,[esp+008h]
		"50"				// push    eax
		"68a9000000"		// push    dword 0000000a9h
		"66894c2412"		// mov     [esp+012h],cx
		"6689542414"		// mov     [esp+014h],dx
		"e8*3******"		// call    near F0065d4d0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*4******"		// call    near F0065cf90 ; CRagConnection__GetPacketSize
		"50"				// push    eax
		"e8********"		// call    near F0065d4d0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*5******"		// call    near F0065d3f0 ; CRagConnection__SendPacket
		"688a000000"		// push    dword 00000008ah
		"b9*6******"		// mov     ecx,dword L008626b8 ; UIWindowMgr__g_windowMgr
		"e8*7******"		// call    near F00523770 ; UIWindowMgr__DeleteWindow
		);
	// 2014-03-14aRagexe.exe jRO
	CSearchCode UIYourItemWnd__SendMsg_0A9Handler_TypeD(
		"b9*1******"		// mov     ecx,dword L00812088 ; CModeMgr__g_modeMgr
		"e8*2******"		// call    near F0051eeb0 ; CModeMgr__GetGameMode
		"668b****"			// mov     cx,[esi+06ch]
		"668b****"			// mov     dx,[esi+070h]
		"b9a9000000"		// mov     eax,dword 0000000a9h
		"6689****"			// mov     [esp+008h],ax
		"8d****"			// lea     eax,[esp+008h]
		"51"				// push    eax
		"68a9000000"		// push    dword 0000000a9h
		"668955**"			// mov     [ebp-012h],dx
		"668945**"			// mov     [ebp-010h],ax
		"e8*3******"		// call    near F006335a0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*4******"		// call    near F006336d0 ; CRagConnection__GetPacketSize
		"50"				// push    eax
		"e8********"		// call    near F006335a0 ; CRagConnection__instanceR
		"8bc8"				// mov     ecx,eax
		"e8*5******"		// call    near F00633550 ; CRagConnection__SendPacket
		"688a000000"		// push    dword 00000008ah
		"b9*6******"		// mov     ecx,dword L0083df40 ; UIWindowMgr__g_windowMgr
		"e8*7******"		// call    near F004f8270 ; UIWindowMgr__DeleteWindow
		);
	// 2014-03-18 Ragexe.exe iRO link time 20140226 155100
	CSearchCode UIYourItemWnd__SendMsg_0A9Handler_TypeE(
		"b9*1******"        //         mov     ecx,dword L0099f930 ; CModeMgr__g_modeMgr
		"e8*2******"        //         call    near F005a5eb0 ; CModeMgr__GetGameMode
		"668b96********"    //         mov     dx,[esi+00000008ch]
		"668b86********"    //         mov     ax,[esi+000000090h]
		"b9a9000000"        //         mov     ecx,dword 0000000a9h
		"66894d**"          //         mov     [ebp-034h],cx
		"8d4d**"            //         lea     ecx,[ebp-034h]
		"51"                //         push    ecx
		"68a9000000"        //         push    dword 0000000a9h
		"668955**"          //         mov     [ebp-032h],dx
		"668945**"          //         mov     [ebp-030h],ax
		"e8*3******"        //         call    near F00716fc0 ; CRagConnection__instanceR
		"8bc8"              //         mov     ecx,eax
		"e8*4******"        //         call    near F007168e0 ; CRagConnection__GetPacketSize
		"50"                //         push    eax
		"e8********"        //         call    near F00716fc0 ; CRagConnection__instanceR
		"8bc8"              //         mov     ecx,eax
		"e8*5******"        //         call    near F00716ee0 ; CRagConnection__SendPacket
		"688a000000"        //         push    dword 00000008ah
		"b9*6******"        //         mov     ecx,dword L009d21c0 ; UIWindowMgr__g_windowMgr
		"e8*7******"        //         call    near F00573a40 ; UIWindowMgr__DeleteWindow
		);

	CSearchCode CMouse_Init_vc6(
		"a1********"		// mov     eax,[ g_hInstance ]
		"53"				// push    ebx
		"56"				// push    esi
		"33db"				// xor     ebx,ebx
		"57"				// push    edi
		"8bf1"				// mov     esi,ecx
		"53"				// push    ebx
		"56"				// push    esi
		"6800070000"		// push    dword 000000700h
		"50"				// push    eax
		);
	CSearchCode CMouse_Init_vc9(
		"a1********"		// mov     eax,[ g_hInstance ]
		"53"				// push    ebx
		"56"				// push    esi
		"33db"				// xor     ebx,ebx
		"53"				// push    ebx
		"8bf1"				// mov     esi,ecx
		"56"				// push    esi
		"6800070000"		// push    dword 000000700h
		"50"				// push    eax
		);
	CSearchCode winmain_init_CMouse_Init_call(
		"b9*1******"		// mov     ecx,g_mouse
		"e8*2******"		// call    near CMouse__Init
		"a1********"		// mov     eax, g_renderer__CRenderer
		);

	CSearchCode funcPlayStrem_based_HighPrest_exe(
		"55"                //   push    ebp
		"8bec"              //   mov     ebp,esp
		"a1*1******"        //   mov     eax,[L006f2534] ; g_soundMode
		"53"                //   push    ebx
		"85c0"              //   test    eax,eax
		"56"                //   push    esi
		"0f84c5000000"      //   jz      near C0040afa7
		"8b5d08"            //   mov     ebx,dword [ebp+008h] ;+streamFileName
		"be********"        //   mov     esi,dword L0074e7c4
		"8bc3"              //   mov     eax,ebx
		"8a10"              //   mov     dl,[eax]
		"8aca"              //   mov     cl,dl
		"3a16"              //   cmp     dl,[esi]
		"751c"              //   jnz     C0040af10                            
		"84c9"              //   test    cl,cl
		);
	CSearchCode funcPlayStrem_based_RagFree_exe(
		"55"                //   push    ebp
		"8bec"              //   mov     ebp,esp
		"81ec00010000"      //   sub     esp,dword 000000100h
		"a1*1******"        //   mov     eax,[L00775d44] ; g_soundMode
		"53"                //   push    ebx
		"85c0"              //   test    eax,eax
		"56"                //   push    esi
		"0f8417010000"      //   jz      near C0041b6bf
		"8b5d08"            //   mov     ebx,dword [ebp+008h] ;+streamFileName
		"be********"        //   mov     esi,dword L007cd9b4
		"8bc3"              //   mov     eax,ebx
		"8a10"              //   mov     dl,[eax]
		"8aca"              //   mov     cl,dl
		"3a16"              //   cmp     dl,[esi]
		"751c"              //   jnz     C0041b5d6                            
		"84c9"              //   test    cl,cl
		);
	CSearchCode funcPlayStrem_based_2011111201aRagexe_exe(
		"81ec04010000"      //   sub     esp,dword 000000104h
		"a1*1******"        //   mov     eax,[L00845990] ; g_soundMode
		"33c4"              //   xor     eax,esp
		"89842400010000"    //   mov     dword [esp+000000100h],eax
		"833d********00"    //   cmp     dword [L0084459c],byte +000h
		"56"                //   push    esi
		"8bb4240c010000"    //   mov     esi,dword [esp+00000010ch] ;+streamFileName
		"0f8406010000"      //   jz      near C0065fd9f
		"b9********"        //   mov     ecx,dword L008e348c
		"8bc6"              //   mov     eax,esi
		"8a10"              //   mov     dl,[eax]
		"3a11"              //   cmp     dl,[ecx]
		"751a"              //   jnz     C0065fcc0                                  
		"84d2"              //   test    dl,dl
		);
	CSearchCode funcPlayStrem_based_20140226_155100iRagexe_exe(
		// F00719260
		"55"                //   push    ebp
		"8bec"              //   mov     ebp,esp
		"81ec********"      //   sub     esp,dword 000000104h
		"a1*1******"        //   mov     eax,[L009b63d0] ; g_soundMode
		"33c5"              //   xor     eax,ebp
		"8945**"            //   mov     dword [ebp-004h],eax
		"833d********00"    //   cmp     dword [L00aa1610],byte +000h
		"56"                //   push    esi
		"8b75**"            //   mov     esi,dword [ebp+008h]
		"0f84********"      //   jz      near C00719393
		);

	CSearchCode SetView_CView_BuildViewMatrix_based_20140226_155100iRagexe_exe(
		// new
		// 005ad138
		"d88688000000"      //   fadd    dword [esi+000000088h]
		"d99e88000000"      //   fstp    dword [esi+000000088h]
		"e8*1******"        //   call    near F005a5eb0 ; CModeMgr__GetGameMode
		"d98688000000"      //   fld     dword [esi+000000088h]
		"8b80*2******"      //   mov     eax,dword [eax+0000000d0h]
		"8b4840"            //   mov     ecx,dword [eax+040h]
		);
	CSearchCode SetView_CView_BuildViewMatrix_based_HightPriest(
		// HightPriest & Ragexe2010_11_02aJ & Ragexe2011-04-29aJ
		//004f1f2e
		"d88688000000"      //   fadd    dword [esi+000000088h]
		"d99e88000000"      //   fstp    dword [esi+000000088h]
		"e8*1******"        //   call    near F004edf20
		"8b80*2******"      //   mov     eax,dword [eax+0000000b4h]
		"8b8e88000000"      //   mov     ecx,dword [esi+000000088h]
		"8b17"              //   mov     edx,dword [edi]
		"51"                //   push    ecx
		"8b4840"            //   mov     ecx,dword [eax+040h]
		);

	LPBYTE pRagexeBase;
	MEMORY_BASIC_INFORMATION mbi;

	pRagexeBase = (LPBYTE)::GetModuleHandle(NULL);
	pRagexeBase += 0x1000;

	if( ::VirtualQuery( pRagexeBase,&mbi,sizeof(mbi) ) )
	{
		DWORD ptr_CMouse_Init = 0;

		p_std_map_packetlen *packetLenMap = 0;

		kDD_LOGGING2( ("MEMORY_BASIC_INFORMATION lpAddres:%08X",pRagexeBase) );
		kDD_LOGGING2( ("mbi.AllocationBase = %08X",mbi.AllocationBase) );
		kDD_LOGGING2( ("mbi.BaseAddress    = %08X",mbi.BaseAddress) );
		kDD_LOGGING2( ("mbi.RegionSize     = %08X",mbi.RegionSize) );

		// snatch the packetLenMap
		for( UINT ii = 0; ii < mbi.RegionSize - 1000 ; ii++ )
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;

			if( UIYourItemWnd__SendMsg_0A9Handler_TypeA.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_0A9Handler_TypeA.Get4BIndexDWORD( &pBase[ii] , '4' );
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov g_temp_esp,esp

				p_std_map_packetlen *plen = (p_std_map_packetlen*)
					*(DWORD*) (*(DWORD*)(g_temp_esp-19*4) + 4);

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_0A9Handler_TypeA
					.GetImmediateDWORD( &pBase[ii], '1' );
				g_CModeMgr__GetGameMode = UIYourItemWnd__SendMsg_0A9Handler_TypeA
					.GetNearJmpAddress( &pBase[ii], '2' );

				kDD_LOGGING2( ("TypeA GetPacketSizeAddr %08X",GetPacketSizeAddr) );
				kDD_LOGGING2( (" esp = %08X",g_temp_esp) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						kDD_LOGGING2( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}else
			if( UIYourItemWnd__SendMsg_0A9Handler_TypeB.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_0A9Handler_TypeB.Get4BIndexDWORD( &pBase[ii] , '4' );
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov g_temp_eax,eax
				__asm mov g_temp_ecx,ecx
				__asm mov g_temp_edx,edx
				p_std_map_packetlen *plen;
				if( g_temp_eax == g_temp_edx ){
					plen = (p_std_map_packetlen*)g_temp_ecx;
				}else{
					plen = (p_std_map_packetlen*)g_temp_edx;
				}

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_0A9Handler_TypeB
					.GetImmediateDWORD( &pBase[ii], '1' );
				g_CModeMgr__GetGameMode = UIYourItemWnd__SendMsg_0A9Handler_TypeB
					.GetNearJmpAddress( &pBase[ii], '2' );

				kDD_LOGGING2( ("TypeB GetPacketSizeAddr     = %08X eax = %08X ecx = %08X edx =%08X",
					GetPacketSizeAddr,g_temp_eax,g_temp_ecx,g_temp_edx) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						kDD_LOGGING2( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}else
			if( UIYourItemWnd__SendMsg_0A9Handler_TypeC.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				Func_CRagConnection__GetPacketSize GetPacketSize;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_0A9Handler_TypeC.Get4BIndexDWORD( &pBase[ii] , '4' );
				GetPacketSize = (Func_CRagConnection__GetPacketSize)GetPacketSizeAddr;
				//GetPacketSize(0x0A9);
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov g_temp_ecx,ecx
				p_std_map_packetlen *plen = (p_std_map_packetlen*)g_temp_ecx;

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_0A9Handler_TypeC
					.GetImmediateDWORD( &pBase[ii], '1' );
				g_CModeMgr__GetGameMode = UIYourItemWnd__SendMsg_0A9Handler_TypeC
					.GetNearJmpAddress( &pBase[ii], '2' );

				kDD_LOGGING2( ("TypeC GetPacketSizeAddr     = %08X ecx = %08X",GetPacketSizeAddr,g_temp_ecx) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						kDD_LOGGING2( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}else
			if( UIYourItemWnd__SendMsg_0A9Handler_TypeD.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				Func_CRagConnection__GetPacketSize GetPacketSize;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_0A9Handler_TypeD.Get4BIndexDWORD( &pBase[ii] , '4' );
				GetPacketSize = (Func_CRagConnection__GetPacketSize)GetPacketSizeAddr;
				//GetPacketSize(0x0A9);
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov g_temp_ecx,ecx
				p_std_map_packetlen *plen = (p_std_map_packetlen*)g_temp_ecx;

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_0A9Handler_TypeD
					.GetImmediateDWORD( &pBase[ii], '1' );
				g_CModeMgr__GetGameMode = UIYourItemWnd__SendMsg_0A9Handler_TypeD
					.GetNearJmpAddress( &pBase[ii], '2' );

				kDD_LOGGING2( ("TypeD GetPacketSizeAddr     = %08X ecx = %08X",GetPacketSizeAddr,g_temp_ecx) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						kDD_LOGGING2( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}else
			if( UIYourItemWnd__SendMsg_0A9Handler_TypeE.PatternMatcher( &pBase[ii] ) )
			{
				DWORD GetPacketSizeAddr;
				Func_CRagConnection__GetPacketSize GetPacketSize;
				GetPacketSizeAddr = 
					UIYourItemWnd__SendMsg_0A9Handler_TypeE.Get4BIndexDWORD( &pBase[ii] , '4' );
				GetPacketSize = (Func_CRagConnection__GetPacketSize)GetPacketSizeAddr;
				//GetPacketSize(0x0A9);
				__asm push 0x0A9
				__asm call GetPacketSizeAddr
				__asm mov g_temp_ecx,ecx
				p_std_map_packetlen *plen = (p_std_map_packetlen*)g_temp_ecx;

				g_pmodeMgr = (CModeMgr*)UIYourItemWnd__SendMsg_0A9Handler_TypeE
					.GetImmediateDWORD( &pBase[ii], '1' );
				g_CModeMgr__GetGameMode = UIYourItemWnd__SendMsg_0A9Handler_TypeE
					.GetNearJmpAddress( &pBase[ii], '2' );

				kDD_LOGGING2( ("TypeE GetPacketSizeAddr     = %08X ecx = %08X",GetPacketSizeAddr,g_temp_ecx) );
				while(1)
				{
					if( plen->key > 0xffff || (plen->key == 0 && plen->value == 0) ){
						packetLenMap = plen;
						kDD_LOGGING2( ("packetLenMap = %08X",packetLenMap) );
						break;
					}
					plen = plen->parent;
				}
				break;
			}
		}

		// get CMouse instance
		for( UINT ii = 0; ii < mbi.RegionSize - CMouse_Init_vc6.GetSize() ; ii++ )
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;
			if( CMouse_Init_vc6.PatternMatcher( &pBase[ii] )
			 || CMouse_Init_vc9.PatternMatcher( &pBase[ii] )
				)
			{
				ptr_CMouse_Init = (DWORD)( &pBase[ii] );
				kDD_LOGGING2( ("find CMouse::Init = %08X",pBase + ii) );
				break;
			}
		}
		if( ptr_CMouse_Init )
		{
			for( int ii = mbi.RegionSize - winmain_init_CMouse_Init_call.GetSize(); ii >= 0 ; ii-- )
			{
				LPBYTE pBase = (LPBYTE)mbi.BaseAddress;
				if( winmain_init_CMouse_Init_call.PatternMatcher( &pBase[ii] ) )
				{
					kDD_LOGGING2( ("find CMouse::Init call : %08X",pBase + ii) );
					if( winmain_init_CMouse_Init_call.NearJmpAddressMatcher( &pBase[ii],'2',ptr_CMouse_Init ) )
					{
						DWORD ptr_g_mouse = winmain_init_CMouse_Init_call.GetImmediateDWORD(&pBase[ii],'1');
						kDD_LOGGING2( ("find g_mouse = %08X",ptr_g_mouse) );
						g_ROmouse = ptr_g_mouse;
						break;
					}
				}
			}
		}

		// get the address of PlayStream function 
		for( UINT ii = 0; ii < mbi.RegionSize - funcPlayStrem_based_HighPrest_exe.GetSize() ; ii++ )
		{
			LPBYTE pBase = (LPBYTE)mbi.BaseAddress;

			if( funcPlayStrem_based_20140226_155100iRagexe_exe.PatternMatcher( &pBase[ii] )	)
			{
				funcRagexe_PlayStream = (tPlayStream)&pBase[ii];
				DWORD pPlayStream;
				pPlayStream = (DWORD)&pBase[ii];
				kDD_LOGGING2( ("based_20140226_155100iRagexe : %08X",&pBase[ii]) );
				kDD_LOGGING2( ("g_soundMode == %08X",(char*)funcPlayStrem_based_20140226_155100iRagexe_exe.GetImmediateDWORD(&pBase[ii],'1')) );
				kDD_LOGGING2( ("void PlayStream(const char *streamFileName) = %08X",pPlayStream) );
				break;
			}
			if( funcPlayStrem_based_HighPrest_exe.PatternMatcher( &pBase[ii] )	)
			{
				funcRagexe_PlayStream = (tPlayStream)&pBase[ii];
				DWORD pPlayStream;
				pPlayStream = (DWORD)&pBase[ii];
				kDD_LOGGING2( ("based_HighPrest_exe : %08X",&pBase[ii]) );
				kDD_LOGGING2( ("g_soundMode == %08X",(char*)funcPlayStrem_based_HighPrest_exe.GetImmediateDWORD(&pBase[ii],'1')) );
				kDD_LOGGING2( ("void PlayStream(const char *streamFileName) = %08X",pPlayStream) );
				break;
			}
			if( funcPlayStrem_based_RagFree_exe.PatternMatcher( &pBase[ii] )	)
			{
				funcRagexe_PlayStream = (tPlayStream)&pBase[ii];
				DWORD pPlayStream;
				pPlayStream = (DWORD)&pBase[ii];
				kDD_LOGGING2( ("based_RagFree_exe : %08X",&pBase[ii]) );
				kDD_LOGGING2( ("g_soundMode == %08X",(char*)funcPlayStrem_based_RagFree_exe.GetImmediateDWORD(&pBase[ii],'1')) );
				kDD_LOGGING2( ("void PlayStream(const char *streamFileName,int playflag) = %08X",pPlayStream) );
				break;
			}
			if( funcPlayStrem_based_2011111201aRagexe_exe.PatternMatcher( &pBase[ii] )	)
			{
				funcRagexe_PlayStream = (tPlayStream)&pBase[ii];
				DWORD pPlayStream;
				pPlayStream = (DWORD)&pBase[ii];
				kDD_LOGGING2( ("based_2011111201aRagexe_exe : %08X",&pBase[ii]) );
				kDD_LOGGING2( ("g_soundMode == %08X",(char*)funcPlayStrem_based_2011111201aRagexe_exe.GetImmediateDWORD(&pBase[ii],'1')) );
				kDD_LOGGING2( ("void PlayStream(const char *streamFileName,int playflag) = %08X",pPlayStream) );
				break;
			}
		}

		// put packetlengthmap
		if( packetLenMap ){
			int packetnums = GetTreeData( packetLenMap->parent );
			if( packetnums ){
				std::stringstream onelinestr;
				for(int ii = 0;ii < packetnums ;ii++){
					if( (ii % 0x40)==0 ){
						onelinestr << "# 0x" << std::setfill('0') << std::setw(4) << std::hex << ii;
						kDD_LOGGING2(( onelinestr.str().c_str() ));
						onelinestr.str("");
					}
					if( (ii % 0x10)==0 ){
						onelinestr << " ";
					}
					onelinestr << std::setfill(' ') << std::setw(4) << std::dec << g_packetLenMap_table[ii] << ",";
					if( (ii % 0x10)==0x0f ){
						kDD_LOGGING2(( onelinestr.str().c_str() ));
						onelinestr.str("");
					}
				}
				kDD_LOGGING2(( onelinestr.str().c_str() ));
				kDD_LOGGING2(( "" ));
			}
		}

	}
}

