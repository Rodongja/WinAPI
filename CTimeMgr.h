#pragma once

class CTimeMgr
{
private:
	SINGLE(CTimeMgr);
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llFrequency;

	double			m_dDT; // 프레임 간의 시간 값
	double			m_dAcc; // 1초 체크를 위한 누적된 DT값
	UINT			m_iCallCount; // 초당 함수 호출 횟수
	UINT			m_iFPS; // 초당 프레임

	// FPS
	// 1. 프레임당 시간 Delta Time (유니티 Time.deltaTime)

public:
	//QueryPerformanceCounter(); // 100만단위 GetTickCount는 1000단위
	void init();
	void update();
	void render();

public:
	double GetDT() { return m_dDT; }
	float GetfDT() { return (float)m_dDT; }
};

