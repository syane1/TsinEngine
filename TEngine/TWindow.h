#pragma once
#include "TCommon.h"
#include <Windows.h>

namespace T3D
{
	class TInputListener;
	class TCanvas;
	//��Ϸ���ڹ�����
	class TGameWindow
	{
	public:
		//ȫ��ָ�룬������WinProc�лٵ�
		static TGameWindow* GWindow;

		//Windowʱ�䴦��
		void StartLoop();

		TGameWindow(const TString& windowTile, HINSTANCE hInstance);
		~TGameWindow();

		//��ʾ����
		void ShowWindow(TBool show);
		void UpdateWindow();

		void QuitApplication();

		HWND getHWnd()const { return mHwnd; }
		HINSTANCE getHInstance()const { return mHinstance; }

		//ע��/�Ƴ�I/O�¼�����
		void AddInputListener(TInputListener* listener);
		void RemoveInputListener(TInputListener* listener);

		//���û��Ƽ�������ͬ��ֻ����һ������Canvas
		void setCanvasListener(TCanvas* canvasListener);

	protected:
		//ȫ��Winʱ��ص�����
		static LRESULT WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//���ڻ��ƺ���
		virtual void OnPaint(HDC hdc);
		virtual void OnUpdate();
		virtual void OnKeyDown(TInt msg);
		virtual void OnKeyRelease(TInt msg);
		virtual void OnMousePress(bool rightPress);
		virtual void OnMouseMove(TInt x, TInt y);
		virtual void OnMouseWheel(TInt delta);

	protected:
		//ע�ᴰ��
		bool RgsWindow();
		//��������
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