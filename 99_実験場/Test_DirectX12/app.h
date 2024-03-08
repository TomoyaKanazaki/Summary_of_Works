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
//  �N���X��`
//==========================================
class App
{
public:

	//�����o�֐�
	App(uint32_t width, uint32_t height); // �R���X�g���N�^
	~App(); // �f�X�g���N�^

	void Run();

private:

	// �����o�֐�
	bool InitApp(); // 
	void TermApp(); // 
	bool InitWnd(); // 
	void TermWnd(); // 
	void MainLoop(); // 

	// �ÓI�����o�֐�
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

	// �����o�ϐ�
	HINSTANCE m_hInst; // �C���X�^���X�n���h��
	HWND m_hWnd; // �E�B���h�E�n���h��
	uint32_t m_Width; // �E�B���h�E�̉���
	uint32_t m_Height; // �E�B���h�E�̏c��

};