#include "PreCompile.h"
#include "EngineWindow.h"

#include "WindowImage.h"

bool UEngineWindow::WindowLive = true;
HINSTANCE UEngineWindow::hInstance;
std::function<bool(HWND, UINT, WPARAM, LPARAM)> UEngineWindow::UserWndProcFunction;

void UEngineWindow::Init(HINSTANCE _hInst)
{
	hInstance = _hInst;
}

unsigned __int64 UEngineWindow::WindowMessageLoop(std::function<void()> _Update, std::function<void()> _End)
{
	MSG msg = {};

	while (WindowLive)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (WindowLive == false)
			{
				break;
			}
		}

		if (_Update != nullptr)
		{
			_Update();
		}
	}

	if (_End != nullptr)
	{
		_End();
	}

	return msg.wParam;
}

void UEngineWindow::SetUserWindowCallBack(std::function<bool(HWND, UINT, WPARAM, LPARAM)> _UserWndProcFunction)
{
	UserWndProcFunction = _UserWndProcFunction;
}

LRESULT CALLBACK UEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (UserWndProcFunction != nullptr)
	{
		if (UserWndProcFunction(hWnd, message, wParam, lParam) == true)
		{
			return true;
		}
	}

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		WindowLive = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

UEngineWindow::UEngineWindow() 
{

}

UEngineWindow::~UEngineWindow() 
{
	BackBufferImage = nullptr;
	WindowImage = nullptr;
}

void UEngineWindow::SetWindowPosition(const FVector& _Pos)
{
	Position = _Pos;

	::SetWindowPos(hWnd, nullptr, Position.iX(), Position.iY(), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void UEngineWindow::SetWindowScale(const FVector& _Scale)
{
	Scale = _Scale;
	BackBufferImage = std::make_shared<UWindowImage>();
	BackBufferImage->Create(WindowImage, Scale);

	RECT Rc = { 0, 0, _Scale.iX(), _Scale.iY() };
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);

	::SetWindowPos(hWnd, nullptr, 0, 0, Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOZORDER | SWP_NOMOVE);
}

void UEngineWindow::SetWindowSmallIcon()
{

}

void UEngineWindow::Open(std::string_view _Title /*= "Title"*/, std::string_view _IconPath /*= ""*/)
{
	HICON hIcon = nullptr;
	if (_IconPath != "")
	{
		hIcon = (HICON)LoadImage(	// returns a HANDLE so we have to cast to HICON
			NULL,					// hInstance must be NULL when loading from a file
			_IconPath.data(),		// the icon file name
			IMAGE_ICON,				// specifies that the file is an icon
			0,						// width of the image (we'll specify default later on)
			0,						// height of the image
			LR_LOADFROMFILE |		// we want to load a file (as opposed to a resource)
			LR_DEFAULTSIZE |		// default metrics based on the type (IMAGE_ICON, 32x32)
			LR_SHARED				// let the system release the handle when it's no longer used
		);
	}

	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = hIcon;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "DefaultWindow";
	wcex.hIconSm = nullptr;

	RegisterClassExA(&wcex);

	int Style = WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_THICKFRAME |
		WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX;

	hWnd = CreateWindowA("DefaultWindow", _Title.data(), Style,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		MsgBoxAssert("������ ������ �����߽��ϴ�.");
		return;
	}

	HDC hDC = GetDC(hWnd);

	if (WindowImage == nullptr)
	{
		WindowImage = std::make_shared<UWindowImage>();
		WindowImage->Create(hDC);
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

void UEngineWindow::ScreenClear()
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(ClearColor.Color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(BackBufferImage->ImageDC, myBrush);
	Rectangle(BackBufferImage->ImageDC, -1, -1, Scale.iX() + 1, Scale.iY() + 1);
	SelectObject(BackBufferImage->ImageDC, oldBrush);
	DeleteObject(myBrush);
}

void UEngineWindow::ScreenUpdate()
{
	FTransform CopyTrans;
	CopyTrans.SetPosition({ Scale.ihX(), Scale.ihY() });
	CopyTrans.SetScale({ Scale.iX(), Scale.iY() });

	WindowImage->BitCopy(BackBufferImage, CopyTrans);
}

void UEngineWindow::CalculateMouseUpdate(float _DeltaTime)
{
	ScreenMousePos = GetMousePosition();
	ScreenMouseDir = ScreenMousePrevPos - ScreenMousePos;
	ScreenMouseDirNormal = ScreenMouseDir.Normalize2DReturn();
	ScreenMousePrevPos = ScreenMousePos;
}

void UEngineWindow::CursorOff()
{
	ShowCursor(FALSE);
}

FVector UEngineWindow::GetMousePosition()
{
	POINT MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(hWnd, &MousePoint);

	return FVector(MousePoint.x, MousePoint.y);
}