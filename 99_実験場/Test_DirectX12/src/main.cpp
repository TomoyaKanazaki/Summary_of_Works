//==========================================
//
//  初めてのDirectX12(main.cpp)
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
//  メイン関数
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hInstancePrev*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// アプリケーションのインスタンスを生成
	CApp* pApp = CApp::GetInstance();

	// NULLチェック
	Utility::MyAssert(pApp == nullptr);

	// アプリケーションを実行
	pApp->Run();

	// アプリケーションの破棄
	if (pApp != nullptr)
	{
		delete pApp;
		pApp = nullptr;
	}

	return 0;
}
