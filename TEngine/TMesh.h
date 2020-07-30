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
		//给顶点是否在Mesh的包围盒内，如果是则返回true，否则返回false
		TBool Intersect(const TVector3D& point);

		void setVisible(TBool visible) { mIsVisible = visible; }
		TBool isVisible()const { return mIsVisible; }

		//是否需要执行剔除操作
		void setCullFlag(TBool needCull) { mObject->needCull = needCull; }

		//绕y轴旋转
		void Yaw(TFloat yDegree);
		//绕z轴旋转
		void Roll(TFloat rDegree);
		//绕x轴旋转
		void Pitch(TFloat pDegree);

		void setCollisionType(COLLISION_TYPE type) { mCollisionType = type; }
		COLLISION_TYPE getCollisionType()const { return mCollisionType; }

		TMaterial* getMaterial(TInt index = 0);
		void setMaterial(TInt index, TString matName);
		
		TObject4D* getObject()const { return mObject; }
		TInt getPolygonNumber()const { return mPolygonNumber; }
		TInt getVertexNumber()const { return mVertexNumber; }
		TInt getSubmeshNumber()const { return mSubmeshNumber; }

		//获取各轴的旋转分量，以角度表示
		TVector3D getRotateVec()const { return mRotate; }
		
		//对此模型进行clone，避免再次度盘读取模型
		TMesh* Clone();

	protected:
		friend class TSceneManager;
		TMesh();
		TMesh(const TString& name, const TString& meshName);

		TObject4D* mObject;

		TInt mPolygonNumber;//模型多边形个数
		TInt mVertexNumber;//模型顶点数

		TInt mSubmeshNumber;//模型子模型个数

		TVector3D mRotate;//模型各轴的旋转分量
		
		TBool mIsVisible;

		COLLISION_TYPE mCollisionType;
	};
}