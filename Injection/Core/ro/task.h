#pragma once

class CMode {
public:
	int m_subMode;    // pointer
	int m_subModeCnt;
	int m_nextSubMode;// pointer +0x08
	int m_fadeInCount;
	int m_loopCond;
	int m_isConnected;
	void* m_helpBalloon;//UITransBalloonText* m_helpBalloon; +0x18
	unsigned long m_helpBalloonTick;
	unsigned long m_mouseAnimStartTick;
	int m_isMouseLockOn;
	int m_screenShotNow;
	struct vector2d m_mouseSnapDiff; // Ç±Ç±Ç‹Ç≈àÍèè 0xd0 or 0xb4
	//
	char m_pad1[28];
	int m_cursorActNum; 
	int m_cursorMotNum; 

	CMode(class CMode&) {};
	CMode::CMode() {};
	void CMode::Initialize() {};
	void PostQuitMsg() {};
	void CMode::ScreenShot() {};
	void CMode::SetCursorAction( int cursorActNum) {};
	void CMode::RunFadeOut( int bDrawLogo) {};
	void CMode::DrawSprite( int x, int y, class CActRes* act, class CSprRes* spr, int actId, int motId, float zoom, float angle, unsigned long color) {};
	void CMode::DrawTexture( struct tagRECT* rect, int angle, float oow, unsigned long argb, class CTexture* tex, struct UVRECT uvRect) {};
	void CMode::DrawBackGround2D( struct tagRECT* rect, int angle, float oow, unsigned long argb, class CTexture* tex, struct UVRECT uvRect) {};
	void CMode::DrawMouseCursor() {};
	void CMode::DrawBattleMode() {};
	void CMode::MakeHelpBalloon( char* helpMsg, int x, int y, unsigned long fntColor) {};
	void CMode::ProcessHelpBalloon() {};
	void CMode::ProcessFadeIn() {};
	void CMode::ProcessKeyBoard() {};
	int GetCursorAction() {};
	int GetCursorMotion() {};
	int GetSubMode() {};
	int GetLoopCond() {};
	void SetNextSubState(int) {};
	void SetSubState(int) {};
	class CMode& operator=(class CMode&) {};

	virtual CMode::~CMode() {};
	virtual int CMode::OnRun() {};
	virtual void CMode::OnInit( char* modeName) {};
	virtual void CMode::OnExit() {};
	virtual void CMode::OnUpdate() {};
	virtual int CMode::SendMsg( int messageId, int val1, int val2, int val3) {};
	virtual void CMode::OnChangeState( int state) {};

};


class CGameMode : public CMode {
public:
	static unsigned long m_lastLockOnPcGid;
	static unsigned long m_dwOldAutoFollowTime;
	static unsigned long m_dwOldDisappearTime;

	int m_areaLeft;
	int m_areaRight;
	int m_areaTop;
	int m_areaBottom;
	char m_rswName[40];
	char m_minimapBmpName[60];
	CWorld* m_world; // d0
	CView* m_view;
	CMousePointer* m_mousePointer;
	unsigned long m_leftBtnClickTick;
	int m_oldMouseX;
	int m_oldMouseY;
	int m_rBtnClickX;
	int m_rBtnClickY;
	unsigned long m_lastPcGid;
	unsigned long m_lastMonGid;
	unsigned long m_lastLockOnMonGid;
	int m_isAutoMoveClickOn;
	int m_isWaitingWhisperSetting;
	int m_isWaitingEnterRoom;
	int m_isWaitingAddExchangeItem;
	unsigned long m_waitingWearEquipAck;
	unsigned long m_waitingTakeoffEquipAck;
	int m_isReqUpgradeSkillLevel;
	int m_exchangeItemCnt;
	int m_isWaitingCancelExchangeItem;
	std::basic_string<char> m_refuseWhisperName;
	std::basic_string<char> m_streamFileName;
	std::basic_string<char> m_lastExchangeCharacterName;
	std::map<unsigned long,NamePair> m_actorNameList;
	std::map<unsigned long,unsigned long> m_actorNameReqTimer;
	std::map<unsigned long,NamePair> m_actorNameListByGID;
	std::map<unsigned long,unsigned long> m_actorNameByGIDReqTimer;
	std::map<unsigned long,int> m_guildMemberStatusCache;
	std::map<unsigned long,CellPos> m_actorPosList;
	std::list<unsigned long> m_pickupReqItemNaidList;
	std::map<unsigned long,unsigned long> m_aidList;
	std::map<unsigned long,ColorCellPos> m_partyPosList;
	std::map<unsigned long,ColorCellPos> m_guildPosList;
	std::map<unsigned long,ColorCellPos2> m_compassPosList;
	std::vector<int,std::allocator<int> > m_menuIdList;
	std::list<unsigned long> m_visibleTrapList;
	std::list<unsigned long> m_emblemReqGdidQueue;
	unsigned long m_lastEmblemReqTick;
	unsigned long m_lastNameWaitingListTick;
	std::vector<PLAY_WAVE_INFO> m_playWaveList;
	std::vector<unsigned long> m_KillerList;
	std::basic_string<char> m_lastWhisperMenuCharacterName;
	std::basic_string<char> m_lastWhisper;
	std::basic_string<char> m_lastWhisperName;
	int m_noMove;
	unsigned long m_noMoveStartTick;
	int m_isOnQuest;
	int m_isPlayerDead;
	int m_canRotateView;
	int m_hasViewPoint;
	short ViewPointData[9];
	int m_receiveSyneRequestTime;
	unsigned long m_syncRequestTime;
	unsigned long m_usedCachesUnloadTick;
	unsigned long m_reqEmotionTick;
	unsigned long m_reqTickChatRoom;
	unsigned long m_reqTickMerchantShop;
	int m_isReqEmotion;
	float m_fixedLongitude;
	unsigned long m_lastCouplePacketAid;
	unsigned long m_lastCouplePacketGid;
	char m_CoupleName[24];
	void* m_nameBalloon;			// class UINameBalloonText* m_nameBalloon;
	void* m_targetNameBalloon;		// class UINameBalloonText* m_targetNameBalloon;
	void* m_broadcastBalloon;		// class UITransBalloonText* m_broadcastBalloon;
	void* m_playerGage;				// class UIPlayerGage* m_playerGage;
	void* m_skillNameBalloon;		// class UITransBalloonText* m_skillNameBalloon;
	void* m_skillMsgBalloon;		// class UITransBalloonText* m_skillMsgBalloon;
	void* m_skillUsedMsgBalloon;	// class UITransBalloonText* m_skillUsedMsgBalloon;
	unsigned long m_skillUsedTick;
	unsigned long m_broadCastTick;
	int m_nameDisplayed;
	int m_nameDisplayed2;
	int m_waitingUseItemAck;
	int m_waitingItemThrowAck;
	int m_waitingReqStatusAck;
	unsigned long m_nameActorAid;
	unsigned long m_lastNaid;
	unsigned long m_menuTargetAID;
	int m_nameBalloonWidth;
	int m_nameBalloonHeight;
	int m_dragType;
	struct DRAG_INFO m_dragInfo;
	struct ChatRoomInfo m_lastChatroomInfo;
	struct SKILL_USE_INFO m_skillUseInfo;
	struct SHOW_IMAGE_INFO m_showImageInfo;
	std::basic_string<char> m_lastChat;
	int m_sameChatRepeatCnt;
	int m_numNotifyTime;
	int m_isCheckGndAlpha;
	int m_lastCardItemIndex;
	int m_SkillBallonSkillId;
	unsigned long m_nameBalloonType;
	unsigned long m_showTimeStartTick;
	int m_recordChatNum;
	std::basic_string<char> m_recordChat[11];
	unsigned long m_recordChatTime[11];
	int m_strikeNum;
	unsigned long m_strikeTime[3];
	unsigned long m_doritime[6];
	int m_isCtrlLock;
	int m_isUseYgdrasil;
	int m_isMakeYgdrasil;
	int m_autoSaveChatCnt;
	CellPos m_posOfBossMon;
	unsigned char m_isBossAlarm;
	unsigned char m_onCopyName;
	class CScheduler m_scheduler;
	unsigned char m_SiyeonStatus[6];
	int m_MouseRollCount;
	int m_MouseRollMax;

	virtual ~CGameMode() {};
};

class CModeMgr {
public:
	int m_loopCond;
	class CMode* m_curMode;
	char m_curModeName[40];
	char m_nextModeName[40];
	int m_curModeType;
	int m_nextModeType;

	CModeMgr::CModeMgr() {};
	void CModeMgr::Run( int modeType, char* worldName) {};
	void CModeMgr::Switch( int modeType, char* modeName) {};
	void CModeMgr::Quit() {};
	int GetLoopCond() {};
	CMode* GetCurMode() {};
	CGameMode* CModeMgr::GetGameMode() {};
	void* CModeMgr::GetLoginMode() {}; // CLoginMode* CModeMgr::GetLoginMode();
};
