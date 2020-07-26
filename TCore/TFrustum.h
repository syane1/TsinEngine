#pragma once
#include "TCommon.h"
#include "TVector.h"
#include "TMatrix.h"
#include "TCore.h"

namespace T3D
{
	enum CAMERA_TYPE
	{
		CAMERA_MODEL_ELUA = 0x0001,		//ŷ�������ģ��
		CAMERA_MODEL_UVN = 0x0002,		//UVN�����ģ��
	};

	struct TFrustum
	{
		TInt		state;				//�����״̬
		TInt		attribute;			//���������
		CAMERA_TYPE	camMode;			

		TVector4D	position;			//���������������ϵ��λ��
		TVector4D	direction;			//�����ע�ӵķ���--����

		//UVNģ��ʹ�ã����ڶ���UVN�����
		TVector4D	camUp;				//UVN�����ģ�͵ĳ�������
		TVector4D	camRight;			//UVN�����ģ�͵ĳ�������
		TVector4D	camLook;			//UVN�����ģ�͵ĳ���ע�ӵ㷽��
		TVector4D	camTarget;			//UVN�����ģ�͵�ע�ӵ�

		TFloat		view_dist;			//�ӵ㵽ͶӰ��ľ���

		TFloat		fov;				//ˮƽ����ʹ�ֱ�������Ұ

		TFloat		clip_z_near;		//���ü���
		TFloat		clip_z_far;			//Զ�ü���

		TPlane3D	clip_plane_R;		//�Ҳü���
		TPlane3D	clip_plane_L;		//��ü���
		TPlane3D	clip_plane_T;		//�ϲü���
		TPlane3D	clip_plane_B;		//�²ü���

		TFloat		viewplane_width;	//ͶӰ����
		TFloat		viewplane_height;	//���ڹ�һ��ͶӰ��Ϊ2x2�������С����Ļ���ڴ�С��ͬ

		TFloat		viewport_width;		//��Ļ/�ӿڵĴ�С
		TFloat		viewport_height;
		TFloat		viewport_center_X;	//��Ļ��������
		TFloat		viewport_center_Y;	

		TFloat		aspect_ratio;		//��Ļ�ĸ߿��

		TMatrix44	mWorldToCamera;		//�������굽�������任����
		TMatrix44	mCameraToRespective;//������굽͸������任����
		TMatrix44	mPerspectiveToScreen;//͸�����굽��Ļ����任����

		TFrustum(CAMERA_TYPE mode, const TVector4D &pos, const TVector4D &dir,
				const TVector4D& target, TFloat  nearZ, TFloat farZ, TFloat ffov,
				TFloat viewportWidth, TFloat viewportHeight);

	};
}