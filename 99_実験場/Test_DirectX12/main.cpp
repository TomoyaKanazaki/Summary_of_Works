//==========================================
//
//  ���߂Ă�DirectX12(main.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "app.h"
#include "utility.h"

//==========================================
//  ���C���֐�
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hInstancePrev*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
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
