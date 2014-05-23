
enum EBtnState
{
	BTN_NONE,
	BTN_DOWN,
	BTN_PRESSED,
	BTN_UP,
	BTN_DBLCLK
};

class CMouse
{
public:
	IDirectInput7*			m_lpdi;
	IDirectInputDevice7*	m_pMouse;
	LPVOID					m_hevtMouse;
	int						m_xDelta;
	int						m_yDelta;
	int						m_xPos;
	int						m_yPos;
	int						m_wheel;
	int						m_oldBtnState[3];
	EBtnState				m_btnState[3];
	int						m_dblclkCnt[3];
	DWORD					m_dblclkTime;
	DWORD					m_bSwapButton;
};

