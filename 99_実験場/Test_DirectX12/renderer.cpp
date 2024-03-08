//==========================================
//
//  �����_�����O�N���X(renderer.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "renderer.h"
#include "utility.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int REFLESH_RATE = 144; // ���t���b�V�����[�g
}

//==========================================
//  �R���X�g���N�^
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
//  �f�X�g���N�^
//==========================================
CRenderer::~CRenderer()
{
}

//==========================================
//  ����������
//==========================================
bool CRenderer::Init(HWND hWnd, int width, int height)
{
	// �f�o�C�X�̐���
	auto hr = D3D12CreateDevice
	(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_pDevice)
	);

	// NULL�`�F�b�N
	if (FAILED(hr)) { return false; }

	// �R�}���h�L���[�̐���
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;
	hr = m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pQueue));

	// NULL�`�F�b�N
	if (FAILED(hr)) { return false; }

	// DXGI�t�@�N�g���[�̐���
	IDXGIFactory4* pFactory = nullptr;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));

	// NULL�`�F�b�N
	if (FAILED(hr)) { return false; }

	// �X���b�v�`�F�C���̐ݒ�
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

	// �X���b�v�`�F�C���̐���
	IDXGISwapChain* pSwapChain = nullptr;
	hr = pFactory->CreateSwapChain(m_pQueue, &chainDesc, &pSwapChain);

	// NULL�`�F�b�N
	if (FAILED(hr))
	{
		Utility::SafeRelease(&pFactory);
		return false;
	}

	// IDGISwapChain3���擾
	hr = pSwapChain->QueryInterface(IID_PPV_ARGS(&m_pSwapChain));

	// NULL�`�F�b�N
	if (FAILED(hr))
	{
		Utility::SafeRelease(&pFactory);
		Utility::SafeRelease(&pSwapChain);
		return false;
	}

	// �o�b�N�o�b�t�@�ԍ����擾
	m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	// �s�v�ɂȂ������������J������
	Utility::SafeRelease(&pFactory);
	Utility::SafeRelease(&pSwapChain);

	// �R�}���h�A���P�[�^�̐���
	for (auto i = 0u; i < FrameCount; ++i)
	{
		hr = m_pDevice->CreateCommandAllocator
		(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_pCmdAllocator[i])
		);

		// NULL�`�F�b�N
		if (FAILED(hr)) { return false; }
	}

	// �R�}���h���X�g�̐���
	hr = m_pDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCmdAllocator[m_FrameIndex],
		nullptr,
		IID_PPV_ARGS(&m_pCmdList)
	);

	// NULL�`�F�b�N
	if (FAILED(hr)) { return false; }

	// �f�B�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = FrameCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;

	// �f�B�X�N���v�^�q�[�v�̐���
	hr = m_pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pHeapRTV));

	// NULL�`�F�b�N
	if (FAILED(hr)) { return false; }

	auto handle = m_pHeapRTV->GetCPUDescriptorHandleForHeapStart();
	auto incrementtSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (auto i = 0u; i < FrameCount; ++i)
	{
		hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pColorBuffer[i]));

		// NULL�`�F�b�N
		if (FAILED(hr)) { return false; }

		D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
		viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice = 0;
		viewDesc.Texture2D.PlaneSlice = 0;

		// �����_�[�^�[�Q�b�g�r���[�̐���
		m_pDevice->CreateRenderTargetView(m_pColorBuffer[i], &viewDesc, handle);
		m_HandleRTV[i] = handle;
		handle.ptr += incrementtSize;
	}

	// �t�F���X�J�E���^�[�����Z�b�g
	for (auto i = 0u; i < FrameCount; ++i)
	{
		m_FenceCounter[i] = 0;
	}

	// �t�F���X�̐���
	hr = m_pDevice->CreateFence
	(
		m_FenceCounter[m_FrameIndex],
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&m_pFence)
	);

	// NULL�`�F�b�N
	if (FAILED(hr)) { return false; }

	m_FenceCounter[m_FrameIndex]++;

	// �C�x���g�̐���
	m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	// NULL�`�F�b�N
	if (m_FenceEvent == nullptr) { return false; }

	// �R�}���h���X�g�����
	m_pCmdList->Close();

	return true;
}

//==========================================
//  �I������
//==========================================
void CRenderer::Term()
{
}

//==========================================
//  �`�揈��
//==========================================
void CRenderer::Render()
{
}

//==========================================
//  GPU�����̑ҋ@
//==========================================
void CRenderer::WaitGPU()
{
}

void CRenderer::Present(uint32_t interval)
{
}
