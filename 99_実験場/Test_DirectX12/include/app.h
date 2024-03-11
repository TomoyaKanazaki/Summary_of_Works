#pragma once // 2重インクルード防止

//==========================================
//
//  アプリケーションクラス(app.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include <Windows.h>
#include <cstdint>

//==========================================
//  前方宣言
//==========================================
class CRenderer; // レンダラークラス

//==========================================
//  クラス定義
//==========================================
class CApp
{
public:

	//メンバ関数
	CApp(uint32_t width, uint32_t height); // コンストラクタ
	~CApp(); // デストラクタ

	void Run();

	// 静的メンバ関数
	static CApp* GetInstance();

private:

	// メンバ関数
	bool InitApp(); // 
	void TermApp(); // 
	bool InitWnd(); // 
	void TermWnd(); // 
	void MainLoop(); // 

	// 静的メンバ関数
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

	// メンバ変数
	HINSTANCE m_hInst; // インスタンスハンドル
	HWND m_hWnd; // ウィンドウハンドル
	uint32_t m_Width; // ウィンドウの横幅
	uint32_t m_Height; // ウィンドウの縦幅
	CRenderer* m_pRenderer; // レンダラー

	// 静的メンバ変数
	static CApp* m_pApp;

};