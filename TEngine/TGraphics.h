#pragma once

#include "../TCore/TGraphics.h"

namespace T3D
{
	//绘图接口
	class TCanvas {
	public:
		virtual ~TCanvas() {}
		//这里只负责绘制模型
		virtual void onPaint() = 0;

		//这里更新场景
		virtual void update() = 0;
	};
}