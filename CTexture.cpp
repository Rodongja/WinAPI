#include "pch.h"
#include "CTexture.h"
#include "CCore.h"

#include "iostream"

CTexture::CTexture()
	: m_hBit(0)
	, m_dc(0)
	, m_bitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}


void CTexture::Load(const wstring& _strFilePath)
{
	// 파일로부터 로딩한  데이터를 비트맵으로 생성
	m_hBit = (HBITMAP)LoadImageW(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	assert(m_hBit);

	// 비트맵과 연결할 DC
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	// 비트맵과 DC 연결
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);

	// 비트맵 정보
	// BITMAP은 BITMAP의 정보(가로,세로 등)를 저장하는 구조체타입
	GetObject(m_hBit,sizeof(BITMAP),&m_bitInfo);
}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	HDC maindc = CCore::GetInst()->GetMainDC();

	// 이중 버퍼링 용도의 비트맵과 DC를 만든다
	m_hBit = CreateCompatibleBitmap(maindc, _iWidth, _iHeight);
	m_dc = CreateCompatibleDC(maindc);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	
	// 새로 비트맵을 전달받으면 기존의 비트맵(1픽셀)은 필요없으므로 삭제
	DeleteObject(hOldBit);

	// 비트맵 정보
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}
