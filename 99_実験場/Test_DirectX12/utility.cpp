//==========================================
//
//  ���p�I���O���(utility.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "utility.h"

//==========================================
//  �Z�[�t�����[�X
//==========================================
template<class T>void Utility::SafeRelease(T* pT)
{
    if (*pT)
    {
        (*pT)->Release();
        *pT = nullptr;
    }
}
