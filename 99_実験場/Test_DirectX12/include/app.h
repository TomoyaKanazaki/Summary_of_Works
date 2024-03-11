#pragma once // 2�d�C���N���[�h�h�~

//==========================================
//
//  �A�v���P�[�V�����N���X(app.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include <Windows.h>
#include <cstdint>

//==========================================
//  �O���錾
//==========================================
class CRenderer; // �����_���[�N���X

//==========================================
//  �N���X��`
//==========================================
class CApp
{
public:

	//�����o�֐�
	CApp(uint32_t width, uint32_t height); // �R���X�g���N�^
	~CApp(); // �f�X�g���N�^

	void Run();

	// �ÓI�����o�֐�
	static CApp* GetInstance();

private:

	// �����o�֐�
	bool InitApp(); // �A�v���P�[�V�����̏�����
	void UninitApp(); // �A�v���P�[�V�����̏I��
	bool InitWnd(); // �E�B���h�E�̏�����
	void UninitWnd(); // �E�B���h�E�̏I��
	void MainLoop(); // ���C�����[�v

	// �ÓI�����o�֐�
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp); // �E�B���h�E�v���V�[�W��

	// �����o�ϐ�
	HINSTANCE m_hInst; // �C���X�^���X�n���h��
	HWND m_hWnd; // �E�B���h�E�n���h��
	uint32_t m_Width; // �E�B���h�E�̉���
	uint32_t m_Height; // �E�B���h�E�̏c��
	CRenderer* m_pRenderer; // �����_���[

	// �ÓI�����o�ϐ�
	static CApp* m_pApp;

};