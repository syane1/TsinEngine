#include "TCamera.h"
#include "TFrustum.h"

namespace T3D
{
	TCamera::TCamera() :mLockTarget(false), mNeedUpdate(true), mRenderMode(RENDER_SOLID)
	{
		mFrustum = new TFrustum(CAMERA_MODEL_UVN, TVector4D::ZERO, TVector4D::ZERO, TVector4D::ZERO, 10, 1000, 75,SCREEN_WIDTH, SCREEN_HEIGHT);
		Update();
	}


	TCamera::~TCamera() { SafeDelete(mFrustum); }

	void TCamera::Update()
	{
		if (mNeedUpdate)
		{
			TMatrix44 mt_inv;
			GetTranslateMatrix44(mt_inv, -mFrustum->position.x, -mFrustum->position.y, -mFrustum->position.z);
			mFrustum->camLook = mFrustum->camTarget - mFrustum->position;
			mFrustum->camUp = TVector4D::UNIT_Y;
			mFrustum->camRight = mFrustum->camUp.CrossProduct(mFrustum->camLook);
			mFrustum->camUp = mFrustum->camLook.CrossProduct(mFrustum->camRight);

			mFrustum->camLook.Normalize();
			mFrustum->camRight.Normalize();
			mFrustum->camUp.Normalize();

			TMatrix44 mat_uvn(mFrustum->camRight.x, mFrustum->camUp.x, mFrustum->camLook.x, 0,
				mFrustum->camRight.y, mFrustum->camUp.y, mFrustum->camLook.y, 0,
				mFrustum->camRight.z, mFrustum->camUp.z, mFrustum->camLook.z, 0,
				0, 0, 0, 1
			);

			mFrustum->mWorldToCamera = mt_inv * mat_uvn;

			mNeedUpdate = false;
		}
	}

	void TCamera::setZNear(TFloat znear)
	{
		mFrustum->clip_z_near = znear;
		mNeedUpdate = true;
	}

	void TCamera::setZFar(TFloat zfar)
	{
		mFrustum->clip_z_far = zfar;
		mNeedUpdate = true;
	}

	void TCamera::setPosition(const TVector3D& pos)
	{
		mFrustum->position.x = pos.x;
		mFrustum->position.y = pos.y;
		mFrustum->position.z = pos.z;

		mNeedUpdate = true;
	}

	void TCamera::setTarget(const TVector3D& target, TBool lockTarget)
	{
		mFrustum->camTarget.x = target.x;
		mFrustum->camTarget.y = target.y;
		mFrustum->camTarget.z = target.z;

		mLockTarget = lockTarget;
		mNeedUpdate = true;
	}

	//基于世界坐标系移动
	void TCamera::Move(const TVector3D& mov)
	{
		mFrustum->position.x += mov.x;
		mFrustum->position.y += mov.y;
		mFrustum->position.z += mov.z;

		if (!mLockTarget)
		{
			mFrustum->camTarget.x += mov.x;
			mFrustum->camTarget.y += mov.y;
			mFrustum->camTarget.z += mov.z;
		}

		mNeedUpdate = true;
	}

	//基于摄像机自身坐标系移动
	void TCamera::moveRelative(const TVector3D& mov)
	{
		TVector4D offset = mFrustum->camRight * mov.x + mFrustum->camUp*mov.y + mFrustum->camLook*mov.z;
		mFrustum->position = mFrustum->position + offset;

		if (!mLockTarget)
		{
			mFrustum->camTarget = mFrustum->camTarget + offset;
		}

		mNeedUpdate = true;
	}

	//绕y轴旋转
	void TCamera::yaw(TFloat yDegree)
	{
		TMatrix44 rotMat;
		TVector4D distence = mFrustum->camTarget - mFrustum->position;
		GetRotateMatrix44(rotMat, mFrustum->camUp, yDegree);
		GetVector4DMulMatrix44(distence, rotMat, distence);
		//旋转后的目标点
		mFrustum->camTarget = distence + mFrustum->position;
		mNeedUpdate = true;
	}

	//绕x轴旋转
	void TCamera::Pitch(TFloat pDegree)
	{
		TMatrix44 rotMat;
		TVector4D distence = mFrustum->camTarget - mFrustum->position;
		GetRotateMatrix44(rotMat, mFrustum->camRight, pDegree);
		GetVector4DMulMatrix44(distence, rotMat, distence);
		//旋转后的目标点
		mFrustum->camTarget = distence + mFrustum->position;
		mNeedUpdate = true;
	}

}