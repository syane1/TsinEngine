#pragma once

#include "TCommon.h"

namespace T3D 
{
	#pragma region 整数/浮点数向量 带引擎的写法

	//二维浮点数向量
	typedef struct {
		TFloat x;
		TFloat y;
	}TVECTOR2f;

	//二维整数向量
	typedef struct {
		TInt x;
		TInt y;
	}TVECTOR2x;

	//三维浮点数向量
	typedef struct {
		TFloat x;
		TFloat y;
		TFloat z;
	}TVECTOR3f;

	//三维整数向量
	typedef struct {
		TInt x;
		TInt y;
		TInt z;
	}TVECTOR3x;

	//四维浮点数向量
	typedef struct {
		TFloat x;
		TFloat y;
		TFloat z;
		TFloat w;
	}TVECTOR4f;

	//四维整数向量
	typedef struct {
		TInt x;
		TInt y;
		TInt z;
		TInt w;
	}TVECTOR4x;

	//浮点平面
	typedef struct {
		TFloat a;
		TFloat b;
		TFloat c;
		TFloat d;
	}TPLANETf;

	//整数平面
	typedef struct {
		TInt a;
		TInt b;
		TInt c;
		TInt d;
	}TPLANEx;

	//4x4浮点矩阵
	typedef struct TMATRIXf
	{
	public :
		TFloat* operator[](const TInt row)
		{
			return &f[row << 2];
		}
		union 
		{
			struct {
				TFloat _11, _12, _13, _14,
						_21, _22, _23, _24,
						_31, _32, _33, _34,
						_41, _42, _43, _44;
			};
			TFloat m[4][4];//二维数组
			TFloat f[16];//一维数组
		};
	}TMATRIXf,*TmMATRIXf;

	//4x4点矩阵
	typedef struct TMATRIXx
	{
	public:
		TInt* operator[](const TInt row)
		{
			return &f[row << 2];
		}
		union
		{
			struct {
				TInt _11, _12, _13, _14,
					_21, _22, _23, _24,
					_31, _32, _33, _34,
					_41, _42, _43, _44;
			};
			TInt m[4][4];//二维数组
			TInt f[16];//一维数组
		};
	}TMATRIXx, *TmMATRIXx; 

	//3x3浮点矩阵
	typedef struct TMATRIX3f
	{
	public:
		TFloat* operator[](const TInt row)
		{
			return &f[row * 3];
		}
		TFloat f[9];
	}TMATRIX3f,*TmMATRIX3f;

	//3x3点矩阵
	typedef struct TMATRIX3x
	{
	public:
		TInt* operator[](const TInt row)
		{
			return &f[row * 3];
		}
		TInt f[9];
	}TMATRIX3x,*TmMATRIX3x;
	#pragma endregion

	//矩阵 MATRIX4x4-TYP
	struct TMatrix44
	{
		static TMatrix44 IDENTITY;

		TFloat m[4][4];

		TMatrix44()
		{
			memset(m, 0, sizeof(TFloat) * 16);
		}

		TMatrix44(const TMatrix44&right)
		{
			m[0][0] = right.m[0][0], m[0][1] = right.m[0][1], m[0][2] = right.m[0][2], m[0][3] = right.m[0][3];
			m[1][0] = right.m[1][0], m[1][1] = right.m[1][1], m[1][2] = right.m[1][2], m[1][3] = right.m[1][3];
			m[2][0] = right.m[2][0], m[2][1] = right.m[2][1], m[2][2] = right.m[2][2], m[2][3] = right.m[2][3];
			m[3][0] = right.m[3][0], m[3][1] = right.m[3][1], m[3][2] = right.m[3][2], m[3][3] = right.m[3][3];
		}

		TMatrix44(TFloat m0_0, TFloat m0_1, TFloat m0_2, TFloat m0_3,
				TFloat m1_0, TFloat m1_1, TFloat m1_2, TFloat m1_3,
				TFloat m2_0, TFloat m2_1, TFloat m2_2, TFloat m2_3,
				TFloat m3_0, TFloat m3_1, TFloat m3_2, TFloat m3_3)
		{
			m[0][0] = m0_0, m[0][1] = m0_1, m[0][2] = m0_2, m[0][3] = m0_3;
			m[1][0] = m1_0, m[1][1] = m1_1, m[1][2] = m1_2, m[1][3] = m1_3;
			m[2][0] = m0_0, m[2][1] = m2_1, m[2][2] = m2_2, m[2][3] = m2_3;
			m[3][0] = m0_0, m[3][1] = m3_1, m[3][2] = m3_2, m[3][3] = m3_3;
		}

		//矩阵转置
		inline TMatrix44 transpose()const
		{
			TMatrix44 tmpMtx44;
			for (TInt i = 0; i < 4; ++i)
			{
				for (TInt j = 0; j < 4; ++j)
					tmpMtx44.m[i][j] = m[j][i];
			}
		}

		//获取矩阵中的某个值
		inline TFloat At(size_t i, size_t j)const
		{
			return m[i][j];
		}

		inline TFloat& At(size_t i, size_t j)
		{
			return m[i][j];
		}

		//矩阵加法操作
		inline TMatrix44 operator+(const TMatrix44& right)const
		{
			return TMatrix44(m[0][0] + right.m[0][0], m[0][1] + right.m[0][1], m[0][2] + right.m[0][2], m[0][3] + right.m[0][3],
							m[1][0] + right.m[1][0], m[1][1] + right.m[1][1], m[1][2] + right.m[1][2], m[1][3] + right.m[1][3],
							m[2][0] + right.m[2][0], m[2][1] + right.m[2][1], m[2][2] + right.m[2][2], m[2][3] + right.m[2][3],
							m[3][0] + right.m[3][0], m[3][1] + right.m[3][1], m[3][2] + right.m[3][2], m[3][3] + right.m[3][3]);
		}

		//矩阵减法操作
		inline TMatrix44 operator-(const TMatrix44& right)const
		{
			return TMatrix44(m[0][0] - right.m[0][0], m[0][1] - right.m[0][1], m[0][2] - right.m[0][2], m[0][3] - right.m[0][3],
				m[1][0] - right.m[1][0], m[1][1] - right.m[1][1], m[1][2] - right.m[1][2], m[1][3] - right.m[1][3],
				m[2][0] - right.m[2][0], m[2][1] - right.m[2][1], m[2][2] - right.m[2][2], m[2][3] - right.m[2][3],
				m[3][0] - right.m[3][0], m[3][1] - right.m[3][1], m[3][2] - right.m[3][2], m[3][3] - right.m[3][3]);
		}

		//矩阵与数的乘法操作
		inline TMatrix44 operator*(const TFloat& right)const
		{
			return TMatrix44(m[0][0] * right, m[0][1] * right, m[0][2] * right, m[0][3] * right,
				m[1][0] * right, m[1][1] * right, m[1][2] * right, m[1][3] * right,
				m[2][0] * right, m[2][1] * right, m[2][2] * right, m[2][3] * right,
				m[3][0] * right, m[3][1] * right, m[3][2] * right, m[3][3] * right);
		}

		//矩阵与数的除法操作
		inline TMatrix44 operator*(const TMatrix44& right)const
		{
			TMatrix44 tmpMtx44;
			for (TInt i=0; i < 4;++i)
			{
				for (TInt j=0; j < 4; ++j)
				{
					for (TInt k=0; k < 4; ++k)
						tmpMtx44.m[i][k] = m[i][k] * right.m[i][k];
				}
			}
			return tmpMtx44;
		}

		//矩阵的赋值
		inline TMatrix44& operator=(const TMatrix44& right)
		{
			m[0][0] = right.m[0][0]; m[0][1] = right.m[0][1]; m[0][2] = right.m[0][2]; m[0][3] = right.m[0][3];
			m[1][0] = right.m[1][0]; m[1][1] = right.m[1][1]; m[1][2] = right.m[1][2]; m[1][3] = right.m[1][3];
			m[2][0] = right.m[2][0]; m[2][1] = right.m[2][1]; m[2][2] = right.m[2][2]; m[2][3] = right.m[2][3];
			m[3][0] = right.m[3][0]; m[3][1] = right.m[3][1]; m[3][2] = right.m[3][2]; m[3][3] = right.m[3][3];
			return *this;
		}

		//判断两个矩阵是否相等
		inline TBool operator==(const TMatrix44& right)const
		{
			return m[0][0] == right.m[0][0] && m[0][1] == right.m[0][1] && m[0][2] == right.m[0][2] && m[0][3] == right.m[0][3] &&
					m[1][0] == right.m[1][0] && m[1][1] == right.m[1][1] && m[1][2] == right.m[1][2] && m[1][3] == right.m[1][3] &&
					m[2][0] == right.m[2][0] && m[2][1] == right.m[2][1] && m[2][2] == right.m[2][2] && m[2][3] == right.m[2][3] &&
					m[3][0] == right.m[3][0] && m[3][1] == right.m[3][1] && m[3][2] == right.m[3][2] && m[3][3] == right.m[3][3];
		}
	};
}