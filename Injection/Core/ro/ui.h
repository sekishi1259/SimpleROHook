#pragma once

class UIWindow {
public:
	class UIWindow* m_parent;
	std::list<UIWindow> m_children; // class std::list<UIWindow *,std::allocator<UIWindow *> > m_children;
	int m_x;
	int m_y;
	int m_w;
	int m_h;
	int m_isDirty;
	void *m_dc; //CDC* m_dc;
	int m_id;
	int m_state;
	int m_stateCnt;
	int m_show;
	unsigned long m_trans;
	unsigned long m_transTarget;
	unsigned long m_transTime;

	UIWindow(class UIWindow&) {};
	UIWindow::UIWindow() {};
/*
	void UIWindow::Create( int cx, int cy);
	void UIWindow::Destroy();
	void UIWindow::AddChild( class UIWindow* window);
	void UIWindow::RemoveChild( class UIWindow* window);
	void UIWindow::RemoveAllChildren();
	void UIWindow::ClearAllChildrenState();
	void UIWindow::InvalidateChildren();
	void UIWindow::SetId int id);
	int GetId();
	int UIWindow::IsFamily( class UIWindow* window);
	class UIWindow* UIWindow::HitTest( int x, int y);
	void UIWindow::DoDraw( int blitToParent);
	unsigned char UIWindow::IsChildOf( class UIWindow* win);
	class UIWindow* UIWindow::GetNextSiblingOf( class UIWindow* wnd);
	class UIWindow* UIWindow::GetPrevSiblingOf( class UIWindow* wnd);
	class UIWindow* UIWindow::GetAncestor();
	void UIWindow::GetGlobalCoor( int& x, int& y);
	int IsShow();
	int GetX();
	int GetY();
	int GetHeight();
	int GetWidth();
	void UIWindow::DrawBitmap( int x, int y, class CBitmapRes* bitmap, unsigned char drawOnlyNoTrans);
	void UIWindow::TextOutA( int x, int y, char* text, int textLen, int fontType, int fontHeight, unsigned long colorRef);
	void UIWindow::TextOutWithRightAlign( int x,  int y, char* text, int textLen, int fontType, int fontHeight, unsigned long colorRef);
	void UIWindow::TextOutWithDecoration( int x,  int y, char* text, int textLen, unsigned long* colorRef, int fontType, int fontHeight);
	void UIWindow::TextOutWithShadow( int x,  int y, char* text, int textLen, unsigned long colorText, unsigned long colorShadow, int fontType, int fontHeight);
	void UIWindow::TextOutWithOutline( int x,  int y, char* text, int textLen, unsigned long colorText, unsigned long colorOutline, int fontType, int fontHeight, unsigned char bold);
	void UIWindow::TextOutWithDecoOutline( int x,  int y, char* text, int textLen, unsigned long* colorText, unsigned long colorOutline, int fontType, int fontHeight, unsigned char bold);
	void UIWindow::TextOutVertical( int x,  int y, char* text, int textLen, int fontType, int fontHeight, unsigned long colorRef);
	void UIWindow::TextOutWithOutlineVertical( int x,  int y, char* text, int textLen, unsigned long colorText, unsigned long colorOutline, int fontType, int fontHeight, unsigned char bold);
	void UIWindow::TextOutWithTwoColors( int x,  int y, char* text, unsigned long colorRef, unsigned long colorRef2, int textLen, int fontType, int fontHeight);
	int UIWindow::GetTextWidth( char* text,  int textLen, int fontType, int fontHeight, unsigned char bold);
	int UIWindow::GetTextHeight( char* text,  int textLen, int fontType, int fontHeight, unsigned char bold);
	int UIWindow::GetTextHeightVertical( char* text,  int textLen, int fontType, int fontHeight, unsigned char bold);
	int UIWindow::GetColorTextWidth( char* text,  int textLen, int fontType, int fontHeight, unsigned char bold);
	char* UIWindow::PolishText( char* text,  int drawWidth, int fontType, int fontHeight, unsigned char bold);
	char* UIWindow::PolishText2( char* text,  int maxNumCharLine);
	void UIWindow::PolishText3( char* text,  class std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >& strings, class std::vector<bool,std::allocator<bool> >& enters, int width);
	void UIWindow::PolishText4( char* text,  class std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >& strings, int width);
	void UIWindow::ClearDC( unsigned long color);
	void UIWindow::ClearDCRect( unsigned long color,  struct tagRECT& rect);
	void UIWindow::DrawBox( int x,  int y, int cx, int cy, unsigned long color);
	void UIWindow::DrawBorderLine( int x,  int y, int w, int h, unsigned long color);
	void UIWindow::DrawTriangleDC( int x1,  int y1, int x2, int y2, int x3, int y3, unsigned long color);
	char* UIWindow::InterpretColor( char* color_text,  unsigned long* colorRef);
	char* UIWindow::FindColorMark( char* text,  char* text_end);
	class UIWindow& operator=(class UIWindow&);
	// void __local_vftable_ctor_closure();
*/
	virtual UIWindow::~UIWindow() {};
/*
	virtual void UIWindow::Invalidate();
	virtual void UIWindow::InvalidateWallPaper();
	virtual void UIWindow::Resize( int cx,  int cy);
	virtual unsigned char UIWindow::IsFrameWnd();
	virtual unsigned char UIWindow::IsUpdateNeed();
	virtual void UIWindow::Move( int x,  int y);
	virtual unsigned char UIWindow::CanGetFocus();
	virtual unsigned char UIWindow::GetTransBoxInfo( struct BOXINFO* __formal);
	virtual unsigned char UIWindow::IsTransmitMouseInput();
	virtual unsigned char UIWindow::ShouldDoHitTest();
	virtual void UIWindow::DragAndDrop( int x,  int y, struct DRAG_INFO* dragInfo);
	virtual void UIWindow::StoreInfo();
	virtual void UIWindow::SaveOriginalPos();
	virtual void UIWindow::MoveDelta( int deltaDragX,  int deltaDragY);
	virtual unsigned long UIWindow::GetColor();
	virtual void UIWindow::SetShow( int visible);
	virtual void UIWindow::OnCreate( int cx,  int cy);
	virtual void UIWindow::OnDestroy();
	virtual void UIWindow::OnProcess();
	virtual void UIWindow::OnDraw();
	virtual void UIWindow::OnDraw2();
	virtual void UIWindow::OnRun();
	virtual void UIWindow::OnSize( int cx,  int cy);
	virtual void UIWindow::OnBeginEdit();
	virtual void UIWindow::OnFinishEdit();
	virtual void UIWindow::OnLBtnDown( int x,  int y);
	virtual void UIWindow::OnLBtnDblClk( int x,  int y);
	virtual void UIWindow::OnMouseMove( int x,  int y);
	virtual void UIWindow::OnMouseHover( int x,  int y);
	virtual void UIWindow::OnMouseShape( int x,  int y);
	virtual void UIWindow::OnLBtnUp( int x,  int y);
	virtual void UIWindow::OnRBtnDown( int x,  int y);
	virtual void UIWindow::OnRBtnUp( int x,  int y);
	virtual void UIWindow::OnRBtnDblClk( int x,  int y);
	virtual void UIWindow::OnWheel( int wheel);
	virtual void UIWindow::RefreshSnap();
	virtual int UIWindow::SendMsg( class UIWindow* sender,  int message, int val1, int val2, int val3, int val4);
	virtual unsigned char UIWindow::GetTransBoxInfo2( struct BOXINFO* __formal);
	virtual void UIWindow::DrawBoxScreen2();
	//virtual void* __vecDelDtor(unsigned int);
*/
};

class UIBalloonText : public UIWindow  {
public:
	unsigned char m_isBold;
	int m_fontSize;
	std::vector< std::basic_string<char> > m_strings;
	// class std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > m_strings;
	unsigned long m_fontColor;
	unsigned long m_bgColor;
	unsigned char m_isBack;
	short m_charfont;

	UIBalloonText(class UIBalloonText&) {};
	UIBalloonText::UIBalloonText() {};
	void UIBalloonText::SetText( char* msg, int maxNumCharLine) {};
	void UIBalloonText::AddText( char* msg) {};
	void SetFntColor(unsigned long, unsigned long) {};
	void UIBalloonText::SetFntSize( int fontSize) {};
	void SetBackTrans(unsigned char) {};
	void SetCharFont(short) {};
	void UIBalloonText::AdjustSizeByText() {};
	class UIBalloonText& operator=(class UIBalloonText&) {};

	virtual UIBalloonText::~UIBalloonText() {};
	virtual void UIBalloonText::OnProcess() {};
	virtual void UIBalloonText::OnDraw() {};
	virtual unsigned char UIBalloonText::ShouldDoHitTest() {};
};

class UITransBalloonText : public UIBalloonText  {
public:
	struct BOXINFO m_transBoxInfo;

	UITransBalloonText(class UITransBalloonText&) {};
	UITransBalloonText::UITransBalloonText() {};
	void UITransBalloonText::SetTransBoxColor( unsigned long transboxColor) {};
	class UITransBalloonText& operator=(class UITransBalloonText&) {};

	virtual UITransBalloonText::~UITransBalloonText() {};
	virtual void UITransBalloonText::OnDraw() {};
	virtual unsigned char UITransBalloonText::GetTransBoxInfo( struct BOXINFO* boxInfo) {};
	virtual void UITransBalloonText::OnCreate( int cx, int cy) {};
	virtual void UITransBalloonText::Move( int x, int y) {};
	virtual void UITransBalloonText::Resize( int cx, int cy) {};
};

