#pragma once

// 1. 프레임 동기화
// 동일 프레임 내에서 같은 키에 대해 키 입력 이벤틀르 가져간다.

// 2. 키 입력 이벤트 처리
// tap, hold, away (유니티 기준 GetButtonDown, GetButton, GetButtonUp)



enum class KEY_STATE
{
	NONE, // 이전에도 지금도 눌리지 않은 상태
	TAP, // 막 누른 시점
	HOLD, // 누르고 있는 시점
	AWAY, // 막 뗀 시점
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	_0,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LBTN,
	RBTN,

	LAST, // enum class의 끝
};

struct tKeyInfo
{
	KEY_STATE	eState;	// 키의 상태값
	bool		bPrevPush;	// 이전 프레임에서 키가 눌렸는지 체크
};

class CKeyMgr
{
	SINGLE(CKeyMgr);
private:
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_vCurMousePos;

public:
	void init();
	void update();
public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	Vec2 GetMousePos() { return m_vCurMousePos; }
};

