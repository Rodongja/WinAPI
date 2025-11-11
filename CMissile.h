#pragma once
#include "CObject.h"
class CMissile :
    public CObject
{

private:
    float       m_fTheta; // 이동 방향
    float       m_fSpeed;

    Vec2 m_vDir;

public:
    void SetDir(bool _fTheta) { m_fTheta = _fTheta; }

    void SetDir(Vec2 _vDir) { m_vDir = _vDir; m_vDir.Normalize(); }

public:
    virtual void update();
    virtual void render(HDC _dc);

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

    // 매크로 버전
    CLONE(CMissile);

    //// 함수 버전
    //CMissile* Clone() { return new CMissile(*this); }

public:
    CMissile();
    ~CMissile();
};

