#pragma once

struct CLightmap {
	unsigned char brightObj[3];
	CTexture* surface;
	struct texCoor coor[4];
	struct COLOR intensity[4];
};


struct CAttrCell {
	float h1;
	float h2;
	float h3;
	float h4;
	int flag;
};


class C3dAttr : public CRes {
public:
	int m_width;
	int m_height;
	int m_zoom;

	std::vector<CAttrCell> m_cells;

	C3dAttr(class C3dAttr&) {};
	C3dAttr::C3dAttr(class Exemplar __formal, char* resid, char* baseDir) {};
	C3dAttr::C3dAttr() {};
	void Create(int w, int h)
	{
		m_cells.reserve( w * h );
	}
	float C3dAttr::GetHeight( float x, float z)	{};

	int C3dAttr::GetAttr( float x, float z){
		long cx,cy;
		ConvertToCellCoor(x,z,cx,cy);
		return m_cells[ cx + cy * m_width ].flag;
	};

	struct CAttrCell* GetCell(int Cellx, int Celly)
	{
		return &m_cells[ Cellx + Celly * m_width ];
	};
	float C3dAttr::RayTest( struct ray3d& ray, int x, int y) {};

	void C3dAttr::SetCellInfo( int Type, int Cellx, int Celly)
	{
		m_cells[ Cellx + Celly * m_width ].flag = Type;
	};

	void C3dAttr::GetWorldPos( float attrX, float attrY, struct vector2d& pos)
	{
		pos.x = ((attrX - m_width / 2) * (float)m_zoom) + 2.5f;
		pos.y = ((attrY - m_height / 2) * (float)m_zoom) + 2.5f;
	};
	void ConvertToCellCoor(float x, float z, long& cx, long& cy)
	{
		cx = (long)( x / m_zoom + m_width /2 );
		cy = (long)( z / m_zoom + m_height /2 );
	};
	void C3dAttr::GetHeightMinMax( struct tagRECT& area, float& topHeight, float& bottomHeight) {};
	class C3dAttr& operator=(class C3dAttr&) {};

	virtual class CRes* C3dAttr::Clone() {};
	virtual unsigned char C3dAttr::Load( char* fName) {};
	virtual void C3dAttr::Reset() {};
	virtual C3dAttr::~C3dAttr() {};
};


class CLightmapMgr {
public:
	std::vector<CLightmap> m_lightmaps;
	std::vector<CTexture*> m_lmSurfaces;
	int m_numLightmaps;
	int m_numLmSurfaces;

	CLightmapMgr(class CLightmapMgr&) {};
	CLightmapMgr::CLightmapMgr() {};
	CLightmapMgr::~CLightmapMgr() {};
	void CLightmapMgr::Create( class CGndRes* gnd) {};
	void CLightmapMgr::Reset() {};
	class CLightmapMgr& operator=(class CLightmapMgr&) {};
};

class C3dGround {
public:
	class C3dAttr* m_attr;
	int m_width;
	int m_height;
	float m_zoom;
	CLightmapMgr m_lightmapMgr;
	int m_numSurfaces;
	float m_waterLevel;
	int m_texAnimCycle;
	int m_wavePitch;
	int m_waveSpeed;
	int m_waterSet;
	float m_waveHeight;
	CTexture* m_waterTex;
	CTexture* m_pBumpMap;
	int m_waterCnt;
	int m_waterOffset;
	int m_isNewVer;

	virtual ~C3dGround() {};
};


class C3dNode {
	struct ColorInfo {
		unsigned long color[3];
		struct COLOR argb[3];
		struct vector2d uv[3];
		ColorInfo() {};
	};
	class C3dActor* m_master;
	void* m_mesh; //class C3dMesh* m_mesh;
	char m_name[128];
	int m_numTexture;
	std::vector<CTexture*> m_texture;
	float m_opacity;
	C3dNode* m_parent;
	std::list<C3dNode*> m_child;
	struct matrix m_ltm;
	C3dPosAnim m_posAnim;
	C3dRotAnim m_rotAnim;
	class C3dScaleAnim m_scaleAnim;
	class std::vector<ColorInfo> m_colorInfo;
	class std::vector<int> m_destVertCol;
	int m_isAlphaForPlayer;
	struct C3dAABB m_aabb;

	virtual ~C3dNode() {};
};


class C3dActor {
	class C3dNode* m_node;
	char m_name[128];
	struct vector3d m_pos;
	struct vector3d m_rot;
	struct vector3d m_scale;
	int m_shadeType;
	int m_curMotion;
	int m_animType;
	int m_animLen;
	float m_animSpeed;
	struct matrix m_wtm;
	struct matrix m_iwtm;
	struct vector3d m_posOffset;
	int m_isMatrixNeedUpdate;
	int m_isBbNeedUpdate;
	struct C3dOBB m_oBoundingBox;
	struct C3dAABB m_aaBoundingBox;
	int m_renderSignature;
	int m_isHideCheck;
	unsigned char m_isHalfAlpha;
	unsigned char m_fadeAlphaCnt;
	std::list<CVolumeBox*> m_volumeBoxList;
	int m_blockType;

	virtual ~C3dActor() {};
};

struct SceneGraphNode {
	struct SceneGraphNode* m_parent;
	struct SceneGraphNode* m_child[4];
	struct C3dAABB m_aabb;
	struct vector3d m_center;
	struct vector3d m_halfSize;
	int m_needUpdate;
	std::vector<C3dActor*> m_actorList;
	C3dGround* m_ground;
	struct tagRECT m_groundArea;
	C3dAttr* m_attr;
	struct tagRECT m_attrArea;

	SceneGraphNode(struct SceneGraphNode&) {};
	SceneGraphNode::SceneGraphNode() {};
	SceneGraphNode::~SceneGraphNode() {};
	void SceneGraphNode::Build( int level) {};
	void SceneGraphNode::InsertObject( class C3dAttr* attr, int level) {};
	void SceneGraphNode::InsertObject( class C3dGround* ground, int level) {};
	void SceneGraphNode::InsertObject( class C3dActor* actor, int level) {};
	void SceneGraphNode::RemoveObject( class C3dActor* actor, int level) {};
	void SceneGraphNode::UpdateVolume( int level) {};
	void SceneGraphNode::UpdateVolumeAfter( int level) {};
	std::vector<C3dActor*>* SceneGraphNode::GetActorList( float x, float z, int level) {};
	void SceneGraphNode::InsertObjectAfter( class C3dActor* actor, int level) {};
	void SceneGraphNode::CopySceneGraph( int Level, struct SceneGraphNode* graph) {};
	void SceneGraphNode::LoadSceneGraph( class CFile* fp, int Level) {};
	struct SceneGraphNode& operator=(struct SceneGraphNode&) {};
};

class CViewFrustum {
public:
	std::list<SceneGraphNode*> m_cubeletListTotal;
	std::list<SceneGraphNode*> m_cubeletListPartial;
	struct plane3d m_planes[6];
	struct vector3d m_planeNormals[6];

	CViewFrustum(class CViewFrustum&);
	CViewFrustum::CViewFrustum();
	void CViewFrustum::Free();
	void CViewFrustum::Build( float hratio, float vratio, struct matrix& viewMatrix, struct SceneGraphNode* rootNode);
	void CViewFrustum::CullSceneNode( struct SceneGraphNode* node, int level, unsigned char isTotallyInside);
	int CViewFrustum::CheckAABBIntersect( struct SceneGraphNode* node);
	int CViewFrustum::CheckOBBIntersect( struct C3dOBB& bb);
	class CViewFrustum& operator=(class CViewFrustum&);
	CViewFrustum::~CViewFrustum();
};

class CView {
public:
	float m_sideQuake;
	float m_frontQuake;
	float m_latitudeQuake;
	unsigned char m_isFPSmode;
	int m_isQuake;
	unsigned long m_quakeStartTick;
	unsigned long m_QuakeTime;
	struct ViewInfo3d m_cur;
	struct ViewInfo3d m_dest;
	struct ViewInfo3d m_backupCur;
	struct ViewInfo3d m_backupDest;
	struct vector3d m_from;
	struct vector3d m_up;
	struct matrix m_viewMatrix;
	struct matrix m_invViewMatrix;
	class CViewFrustum m_viewFrustum;
	class CWorld* m_world;
	void* m_skyBox;//class CSkyBoxEllipse* m_skyBox;

	CView(class CView&) {};
	CView::CView() {};
	float GetCurLongitude() {};
	float GetCurLatitude() {};
	float GetCurDistance() {};
	float GetDestLongitude() {};
	float GetDestLatitude() {};
	float GetDestDistance() {};
	struct vector3d GetCurAt() {};
	struct vector3d GetFrom() {};
	struct matrix GetViewMatrix() {};
	void SetDestLongitude(float) {};
	void SetDestDistance(float) {};
	void SetDestLatitude(float) {};
	void SetDestAt(float, float, float) {};
	void SetCurLongitude(float) {};
	void SetCurDistance(float) {};
	void SetCurLatitude(float) {};
	void SetCurAt(float, float, float) {};
	void AdjustDestLongitude(float) {};
	void HoldAt() {};
	void ResetLongitude(float) {};
	void ResetLatitude(float) {};
	void ResetDistance(float) {};
	void ResetAt(float, float, float) {};
	void PushCamera() {};
	void PopCamera() {};
	void CView::AddLongitude( float deltaLongitude) {};
	void CView::AddLatitude( float deltaLatitude) {};
	void CView::AddDistance( float deltaDistance) {};
	void CView::SetQuake( int isQuake, int Type, float sideQuake, float frontQuake, float latitudeQuake) {};
	void CView::SetQuakeInfo( float sideQuake, float frontQuake, float latitudeQuake) {};
	void CView::GetEeyeVector( struct vector3d* eyeVector) {};
	struct vector3d* GetEeyeFromVector() {};
	unsigned char IsFPSmode() {};
	void CView::InterpolateViewInfo() {};
	void CView::ProcessQuake() {};
	void CView::BuildViewMatrix() {};
	class CView& operator=(class CView&) {};

	virtual CView::~CView() {};
	virtual void CView::OnEnterFrame() {};
	virtual void CView::OnExitFrame() {};
	virtual void CView::OnRender() {};
	virtual void CView::OnCalcViewInfo() {};
};

class CWorld {
public:
	class CMode* m_curMode;
	std::list<CGameObject*> m_gameObjectList;
	std::list<CGameActor*> m_actorList;
	std::list<CItem*> m_itemList;
	std::list<CSkill*> m_skillList;
	C3dGround* m_ground;
	CPlayer* m_player;
	C3dAttr* m_attr;
	std::vector<C3dActor*> m_bgObjList;
	long m_bgObjCount;
	long m_bgObjThread;
	int m_isPKZone;
	int m_isSiegeMode;
	int m_isBattleFieldMode;
	int m_isEventPVPMode;
	struct SceneGraphNode m_rootNode;
	struct SceneGraphNode* m_Calculated;

	virtual ~CWorld() {};
};

