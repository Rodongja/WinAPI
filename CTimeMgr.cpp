#include "pch.h"
#include "CTimeMgr.h"

#include "CCore.h"

CTimeMgr::CTimeMgr()
	: m_llCurCount{}
	, m_llPrevCount{}
	, m_llFrequency{}
	, m_dDT(0.)
	, m_dAcc(0.)
	, m_iCallCount(0)
	, m_iFPS(0)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	//// 유니온
	//// 모든 변수는 메모리 공간을 공유함
	//m_llPrevCount.QuadPart;		// 8바이트
	//m_llPrevCount.u;				// 8바이트
	//m_llPrevCount.u.HighPart;		// 4바이트
	//m_llPrevCount.u.LowPart;		// 4바이트
	//m_llPrevCount.HighPart;		// 4바이트
	//m_llCurCount.LowPart;			// 4바이트

	// 현재 카운트
	QueryPerformanceCounter(&m_llPrevCount);

	// 초당 카운트 횟수(1000만)
	QueryPerformanceFrequency(&m_llFrequency);
}

void CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	// 이전 프레임의 카운팅과 현재 프레임의 카운팅 값의 차이를 구한다.
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;

	// 이전 카운트 값을 현재 값으로 갱신 (다음번 계산을 위해서)
	m_llPrevCount = m_llCurCount;

#ifdef _DEBUG
	// 포커싱에서 벗어나도 DT가 일정값 이상 못흐르게 설정 (모달방식 창)
	if (m_dDT > (1. / 60.))
	{
		m_dDT = (1. / 60.);
	}
#endif

}

void CTimeMgr::render()
{
	// FPS 계산을 위해 초당 몇번 호출되는지 세기
	++m_iCallCount;
	m_dAcc += m_dDT; // DT 누적

	if (m_dAcc >= 1.)
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d,  DT : %1f", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);
	}
}
