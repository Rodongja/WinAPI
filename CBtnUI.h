#pragma once
#include "CUI.h"

// 멤버함수 포인터 를 실제로 사용할 때 해당 클래스의 정의가 필요함. 전방선언만 하면 부족함.
#include "CScene.h"
#include "CObject.h"

//class CScene;
//class CObject;

typedef void(*BTN_FUNC) (DWORD_PTR, DWORD_PTR); // 함수 포인터

//// 56화 10:30 )   기본적으로 EnginObj같은 공통적인 부모를 하나 두고 ccore와 같은 모든 클래스를 상속시키는게 구조상 설계가 쉬움.
////                EnginObj와 같이 설계를 하면 typedef void(CEngineObj::*MEMFUNC)(void);와 같이 활용이 쉬워짐
//typedef void(CScene_Tool::*TOOL_SCENE_MEMFUNC)(void);
typedef void(CScene::* SCENE_MEMFUNC)(void);
typedef void(CUI::* UI_MEMFUNC)(void);

class CBtnUI :
    public CUI
{
private:
    BTN_FUNC        m_pFunc;
    DWORD_PTR       m_param1;
    DWORD_PTR       m_param2;

    SCENE_MEMFUNC   m_pSceneFunc;
    UI_MEMFUNC  m_pUIFunc;
    CScene* m_pSceneInst;
    CUI* m_pUIInst;

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();
    
    void SetClickedCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2);

    // 멤버함수 포인터
    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc);
    void SetClickedCallBack(CUI* _pUI, UI_MEMFUNC _pObjectFunc);

    CLONE(CBtnUI);

public:
    CBtnUI(CScene* _pScene);
    ~CBtnUI();

};

