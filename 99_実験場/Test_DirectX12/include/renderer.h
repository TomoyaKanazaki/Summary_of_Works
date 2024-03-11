#pragma once // 2重インクルード防止

//==========================================
//
//  レンダリングクラス(renderer.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "dxgi.lib")

//==========================================
//  クラス定義
//==========================================
class CRenderer
{
public:

	// メンバ関数
	CRenderer();
	~CRenderer();

	bool Init(HWND hWnd, int width, int height);
	void Term();
	void Render();
	void WaitGPU();

	// 静的メンバ関数
	static CRenderer* GetInstance();

private:

	// メンバ関数
	void Present(uint32_t interval);
	bool CreateQueue(); // コマンドキューの生成
	bool CreateSwapChain(HWND hWnd, int width, int height); // スワップチェインの生成
	bool CreateAllocator(); // コマンドアロケータの生成
	bool CreateList(); // コマンドリストの生成
	bool CreateTargetView(); // レンダーターゲットビューの生成
	bool CreateFence(); // フェンスの生成

	// 静的メンバ変数
	static const uint32_t FrameCount = 2; // フレームバッファ数
	static CRenderer* m_pRenderer;

	// メンバ変数
	ID3D12Device* m_pDevice; // デバイス
	ID3D12CommandQueue* m_pQueue; // コマンドキュー
	IDXGISwapChain3* m_pSwapChain; // スワップチェイン
	ID3D12Resource* m_pColorBuffer[FrameCount]; // カラーバッファ
	ID3D12CommandAllocator* m_pCmdAllocator[FrameCount]; // コマンドアロケータ
	ID3D12GraphicsCommandList* m_pCmdList; // コマンドリスト
	ID3D12DescriptorHeap* m_pHeapRTV; // ディスクリプタヒープ(レンダーターゲットビュー)
	ID3D12Fence* m_pFence; // フェンス
	HANDLE m_FenceEvent; // フェンスイベント
	uint64_t m_FenceCounter[FrameCount]; // フェンスカウンター
	uint32_t m_FrameIndex; // フレーム番号
	D3D12_CPU_DESCRIPTOR_HANDLE m_HandleRTV[FrameCount]; // CPUディスクリプタ(レンダラーターゲットビュー)

};
