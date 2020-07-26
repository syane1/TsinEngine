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

		//�������淶��
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

		//�����ĵ��
		inline TFloat DotProduct(const TVector2D&right)const
		{
			return x * right.x + y * right.y;
		}

		//������������֮��ļнǣ����ط�ʽΪ����
		inline TFloat AngleBetween(const TVector2D&right)const
		{
			TFloat lenProduct = Length()*right.Length();
			//������
			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;

			TFloat f = DotProduct(right) / lenProduct;
			f = Clamp(f, -1.0f, 1.0f);

			return acos(f);
		}

		//�жϳ����Ƿ�Ϊ��
		inline bool IsZero() const
		{
			return Length() < EPSILON_E6;
		}
		
		//������������
		inline TFloat Length() const
		{
			return  sqrt(x*x + y * y);
		}

		//���س��ȵ�ƽ��
		inline TFloat SquaredLength() const
		{
			return x * x + y * y;
		}

		//���������ĳ˷�
		inline TVector2D operator*(float right) const
		{
			return TVector2D(x*right, y*right);
		}

		//�������
		inline TVector2D operator*(const TVector2D&  right)const
		{
			return TVector2D(x*right.x, y*right.y);
		}

		//�����ӷ�
		inline TVector2D operator+(const TVector2D&right)const
		{
			return TVector2D(x + right.x, y + right.y);
		}

		//��������
		inline TVector2D operator-(const TVector2D& right)const
		{
			return TVector2D(x - right.x, y - right.y);
		}

		//����ȡ��
		inline TVector2D operator~()
		{
			x = -x;
			y = -y;
			return *this;
		}

		//����ȡ��
		inline TVector2D operator+()
		{
			x = +x;
			y = +y;
			return *this;
		}

		//�ж����������Ƿ����
		inline bool operator==(const TVector2D & right)const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y);
		}

		//�����ĸ�ֵ
		inline TVector2D operator=(const TVector2D&right)
		{
			x = right.x;
			y = right.y;
			return *this;
		}
	};

	//3D������VECTOR3D_TYP
	struct TVector3D
	{
		static TVector3D ZERO;
		static TVector3D UNIT_X;
		static TVector3D UNIT_Y;
		static TVector3D UNIT_Z;

		TFloat x;
		TFloat y;
		TFloat z;
		//���캯��
		TVector3D():x(0), y(0), z(0) {}
		TVector3D(TFloat xf, TFloat yf, TFloat zf) :x(xf), y(yf), z(zf) {}
		TVector3D(const TVector3D& v) :x(v.x), y(v.y), z(v.z) {}
		
		//�������
		inline TFloat DotProduct(const TVector3D&right)const
		{
			return x * right.x + y * right.y + z * right.z;
		}
		
		//�������
		inline TVector3D CrossProduct(const TVector3D&right)const
		{
			return TVector3D(y*right.z - z * right.y,
								z*right.x - x * right.z,
								x*right.y - y * right.x
							);
		}

		//�������淶��
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

		//������������֮��ļнǣ����ط�ʽΪ����
		inline TFloat AngleBetween(const TVector3D&right)const
		{
			TFloat lenProduct = Length()*right.Length();

			//������
			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;

			TFloat f = DotProduct(right) / lenProduct;
			return acos(f);
		}

		//�����ĳ���
		inline TFloat Length()const
		{
			return sqrt(x*x + y * y + z * z);
		}

		//���������ƽ��
		inline TFloat SquaredLength()const
		{
			return x * x + y * y + z * z;
		}

		//��������ֵ���
		inline TVector3D operator*(TFloat right)const
		{
			return TVector3D(x*right, y*right, z*right);
		}

		//�������������
		inline TVector3D operator*(const TVector3D&right)const
		{
			return TVector3D(x*right.x, y*right.y, z*right.z);
		}

		//�����ļӷ�
		inline TVector3D operator+(const TVector3D&right)const
		{
			return TVector3D(x + right.x, y + right.y, z + right.z);
		}

		//�����ļ���
		inline TVector3D operator-(const TVector3D&right)const
		{
			return TVector3D(x - right.x, y - right.y, z - right.z);
		}

		//����ȡ��
		inline TVector3D operator-()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}

		//����ȡ��
		inline TVector3D operator+()
		{
			x = +x;
			y = +y;
			z = +z;
			return *this;
		}

		//���������Ƿ����
		inline bool operator==(const TVector3D&right)
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y) && EqualFloat(z, right.z);
		}

		//������ֵ
		inline TVector3D operator=(const TVector3D& right)
		{
			x = right.x;
			y = right.y;
			z = right.z;
			return *this;
		}

	};

	//4D������VECTOR4D_TYP
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

		//���캯��
		TVector4D() :x(0), y(0), z(0), w(1) {}
		TVector4D(TFloat xf, TFloat yf, TFloat zf, TFloat wf=1) :x(xf), y(yf), z(zf), w(wf) {}
		TVector4D(const TVector4D&v) :x(v.x), y(v.y), z(v.z), w(v.w) {}

		//�����ĳ���
		inline TFloat Length()const
		{
			return sqrt(x*x + y * y + z * z );
		}

		//�������ȵ�ƽ��
		inline TFloat SquaredLength() const
		{
			return x * x + y * y + z * z ;
		}

		//�������淶��
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

		//���������Ƿ�Ϊ��
		inline bool IsZero() const
		{
			return Length() < EPSILON_E6;
		}

		//������������֮��ļнǣ����ط�ʽΪ����
		inline TFloat AngleBetween(const TVector4D&right)const
		{
			TFloat lenProduct = Length()*right.Length();

			//������
			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;
			TFloat f = DotProduct(right) / lenProduct;
			f=Clamp(f,-1.0f,1.0f);

			return acos(f);
		}

		//�����ĵ��
		inline TFloat DotProduct(const TVector4D&right)const
		{
			return x * right.x+y*right.y+z*right.z;
		}

		//�������
		inline TVector4D CrossProduct(const TVector4D&right)const
		{
			return TVector4D(y*right.z - z * right.y,
								z*right.x*right.x - x * right.z,
								x*right.y - y * right.x,
								1
							);
		}

		//�����������
		inline TVector4D operator*(TFloat right)const
		{
			return TVector4D(x*right, y*right, z*right, 1);
		}

		//�������
		inline TVector4D operator*(const TVector4D& right)const
		{
			return TVector4D(x*right.x, y*right.y, z*right.z, 1);
		}

		//�����ӷ�
		inline TVector4D operator+(const TVector4D& right)const
		{
			return TVector4D(x + right.x, y + right.y, z + right.z, 1);
		}

		//����ȡ��
		inline TVector4D operator-()const
		{
			return TVector4D(-x, -y, -z, w);
		}
		
		//��������
		inline TVector4D operator-(const TVector4D& right)const
		{
			return TVector4D(x - right.x, y - right.y, z - right.z, 1);
		}

		//�����Ƿ����
		inline bool operator==(const TVector4D&right)const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y) &&
				EqualFloat(z, right.z) && EqualFloat(w, right.w);
		}
		//������ֵ
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