#pragma once

#include <cmath>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <iostream>

#include "T3DHeader.h"

namespace T3D 
{
	//类型定义
	typedef char			TChar;
	typedef short			TShort;
	typedef float			TFloat;
	typedef int				TInt;
	typedef bool			TBool;
	typedef long			TLong;
	typedef double			TDouble;
	typedef std::string		TString;
	typedef std::wstring	TWString;

	typedef unsigned char	TUChar;
	typedef unsigned short	TUShort;
	typedef unsigned int	TUInt;
	
	//窗口大小
	const TInt SCREEN_WIDTH = 800;
	const TInt SCREEN_HEIGHT = 600;

	//项目默认名称
	const TString DEFAULT_PROJECT_NAME = "T3D_Default_Project";
	const TString DEFAULT_NAME = "T3D_Default";

	const TString DefaultPath = "Media/";
	static TString GetPath(const TString& file) { return DefaultPath + file; }

	//数学常量
	#define PI 3.14159265358979f
	#define PI2 (PI * 2.0f)
	#define PI_HAlF (PI * 0.5f)
	#define PI_QUATER (PI * 0.25f)
	#define PI_INV 0.31830988618379f

	//计算精度
	#define EPSILON_E6 ((TFloat)(1E-6))
	#define EPSILON_E12 ((TFloat)(1E-12))
	#define EPSILON_E14 ((TFloat)(1E-14))
	
	//角度与弧度转换
	#define Degree_To_Radian(deg)(deg*0.017453f)
	#define Radian_To_Degree(deg)(deg*0.017453f)

	#define Abs(a)((a)>0?(a):(-a))
	#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000)?1:0)
	#define KEY_UP(vk_code)((GetAsyncKeyState(vk_code) & 0x8000)?0:1)

	//安全删除
	#define SafeDelete(ptr) if(ptr){delete ptr;ptr=0;}
	#define SafeDeleteArray(ptr) if(ptr){delete [] ptr;ptr=0;}
	//输出
	#define COUT(a)std::cout<<#a<<":"<<(a)<<std::endl;
	
	//颜色定义
	#define TColor_16b(r,g,b)(((r) & 0xff)<<16|((g) & 0xff)<<8|(b) & 0xff)
	//TColor结构定义
	struct TColor
	{
		TUChar r, g, b, a;
		TColor(TInt color, TUChar alpha = 255)
		{
			r = (unsigned char)((color & 0xf0000)) >> 16;
			g = (unsigned char)((color & 0x00ff00) >> 8);
			b = (unsigned char)(color & 0x0000ff);
			a = alpha;
		}
		TColor(TUChar ri = 0, TUChar gi = 0, TUChar bi = 0, TUChar ai = 255) :r(ri), g(gi), b(bi), a(ai) {}

		TInt ToInt() const { return TColor_16b(r, g, b); }
		TColor operator*(const TColor &c)const
		{
			TInt ri = r * c.r >> 8;		//r * c.r / 256
			TInt gi = g * c.g >> 8;
			TInt bi = b * c.b >> 8;
			TInt ai = a * c.a >> 8;
			return TColor(ri, gi, bi, ai);
		}
	};
}
