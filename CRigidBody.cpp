#include "pch.h"
#include "CRigidBody.h"

#include "CObject.h"
#include "CTimeMgr.h"



CRigidBody::CRigidBody()
	: m_pOwner(nullptr)
	, m_fMass(1.f)
	, m_fFricCoeff(400.f)
	, m_vMaxVelocity(Vec2(200.f,600.f))
{

}

CRigidBody::~CRigidBody()
{

}

void CRigidBody::finalupdate()
{
	// 힘의 크기
	float fForce = m_vForce.Length();
	Vec2 vForceDir = m_vForce;
	float m_fAccel = 0.f;
	
	if (0.f != fForce)
	{
		// 힘의 방향
		vForceDir.Normalize();

		// 가속도의 크기
		m_fAccel = fForce / m_fMass;
		

		// 가속도
		m_vAccel = m_vForce * m_fAccel;
		//m_vAccel = m_vForce * fForce / m_fMass;
	}

	// 중력 가속도
	m_vAccel += m_vAccelA;

	// 속도
	m_vVelocity += m_vAccel * fDT;

	// 마찰력에 의한 반대방향으로의 가속도 적용
	if (!m_vVelocity.IsZero())
	{
		Vec2 vFircDir = -m_vVelocity;
		vFircDir.Normalize();

		Vec2 vFriction = vFircDir * m_fFricCoeff * fDT;
		float fVelLength = m_vVelocity.Length();

		if (fVelLength <= vFriction.Length())
		{
			// 마찰 가속도가 본래 속도보다 더 큰 경우 역방향으로 밀려나지 않기 위해 0으로 설정
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}
	}

	// 속도 제한 검사
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x) && m_vVelocity.x != 0.f)
	{
		m_vVelocity.x = (m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);
	}	
	
	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y) && m_vVelocity.x != 0.f)
	{
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);
	}

	// 속도에 따른 오브젝트 이동
	Move();

	// 힘 초기화
	m_vForce = Vec2(0.f, 0.f);
	// 가속도 초기화
	m_vAccel = Vec2(0.f, 0.f);
	m_vAccelA = Vec2(0.f,0.f);
}

void CRigidBody::Move()
{
	// 예외처리
	if (m_pOwner == nullptr)
	{
		assert(m_pOwner && "m_pOwner가 nullptr입니다!");
		return;
	}

	// 이동 속력
	float fSpeed = m_vVelocity.Length();

	if (0.f != fSpeed)
	{
		// 이동 방향
		Vec2 vDir = m_vVelocity;
		vDir.Normalize();

		// m_vVelocty = vDir * fSpeed

		Vec2 vPos = m_pOwner->GetPos();

		vPos += m_vVelocity * fDT;

		m_pOwner->SetPos(vPos);
	}
}