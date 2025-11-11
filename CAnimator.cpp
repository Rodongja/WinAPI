#include "pch.h"
#include "CAnimator.h"

#include "CAnimation.h"


CAnimator::CAnimator()
	: m_pCurAnim(nullptr)
	, m_pOwner(nullptr)
	, m_bRepeat(false)
{
}

CAnimator::~CAnimator()
{
	Safe_Delete_Map(m_mapAnim);
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	m_pCurAnim = FindAnimation(_strName);
	m_bRepeat = _bRepeat;
}

void CAnimator::update()
{
	//if (nullptr != m_pCurAnim)
	//{
	//	m_pCurAnim->update();

	//	// m_bRepeat를 먼저 비교하는 이유
	//	// &&이기때문에 m_bRepeat가 false면 자동으로 IsFinish를 넘겨버림
	//	// 최적화를 위하여 순서를 앞으로 둠
	//	if (m_bRepeat && m_pCurAnim->IsFinish())
	//	{
	//		m_pCurAnim->SetFrame(0);
	//	}
	//}
}

void CAnimator::finalupdate()
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->update();

		// m_bRepeat를 먼저 비교하는 이유
		// &&이기때문에 m_bRepeat가 false면 자동으로 IsFinish를 넘겨버림
		// 최적화를 위하여 순서를 앞으로 둠
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::render(HDC _dc)
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->render(_dc);
	}
}

void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	// 이미 존재하는 이름으로 애니메이션 생성을 시도할 때
	CAnimation* pAnim = FindAnimation(_strName);
	// 예외처리
	assert(nullptr == pAnim);

	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_pTex, _vLT, _vSliceSize, _vStep, _fDuration, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimator::LoadAnimation(const wstring& _strRelativePath, const Vec2& _vOffset)
{
	CAnimation* pAnim = new CAnimation;
	pAnim->Load(_strRelativePath);
	pAnim->m_pAnimator = this;

	// 애니메이션의 모든 프레임에 오프셋 적용
	for (UINT i = 0; i < pAnim->GetMaxFrame(); ++i)
	{
		pAnim->GetFrame(i).vOffset += _vOffset;
	}

	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}
