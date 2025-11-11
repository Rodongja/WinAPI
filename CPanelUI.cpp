#include "CPanelUI.h"

#include "CKeyMgr.h"


CPanelUI::CPanelUI(CScene* _pScene)
	: CUI(false, _pScene) // CUI의 기본생성자가 없으므로 반드시 기본값을 설정해줘야함
	, m_pScene(_pScene)
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::update()
{

}

void CPanelUI::render(HDC _dc)
{
	CUI::render(_dc);
}

void CPanelUI::MouseOn()
{
	if (IsLbtnDown())
	{
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		Vec2 vCurPos = GetPos();
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnDown()
{
	m_vDragStart = MOUSE_POS;
}

void CPanelUI::MouseLbtnUp()
{
}