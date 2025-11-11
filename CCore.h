#pragma once

// 싱글톤 패턴
// 객체의 생성을 1개로 제한
// 어디서든 쉽게 접근 가능

//// 동적할당
//class CCore
//{
//private:
//	static CCore* g_pInst;
//
//public:
//	// 정적 멤버 함수
//	static CCore* GetInstance()
//	{
//		// 최초 호출 된 경우
//		// 정적 변수
//		if (nullptr == g_pInst)
//		{
//			g_pInst = new CCore;
//		}
//
//		// 2번 이상 호출된 경우 아무 일도 일어나지 않음
//		//else
//		//{
//
//		//}
//		return g_pInst;
//	}
//
//	// 할당 해제
//	static void Release()
//	{
//		if (nullptr != g_pInst)
//		{
//			delete g_pInst;
//			g_pInst = nullptr;
//		}
//	}
//
//private:
//	CCore();
//	~CCore();
//};

class CTexture;

// 동작 중간에 할당 해제가 불가능한 형태
class CCore
{
	SINGLE(CCore);

private:
	HWND		m_hWnd;			// 메인 윈도우 핸들
	POINT		m_ptResolution;	// 메인 윈도우 해상도
	HDC			m_hDC;			// 메인 윈도우에 Draw 할 DC

	//HBITMAP m_hBit;			// 비트맵 핸들
	//HDC		m_memDC;		// 사본용 DC

	CTexture*	m_pMemTex;		// 백버퍼 텍스쳐

	// 자주 사용하는 GDI Object
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];

	HMENU	m_hMenu; // Tool Scene에서만 사용

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();

public:
	void DockMenu();
	void UndockMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);

private:
	void Clear();
	void CreateBrushPen();


public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }

	POINT GetResolution() { return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }
};