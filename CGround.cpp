#include "CGround.h"

#include "CCollider.h"
#include "CGravity.h"
#include "CPlayer.h"
#include "CRigidBody.h"

CGround::CGround()
{
	CreateCollider();
}

CGround::~CGround()
{
}

void CGround::start()
{
	GetCollider()->SetScale(Vec2(GetScale()));
}

void CGround::update()
{

}

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

// 지속적인 충돌 처리
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
					// 플레이어가 플랫폼의 오른쪽에 위치한 경우 겹침을 해소하기 위해 오른쪽으로 이동
					pPlayer->SetPos(Vec2(vPlayerPos.x + overlapX, vPlayerPos.y));
				}
				else
				{
					// 플레이어가 플랫폼의 왼쪽에 위치한 경우 겹침을 해소하기 위해 왼쪽으로 이동
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
				if (deltaY < 0.f)
				{
					// 플레이어가 플랫폼 위로 내려와 착지한 경우
					// 플레이어의 바닥이 플랫폼의 위쪽 경계와 맞닿도록 위치 보정
					float newY = (vPlatformPos.y - halfPlatformY) - halfPlayerY;
					pPlayer->SetPos(Vec2(vPlayerPos.x, newY));
					pPlayer->GetGravity()->SetGround(true);
					pPlayer->ResetJump();
					pPlayer->SetJump(false);
				}
				else
				{
					// 플레이어가 플랫폼 밑(천장)에 부딪힌 경우
					// 플레이어의 위쪽이 플랫폼의 아래쪽 경계와 맞닿도록 위치 보정
					float newY = (vPlatformPos.y + halfPlatformY) + halfPlayerY;
					pPlayer->SetPos(Vec2(vPlayerPos.x, newY));
					// 천장 충돌 시 y축 속도를 강제로 조정하여 상승을 막음
					Vec2 curVelocity = pPlayer->GetRigidBody()->GetVelocity();
					pPlayer->GetRigidBody()->SetVelocity(Vec2(curVelocity.x, 0.f));
					pPlayer->GetGravity()->SetGround(false);
				}
			}
		}

		//if (pVec.y >= 0 && fPlayerBottom > fPlatformTop)
		//{
		//	// 플레이어가 내려가거나 정지 중일 때,
		//	// 그리고 플레이어의 Bottom이 플랫폼의 y보다 아래에 있으면 (즉, 플랫폼 위에 착지)
		//	pPlayer->GetGravity()->SetGround(true);
		//	pPlayer->SetPos(Vec2(vPlayerPos.x, fPlatformTop - vPlayerScale.y/2));
		//	pPlayer->ResetJump();
		//	pPlayer->SetJump(false);
		//}
		//else if (pVec.y < 0 && fPlayerTop >  vPlatformPos.y)
		//{
		//	// 플레이어가 위로 상승 중일 때,
		//	// 그리고 플레이어의 To p이 플랫폼의 y보다 위에 있으면 (즉, 플랫폼의 밑(천장)에 부딪힘)
		//	pPlayer->GetGravity()->SetGround(false);

		//	// 천장과의 충돌 시 수직 속도를 강제로 변경하여 더 이상의 상승을 막음
		//	pPlayer->GetRigidBody()->SetVelocity(Vec2(0.f, 100.f));

		//	// 플레이어를 밑으로 1픽셀 내림
		//	pPlayer->SetPos(Vec2(vPlayerPos.x,vPlayerPos.y + 1));
		//}

		//// [좌우 충돌 처리]
		//// 오른쪽으로 이동 중일 때 (pVec.x > 0) 
		//// 그리고 플레이어의 오른쪽(fPlayerRight)이 플랫폼의 왼쪽(fPlatformLeft)을 침범했으며,
		//// 플레이어의 왼쪽은 아직 플랫폼의 왼쪽보다 왼쪽에 있다면
		//if (pVec.x > 0 && fPlayerRight > fPlatformLeft && fPlayerLeft < fPlatformLeft)
		//{
		//	// 오른쪽 충돌: 수평 속도를 0으로 설정하고, 겹침만큼 위치 보정
		//	pPlayer->GetRigidBody()->SetVelocity(Vec2(0.f, pVec.y));
		//	pPlayer->SetPos(Vec2(fPlatformLeft - vPlayerScale.x / 2, vPlayerPos.y));
		//	pPlayer->GetGravity()->SetGround(false);
		//}
		//// 왼쪽으로 이동 중일 때 (pVec.x < 0) 
		//// 그리고 플레이어의 왼쪽(fPlayerLeft)이 플랫폼의 오른쪽(fPlatformRight)을 침범했으며,
		//// 플레이어의 오른쪽은 아직 플랫폼의 오른쪽보다 오른쪽에 있다면
		//else if (pVec.x < 0 && fPlayerLeft < fPlatformRight && fPlayerRight > fPlatformRight)
		//{
		//	// 왼쪽 충돌: 수평 속도를 0으로 설정하고, 겹침만큼 위치 보정
		//	pPlayer->GetRigidBody()->SetVelocity(Vec2(0.f, pVec.y));
		//	pPlayer->SetPos(Vec2(fPlatformRight + vPlayerScale.x / 2, vPlayerPos.y));
		//	pPlayer->GetGravity()->SetGround(false);
		//}
	}
}

////// 지속적인 충돌 처리
////void CGround::OnCollision(CCollider* _pOther)
////{
////	CObject* pOtherObj = _pOther->GetObj();
////	// CPlayer로 변환
////	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pOtherObj);
////
////	// dynamic_cast를 이용해 플레이어가 맞는지 검사
////	if (pPlayer != nullptr)
////	{
////		// pPlayer와 충돌중인 모든 오브젝트들을 검사
////		for (CObject* pTarget : pPlayer->m_hTarget)
////		{
////			// 충돌한 오브젝트 중에 플랫폼이 있으면
////			if (GROUP_TYPE::GROUND == pTarget->GetType())
////			{
////				// 플레이어의 위치와 크기 가져오기
////				Vec2 vPlayerPos = pPlayer->GetPos(); // 플레이어의 현재 위치
////				Vec2 vPlayerScale = pPlayer->GetScale(); // 플레이어의 콜라이더 크기
////
////				// 플랫폼의 위치와 크기 가져오기
////				Vec2 vPlatformPos = GetCollider()->GetFinalPos(); // 플랫폼 위치
////				Vec2 vPlatformScale = GetCollider()->GetScale(); // 플랫폼 콜라이더 크기
////
////				// 플레이어의 오프셋 위치
////				Vec2 vPlayerOffsetPos = pPlayer->GetCollider()->GetOffsetPos();
////				Vec2 vPlayerColliderScale = pPlayer->GetCollider()->GetScale();
////
////				// 플레이어의 경계선
////				float fPlayerTop = vPlayerPos.y + vPlayerOffsetPos.y - vPlayerScale.y / 2;
////				float fPlayerBottom = vPlayerPos.y + vPlayerOffsetPos.y + vPlayerScale.y / 2;
////				float fPlayerLeft = vPlayerPos.y + vPlayerOffsetPos.x - vPlayerScale.x / 2;
////				float fPlayerRight = vPlayerPos.y + vPlayerOffsetPos.x + vPlayerScale.x / 2;
////
////				// 플랫폼의 경계선
////				float fPlatformTop = vPlatformPos.y - vPlatformScale.y / 2;
////				float fPlatformBottom = vPlatformPos.y + vPlatformScale.y / 2;
////				float fPlatformLeft = vPlatformPos.x - vPlatformScale.x / 2;
////				float fPlatformRight = vPlatformPos.x + vPlatformScale.x / 2;
////
////				if(fPlayerRight < fPlatformLeft || fPlayerLeft > fPlatformRight)
////
////				
////				//// fLen의 값이 바뀌는 이유는 abs때문으로 의심됨
////				//// 절대값으로 계산하니까 오류가 생기는거같음
////
////				//// X축 거리 계산 (콜라이더 크기 포함)
////				//float fLenX = fabs((vPlayerPos.x + vPlayerOffsetPos.x) - vPlatformPos.x);
////				//float fOverlapX = (vPlayerScale.x / 2.f + vPlatformScale.x / 2.f) - fLenX;
////
////				//// Y축 거리 계산 (콜라이더 크기 포함)10
////				//float fLenY = fabs((vPlayerPos.y + vPlayerOffsetPos.y) - vPlatformPos.y);
////				//float fOverlapY = (vPlayerScale.y / 2.f + vPlatformScale.y / 2.f) - fLenY;
////
////				//// 충돌한 경우
////				//if (fOverlapX > 0 && fOverlapY > 0)
////				//{
////				//	// x의 충돌값이 y의 충돌값 보다 큰경우 (위나 아래로 충돌한 경우)
////				//	if (fOverlapX > fOverlapY)
////				//	{  
////				//		// 위에서 충돌한 경우
////				//		if (fPlayerBottom > vPlatformPos.y)
////				//		{
////				//			pPlayer->ResetJump();
////
////  		//					pPlayer->GetGravity()->SetGround(true);
////				//			pPlayer->SetJump(false);
////				//		}
////				//		// 아래에서 충돌한 경우d
////				//		if (fPlayerTop > vPlatformPos.y)
////				//		{
//// 			//				pPlayer->GetRigidBody()->SetVelocity(Vec2(0.f, 100.f));
////				//			pPlayer->GetGravity()->SetGround(false);
////				//		}
////				//	}
////				//}
////			}
////		}
////	}


	/*if (pPlayer != nullptr)
	{
		// pPlayer와 충돌중인 모든 오브젝트들을 검사
		for (CObject* pTarget : pPlayer->m_hTarget)
		{
			// 충돌한 오브젝트 중에 플랫폼이 있으면
			if (GROUP_TYPE::GROUND == pTarget->GetType())
			{
				// 충돌 중인데도 Ground가 풀려있으면 다시 true로 설정
				pOtherObj->GetGravity()->SetGround(true);

				// 플레이어의 위치와 크기 가져오기
				Vec2 vObjPos = pPlayer->GetPos(); // 플레이어의 현재 위치
				Vec2 vObjScale = pPlayer->GetScale(); // 플레이어의 콜라이더 크기

				// 플랫폼의 위치와 크기 가져오기
				Vec2 vPlatformPos = GetCollider()->GetFinalPos(); // 플랫폼 위치
				Vec2 vPlatformScale = GetCollider()->GetScale(); // 플랫폼 콜라이더 크기

				// X축 거리 계산 (콜라이더 크기 포함)
				float fLenX = abs(vObjPos.x - vPlatformPos.x);
				float fOverlapX = (vObjScale.x / 2.f + vPlatformScale.x / 2.f) - fLenX;

				// Y축 거리 계산 (콜라이더 크기 포함)
				float fLenY = abs(vObjPos.y - vPlatformPos.y);
				float fOverlapY = (vObjScale.y / 2.f + vPlatformScale.y / 2.f) - fLenY;

				// 플랫폼 상단 경계
				float fPlatformTop = vPlatformPos.y;

				// 플레이어의 오프셋 위치
				Vec2 vOffsetPos = pPlayer->GetCollider()->GetOffsetPos();

				// 플레이어 콜라이더 중심 좌표 계산
				Vec2 vColliderPos = vObjPos + GetCollider()->GetOffsetPos();

				// 충돌 확인
				if (fOverlapX > 0 && fOverlapY > 0)
				{
					// X축 충돌값이 Y축 충돌값 보다 높은 경우
					if (fOverlapX > fOverlapY)
					{
						// 플레이어가 플랫폼 위에 있을 경우에만 위치 보정
						if ( vObjPos.y < vPlatformPos.y)
						{
							// 플랫폼 위에 착지한 것으로 판정
							pPlayer->SetPos(Vec2(
								vObjPos.x,
								fPlatformTop - vOffsetPos.y - vObjScale.y / 2.f
							));

							// 중력 상태를 착지로 설정
							pPlayer->GetGravity()->SetGround(true);
						}
						// 플레이어.x가 플랫폼.x보다 낮은 경우
						else
						{
							// 플랫폼 밑에서 충돌한것이므로 그대로 추락
							pPlayer->GetRigidBody()->SetVelocity(Vec2(0.f, 100.f));

							// 중력 다시 적용
							pPlayer->GetGravity()->SetGround(false);
						}
					}

					// 옆면에서 충돌한 경우
					else
					{
						// 좌측에서 충돌한경우
						if (vObjPos.x < vPlatformPos.x - (vPlatformScale.x / 2.f))
						{
							// 캐릭터를 플랫폼 좌측으로 밀어냄
							pPlayer->SetPos(Vec2(
								vPlatformPos.x - (vPlatformScale.x / 2) - (pPlayer->GetCollider()->GetScale().x / 2) - vOffsetPos.x,
								vObjPos.y)
							);

							float new_y = pPlayer->GetRigidBody()->GetVelocity().y;

							pPlayer->GetRigidBody()->SetVelocity(Vec2(0.f,new_y + 100.f));

							// 중력 상태를 착지로 설정
							pPlayer->GetGravity()->SetGround(false);
						}

						// 우측에서 충돌한 경우
						else if (vObjPos.x > vPlatformPos.x + (vPlatformScale.x / 2.f))
						{
							// 캐릭터를 플랫폼 좌측으로 밀어냄
							pPlayer->SetPos(Vec2(
								vPlatformPos.x + (vPlatformScale.x / 2) + (pPlayer->GetCollider()->GetScale().x / 2) + vOffsetPos.x,
								vObjPos.y)
							);

							float new_y = pPlayer->GetRigidBody()->GetVelocity().y;

							pPlayer->GetRigidBody()->SetVelocity(Vec2(0.f, new_y + 100.f));

							// 중력 상태를 착지로 설정
							pPlayer->GetGravity()->SetGround(false);
						}
					}
				}
			}
			// 충돌한 오브젝트 중에 플랫폼이 없으면
			else
			{
			}
		}
	}*/

	/*//if (pOtherObj->GetType() == GROUP_TYPE::PLAYER)
	//{
	//	// 땅에 닿아 있지 않을 때만 SetGround(true) 호출
	//	if (!pOtherObj->GetGravity()->GetGround())
	//	{
	//		pOtherObj->GetGravity()->SetGround(true);
	//	}

	//	// 플레이어의 위치와 크기 가져오기
	//	Vec2 vObjPos = pOtherObj->GetPos(); // 플레이어의 현재 위치
	//	Vec2 vObjScale = pOtherObj->GetScale(); // 플레이어의 콜라이더 크기

	//	// 플랫폼의 위치와 크기 가져오기
	//	Vec2 vPos = GetCollider()->GetFinalPos(); // 플랫폼 위치
	//	Vec2 vScale = GetCollider()->GetScale(); // 플랫폼 콜라이더 크기

	//	// X축 거리 계산 (콜라이더 크기 포함)
	//	float fLenX = abs(vObjPos.x - vPos.x);
	//	float fValueX = (vObjScale.x / 2.f + vScale.x / 2.f) - fLenX;

	//	// Y축 거리 계산 (콜라이더 크기 포함)
	//	float fLenY = abs(vObjPos.y - vPos.y);
	//	float fValueY = (vObjScale.y / 2.f + vScale.y / 2.f) - fLenY;

	//	// X축과 Y축에서의 충돌 판정
	//	if (fLenY < (vObjScale.y / 2.f + vScale.y / 2.f) && fLenX < (vObjScale.x / 2.f + vScale.x / 2.f)) {
	//		// 수직/수평 거리 모두 두 객체 크기 합보다 작을 경우 충돌
	//		if (fLenY < fLenX) {
	//			// Y축 충돌이 더 가까운 경우 (플랫폼 위/아래)
	//			if (vObjPos.y > vPos.y) {
	//				// 플랫폼 위에서 충돌
	//				vObjPos = pOtherObj->GetPos();
	//				vObjPos.y -= fValueY;
	//				pOtherObj->SetPos(vObjPos);
	//			}
	//			//else if (vObjPos.y < vPos.y) {
	//			//	// 플랫폼 아래에서 충돌
	//			//	vObjPos = pOtherObj->GetPos();
	//			//	vObjPos.y += fValueY;
	//			//	pOtherObj->SetPos(vObjPos);
	//			//}
	//		}
	//		//else {
	//		//	// X축 충돌이 더 가까운 경우 (좌측/우측)
	//		//	if (vObjPos.x < vPos.x) {
	//		//		// 좌측에서 충돌 → 오른쪽으로 밀기
	//		//		vObjPos.x -= fValueX;
	//		//	}
	//		//	else if (vObjPos.x > vPos.x) {
	//		//		// 우측에서 충돌 → 왼쪽으로 밀기
	//		//		vObjPos.x += fValueX;
	//		//	}
	//		//	pOtherObj->SetPos(vObjPos);
	//		//}
	//	}

	//	//pOtherObj->GetGravity()->SetGround(true);

	//	//Vec2 vObjPos = _pOther->GetFinalPos();
	//	//Vec2 vObjScale = _pOther->GetScale();

	//	//Vec2 vPos = GetCollider()->GetFinalPos();
	//	//Vec2 vScale = GetCollider()->GetScale();

	//	//float fLen = abs(vObjPos.y - vPos.y);
	//	//float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

	//	//vObjPos = pOtherObj->GetPos();
	//	//vObjPos.y -= fValue;

	//	//pOtherObj->SetPos(vObjPos);
	//}*/
//}

void CGround::OnCollisionExit(CCollider* _pOther)
{
	// CPlayer로 변환
	CObject* pOtherObj = _pOther->GetObj();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pOtherObj);

	if (pPlayer)
	{
		pPlayer->GetGravity()->SetGround(false);
		pPlayer->RemoveTarget(this);
	}
}