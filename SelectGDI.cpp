#include "pch.h"
#include "SelectGDI.h"
#include "CCore.h"

SelectGDI::SelectGDI(HDC _dc, PEN_TYPE _ePenType)
	: m_hDC(_dc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{	
	HPEN hGreenPen = CCore::GetInst()->GetPen(_ePenType);
	m_hDefaultPen = (HPEN)SelectObject(_dc, hGreenPen);
}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE _eBrushType)
	: m_hDC(_dc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HBRUSH hHollowBrush = CCore::GetInst()->GetBrush(_eBrushType);
	m_hDefaultBrush = (HBRUSH)SelectObject(_dc, hHollowBrush);
}

SelectGDI::~SelectGDI()
{
	(HPEN)SelectObject(m_hDC, m_hDefaultPen);
	(HBRUSH)SelectObject(m_hDC, m_hDefaultBrush);
}
