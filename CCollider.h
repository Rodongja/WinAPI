#pragma once

class CObject;

class CCollider
{
private:
	static UINT	g_iNextID;			

	CObject*	m_pOwner;			// collider를 소유하고 있는 오브젝트
	unordered_set<CObject*>	m_hTarget;			// 충돌 한 오브젝트
	Vec2		m_vOffsetPos;		// 오브젝트로부터 상대적인 위치
	Vec2		m_vFinalPos;		// finalupdate에서 매 프레임마다 계산
	Vec2		m_vScale;			// 충돌체의 크기

	UINT		m_iID;				// 충돌체 고유한 ID 값
	UINT		m_iCol;

	bool		m_bActive;			// 충돌체 활성화 여부

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	void AddTarget(CObject* _Target);

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }

	Vec2 GetFinalPos() { return m_vFinalPos; }

	CObject* GetObj() { return m_pOwner; }
	unordered_set<CObject*> GetTarget() { return m_hTarget; }

	UINT GetID() { return m_iID; }

public:
	void finalupdate();
	void render(HDC _dc);

public:
	// 충돌 시점 함수
	void OnCollision(CCollider* _pOther); // 충돌 중인 경우 호출되는 함수
	void OnCollisionEnter(CCollider* _pOther); // 충돌 진입 시 호출되는 함수
	void OnCollisionExit(CCollider* _pOther); // 충돌 해제 시 호출되는 함수

	// 충돌체는 대입할 경우가 없으므로 대입 연산 방지
	CCollider& operator = (CCollider& _origin) = delete;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class CObject;
};

