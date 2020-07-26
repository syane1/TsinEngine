#include "TMath.h"
#include "TVector.h"
#include "TMatrix.h"
#include "TCore.h"

namespace T3D
{
	//获取平移变换矩阵
	void GetTranslateMatrix44(TMatrix44& mat, TFloat x, TFloat y, TFloat z)
	{
		mat.m[0][0] = 1, mat.m[0][1] = 0, mat.m[0][2] = 0, mat.m[0][3] = 0,
		mat.m[1][0] = 0, mat.m[1][1] = 1, mat.m[1][2] = 0, mat.m[1][3] = 0,
		mat.m[2][0] = 0, mat.m[2][1] = 0, mat.m[2][2] = 1, mat.m[2][3] = 0,
		mat.m[3][0] = x, mat.m[3][1] = y, mat.m[3][2] = z, mat.m[3][3] = 1;
	}

	//获取缩放变换矩阵
	void GetScaleMatrix44(TMatrix44& mat, TFloat x, TFloat y, TFloat z)
	{
		mat.m[0][0] = x, mat.m[0][1] = 0, mat.m[0][2] = 0, mat.m[0][3] = 0,
		mat.m[1][0] = 0, mat.m[1][1] = y, mat.m[1][2] = 0, mat.m[1][3] = 0,
		mat.m[2][0] = 0, mat.m[2][1] = 0, mat.m[2][2] = z, mat.m[2][3] = 0,
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	//获取沿X轴旋转变换矩阵
	//旋转参数为角度，非弧度
	void GetRotateMatrix44Z(TMatrix44 & mat, TFloat degree)
	{
		TFloat sinA = sin(Degree_To_Radian(degree));
		TFloat cosA = cos(Degree_To_Radian(degree));


		mat.m[0][0] = cosA, mat.m[0][1] = sinA, mat.m[0][2] = 0, mat.m[0][3] = 0,
		mat.m[1][0] = -sinA, mat.m[1][1] = cosA, mat.m[1][2] = 0, mat.m[1][3] = 0,
		mat.m[2][0] = 0, mat.m[2][1] = 0, mat.m[2][2] = 1, mat.m[2][3] = 0,
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}     

	//获取沿Y轴旋转变换矩阵
	//旋转参数为角度，非弧度
	void GetRotateMatrix44X(TMatrix44& mat, TFloat degree)
	{
		TFloat sinA = sin(Degree_To_Radian(degree));
		TFloat cosA = cos(Degree_To_Radian(degree));


		mat.m[0][0] = 1, mat.m[0][1] = 0, mat.m[0][2] = 0, mat.m[0][3] = 0,
		mat.m[1][0] = 0, mat.m[1][1] = cosA, mat.m[1][2] = sinA, mat.m[1][3] = 0,
		mat.m[2][0] = 0, mat.m[2][1] = -sinA, mat.m[2][2] = cosA, mat.m[2][3] = 0,
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	//获取沿Z轴旋转变换矩阵
	//旋转参数为角度，非弧度
	void GetRotateMatrix44Y(TMatrix44& mat, TFloat degree)
	{
		TFloat sinA = sin(Degree_To_Radian(degree));
		TFloat cosA = cos(Degree_To_Radian(degree));


		mat.m[0][0] = cosA, mat.m[0][1] = 0, mat.m[0][2] = -sinA, mat.m[0][3] = 0,
		mat.m[1][0] = 0, mat.m[1][1] = 1, mat.m[1][2] = sinA, mat.m[1][3] = 0,
		mat.m[2][0] = sinA, mat.m[2][1] = 0, mat.m[2][2] = cosA, mat.m[2][3] = 0,
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	void GetRotateMatrix44(TMatrix44& mat, const TVector4D& n, TFloat degree)
	{
		TFloat sinA = sin(Degree_To_Radian(degree));
		TFloat cosA = cos(Degree_To_Radian(degree));

		TFloat oneCosA = 1 - cosA;

		mat.m[0][0] = n.x*n.x*oneCosA+cosA, mat.m[0][1] = n.x*n.y*oneCosA+n.x*sinA, mat.m[0][2] = n.x*n.z*oneCosA-n.y*sinA, mat.m[0][3] = 0,
		mat.m[1][0] = n.x*n.y*oneCosA-n.z*sinA, mat.m[1][1] = n.y*n.y*oneCosA+cosA, mat.m[1][2] = n.y*n.z*oneCosA+n.x*sinA, mat.m[1][3] = n.y*n.y*oneCosA+cosA,
		mat.m[2][0] = n.x*n.z*oneCosA+n.y*sinA, mat.m[2][1] = n.y*n.z*oneCosA-n.x*sinA, mat.m[2][2] = n.z*n.z*oneCosA+cosA, mat.m[2][3] = 0,
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	//3D向量x44矩阵
	void GetVector3DMulMatrix44(const TVector3D& vec, const TMatrix44& mat, TVector3D& result)
	{
		TFloat x = vec.x*mat.m[0][0] + vec.y*mat.m[1][0] + vec.z*mat.m[2][0] + mat.m[3][0];
		TFloat y = vec.x*mat.m[0][1] + vec.y*mat.m[1][1] + vec.z*mat.m[2][1] + mat.m[3][1];
		TFloat z = vec.x*mat.m[0][2] + vec.y*mat.m[1][2] + vec.z*mat.m[2][2] + mat.m[3][2];

		result.x = x, result.y = y, result.z = z;
	}

	//4D向量x44矩阵
	void GetVector4DMulMatrix44(const TVector4D& vec, const TMatrix44& mat, TVector4D& result)
	{
		TFloat x = vec.x*mat.m[0][0] + vec.y*mat.m[1][0] + vec.z*mat.m[2][0] + mat.m[3][0];
		TFloat y = vec.x*mat.m[0][1] + vec.y*mat.m[1][1] + vec.z*mat.m[2][1] + mat.m[3][1];
		TFloat z = vec.x*mat.m[0][2] + vec.y*mat.m[1][2] + vec.z*mat.m[2][2] + mat.m[3][2];
		TFloat w = 1;

		result.x = x, result.y = y, result.z = z, result.w = w;
	}

	void GetVertex4DMulMatrix44(const TVertex4D& vec, const TMatrix44& mat, TVertex4D& result)
	{
		//变换顶点
		TFloat x = vec.x*mat.m[0][0] + vec.y*mat.m[1][0] + vec.z*mat.m[2][0] + mat.m[3][0];
		TFloat y = vec.x*mat.m[0][1] + vec.y*mat.m[1][1] + vec.z*mat.m[2][1] + mat.m[3][1];
		TFloat z = vec.x*mat.m[0][2] + vec.y*mat.m[1][2] + vec.z*mat.m[2][2] + mat.m[3][2];
		TFloat w = 1;

		result.x = x, result.y = y, result.z = z, result.w = w;
	}

	void GetVertex4DAddVector4D(const TVertex4D& v, const TVertex4D& p, TVertex4D& result)
	{
		//定点变换
		TFloat x = v.x + p.x;
		TFloat y = v.y + p.y;
		TFloat z = v.z + p.z;
		
		result.x = x, result.y = y, result.z = z, result.w = 1;

	}

}