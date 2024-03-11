//==========================================
//
//  実用的名前空間(utility.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "utility.h"
#include <assert.h>

//==========================================
//  マイアサート(trueの時に止まる)
//==========================================
void Utility::MyAssert(bool bFrag)
{
	assert(!bFrag);
}
