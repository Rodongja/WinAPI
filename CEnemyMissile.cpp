#include "pch.h"
#include "CEnemyMissile.h"

const int CEnemyMissile::type_count = static_cast<int>(TYPE::TYPE_COUNT);


CEnemyMissile::CEnemyMissile()
	: type(TYPE::TYPE1)
{
}

CEnemyMissile::~CEnemyMissile()
{
}

TYPE CEnemyMissile::GetType() const
{
	return type;
}

void CEnemyMissile::SetType(TYPE _type)
{
	type = _type;
}