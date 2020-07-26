#include "TFrustum.h"

namespace T3D
{
	TFrustum::TFrustum(CAMERA_TYPE mode, const TVector4D &pos, const TVector4D &dir,
		const TVector4D& target, TFloat nearZ, TFloat farZ, TFloat ffov,
		TFloat viewportWidth, TFloat viewportHeight)
		:camMode(mode), position(pos), direction(dir), camTarget(target),
		clip_z_near(nearZ), clip_z_far(farZ), fov(ffov),
		viewport_width(viewportWidth), viewport_height(viewportHeight),
		camUp(TVector4D::UNIT_Y), camRight(TVector4D::UNIT_X), camLook(TVector4D::UNIT_Z),
		mWorldToCamera(TMatrix44::IDENTITY), mCameraToRespective(TMatrix44::IDENTITY),
		mPerspectiveToScreen(TMatrix44::IDENTITY)
	{
		viewport_center_X = (viewport_width - 1)*0.5f;
		viewport_center_Y = (viewport_height - 1)*0.5f;

		aspect_ratio = viewport_width / viewport_height;

		viewplane_width = 2.0f;
		viewplane_height = 2.0f / aspect_ratio;

		TFloat tan_fov_div2 = tan(Degree_To_Radian(fov*0.5f));
		view_dist = 0.5f*viewplane_width / tan_fov_div2;

		//设置裁剪平面
		//裁剪平面坐标是基于摄像机坐标系的
		TVector4D point = TVector4D::ZERO;
		//右裁剪面
		TVector4D normalR = TVector4D(-view_dist, 0, viewplane_width*0.5f);
		clip_plane_L = TPlane3D(point, normalR);

		//左裁剪面
		TVector4D normalL = TVector4D(view_dist, 0, viewplane_width*0.5f);
		clip_plane_R = TPlane3D(point, normalL);

		//上裁剪面
		TVector4D normalT = TVector4D(0, -view_dist, viewplane_height*0.5f);
		clip_plane_T = TPlane3D(point, normalT);

		//下裁剪面
		TVector4D normalB = TVector4D(0, view_dist, viewplane_height*0.5f);
		clip_plane_B = TPlane3D(point, normalB);

		//摄像机坐标系到透视坐标系变换矩阵，经过矩阵变换之后需要除以w
		mPerspectiveToScreen = TMatrix44(view_dist, 0, 0, 0,
										0, view_dist*aspect_ratio, 0, 0,
	 									0, 0, 1, 1,
										0, 0, 0, 0);
	}
}