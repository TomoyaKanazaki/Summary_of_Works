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
		Utility::SafeRelease(&pFactory);
		return false;
	}

	// IDGISwapChain3を取得
	hr = pSwapChain->QueryInterface(IID_PPV_ARGS(&m_pSwapChain));

	// NULLチェック
	if (FAILED(hr))
	{
		Utility::SafeRelease(&pFactory);
		Utility::SafeRelease(&pSwapChain);
		return false;
	}

	// バックバッファ番号を取得
	m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	// 不要になったメモリを開放する
	Utility::SafeRelease(&pFactory);
	Utility::SafeRelease(&pSwapChain);

	// コマンドアロケータの生成
	for (auto i = 0u; i < FrameCount; ++i)
	{
		hr = m_pDevice->CreateCommandAllocator
		(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_pCmdAllocator[i])
		);

		// NULLチェック
		if (FAILED(hr)) { return false; }
	}

	// コマンドリストの生成
	hr = m_pDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCmdAllocator[m_FrameIndex],
		nullptr,
		IID_PPV_ARGS(&m_pCmdList)
	);

	// NULLチェック
	if (FAILED(hr)) { return false; }

	// ディスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = FrameCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;

	// ディスクリプタヒープの生成
	hr = m_pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pHeapRTV));

	// NULLチェック
	if (FAILED(hr)) { return false; }

	auto handle = m_pHeapRTV->GetCPUDescriptorHandleForHeapStart();
	auto incrementtSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (auto i = 0u; i < FrameCount; ++i)
	{
		hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pColorBuffer[i]));

		// NULLチェック
		if (FAILED(hr)) { return false; }

		D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
		viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice = 0;
		viewDesc.Texture2D.PlaneSlice = 0;

		// レンダーターゲットビューの生成
		m_pDevice->CreateRenderTargetView(m_pColorBuffer[i], &viewDesc, handle);
		m_HandleRTV[i] = handle;
		handle.ptr += incrementtSize;
	}

	// フェンスカウンターをリセット
	for (auto i = 0u; i < FrameCount; ++i)
	{
		m_FenceCounter[i] = 0;
	}

	// フェンスの生成
	hr = m_pDevice->CreateFence
	(
		m_FenceCounter[m_FrameIndex],
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&m_pFence)
	);

	// NULLチェック
	if (FAILED(hr)) { return false; }

	m_FenceCounter[m_FrameIndex]++;

	// イベントの生成
	m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	// NULLチェック
	if (m_FenceEvent == nullptr) { return false; }

	// コマンドリストを閉じる
	m_pCmdList->Close();

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
