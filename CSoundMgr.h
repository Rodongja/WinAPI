#pragma once
// sound 용도 - 헤더 순서 중요
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

// 코드
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

class CSound;

class CSoundMgr
{
	SINGLE(CSoundMgr);

private:
	// 사운드 매니저는 로딩한 사운드를 관리하지 않음
	// 로딩한 사운드는 리소스 이므로 리소스 매니저에서 관리함
	LPDIRECTSOUND8	m_pSound;	// 사운드카드 대표 객체
	CSound* m_pBGM;		// 현재 지정된 BGM Sound

public:
	int init(void);
	LPDIRECTSOUND8 GetSoundDevice() { return m_pSound; }
	void RegisterToBGM(CSound* _pSound);
};