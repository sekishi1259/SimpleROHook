#pragma once

struct RPTriFace {
	struct tlvertex3d verts[3];
	CTexture* tex;
	unsigned long srcAlphaMode;
	unsigned long destAlphaMode;

	RPTriFace() {};
};

struct objectSegment {
	struct vector3d pos;
	struct vector3d deltaPos;
	float longitude;
	float latitude;
	struct matrix mat;

	objectSegment() {};
	struct objectSegment& operator=(struct objectSegment&) {};
};

struct KAC_XFORMDATA {
	float x;
	float y;
	float u;
	float v;
	float us;
	float vs;
	float u2;
	float v2;
	float us2;
	float vs2;
	float ax[4];
	float ay[4];
	float aniframe;
	unsigned long anitype;
	float anidelta;
	float rz;
	float crR;
	float crG;
	float crB;
	float crA;
	unsigned long srcalpha;
	unsigned long destalpha;
	unsigned long mtpreset;
};

struct KAC_KEYFRAME {
	int iFrame;
	unsigned long dwType;
	struct KAC_XFORMDATA XformData;
};

struct KAC_LAYER {
	int cTex;
	int iCurAniFrame;
	class CTexture* m_tex[110];
	char* m_texName[110];
	int cAniKey;
	struct KAC_KEYFRAME* aAniKey;
	KAC_LAYER::KAC_LAYER() {};
	KAC_LAYER::~KAC_LAYER() {};

	CTexture* KAC_LAYER::GetTexture(int iTex) {};
};

struct KANICLIP {
	int nFPS;
	int cFrame;
	int cLayer;
	int cEndLayer;
	struct KAC_LAYER aLayer[60];

	KANICLIP() {};
	KANICLIP::~KANICLIP() {};
};


class CGameObject {
public:
	CGameObject(class CGameObject&) {};
	CGameObject::CGameObject() {};
	void CGameObject::OnInit() {};
	void CGameObject::OnExit() {};
	int  CGameObject::Get8Dir2(float rot, int isReal8Dir) {};
	int  CGameObject::GetDir(float rot) {};
	class CGameObject& operator=(class CGameObject&) {};

	virtual CGameObject::~CGameObject() {};
	virtual unsigned char CGameObject::OnProcess() { return 0;};
	virtual void CGameObject::SendMsg( class CGameObject* sender, int message, int arg1, int arg2, int arg3) {};
	virtual void CGameObject::Render( struct matrix& ptm) {};
	virtual int CGameObject::Get8Dir( float rot) { return 0; };
};


class CRenderObject : public CGameObject  {
public:
	struct vector3d m_pos;
	std::basic_string<char> m_bodyPaletteName;	// class std::basic_string<char,std::char_traits<char>,std::allocator<char> > m_bodyPaletteName;
	int m_baseAction;
	int m_curAction;
	int m_curMotion;
	int m_oldBaseAction;
	int m_oldMotion;
	int m_bodyPalette;
	float m_roty; // body rot
	float m_zoom;
	float m_shadowZoom;
	float m_motionSpeed;
	float m_lastPixelRatio;
	float m_loopCountOfmotionFinish;
	float m_modifyFactorOfmotionSpeed;
	float m_modifyFactorOfmotionSpeed2;
	int m_motionType;
	int m_stateId;
	int m_oldstateId;
	int m_sprShift;
	int m_sprAngle;
	int m_offsetOow;
	unsigned long m_colorOfSingleColor;
	unsigned long m_singleColorStartTick;
	unsigned long m_stateStartTick;
	struct ACTOR_COLOR m_oldColor;
	struct ACTOR_COLOR m_curColor;
	union {
		unsigned long m_sprArgb;
		struct COLOR m_sprColor; // deleted for after HightPrieast.exe?
	};
	unsigned char m_isLieOnGround;
	unsigned char m_isMotionFinished;
	unsigned char m_isMotionFreezed;
	unsigned char m_isSingleColor;
	unsigned char m_isVisible;
	unsigned char m_isVisibleBody;
	unsigned char m_alwaysTopLayer;
	unsigned char m_isSprArgbFixed;
	unsigned char m_shadowOn;
	unsigned char m_shouldAddPickInfo;
	int m_isPc;
	int m_lastTlvertX;
	int m_lastTlvertY;
	unsigned char m_forceAct;
	unsigned char m_forceMot;
	unsigned char m_forceAct2[5];
	unsigned char m_forceMot2[5];
	unsigned char m_forceMaxMot;
	unsigned char m_forceAnimSpeed;
	unsigned char m_forceFinishedAct;
	unsigned char m_forceFinishedMot;
	unsigned char m_forceStartMot;
	int m_isForceState;
	int m_isForceAnimLoop;
	int m_isForceAnimation;
	int m_isForceAnimFinish;
	int m_isForceState2;
	int m_isForceState3;
	unsigned long m_forceStateCnt;
	unsigned long m_forceStateEndTick;
	int m_BodyLight;
	char m_BeZero;
	char m_BodyFlag;
	unsigned char m_BodySin;
	unsigned char m_BodySin2;
	unsigned char m_BodySin3;
	unsigned char m_BodySin4;
	unsigned char m_BodySin5;
	short m_BodyTime;
	unsigned char m_BodyTime2;
	unsigned char m_BodyTime3;
	short m_FlyMove;
	unsigned char m_FlyNow;
	char m_camp;
	short m_charfont;
	unsigned char m_BodyAni;
	unsigned char m_BodyAct;
	unsigned char m_BodyAniFrame;
	class CSprRes* m_sprRes;
	class CActRes* m_actRes;

	void* dummy01; // added for later HightPrieast.exe?
#ifdef JRO_CLIENT_STRUCTURE
	void* dummy02; // added for later HightPrieast.exe?
#endif
	CRenderObject(class CRenderObject&) {};
	CRenderObject::CRenderObject() {};
/*
	void CRenderObject::OnInit();
	void CRenderObject::OnExit();
	void CRenderObject::UpdateTlvertex struct matrix& vtm);
	void CRenderObject::RenderBody struct matrix& vtm, char FLAG1);
	void CRenderObject::RenderMercenary struct matrix& vtm, int MerJob);
	void CRenderObject::RenderShadow struct matrix& vtm, float zoom);
	void CRenderObject::RenderSprite struct RPSprite& spr, unsigned long* pal, int renderFlag);
	float CRenderObject::CalcDir float sx, float sz, float x, float z);
	void CRenderObject::CalcDir float x, float z);
	void CRenderObject::ModifyArgb int a, int r, int g, int b);
	void CRenderObject::SetArgb int a, int r, int g, int b);
	void CRenderObject::ProcessSound();
	void CRenderObject::SetLmIntensity float& lmRIntensity, float& lmGIntensity, float& lmBIntensity);
	float CRenderObject::CalcDist float x, float y, float z);
	float CRenderObject::CalcDist float x, float z);
	float GetStateCnt();
	float GetLastPixelRatio();
	unsigned char CRenderObject::GetTargetPos unsigned long targetGid, struct vector3d& tPos);
	int GetLastTlvertX();
	int GetLastTlvertY();
	void CRenderObject::SetForceAnimSpeed(unsigned char speed);
	void CRenderObject::SetForceAnimEndTick unsigned long tick);
	void CRenderObject::SetForceAnimation int act, int mot, int maxMot, int finishAct, int finishMot, int isAnim, unsigned long endTick, int isLoop);
	void CRenderObject::SetForceAnimation2 int act, int mot, int act2, int mot2, int act3, int mot3, int act4, int mot4, int act5, int mot5, int isAnim, unsigned long endTick, int isLoop);
	void CRenderObject::SetForceAnimation3 int act, int mot, int maxMot, int isAnim, unsigned long endTick, int isLoop);
	void CRenderObject::SetSprName char* sprName);
	char* CRenderObject::GetSprName();
	class CSprRes* CRenderObject::GetSprRes();
	void CRenderObject::Palette_BW class CPaletteRes* palRes);
	void CRenderObject::SetActName char* actName);
	void CRenderObject::SetCharacterSquare float* top, float* btm, float* left, float* right);
	void CRenderObject::SetCharacterAngle float* angle);
	void CRenderObject::SetCharacterPreAngle int layer, int* imfOffX, int* imfOffY, int* head_imfOffX, int* head_imfOffY);
	void CRenderObject::SetCharacterLight( struct RPSprite spr, unsigned long* pal);
	void CRenderObject::SetCharacterPixelRatio float* pixelratio);
	short CRenderObject::SetCharacterFlying();
	class CActRes* CRenderObject::SetCharacterActRes int m_accessory2, int layer);
	class CSprRes* CRenderObject::SetCharacterSprRes int m_accessory2, int layer);
	char* CRenderObject::GetActName();
	class CActRes* CRenderObject::GetActRes();
	class CRenderObject& operator=(class CRenderObject&);
*/
	virtual CRenderObject::~CRenderObject() {};
/*
	virtual unsigned char CRenderObject::OnProcess();
	virtual void CRenderObject::Render struct matrix& vtm);
	virtual void CRenderObject::SetRenderInfo struct RENDER_INFO_RECT& info, float tlvertX, float tlvertY);
	virtual void CRenderObject::SetTlvert float tlvertX, float tlvertY);
	virtual void CRenderObject::SendMsg class CGameObject* sender, int message, int arg1, int arg2, int arg3);
	virtual void CRenderObject::SetAction int action, int speed, int type);
	virtual void CRenderObject::ProcessMotion();
*/
};

class CMousePointer : public CRenderObject  {
public:
	int m_visibleTime;

	CMousePointer(class CMousePointer&) {};
	CMousePointer::CMousePointer() {};
	void CMousePointer::OnInit() {};
	void CMousePointer::OnExit() {};
	class CMousePointer& operator=(class CMousePointer&) {};

	virtual CMousePointer::~CMousePointer() {};
	virtual void CMousePointer::SendMsg( class CGameObject* sender, int message, int arg1, int arg2, int arg3) {};
	virtual unsigned char CMousePointer::OnProcess() {};
};

class CItem : public CRenderObject  {
public:
	class std::basic_string<char> m_itemName;
	short m_itemid;
	short m_unknownid;
	int m_itemid2;
	unsigned long m_aid;
	int m_isJumping;
	float m_sfallingSpeed;
	float m_sPosY;

	CItem(class CItem&);
	CItem::CItem();
	void CItem::OnInit();
	void CItem::OnExit();
	unsigned long GetAID();
	class CItem& operator=(class CItem&);

	virtual CItem::~CItem();
	virtual unsigned char CItem::OnProcess();
	virtual void CItem::Render( struct matrix& vtm);
	virtual void CItem::SendMsg( class CGameObject* sender, int message, int arg1, int arg2, int arg3);
	virtual void CItem::SetRenderInfo( struct RENDER_INFO_RECT& info, float tlvertX, float tlvertY);
};

class CEffectPrim : public CRenderObject  {
public:
	int m_isChangeProcessed;
	class CRagEffect* m_master;
	class CTexture** m_texture;
	char* m_playWaveName;
	int m_isHit;
	int m_chSpeed;
	int m_AlphaShow;
	int m_chRadiSpeed;
	int m_isDisappear;
	struct matrix m_matrix;
	struct matrix m_rotMat;
	unsigned long m_argb;
	unsigned long m_renderFlag;
	int m_any;
	int m_type;
	int m_count;
	int m_chCnt;
	int m_pattern;
	int m_stateCnt;
	int m_duration;
	int m_animSpeed;
	int m_fadeInCnt;
	int m_fadeOutCnt;
	int m_numSegment;
	int m_validAngle;
	int m_RGBCycleCnt;
	int m_ChangePoint;
	int m_totalTexture;
	int m_playWaveType;
	int m_notifyCnt[10];
	int m_RGBCycleDelay;
	int m_commonPattern;
	int m_red;
	int m_green;
	int m_blue;
	int m_redSpeed;
	int m_greenSpeed;
	int m_blueSpeed;
	unsigned char m_repeatAnim;
	std::vector<float> m_chPoint;
	std::vector<float> m_chVal1;
	std::vector<float> m_chVal2;
	std::vector<float> m_chVal3;
	float m_arcAngle;
	float m_minRadius;
	float m_rotAxis[3];
	float m_ChangeSpeed;
	float m_ChangeAccel;
	float m_ChangeSpeedx;
	float m_ChangeSpeedy;
	float m_ChangeSpeedz;
	float m_ChangeAccelx;
	float m_ChangeAccely;
	float m_ChangeAccelz;
	float m_speed;
	float m_accel;
	float m_ChangeWidthAccel;
	float m_ChangeWidthSpeed;
	float m_ChangeHeightSpeed;
	float m_ChangeHeightAccel;
	float m_maxAlpha;
	float m_minAlpha;
	float m_u1;
	float m_u2;
	float m_v1;
	float m_v2;
	float m_gravSpeed;
	float m_gravAccel;
	float m_innerSize;
	float m_outerSize;
	float m_innerSpeed;
	float m_outerSpeed;
	float m_innerAccel;
	float m_outerAccel;
	float m_longitude;
	float m_latitude;
	float m_roll;
	float m_size;
	float m_sizeSpeed;
	float m_sizeAccel;
	float m_alpha;
	float m_alphaSpeed;
	float m_alphaDelta;
	float m_longSpeed;
	float m_latiSpeed;
	float m_rollSpeed;
	float m_longAccel;
	float m_latiAccel;
	float m_rollAccel;
	float m_radius;
	float m_radiusSpeed;
	float m_radiusAccel;
	float m_widthSize;
	float m_widthSpeed;
	float m_widthAccel;
	float m_heightSize;
	float m_heightSpeed;
	float m_heightAccel;
	float m_maxHeightSize;
	struct vector3d m_speed3d;
	struct vector3d m_orgPos;
	struct vector3d m_deltaPos;
	struct vector3d m_deltaPos2;
	struct vector3d m_deltaPos3;
	struct vector3d m_deltaPosAccel;
	struct vector3d m_segPos[4];
	struct TeiEffect m_GI[4];
	struct PrimSegment m_segment[50];
	struct RPTriFace m_rp[512];

	virtual ~CEffectPrim() {};
};

class CRagEffect : public CRenderObject  {
public:
	class CRenderObject* m_master;
	float m_param[4];
	float m_longitude;
	float m_latitude;
	float m_tlvertX;
	float m_tlvertY;
	float m_tlvertZ;
	int m_defZ;
	int m_isDirection;
	int m_type;
	int m_level;
	int m_flag;
	int m_count;
	int m_stateCnt;
	int m_duration;
	struct vector3d m_deltaPos;
	struct vector3d m_deltaPos2;
	struct vector3d m_deltaPos3;
	class std::list<CEffectPrim*> m_primList;
	struct objectSegment m_objSeg[20];
	class CEZeffectRes* m_ezEffectRes;
	struct KANICLIP* m_aniClips;
	struct KAC_XFORMDATA m_actXformData[128];
	struct RPTriFace m_rp[512];
	int m_iCurLayer;
	int m_cEndLayer;
	unsigned char m_isLayerDrawn[128];
	int m_aiCurAniKey[128];

	virtual ~CRagEffect() {};
};

class CAbleToMakeEffect : public CRenderObject  {
public:
	int m_efId;
	int m_Sk_Level;
	int m_isLoop;
	std::list<CRagEffect*> m_effectList;
	CRagEffect* m_beginSpellEffect;
	CRagEffect* m_magicTargetEffect;

	virtual ~CAbleToMakeEffect() {};
};

struct WBA {
	int message;
	int damage;
	unsigned long time;
	unsigned long targetGid;
	int tCellX;
	int tCellY;
	int effectId1;
	int effectId2;
	int effectLevel;
	float attackedSpeedFactor;
	WBA(int, int, unsigned long, unsigned long, int, int, int, int, int, float) {};
};

class CMsgEffect : public CAbleToMakeEffect  {
public:
	class CGameActor* m_masterActor;
	struct vector3d m_destPos;
	struct vector3d m_targetPos;
	struct vector3d m_destPos2;
	unsigned long m_masterGid;
	int m_isDisappear;
	float m_dist;
	float m_orgPosY;
	float m_orgPosX;
	float m_orgZoom;
	float m_moveDist;
	int m_stateCnt;
	int m_oldAction;
	int m_msgEffectType;
	int m_orgSprShift;
	int m_isAttackTarget;
	int m_isAnimation;

	virtual ~CMsgEffect() {};
};

class CGameActor : public CAbleToMakeEffect  {
public:
	int m_moveDestX;
	int m_moveDestY;
	unsigned long m_speed;
	int m_isCounter;
	int m_isTrickDead;
	int m_isPlayHitWave;
	int m_isAsuraAttack;
	char* m_emblemWnd;
	char* m_WordDisplayWnd;
	char m_hitWaveName[128];
	unsigned long m_colorEndTick;
	short m_clevel;
#if 1 // changed for later HightPrieast.exe?
	short m_gactpad0; // added for later HightPrieast.exe?
#else
	short m_MaxHp;
	short m_Hp;
	short m_MaxSp;
	short m_Sp;
	int m_Exp;
	short m_Str;
	short m_Int;
	short m_Dex;
	short m_Vit;
	short m_Luk;
	short m_Agi;
#endif
	struct vector3d m_accel;
	class CPathInfo m_path;
	unsigned long m_moveStartTime;
	unsigned char m_isNeverAnimation;
	int m_pathStartCell;
#ifndef JRO_CLIENT_STRUCTURE
	float m_dist;
#endif
	unsigned long m_lastProcessStateTime;
	unsigned long m_lastServerTime;
	unsigned long m_chatTick;
	unsigned long m_targetGid;
	float m_attackMotion;
	int m_isBladeStop;
	unsigned long m_gid;
	int m_job;
	int m_sex;
	void* m_balloon;//class UIBalloonText* m_balloon;
	void* m_chatTitle;//class UIChatRoomTitle* m_chatTitle;
	void* m_merchantShopTitle;//class UIMerchantShopTitle* m_merchantShopTitle;
	void* m_skillRechargeGage;//class UIRechargeGage* m_skillRechargeGage;

	void* m_unknownobj01; // added for later HightPrieast.exe?

	unsigned long m_freezeEndTick;
	unsigned long m_petEmotionStartTick;
	unsigned long m_skillRechargeEndTick;
	unsigned long m_skillRechargeStartTick;
	int m_chatWidth;
	int m_chatHeight;
	int m_nameWidth;
	int m_xSize;
	int m_ySize;
	int m_headType;
	std::list<WBA> m_willBeAttackList;
	std::list<WBA> m_willBeAttackedList;

	int m_gactpad1[4]; // added for later HightPrieast.exe?

	int m_willBeDead;
	int m_is99;
	char m_99;
	int m_bodyState;
	int m_effectState;
	int m_healthState;
	int m_pkState;
	float m_damageDestX;
	float m_damageDestZ;
	unsigned long m_effectLaunchCnt;
	unsigned long m_vanishTime;
	int m_actorType;
	int m_bIsMemberAndVisible;
	int m_gdid;
	int m_emblemVersion;
	void* m_homunAI;//class CMercenaryAI* m_homunAI;
	void* m_merAI;//class CMercenaryAI* m_merAI;
	unsigned char m_objectType;
	struct _MSG2AI m_homunMsg;
	struct _MSG2AI m_homunResMsg;
	struct _MSG2AI m_merMsg;
	struct _MSG2AI m_merResMsg;


	class CMsgEffect* m_birdEffect;
	class CMsgEffect* m_cartEffect;
	class CMsgEffect* m_pkEffect;
	class CMsgEffect* m_iceEffect;
	class CMsgEffect* m_curseEffect;
	class CMsgEffect* m_sleepEffect;
	class CMsgEffect* m_stunEffect;
	class CMsgEffect* m_silenceEffect;
	class CMsgEffect* m_angelusEffect;
	class CMsgEffect* m_crusisEffect;
	class CMsgEffect* m_chatProhibitEffect;
	class CMsgEffect* m_cursorEffect;
	std::list<CMsgEffect*> m_msgEffectList;
//	int m_gactpad2[9];
	int m_gactpad2[4];
	virtual ~CGameActor() {};
};

class CSkill : public CGameActor  {
public:
	long m_cskillpad;//

	unsigned long m_launchCnt;
	unsigned long m_SkillStartTime;
	unsigned long m_aid;
//	class C3dActor* m_3dactor;
//	class CRagEffect* m_LoopEffect;
	long m_framecounter; // ?? added for later HightPrieast.exe?
	int m_effectId;

	virtual ~CSkill() {};
};

class CPc : public CGameActor  {
public:
	int m_pcppad[5];//

	std::basic_string<char> m_imfName;
	int m_honor;
	int m_virtue;
	int m_headDir;
	int m_head;
	int m_headPalette;
	int m_weapon;
	int m_accessory;
	int m_accessory2;
	int m_accessory3;
	int m_shield;

//	int m_shoe; // deleted for later HightPrieast.exe?
//	int m_shoe_count; // deleted for later HightPrieast.exe?
//	struct vector3d shoe_pos; // deleted for later HightPrieast.exe?
	int m_renderWithoutLayer;
//	std::basic_string<char> m_headPaletteName;  // deleted for later HightPrieast.exe?
	void* m_gage;//UIPcGage* m_gage;
//	long m_pk_rank;  // deleted for later HightPrieast.exe?
//	long m_pk_total; // deleted for later HightPrieast.exe?

	std::vector<CSprRes*> m_sprResList;
	std::vector<CActRes*> m_actResList;

	long m_cpcpad1;// 0x101?? added for later HightPrieast.exe?
	long m_cpcpad2;// 0x101??
	long m_cpcpad3;// 0x101??
	long m_cpcpad4;// 0x101??
	long m_cpcpad5;// 0x101??
	long m_cpcpad6;// 0x101??
	long m_cpcpad7;// 0x101??
	long m_cpcpad8;// 0x101??

	virtual ~CPc() {};
};

class CPlayer : public CPc  {
public:
	int m_destCellX;
	int m_destCellZ;
	unsigned long m_attackReqTime;
	unsigned long m_preMoveStartTick;
	enum PROCEEDTYPE m_proceedType;
	int m_preMoveOn;
	int m_attackMode;
	int m_isAttackRequest;
	int m_isWaitingMoveAck;
	int m_isPreengageStateOfMove;
	unsigned long m_proceedTargetGid;
	unsigned long m_totalAttackReqCnt;
	unsigned long m_tickOfMoveForAttack;
	unsigned long m_moveReqTick;
	unsigned long m_standTick;
	int m_skillId;
	int m_skillAttackRange;
	int m_skillUseLevel;
	int m_gSkillDx;
	int m_gSkillDy;
	int m_preengageXOfMove;
	int m_preengageYOfMove;
	CRagEffect* m_statusEffect;

	virtual~CPlayer() {};
};
