# 2D 게임 엔진 구현

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

#### 2.4 타일 충돌
	- 콜리전 매니저에서 충돌체크 확인
	
			void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
			{
				// 더 작은 값의 그룹 타입을 행으로
				// 큰 값을 열(비트)로 사용
			
				UINT iRow = (UINT)_eLeft;
				UINT iCol = (UINT)_eRight;
			
				if (iRow > iCol)
				{
					iRow = (UINT)_eRight;
					iCol = (UINT)_eLeft;
				}
			
				if (m_arrCheck[iRow] & (1 << iCol)) // 비트가 1이면
				{
					// 비트를 0로 설정
					m_arrCheck[iRow] &= ~(1 << iCol);
				}
				else // 비트가 0이면
				{
					// 비트를 1로 설정
					m_arrCheck[iRow] |= (1 << iCol);
				}
			}

   - CGround의 OnCollisionEnter
  		- 이전 틱에 충돌을 하지 않았던 경우 호출
     
		  		void CGround::OnCollisionEnter(CCollider* _pOther)
				{
					// 다른 충돌체와 충돌했을때
				 	CObject* pOtherObj = _pOther->GetObj();
					if (pOtherObj->GetType() == GROUP_TYPE::PLAYER)
					{
						//// 땅에 닿았음을 알림
						//pOtherObj->GetGravity()->SetGround(true);
				
						// CPlayer로 변환
						CPlayer* pPlayer = dynamic_cast<CPlayer*>(pOtherObj);
						// pPlayer가 nullptr이 아닌 경우에만(충돌한 _pOther가 Player인 경우에만) AddTarget 호출
						if (pPlayer != nullptr)
						{
							pPlayer->AddTarget(this);
						}
					}
				}

   - CGround의 OnCollision

	 		void CGround::OnCollision(CCollider* _pOther)
			{
				CObject* pOtherObj = _pOther->GetObj();
				// CPlayer로 변환
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pOtherObj);
			
				// dynamic_cast를 이용해 플레이어가 맞는지 검사
				if (pPlayer != nullptr)
				{
					// 플레이어의 위치와 크기 가져오기
					Vec2 vPlayerPos = pPlayer->GetPos(); // 플레이어의 현재 위치
					Vec2 vPlayerScale = pPlayer->GetScale(); // 플레이어의 콜라이더 크기
			
					// 플랫폼의 위치와 크기 가져오기
					Vec2 vPlatformPos = GetCollider()->GetFinalPos(); // 플랫폼 위치
					Vec2 vPlatformScale = GetCollider()->GetScale(); // 플랫폼 콜라이더 크기
			
					// 플레이어의 경계선
					float fPlayerTop = vPlayerPos.y - vPlayerScale.y / 2;
					float fPlayerBottom = vPlayerPos.y + vPlayerScale.y / 2;
					float fPlayerLeft = vPlayerPos.x - vPlayerScale.x / 2;
					float fPlayerRight = vPlayerPos.x + vPlayerScale.x / 2;
			
					// 플랫폼의 경계선
					float fPlatformTop = vPlatformPos.y - vPlatformScale.y / 2;
					float fPlatformBottom = vPlatformPos.y + vPlatformScale.y / 2;
					float fPlatformLeft = vPlatformPos.x - vPlatformScale.x / 2;
					float fPlatformRight = vPlatformPos.x + vPlatformScale.x / 2;
			
					// 플레이어와 플랫폼 크기의 절반
					float halfPlayerX = vPlayerScale.x * 0.5f;
					float halfPlayerY = vPlayerScale.y * 0.5f;
					float halfPlatformX = vPlatformScale.x * 0.5f;
					float halfPlatformY = vPlatformScale.y * 0.5f;
			
					// 중심 좌표 차이 계산
					float deltaX = vPlayerPos.x - vPlatformPos.x;
					float deltaY = vPlayerPos.y - vPlatformPos.y;
			
					// x축, y축 오버랩(겹침) 계산
					float overlapX = (halfPlayerX + halfPlatformX) - fabs(deltaX);
					float overlapY = (halfPlayerY + halfPlatformY) - fabs(deltaY);
			
					Vec2 pVec = pPlayer->GetRigidBody()->GetVelocity();
			
					// 오버랩이 양수이면 실제 충돌 상태임
					if (overlapX > 0.f && overlapY > 0.f)
					{
						// 어느 축의 충돌이 더 약한지(겹침 정도가 작은지) 판단
						if (overlapX < overlapY)
						{
							// [좌우(수평) 충돌 처리]
							if (deltaX > 0.f)
							{
								// 플레이어가 플랫폼의 오른쪽에 위치한 경우:
								// 겹침을 해소하기 위해 오른쪽으로 이동
								pPlayer->SetPos(Vec2(vPlayerPos.x + overlapX, vPlayerPos.y));
							}
							else
							{
								// 플레이어가 플랫폼의 왼쪽에 위치한 경우:
								// 겹침을 해소하기 위해 왼쪽으로 이동
								pPlayer->SetPos(Vec2(vPlayerPos.x - overlapX, vPlayerPos.y));
							}
							// 수평 충돌 시 x축 속도를 0으로 설정 (y축 속도는 그대로 유지)
							Vec2 curVelocity = pPlayer->GetRigidBody()->GetVelocity();
							pPlayer->GetRigidBody()->SetVelocity(Vec2(0.f, curVelocity.y));
							pPlayer->GetGravity()->SetGround(false);
						}
						else
						{
							// [상하(수직) 충돌 처리]
							// **좌표 체계 주의:** 여기서는 y가 아래로 커진다고 가정합니다.
							if (deltaY < 0.f)
							{
								// 플레이어의 중심이 플랫폼보다 위에 있으면,
								// 즉, 플레이어가 플랫폼 위로 내려와 착지한 경우:
								// 플레이어의 바닥이 플랫폼의 위쪽 경계와 맞닿도록 위치 보정
								float newY = (vPlatformPos.y - halfPlatformY) - halfPlayerY;
								pPlayer->SetPos(Vec2(vPlayerPos.x, newY));
								pPlayer->GetGravity()->SetGround(true);
								pPlayer->ResetJump();
								pPlayer->SetJump(false);
							}
							else
							{
								// 플레이어의 중심이 플랫폼보다 아래에 있으면,
								// 즉, 플레이어가 플랫폼 밑(천장)에 부딪힌 경우:
								// 플레이어의 위쪽이 플랫폼의 아래쪽 경계와 맞닿도록 위치 보정
								float newY = (vPlatformPos.y + halfPlatformY) + halfPlayerY;
								pPlayer->SetPos(Vec2(vPlayerPos.x, newY));
								// 천장 충돌 시 y축 속도를 강제로 조정하여 상승을 막음
								Vec2 curVelocity = pPlayer->GetRigidBody()->GetVelocity();
								pPlayer->GetRigidBody()->SetVelocity(Vec2(curVelocity.x, 100.f));
								pPlayer->GetGravity()->SetGround(false);
							}
						}
					}	
				}
			}
  
### 2.5 중력

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
