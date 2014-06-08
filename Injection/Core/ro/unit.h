#pragma once

struct texCoor {
	float u;
	float v;
	void Set(float _u, float _v) { u=_u;v=_v; };
};

struct ColorCellPos {
	int x;
	int y;
	unsigned long color;
};

struct ColorCellPos2 {
	int x;
	int y;
	int type;
	unsigned long color;
	unsigned long startTime;
};

struct CAttachPointInfo {
	int x;
	int y;
	int m_attr;
};

struct ChatRoomInfo {
	std::basic_string<char> title;
	std::basic_string<char> pass;
	int roomType;
	int numPeople;
	int maxNumPeople;
	int roomId;

	ChatRoomInfo(struct ChatRoomInfo&) {};
	ChatRoomInfo() {};
	struct ChatRoomInfo& operator=(struct ChatRoomInfo&) {};
	ChatRoomInfo::~ChatRoomInfo() {};
};

struct NamePair {
	std::basic_string<char> cName;
	std::basic_string<char> pName;
	std::basic_string<char> gName;
	std::basic_string<char> rName;

	void Clear() {};
	unsigned char IsEmpty() {};
	NamePair::NamePair( struct NamePair& __that) {};
	NamePair::NamePair() {};
	struct NamePair& operator=(struct NamePair&) {};
	NamePair::~NamePair() {};
};


struct vector2d {
	float x;
	float y;

	vector2d(float, float){};
	vector2d() : x(.0f),y(.0f) {};
	void Set(float, float){};
	void Normalize() {};
};



struct vector3d {
	float x;
	float y;
	float z;

	vector3d::vector3d(float nx, float ny, float nz) : x(nx),y(ny),z(nz) {};
	vector3d::vector3d() : x(.0f),y(.0f),z(.0f) {};
	void Set(float nx, float ny, float nz) { x=nx;y=ny;z=nz; };

	void MatrixMult(struct vector3d&, struct matrix&);

	void CrossProduct(struct vector3d& u, struct vector3d& v)
	{
		x = u.y * v.z - u.z * v.y;
		y = u.z * v.x - u.x * v.z;
		z = u.x * v.y - u.y * v.x;
	};
	void Normalize()
	{
		float v = Magnitude();
		x /= v;
		y /= v;
		z /= v;
	};
	float Magnitude()
	{
		return pow( x*x + y*y + z*z , 0.5f );
		//return sqrl( x*x + y*y + z*z );
	};
	float Angle(struct vector3d&) {};
	float DotProduct(struct vector3d& v)
	{
		return (x * v.x + y * v.y + z * v.z);
	};
	void vector3d::MATRIX_TO_VECTOR(struct matrix& dir) {};

	struct vector3d& vector3d::operator=(struct vector3d& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		return *this;
	};
	struct vector3d& operator+=(struct vector3d& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	};
	struct vector3d& operator-=(struct vector3d& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	};
	struct vector3d& operator*=(float& v)
	{
		this->x *= v;
		this->y *= v;
		this->z *= v;
		return *this;
	};
	struct vector3d  operator*(float& v) {
		this->x *= v;
		this->y *= v;
		this->z *= v;
		return *this;
	};
};

struct matrix {
	float v11;
	float v12;
	float v13;
	float v21;
	float v22;
	float v23;
	float v31;
	float v32;
	float v33;
	float v41;
	float v42;
	float v43;

	void SetRow1(float, float, float) {};
	void SetRow2(float, float, float) {};
	void SetRow3(float, float, float) {};
	void SetRow4(float, float, float) {};
	void MakeIdentity() {};
	void matrix::MakeInverse( struct matrix& src) {};
	void MakeTranslate(struct vector3d&) {};
	void matrix::MakeTranslate( float x, float y, float z) {};
	void matrix::MakeXRotation( float angle) {};
	void matrix::MakeYRotation( float angle) {};
	void matrix::MakeZRotation( float angle) {};
	void matrix::MakeScale( float x, float y, float z) {};
	void matrix::AppendTranslate( struct vector3d& v) {};
	void matrix::AppendXRotation( float angle) {};
	void matrix::AppendYRotation( float angle) {};
	void matrix::AppendZRotation( float angle) {};
	void matrix::AppendScale( float x, float y, float z) {};
	void matrix::PrependTranslate( struct vector3d& v) {};
	void PrependScale(float*) {};
	void matrix::PrependScale( float x, float y, float z) {};
	void matrix::MakeTransform( struct rotKeyframe& ani) {};
	void matrix::MakeTransform( float* rotaxis, float rotangle) {};
	void matrix::MakeTransform( struct vector3d& pos, float* rotaxis, float rotangle) {};
	void matrix::MakeView( struct vector3d& from, struct vector3d& at, struct vector3d& world_up) {};
	void matrix::MultMatrix( struct matrix& a, struct matrix& b) {};
	int matrix::IsIdentity() { return 0;};
	void matrix::VECTOR_TO_VIEW( struct vector3d& base, struct vector3d& point, struct vector3d& up, struct vector3d& right) {};
	void matrix::VIEW_MATRIX( struct vector3d& base, struct vector3d& point, int roll) {};
	void matrix::VECTOR_TO_REV_VIEW( struct vector3d& base, struct vector3d& point, struct vector3d& up, struct vector3d& right) {};
	void matrix::REV_VIEW_MATRIX( struct vector3d& base, struct vector3d& point, int roll) {};
	void matrix::VECTOR_TO_MATRIX( struct vector3d& view_dir) {};
	void matrix::NORMALIZE_SCALE( struct matrix& sour) {};
	void matrix::REVERSE_MATRIX( struct matrix& sour) {};
	void matrix::UPVECTOR_TO_MATRIX( struct vector3d up) {};
};

struct plane3d {
	float x;
	float y;
	float z;
	float w;
	plane3d(float, float, float, float) {};
	plane3d() {};
	void Set(float, float, float, float) {};
	void MatrixMult(struct matrix&, struct plane3d&) {};
};

struct tlvertex3d {
	float x;
	float y;
	float z;
	float oow;
	union {
		unsigned long color;
		struct COLOR argb;
	};
	unsigned long specular;
	union {
		struct {
			float tu;
			float tv;
		};
		struct texCoor coord;
	};

	tlvertex3d(float, float, float, float, unsigned long, unsigned long, float, float) {};
	tlvertex3d::tlvertex3d() {};
};

struct ViewInfo3d {
	struct vector3d at;
	float latitude;
	float longitude;
	float distance;

	ViewInfo3d() {};
	struct ViewInfo3d& operator=(struct ViewInfo3d&) {};
};

struct CellPos {
	int x;
	int y;
};

struct C3dAABB {
	struct vector3d min;
	struct vector3d max;

	C3dAABB() {};
	struct C3dAABB& operator=(struct C3dAABB&) {};
};

struct C3dOBB {
	struct vector3d halfSize;
	struct vector3d center;
	struct vector3d u;
	struct vector3d v;
	struct vector3d w;
	struct vector3d vertices[8];

	C3dOBB() {};
	struct C3dOBB& operator=(struct C3dOBB&) {};
};

struct CVolumeBox {
	struct vector3d m_size;
	struct vector3d m_pos;
	struct vector3d m_rot;
	struct vector3d m_worldVert[8];
	struct matrix m_vtm;
	struct matrix m_ivtm;
	int flag;

	CVolumeBox() {};
	struct CVolumeBox& operator=(struct CVolumeBox&) {};
};


struct posKeyframe {
	int frame;
	float px;
	float py;
	float pz;

	posKeyframe(int, float, float, float) {};
	posKeyframe() {};
	void posKeyframe::Slerp( float t, struct posKeyframe& start, struct posKeyframe& end) {};
};

struct scaleKeyframe {
	int frame;
	float sx;
	float sy;
	float sz;
	float qx;
	float qy;
	float qz;
	float qw;

	scaleKeyframe(int, float, float, float, float, float, float, float) {};
	scaleKeyframe() {};
	void scaleKeyframe::Slerp( float t, struct scaleKeyframe& start, struct scaleKeyframe& end, int spin) {};
};

struct rotKeyframe {
	int frame;
	float qx;
	float qy;
	float qz;
	float qw;

	rotKeyframe(int, float, float, float, float) {};
	rotKeyframe() {};
	void rotKeyframe::Slerp( float t, struct rotKeyframe& start, struct rotKeyframe& end, int spin) {};
};

class C3dPosAnim {
	std::vector<posKeyframe> m_animdata;
};
class C3dScaleAnim {
	std::vector<scaleKeyframe> m_animdata;
};
class C3dRotAnim {
	std::vector<rotKeyframe> m_animdata;
};

struct TeiEffect {
	char life;
	short alphaB;
	short alphaT;
	short full_display_angle;
	float max_height;
	int process;
	short RotStart;
	float height[21];
	char flag1[21];
	short rise_angle;
	float distance;
	struct vector3d vecB_now;
	struct vector3d vecT_now;
	struct vector3d vecB_pre;
	struct vector3d vecT_pre;

	TeiEffect() {};
	struct TeiEffect& operator=(struct TeiEffect&) {};
};

struct PrimSegment {
	struct vector3d pos;
	struct vector3d segPos[4];
	float radius;
	float size;
	float longitude;
	struct matrix mat;
	int red;
	int green;
	int blue;
	float alpha;
	unsigned long argb;

	PrimSegment() {};
	struct PrimSegment& operator=(struct PrimSegment&) {};
};


struct PathCell {
	int x;
	int y;
	int dir;
	unsigned long time;
};

class CPathInfo {
public:
	std::vector<PathCell> m_pathData;
	int m_startCell;
	float m_startPointX;
	float m_startPointY;

	CPathInfo() {};
};

struct _MSG2AI {
	int command;
	int x;
	int y;
	int skillLevel;
	int skillID;
	unsigned long targetID;

	_MSG2AI::_MSG2AI() { Init(); };
	void Init() {};
};
