#include "TGraphics.h"
#include "TUtil.h"

namespace T3D
{
	TBitmap::TBitmap(const TString& filename) :name(filename), pixels(NULL), pitch(0), width(0), height(0), valid(false)
	{
		//���ʽű������Ȼ�ȡĬ��·��
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

	//��������
	TBitmap::~TBitmap()
	{
		DeleteObject(hBitmap);
		DeleteDC(bitmapHDC);
		SafeDeleteArray(pixels);
	}
	
	//��ȡ����
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

	//��ʼ����ͼϵͳ
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

		//����һ���ڴ�������ȡ������ָ��
		void *pBits = NULL;
		GBufferedHandle = ::CreateDIBSection(GBufferedHDC, &info, DIB_RGB_COLORS, &pBits, NULL, 0);
		::SelectObject(GBufferedHDC, GBufferedHandle);

		//���"dib.dsBM.bmBits"��ָ��hdib������
		::GetObject(GBufferedHandle, sizeof(DIBSECTION), &GDIBSection);
		GDatas = (BYTE*)GDIBSection.dsBm.bmBits;
		GPitch = GDIBSection.dsBm.bmWidthBytes;

		GBgBrush = ::CreateSolidBrush(RGB(0, 0, 0));
		::SelectObject(GBufferedHDC, GBgBrush);
		//��������
		HFONT hfnt = (HFONT)::GetStockObject(OEM_FIXED_FONT);
		::SelectObject(GBufferedHDC, hfnt);
		//�������ֱ���Ϊ͸��ɫ
		::SetBkMode(GBufferedHDC, TRANSPARENT);

		GZBuffer = new TFloat[SCREEN_WIDTH*SCREEN_HEIGHT];
		memset(GZBuffer, 0, sizeof(TFloat)*SCREEN_WIDTH*SCREEN_HEIGHT);
		
		return true;
	}

	//�رջ�ͼϵͳ
	void TGraphics::shutdownGraphics()
	{
		::DeleteObject(GPen);
		::DeleteObject(GBgBrush);
		::DeleteObject(GBufferedHandle);
		::DeleteDC(GBufferedHDC);
	}

	//��յ�ǰ����������������ɫ����Ϊ��ɫ
	void TGraphics::clearBuffer(const TColor &c)
	{
		::FillRect(GBufferedHDC, &GBufferSize, GBgBrush);
		//������Ȼ���
		//ע�⣬����memsetֻ�ܶ�Int���͵�������г�ʼ��������ֱ��ʹ����Int����
		//��û��ʹ��float���ͣ�Ӧ��ʹ��float
		::memset(GZBuffer, 0, sizeof(TFloat)*SCREEN_WIDTH*SCREEN_HEIGHT);
	}

	//�ڵ�ǰ�������ڻ���һ����
	void TGraphics::drawLine(TInt x0, TInt y0, TInt x1, TInt y1, const TColor &c)
	{
		::SelectObject(GBufferedHDC, GetStockObject(DC_PEN));
		::SetDCPenColor(GBufferedHDC, RGB(c.r, c.g, c.b));

		::MoveToEx(GBufferedHDC, x0, y0, NULL);
		::LineTo(GBufferedHDC, x1, y1);

		//::DeleteObject(hPen);
	}
	//�����ַ���
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

	//���Z���ֵ
	TBool TGraphics::checkZ(TInt x, TInt y, TFloat z)
	{
		//����ZӦ��ʹ��float�������洢�����ʹ��int���ͣ���ô�ᵼ�¾��ȶ�ʧ
		//�������������

		TInt index = y * SCREEN_WIDTH + x;
		TFloat divZ = 1.0f / z;
		//�����ǻ���1/Z���ıȽ�
		if (GZBuffer[index] > divZ)
			return false;

		GZBuffer[index] = divZ;
		return true;
	}

	void TGraphics::setPixel(TInt x, TInt y,/*TFloat z,*/const TColor& c)
	{
		//���ﱾ��Ӧ�ü���Zֵ����Ϊ�˱����Image���صĶ�ȡ����Z�������˳���
		//��ˣ��ڵ���setPixel֮ǰӦ�ȼ��checkZ������true�ٵ���setPixel

		BYTE* pSrcPix = GDatas + (GPitch*y);
		pSrcPix += x * 4;
		//��ɫ��ֵ���̣������죬͸����
		pSrcPix[0] = c.b;
		pSrcPix[1] = c.g;
		pSrcPix[2] = c.r;
		pSrcPix[3] = c.a;
	}

	TColor TGraphics::getPixel(TInt x, TInt y)
	{
		//ָ��ÿ�����ڴ�����ǰ�ƶ�һ������
		BYTE* pSrcPix = GDatas + (GPitch * y);
		pSrcPix += x * 4;
		return TColor(pSrcPix[2], pSrcPix[1], pSrcPix[0]);
	}

	//���Ѿ����ƺõĻ������ݽ���Graphics����Ļ�ϻ��ƣ�������ǰ����������Ϊ��һ��������
	void TGraphics::fillBuffer(HDC hdc)
	{
		::BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GBufferedHDC, 0, 0, SRCCOPY); 

	}

}