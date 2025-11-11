#include "CGravity.h"

#include "CObject.h"
#include "CRigidBody.h"

CGravity::CGravity()
	: m_pOwner(nullptr)
	, m_bGround(false)
	, m_fGravity(1000.f) 
{
}

CGravity::~CGravity()
{
}

void CGravity::SetGround(bool _b)
{
	m_bGround = _b;

	if (true == m_bGround)
	{
		// 땅에 닿았을 때 수직 속도 제거
		Vec2 vVec = m_pOwner->GetRigidBody()->GetVelocity();

		// 만약 플레이어가 하강중이라면 수직 속도를 0으로 리셋
		if (vVec.y > 0.f)
		{
			m_pOwner->GetRigidBody()->SetVelocity(Vec2(vVec.x, 0.f));
		}
	}
	else
	{
		// 공중에 떠있을 때 중력 재적용
		m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, m_fGravity));
	}
}

void CGravity::finalupdate()
{
	//m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, m_fGravity));

	if (true == m_bGround)
	{
		// 땅에 있을 때는 중력 제거
		m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 0.f));
	}
	else
	{
		// 공중에 있을 때만 중력 적용
		m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, m_fGravity));
	}
}