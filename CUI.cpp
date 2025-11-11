#include "CUI.h"

#include "CKeyMgr.h"
#include "CCamera.h"

#include "SelectGDI.h"

#include "algorithm"
#include "CScene.h"
#include "CSceneMgr.h"


// 기본 생성자를 만들지 않아 반드시 _bCamAff 인자의 값을 명시해야함
CUI::CUI(bool _bCamAff, CScene* _pScene)
	: m_pParentUI(nullptr)
	, m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
	, m_pScene(_pScene)
{
}

// 깊은복사 (CLONE)
CUI::CUI(const CUI& _origin)
	: CObject(_origin) // 부모클래스를 상속하는 자식클래스의 복사생성자를 직접 만들면(ex) 깊은복사) 반드시 상속받은 부모클래스(CObject) 부분의 복사생성자를 명시적으로 지정해줘야함.
	, m_pParentUI(nullptr)
	, m_bCamAffected(_origin.m_bCamAffected)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, m_pScene(_origin.m_pScene)
{
	for (size_t i = 0; i < _origin.m_vecChildUI.size(); ++i)
	{
		AddChild(_origin.m_vecChildUI[i]->Clone());
	}
}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}

void CUI::update()
{

	// 자식 UI 업데이트는 마지막에 해야함
	update_child();
}

void CUI::finalupdate()
{
	CObject::finalupdate();

	// UI의 최종 좌표 계산
	// 부모가 없으면 자신의 오프셋 포지션이 곧 최종 위치
	m_vFinalPos = GetPos();

	if (GetParent())
	{
		// 부모가 있으면 부모의 포지션 + 오프셋 포지션
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}

	// UI Mouse On 체크
	MouseOnCheck();

	finalupdate_child();
}

void CUI::render(HDC _dc)
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	// 카메라 영향을 받는 UI인경우
	if (m_bCamAffected)
	{
		// 카메라 위치 반영
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	}

	if (m_bLbtnDown)
	{
		SelectGDI select(_dc, PEN_TYPE::GREEN);	
		
		Rectangle(
			_dc
			, int(vPos.x)
			, int(vPos.y)
			, int(vPos.x + vScale.x)
			, int(vPos.y + vScale.y)
		);
	}
	else
	{
		// 움직이지 않는 UI 생성
		Rectangle(
			_dc
			, int(vPos.x)
			, int(vPos.y)
			, int(vPos.x + vScale.x)
			, int(vPos.y + vScale.y)
		);
	}



	// CUI.h는 추상클래스로 구현하여 직접 구현하지 않고 하위 클래스를 추가로 생성하여 구현할 예정
	// 움직이는 UI를 생성하려면 절대값이 아닌 실제 좌표값을 추가로 CMoveUI.h를 만들어서 오버라이딩 하여 구현하는게 좋음 ex) hp바, 표지판 방향지시UI

	// 자식 UI 렌더는 마지막에 해야함
	render_child(_dc);
}

void CUI::update_child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->update();
	}
}

void CUI::finalupdate_child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->finalupdate();
	}
}

void CUI::render_child(HDC _dc)
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->render(_dc);
	}
}

void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
	}

	// UI 위에 마우스가 있는지 확인
	if (m_vFinalPos.x <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + vScale.x
		&& m_vFinalPos.y <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + vScale.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CUI::MouseOn()
{
}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}

//void CUI::DeleteSelfAndAllChildUI()
//{
//	//// 1. 모든 자식 UI 삭제
//	//for (int i = 0; i < m_vecChildUI.size() ;)
//	//{
//	//	if (nullptr != m_vecChildUI[i])
//	//	{
//	//		m_vecChildUI[i]->DeleteSelfAndAllChildUI();  // 재귀적으로 자식의 자식도 삭제
//	//	}
//	//	else
//	//	{
//	//		++i;
//	//	}
//	//}
//
//	//// 자식 UI 벡터 비움
//	//m_vecChildUI.clear();
//
//	//// 2. 부모 UI에서 현재 객체를 자식 목록에서 제거
//	//if (nullptr != m_pParentUI) 
//	//{
//	//	// 자식 벡터의 참조를 가져오기
//	//	vector<CUI*>& vChild = m_pParentUI->GetChildUINonConst();
//
//	//	for (int j = 0; j < vChild.size();)
//	//	{
//	//		if (this == vChild[j])
//	//		{
//	//			vChild.erase(vChild.begin() + j);
//	//			break;  // 삭제 후 반복 종료
//	//		}
//	//		else
//	//		{
//	//			++j;
//	//		}
//	//	}
//	//}
//
//
//
//	//// 3. 최종적으로 자신 삭제
//	//delete this;
//}

template <typename T>
void safeDelete(T*& ptr)
{
	if (ptr)
	{
		// 삭제 전에 cleanup 작업을 진행
		ptr->DeleteThisUI();
		ptr = nullptr;
	}
}

// 수정된 DeleteAllUI: 부모 UI가 존재하면 부모를, 없으면 자기 자신을 safeDelete로 삭제
void CUI::DeleteAllUI()
{
	if (m_pParentUI)
	{
		// 부모 UI 삭제: 부모가 내부적으로 자식도 삭제하도록 구성되어 있다면
		// UI 매니저나 해당 컨테이너에서 m_pParentUI에 대한 포인터를 safeDelete로 처리
		safeDelete(m_pParentUI);
	}
	else
	{
		delete this;
		// ※ 주의: this를 직접 safeDelete로 삭제하는 경우,
		// 이 함수를 호출한 쪽에서 이미 보관 중인 포인터가 있다면 그곳도 nullptr로 설정해야 함
	}
}

// 수정된 DeleteThisUI: cleanup 작업만 수행하고 실제 메모리 해제는 safeDelete()에서 진행
void CUI::DeleteThisUI()
{
	// 1. 재귀적으로 자식 UI 삭제
	DeleteAllChildrenUI();

	// 2. 버튼 콜백 해제
	// (버튼 콜백 관련 해제 코드가 있다면 여기에 추가)

	// 3. 부모 UI가 있으면 부모의 자식 목록에서 제거
	if (m_pParentUI)
	{
		m_pParentUI->RemoveChild(this);
	}

	// 4. 씬의 객체 컨테이너에서 제거 (댕글링 포인터 방지를 위해)
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	if (pScene)
	{
	pScene->RemoveObject(this);
	}
}

/*
void CUI::DeleteAllUI()
{
	// 부모 UI가 존재 하면
	if (m_pParentUI)
	{
		m_pParentUI->DeleteThisUI();  // 부모 UI 삭제
	}
	// 부모 UI가 존재하지 않으면
	else
	{
		DeleteThisUI();
	}
}

void CUI::DeleteThisUI()
{
	// 1. 재귀함수로 자식 UI 삭제
	DeleteAllChildrenUI();

	// 2. 버튼 콜백 해제


	// 3. 부모 UI가 있으면 연결 해제
	if (m_pParentUI)
	{
		m_pParentUI->RemoveChild(this);
	}

	// 4. 씬의 객체 컨테이너에서 제거 (댕글링 포인터 방지)
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	if (pScene)
	{
		pScene->RemoveObject(this);
	}

	// 5. 안전하게 객체 메모리 해제
	delete this;
}
*/

//void CUI::DeleteAllUI()
//{
//	// 1. 재귀함수로 자식 UI 삭제
//	DeleteAllChildrenUI();
//
//	// 2. 버튼 콜백 해제
//
//
//	// 3. 포인터 null로 초기화
//
//
//	// 4. delete
//
//	// 5. 부모에서 자식 포인터 null로 초기화
//}

void CUI::DeleteAllChildrenUI()
{	
	// 자식 리스트를 순회하면서 각 자식을 재귀적으로 삭제

	// 재귀함수로 자식UI 순회
	for (int i = 0; i < m_vecChildUI.size();)
	{
		if (nullptr != m_vecChildUI[i])
		{
			m_vecChildUI[i]->DeleteAllChildrenUI();  // 재귀적으로 자식의 자식도 삭제

			// 자식 삭제 후 nullptr로 초기화
			delete m_vecChildUI[i];
			m_vecChildUI[i] = nullptr;

			// nullptr이 된 자식 UI를 벡터에서 삭제
			m_vecChildUI.erase(m_vecChildUI.begin() + i);
		}
		else
		{
			++i;
		}
	}
}

void CUI::RemoveChild(CUI* _pChild)
{
	vector<CUI*>::iterator iter = find(m_vecChildUI.begin(), m_vecChildUI.end(), _pChild);
	if (iter != m_vecChildUI.end())
	{
		delete *iter;
		m_vecChildUI.erase(iter);
	}
}
