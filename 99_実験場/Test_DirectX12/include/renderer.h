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
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//==========================================
//  トランスフォーム構造体定義
//==========================================
struct alignas(256) Transform
{
	DirectX::XMMATRIX World; // ワールド行列
	DirectX::XMMATRIX View; // ビュー行列
	DirectX::XMMATRIX Proj; // 射影行列
};

//==========================================
//  コンスタントバッファビュー構造体定義
//==========================================
template<typename T>
struct ConstantBufferView
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC Desc; // 定数バッファの構成設定
	D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU; // CPUディスクリプタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU; // GPUディスクリプタハンドル
	T* pBuffer; // バッファ先頭へのポインタ
};

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
	void Uninit();
	void Render();
	void WaitGPU();

	// 静的メンバ関数
	static CRenderer* GetInstance();

private:

	// タイプ定義
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;

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
	ComPtr <ID3D12Device> m_pDevice; // デバイス
	ComPtr <ID3D12CommandQueue> m_pQueue; // コマンドキュー
	ComPtr <IDXGISwapChain3> m_pSwapChain; // スワップチェイン
	ComPtr <ID3D12Resource> m_pColorBuffer[FrameCount]; // カラーバッファ
	ComPtr <ID3D12CommandAllocator> m_pCmdAllocator[FrameCount]; // コマンドアロケータ
	ComPtr <ID3D12GraphicsCommandList> m_pCmdList; // コマンドリスト
	ComPtr <ID3D12DescriptorHeap> m_pHeapRTV; // ディスクリプタヒープ(レンダーターゲットビュー)
	ComPtr <ID3D12Fence> m_pFence; // フェンス

	ComPtr <ID3D12DescriptorHeap> m_pHeapCBV; // ディスクリプタヒープ
	ComPtr <ID3D12Resource> m_pVB; // 頂点バッファ
	ComPtr <ID3D12Resource> m_pCB[FrameCount]; // 定数バッファ
	ComPtr <ID3D12RootSignature> m_pRootSignature; // ルートシグニチャ
	ComPtr <ID3D12PipelineState> m_pPSO; // パイプラインステート

	HANDLE m_FenceEvent; // フェンスイベント
	uint64_t m_FenceCounter[FrameCount]; // フェンスカウンター
	uint32_t m_FrameIndex; // フレーム番号
	D3D12_CPU_DESCRIPTOR_HANDLE m_HandleRTV[FrameCount]; // CPUディスクリプタ(レンダラーターゲットビュー)

	D3D12_VERTEX_BUFFER_VIEW m_VBV; // 頂点バッファビュー
	D3D12_VIEWPORT m_ViewPort; // ビューポート
	D3D12_RECT m_Scissor; // シザー矩形
	ConstantBufferView<Transform> m_CBV[FrameCount]; // 定数バッファビュー
	float m_RotateAngle; // 回転角

};
