#include "pch.h"
#include "CObject.h"

//#include "CKeyMgr.h" 
//
//#include "CTimeMgr.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CGravity.h"
#include "CPlayer.h"

CObject::CObject()
	: m_vPos{}
	, m_vScale{}
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_bAlive(true)	
{
}

CObject::CObject(const CObject& _origin)
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_bAlive(true)
{
	// 충돌체는 그대로 원본의 주소를 복사하면 새로 만든 충돌체가 원본을 가리키므로 직접 생성해줘야함
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}

	// Animator 깊은복사
	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}

	if (_origin.m_pRigidBody)
	{
		m_pRigidBody = new CRigidBody(*_origin.m_pRigidBody);
		m_pRigidBody->m_pOwner = this;
	}

	if (_origin.m_pGravity)
	{
		m_pGravity = new CGravity(*_origin.m_pGravity);
		m_pGravity->m_pOwner = this;
	}
}

CObject::~CObject()
{
	if (nullptr != m_pCollider)
	{
		delete m_pCollider;
	}
	if (nullptr != m_pAnimator)
	{
		delete m_pAnimator;
	}
	if (nullptr != m_pRigidBody)
	{
		delete m_pRigidBody;
	}
	if (nullptr != m_pGravity)
	{
		delete m_pGravity;
	}
}

// CCore가 update(혹은 render)를 하면 SceneMgr의 함수를 호출하고
// SceneMgr에서 호출하면 이어서 Scene에서 호출하고
// Scene은 이어서 Object를 호출하고
// 마지막으로 object에서 update와 render를 실행

//void CObject::render(HDC _dc)
//{
//	// vPos와 vScale은 Object가 생성되면서 세팅됨. (CScene_Start.cpp의 Enter함수)
//	Rectangle(_dc
//		, (int)(m_vPos.x - m_vScale.x / 2.f)
//		, (int)(m_vPos.y - m_vScale.y / 2.f)
//		, (int)(m_vPos.x + m_vScale.x / 2.f)
//		, (int)(m_vPos.y + m_vScale.y / 2.f));
//}


void CObject::finalupdate()
{
	if (m_pAnimator)
	{
		m_pAnimator->finalupdate();
	}

	if (m_pGravity)
	{
		m_pGravity->finalupdate();
	}
	
	if (m_pRigidBody)
	{
		m_pRigidBody->finalupdate();
	}

	if (m_pCollider)
	{
		m_pCollider->finalupdate();
	}
}

void CObject::render(HDC _dc)
{
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	// vPos와 vScale은 Object가 생성되면서 세팅됨. (CScene_Start.cpp의 Enter함수)
	Rectangle(_dc
		, (int)(vRenderPos.x - m_vScale.x / 2.f)
		, (int)(vRenderPos.y - m_vScale.y / 2.f)
		, (int)(vRenderPos.x + m_vScale.x / 2.f)
		, (int)(vRenderPos.y + m_vScale.y / 2.f));

	component_render(_dc);
}
 
void CObject::component_render(HDC _dc)
{
	if (nullptr != m_pAnimator)
	{
		m_pAnimator->render(_dc);
	}
	if (nullptr != m_pCollider)
	{
		m_pCollider->render(_dc);
	}
}

void CObject::RemeberThisObject()
{

}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void CObject::CreateRigidBody()
{
	m_pRigidBody = new CRigidBody;
	m_pRigidBody->m_pOwner = this;
}

void CObject::CreateGravity()
{
	m_pGravity = new CGravity;
	m_pGravity->m_pOwner = this;
}