#pragma once // 2�d�C���N���[�h�h�~

//==========================================
//
//  �����_�����O�N���X(renderer.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "dxgi.lib")

//==========================================
//  �N���X��`
//==========================================
class CRenderer
{
public:

	// �����o�֐�
	CRenderer();
	~CRenderer();

	bool Init(HWND hWnd, int width, int height);
	void Term();
	void Render();
	void WaitGPU();

	// �ÓI�����o�֐�
	static CRenderer* GetInstance();

private:

	// �^�C�v��`
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �����o�֐�
	void Present(uint32_t interval);
	bool CreateQueue(); // �R�}���h�L���[�̐���
	bool CreateSwapChain(HWND hWnd, int width, int height); // �X���b�v�`�F�C���̐���
	bool CreateAllocator(); // �R�}���h�A���P�[�^�̐���
	bool CreateList(); // �R�}���h���X�g�̐���
	bool CreateTargetView(); // �����_�[�^�[�Q�b�g�r���[�̐���
	bool CreateFence(); // �t�F���X�̐���

	// �ÓI�����o�ϐ�
	static const uint32_t FrameCount = 2; // �t���[���o�b�t�@��
	static CRenderer* m_pRenderer;

	// �����o�ϐ�
	ComPtr <ID3D12Device> m_pDevice; // �f�o�C�X
	ComPtr <ID3D12CommandQueue> m_pQueue; // �R�}���h�L���[
	ComPtr <IDXGISwapChain3> m_pSwapChain; // �X���b�v�`�F�C��
	ComPtr <ID3D12Resource> m_pColorBuffer[FrameCount]; // �J���[�o�b�t�@
	ComPtr <ID3D12CommandAllocator> m_pCmdAllocator[FrameCount]; // �R�}���h�A���P�[�^
	ComPtr <ID3D12GraphicsCommandList> m_pCmdList; // �R�}���h���X�g
	ComPtr <ID3D12DescriptorHeap> m_pHeapRTV; // �f�B�X�N���v�^�q�[�v(�����_�[�^�[�Q�b�g�r���[)
	ComPtr <ID3D12Fence> m_pFence; // �t�F���X
	HANDLE m_FenceEvent; // �t�F���X�C�x���g
	uint64_t m_FenceCounter[FrameCount]; // �t�F���X�J�E���^�[
	uint32_t m_FrameIndex; // �t���[���ԍ�
	D3D12_CPU_DESCRIPTOR_HANDLE m_HandleRTV[FrameCount]; // CPU�f�B�X�N���v�^(�����_���[�^�[�Q�b�g�r���[)

};
