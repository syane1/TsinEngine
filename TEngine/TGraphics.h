#pragma once

#include "../TCore/TGraphics.h"

namespace T3D
{
	//��ͼ�ӿ�
	class TCanvas {
	public:
		virtual ~TCanvas() {}
		//����ֻ�������ģ��
		virtual void onPaint() = 0;

		//������³���
		virtual void update() = 0;
	};
}