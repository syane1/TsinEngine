#pragma once

#include "TCore.h"

namespace T3D
{
	enum RenderMode 
	{
		RENDER_WIRE,			//线框渲染
		RENDER_SOLID,			//实体渲染
	};

	class TCamera
	{
	public:
		~TCamera();

		void Update();

		//设置近裁剪面距离
		void setZNear(TFloat znear);
		//设置远裁剪面距离
		void setZFar(TFloat zfar);

		//设置相机世界坐标
		void setPosition(const TVector3D& pos);
		//设置摄像机观察点，当lockTarget为true时，锁定观察点
		void setTarget(const TVector3D& target, TBool lockTarget = false);
		void releaseTarget() { mLockTarget = false; }

		//基于世界坐标系移动
		void Move(const TVector3D& mov);
		//基于摄像机自身坐标系移动
		void moveRelative(const TVector3D& mov);

		//绕y轴旋转
		void yaw(TFloat yDegree);
		//绕x轴旋转
		void Pitch(TFloat pDegree);

		//设置渲染模式，线框或实体
		void setRenderMode(RenderMode mode) { mRenderMode = mode; }
		RenderMode getRenderMode()const { return mRenderMode; }
		
		TFrustum* getFrustum()const { return mFrustum; }

	protected:
		friend class TSceneManager;
		TCamera();
		TFrustum* mFrustum;

		TBool mLockTarget;
		TBool mNeedUpdate;

		RenderMode mRenderMode;
	};

}