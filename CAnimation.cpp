#include "pch.h"
#include "CAnimation.h"

#include "CTimeMgr.h"

#include "CAnimator.h"
#include "CTexture.h"
#include "Cobject.h"
#include "CCamera.h"

#include "CResourceMgr.h"
#include "CPathMgr.h"

CAnimation::CAnimation()
	: m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}

//void CAnimation::update()
//{
//	// m_bFinish가 true인경우 애니메이션 재생을 하지 않음
//	if (m_bFinish)
//	{
//		return;
//	}
//
//	m_fAccTime += fDT;
//
//	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime)
//	{
//		++m_iCurFrm;
//
//		if (m_vecFrm.size() <= m_iCurFrm)
//		{
//			// -1을 수정해야하지 않을까? 이대로는 반복재생 안하면 이미지가 사리지지 않나?
//			//m_iCurFrm = -1;
//			m_iCurFrm = 0;
//			m_bFinish = true;
//		}
//
//		// 인덱스가 0부터 애니메이션 최대 사이즈까지의 값으로 들어올때만 DT와 누적시간의 값을 보정하도록 함
//		if (m_iCurFrm >= 0 && m_iCurFrm < m_vecFrm.size())
//		{
//			m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
//		}
//	}
//}

void CAnimation::update()
{
	// m_bFinish가 true인경우 애니메이션 재생을 하지 않음
	if (m_bFinish)
	{
		return;
	}

	m_fAccTime += fDT;

	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime)
	{
		++m_iCurFrm;

		if (m_vecFrm.size() <= m_iCurFrm)
		{
			// -1을 수정해야하지 않을까? 이대로는 반복재생 안하면 이미지가 사리지지 않나?
			//m_iCurFrm = -1;
			m_iCurFrm = 0;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}

		// 인덱스가 0부터 애니메이션 최대 사이즈까지의 값으로 들어올때만 DT와 누적시간의 값을 보정하도록 함
		if (m_iCurFrm >= 0 && m_iCurFrm < m_vecFrm.size())
		{
			m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
		}
	}
}

void CAnimation::render(HDC _dc)
{
	if (m_bFinish)
	{
		return;
	}

	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	vPos += m_vecFrm[m_iCurFrm].vOffset;	// Object Position에 Offset만큼 추가로 위치 이동

	// 렌더링 좌표로 변환
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vPos);

	TransparentBlt(_dc
		, (int)(vRenderPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)
		, (int)(vRenderPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, m_pTex->GetDC()								   
		, (int)(m_vecFrm[m_iCurFrm].vLT.x)
		, (int)(m_vecFrm[m_iCurFrm].vLT.y)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, RGB(255, 0, 255)
	);
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;

	tAnimFrm frm = {};
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration	= _fDuration;
		frm.vSlice		= _vSliceSize;
		frm.vLT			= _vLT + _vStep * (float)i;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	assert(pFile);

	//// 임시 코드
	//int a = 0;
	//fwrite(&a, sizeof(int), 1, pFile); // 데이터 주소, 데이터 크기, 개수, 파일 주소

	// Animation의 이름을 저장한다.
	// 데이터 직렬화(serialization)
	fprintf(pFile,"[Animation Name]\n");
	//SaveWString(m_strName, pFile);
	string strName = string(m_strName.begin(), m_strName.end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile,"\n");

	// Animation의 Texture
	fprintf(pFile, "[Texture Name]\n");
	strName = string(m_pTex->GetKey().begin(), m_pTex->GetKey().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	fprintf(pFile, "[Texture Path]\n");
	strName = string(m_pTex->GetRelativePath().begin(), m_pTex->GetRelativePath().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	// 프레임 개수
	fprintf(pFile, "[Frame Count]\n");
	fprintf(pFile, "%d\n", (int)m_vecFrm.size());

	fprintf(pFile, "\n");
	fprintf(pFile, "==============================\n");
	fprintf(pFile, "\n");

	// 모든 프레임 정보
	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fprintf(pFile, "[Frame Index]\n");
		fprintf(pFile, "%d\n",(int)i);

		fprintf(pFile, "[Left Top]\n");
		fprintf(pFile, "%d, %d\n", (int)m_vecFrm[i].vLT.x, (int)m_vecFrm[i].vLT.y);

		fprintf(pFile, "[Slice Size]\n");
		fprintf(pFile, "%d, %d\n", (int)m_vecFrm[i].vSlice.x, (int)m_vecFrm[i].vSlice.y);

		fprintf(pFile, "[Offset]\n");
		fprintf(pFile, "%d, %d\n", (int)m_vecFrm[i].vOffset.x, (int)m_vecFrm[i].vOffset.y);

		fprintf(pFile, "[Duration]\n");
		fprintf(pFile, "%f\n", (float)m_vecFrm[i].fDuration);

		fprintf(pFile, "\n");
	}

	//size_t iFrameCount = m_vecFrm.size();
	//fwrite(&iFrameCount, sizeof(size_t), 1, pFile);
	//
	//// 모든 프레임 정보
	//fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);
	
	//m_pAnimator;	// Animation 소유 Animator // 애니메이터는 직접 저장할 부분이 아님, 이미 갖고있는 정보
	//m_iCurFrm;		// 현재 프레임
	//m_fAccTime;		// 누적 시간
	//m_bFinish;		// 애니메이션 종료 체크

	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	//// 임시 코드
	//int a = 0;
	//fwrite(&a, sizeof(int), 1, pFile); // 데이터 주소, 데이터 크기, 개수, 파일 주소

	// Animation의 이름을 로드한다.
	// 데이터 직렬화(serialization)
	string str;
	char szBuff[256] = {};
	//fscanf_s(pFile, "%s",szBuff,256);

	FScanf(szBuff, pFile);	// [Animation Name]
	FScanf(szBuff, pFile);	// 실제 애니메이션 이름
	str = szBuff;
	m_strName = wstring(str.begin(), str.end());

	FScanf(szBuff, pFile);	// [Texture Name]
	FScanf(szBuff, pFile);	// 실제 텍스쳐 이름
	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());

	FScanf(szBuff, pFile);	// [Texture Path]
	FScanf(szBuff, pFile);	// 실제 텍스쳐 경로
	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());

	m_pTex = CResourceMgr::GetInst()->LoadTexture(strTexKey, strTexPath);

	FScanf(szBuff, pFile);	// [Frame Count]
	int iFrameCount = 0;
	fscanf_s(pFile, "%d", &iFrameCount);

	// 모든 프레임 정보
	tAnimFrm frm = {};
	for (int i = 0; i < iFrameCount; ++i)
	{
		POINT pt = {};

		while (true)
		{
			FScanf(szBuff, pFile);



			if (!strcmp("[Frame Index]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
			}
			else if (!strcmp("[Left Top]", szBuff))
			{
				fscanf_s(pFile, "%d, %d", &pt.x, &pt.y);
				//fscanf_s(pFile, "%d", &pt.y);

				frm.vLT = Vec2(pt);
			}
			else if (!strcmp("[Slice Size]", szBuff))
			{
				fscanf_s(pFile, "%d, %d", &pt.x, &pt.y);
				//fscanf_s(pFile, "%d", &pt.y);

				frm.vSlice = pt;
			}
			else if (!strcmp("[Offset]", szBuff))
			{
				fscanf_s(pFile, "%d, %d", &pt.x, &pt.y);
				//fscanf_s(pFile, "%d", &pt.y);
				
				frm.vOffset = pt;
			}
			else if (!strcmp("[Duration]", szBuff))
			{
				fscanf_s(pFile, "%f", &frm.fDuration);
				break;
			}
		}

		m_vecFrm.push_back(frm);
	}

	fclose(pFile);
}
