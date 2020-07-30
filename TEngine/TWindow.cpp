#include "TWindow.h"
#include "Windowsx.h"
#include "TGraphics.h"
#include "TInputListener.h"
//#include "TCanvas.h"
#include <ctime>

namespace T3D
{
	TGameWindow* TGameWindow::GWindow;

	LRESULT TGameWindow::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (!TGameWindow::GWindow)
			return DefWindowProc(hWnd, msg, wParam, lParam);

		switch (msg)
		{
			//绘制
		case WM_MOVING:
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			TGameWindow::GWindow->OnPaint(hdc);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_TIMER:
		{
			TGameWindow::GWindow->UpdateWindow();
			break;
		}
		//键盘按下
		case WM_KEYDOWN:
		{
			TGameWindow::GWindow->OnKeyDown(wParam);
			break;
		}
		//键盘释放
		case WM_KEYUP:
		{
			TGameWindow::GWindow->OnKeyRelease(wParam);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			TShort delta = GET_WHEEL_DELTA_WPARAM(wParam);//wheel rotation
			TGameWindow::GWindow->OnMouseWheel(delta*0.5f / PI);
		}
		//鼠标移动
		case WM_MOUSEMOVE:
		{
			TInt x = GET_X_LPARAM(lParam);
			TInt y = GET_Y_LPARAM(lParam);
			TGameWindow::GWindow->OnMouseMove(x, y);
			break;
		}
		//关闭窗口
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	TGameWindow::TGameWindow(const TString& windowTitle, HINSTANCE hInstance):mWindowTitle(windowTitle),mHinstance(hInstance),mCanvasListener(NULL),mQuite(false)
	{
		if (!(RgsWindow() && CrtWindow()))
		{
			throw "Init Window Failed";
		}
	}

	TGameWindow::~TGameWindow()
	{
		DestroyWindow(mHwnd);
		for (InputItr itr = mInputListener.begin(); itr != mInputListener.end(); ++itr)
		{
			SafeDelete(*itr);
		}

		SafeDelete(mCanvasListener);
	}

	//显示窗口
	void TGameWindow::ShowWindow(TBool show)
	{
		::ShowWindow(mHwnd, SW_SHOW);
		::UpdateWindow(mHwnd);
	}

	void TGameWindow::UpdateWindow()
	{
		//刷新窗口
		::InvalidateRect(mHwnd, NULL, FALSE);
		::UpdateWindow(mHwnd);
	}

	void TGameWindow::setCanvasListener(TCanvas* canvasListener)
	{
		mCanvasListener = canvasListener;
	}

	void TGameWindow::OnUpdate()
	{
		
	}

	//窗口绘制函数
	void TGameWindow::OnPaint(HDC hdc)
	{
		if (mCanvasListener)
		{
			TGraphics::clearBuffer(TColor(0, 0, 0));
			mCanvasListener->update();
			mCanvasListener->onPaint();
			TGraphics::fillBuffer(hdc);
		}
	}

	void TGameWindow::AddInputListener(TInputListener* listener)
	{
		mInputListener.push_back(listener);
	}

	void TGameWindow::RemoveInputListener(TInputListener* listener)
	{
		for (InputItr itr = mInputListener.begin(); itr != mInputListener.end(); ++itr)
		{
			if (*itr == listener)
			{
				mInputListener.erase(itr++);
			}
		}
	}

	void TGameWindow::OnKeyDown(TInt msg)
	{
		//::MessageBox(0,"Window onKeyDown",0,0);
		for (InputItr itr = mInputListener.begin(); itr != mInputListener.end(); ++itr)
			(*itr)->keyPress(msg);
	}

	void TGameWindow::OnKeyRelease(TInt msg)
	{
		for (InputItr itr = mInputListener.begin(); itr != mInputListener.end(); ++itr)
			(*itr)->keyRelease(msg);
	}

	void TGameWindow::OnMousePress(bool rightPress)
	{
		if (rightPress)
			::MessageBox(0, "Window R onMousePress", 0, 0);
		else
			::MessageBox(0, "Window L onMousePress", 0, 0);
	}

	void TGameWindow::OnMouseMove(TInt x, TInt y)
	{
		for (InputItr itr = mInputListener.begin(); itr != mInputListener.end(); ++itr)
			(*itr)->mouseMove(x, y);
	}

	void TGameWindow::OnMouseWheel(TInt delta)
	{
		for (InputItr itr = mInputListener.begin(); itr != mInputListener.end(); ++itr)
			(*itr)->mouseWheel(delta);
	}

	void TGameWindow::QuitApplication()
	{
		mQuite = true;
	}

	void TGameWindow::StartLoop()
	{
		MSG msg;
		while (!mQuite)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
				OnUpdate();
			}
			else
				::Sleep(10);
		}
	}

	bool TGameWindow::CrtWindow()
	{
		mHwnd = ::CreateWindow("T3D", mWindowTitle.c_str(),
			//窗口大小不可改变
			WS_OVERLAPPEDWINDOW&(~WS_MAXIMIZEBOX)&(~WS_THICKFRAME),
			CW_USEDEFAULT, CW_USEDEFAULT,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			0, 0, mHinstance, 0
		);

		if (!mHwnd)
		{
			::MessageBox(0, "Create Window Failed", 0, 0);
			return false;
		}

		::SetTimer(mHwnd, WM_TIMER, 35, NULL);

		//手动播种
		srand(time(NULL));

		return true;
	}

	bool TGameWindow::RgsWindow()
	{
		WNDCLASS wind;
		{
			wind.style = CS_HREDRAW | CS_VREDRAW;
			wind.lpfnWndProc = (WNDPROC)TGameWindow::WinProc;
			wind.cbClsExtra = 0;
			wind.cbWndExtra = 0;
			wind.hInstance = mHinstance;
			wind.hIcon = LoadIcon(0, IDI_APPLICATION);
			wind.hCursor = LoadCursor(0, IDC_ARROW);
			wind.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wind.lpszMenuName = 0;
			wind.lpszClassName = "T3D";
		}

		if (!RegisterClass(&wind))
		{
			::MessageBox(0, "Register Class Failed", 0, 0);
			return false;
		}

		return true;
	}

}