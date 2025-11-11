#include "pch.h"
#include "func.h"

#include "CEventMgr.h"

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;
	evn.wParam = (DWORD_PTR)_eGroup;

	CEventMgr::GetInst()->AddEvent(evn);
}

void DeleteObject(CObject* _pObj)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)_eNext;

	CEventMgr::GetInst()->AddEvent(evn);
}

void ChageAIState(AI* _pAI, MON_STATE _eNextState)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CHANGE_AI_STATE;
	evn.lParam = (DWORD_PTR)_pAI;
	evn.wParam = (DWORD_PTR)_eNextState;

	CEventMgr::GetInst()->AddEvent(evn);
}

void FScanf(char* _pOutBuff, FILE* _pFile)
{
	int i = 0;
	while (i < 255)
	{
		char c = (char)getc(_pFile);
		if (c == '\n' || c == EOF)
		{
			_pOutBuff[i] = '\0';
			break;
		}
		_pOutBuff[i++] = c;
	}
}

void SaveWString(const wstring& _str, FILE* _pFile)
{
	// 주소값 그 자체가 아닌 주소값이 가리키는 데이터가 필요한것이므로 그 데이터를 저장해야함
	//fwrite(&m_strName, sizeof(wstring), 1, pFile); // 잘못된 wstring 저장 방식
	const wchar_t* pStrName = _str.c_str();
	size_t iLen = _str.length();

	// 문자 길이 저장
	fwrite(&iLen, sizeof(size_t), 1, _pFile);

	// 문자 저장
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);
}

void LoadWString(wstring& _str, FILE* _pFile)
{
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);

	// 애니메이션 이름의 최대 길이(버퍼 크기)
	wchar_t szBuff[256] = {};

	fread(szBuff, sizeof(wchar_t), iLen, _pFile);

	_str = szBuff;
}

