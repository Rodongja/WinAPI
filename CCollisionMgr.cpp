#include "pch.h"
#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

//// 유니온
//struct test {
//	int a;
//	float b;
//	// 모두 다 같은 메모리 저장공간을 공유함
//};

CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}


void CCollisionMgr::update()
{
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow)
	{
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionGroupUpdate((GROUP_TYPE)iRow,(GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// pCurScene->GetGroupObject(_eLeft);가 참조(&)받은 const vector면 받는쪽(vecLeft)도 const &로 받아야함
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);
	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->GetCollider()) // 충돌체를 보유하지 않는 경우
		{
			continue;
		}

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			if (nullptr == vecRight[j]->GetCollider() // 충돌체를 보유하지 않는 경우
				|| vecLeft[i] == vecRight[j]) // 자기 자신과의 체크 방지
			{
				continue;
			}

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();


			// 두 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			if (iter == m_mapColInfo.end()) // 이전 프레임 충돌한적 없고 검사를 최초로 한 경우
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}


			if (IsCollision(pLeftCol, pRightCol))
			{
				COLLIDER_ID ID;
				ID.Left_id = pLeftCol->GetID();
				ID.Right_id = pRightCol->GetID();

				iter = m_mapColInfo.find(ID.ID);

				if (m_mapColInfo.end() == iter) {
					// 충돌 정보가 없는 경우
					m_mapColInfo.insert(make_pair(ID.ID, false));
					iter = m_mapColInfo.find(ID.ID);
				}

				// 현재 충돌 중
				// iter->second; // 이전 프레임 충돌 정보
				if (iter->second)
				{
					// 이전 프레임에도 충돌 확인
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// 둘중 하나 이상 삭제 예정이라면 충돌 해제 시켜준다
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}
				else
				{
					// 이전 프레임에는 충돌 안함
					// 둘중 하나 이상 삭제 예정이라면 충돌 하지 않은것으로 취급
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}

			else
			{
				// 현재 충돌 안함
				if (iter->second)
				{
					// 이전에는 충돌했다가 현재는 충돌안함
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
				else
				{
					// 이전에도 충돌하지 않았고 현재도 충돌 안함

				}
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f && abs(vRightPos.y - vLeftPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)
	{
		// 충돌 확인
		return true;
	}

	return false;
}


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
