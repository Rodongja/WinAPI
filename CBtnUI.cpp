#include "CBtnUI.h"

CBtnUI::CBtnUI(CScene* _pScene)
	: CUI(false, _pScene)
	, m_param1(0)
	, m_param2(0)
	, m_pSceneInst(_pScene)
	, m_pSceneFunc(nullptr)
	, m_pUIInst(nullptr)
	, m_pUIFunc(nullptr)
{
}

CBtnUI::~CBtnUI()
{
}

void CBtnUI::MouseOn()
{
}

void CBtnUI::MouseLbtnDown()
{
}

void CBtnUI::MouseLbtnUp()
{
}

void CBtnUI::MouseLbtnClicked()
{
	if (nullptr != m_pFunc)
	{
		m_pFunc(m_param1,m_param2);
	}

	if (m_pSceneInst && m_pSceneFunc)
	{
		((*m_pSceneInst).*m_pSceneFunc)();
	}

	if (m_pUIInst && m_pUIFunc)
	{
		((*m_pUIInst).*m_pUIFunc)();
	}
}

void CBtnUI::SetClickedCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2)
{
	m_pFunc = _pFunc;
	m_param1 = _param1;
	m_param2 = _param2;
}

void CBtnUI::SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc)
{
	m_pSceneInst = _pScene;
	m_pSceneFunc = _pSceneFunc;
}

void CBtnUI::SetClickedCallBack(CUI* _pUI, UI_MEMFUNC _pUIFunc)
{
	m_pUIInst = _pUI;
	m_pUIFunc = _pUIFunc;
}
