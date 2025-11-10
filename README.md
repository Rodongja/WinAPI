# 2D 게임 엔진 샘플 프로젝트

## 개요
본 프로젝트는 C++과 WinAPI를 기반으로 한 2D 게임 엔진입니다.  
플레이어, 몬스터, 타일, 투사체 등 다양한 객체를 관리하고, AI 상태 전환, 물리 계산, 이벤트 기반 시스템을 포함합니다.
기반은 유튜브 강좌를 참고하고 플레이어의 충돌은 직접 구현했습니다. (https://www.youtube.com/watch?v=dlFr-OnHlWU&list=PL4SIC1d_ab-ZLg4TvAO5R4nqlJTyJXsPK&index=1)

---

## 주요 기능

### 1. 객체 및 그룹 관리
- CObject를 기반으로 모든 게임 객체 생성
- 객체 그룹 구분 (GROUP_TYPE)
  - DEFAULT, TILE, GROUND, MONSTER, PLAYER, PROJ_PLAYER, PROJ_MONSTER, UI 등
- 객체 생성/삭제 시 이벤트(CEventMgr) 발생

### 1.1 오브젝트 배치 예제:
 - 씬 생성시에는 DirectSpawn 가능

       CObject* pObj = new CPlayer;
       pObj->SetName(L"Player");
	     pObj->SetPos(Vec2(640.f,384.f));
	     pObj->SetScale(Vec2(20.f,40.f));

       CObject* pGround = new CGround;
       pGround->SetName(L"Ground");
       pGround->SetPos(Vec2(640.f, 584.f));
       pGround->SetScale(Vec2(1000.f, 60.f));
       AddObject(pGround, GROUP_TYPE::GROUND);

- 런타임 중에는 EventMgr 사용

      CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
      AddObject(pMon, GROUP_TYPE::MONSTER);
      CreateObject(pMon, GROUP_TYPE::MONSTER);

### 1.2 오브젝트 삭제 예제

      void CMonster::OnCollisionEnter(CCollider* _pOther)
      {
	        CObject* pOtherObj = _pOther->GetObj();
	
	        if (pOtherObj->GetName() == L"Missile_Player")
	        {
        		m_iHP -= 1;
	        	if (m_iHP <= 0)
	        	{
	        		DeleteObject(this);
	        	}
        	}
        }
---

### 2. 물리 충돌 처리 (CRigidBody)

플레이어 및 이동 객체의 물리적 이동과 충돌을 담당합니다.

#### 2.1 속성
- m_vForce : 외부 힘 (키 입력, 중력 등)
- m_vAccel : 기본 가속도 (F = m * a)
- m_vAccelA : 추가 가속도 (중력)
- m_fMass : 질량
- m_vVelocity : 속도 (크기 = 속력, 방향 포함)
- m_vMaxVelocity : 최대 속도 제한
- m_fFricCoeff : 마찰 계수

#### 2.2 이동 계산

    m_vAccel = m_vForce / m_fMass + m_vAccelA; // 가속도 계산  
    m_vVelocity += m_vAccel * fDT;             // 속도 갱신  
    LimitVelocity();                            // 최대 속도 제한  
    Move();                                     // 위치 변경

#### 2.3 충돌 판정
- 타일 충돌 : 이동 후 타일/바닥과 겹치면 위치 보정
- 중력 및 바닥 감지 : y축 속도 조절, 땅 위에 정지  
- 천장/벽 충돌 : 충돌 방향 속도 제거  
- delta time(fDT) 기반 이동량 적용

### 2.4 타일 충돌

  
### 2.5 중력 코드

    void CGravity::finalupdate()
    {
        if (true == m_bGround)
        {
            // 땅에 있을 때는 중력 제거
            m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 0.f));
        }
        else
        {
            // 공중에 있을 때만 중력 적용
            m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, m_fGravity));
        }
    }
    
---

### 3. AI 상태 관리
- AI 클래스가 소유 객체(CMonster)와 상태(CState)를 관리
- 상태(State) 종류(MON_STATE)
  - IDLE, PATROL, TRACE
- 상태 전환 시 이벤트 발생 및 Enter/Exit 함수 호출

예제:

    AI* pAI = new AI();
    pAI->AddState(new CIdleState());
    pAI->AddState(new CTraceState());
    pAI->SetCurState(MON_STATE::IDLE);

// 상태 변경

    pAI->ChangeState(MON_STATE::TRACE);

---

### 4. 이벤트 시스템
- CEventMgr를 통해 객체 생성, 삭제, 씬 변경, AI 상태 변경 이벤트 발생
- 이벤트를 통해 시스템 간 결합도를 낮추고 독립적으로 처리 가능

예제:

    ChangeScene(SCENE_TYPE::STAGE_01);
    ChageAIState(pAI, MON_STATE::TRACE);

---
