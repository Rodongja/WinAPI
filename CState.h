#pragma once

class AI;
class CMonster;

class CState
{
private:
	AI*			m_pAI;
	MON_STATE	m_eState;

public:
	AI* GetAI() { return m_pAI; }
	MON_STATE GetType() { return m_eState; }
	CMonster* GetMonster();

public:
	// 순수 가상함수
	virtual void update() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;

public:
	CState(MON_STATE _eState);
	virtual ~CState();	// 상속을 시켜서 파생을 할것이기 때문에 소멸자에 가상함수를 붙여줌

	friend class AI;
};

