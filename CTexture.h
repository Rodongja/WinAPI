#pragma once
#include "CResource.h"
class CTexture :
    public CResource
{
private:
    HDC         m_dc;       // 리소스의 dc
    HBITMAP     m_hBit;
    BITMAP      m_bitInfo;

public:
    void Load(const wstring& _strFilePath);
    void Create(UINT _iWidth, UINT _iHeight);

    UINT Width() { return m_bitInfo.bmWidth; }
    UINT Height() { return m_bitInfo.bmHeight; }

    HDC GetDC() { return m_dc; }
private:
    CTexture();
    virtual ~CTexture();

    friend class CResourceMgr;
};

