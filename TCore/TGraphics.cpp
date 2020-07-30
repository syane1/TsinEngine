#include "TGraphics.h"
#include "TUtil.h"

namespace T3D
{
	TBitmap::TBitmap(const TString& filename) :name(filename), pixels(NULL), pitch(0), width(0), height(0), valid(false)
	{
		//材质脚本，首先获取默认路径
		hBitmap = (HBITMAP)::LoadImage(GHInstance, GetPath(filename).c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hBitmap != NULL) 
		{
			bitmapHDC = ::CreateCompatibleDC(NULL);
			::SelectObject(bitmapHDC, (HGDIOBJ)hBitmap);
			::GetObject(hBitmap, sizeof(BITMAP), &bitmap);

			width = bitmap.bmWidth;
			height = bitmap.bmHeight;
			pitch = bitmap.bmHeight;
			valid = true;

			pixels = new TColor[width*height];
			for (TInt i = 0; i < getWidth(); i++)
			{
				for (TInt j = 0; j < getWidth(); j++)
				{  
					COLORREF color = ::GetPixel(bitmapHDC, i, j);
					pixels[j*width + i] = TColor(GetRValue(color), GetGValue(color), GetBValue(color));
				}
			}
		}
	}

	//析构函数
	TBitmap::~TBitmap()
	{
		DeleteObject(hBitmap);
		DeleteDC(bitmapHDC);
		SafeDeleteArray(pixels);
	}
	
	//获取像素
	TColor TBitmap::getPixel(TInt x, TInt y)
	{
		return pixels[y*pitch + x];
	}

	HINSTANCE GHInstance;

	HBITMAP			TGraphics::GBufferedHandle;
	HDC				TGraphics::GBufferedHDC;
	HBRUSH			TGraphics::GBgBrush;
	HPEN			TGraphics::GPen;
	HINSTANCE		TGraphics::GInstatnce;
	RECT			TGraphics::GBufferSize;
	DIBSECTION		TGraphics::GDIBSection;
	BYTE			*TGraphics::GDatas;
	TInt			TGraphics::GPitch;

	TFloat			*TGraphics::GZBuffer;

	//初始化绘图系统
	bool TGraphics::initGraphics(HINSTANCE hinstance)
	{
		GBufferedHDC = ::CreateCompatibleDC(NULL);

		//////////////
		BITMAPINFO info = { 0 };
		info.bmiHeader.biSize = sizeof(info.bmiHeader);
		info.bmiHeader.biWidth = SCREEN_WIDTH;
		info.bmiHeader.biHeight = -SCREEN_HEIGHT;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 32;
		info.bmiHeader.biCompression = BI_RGB;
		info.bmiHeader.biSizeImage = SCREEN_WIDTH * SCREEN_HEIGHT * 32 / 8;

		//创建一块内存纹理并获取其数据指针
		void *pBits = NULL;
		GBufferedHandle = ::CreateDIBSection(GBufferedHDC, &info, DIB_RGB_COLORS, &pBits, NULL, 0);
		::SelectObject(GBufferedHDC, GBufferedHandle);

		//这里："dib.dsBM.bmBits"将指向hdib的像素
		::GetObject(GBufferedHandle, sizeof(DIBSECTION), &GDIBSection);
		GDatas = (BYTE*)GDIBSection.dsBm.bmBits;
		GPitch = GDIBSection.dsBm.bmWidthBytes;

		GBgBrush = ::CreateSolidBrush(RGB(0, 0, 0));
		::SelectObject(GBufferedHDC, GBgBrush);
		//设置字体
		HFONT hfnt = (HFONT)::GetStockObject(OEM_FIXED_FONT);
		::SelectObject(GBufferedHDC, hfnt);
		//设置文字背景为透明色
		::SetBkMode(GBufferedHDC, TRANSPARENT);

		GZBuffer = new TFloat[SCREEN_WIDTH*SCREEN_HEIGHT];
		memset(GZBuffer, 0, sizeof(TFloat)*SCREEN_WIDTH*SCREEN_HEIGHT);
		
		return true;
	}

	//关闭绘图系统
	void TGraphics::shutdownGraphics()
	{
		::DeleteObject(GPen);
		::DeleteObject(GBgBrush);
		::DeleteObject(GBufferedHandle);
		::DeleteDC(GBufferedHDC);
	}

	//清空当前换红区，并将其颜色设置为黑色
	void TGraphics::clearBuffer(const TColor &c)
	{
		::FillRect(GBufferedHDC, &GBufferSize, GBgBrush);
		//重置深度缓存
		//注意，这里memset只能对Int类型的数组进行初始化，所以直接使用了Int类型
		//而没有使用float类型，应该使用float
		::memset(GZBuffer, 0, sizeof(TFloat)*SCREEN_WIDTH*SCREEN_HEIGHT);
	}

	//在当前缓冲区内绘制一条线
	void TGraphics::drawLine(TInt x0, TInt y0, TInt x1, TInt y1, const TColor &c)
	{
		::SelectObject(GBufferedHDC, GetStockObject(DC_PEN));
		::SetDCPenColor(GBufferedHDC, RGB(c.r, c.g, c.b));

		::MoveToEx(GBufferedHDC, x0, y0, NULL);
		::LineTo(GBufferedHDC, x1, y1);

		//::DeleteObject(hPen);
	}
	//绘制字符串
	void TGraphics::drawString(const TString& str, TInt x, TInt y, const TColor& c)
	{
		::SetTextColor(GBufferedHDC, RGB(c.r, c.g, c.b));
		::TextOut(GBufferedHDC, x, y, str.c_str(), str.length());
	}

	void TGraphics::fillTriangle(TInt x0, TInt y0, TInt x1, TInt y1, TInt x2, TInt y2, const TColor& c)
	{
	}

	void TGraphics::enableSmoothingMode(TBool enable)
	{
	}

	//检查Z深度值
	TBool TGraphics::checkZ(TInt x, TInt y, TFloat z)
	{
		//这里Z应该使用float类型来存储，如果使用int类型，那么会导致精度丢失
		//产生错误的现象

		TInt index = y * SCREEN_WIDTH + x;
		TFloat divZ = 1.0f / z;
		//这里是基于1/Z做的比较
		if (GZBuffer[index] > divZ)
			return false;

		GZBuffer[index] = divZ;
		return true;
	}

	void TGraphics::setPixel(TInt x, TInt y,/*TFloat z,*/const TColor& c)
	{
		//这里本来应该计算Z值，但为了避免对Image像素的读取，将Z检查分离了出来
		//因此，在调用setPixel之前应先检测checkZ，返回true再调用setPixel

		BYTE* pSrcPix = GDatas + (GPitch*y);
		pSrcPix += x * 4;
		//颜色赋值，绿，蓝，红，透明度
		pSrcPix[0] = c.b;
		pSrcPix[1] = c.g;
		pSrcPix[2] = c.r;
		pSrcPix[3] = c.a;
	}

	TColor TGraphics::getPixel(TInt x, TInt y)
	{
		//指针每次在内存中向前移动一个像素
		BYTE* pSrcPix = GDatas + (GPitch * y);
		pSrcPix += x * 4;
		return TColor(pSrcPix[2], pSrcPix[1], pSrcPix[0]);
	}

	//将已经绘制好的缓冲区递交给Graphics在屏幕上绘制，并将当前缓冲区设置为另一个缓冲区
	void TGraphics::fillBuffer(HDC hdc)
	{
		::BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GBufferedHDC, 0, 0, SRCCOPY); 

	}

}