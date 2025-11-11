#pragma once

//#define Add(a,b) a + b

// Singleton ¸ÅÅ©·Î
#define SINGLE(type) public:\
						static type* GetInst()\
						{\
							static type mgr;\
							return &mgr;\
						}\
					private:\
						type();\
						~type();

#define fDT CTimeMgr::GetInst()->GetfDT()
#define DT CTimeMgr::GetInst()->GetDT()

#define CLONE(type) type* Clone() {return new type(*this);}

#define KEY_CHECK(key,state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key,KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key,KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key,KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key,KEY_STATE::NONE)
#define MOUSE_POS CKeyMgr::GetInst()->GetMousePos()

#define VK_KEY_1 0x31
#define VK_KEY_2 0x32
#define VK_KEY_3 0x33
#define VK_KEY_4 0x34
#define VK_KEY_5 0x35
#define VK_KEY_6 0x36
#define VK_KEY_7 0x37
#define VK_KEY_8 0x38
#define VK_KEY_9 0x39
#define VK_KEY_0 0x30

#define TILE_SIZE 64

enum class GROUP_TYPE
{
	DEFAULT,		// 0
	TILE,			// 1
	GROUND,			// 2
	MONSTER,		// 3
	PLAYER,			// 4
	PROJ_PLAYER,	// 5
	PROJ_MONSTER,	// 6

	UI = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	STAGE_01,
	STAGE_02,

	END,
};

enum class BRUSH_TYPE
{
	HOLLOW,
	BLACK,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,


	END,
};

enum class MON_STATE
{
	IDLE,
	PATROL,
	TRACE,
	ATTACK,
	RETREAT,
	DEAD,
	END,
};