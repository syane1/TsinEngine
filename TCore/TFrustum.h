#pragma once
#include "TCommon.h"
#include "TVector.h"
#include "TMatrix.h"
#include "TCore.h"

namespace T3D
{
	enum CAMERA_TYPE
	{
		CAMERA_MODEL_ELUA = 0x0001,		//欧拉摄像机模型
		CAMERA_MODEL_UVN = 0x0002,		//UVN摄像机模型
	};

	struct TFrustum
	{
		TInt		state;				//摄像机状态
		TInt		attribute;			//摄像机属性
		CAMERA_TYPE	camMode;			

		TVector4D	position;			//摄像机在世界坐标系的位置
		TVector4D	direction;			//摄像机注视的方向--朝向

		//UVN模型使用，用于定义UVN摄像机
		TVector4D	camUp;				//UVN摄像机模型的朝上向量
		TVector4D	camRight;			//UVN摄像机模型的超右向量
		TVector4D	camLook;			//UVN摄像机模型的朝向注视点方向
		TVector4D	camTarget;			//UVN摄像机模型的注视点

		TFloat		view_dist;			//视点到投影面的距离

		TFloat		fov;				//水平方向和垂直方向的视野

		TFloat		clip_z_near;		//近裁剪面
		TFloat		clip_z_far;			//远裁剪面

		TPlane3D	clip_plane_R;		//右裁剪面
		TPlane3D	clip_plane_L;		//左裁剪面
		TPlane3D	clip_plane_T;		//上裁剪面
		TPlane3D	clip_plane_B;		//下裁剪面

		TFloat		viewplane_width;	//投影面宽高
		TFloat		viewplane_height;	//对于归一化投影，为2x2，否则大小与屏幕窗口大小相同

		TFloat		viewport_width;		//屏幕/视口的大小
		TFloat		viewport_height;
		TFloat		viewport_center_X;	//屏幕中心坐标
		TFloat		viewport_center_Y;	

		TFloat		aspect_ratio;		//屏幕的高宽比

		TMatrix44	mWorldToCamera;		//世界坐标到相机坐标变换矩阵
		TMatrix44	mCameraToRespective;//相机坐标到透视坐标变换矩阵
		TMatrix44	mPerspectiveToScreen;//透视坐标到屏幕坐标变换矩阵

		TFrustum(CAMERA_TYPE mode, const TVector4D &pos, const TVector4D &dir,
				const TVector4D& target, TFloat  nearZ, TFloat farZ, TFloat ffov,
				TFloat viewportWidth, TFloat viewportHeight);

	};
}