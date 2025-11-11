#pragma once

class CObject;
class AI;

void CreateObject(CObject* _pObj,GROUP_TYPE _eGroup);
void DeleteObject(CObject* _pObj);
void ChangeScene(SCENE_TYPE _eNext);
void ChageAIState(AI* _pAI, MON_STATE _eNextState);


template<typename T>
void Safe_Delete_Vec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
		{
			delete _vec[i];
			_vec[i] = nullptr; // 포인터를 nullptr로 초기화
		}
	}
	_vec.clear();
}

//template<typename T1, typename T2>
//void Safe_Delete_Map(map<T1, T2>& _map)
//{
//	// template 내에서 이너클래스 사용하기 위해 typename 입력해야함(문법)
//	typename map<T1, T2>::iterator iter = _map.begin();
//
//	for (; iter != _map.end(); ++iter)
//	{
//		if (nullptr != iter->second)
//		{
//			delete iter->second;
//		}
//		_map.clear();
//	}
//}

template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	// template 내에서 이너클래스 사용하기 위해 typename 입력해야함(문법)
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end();)
	{
		if (nullptr != iter->second)
		{
			delete iter->second;
			iter->second = nullptr;
		}
		iter = _map.erase(iter);
	}
}

void FScanf(char* _pOutBuff, FILE* _pFile);

void SaveWString(const wstring& _str, FILE* _pFile);

void LoadWString(wstring& _str, FILE* _pFile);