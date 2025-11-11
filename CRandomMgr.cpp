#include "pch.h"
#include "CRandomMgr.h"

#include <random>

CRandomMgr::CRandomMgr()
	: gen(rd())
{
}
CRandomMgr::~CRandomMgr()
{
}

int CRandomMgr::GetRd(int min, int max)
{
	gen = std::mt19937(rd()); // gen 업데이트

	std::uniform_int_distribution<int> dis(min, max); // dis 초기화
	return dis(gen); // 무작위 숫자 생성
}

void CRandomMgr::init()
{
	gen = std::mt19937(rd()); // gen 초기화
}

void CRandomMgr::update()
{
}
