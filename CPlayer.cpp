#include "pch.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CMissile.h"
#include "CResourceMgr.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "CRigidBody.h"
#include "CGravity.h"

CPlayer::CPlayer()
	: MissileType(1)
	, m_hTarget()
	, m_iDir(1)
	, m_iPrevDir(1)
	, m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_fSpeed(1000.f)
	, m_fJumpPower(500.f)
	, m_iMaxJump(2)
	, m_iJumpCnt(0)
	, m_vTextureOffset(0.f,-11.f)
	//, m_pTex(nullptr)
{
	//// Texture 로딩하기
	//m_pTex = new CTexture;

	//wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	//strFilePath += L"texture\\Player.bmp";
	//m_pTex->Load(strFilePath);

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(20.f, 40.f));

	CreateRigidBody();

	// Texture 로딩
	CTexture* pTex = CResourceMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\Animation\\link1.bmp");
	
	//m_pTex = CResourceMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\Animation\\new_link.bmp");

	CreateAnimator();


 	GetAnimator()->LoadAnimation(L"animation\\player_idle_down.anim", m_vTextureOffset);
	GetAnimator()->LoadAnimation(L"animation\\player_idle_left.anim", m_vTextureOffset);
	GetAnimator()->LoadAnimation(L"animation\\player_idle_up.anim", m_vTextureOffset);
	GetAnimator()->LoadAnimation(L"animation\\player_idle_right.anim", m_vTextureOffset);

	GetAnimator()->LoadAnimation(L"animation\\player_walk_down.anim", m_vTextureOffset);
	GetAnimator()->LoadAnimation(L"animation\\player_walk_left.anim", m_vTextureOffset);
	GetAnimator()->LoadAnimation(L"animation\\player_walk_up.anim", m_vTextureOffset);
	GetAnimator()->LoadAnimation(L"animation\\player_walk_right.anim", m_vTextureOffset);

	//GetAnimator()->CreateAnimation(L"IDLE_DOWN", pTex, Vec2(0.f, 0.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.5, 3);
	//GetAnimator()->CreateAnimation(L"IDLE_LEFT", pTex, Vec2(0.f, 65.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.5, 3);
	//GetAnimator()->CreateAnimation(L"IDLE_UP", pTex, Vec2(0.f, 130.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.5, 1);
	//GetAnimator()->CreateAnimation(L"IDLE_RIGHT", pTex, Vec2(0.f, 195.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.5, 3);

	//GetAnimator()->CreateAnimation(L"WALK_DOWN", pTex, Vec2(0.f, 260.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.2f, 10);
	//GetAnimator()->CreateAnimation(L"WALK_LEFT", pTex, Vec2(0.f, 325.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.2f, 10);
	//GetAnimator()->CreateAnimation(L"WALK_UP", pTex, Vec2(0.f, 390.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.2f, 10);
	//GetAnimator()->CreateAnimation(L"WALK_RIGHT", pTex, Vec2(0.f, 455.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.2f, 10);

	////GetAnimator()->Play(L"IDLE_DOWN", true);

	//// Animation 저장해보기
	//GetAnimator()->FindAnimation(L"IDLE_DOWN")->Save(L"animation\\player_idle_down.anim");
	//GetAnimator()->FindAnimation(L"IDLE_LEFT")->Save(L"animation\\player_idle_left.anim");
	//GetAnimator()->FindAnimation(L"IDLE_UP")->Save(L"animation\\player_idle_up.anim");
	//GetAnimator()->FindAnimation(L"IDLE_RIGHT")->Save(L"animation\\player_idle_right.anim");

	//GetAnimator()->FindAnimation(L"WALK_DOWN")->Save(L"animation\\player_walk_down.anim");
	//GetAnimator()->FindAnimation(L"WALK_LEFT")->Save(L"animation\\player_walk_left.anim");
	//GetAnimator()->FindAnimation(L"WALK_UP")->Save(L"animation\\player_walk_up.anim");
	//GetAnimator()->FindAnimation(L"WALK_RIGHT")->Save(L"animation\\player_walk_right.anim");

	//// 오프셋 설정
	//CAnimation* pAnim = GetAnimator()->FindAnimation(L"IDLE_LEFT");

	//for (UINT i = 0; i < pAnim->GetMaxFrame(); ++i)
	//{
	//	pAnim->GetFrame(i).vOffset = Vec2(0.f, -20.f);
	//}

	m_iJumpCnt = m_iMaxJump;

	CreateGravity();
}

CPlayer::~CPlayer()
{
	//if (nullptr != m_pTex)
	//{
	//	delete m_pTex;
	//}
}

void CPlayer::update()
{
	// Vec 버전
	//Vec2 vPos = GetPos();


	//if (KEY_HOLD(KEY::W))
	//{
	//	vPos.y -= 200.f * fDT;
	//}

	//if (KEY_HOLD(KEY::S))
	//{
	//	vPos.y += 200.f * fDT;
	//}

	//if (KEY_HOLD(KEY::A))
	//{
	//	vPos.x -= 200.f * fDT;
	//}

	//if (KEY_HOLD(KEY::D))
	//{
	//	vPos.x += 200.f * fDT;
	//}

	//if (KEY_TAP(KEY::SPACE))
	//{
	//	CreateMissile();
	//}

	//if (KEY_TAP(KEY::_1))
	//{
	//	SetMissileType(1);
	//}

	//if (KEY_TAP(KEY::_2))
	//{
	//	SetMissileType(2);
	//}
	//
	//SetPos(vPos);

	//GetAnimator()->update();

	//// RigidBody 버전

	//CRigidBody* pRigid = GetRigidBody();

	//if (KEY_HOLD(KEY::W))
	//{
	//	pRigid->AddForce(Vec2(0.f, -200.f));
	//}

	//if (KEY_HOLD(KEY::S))
	//{
	//	pRigid->AddForce(Vec2(0.f, 200.f));
	//}

	//if (KEY_HOLD(KEY::A))
	//{
	//	pRigid->AddForce(Vec2(-200.f, 0.f));
	//}

	//if (KEY_HOLD(KEY::D))
	//{
	//	pRigid->AddForce(Vec2(200.f, 0.f));
	//}
	//if (KEY_TAP(KEY::W))
	//{
	//	pRigid->AddForce(Vec2(0.f, -100.f));
	//}

	//if (KEY_TAP(KEY::S))
	//{
	//	pRigid->AddForce(Vec2(0.f, 100.f));
	//}

	//if (KEY_TAP(KEY::A))
	//{
	//	pRigid->AddForce(Vec2(-100.f, 0.f));
	//}

	//if (KEY_TAP(KEY::D))
	//{
	//	pRigid->AddForce(Vec2(100.f, 0.f));
	//}

	// 이전 상태 업데이트

	UpdateMove();
	UpdateState();
	UpdateAnimation();

	//if (KEY_TAP(KEY::SPACE))
	//{
	//	CreateMissile();
	//}
	//
	//if (KEY_TAP(KEY::_1))
	//{
	//	SetMissileType(1);
	//}
	//
	//if (KEY_TAP(KEY::_2))
	//{
	//	SetMissileType(2);
	//}

	if (KEY_TAP(KEY::ENTER))
	{
		SetPos(Vec2(640.f, 384.f));
	}
	
	GetAnimator()->update();

	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;
}

void CPlayer::render(HDC _dc)
{
	//int iWidth = (int)m_pTex->Width();
	//int iHeight = (int)m_pTex->Height();

	//Vec2 vPos = GetPos();

	//BitBlt(_dc
	//	, (int)(vPos.x - (float)(iWidth / 2))
	//	, (int)(vPos.y - (float)(iHeight / 2))
	//	, iWidth
	//	, iHeight
	//	, m_pTex->GetDC()
	//	, 0, 0, SRCCOPY);

	// 마젠타 색상을 분리함(크로마키같은 원리)
	// TransparentBlt 헤더는 wingdi.h에 포함되어있고 wingdi.h는 Windows.h가 포함하고 있으나
	// 구현부분은 Msimg32.lib에 포함되어있어서 Msimg32.lib 없이는 선언만 가능하고 구현이 안되어 에러가 남 (pch.h의 #pragma comment(lib, "Msimg32.lib"))
	//TransparentBlt(_dc
	//	, (int)(vPos.x - (float)(iWidth / 2))
	//	, (int)(vPos.y - (float)(iHeight / 2))
	//	, iWidth
	//	, iHeight
	//	, m_pTex->GetDC()
	//	, 0, 0, iWidth, iHeight
	//	, RGB(255, 0, 255));

	// 컴포넌트(충돌체, etc...)가 있는 경우 렌더
	component_render(_dc);

	//CTexture* pTex = CResourceMgr::GetInst()->LoadTexture(L"PlayerAlpha", L"texture\\PlayerAlpha.bmp");

	//Vec2 vPos = GetPos();
	//vPos = CCamera::GetInst()->GetRenderPos(vPos);

	//float width = (float)pTex->Width();
	//float height = (float)pTex->Height();

	//BLENDFUNCTION bf = {};

	//bf.BlendOp = AC_SRC_OVER;
	//bf.BlendFlags = 0;
	//bf.AlphaFormat = AC_SRC_ALPHA;
	//bf.SourceConstantAlpha = 127;



	//AlphaBlend(_dc
	//	, int(vPos.x - width / 2.f)
	//	, int(vPos.y - height / 2.f)
	//	, int(width), int(height)
	//	, pTex->GetDC()
	//	, 0, 0, int(width), int(height)
	//	, bf);

}

void CPlayer::CreateMissile()
{
	if (1 == MissileType)
	{
		// 미사일을 발사합니다.
		FireMissile(Vec2(0.f, -1.f));  // 위쪽 방향
	}

	else if (2 == MissileType)
	{
		// 미사일을 발사합니다.
		FireMissile(Vec2(-1.f, -7.f)); // 왼쪽 아래 방향
		FireMissile(Vec2(0.f, -1.f));  // 위쪽 방향
		FireMissile(Vec2(1.f, -7.f));  // 오른쪽 아래 방향
	}
}

void CPlayer::UpdateMove()
{
	CRigidBody* pRigid = GetRigidBody();

	//if (KEY_HOLD(KEY::W))
	//{
	//	pRigid->AddForce(Vec2(0.f, -m_fSpeed));
	//}

	//if (KEY_HOLD(KEY::S))
	//{
	//	pRigid->AddForce(Vec2(0.f, m_fSpeed));
	//}

	if (KEY_HOLD(KEY::A) && KEY_HOLD(KEY::D))
	{
	}
	else if (KEY_HOLD(KEY::A))
	{
		pRigid->AddForce(Vec2(-m_fSpeed, 0.f));
	}
	else if (KEY_HOLD(KEY::D))
	{
		pRigid->AddForce(Vec2(m_fSpeed, 0.f));
	}

	

	//if (KEY_TAP(KEY::W))
	//{
	//	pRigid->AddVelocity(Vec2(0.f, -(m_fSpeed - 30.f)));
	//}

	//if (KEY_TAP(KEY::S))
	//{
	//	pRigid->AddVelocity(Vec2(0.f, (m_fSpeed - 30.f)));
	//}

	//if (KEY_TAP(KEY::A))
	//{
	//	pRigid->SetVelocity(Vec2(-(m_fSpeed - 30.f), pRigid->GetVelocity().y));
	//}

	//if (KEY_TAP(KEY::D))
	//{
	//	pRigid->SetVelocity(Vec2((m_fSpeed - 30.f), pRigid->GetVelocity().y));
	//}
}

// 키 입력에 따른 애니메이션 상태 설정
void CPlayer::UpdateState()
{
	if (KEY_HOLD(KEY::A) && KEY_HOLD(KEY::D))
	{
	}
	else if (KEY_HOLD(KEY::A))
	{
		m_iDir = -1;
		if (PLAYER_STATE::JUMP != m_eCurState)
		{
			m_eCurState = PLAYER_STATE::WALK;
		}
	}
	else if (KEY_HOLD(KEY::D))
	{
		m_iDir = 1;
		if (PLAYER_STATE::JUMP != m_eCurState)
		{
			m_eCurState = PLAYER_STATE::WALK;
		}
	}
	
	if (0.f == GetRigidBody()->GetSpeed() && PLAYER_STATE::JUMP != m_eCurState)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}

	if (KEY_TAP(KEY::SPACE) && m_iJumpCnt > 0)
	{
		m_eCurState = PLAYER_STATE::JUMP;
		if (GetRigidBody())
		{
			GetRigidBody()->SetVelocity(Vec2(GetRigidBody()->GetVelocity().x, - m_fJumpPower));
		}
		m_iJumpCnt -= 1;
		SetJump(true);
	}
}

void CPlayer::UpdateAnimation()
{
	if (m_ePrevState != m_eCurState && m_iPrevDir == m_iDir)
	{
		return;
	}

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		if (m_iDir == -1)
		{
			GetAnimator()->Play(L"IDLE_LEFT", true);
		}
		else
		{
			GetAnimator()->Play(L"IDLE_RIGHT", true);
		}
	}
		break;
	case PLAYER_STATE::WALK:
	{
		if (m_iDir == -1)
		{
			GetAnimator()->Play(L"WALK_LEFT", true);
		}
		else
		{
			GetAnimator()->Play(L"WALK_RIGHT", true);
		}
	}
		break;
	case PLAYER_STATE::ATTACK:
		break;
	case PLAYER_STATE::JUMP:
	{
		if (m_iDir == -1)
		{
			GetAnimator()->Play(L"WALK_LEFT", true);
		}
		else
		{
			GetAnimator()->Play(L"WALK_RIGHT", true);
		}
	}
		break;
	case PLAYER_STATE::HIT:
		break;
	case PLAYER_STATE::DEAD:
		break;
	}
}

void CPlayer::updateGravity()
{
	// 중력 적용
	GetRigidBody()->AddForce(Vec2 (0.f , 500.f));
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	//CObject* pOtherObj = _pOther->GetObj();
	//if (L"Ground" == pOtherObj->GetName())
	//{
	//	Vec2 vPos = GetPos();
	//	if (vPos.y < pOtherObj->GetPos().y)
	//	{
	//		m_eCurState = PLAYER_STATE::IDLE;
	//	}
	//}
	//m_iJumpCnt = m_iMaxJump;

	// 1. 한 틱에 여러개의 Object가 Player의 Collider와 충돌한 경우 오류가 발생할 가능성이 있음
	// 2. 그래서 Player에 m_hTarget을 추가하고 제거하는 AddTarget 함수와 RemoveTarget 함수를 만들고
	// 3. Object가 Player와 충돌한 Object->OnCollisionEnter() 에서 Player->AddTarget(),RemoveTarget()에 접근하는 방식으로 구현

	CObject* pOtherObj = _pOther->GetObj();

	if (GROUP_TYPE::GROUND == pOtherObj->GetType())
	{
		Vec2 vPos = GetPos();
		Vec2 vOtherPos = pOtherObj->GetPos();
		Vec2 vOtherScale = pOtherObj->GetScale();

		// 플레이어의 위치가 땅의 윗부분에 있을 때만 IDLE 상태로 변경
		if (vPos.y < vOtherPos.y - (vOtherScale.y / 2.f))
		{
			m_eCurState = PLAYER_STATE::IDLE;
			
			//this->GetGravity()->SetGround(true); // 땅에 닿았음을 중력에 알림
		}
	}
	//ResetJump(); // 점프 횟수 초기화
}

//void CPlayer::OnCollisionExit(CCollider* _pOther)
//{
//	CObject* pOtherObj = _pOther->GetObj();	
//
//	if (GROUP_TYPE::GROUND == pOtherObj->GetType())
//	{
//		this->GetGravity()->SetGround(false);
//	}
//}

void CPlayer::SetMissileType(int _type)
{
	MissileType = _type;
}

void CPlayer::FireMissile(const Vec2& dir)
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"Missile_Player");
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(25.f, 25.f));
	pMissile->SetDir(dir);

	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);

	CreateObject(pMissile,GROUP_TYPE::PROJ_PLAYER);
}

void CPlayer::AddTarget(CObject* pTarget)
{
	if (pTarget != nullptr) {
		m_hTarget.insert(pTarget); // 중복 방지를 위해 unordered_set 사용
	}
}

void CPlayer::RemoveTarget(CObject* pTarget)
{
	m_hTarget.erase(pTarget);
}

