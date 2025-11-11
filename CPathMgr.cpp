#include "pch.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr()
	:m_szContentPath{}
{

}

CPathMgr::~CPathMgr()
{

}

void CPathMgr::init()
{
	// 현재 경로 가져오기
	GetCurrentDirectory(255,m_szContentPath);
	
	int iLen = (int)wcslen(m_szContentPath);

	// 상위폴더로 
	for (int i = iLen - 1; 0 <= i; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");

	// + bin\\content\\

	SetWindowText(CCore::GetInst()->GetMainHwnd(), m_szContentPath);


}

wstring CPathMgr::GetRelativePath(const wchar_t* _filepath)
{
	wstring strFilePath = _filepath;

	//int iPos = strFilePath.rfind(m_szContentPath, strFilePath.length());

	size_t iAbsLen = wcslen(m_szContentPath);
	size_t iFullLen = strFilePath.length();

	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);

	return strRelativePath;
}
