#include "pch.h"
#include "CScene.h"

#include "CObject.h"
#include "CTile.h"
#include "CResourceMgr.h"
#include "CPathMgr.h"

#include "CCore.h"
#include "CCamera.h"

CScene::CScene()
	: m_iTileX(0)
	, m_iTileY(0)
	, m_pPlayer(nullptr)
{
}

CScene::~CScene()
{
	// 모든 벡터 선택
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		// 해당 벡터의 모든 오브젝트 선택
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// m_arrObj[i] 그룹 벡터의 j 오브젝트 삭제
			delete m_arrObj[i][j];
		}
	}

	// m_arrObj[i] 벡터(Default, NPC, Player...)는 자동으로 소멸(delete)
	// 직접 동적할당한 m_arrObj[i][j]는 m_arrObj[i]에서 주소값을 갖고있기 때문에 직접 삭제해줘야함
}

void CScene::start()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->start();
		}
	}
}

void CScene::update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (!m_arrObj[i][j]->IsDead())
			{
				m_arrObj[i][j]->update();
			}
		}
	}
}

void CScene::finalupdate()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->finalupdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		if ((UINT)GROUP_TYPE::TILE == i)
		{
			render_tile(_dc);
			continue;
		}

		vector<CObject*>::iterator iter = m_arrObj[i].begin();

		// 만약 Tile도 도중에 삭제될 가능성이 있다면 데드체크를 해서 빼줘야함
		for (; iter != m_arrObj[i].end();)
		{
			if (nullptr == *iter)
			{
				++iter;
			}
			else if (!(*iter)->IsDead())
			{
				(*iter)->render(_dc);
				++iter;
			}
			else
			{
				delete* iter;
				*iter = nullptr;
				iter = m_arrObj[i].erase(iter);
			}
		}
	}
}

void CScene::RemoveObject(CObject* pObj)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		auto iter = std::find(m_arrObj[i].begin(), m_arrObj[i].end(), pObj);
		if (iter != m_arrObj[i].end())
		{
			m_arrObj[i].erase(iter);
			break;
		}
	}
}

void CScene::render_tile(HDC _dc)
{
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution / 2.f;

	int iTileSize = TILE_SIZE;

	int iLTCol = (int)vLeftTop.x / iTileSize; // 열
	int iLTRow = (int)vLeftTop.y / iTileSize; // 행
	int iLTIdx = (m_iTileX * iLTRow) + iLTCol;

	// 빈 공간 방지
	// 카메라가 정확히 타일과 겹치지 않을때 1개의 여백이 발생할 수 있음
	int iClientWidth = ((int)vResolution.x / iTileSize)+1;
	int iClientHeight = ((int)vResolution.y / iTileSize)+1;

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); ++iCurRow)
	{
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); ++iCurCol)
		{
			if (iCurCol < 0 || m_iTileX <= iCurCol || iCurRow < 0 || m_iTileY <= iCurRow)
			{
				continue;
			}

			int iIdx = (m_iTileX * iCurRow) + iCurCol;

			vecTile[iIdx]->render(_dc);
		}
	}

	// 만약 Tile도 도중에 삭제될 가능성이 있다면 render에서 한 것처럼 데드체크를 해서 빼줘야함

	// 텍스처를 하나 만들고 BitBlt으로 복사해 오면 더 최적화 가능
}

void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	//Safe_Delete_Vec<CObject*>(m_arrObj[(UINT)_eTarget]);		
	Safe_Delete_Vec(m_arrObj[(UINT)_eTarget]);					// 함수 템플릿
}

void CScene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

void CScene::CreateTile(UINT _iXCount, UINT _iYCount)
{
	// 기존 있던 타일 삭제
	DeleteGroup(GROUP_TYPE::TILE);

	m_iTileX = _iXCount;
	m_iTileY = _iYCount;

	CTexture * pTileTex = CResourceMgr::GetInst()->LoadTexture(L"Title", L"texture\\tile\\TILE.bmp");

	// 타일 생성
	for (UINT i = 0; i < _iYCount; ++i)
	{
		for (UINT j = 0; j < _iXCount; ++j)
		{
			CTile* pTile = new CTile();
			pTile->SetPos(Vec2((float)(j * TILE_SIZE), (float)(i * TILE_SIZE)));
			pTile->SetTexture(pTileTex);
			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}
}

void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;
	// 커널 오브젝트
	FILE* pFile = nullptr;

	// 2중포인터 필요
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb"); // 파일 개방 모드 w = 쓰기 r = 읽기 b = bynary

	assert(pFile);
	
	// 타일 가로세로 개수 불러오기
	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	// 불러온 개수에 맞게 EmptyTile들 만들어두기
	CreateTile(xCount, yCount);

	// 만들어진 타일 개별로 필요한 정보를 불러오게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}

	fclose(pFile);
}
