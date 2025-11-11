#pragma once

class CObject;
class CTexture;

enum class CAM_EFFECT 
{
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct tCamEffect
{
	CAM_EFFECT	eEffect;		// 카메라 효과
	float		fDuration;	// 효과 최대 진행 시간
	float		fCurTime;		// 카메라 효과 현재 진행된 시간
};

class CCamera
{
	SINGLE(CCamera);
private:
	Vec2				m_vLookAt;			// 카메라가 보고있는 위치
	Vec2				m_vCurLookAt;		// 이전 위치와 현재 위치의 보정 위치
	Vec2				m_vPrevLookAt;		// 카메라가 보는 이전 프레임 위치

	CObject*			m_pTargetObj;		// 카메라 타겟 오브젝트
	Vec2				m_vDiff;			// 해상도 중심 위치와 카메라 LookAt간의 차이 값

	float				m_fTime;			// 타겟을 따라가는데 걸리는 시간
	float				m_fSpeed;			// 타겟을 따라가는 속도
	float				m_fAccTime;			// 누적 시간

	// 카메라 지수 감속
	float				m_fMinSpeed;		// 지수 감속 최소 시간
	float				m_k;				// 지수 감속 계수 (1 : 빠름->느림 0.1 : 느림->빠름)

	list<tCamEffect>	m_listCamEffect;
	CTexture* m_pVeilTex;			// 페이드 아웃 효과를 위한 카메라 가림막 텍스쳐(검은색)


public:
	void SetLookAt(Vec2 _vLook);
	void SetPos(Vec2 _vPos) { m_vLookAt = _vPos; }
	void SetTarget(CObject* _pTarget) { m_pTargetObj = _pTarget; }
	Vec2 GetLookAt() { return m_vCurLookAt; }
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }
	Vec2 GetRealPos(Vec2 _vRenderPos) { return _vRenderPos + m_vDiff; }

	void FadeIn(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_IN;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (0.f == _fDuration)
		{
			assert(nullptr);
		}
	}

	void FadeOut(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_OUT;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (0.f == _fDuration)
		{
			assert(nullptr);
		}
	}

public:
	void init();
	void update();
	void render(HDC _dc);

private:
	void CalDiffv1();
	void CalDiffv2();
	void CalDiffv3();
	void CalDiffv4();
	void CalDiffv5();

public:
	// 컴파일러가 생성자 소멸자를 기본으로 생성해줘서 만들 필요는 없으나 자신이 직접 초기화를 해야하는 상황이면 만들어 줘야함
	//CCamera();
	//~CCamera();
};

