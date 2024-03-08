#pragma once // 2重インクルード防止

//==========================================
//
//  実用的名前空間(utility.h)
//  Author : Tomoya Kanazaki
//
//==========================================
 
//==========================================
//  名前空間定義
//==========================================
namespace Utility
{
    // セーフリリース
	template <class T> void SafeRelease(T** ppT)
    {
        if (*ppT)
        {
            (*ppT)->Release();
            *ppT = nullptr;
        }
    }
}
