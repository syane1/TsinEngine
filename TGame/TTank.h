#pragma once

#include "TObject.h"
#include "TSceneManager.h"
#include <list>

using namespace T3D;
namespace TYas3D
{
	//子弹类型
	enum BulletType
	{
		BULLET_BALL,			//球形子弹
		BULLET_ROCKET,			//导弹型子弹
	};
	//坦克类型
	enum TankType
	{
		TANK_AI,
		TANK_PLAYER,
	};
	//Tank的基类，Tank分为两部分，一部分是脑袋，一部分吗是车身，脑袋可以旋转
	//脑袋是基于车身移动的
	class TBullet;
	class TCamera;
	class TGameManager;
	class TTank :public TObject
	{
		typedef std::list<TBullet*>::iterator BulletItr;
	public:
		virtual ~TTank();

		//移动Tank
		void Move(const TVector3D& mov);
		//向前移动Tank，指按照当前Tank的朝向为前
		TBool MoveUp();
		//向后移动Tank,指按照当前Tank的朝向的反方向
		TBool MoveDown();
		//向左转Tank车体
		void TurnLeft();
		//向右转Tank车体
		void TurnRight();

		//炮台左转
		void FireAimLeft();
		//炮台右转
		void FireAimRight();

		//获得当前Tank的世界坐标
		TVector3D getPosition()const { return mBody->getPosition(); }
		void setPosition(const TVector3D& pos);

		//Tank沿y轴旋转，参数为角度
		void Yaw(TFloat degree);

		//设置子弹类型BULLET_BALL,BULLET_ROCKET
		void setBulletType(BulletType type) { mBulletType = type; }
		BulletType getBulletType()const { return mBulletType; }
		TankType getTankType() { return mTankType; }

		//是否可见
		TBool IsVisible()const { return mBody->isVisible(); }
		void setVisible(TBool visible);

		//给定的点是否在Tank的包围盒里
		TBool Intersect(const TVector3D& point);

		//被击中后自动调用
		virtual void OnHited();
		//开火
		virtual void Fire();
		//更新Tank
		virtual void Update();
		//获取摄像机应该移动的向量
		TVector3D getCameraStep()const { return mMoveVec; }

	protected:
		friend class TGameManager;
		friend class TPlayerController;
		TTank(const TString& name, const TString& meshName, TGameManager* gameMgr);

		//更新瞄准器朝向，子弹发射方向
		void UpdateFireDir(TFloat degree);
	protected:
		TMesh* mHead;
		TMesh* mBody;

		TVector3D mMoveVec;//摄像机应该移动的向量
		TVector3D mFireDir;//开火方向
		TFloat mSpeed;//移动速度
		TFloat mFireFrequency;//开火频率

		//TCamera* mCamera;

		TInt mLastFire;//开火时间记录
		TFloat mColOffset;//碰撞偏移量

		TSceneManager* mScene;
		TGameManager* mGameMgr;

		BulletType mBulletType;
		TankType mTankType;
	};

}