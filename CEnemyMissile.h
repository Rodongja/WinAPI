#pragma once
#include "CMissile.h"

enum class TYPE 
{
    TYPE1,          // 타입을 추가하고싶다면 이곳에서 TYPE2 TYPE3 이렇게 추가만 하면 됨
    TYPE_COUNT
};

class CEnemyMissile :
    public CMissile
{
private: 
    TYPE type;
    static const int type_count;
       
public: 
    TYPE GetType() const;
    void SetType(TYPE _type);

public:
    CEnemyMissile();
    ~CEnemyMissile();
};

