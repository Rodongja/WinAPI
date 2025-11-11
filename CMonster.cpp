#include "pch.h"
#include "CMonster.h"

#include "CTimeMgr.h"
#include "CMissile.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"

#include "AI.h"

#include <windows.h>
#include "CRandomMgr.h"
#include "CCollider.h"

CMonster::CMonster()
	: m_tInfo{}
	//, m_pAI(nullptr)

	//: m_vCenterPos(Vec2(0.f,0.f))
	//, m_fSpeed(100.f)
	////, m_fMaxDistance(50.f)
	////, m_iDir(1)
	//, m_iHP(5)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(40.f, 40.f));
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
	{
		delete m_pAI;
	}
}

void CMonster::update()
{
	if (nullptr != m_pAI)
	{
		m_pAI->update();
	}	
}

void CMonster::CreateMissile()
{
	// 미사일을 발사합니다.
	FireMissile(Vec2(0.f, 1.f));  // 아래쪽 방향
}

void CMonster::FireMissile(const Vec2& dir)
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	CMissile* pMissile = new CMissile;
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(25.f, 25.f));
	pMissile->SetDir(dir);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);
}


//void CMonster::update()
//{
//	Vec2 vCurPos = GetPos();
//
//	// 진행 방향으로 시간당 m_fSpeed만큼 이동
//	//vCurPos.x += fDT * m_fSpeed * m_iDir;
//
//	float fDist = abs(m_vCenterPos.x - vCurPos.x) - m_fMaxDistance;
//
//	// 배회 거리 기준량을 넘어섰는지 확인(절대값)
//	if (0.f < fDist)
//	{
//		// 방향 변경
//		m_iDir *= -1;
//		vCurPos.x += fDist * m_iDir;
//	}
//
//	static float elapsedTime = 0.0f;
//	int wait = 0;
//	wait = CRandomMgr::GetInst()->GetRd(5,7);
//
//	elapsedTime += fDT;
//	if (elapsedTime >= wait) // 대기
//	{
//		elapsedTime = 0.0f; // 누적 시간 리셋
//		// 카메라를 움직여도 같은자리에서 발사체가 나가는 현상은 Pos를 RealPos로 수정(오브젝트, 충돌체 등)하고 렌더함수는 RenderPos로 해야하지 않을까?
//		CreateMissile();
//	}
//
//	SetPos(vCurPos);
//
//	m_vCenterPos;
//}

//void CMonster::update()
//{
//	return;
//	Vec2 vCurPos = GetPos();
//
//	// 진행 방향으로 시간당 m_fSpeed만큼 이동
//	vCurPos.x += fDT * m_fSpeed * m_iDir;
//
//	// 배회 거리 기준량을 넘어섰는지 확인(절대값)
//	float fDist = abs(m_vCenterPos.x - vCurPos.x) - m_fMaxDistance;
//	if (0.f < fDist)
//	{
//		// 방향 변경
//		m_iDir *= -1;
//		vCurPos.x += fDist * m_iDir;
//	}
//
//	SetPos(vCurPos);
//}

void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	m_pAI->m_pOwner = this;
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	
	if (pOtherObj->GetName() == L"Missile_Player")
	{
		//m_iHP -= 1;
		//if (m_iHP <= 0)
		//{
		//	DeleteObject(this);
		//}
	}

}