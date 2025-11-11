#pragma once
#include "CObject.h"

enum class PLAYER_STATE
{
    IDLE,
    WALK,
    ATTACK,
    JUMP,
    HIT,
    DEAD
};

enum class PLAYER_ATTACK_STATE
{
    NORMAL_ATT_1,
    NORMAL_ATT_2,
    NORMAL_ATT_3,

    SKILL_ATT_1,
    // ....
};

class CTexture;

class CPlayer :
    public CObject
{
private:
    int                 MissileType;


    PLAYER_STATE        m_ePrevState;
    PLAYER_STATE        m_eCurState;

    int                 m_iDir;
    int                 m_iPrevDir;
    float               m_fSpeed;
    float               m_fJumpPower;
    int                 m_iMaxJump;
    int                 m_iJumpCnt;
    bool                b_isJump;

    Vec2                m_vTextureOffset;

public:
    unordered_set<CObject*> m_hTarget;

private:
    //CTexture* m_pTex;

public :
    virtual void update();
    virtual void render(HDC _dc);

private:
    void CreateMissile();
    void UpdateMove();
    void UpdateState();
    void UpdateAnimation();
    void updateGravity();

    virtual void OnCollisionEnter(CCollider* _pOther);
    //virtual void OnCollisionExit(CCollider* _pOther);

public:
    void SetMissileType(int _type);
    void FireMissile(const Vec2& dir);
    int GetJumpCount() { return m_iJumpCnt; }
    int GetMaxJump() { return m_iMaxJump; }
    void ResetJump() { m_iJumpCnt = m_iMaxJump; }
    void SetJump(bool _b) { b_isJump = _b; }

    void AddTarget(CObject* pTarget);
    void RemoveTarget(CObject* pTarget);

    CLONE(CPlayer);
public:
    CPlayer();

    // 구현 할 필요가 없음
    //CPlayer(const CPlayer& _origin)
    //    : CObject(_origin)            // 내가 만든 CObject 복사방식을 따라서 정상작동함
    //    , m_pTex(_origin.m_pTex)      // Texture는 여러 오브젝트가 하나의 리소스를 돌려쓰기위해 만든것이므로 주소값이 중복되어야 정상작동함
    //{}                                // 그러므로 Player는 따로 복사 생성자를 구현할 필요가 없음

    // 매크로 버전

    //// 함수 버전
    //CPlayer* Clone() { return new CPlayer(*this); }

    ~CPlayer();
};

