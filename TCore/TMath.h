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

	//��val�ķ�Χ������min~max֮��
	template<typename T>
	const T& Clamp(const T& val, const T& minV, const T& maxV){
		return Min(Max(val, minV), maxV);
	}

	//��ȡƽ�Ʊ任����
	void GetTranslateMatrix44(TMatrix44& mat, TFloat x, TFloat y, TFloat z);

	//��ȡ���ű任����
	void GetScaleMatrix44(TMatrix44& mat, TFloat x, TFloat y, TFloat z);

	//��ȡ��x����ת�任����
	//��ת����Ϊ�Ƕȣ��ǻ���
	void GetRotateMatrix44X(TMatrix44& mat, TFloat degree);
	//��ȡ��y����ת�任����
	//��ת����Ϊ�Ƕȣ��ǻ���
	void GetRotateMatrix44Y(TMatrix44& mat, TFloat degree);
	//��ȡ��z����ת�任����
	//��ת����Ϊ�Ƕȣ��ǻ���
	void GetRotateMatrix44Z(TMatrix44& mat, TFloat degree);
	//����������ת
	//��ת����Ϊ�Ƕȣ��ǻ���
	void GetRotateMatrix44(TMatrix44& mat, const TVector4D& normal, TFloat degree);

	void GetVertex4DAddVector4D(const TVertex4D& v, const TVector4D& p, TVertex4D& result);

	//3D����x44����
	void GetVector3DMulMatrix44(const TVector3D& vec, const TMatrix44& mat, TVector3D &result);

	//4D����x44����
	void GetVector4DMulMatrix44(const TVector4D& vec, const TMatrix44& mat, TVector4D& result);

	void GetVertex4DMulMatrix44(const TVertex4D& vec, const TMatrix44& mat, TVertex4D& result);

}