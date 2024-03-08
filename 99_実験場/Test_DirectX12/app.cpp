//==========================================
//
//  �A�v���P�[�V�����N���X(app.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "app.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const auto ClassName = TEXT("WindowClass"); // �E�B���h�E�N���X�l�[��
	const auto WindowName = TEXT("DirectX12Test"); // �E�B���h�E�l�[��
}

//==========================================
//  �R���X�g���N�^
//==========================================
App::App(uint32_t width, uint32_t height) :
	m_hInst(nullptr),
	m_hWnd(nullptr),
	m_Width(width),
	m_Height(height)
{
	// Do Nothing
}

//==========================================
//  �f�X�g���N�^
//==========================================
App::~App()
{
	// Do Nothing
}

//==========================================
//  ���s
//==========================================
void App::Run()
{
	// ����������
	if (InitApp())
	{
		// ���C�����[�v
		MainLoop();
	}

	// �I������
	TermApp();
}

//==========================================
//  ����������
//==========================================
bool App::InitApp()
{
	// �E�B���h�E�̏�����
	if (!InitWnd()) { return false; } // �������Ɏ��s

	// ����I��
	return true;
}

//==========================================
//  �E�B���h�E�̏�����
//==========================================
bool App::InitWnd()
{
	// �C���X�^���X�n���h���̎擾
	auto hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr) { return false; } // �������Ɏ��s

	// �E�B���h�E�̐ݒ�
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

	// �E�B���h�E�̓o�^
	if (!RegisterClassEx(&wc)) { return false; } // �������Ɏ��s

	// �C���X�^���X�n���h���ݒ�
	m_hInst = hInst;

	// �E�B���h�E�̃T�C�Y��ݒ�
	RECT rc = {};
	rc.right = static_cast<LONG>(m_Width);
	rc.bottom = static_cast<LONG>(m_Height);

	// �E�B���h�E�T�C�Y�𒲐�
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, FALSE);

	// �E�B���h�E�𐶐�
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

	// NULL�`�F�b�N
	if (m_hWnd == nullptr) { return false; }

	// �E�B���h�E��\��
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	// �E�B���h�E���X�V
	UpdateWindow(m_hWnd);

	// �E�B���h�E�Ƀt�H�[�J�X��ݒ�
	SetFocus(m_hWnd);

	// ����I��
	return true;
}

//==========================================
//  �E�B���h�E�v���V�[�W��
//==========================================
LRESULT CALLBACK App::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
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
//  ���C�����[�v
//==========================================
void App::MainLoop()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != TRUE) { continue; }

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//==========================================
//  �I������
//==========================================
void App::TermApp()
{
	// �E�B���h�E�̏I������
	TermWnd();
}

//==========================================
//  �E�B���h�E�̏I������
//==========================================
void App::TermWnd()
{
	// �E�B���h�E�̓o�^������
	if (m_hInst != nullptr)
	{
		UnregisterClass(ClassName, m_hInst);
	}

	m_hInst = nullptr;
	m_hWnd = nullptr;
}
