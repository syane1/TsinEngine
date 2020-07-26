#pragma once
#include "TCommon.h"
#include "TMath.h"
#include "TUtil.h"

namespace T3D{
	struct TVector2D
	{
		static TVector2D ZERO;
		static TVector2D UNIT_X;
		static TVector2D UNIT_Y;

		TFloat x;
		TFloat y;

		TVector2D():x(0),y(0){}
		TVector2D(TFloat xf, TFloat yf) :x(xf), y(yf) {}
		TVector2D(const TVector2D& v) :x(v.x), y(v.y) {}

		//将向量规范化
		inline TFloat Normalize()
		{
			TFloat len = Length(); 
			if (len > EPSILON_E6)
			{
				TFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
			}
			return len;
		}

		//向量的点乘
		inline TFloat DotProduct(const TVector2D&right)const
		{
			return x * right.x + y * right.y;
		}

		//返回两个向量之间的夹角，返回方式为弧度
		inline TFloat AngleBetween(const TVector2D&right)const
		{
			TFloat lenProduct = Length()*right.Length();
			//除零检查
			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;

			TFloat f = DotProduct(right) / lenProduct;
			f = Clamp(f, -1.0f, 1.0f);

			return acos(f);
		}

		//判断长度是否为零
		inline bool IsZero() const
		{
			return Length() < EPSILON_E6;
		}
		
		//返回向量长度
		inline TFloat Length() const
		{
			return  sqrt(x*x + y * y);
		}

		//返回长度的平方
		inline TFloat SquaredLength() const
		{
			return x * x + y * y;
		}

		//向量与数的乘法
		inline TVector2D operator*(float right) const
		{
			return TVector2D(x*right, y*right);
		}

		//向量点积
		inline TVector2D operator*(const TVector2D&  right)const
		{
			return TVector2D(x*right.x, y*right.y);
		}

		//向量加法
		inline TVector2D operator+(const TVector2D&right)const
		{
			return TVector2D(x + right.x, y + right.y);
		}

		//向量减法
		inline TVector2D operator-(const TVector2D& right)const
		{
			return TVector2D(x - right.x, y - right.y);
		}

		//向量取反
		inline TVector2D operator~()
		{
			x = -x;
			y = -y;
			return *this;
		}

		//向量取正
		inline TVector2D operator+()
		{
			x = +x;
			y = +y;
			return *this;
		}

		//判断两个向量是否相等
		inline bool operator==(const TVector2D & right)const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y);
		}

		//向量的赋值
		inline TVector2D operator=(const TVector2D&right)
		{
			x = right.x;
			y = right.y;
			return *this;
		}
	};

	//3D向量：VECTOR3D_TYP
	struct TVector3D
	{
		static TVector3D ZERO;
		static TVector3D UNIT_X;
		static TVector3D UNIT_Y;
		static TVector3D UNIT_Z;

		TFloat x;
		TFloat y;
		TFloat z;
		//构造函数
		TVector3D():x(0), y(0), z(0) {}
		TVector3D(TFloat xf, TFloat yf, TFloat zf) :x(xf), y(yf), z(zf) {}
		TVector3D(const TVector3D& v) :x(v.x), y(v.y), z(v.z) {}
		
		//向量点乘
		inline TFloat DotProduct(const TVector3D&right)const
		{
			return x * right.x + y * right.y + z * right.z;
		}
		
		//向量叉积
		inline TVector3D CrossProduct(const TVector3D&right)const
		{
			return TVector3D(y*right.z - z * right.y,
								z*right.x - x * right.z,
								x*right.y - y * right.x
							);
		}

		//将向量规范化
		inline TFloat Normalize()
		{
			TFloat len = Length();
			if (len > EPSILON_E6)
			{
				TFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
				z *= invLength;
			}
			return len;
		}

		inline bool IsZero() const
		{
			return Length() < EPSILON_E6;
		}

		//返回两个向量之间的夹角，返回方式为弧度
		inline TFloat AngleBetween(const TVector3D&right)const
		{
			TFloat lenProduct = Length()*right.Length();

			//除零检查
			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;

			TFloat f = DotProduct(right) / lenProduct;
			return acos(f);
		}

		//向量的长度
		inline TFloat Length()const
		{
			return sqrt(x*x + y * y + z * z);
		}

		//向量距离的平方
		inline TFloat SquaredLength()const
		{
			return x * x + y * y + z * z;
		}

		//向量与数值相乘
		inline TVector3D operator*(TFloat right)const
		{
			return TVector3D(x*right, y*right, z*right);
		}

		//向量与向量相乘
		inline TVector3D operator*(const TVector3D&right)const
		{
			return TVector3D(x*right.x, y*right.y, z*right.z);
		}

		//向量的加法
		inline TVector3D operator+(const TVector3D&right)const
		{
			return TVector3D(x + right.x, y + right.y, z + right.z);
		}

		//向量的减法
		inline TVector3D operator-(const TVector3D&right)const
		{
			return TVector3D(x - right.x, y - right.y, z - right.z);
		}

		//向量取反
		inline TVector3D operator-()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}

		//向量取正
		inline TVector3D operator+()
		{
			x = +x;
			y = +y;
			z = +z;
			return *this;
		}

		//两个向量是否相等
		inline bool operator==(const TVector3D&right)
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y) && EqualFloat(z, right.z);
		}

		//向量赋值
		inline TVector3D operator=(const TVector3D& right)
		{
			x = right.x;
			y = right.y;
			z = right.z;
			return *this;
		}

	};

	//4D向量：VECTOR4D_TYP
	struct TVector4D
	{
		static TVector4D ZERO;
		static TVector4D UNIT_X;
		static TVector4D UNIT_Y;
		static TVector4D UNIT_Z;

		TFloat x;
		TFloat y;
		TFloat z;
		TFloat w;

		//构造函数
		TVector4D() :x(0), y(0), z(0), w(1) {}
		TVector4D(TFloat xf, TFloat yf, TFloat zf, TFloat wf=1) :x(xf), y(yf), z(zf), w(wf) {}
		TVector4D(const TVector4D&v) :x(v.x), y(v.y), z(v.z), w(v.w) {}

		//向量的长度
		inline TFloat Length()const
		{
			return sqrt(x*x + y * y + z * z );
		}

		//向量长度的平方
		inline TFloat SquaredLength() const
		{
			return x * x + y * y + z * z ;
		}

		//将向量规范化
		inline TFloat Normalize()
		{
			TFloat len = Length();
			if (len > EPSILON_E6)
			{
				TFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
				z *= invLength;
				w = 1;
			}

			return len;
		}

		//向量长度是否为零
		inline bool IsZero() const
		{
			return Length() < EPSILON_E6;
		}

		//返回两个向量之间的夹角，返回方式为弧度
		inline TFloat AngleBetween(const TVector4D&right)const
		{
			TFloat lenProduct = Length()*right.Length();

			//除零检查
			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;
			TFloat f = DotProduct(right) / lenProduct;
			f=Clamp(f,-1.0f,1.0f);

			return acos(f);
		}

		//向量的点乘
		inline TFloat DotProduct(const TVector4D&right)const
		{
			return x * right.x+y*right.y+z*right.z;
		}

		//向量叉积
		inline TVector4D CrossProduct(const TVector4D&right)const
		{
			return TVector4D(y*right.z - z * right.y,
								z*right.x*right.x - x * right.z,
								x*right.y - y * right.x,
								1
							);
		}

		//向量与数相乘
		inline TVector4D operator*(TFloat right)const
		{
			return TVector4D(x*right, y*right, z*right, 1);
		}

		//向量点积
		inline TVector4D operator*(const TVector4D& right)const
		{
			return TVector4D(x*right.x, y*right.y, z*right.z, 1);
		}

		//向量加法
		inline TVector4D operator+(const TVector4D& right)const
		{
			return TVector4D(x + right.x, y + right.y, z + right.z, 1);
		}

		//向量取反
		inline TVector4D operator-()const
		{
			return TVector4D(-x, -y, -z, w);
		}
		
		//向量减法
		inline TVector4D operator-(const TVector4D& right)const
		{
			return TVector4D(x - right.x, y - right.y, z - right.z, 1);
		}

		//向量是否相等
		inline bool operator==(const TVector4D&right)const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y) &&
				EqualFloat(z, right.z) && EqualFloat(w, right.w);
		}
		//向量赋值
		inline TVector4D& operator=(const TVector4D&right)
		{
			x = right.x;
			y = right.y;
			z = right.z;
			w = right.w;

			return *this;
		}
	};
}