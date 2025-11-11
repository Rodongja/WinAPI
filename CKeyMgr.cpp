#include "pch.h"
#include "CKeyMgr.h"

#include "CCore.h"

int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT,	//LEFT,
	VK_RIGHT,	//RIGHT,
	VK_UP,	//UP,
	VK_DOWN,	//DOWN,

		'Q',
		'W',
		'E',
		'R',
		'T',
		'Y',
		'U',
		'I',
		'O',
		'P',
		'A',
		'S',
		'D',
		'F',
		'G',
		'H',
		'J',
		'K',
		'L',
		'Z',
		'X',
		'C',
		'V',
		'B',
		'N',
		'M',

		VK_KEY_1,
		VK_KEY_2,
		VK_KEY_3,
		VK_KEY_4,
		VK_KEY_5,
		VK_KEY_6,
		VK_KEY_7,
		VK_KEY_8,
		VK_KEY_9,
		VK_KEY_0,

		VK_MENU,//ALT,
		VK_CONTROL,//CTRL,
		VK_LSHIFT,//LSHIFT,
		VK_SPACE,//SPACE,
		VK_RETURN,//ENTER,
		VK_ESCAPE,//ESC,

		VK_LBUTTON,
		VK_RBUTTON,

		//LAST, 배열의 끝을 알리기 위함이므로 추가할 필요 없음
};

CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::init()
{
	// 키의 총 개수(LAST)만큼 NONE(누르지 않은 상태)로 초기화
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE,false });
	}

	//m_vecKey[(int)KEY::LEFT].eState;
	//m_vecKey[(int)KEY::LEFT].bPrevPush;

}

void CKeyMgr::update()
{
	//// 윈도우 포커싱 알아내기
	//HWND hMainWnd = CCore::GetInst()->GetMaindHwnd();
	// 현재 실행중인 윈도우 알아내는 함수 GetFocus();
	HWND hWnd = GetFocus();

	// 윈도우 포커싱 중일 때 키 이벤트 동작
	if (nullptr != hWnd)
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// 키가 눌려있다.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에도 눌려있었다
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					// 이전에 눌려있지 않았다
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				// 이전 키 상태 갱신
				m_vecKey[i].bPrevPush = true;
			}
			// 키가 안눌려있다
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에 눌려있었다
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					// 이전에 눌려있지 않았다
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				// 이전 키 상태 갱신
				m_vecKey[i].bPrevPush = false;
			}
		}

		// Mouse 위치 계산
		POINT ptPos = {};
		GetCursorPos(&ptPos);

		ScreenToClient(CCore::GetInst()->GetMainHwnd(),&ptPos);

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
	}

	// 윈도우 포커싱 해제상태
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;
			if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::HOLD == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == m_vecKey[i].eState)
			{https://www.youtube.com/watch?v=Q47cpjahcYg&list=PL4SIC1d_ab-ZLg4TvAO5R4nqlJTyJXsPK&index=2
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}
	}
}