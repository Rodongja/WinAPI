#pragma once

class CResource;
class CTexture;
class CSound;

class CResourceMgr
{
	SINGLE(CResourceMgr);
private:
	map<wstring,CResource*>		m_mapTex;
	map<wstring,CResource*>		m_mapSound;

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strrRelativePath);
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);
	CTexture* FindTexture(const wstring& _strKey);

	CSound* LoadSound(const wstring& _strKey, const wstring& _strRelativePath);
	CSound* FindSound(const wstring& _strKey);
};

