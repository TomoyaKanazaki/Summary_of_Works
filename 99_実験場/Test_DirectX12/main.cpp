//==========================================
//
//  ���߂Ă�DirectX12(main.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "app.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int SCREEN_WIDTH = 1280; // �E�B���h�E�̉���
	const int SCREEN_HEIGHT = 720; // �E�B���h�E�̍���
}

//==========================================
//  ���C���֐�
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hInstancePrev*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	// �A�v���P�[�V���������s
	App app(SCREEN_WIDTH, SCREEN_HEIGHT);
	app.Run();

	return 0;
}
