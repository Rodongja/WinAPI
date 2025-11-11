#pragma once

#include "global.h"

#include "CMonFactory.h"


// 컴파일 속도를 빠르게 하기 위해 전방선언을 함
class CObject;

class CScene
{
private:
	// CObject는 전방선언을 했기 때문에 포인터로만 접근이 가능
	// CObject*로 접근을 하면 모든 CObject 자식타입에 접근이 가능

	//// vector가 아니라 트리같은 다른 자료구조를 써도 되지 않을까?

	vector<CObject*>	m_arrObj[(UINT)GROUP_TYPE::END]; // 오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언
	
	//m_arrObj[(UINT)GROUP_TYPE::PLAYER];

	wstring				m_strName;	//Scene 이름

	UINT				m_iTileX;	// 타일 가로 개수
	UINT				m_iTileY;	// 타일 세로 개수

	CObject*			m_pPlayer;	// Player

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	CObject* GetPlayer() { return m_pPlayer; }

	// 모든 Scene들의 update, render가 똑같을것이기 때문에 오버라이딩이 필요 없으므로 virtual 선언을 하지 않아도 됨
	virtual void start(); // Scene이 시작되기 직전에 호출되는 함수
	virtual void update();
	virtual void finalupdate();
	virtual void render(HDC _dc);

	void render_tile(HDC _dc);

	// 순수 가상함수
	// 부모클래스에 해당하는 Scene은 직접 개체 생성이 불가능함
	// 순수 가상함수를 갖고있는 클래스를 추상클래스라고 함
	// 실제로 객체화 할 수 있는게 아니라 상속을 통해 구현해야할 함수를 알리는 목적(interface)
	// 자식 클래스가 반드시 순수 가상함수를 구현하지 않으면 정상적인 실행이 불가능함

	virtual void Enter() = 0;	// 해당 Scene에 진입 시 호출
	virtual void Exit() = 0;	// 해당 Scene을 탈출 시 호출

//protected:
//	// 함수를 헤더에 구현하면 inline처리가 되어 호출비용이 들지 않음
//	void AddObject(CObject* _pObj, GROUP_TYPE _eType)
//	{
//		m_arrObj[(UINT)_eType].push_back(_pObj);
//	}

public:
	// 함수를 헤더에 구현하면 inline처리가 되어 호출비용이 들지 않음
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_arrObj[(UINT)_eType].push_back(_pObj);	_pObj->SetType(_eType); }
	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }

	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();
	void CreateTile(UINT _iXCount, UINT _iYCount);
	void LoadTile(const wstring& _strRelativePath);

	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

	void RemoveObject(CObject* pObj);

public:

public:
	CScene();
	virtual ~CScene();
};

