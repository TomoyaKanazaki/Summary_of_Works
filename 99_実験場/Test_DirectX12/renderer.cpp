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
	const int REFLESH_RATE = 60; // ���t���b�V�����[�g
}

//==========================================
//  �ÓI�����o�ϐ��錾
//==========================================
CRenderer* CRenderer::m_pRenderer = nullptr;

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
		Utility::SafeRelease(pFactory);
		return false;
	}

	// IDGISwapChain3���擾
	hr = pSwapChain->QueryInterface(IID_PPV_ARGS(&m_pSwapChain));

	// NULL�`�F�b�N
	if (FAILED(hr))
	{
		Utility::SafeRelease(pFactory);
		Utility::SafeRelease(pSwapChain);
		return false;
	}

	// �o�b�N�o�b�t�@�ԍ����擾
	m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	// �s�v�ɂȂ������������J������
	Utility::SafeRelease(pFactory);
	Utility::SafeRelease(pSwapChain);

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
	// GPU�����̊�����ҋ@
	WaitGPU();

	// �C�x���g�̔j��
	if (m_FenceEvent != nullptr)
	{
		CloseHandle(m_FenceEvent);
		m_FenceEvent = nullptr;
	}

	// �t�F���X�̔j��
	Utility::SafeRelease(m_pFence);

	// �����_�[�^�[�Q�b�g�r���[�̔j��
	Utility::SafeRelease(m_pHeapRTV);
	for (auto i = 0u; i < FrameCount; ++i)
	{
		Utility::SafeRelease(m_pColorBuffer[i]);
	}

	// �R�}���h���X�g�̔j��
	Utility::SafeRelease(m_pCmdList);

	// �R�}���h�A���P�[�^�̔j��
	for (auto i = 0u; i < FrameCount; ++i)
	{
		Utility::SafeRelease(m_pCmdAllocator[i]);
	}

	// �X���b�v�`�F�C���̔j��
	Utility::SafeRelease(m_pSwapChain);

	// �R�}���h�L���[�̔j��
	Utility::SafeRelease(m_pQueue);

	// �f�o�C�X�̔j��
	Utility::SafeRelease(m_pDevice);
}

//==========================================
//  �`�揈��
//==========================================
void CRenderer::Render()
{
	// �R�}���h�̋L�^���J�n
	m_pCmdAllocator[m_FrameIndex]->Reset();
	m_pCmdList->Reset(m_pCmdAllocator[m_FrameIndex], nullptr);

	// ���\�[�X�o���A�̐ݒ�
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_pColorBuffer[m_FrameIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	// ���\�[�X�o���A
	m_pCmdList->ResourceBarrier(1, &barrier);

	// �����_�[�Q�b�g�̐ݒ�
	m_pCmdList->OMSetRenderTargets(1, &m_HandleRTV[m_FrameIndex], FALSE, nullptr);

	// �N���A�J���[�̐ݒ�
	float clearColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };

	// �����_�[�^�[�Q�b�g�r���[���N���A
	m_pCmdList->ClearRenderTargetView(m_HandleRTV[m_FrameIndex], clearColor, 0, nullptr);

	// �`�揈��
	// To Do : �|���S���`�揈�����ォ��ǉ�����

	// ���\�[�X�o���A�̐ݒ�
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_pColorBuffer[m_FrameIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	// ���\�[�X�o���A
	m_pCmdList->ResourceBarrier(1, &barrier);

	// �R�}���h�̋L�^���I��
	m_pCmdList->Close();

	// �R�}���h������
	ID3D12CommandList* ppCmdLists[] = { m_pCmdList };
	m_pQueue->ExecuteCommandLists(1, ppCmdLists);

	// ��ʂɕ\��
	Present(1);
}

//==========================================
//  GPU�����̑ҋ@
//==========================================
void CRenderer::WaitGPU()
{
	Utility::MyAssert(m_pQueue == nullptr);
	Utility::MyAssert(m_pFence == nullptr);
	Utility::MyAssert(m_FenceEvent == nullptr);

	// �V�O�i������
	m_pQueue->Signal(m_pFence, m_FenceCounter[m_FrameIndex]);

	// �������ɃC�x���g��ݒ肷��
	m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex], m_FenceEvent);

	// �ҋ@����
	WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);

	// �J�E���^�[�𑝂₷
	++m_FenceCounter[m_FrameIndex];
}

//==========================================
//  ���t���[���̏���
//==========================================
void CRenderer::Present(uint32_t interval)
{
	// ��ʂɕ\��
	m_pSwapChain->Present(interval, 0);

	// �V�O�i������
	const auto currentValue = m_FenceCounter[m_FrameIndex];
	m_pQueue->Signal(m_pFence, currentValue);

	// �o�b�N�o�b�t�@�ԍ����X�V
	m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	// ���̃t���[���̕`�揀�����܂��ł���Αҋ@����
	if (m_pFence->GetCompletedValue() < m_FenceCounter[m_FrameIndex])
	{
		m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex], m_FenceEvent);
		WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);
	}

	// ���̃t���[���̃t�F���X�J�E���^�[�𑝂₷
	m_FenceCounter[m_FrameIndex] = currentValue + 1;
}

//==========================================
//  CRenderer�N���X�̃C���X�^���X�擾
//==========================================
CRenderer* CRenderer::GetInstance()
{
	//�C���X�^���X����
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;
	}

	return m_pRenderer;
}
