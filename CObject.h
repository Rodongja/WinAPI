#pragma once

#include "global.h"
#include "CCamera.h"

class CCollider;
class CAnimator;
class CRigidBody;
class CGravity;
class CPlayer;

class CObject
{
private:
	wstring		m_strName;		// 오브젝트 이름
	GROUP_TYPE	m_eType;		// 오브젝트 타입
	
	Vec2		m_vPos;			// 오브젝트 좌표		struct.h
	Vec2		m_vScale;		// 오브젝트 크기		struct.h

	// Component
	// 하나의 컴포넌트를 상속하는 구조로 만들어보기
	CCollider*	m_pCollider;
	CAnimator*	m_pAnimator;
	CRigidBody* m_pRigidBody;
	CGravity*	m_pGravity;

	bool		m_bAlive;

public:
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	void SetType(GROUP_TYPE _eType) { m_eType = _eType; }
	GROUP_TYPE GetType() { return m_eType; }

	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CRigidBody* GetRigidBody() { return m_pRigidBody; }
	CGravity* GetGravity() { return m_pGravity; }

	void RemeberThisObject();
	
	bool IsDead() { return !m_bAlive; }

	void CreateCollider();
	void CreateAnimator();
	void CreateRigidBody();
	void CreateGravity();

	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

private:
	void SetDead() { m_bAlive = false; }

public:
	virtual void start() {}; // Scene이 시작되기 직전에 호출되는 함수
	virtual void update() = 0; // 순수 가상함수를 포함하는 클래스는 추상클래스
	//virtual void finalupdate() final; // 자식 클래스에서 오버라이딩 하는것을 방지하여 부모 클래스의 원본 함수만 설계 가능
	// 의도한대로 설계하려면 NON-UI 오브젝트와 UI 오브젝트를 따로 묶어서 설계해야함 (47화 35:50)
	virtual void finalupdate(); // 하위 클래스에서 finalupdate 오버라이딩 허용
	virtual void render(HDC _dc);

	void component_render(HDC _dc);

	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();	//상속 목적이므로 virtual 소멸자가 필요
	
	friend class CEventMgr;
};

