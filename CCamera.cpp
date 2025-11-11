#include "pch.h"
#include "CCamera.h"

#include "CObject.h"
#include "CCore.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CResourceMgr.h"
#include "CTexture.h"

CCamera::CCamera()
	: m_pTargetObj(nullptr)
	, m_fTime(0.5f)
	, m_fSpeed(0.f)
	, m_fAccTime(0.5f)
	, m_pVeilTex(nullptr)

	// 지수 감속 관련 초기화
	, m_fMinSpeed(10.f)
	, m_k(1.f)
{
}

CCamera::~CCamera()
{

}

void CCamera::SetLookAt(Vec2 _vLook)
{
	m_vLookAt = _vLook;
	float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length();
	m_fSpeed = fMoveDist / m_fTime;
	m_fAccTime = 0.f;
}

void CCamera::init()
{
	// 해상도 불러오기
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	// 해상도 사이즈에 맞는 가림막 텍스쳐 생성
	m_pVeilTex = CResourceMgr::GetInst()->CreateTexture(L"CameraVeil",(UINT)vResolution.x, (UINT)vResolution.y);
}

void CCamera::update()
{
	if (m_pTargetObj)
	{
		// 타겟이 죽으면 타겟 해제
		if (m_pTargetObj->IsDead())
		{
			m_pTargetObj = nullptr;
		}
		else
		{
			// 설정된 타겟으로 카메라 위치 설정
			m_vLookAt = m_pTargetObj->GetPos();

			// 타겟이 새로 설정되었을 때 이전 위치 초기화
			if (m_fAccTime == 0.f)  // 초기 상태일 때만 이전 위치 초기화
			{
				m_vPrevLookAt = m_vLookAt;
			}
		}
	}
	else
	{
		if (KEY_HOLD(KEY::UP))
			m_vLookAt.y -= 500.f * fDT;
		if (KEY_HOLD(KEY::DOWN))
			m_vLookAt.y += 500.f * fDT;
		if (KEY_HOLD(KEY::LEFT))
			m_vLookAt.x -= 500.f * fDT;
		if (KEY_HOLD(KEY::RIGHT))
			m_vLookAt.x += 500.f * fDT;
	}


	// 화면 중앙 좌표와 카메라 LooAt 좌표간의 차이값 계산
	CalDiffv1();
}

void CCamera::render(HDC _dc)
{
	if (m_listCamEffect.empty())
	{
		return;
	}

	// 시간 누적값을 체크
	tCamEffect& effect = m_listCamEffect.front();
	effect.fCurTime += fDT;

	float fRatio = 0.f; // 이펙트 진행 비율

	// 효과 진행
	fRatio = effect.fCurTime / effect.fDuration;
	
	if (fRatio < 0.f)
	{
		fRatio = 0.f;
	}
	if (fRatio > 1.f)
	{
		fRatio = 1.f;
	}

	int iAlpha = 0;

	if (CAM_EFFECT::FADE_OUT == effect.eEffect)
	{
		// 알파값 (0~1)
		iAlpha = (int)(255.f * fRatio);
	}

	else if (CAM_EFFECT::FADE_IN == effect.eEffect)
	{
		// 알파값 (0~1)
		iAlpha = (int)(255.f * (1.f - fRatio));
	}
	
	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = iAlpha;

	AlphaBlend(_dc
		, 0
		, 0
		, (int)m_pVeilTex->Width()
		, (int)m_pVeilTex->Height()
		, m_pVeilTex->GetDC()
		, 0, 0, (int)m_pVeilTex->Width(), (int)m_pVeilTex->Height()
		, bf);

	// 진행 시간이 이팩트 최대 지정 시간을 넘어선 경우
	if (effect.fDuration < effect.fCurTime)
	{
		// 효과 종료
		m_listCamEffect.pop_front();
	}
}

// 등속운동 버전
void CCamera::CalDiffv1()
{
	// 이전 LookAt과 현재 Look의 차이값을 보정해서 현재의 LookAt을 구한다
	// 방향벡터 : 목적지 - 출발지

	// fCameraSpeed처럼 고정된 속도로 이동하는 방식보다
	// 일정 시간(DT)에 걸쳐서 나눠서 이동하면 오차 보정이 가능함

	//float fCameraSpeed = 500.f;
	m_fAccTime += fDT;

	if (m_fTime <= m_fAccTime)
	{
		// 지정한 시간(2초)가 지나면 거리가 얼마남았든 목적지로 설정
		m_vCurLookAt = m_vLookAt;
	}
	else
	{
		// 아직 이동중일 때
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;

		if (!vLookDir.IsZero())
		{
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * fDT;
		}
	}

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;
	
	// 한번에 카메라 이동
	//m_vDiff = m_vLookAt - vCenter;

	// 천천히 보정값으로 카메라 이동
	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;
}

void CCamera::CalDiffv2()
{	

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_fAccTime += fDT;

	if (m_fTime <= m_fAccTime)
	{
		// 지정한 시간(2초)가 지나면 거리가 얼마남았든 목적지로 설정
		m_vCurLookAt = m_vLookAt;
		m_fAccTime = 0.f;
		m_fSpeed = 0.f;
	}
	else
	{
		// 지수 감속 적용
		if (m_fSpeed == 0.f)
		{
			// 속도가 0이면 최소 속도로 설정
			m_fSpeed = 500.f;  // 초기 속도 설정
		}

		float speed = m_fMinSpeed + (m_fSpeed - m_fMinSpeed) * exp(-m_k * m_fAccTime);

		// 카메라 이동
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;
		float distance = vLookDir.Length();

		// 임계값 이하로 도달하면 타겟 위치로 고정
		if (distance < 1.f)  // 1 픽셀 이하로 차이가 나면 타겟에 바로 위치
		{
			m_vCurLookAt = m_vLookAt;
		}
		else
		{
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * speed * fDT;
		}
		m_vPrevLookAt = m_vCurLookAt;
	}
}

void CCamera::CalDiffv3()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_fAccTime += fDT;

	if (m_fTime <= m_fAccTime)
	{
		// 지정한 시간(2초)가 지나면 거리가 얼마남았든 목적지로 설정
		m_vCurLookAt = m_vLookAt;
		m_fAccTime = 0.f;
		m_fSpeed = 0.f;
	}
	else
	{
		float speed = m_fMinSpeed + (m_fSpeed - m_fMinSpeed) * exp(-m_k * m_fAccTime);


		// 카메라 이동
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;
		float distance = vLookDir.Length();

		if (distance < 1.f)  // 1 픽셀 이하로 차이가 나면 타겟에 바로 위치
		{
			m_vCurLookAt = m_vLookAt;
		}
		else
		{
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * speed * fDT;
		}
		m_vPrevLookAt = m_vCurLookAt;
	}

	// 천천히 보정값으로 카메라 이동
	m_vDiff = m_vCurLookAt - vCenter;
}

void CCamera::CalDiffv4()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_fAccTime += fDT;

	Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;
	float distance = vLookDir.Length();

	if (m_fTime <= m_fAccTime || distance < 1.f)
	{
		// 지정한 시간(2초)가 지나면 거리가 얼마남았든 목적지로 설정
		m_vCurLookAt = m_vLookAt;
		m_fAccTime = 0.f;
		m_fSpeed = 0.f;

		m_vDiff = m_vLookAt - vCenter;
	}
	else
	{
		// 카메라 이동
		float speed = m_fMinSpeed + (m_fSpeed - m_fMinSpeed) * exp(-m_k * m_fAccTime);

		// 카메라 이동
		m_vCurLookAt = m_vPrevLookAt + (m_vLookAt - m_vPrevLookAt).Normalize() * speed * fDT;

		// 천천히 보정값으로 카메라 이동
		m_vDiff = m_vCurLookAt - vCenter;
		m_vPrevLookAt = m_vCurLookAt;
	}
}

void CCamera::CalDiffv5()
{
	m_fAccTime += fDT;

	if (m_fTime <= m_fAccTime)
	{
		// 지정한 시간(2초)가 지나면 거리가 얼마남았든 목적지로 설정
		m_vCurLookAt = m_vLookAt;
	}
	else
	{
		// 아직 이동중일 때
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;
		m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * fDT;
	}

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	// 한번에 카메라 이동
	//m_vDiff = m_vLookAt - vCenter;

	// 천천히 보정값으로 카메라 이동
	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;
}
