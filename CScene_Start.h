#pragma once
#include "CScene.h"

class CScene_Start :
    public CScene
{
private:
    Vec2    m_vForcePos;
    float   m_fForceRadius;
    float   m_fCurRadius;
    float   m_fForce;
    bool    m_bUseForce;

public: 
    // void Start();와 같이 virtual을 생략해도 자동으로 가상함수로 적용되나
    // 가상함수임을 강조하기위해 자식클래스 또한 명시적으로 virtual을 써줘야 이해가 쉽다
    virtual void update();
    virtual void render(HDC _dc);

    virtual void Enter(); 
    virtual void Exit();

public:
    void CreateForce();


public:
    CScene_Start();
    ~CScene_Start();
};

