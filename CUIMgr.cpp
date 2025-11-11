#include "pch.h"
#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CUI.h"

#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	: m_pFocusedUI(nullptr)
{

}
CUIMgr::~CUIMgr()
{

}

void CUIMgr::update()
{
	// 1. FocusedUI 확인
	m_pFocusedUI = GetFocusedUI();

	if (!m_pFocusedUI)
	{
		return;
	}

	// 2. FocusedUI 들 중 실제 타게팅된 UI를 가져온다
	CUI* pTargetUI = GetTargetUI(m_pFocusedUI);

	bool bLbtnTap = KEY_TAP(KEY::LBTN);
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	// GetTargetUI에 등록된 UI라면 이미 마우스가 올라와있으니 IsMouseOn 조건체크는 안해도 됨
	if (nullptr != pTargetUI)
	{
		pTargetUI->MouseOn();

		if (bLbtnTap)
		{
			pTargetUI->MouseLbtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAway)
		{
			pTargetUI->MouseLbtnUp();

			// GetTargetUI에서 이미 m_bLbtnDown가 false로 설정되어있어서 조건체크에 걸리지 않는 오류가 있음
			if (pTargetUI->m_bLbtnDown)
			{
				pTargetUI->MouseLbtnClicked();
			}
			// 왼쪽버튼을 떼어서 체크 해제
			pTargetUI->m_bLbtnDown = false;
		}
	}

	/*//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	//// vecUI == 부모 UI (최상위 UI)
	//const vector<CObject*>& vecUI = pCurScene->GetGroupObject(GROUP_TYPE::UI);

	//for (size_t i = 0; i < vecUI.size(); ++i)
	//{
	//	// vecUI는 CObject*이므로 CUI*로 다운캐스팅 해야함
	//	CUI* pUI = (CUI*)vecUI[i];

	//	// 마우스를 올리고있는 최하계층의 UI 주소 받아오기
	//	// UI 들 중 실제 타게팅된 UI를 가져온다
	//	pUI = GetTargetUI(pUI);

	//	// GetTargetUI에 등록된 UI라면 이미 마우스가 올라와있으니 IsMouseOn 조건체크는 안해도 됨
	//	if (nullptr != pUI)
	//	{
	//		pUI->MouseOn();

	//		if (bLbtnTap)
	//		{
	//			pUI->MouseLbtnDown();
	//			pUI->m_bLbtnDown = true;
	//		}
	//		else if (bLbtnAway)
	//		{
	//			pUI->MouseLbtnUp();

	//			// GetTargetUI에서 이미 m_bLbtnDown가 false로 설정되어있어서 조건체크에 걸리지 않는 오류가 있음
	//			if (pUI->m_bLbtnDown)
	//			{
	//				pUI->MouseLbtnClicked();
	//			}
	//			// 왼쪽버튼을 떼어서 체크 해제
	//			pUI->m_bLbtnDown = false;
	//		}
	//	}

	//	//// 마우가 UI 안에 있을때 확인
	//	//if (pUI->IsMouseOn())
	//	//{
	//	//	pUI->MouseOn();
	//	//	
	//	//	if (bLbtnTap)
	//	//	{				
	//	//		pUI->MouseLbtnDown();
	//	//		pUI->m_bLbtnDown = true;
	//	//	}
	//	//	else if (bLbtnAway)
	//	//	{
	//	//		pUI->MouseLbtnUp();
	//	//		if (pUI->m_bLbtnDown)
	//	//		{
	//	//			pUI->MouseLbtnClicked();
	//	//		}
	//	//		// 왼쪽버튼을 떼어서 체크 해제
	//	//		pUI->m_bLbtnDown = false;
	//	//	}
	//	//}
	//	//// 마우스가 UI 밖에 있을때 확인
	//	//else
	//	//{
	//	//	if (bLbtnAway)
	//	//	{
	//	//		// 왼쪽버튼을 떼어서 체크 해제
	//	//		pUI->m_bLbtnDown = false;
	//	//	}
	//	//}
	//}*/
}

void CUIMgr::SetFocusedUI(CUI* _pUI)
{
	// 포커스된 UI를 포커싱하는 단축키를 눌렀거나 nullptr을 전달해 포커스를 해제한 경우
	if (m_pFocusedUI == _pUI || nullptr == _pUI)
	{
		m_pFocusedUI = _pUI;
		return;
		// 포커스된 UI를 포커싱하는 단축키를 눌렀을 경우에 포커스된 창을 닫아야하지 않을까?
		// 예를들어 인벤토리 창이 포커싱 되어있는데(열려있는데) I키를 누르면 인벤토리가 닫히는것처럼
	}

	m_pFocusedUI = _pUI;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();

	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (m_pFocusedUI == *iter)
		{
			break;
		}
	}

	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);
}

CUI* CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();

	bool bLbtnTap = KEY_TAP(KEY::LBTN);

	// 기존 포커싱 UI를 받아두고 변경되었는지 확인
	CUI* pFocusedUI = m_pFocusedUI;

	if (!bLbtnTap)
	{
		return m_pFocusedUI;
	}
	
	// 왼쪽버튼 TAP이 발생햇다는 전제
	vector<CObject*>::iterator targetiter = vecUI.end();
	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetiter = iter;
		}
	}

	// 이번에 Focus된 UI가 없는 경우
	if (vecUI.end() == targetiter)
	{
		return nullptr;
	}
	
	pFocusedUI = (CUI*)*targetiter;

	// 백터내에서 맨 뒤로 순번교체
	vecUI.erase(targetiter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;

	// 그런데 A,B,C UI가 있고 A,B,C의 순서는 B->A->C일때, A와 B가 겹친곳을 누르면 C->B->A 순서대로 렌더링이 되는게 아니고  
}

CUI* CUIMgr::GetTargetUI(CUI* _pParentUI)	// 부모 UI 내에서 실제로 타겟팅 된 UI를 찾아서 반환하는 함수
{
	// 1. 부모 UI 포함, 모든 자식들을 검사한다.
	// 2. 타겟 UI 중 우선순위가 높은 기준은 더 낮은 계층의 자식 UI (같은 계층의 경우는 UI가 겹쳣다는 뜻이므로 구조적 오류임)
	// 3. queue를 사용해 레벨순회를 해야함. 

	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	CUI* pTargetUI = nullptr;

	// 큐 역할을 할 리스트 선언
	// 자주 사용할 변수이므로 static 선언
	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;

	// static이므로 clear 해야함
	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);

	// queue에 아무것도 없을때까지 반복
	while (!queue.empty())
	{
		// 큐에서 데이터 하나 꺼내기
		CUI* pUI = queue.front();

		// 꺼낸 데이터 지우기
		queue.pop_front();

		// 큐에서 꺼낸 UI가 TargetUI인지 확인
		// 타겟 UI 중 우선순위가 높은 기준은 더 낮은 계층의 자식 UI
		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)
			{
				vecNoneTarget.push_back(pTargetUI);
			}

			pTargetUI = pUI;
		}
		else
		{
			vecNoneTarget.push_back(pUI);
		}
		// queue에 자식 데이터 추가
		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}	

	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); ++i)
		{
			// 마우스가 UI 밖에 있을때 확인
			// 왼쪽버튼을 떼어서 체크 해제
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}
