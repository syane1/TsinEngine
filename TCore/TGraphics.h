#pragma once

#include "TCommon.h"
#include <Windows.h>

//ͼ����Ⱦ�ײ��ʵ��

namespace T3D 
{
	extern HINSTANCE GHInstance;

	//ͼƬ��װ������ͼƬʹ�õ�GDI+��ͼƬ��
	//��������������з�װ����ƽ̨��صĶ�������

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
		//��ʼ����ͼϵͳ
		static TBool initGraphics(HINSTANCE hinstance);
		//�رջ�ͼϵͳ
		static void shutdownGraphics();

		//���Zֵ������true���ʾͨ�������Ե���setPixel
		static TBool checkZ(TInt x, TInt y, TFloat z);
		static void setPixel(TInt x, TInt y,/*TFloat z,*/const TColor &c);
		static TColor getPixel(TInt x, TInt y);
		//�ڵ�ǰ�������ڻ���һ����
		static void drawLine(TInt x0, TInt y0, TInt x1, TInt y1, const TColor& c);
		//�����ַ���
		static void drawString(const TString& str, TInt x, TInt y, const TColor& c = TColor(255, 255, 255));
		//���������
		static void fillTriangle(TInt x0, TInt y0, TInt x1, TInt y1, TInt x2, TInt y2, const TColor& c = TColor(255, 255, 255));
		//ƽ��ģʽ
		static void enableSmoothingMode(TBool enable);

		//�����ǰ����������������ɫ����Ϊ��ɫ
		static void clearBuffer(const TColor& c = TColor());
		//���Ѿ����ƺõĻ������ݽ���Graphics����Ļ�ϻ��ƣ�������ǰ����������Ϊ��һ��������
		static void fillBuffer(HDC hdc);
		//��ȡ��Ļ�Ŀ�͸�
		static TInt getScreenWidth() { return SCREEN_WIDTH; }
		static TInt getScreenHeight() { return SCREEN_HEIGHT; }

	private:

		//�������
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