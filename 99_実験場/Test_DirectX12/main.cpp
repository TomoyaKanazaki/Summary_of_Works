//==========================================
//
//  初めてのDirectX12(main.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "app.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int SCREEN_WIDTH = 1280; // ウィンドウの横幅
	const int SCREEN_HEIGHT = 720; // ウィンドウの高さ
}

//==========================================
//  メイン関数
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hInstancePrev*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	// アプリケーションを実行
	App app(SCREEN_WIDTH, SCREEN_HEIGHT);
	app.Run();

	return 0;
}
