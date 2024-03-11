//==========================================
//
//  アプリケーションクラス(app.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "app.h"
#include "renderer.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int SCREEN_WIDTH = 1280; // ウィンドウの横幅
	const int SCREEN_HEIGHT = 720; // ウィンドウの高さ
	const auto ClassName = TEXT("WindowClass"); // ウィンドウクラスネーム
	const auto WindowName = TEXT("DirectX12Test"); // ウィンドウネーム
}

//==========================================
//  静的メンバ変数宣言
//==========================================
CApp* CApp::m_pApp = nullptr;

//==========================================
//  コンストラクタ
//==========================================
CApp::CApp(uint32_t width, uint32_t height) :
	m_hInst(nullptr),
	m_hWnd(nullptr),
	m_Width(width),
	m_Height(height),
	m_pRenderer(nullptr)
{
	// Do Nothing
}

//==========================================
//  デストラクタ
//==========================================
CApp::~CApp()
{
	// Do Nothing
}

//==========================================
//  CAppクラスのインスタンス取得
//==========================================
CApp* CApp::GetInstance()
{
	//インスタンス生成
	if (m_pApp == nullptr)
	{
		m_pApp = new CApp(SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	return m_pApp;
}

//==========================================
//  初期化処理
//==========================================
bool CApp::InitApp()
{
	// ウィンドウの初期化
	if (!InitWnd()) { return false; } // 初期化に失敗

	// レンダラーの取得
	m_pRenderer = CRenderer::GetInstance();

	// レンダラーの初期化
	if (!m_pRenderer->Init(m_hWnd, m_Width, m_Height)) { return false; } // 初期化に失敗

	// 正常終了
	return true;
}

//==========================================
//  ウィンドウの初期化
//==========================================
bool CApp::InitWnd()
{
	// インスタンスハンドルの取得
	auto hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr) { return false; } // 初期化に失敗

	// ウィンドウの設定
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = ClassName;
	wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

	// ウィンドウの登録
	if (!RegisterClassEx(&wc)) { return false; } // 初期化に失敗

	// インスタンスハンドル設定
	m_hInst = hInst;

	// ウィンドウのサイズを設定
	RECT rc = {};
	rc.right = static_cast<LONG>(m_Width);
	rc.bottom = static_cast<LONG>(m_Height);

	// ウィンドウサイズを調整
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, FALSE);

	// ウィンドウを生成
	m_hWnd = CreateWindowEx
	(
		0,
		ClassName,
		WindowName,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		m_hInst,
		nullptr
	);

	// NULLチェック
	if (m_hWnd == nullptr) { return false; }

	// ウィンドウを表示
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	// ウィンドウを更新
	UpdateWindow(m_hWnd);

	// ウィンドウにフォーカスを設定
	SetFocus(m_hWnd);

	// 正常終了
	return true;
}

//==========================================
//  ウィンドウプロシージャ
//==========================================
LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wp, lp);
}

//==========================================
//  終了処理
//==========================================
void CApp::TermApp()
{
	// レンダラーの終了処理
	m_pRenderer->Term();

	// ウィンドウの終了処理
	TermWnd();
}

//==========================================
//  ウィンドウの終了処理
//==========================================
void CApp::TermWnd()
{
	// ウィンドウの登録を解除
	if (m_hInst != nullptr)
	{
		UnregisterClass(ClassName, m_hInst);
	}

	m_hInst = nullptr;
	m_hWnd = nullptr;
}

//==========================================
//  実行
//==========================================
void CApp::Run()
{
	// 初期化処理
	if (InitApp())
	{
		// メインループ
		MainLoop();
	}

	// 終了処理
	TermApp();
}

//==========================================
//  メインループ
//==========================================
void CApp::MainLoop()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 描画
			m_pRenderer->Render();
		}
	}
}
