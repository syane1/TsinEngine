#pragma once

#include "TCore.h"

namespace T3D
{
	enum RenderMode 
	{
		RENDER_WIRE,			//�߿���Ⱦ
		RENDER_SOLID,			//ʵ����Ⱦ
	};

	class TCamera
	{
	public:
		~TCamera();

		void Update();

		//���ý��ü������
		void setZNear(TFloat znear);
		//����Զ�ü������
		void setZFar(TFloat zfar);

		//���������������
		void setPosition(const TVector3D& pos);
		//����������۲�㣬��lockTargetΪtrueʱ�������۲��
		void setTarget(const TVector3D& target, TBool lockTarget = false);
		void releaseTarget() { mLockTarget = false; }

		//������������ϵ�ƶ�
		void Move(const TVector3D& mov);
		//�����������������ϵ�ƶ�
		void moveRelative(const TVector3D& mov);

		//��y����ת
		void yaw(TFloat yDegree);
		//��x����ת
		void Pitch(TFloat pDegree);

		//������Ⱦģʽ���߿��ʵ��
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