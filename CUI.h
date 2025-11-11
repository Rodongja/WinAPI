#pragma once
#include "CObject.h"

class CScene;

class CUI :
    public CObject
{
private:
    vector<CUI*>    m_vecChildUI;
    CUI*            m_pParentUI;
    CScene*         m_pScene;

    Vec2            m_vFinalPos;

    bool            m_bCamAffected;     // UI가 카메라에 영향을 받는 유무
    bool            m_bMouseOn;         // UI 위에 마우스가 있는지 유무
    bool            m_bLbtnDown;        // UI에 왼쪽버튼이 눌린적이 있는지 유무

public:
    Vec2 GetFinalPos() { return m_vFinalPos; }    
    CUI* GetParent() { return m_pParentUI; }
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }

    void AddChild(CUI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; }
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }
    vector<CUI*>& GetChildUINonConst() { return m_vecChildUI; }
    //vector<CUI*>& GetChildUI() { return m_vecChildUI; }
    //const vector<CUI*>& GetChildUI() const { return m_vecChildUI; }

public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(HDC _dc);

private:
    void update_child();
    void finalupdate_child();
    void render_child(HDC _dc);

    void MouseOnCheck();

public:
    virtual void MouseOn();

    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    // 순수 가상함수 상태로 만들어 하위 클래스인 CPanelUI와 CBtnUI에서 Clone() 함수가 실행되게함
    virtual CUI* Clone() = 0;

    // 내가 만든 UI 삭제 함수
public:

    //void DeleteSelfAndAllChildUI();
    void SetParent(CUI* _pParent) { m_pParentUI = _pParent; }
    void DeleteAllUI();
    void DeleteThisUI();
    void DeleteAllChildrenUI();
    void RemoveChild(CUI* _pChild);


public:
    CUI(bool _bCamAff, CScene* _pScene);
    CUI(const CUI& _origin);
    virtual ~CUI(); // CObject에서 파생

    friend class CUIMgr;
};

