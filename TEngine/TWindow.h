#pragma once
#include "TCommon.h"
#include <Windows.h>

namespace T3D
{
	class TInputListener;
	class TCanvas;
	//游戏窗口管理类
	class TGameWindow
	{
	public:
		//全局指针，用于在WinProc中毁掉
		static TGameWindow* GWindow;

		//Window时间处理
		void StartLoop();

		TGameWindow(const TString& windowTile, HINSTANCE hInstance);
		~TGameWindow();

		//显示窗口
		void ShowWindow(TBool show);
		void UpdateWindow();

		void QuitApplication();

		HWND getHWnd()const { return mHwnd; }
		HINSTANCE getHInstance()const { return mHinstance; }

		//注册/移除I/O事件监听
		void AddInputListener(TInputListener* listener);
		void RemoveInputListener(TInputListener* listener);

		//设置绘制监听器，同事只能有一个绘制Canvas
		void setCanvasListener(TCanvas* canvasListener);

	protected:
		//全局Win时间回调函数
		static LRESULT WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//窗口绘制函数
		virtual void OnPaint(HDC hdc);
		virtual void OnUpdate();
		virtual void OnKeyDown(TInt msg);
		virtual void OnKeyRelease(TInt msg);
		virtual void OnMousePress(bool rightPress);
		virtual void OnMouseMove(TInt x, TInt y);
		virtual void OnMouseWheel(TInt delta);

	protected:
		//注册窗口
		bool RgsWindow();
		//创建窗口
		bool CrtWindow();

	protected:
		HWND mHwnd;
		TString mWindowTitle;
		HINSTANCE mHinstance;
		std::vector<TInputListener*> mInputListener;
		TCanvas* mCanvasListener;
		
		TBool mQuite;
		typedef std::vector<TInputListener*>::iterator InputItr;
	};
}