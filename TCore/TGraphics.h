#pragma once

#include "TCommon.h"
#include <Windows.h>

//图形渲染底层分实现

namespace T3D 
{
	extern HINSTANCE GHInstance;

	//图片封装，由于图片使用的GDI+的图片类
	//所以这里对它进行封装，将平台相关的东西隐藏

	class TBitmap
	{
	public:
		TInt width, height;

		TBitmap(const TString& filename);
		~TBitmap();

		inline TString getName()const { return name; }
		inline bool isValid()const { return valid; }
		TColor getPixel(TInt x, TInt y);
		inline TInt getHeight()const { return height; }
		inline TInt getWidth()const { return width; }

	private:
		TString		name;
		HBITMAP		hBitmap;
		BITMAP		bitmap;
		HDC			bitmapHDC;

	public:
		TColor		*pixels;
		TInt		Pitch;
		TBool		valid;
	};

	class TGraphics
	{
	public:
		//初始化绘图系统
		static TBool initGraphics(HINSTANCE hinstance);
		//关闭绘图系统
		static void shutdownGraphics();

		//检测Z值，返回true则表示通过，可以调用setPixel
		static TBool checkZ(TInt x, TInt y, TFloat z);
		static void setPixel(TInt x, TInt y,/*TFloat z,*/const TColor &c);
		static TColor getPixel(TInt x, TInt y);
		//在当前缓冲区内绘制一条线
		static void drawLine(TInt x0, TInt y0, TInt x1, TInt y1, const TColor& c);
		//绘制字符串
		static void drawString(const TString& str, TInt x, TInt y, const TColor& c = TColor(255, 255, 255));
		//填充三角形
		static void fillTriangle(TInt x0, TInt y0, TInt x1, TInt y1, TInt x2, TInt y2, const TColor& c = TColor(255, 255, 255));
		//平滑模式
		static void enableSmoothingMode(TBool enable);

		//情况当前缓冲区，并将其颜色设置为黑色
		static void clearBuffer(const TColor& c = TColor());
		//将已经绘制好的缓冲区递交给Graphics在屏幕上绘制，并将当前缓冲区设置为另一个缓冲区
		static void fillBuffer(HDC hdc);
		//获取屏幕的宽和高
		static TInt getScreenWidth() { return SCREEN_WIDTH; }
		static TInt getScreenHeight() { return SCREEN_HEIGHT; }

	private:

		//保存变量
		static HBITMAP		GBufferedHandle;
		static HDC			GBufferedHDC;
		static HBRUSH		GBgBrush;
		static HPEN			GPen;
		static HINSTANCE	GInstatnce;

		static BYTE			*GDatas;
		static TInt			GPitch;

		static DIBSECTION	GDIBSection;

		static RECT			GBufferSize;
		
		static TFloat		*GZBuffer;
	};
}