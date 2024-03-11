//==========================================
//
//  �A�v���P�[�V�����N���X(app.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "app.h"
#include "renderer.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int SCREEN_WIDTH = 1280; // �E�B���h�E�̉���
	const int SCREEN_HEIGHT = 720; // �E�B���h�E�̍���
	const auto ClassName = TEXT("WindowClass"); // �E�B���h�E�N���X�l�[��
	const auto WindowName = TEXT("DirectX12Test"); // �E�B���h�E�l�[��
}

//==========================================
//  �ÓI�����o�ϐ��錾
//==========================================
CApp* CApp::m_pApp = nullptr;

//==========================================
//  �R���X�g���N�^
//==========================================
CApp::CApp(uint32_t width, uint32_t height) :
	m_hInst(nullptr),
	m_hWnd(nullptr),
	m_Width(width),
	m_Height(height),
	m_pRenderer(nullptr)
{
	// Do Nothing
}

//==========================================
//  �f�X�g���N�^
//==========================================
CApp::~CApp()
{
	// Do Nothing
}

//==========================================
//  CApp�N���X�̃C���X�^���X�擾
//==========================================
CApp* CApp::GetInstance()
{
	//�C���X�^���X����
	if (m_pApp == nullptr)
	{
		m_pApp = new CApp(SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	return m_pApp;
}

//==========================================
//  ����������
//==========================================
bool CApp::InitApp()
{
	// �E�B���h�E�̏�����
	if (!InitWnd()) { return false; } // �������Ɏ��s

	// �����_���[�̎擾
	m_pRenderer = CRenderer::GetInstance();

	// �����_���[�̏�����
	if (!m_pRenderer->Init(m_hWnd, m_Width, m_Height)) { return false; } // �������Ɏ��s

	// ����I��
	return true;
}

//==========================================
//  �E�B���h�E�̏�����
//==========================================
bool CApp::InitWnd()
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
LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
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
//  �I������
//==========================================
void CApp::TermApp()
{
	// �����_���[�̏I������
	m_pRenderer->Term();

	// �E�B���h�E�̏I������
	TermWnd();
}

//==========================================
//  �E�B���h�E�̏I������
//==========================================
void CApp::TermWnd()
{
	// �E�B���h�E�̓o�^������
	if (m_hInst != nullptr)
	{
		UnregisterClass(ClassName, m_hInst);
	}

	m_hInst = nullptr;
	m_hWnd = nullptr;
}

//==========================================
//  ���s
//==========================================
void CApp::Run()
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
//  ���C�����[�v
//==========================================
void CApp::MainLoop()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// �`��
			m_pRenderer->Render();
		}
	}
}
