#pragma once
#include "CState.h"
class CPatrolState :
    public CState
{
private:
    
public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    CPatrolState();
    ~CPatrolState();
};

