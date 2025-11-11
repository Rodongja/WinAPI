#include "pch.h"
#include "CScene_Start.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CCore.h"

#include "CPathMgr.h"
#include "CTexture.h"

#include "CCollisionMgr.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CCamera.h"

#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"

#include "CRigidBody.h"
#include "SelectGDI.h"
#include "CTimeMgr.h"

#include "CGround.h"

CScene_Start::CScene_Start()
	: m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(1000.f)
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::update()
{
	if (KEY_HOLD(KEY::LBTN))
	{
		m_bUseForce = true;
		CreateForce();
	}
	else
	{
		m_bUseForce = false;
	}
 
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		const vector<CObject*>& vecObj = GetGroupObject((GROUP_TYPE)i);

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (!vecObj[j]->IsDead())
			{
				if (m_bUseForce && vecObj[j]->GetRigidBody())
				{
					Vec2 vDiff = vecObj[j]->GetPos() - m_vForcePos;
					float fLen = vDiff.Length();
					if (fLen < m_fForceRadius)
					{ 
						float fRatio = 1.f - (fLen / m_fForceRadius);
						float fForce = m_fForce* fRatio;

						vecObj[j]->GetRigidBody()->AddForce(vDiff.Normalize() * fForce);
					}
				}
				vecObj[j]->update();
			}			
		}
	}

	if (KEY_TAP(KEY::P))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}

	//if (KEY_TAP(KEY::LBTN))
	//{
	//	// 우리가 클릭한 마우스의 좌표는 렌더링 좌표이기 때문에 실제 좌표값으로 계산해줘야함
	//	Vec2 vLookAt = CCamera::GetInst()->GetRealPos(MOUSE_POS);
	//	CCamera::GetInst()->SetLookAt(vLookAt);
	//}
}
void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);

	if (!m_bUseForce)
	{
		return;
	}

	SelectGDI gdi1(_dc, BRUSH_TYPE::HOLLOW);
	SelectGDI gdi2(_dc, PEN_TYPE::GREEN);

	m_fCurRadius += m_fForceRadius * 3.f * fDT;
	if (m_fCurRadius > m_fForceRadius)
	{
		m_fCurRadius = 0.f;
	}

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vForcePos);

	Ellipse(_dc
		, (int)(vRenderPos.x - m_fCurRadius)
		, (int)(vRenderPos.y - m_fCurRadius)
		, (int)(vRenderPos.x + m_fCurRadius)
		, (int)(vRenderPos.y + m_fCurRadius));
}


void CScene_Start::Enter()
{
	// Object 추가
	CObject* pObj = new CPlayer;
	
	// 부모멤버여도 private에는 접근 불가능
	// 그러므로 protected 사용
	// 하지만 디버깅이 힘들어짐
	// 부모멤버는 private를 유지한채로 부모클래스에서 protected 함수를 만드는게 더 좋을수도 있음
	//m_arrObj[(UINT)GROUP_TYPE::DEFAULT].push_back(pObj); 

	// 추가는 자식클래스에서 하지만 삭제는 코드 재사용(상속)을 위하여 부모클래스인 CScene.cpp에서 함
	pObj->SetName(L"Player");
	pObj->SetPos(Vec2(640.f,384.f));
	pObj->SetScale(Vec2(20.f,40.f));

	// 함수버전
	AddObject(pObj, GROUP_TYPE::PLAYER);

	RegisterPlayer(pObj);

	// 카메라 플레이어 오브젝트 추적
	//CCamera::GetInst()->SetTarget(pObj);

	//// 또 다른 플레이어 생성
	//CObject* pOtherPlayer = new CPlayer(*(CPlayer*)pObj);
	//pObj->SetPos(Vec2(740.f, 384.f));
	//AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);

	//CObject* pOtherPlayer = pObj->Clone();
	//pOtherPlayer->SetPos(Vec2(740.f, 380.4f));
	//AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);

	//// 몬스터 배치 
	//int iMonCount = 1;
	////float fMoveDist = 25.f;
	//float fObjScale = 50.f;

	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	////float fTerm = (vResolution.x - ((fMoveDist + fObjScale / 2.f) * 2)) / (float)(iMonCount - 1);

	//AI* pAI = new AI;
	//pAI->AddState(new CIdleState);
	//pAI->AddState(new CTraceState);

	//CMonster* pMonsterObj = nullptr;


	//for (int i = 0; i < iMonCount; ++i)
	//{
	//	// Monster Object 추가
	//	pMonsterObj = new CMonster;
	//	pMonsterObj->SetName(L"Monster");
	//	//pMonsterObj->SetPos(Vec2((fMoveDist + fObjScale / 2.f) + (float)i * fTerm, 50.f));

	//	//pMonsterObj->SetCenterPos(pMonsterObj->GetPos());
	//	//pMonsterObj->SetMoveDistance(fMoveDist);
	//	pMonsterObj->SetScale(Vec2(fObjScale, fObjScale));

	//	pMonsterObj->SetPos(vResolution / 2.f - Vec2(0.f, 300.f));
	//	pMonsterObj->SetAI(pAI);


	//	AddObject(pMonsterObj, GROUP_TYPE::MONSTER);
	//}

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	//// 몬스터 배치 
	//// 씬이 시작할 때 배치하는것이니 Enter에서 배치하지만
	//// 씬이 시작하고 난 뒤에는 이벤트 매니저를 통해 배치해야함
	//CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	//AddObject(pMon, GROUP_TYPE::MONSTER);
	////CreateObject(pMon, GROUP_TYPE::MONSTER);

	// 땅 물체 배치
	CObject* pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(640.f, 584.f));
	pGround->SetScale(Vec2(1000.f, 60.f));
	AddObject(pGround, GROUP_TYPE::GROUND);

	pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(740.f, 400.f));
	pGround->SetScale(Vec2(300.f, 50.f));
	AddObject(pGround, GROUP_TYPE::GROUND);

	pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(1040.f, 470.f));
	pGround->SetScale(Vec2(60.f, 200.f));
	AddObject(pGround, GROUP_TYPE::GROUND);

	pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(340.f, 470.f));
	pGround->SetScale(Vec2(60.f, 200.f));
	AddObject(pGround, GROUP_TYPE::GROUND);

	pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(640.f, 200.f));
	pGround->SetScale(Vec2(100.f, 100.f));
	AddObject(pGround, GROUP_TYPE::GROUND);

	pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(590.f, 500.f));
	pGround->SetScale(Vec2(50.f, 50.f));
	AddObject(pGround, GROUP_TYPE::GROUND);

	// 타일 로딩
	//LoadTile(L"Tile\\Start.tile");

	// 충돌 지정
	// PLAYER 그룹과 MONSTER 그룹 간의 충돌체크
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);

	// Camera Look 지정
	//CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	CCamera::GetInst()->SetLookAt(pObj->GetPos());

	// Camera 효과 지정
	CCamera::GetInst()->FadeOut(1.f);
	CCamera::GetInst()->FadeIn(1.f);

	start();
}


void CScene_Start::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}

void CScene_Start::CreateForce()
{
	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
}
