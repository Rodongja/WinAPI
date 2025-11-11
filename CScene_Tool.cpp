#include "pch.h"
#include "CScene_Tool.h"
#include "CKeyMgr.h"
#include "CTile.h"

#include "CCore.h"
#include "CResourceMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"

#include "Resource.h"

#include "CUIMgr.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

void ChangeScene(DWORD_PTR, DWORD_PTR);

CScene_Tool::CScene_Tool()
	: m_pUI(nullptr)
{
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::Enter()
{
	// 툴 Scene에서 사용할 메뉴를 붙인다
	CCore::GetInst()->DockMenu();
	
	// 타일 생성
	CreateTile(5, 5);

	// UI 생성
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CUI* pPanelUI = new CPanelUI(pScene);
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(300.f,370.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));
	
	// 버튼 리스트 선언
	vector<CBtnUI*> vButtons;
	// 버튼 시작 지점
	Vec2 startPos(20.f, 20.f);
	// 버튼 크기
	Vec2 btnSize(50.f, 50.f);
	// 버튼 간격
	float gap = 20.f;
	// 버튼 행 개수
	int row = 5;
	// 버튼 열 개수
	int col = 4;
	// 버튼 인덱스 초기화
	int btnIdx = 0;

	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			// 새로운 버튼 생성
			CBtnUI* pBtnUI = new CBtnUI(this);

			// 인덱스를 포함하여 고유한 이름 설정
			std::wstring btnName = L"ChildUI_" + std::to_wstring(btnIdx);
			pBtnUI->SetName(btnName.c_str());
			pBtnUI->SetScale(btnSize);

			// 버튼 위치 설정
			float xPos = startPos.x + j * (btnSize.x + gap);
			float yPos = startPos.y + i * (btnSize.y + gap);
			pBtnUI->SetPos(Vec2(xPos, yPos));

			// 버튼 리스트에 추가
			vButtons.push_back(pBtnUI);
			pPanelUI->AddChild(pBtnUI); // 패널에 버튼 추가

			pBtnUI->SetParent(pPanelUI);


			if (btnIdx == 18)
			{
				// 18번 버튼을 누르면 세이브
				pBtnUI->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);
			}
			else if (btnIdx == 19)
			{
				// 19번 버튼을 누르면 닫기
				pBtnUI->SetClickedCallBack(pBtnUI, (UI_MEMFUNC)&CUI::DeleteAllUI);
			}


			++btnIdx;  // 버튼 인덱스 증가
		}
	}
	AddObject(pPanelUI, GROUP_TYPE::UI);



	//CBtnUI* pBtnUI = new CBtnUI;
	//pBtnUI->SetName(L"ChildUI");
	//pBtnUI->SetScale(Vec2(50.f, 50.f));
	//pBtnUI->SetPos(Vec2(0.f, 0.f));

	//// 멤버함수 포인터는 &를 반드시 써서 주소를 줘야함
	//// (SCENE_MEMFUNC)로 캐스팅을 반드시 해 줘야함
	//((CBtnUI*)pBtnUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData); 

	//pPanelUI->AddChild(pBtnUI);
	/*AddObject(pPanelUI, GROUP_TYPE::UI);*/

	//// 복사본 UI
	//CUI* pClonePanel = pPanelUI->Clone();
	//pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-300.f,0.f));
	 
	//// 전역함수 포인터는 &를 생략해도 됨. 정석적으로는 SetClickedCallBack(&ChangeScene, 0, 0);와같이 &를 사용해야함
	//((CBtnUI*)pClonePanel->GetChildUI()[0])->SetClickedCallBack(ChangeScene, 0, 0); 
	//AddObject(pClonePanel, GROUP_TYPE::UI);

	//m_pUI = pClonePanel;

	// Camera Look 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
}

void CScene_Tool::Exit()
{
	// 메뉴바 제거
	CCore::GetInst()->UndockMenu();

	DeleteAll();
}

void CScene_Tool::SetTileIdx()
{
	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

		int iTileX = (int)GetTileX();
		int iTileY = (int)GetTileY();

		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		if (vMousePos.x < 0.f || iTileX <= iCol || vMousePos.y < 0.f || iTileY <= iRow)
		{
			return;
		}

		UINT iIdx = iRow * iTileX + iCol;

		const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
		((CTile*)vecTile[iIdx])->AddImgIdx();
		
		// 범위 밖 클릭하면 vector에서 충돌이 발생하니 예외처리 필요
	}
}

void CScene_Tool::SaveTileData()
{
	wchar_t szName[256] = {}; // lpstrFile은 주소만 가리키기 때문에 실질적으로 저장이 가능한 배열이 필요함

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0; // 기본 필터
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	// Modal 방식
	// 창이 모든 포커싱을 가져가 다른 창들은 동작 하지 않음
	if (GetSaveFileName(&ofn))
	{
		CPathMgr::GetInst()->GetRelativePath(szName);
		SaveTile(szName);
	}
}

void CScene_Tool::SaveTile(const wstring& _strFilePath)
{
	// 커널 오브젝트
	FILE* pFile = nullptr;

	// 2중포인터 필요
	_wfopen_s(&pFile, _strFilePath.c_str(),L"wb"); // 파일 개방 모드 w = 쓰기 r = 읽기 b = binary

	// 파일 저장 실패
	assert(pFile);

	// 타일 가로세로 개수 저장
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);

	// 모든 타일들을 개별적으로 저장할 데이터를 저장하게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Save(pFile);
	}
	
	fclose(pFile);
}

void CScene_Tool::LoadTileData()
{
	wchar_t szName[256] = {}; // lpstrFile은 주소만 가리키기 때문에 실질적으로 저장이 가능한 배열이 필요함

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0; // 기본 필터
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal 방식
	// 창이 모든 포커싱을 가져가 다른 창들은 동작 하지 않음
	if (GetOpenFileName(&ofn))
	{
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	}
}

void CScene_Tool::update()
{
	CScene::update();

	SetTileIdx();

	//if (KEY_TAP(KEY::LSHIFT))
	//{
	//	// UI포커스 변경
	//	//CUIMgr::GetInst()->SetFocusedUI(m_pUI);
	//	
	//	// 타일 저장
	//	SaveTileData();
	//}

	if (KEY_TAP(KEY::CTRL))
	{
		LoadTileData();
	}


	//if (KEY_TAP(KEY::ENTER))
	//{
	//	ChangeScene(SCENE_TYPE::START);
	//}
}

// 씬 변경 함수는 인자가 필요 없으니 형태만 맞춰줌
void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
}


// ======================
// Tile Count Window Proc
// ======================
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) // CALLBACK == _stdcall 함수 호출 규약
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)  
		{
			// 분기 처리는 2가지 방식이 가능함
			// 1. 리턴을 하기전에 if문으로 해결
			// 2. LOWORD값을 리턴하여 상위 함수에서 if문으로 해결
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false); // GetDlgItemInt는 문자열 데이터를 숫자로 받아옴
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			// ToolScene 확인
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene); // dynamic_cast는 캐스팅이 실패하면 nullptr이 나옴
			assert(pToolScene);

			pToolScene->DeleteGroup(GROUP_TYPE::TILE);
			pToolScene->CreateTile(iXCount, iYCount);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}