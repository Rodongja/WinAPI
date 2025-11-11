#include "pch.h"
#include "CResourceMgr.h"

#include "CPathMgr.h"
#include "CTexture.h"

CResourceMgr::CResourceMgr()
{

}

CResourceMgr::~CResourceMgr()
{
	//map<wstring, CTexture*>::iterator iter = m_mapTex.begin();
	//for (; iter != m_mapTex.end(); ++iter)
	//{
	//	delete iter->second;
	//}
	Safe_Delete_Map(m_mapTex);
}

CTexture* CResourceMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	pTex = new CTexture;
	pTex->Load(strFilePath);
	pTex->SetKey(_strKey);
	pTex->SetRelativePath(_strRelativePath);

	

	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}

CTexture* CResourceMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	pTex = new CTexture;
	pTex->Create(_iWidth,_iHeight);
	pTex->SetKey(_strKey);

	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}

CTexture* CResourceMgr::FindTexture(const wstring& _strKey)
{
	map<wstring, CResource*>::iterator iter = m_mapTex.find(_strKey);

	if (iter == m_mapTex.end())
	{
		return nullptr;
	}

	return (CTexture*)iter->second;
}

CSound* CResourceMgr::LoadSound(const wstring& _strKey, const wstring& _strRelativePath)
{
	return nullptr;
}

CSound* CResourceMgr::FindSound(const wstring& _strKey)
{
	return nullptr;
}
