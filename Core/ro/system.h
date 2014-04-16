#pragma once

//#define JRO_CLIENT_STRUCTURE

struct ACTOR_COLOR {
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};
struct COLOR {
	union{
		unsigned char b,g,r,a;
		unsigned long color;
	};
};

struct UVRECT {
	float u1;
	float v1;
	float u2;
	float v2;
	UVRECT(float _u1, float _v1, float _u2, float _v2)
	{
		u1 = _u1;
		v1 = _v1;
		u2 = _u2;
		v2 = _v2;
	};
	UVRECT() : u1(0),v1(0),u2(0),v2(0) {};
};

struct RENDER_INFO_RECT {
  float left;
  float top;
  float right;
  float bottom;
  float oow;
  void RENDER_INFO_RECT::SetInfo( float fleft, float ftop, float fright, float fbottom, float foow);
  void RENDER_INFO_RECT::Update( struct RPSprite& spr);
};

struct BOXINFO {
	int x;
	int y;
	int cx;
	int cy;
	int drawEdge;
	int color;
	int color2;
};

enum ENUM_DRAGTYPE {
	DT_NODRAG = 0x0,
	DT_FROM_ITEMWND = 0x1,
	DT_FROM_EQUIPWND = 0x2,
	DT_FROM_ITEMSHOPWND = 0x3,
	DT_FROM_ITEMPURCHASEWND = 0x4,
	DT_FROM_ITEMSELLWND = 0x5,
	DT_FROM_ITEMSTOREWND = 0x6,
	DT_FROM_SHORTCUTWND = 0x7,
	DT_FROM_SKILLLISTWND = 0x8,
	DT_FROM_MERCHANTITEMWND = 0x9,
	DT_FROM_MERCHANTSHOPMAKEWND = 0xa,
	DT_FROM_MERCHANTMIRRORITEMWND = 0xb,
	DT_FROM_MERCHANTITEMSHOPWND = 0xc,
	DT_FROM_MERCHANTITEMPURCHASEWND = 0xd,
	DT_FROM_METALPROCESSWND = 0xe,
	DT_FROM_HOSKILLLISTWND = 0xf,
	DT_FROM_MAILWND = 0x10,
	DT_FROM_MERSKILLLISTWND = 0x11,
};

struct DRAG_INFO {
	enum ENUM_DRAGTYPE m_dragType;
	int m_dragItemIndex;
	int m_dragItemPrice;
	int m_dragItemRealPrice;
	int m_numDragItem;
	int m_slotNum;
	int m_dragItemType;
	int m_refiningLevel;
	unsigned char m_isIdentified;
	std::basic_string<char> m_dragSprName;
	std::basic_string<char> m_dragItemName;
	std::basic_string<char> m_skillName;
	int m_skillUseLevel;
	int m_slot[4];

	DRAG_INFO(struct DRAG_INFO&) {};
	DRAG_INFO() {};
	struct DRAG_INFO& operator=(struct DRAG_INFO&) {};
	DRAG_INFO::~DRAG_INFO() {};
};

struct PLAY_WAVE_INFO {
	std::basic_string<char> wavName;
	unsigned long nAID;
	unsigned long term;
	unsigned long endTick;

	PLAY_WAVE_INFO(struct PLAY_WAVE_INFO&) {};
	PLAY_WAVE_INFO() {};
	struct PLAY_WAVE_INFO& PLAY_WAVE_INFO::operator=( struct PLAY_WAVE_INFO& __that) {};
	PLAY_WAVE_INFO::~PLAY_WAVE_INFO() {};
};

enum ENUM_SKILL_USE_TYPE {
	SUT_NOSKILL = 0x0,
	SUT_TO_GROUND = 0x1,
	SUT_TO_CHARACTER = 0x2,
	SUT_TO_ITEM = 0x3,
	SUT_TO_ALL = 0x4,
	SUT_TO_SKILL = 0x5,
	SUT_TO_SKILLGROUND_WITHTALKBOX = 0x6,
};

struct SKILL_USE_INFO {
	enum ENUM_SKILL_USE_TYPE m_skillUseType;
	int m_skillId;
	int m_attackRange;
	int m_useLevel;
};

struct SHOW_IMAGE_INFO {
	int type;
	std::basic_string<char> imageName;

	SHOW_IMAGE_INFO(struct SHOW_IMAGE_INFO&) {};
	SHOW_IMAGE_INFO() {};
	struct SHOW_IMAGE_INFO& operator=(struct SHOW_IMAGE_INFO&) {};
	SHOW_IMAGE_INFO::~SHOW_IMAGE_INFO() {};
};

struct POINTER_FUNC {
	int excuteType;
	void  (*pfunc)();
	long startTime;
};

class CScheduler {
public:
	std::multimap<long,POINTER_FUNC> m_scheduleList;
	CScheduler(class CScheduler&) {};
	CScheduler::CScheduler() {};
	void CScheduler::OnRun() {};
	unsigned char CScheduler::InsertInList(long excuteTime, int excuteType, void  (*pfunc)()) {};
	unsigned char CScheduler::InsertInList(long excuteTime, struct POINTER_FUNC pfunc) {};
	class CScheduler& operator=(class CScheduler&) {};

	virtual CScheduler::~CScheduler() {};
};

class Exemplar {
public:
	Exemplar() {};
};

class CHash {
public:
	unsigned long m_HashCode;
	char m_String[252];
};


enum PixelFormat {
	PF_A1R5G5B5 = 0x0,
	PF_A4R4G4B4 = 0x1,
	PF_R5G6B5 = 0x2,
	PF_R5G5B5 = 0x3,
	PF_A8R8G8B8 = 0x4,
	PF_BUMP = 0x5,
	PF_LAST = 0x6,
	PF_UNSUPPORTED = 0xff,
};

enum PROCEEDTYPE {
	PT_NOTHING = 0x0,
	PT_ATTACK = 0x1,
	PT_PICKUPITEM = 0x2,
	PT_SKILL = 0x3,
	PT_GROUNDSKILL = 0x4,
	PT_ATTACK_2 = 0x5,
	PT_TOUCH_SKILL = 0x6,
};
