#pragma once

#include <random>

class CRandomMgr
{
	SINGLE(CRandomMgr);

private:
	std::random_device rd;
	std::mt19937 gen;

public:
	int GetRd(int min, int max);

	void init();
	void update();
};

