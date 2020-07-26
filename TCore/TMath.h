#pragma once

#include "TCommon.h"

namespace T3D
{
	template<class T>
	const T&Min(const T& a, const T& b){
		return(a < b) ? a : b;
	}

	template<class T>
	const T& Max(const T&a, const T& b) {
		return (b < a) ? a : b;
	}

	//将val的范围限制在min~max之间
	template<typename T>
	const T& Clamp(const T& val, const T& minV, const T& maxV){
		return Min(Max(val, minV), maxV);
	}

	//获取平移变换矩阵
	void GetTranslateMatrix44(TMatrix44& mat, TFloat x, TFloat y, TFloat z);

	//获取缩放变换矩阵
	void GetScaleMatrix44(TMatrix44& mat, TFloat x, TFloat y, TFloat z);

	//获取沿x轴旋转变换矩阵
	//旋转参数为角度，非弧度
	void GetRotateMatrix44X(TMatrix44& mat, TFloat degree);
	//获取沿y轴旋转变换矩阵
	//旋转参数为角度，非弧度
	void GetRotateMatrix44Y(TMatrix44& mat, TFloat degree);
	//获取沿z轴旋转变换矩阵
	//旋转参数为角度，非弧度
	void GetRotateMatrix44Z(TMatrix44& mat, TFloat degree);
	//沿任意轴旋转
	//旋转参数为角度，非弧度
	void GetRotateMatrix44(TMatrix44& mat, const TVector4D& normal, TFloat degree);

	void GetVertex4DAddVector4D(const TVertex4D& v, const TVector4D& p, TVertex4D& result);

	//3D向量x44矩阵
	void GetVector3DMulMatrix44(const TVector3D& vec, const TMatrix44& mat, TVector3D &result);

	//4D向量x44矩阵
	void GetVector4DMulMatrix44(const TVector4D& vec, const TMatrix44& mat, TVector4D& result);

	void GetVertex4DMulMatrix44(const TVertex4D& vec, const TMatrix44& mat, TVertex4D& result);

}