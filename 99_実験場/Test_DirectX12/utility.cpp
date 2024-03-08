//==========================================
//
//  実用的名前空間(utility.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "utility.h"

//==========================================
//  セーフリリース
//==========================================
template<class T>void Utility::SafeRelease(T* pT)
{
    if (*pT)
    {
        (*pT)->Release();
        *pT = nullptr;
    }
}
