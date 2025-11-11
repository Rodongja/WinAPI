#include "pch.h"
#include "CCollider.h"

#include "CObject.h"
#include "CCore.h"
#include "CCamera.h"
#include "SelectGDI.h"


UINT CCollider::g_iNextID = 0;

CCollider::CCollider()
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++)
	, m_iCol(0)
	, m_bActive(true)
{
}

// 충돌체 복사생성자는 g_iNextID 충돌을 피하기 위해 깊은 복사를 해야함 m_vFinalPos는 매 프레임마다 다시 계산하기 때문에 생략
CCollider::CCollider(const CCollider& _origin)
	: m_pOwner(nullptr)						// collider를 소유하고 있는 오브젝트
	, m_vOffsetPos(_origin.m_vOffsetPos)	// 오브젝트로부터 상대적인 위치
	, m_vScale(_origin.m_vScale)			// 충돌체의 크기
	, m_iID(g_iNextID++)					// 충돌체 고유한 ID 값
	, m_iCol(0)
	, m_bActive(_origin.m_bActive)
{
} 

CCollider::~CCollider()
{
}

void CCollider::AddTarget(CObject* _Target)
{
	// 이미 리스트에 존재하는지 확인
	if (std::find(m_hTarget.begin(), m_hTarget.end(), _Target) == m_hTarget.end())
	{
		m_hTarget.insert(_Target);
	}
}

void CCollider::finalupdate()
{
	// Object의 위치를 따라감
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;

	assert(0 <= m_iCol); // m_iCol은 0보다 작을 수 없음
}

void CCollider::render(HDC _dc)
{
	//HPEN hGreenPen = CCore::GetInst()->GetPen(PEN_TYPE::GREEN);
	//HPEN hDefaultPen = (HPEN)SelectObject(_dc, hGreenPen);

	//HBRUSH hHollowBrush = CCore::GetInst()->GetBrush(BRUSH_TYPE::HOLLOW);
	//HBRUSH hDefaultBrush = (HBRUSH)SelectObject(_dc, hHollowBrush);

	//Rectangle(_dc
	//	, m_vFinalPos.x - m_vScale.x / 2.f
	//	, m_vFinalPos.y - m_vScale.y / 2.f
	//	, m_vFinalPos.x + m_vScale.x / 2.f
	//	, m_vFinalPos.y + m_vScale.y / 2.f);

	//(HPEN)SelectObject(_dc, hDefaultPen);
	//(HBRUSH)SelectObject(_dc, hDefaultBrush);

	PEN_TYPE ePen = PEN_TYPE::GREEN;

	if (m_iCol)
	{
		ePen = PEN_TYPE::RED;
	}

	SelectGDI pen(_dc, ePen);
	SelectGDI brush(_dc, BRUSH_TYPE::HOLLOW);

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);

		Rectangle(_dc
		, (int)vRenderPos.x - m_vScale.x / 2.f
		, (int)vRenderPos.y - m_vScale.y / 2.f
		, (int)vRenderPos.x + m_vScale.x / 2.f
		, (int)vRenderPos.y + m_vScale.y / 2.f);
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	++m_iCol;
	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	--m_iCol;
	m_pOwner->OnCollisionExit(_pOther);
}

