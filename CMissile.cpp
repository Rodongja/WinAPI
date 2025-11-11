#include "pch.h"
#include "CMissile.h"
#include "CTimeMgr.h"
#include "CCollider.h"

CMissile::CMissile()
	: m_fTheta(M_PI / 2.f) // 이동 방향
	, m_fSpeed(600.f)
	, m_vDir(Vec2(1.f, 1.f))
{
	// 벡터의 정규화 (normalize)
	m_vDir.Normalize();
	CreateCollider();
	GetCollider()->SetScale(Vec2(15.f, 15.f));
}

CMissile::~CMissile()
{
}

void CMissile::update()
{
	Vec2 vPos = GetPos();

	//vPos.x += m_fSpeed * cosf(m_fTheta) * fDT;
	//vPos.y -= m_fSpeed * sinf(m_fTheta) * fDT;

	vPos.x += m_fSpeed * m_vDir.x * fDT;
	vPos.y += m_fSpeed * m_vDir.y * fDT;

	SetPos(vPos);
}

void CMissile::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();
	Ellipse(_dc
		, (int)(vRenderPos.x - vScale.x / 2.f)
		, (int)(vRenderPos.y - vScale.y / 2.f)
		, (int)(vRenderPos.x + vScale.x / 2.f)
		, (int)(vRenderPos.y + vScale.y / 2.f));

	component_render(_dc);
}

void CMissile::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster")
	{
		DeleteObject(this);
	}
}
