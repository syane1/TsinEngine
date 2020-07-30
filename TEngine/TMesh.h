#pragma once

#include "TCommon.h"
#include "TCore.h"
#include "TMaterial.h"
#include "TObject.h"

namespace T3D
{
	enum COLLISION_TYPE
	{
		COLLISION_COLLISION,
		COLLISION_IGNORE,
	};

	class TMesh :public TObject
	{
	public:
		~TMesh();

		void setScale(const TVector3D& scale);
		void setPosition(const TVector3D& pos);
		TVector3D getPosition()const;
		void Move(const TVector3D& mov);
		//�������Ƿ���Mesh�İ�Χ���ڣ�������򷵻�true�����򷵻�false
		TBool Intersect(const TVector3D& point);

		void setVisible(TBool visible) { mIsVisible = visible; }
		TBool isVisible()const { return mIsVisible; }

		//�Ƿ���Ҫִ���޳�����
		void setCullFlag(TBool needCull) { mObject->needCull = needCull; }

		//��y����ת
		void Yaw(TFloat yDegree);
		//��z����ת
		void Roll(TFloat rDegree);
		//��x����ת
		void Pitch(TFloat pDegree);

		void setCollisionType(COLLISION_TYPE type) { mCollisionType = type; }
		COLLISION_TYPE getCollisionType()const { return mCollisionType; }

		TMaterial* getMaterial(TInt index = 0);
		void setMaterial(TInt index, TString matName);
		
		TObject4D* getObject()const { return mObject; }
		TInt getPolygonNumber()const { return mPolygonNumber; }
		TInt getVertexNumber()const { return mVertexNumber; }
		TInt getSubmeshNumber()const { return mSubmeshNumber; }

		//��ȡ�������ת�������ԽǶȱ�ʾ
		TVector3D getRotateVec()const { return mRotate; }
		
		//�Դ�ģ�ͽ���clone�������ٴζ��̶�ȡģ��
		TMesh* Clone();

	protected:
		friend class TSceneManager;
		TMesh();
		TMesh(const TString& name, const TString& meshName);

		TObject4D* mObject;

		TInt mPolygonNumber;//ģ�Ͷ���θ���
		TInt mVertexNumber;//ģ�Ͷ�����

		TInt mSubmeshNumber;//ģ����ģ�͸���

		TVector3D mRotate;//ģ�͸������ת����
		
		TBool mIsVisible;

		COLLISION_TYPE mCollisionType;
	};
}