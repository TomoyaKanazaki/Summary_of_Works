//==========================================
//
//  初めてのDirectX12(main.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "app.h"
#include "utility.h"

//==========================================
//  メイン関数
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hInstancePrev*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
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
