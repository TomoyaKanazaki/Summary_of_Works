//==========================================
//
//  レンダリングクラス(renderer.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "renderer.h"
#include "utility.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int REFLESH_RATE = 144; // リフレッシュレート
}

//==========================================
//  コンストラクタ
//==========================================
CRenderer::CRenderer() : 
	m_pDevice(nullptr),
	m_pQueue(nullptr),
	m_pSwapChain(nullptr),
	m_pColorBuffer(),
	m_pCmdAllocator(),
	m_pCmdList(nullptr),
	m_pHeapRTV(nullptr),
	m_pFence(nullptr),
	m_FenceEvent(),
	m_FenceCounter(),
	m_FrameIndex(),
	m_HandleRTV()
{
}

//==========================================
//  デストラクタ
//==========================================
CRenderer::~CRenderer()
{
}

//==========================================
//  初期化処理
//==========================================
bool CRenderer::Init(HWND hWnd, int width, int height)
{
	// デバイスの生成
	auto hr = D3D12CreateDevice
	(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_pDevice)
	);

	// NULLチェック
	if (FAILED(hr)) { return false; }

	// コマンドキューの生成
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;
	hr = m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pQueue));

	// NULLチェック
	if (FAILED(hr)) { return false; }

	// DXGIファクトリーの生成
	IDXGIFactory4* pFactory = nullptr;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));

	// NULLチェック
	if (FAILED(hr)) { return false; }

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC chainDesc = {};
	chainDesc.BufferDesc.Width = width;
	chainDesc.BufferDesc.Height = height;
	chainDesc.BufferDesc.RefreshRate.Numerator = REFLESH_RATE;
	chainDesc.BufferDesc.RefreshRate.Denominator = 1;
	chainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	chainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	chainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	chainDesc.SampleDesc.Count = 1;
	chainDesc.SampleDesc.Quality = 0;
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.BufferCount = FrameCount;
	chainDesc.OutputWindow = hWnd;
	chainDesc.Windowed = TRUE;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェインの生成
	IDXGISwapChain* pSwapChain = nullptr;
	hr = pFactory->CreateSwapChain(m_pQueue, &chainDesc, &pSwapChain);

	// NULLチェック
	if (FAILED(hr))
	{
		Utility::SafeRelease(pFactory);
		return false;
	}



	return true;
}

//==========================================
//  終了処理
//==========================================
void CRenderer::Term()
{
}

//==========================================
//  描画処理
//==========================================
void CRenderer::Render()
{
}

//==========================================
//  GPU処理の待機
//==========================================
void CRenderer::WaitGPU()
{
}

void CRenderer::Present(uint32_t interval)
{
}
