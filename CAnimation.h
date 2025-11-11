#pragma once

#include "global.h"

class CAnimator;
class CTexture;

struct tAnimFrm
{
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vOffset;
	float	fDuration;
};

class CAnimation
{
private:
	wstring				m_strName;		// Animation 이름
	CAnimator*			m_pAnimator;	// Animation 소유 Animator
	CTexture*			m_pTex;			// Animation의 Texture
	vector<tAnimFrm>	m_vecFrm;		// 모든 프레임 정보
	int					m_iCurFrm;		// 현재 프레임
	float				m_fAccTime;		// 누적 시간

	bool				m_bFinish;		// 애니메이션 종료 체크

public:
	const wstring& GetName() { return m_strName; }
	bool IsFinish() { return m_bFinish; }
	void SetFrame(int _iFrameIdx)
	{
		m_bFinish = false;
		m_iCurFrm = _iFrameIdx;
		m_fAccTime = 0.f;
	}

	tAnimFrm& GetFrame(int _iIdx) {return m_vecFrm[_iIdx];} // 받아간쪽에서 수정할 수 있도록 레퍼런스 참조
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }

private:
	void SetName(const wstring& _strName) { m_strName = _strName; }

public:
	void update();
	void render(HDC _dc);
	void Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
	
public:
	void Save(const wstring& _strRelativePath);
	void Load(const wstring& _strRelativePath);

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

