#pragma once
#include "CObject.h"

struct tMonInfo
{
    float fSpeed;         // 속도
    float fHP;            // 체력
    float fRecogRange;    // 인지 범위
    float fAttRange;      // 공격 범위
    float fAtt;           // 공격력
};

class AI;

class CMonster :
    public CObject
{
private:
    tMonInfo m_tInfo;
    AI*     m_pAI;

public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    //void SetMoveDistance(float _f) { m_fMaxDistance = _f; }
    //void SetCenterPos(Vec2 _vPos) { m_vCenterPos = _vPos; }
    //Vec2 GetCenterPos() { return m_vCenterPos; }
    const tMonInfo& GetInfo() { return m_tInfo; }

    void SetAI(AI* _AI);

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

private:
    void CreateMissile();

    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }

public:
    void FireMissile(const Vec2& dir);

public:
    virtual void update();

    // 매크로 버전
    CLONE(CMonster)

    //// 함수 버전
    //CMonster* Clone() { return new CMonster(*this); }

public:
    CMonster();
    ~CMonster();

    friend class CMonFactory;
};

