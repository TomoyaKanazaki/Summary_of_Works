//==========================================
//
//  ���߂Ă�DirectX12(main.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#if defined(DEBUG) || defined(_DEBUG)

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif

#include "app.h"
#include "utility.h"

//==========================================
//  ���C���֐�
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hInstancePrev*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// �A�v���P�[�V�����̃C���X�^���X�𐶐�
	CApp* pApp = CApp::GetInstance();

	// NULL�`�F�b�N
	Utility::MyAssert(pApp == nullptr);

	// �A�v���P�[�V���������s
	pApp->Run();

	// �A�v���P�[�V�����̔j��
	if (pApp != nullptr)
	{
		delete pApp;
		pApp = nullptr;
	}

	return 0;
}
