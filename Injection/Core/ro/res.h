#pragma once

class CRes {
public:
	virtual ~CRes(){};

	int m_lockCnt;
	unsigned long m_timeStamp;
	int m_extIndex;
	class CHash m_fName;
};

class CSurface {
public:
	unsigned long m_w;
	unsigned long m_h;
	struct IDirectDrawSurface7* m_pddsSurface;

	CSurface(class CSurface&) {};
	CSurface() {};
	CSurface::CSurface( unsigned long w, unsigned long h, struct IDirectDrawSurface7* surface) {};
	CSurface::CSurface( unsigned long w, unsigned long h) {};
	struct IDirectDrawSurface7* GetDDSurface() {};
	unsigned char CSurface::Create( unsigned long w, unsigned long h) {};
	class CSurface& operator=(class CSurface&) {};

	virtual CSurface::~CSurface() {};
	virtual void CSurface::Update( int x, int y, int width, int height, unsigned long* image, unsigned char blackkey, int lPitch) {};
	virtual void CSurface::ClearSurface( struct tagRECT* rect, unsigned long color) {};
	virtual void CSurface::DrawSurface( int x, int y, int width, int height, unsigned long color) {};
	virtual void CSurface::DrawSurfaceStretch( int x, int y, int width, int height) {};
};


class CTexture : public CSurface  {
public:
	static float m_uOffset;
	static float m_vOffset;

	enum PixelFormat m_pf;
	unsigned char m_blackkey;
	unsigned long m_updateWidth;
	unsigned long m_updateHeight;
	char m_texName[256];
	long m_lockCnt;
	unsigned long m_timeStamp;

	static void __cdecl SetUVOffset(float, float) {};
/*
	void CTexture::UpdateSprite( int x, int y, int width, int height, struct SprImg& img, unsigned long* pal);
	float GetUAdjust();
	float GetVAdjust();
	enum PixelFormat GetPixelFormat();
	long CTexture::Lock();
	long CTexture::Unlock();
	long GetLockCount();
	void UpdateStamp();
	void SetName(char*);
	unsigned char CTexture::CopyTexture( class CTexture* srcTex, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh);
*/
	CTexture(class CTexture&) {};
	CTexture::CTexture( unsigned long w, unsigned long h, enum PixelFormat pf, struct IDirectDrawSurface7* surface) {};
	CTexture::CTexture( unsigned long w, unsigned long h, enum PixelFormat pf) {};
/*
	unsigned char CTexture::Create( unsigned long w, unsigned long h, enum PixelFormat pf);
	unsigned char CTexture::CreateBump( unsigned long w, unsigned long h, struct IDirectDrawSurface7* pSurface);
	unsigned char CTexture::CreateBump( unsigned long w, unsigned long h);
	void CTexture::SetUVAdjust( unsigned long width, unsigned long height);
	void CTexture::UpdateMipmap( struct tagRECT& src);
	class CTexture& operator=(class CTexture&);

	virtual void CTexture::Update( int x, int y, int width, int height, unsigned long* image, unsigned char blackkey, int lPitch);
	virtual void CTexture::ClearSurface( struct tagRECT* rect, unsigned long color);
	virtual void CTexture::DrawSurface( int x, int y, int w, int h, unsigned long color);
	virtual void CTexture::DrawSurfaceStretch( int x, int y, int w, int h);
*/	
	virtual CTexture::~CTexture() {};
};

struct SprImg {
	short width;
	short height;
	short isHalfW;
	short isHalfH;
	CTexture* tex;
	unsigned char* m_8bitImage;
};

class CSprRes : public CRes  {
public:
	unsigned long m_pal[256];
	std::vector<SprImg> m_sprites[2];
	int m_count;

	CSprRes(class CSprRes&) {};
	CSprRes::CSprRes( class Exemplar __formal, char* resid, char* baseDir) {};
	CSprRes::CSprRes() {};
	void Unload() {};
	unsigned char* CSprRes::ZeroCompression(unsigned char* Image, int x, int y, unsigned short& Size) {};
	unsigned char* CSprRes::ZeroDecompression(unsigned char* compressedImage, int width, int height) {};
	unsigned char* CSprRes::HalfImage(unsigned char* Image, int x, int y, int isXHalf, int isYHalf) {};
	class CSprRes& operator=(class CSprRes&) {};

	virtual class CRes* CSprRes::Clone() {};
	virtual unsigned char CSprRes::Load(char* fName) {};
	virtual void CSprRes::Reset() {};
	virtual CSprRes::~CSprRes() {};
};

struct CSprClip {
	int x;
	int y;
	int sprIndex;
	int flags;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	float zoomx;
	float zoomy;
	int angle;
	int clipType;
};
struct CMotion {
	struct tagRECT range1;
	struct tagRECT range2;
	std::vector<CSprClip> sprClips;
	int numClips;
	int m_eventId;
	std::vector<CAttachPointInfo> attachInfo;
	int attachCnt;
};

struct CAction {
	std::vector<CMotion> motions;
	CAction() {};
};
class CActRes : public CRes  {
public:
	std::vector<CAction> actions;
	int numMaxClipPerMotion;
	std::vector<std::basic_string<char>> m_events;
	std::vector<float> m_delay;
};