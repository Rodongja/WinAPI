#include "pch.h"
#include "CCore.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CObject.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"

#include "CRandomMgr.h"

#include "CResourceMgr.h"
#include "CTexture.h"
#include "SelectGDI.h"

#include "resource.h"

//CObject g_obj;

CCore::CCore()
	: m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	//, m_hBit(0)
	//, m_memDC(0)
	, m_arrBrush{}
	, m_arrPen{}
{
}
 
// 이중 포인터
//void Test(int** a)
//{
//	*a = new int;
//}

CCore::~CCore()
{
	ReleaseDC(m_hWnd,m_hDC);

	//// CreateCompatibleDC와 CreateCompatibleBitmap로만든건 Release가 아닌 Delete로 삭제해야함
	//DeleteDC(m_memDC);
	//DeleteObject(m_hBit);

	// 펜 삭제
	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}

	DestroyMenu(m_hMenu); // DeleteMenu는 메뉴의 항목 삭제 DestroyMenu는 메뉴바 전체를 삭제
}


int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	// 이중 포인터
	//int* a = nullptr;
	//Test(&a);

	//HRESULT hr = S_OK;

	//if (FAILED(S_OK))
	//{

	//}

	//E_FAIL;

	//return E_FAIL;

	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// 해상도에 맞게 윈도우 크기 조정
	ChangeWindowSize(Vec2((float)_ptResolution.x, (float)_ptResolution.y), false);

	// 메뉴바 생성
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_CLIENT));

	m_hDC = GetDC(m_hWnd);

	// 이중 버퍼링 용도의 텍스쳐 한장을 만든다
	m_pMemTex = CResourceMgr::GetInst()->CreateTexture(L"BackBuffer",(UINT)m_ptResolution.x,(UINT)m_ptResolution.y);

	//// 이중 버퍼링 용도의 비트맵과 DC를 만든다
	//m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	//m_memDC = CreateCompatibleDC(m_hDC);

	//HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	//// 새로 비트맵을 전달받으면 기존의 비트맵(1픽셀)은 필요없으므로 삭제
	//DeleteObject(hOldBit);

	// 자주 사용 할 펜 및 브러쉬 생성
	CreateBrushPen();

	// Manager 초기화
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();
	CRandomMgr::GetInst()->init();
	CCamera::GetInst()->init();

	//// Sound 로드 테스트
	// 주로 FMOD가 사용됨
	//CResourceMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\DM.wav");
	//CSound* pNewSound = CResourceMgr::GetInst()->FindSound(L"BGM_01");

	//pNewSound->Play();
	//pNewSound->Play(true); // 반복재생

	// pNewSound->SetPosition(50.f);	// 백분율, 소리 위치 설정, 버퍼 내 재생 위치 포인터
	// pNewSound->PlayAsBGM(true);
	// pNewSound->SetVolume(60.f);

	//g_obj.SetPos(Vec2( (float)m_ptResolution.x / 2, (float)m_ptResolution.y / 2 ));
	//g_obj.SetScale(Vec2(100, 100));

	return S_OK;
}


void CCore::progress()
{
	//static int callcount = 0;
	//++callcount;
	//static int iPrevCount = GetTickCount();

	//int iCurCount = GetTickCount();
	//if (iCurCount - iPrevCount > 1000)
	//{
	//	iPrevCount = iCurCount;
	//	callcount = 0;
	//}

	// ==============
	// Manager Update
	// ==============

	CTimeMgr::GetInst()->update();
	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();

	// ============
	// Scene Update
	// ============

	CSceneMgr::GetInst()->update();

	// 충돌 체크
	CCollisionMgr::GetInst()->update();

	// UI 이벤트 체크
	CUIMgr::GetInst()->update();

	// 랜덤함수 업데이트
	CRandomMgr::GetInst()->update();

	// =========
	// Rendering
	// =========

	Clear();


	CSceneMgr::GetInst()->render(m_pMemTex->GetDC());
	CCamera::GetInst()->render(m_pMemTex->GetDC());

	// 이중 버퍼링
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_pMemTex->GetDC(), 0, 0, SRCCOPY);

	CTimeMgr::GetInst()->render();

	// ==============
	// 이벤트 지연처리
	// ==============
	CEventMgr::GetInst()->update();
}

void CCore::DockMenu()
{
	SetMenu(m_hWnd, m_hMenu);
	ChangeWindowSize(GetResolution(), true);
}

void CCore::UndockMenu()
{
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(GetResolution(), false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt = { 0,0,(long)_vResolution.x,(long)_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu); // true 메뉴바 있음 false 메뉴바 없음
	//rt.right - rt.left; // 윈도우 좌우 여백 크기
	//rt.bottom - rt.top; // 윈도우 상하 여백 크기(메뉴 포함)
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}

void CCore::Clear()
{	
	// 화면 Clear
	SelectGDI gdi(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);

	// 오프 스크린에 그리기
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
}

void CCore::CreateBrushPen()
{
	// hollow brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);

	// red blue green pen
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}

//void CCore::update()
//{
//	Vec2 vPos = g_obj.GetPos();
//
//	// // 비동기 키 입출력 함수
//	// // 윈도우가 포커싱 되어있지 않아도 입력이 되는 문제가 있음
//	//if (GetAsyncKeyState(VK_LEFT) & 0x8000) // 왼쪽 키가 눌린 순간
//	//{
//	//	vPos.x -= 200.f * fDT;
//	//	//vPos.x -= 100.f* DeltaTime;
//	//}	
//	//
//	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) // 오른쪽 키가 눌린 순간
//	//{
//	//	vPos.x += 200.f * fDT;
//	//	//vPos.x += 100.f * DeltaTime;
//	//}
//
//	if (CKeyMgr::GetInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD) // KEY_STATE) TAP,HOLD,AWAY로 나뉘어짐
//	{
//		vPos.x -= 200.f * CTimeMgr::GetInst()->GetfDT();
//		//vPos.x -= 100.f* DeltaTime;
//	}
//
//	if (CKeyMgr::GetInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD) 
//	{
//		vPos.x += 200.f * CTimeMgr::GetInst()->GetfDT();
//		//vPos.x += 100.f * DeltaTime;
//	}
//
//	g_obj.SetPos(vPos);
//}


//void CCore::render()
//{
//	// 화면 Clear
//	// 화면 테두리 1픽셀은 검은색으로 그리게 되므로 화면 전체를 클리어 해주려면 1의 유격을 줘야함
//	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
//
//	// 메세지가 안들어와도 그리기 같은 작업이 가능함
//	// 그리기
//	Vec2 vPos = g_obj.GetPos();
//	Vec2 vScale = g_obj.GetScale();
//	Rectangle(m_memDC
//		, int(vPos.x - vScale.x / 2.f)
//		, int(vPos.y - vScale.y / 2.f)
//		, int(vPos.x + vScale.x / 2.f)
//		, int(vPos.y + vScale.y / 2.f));
//
//	// 임시 비트맵(m_memDC)에 그려진 내용을 윈도우(m_hDC)로 복사
//	// 이런 단순 연산 방식은 cpu의 성능을 크게 요구함
//	// 윈도우 API는 cpu를 사용하지만 direct같은 방식은 gpu를 사용하여 훨씬 빠르게 처리 가능
//	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC,0,0,SRCCOPY);
//	
//}
