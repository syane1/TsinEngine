#pragma once

#include "TObject.h"
#include "TSceneManager.h"
#include <list>

using namespace T3D;
namespace TYas3D
{
	//�ӵ�����
	enum BulletType
	{
		BULLET_BALL,			//�����ӵ�
		BULLET_ROCKET,			//�������ӵ�
	};
	//̹������
	enum TankType
	{
		TANK_AI,
		TANK_PLAYER,
	};
	//Tank�Ļ��࣬Tank��Ϊ�����֣�һ�������Դ���һ�������ǳ����Դ�������ת
	//�Դ��ǻ��ڳ����ƶ���
	class TBullet;
	class TCamera;
	class TGameManager;
	class TTank :public TObject
	{
		typedef std::list<TBullet*>::iterator BulletItr;
	public:
		virtual ~TTank();

		//�ƶ�Tank
		void Move(const TVector3D& mov);
		//��ǰ�ƶ�Tank��ָ���յ�ǰTank�ĳ���Ϊǰ
		TBool MoveUp();
		//����ƶ�Tank,ָ���յ�ǰTank�ĳ���ķ�����
		TBool MoveDown();
		//����תTank����
		void TurnLeft();
		//����תTank����
		void TurnRight();

		//��̨��ת
		void FireAimLeft();
		//��̨��ת
		void FireAimRight();

		//��õ�ǰTank����������
		TVector3D getPosition()const { return mBody->getPosition(); }
		void setPosition(const TVector3D& pos);

		//Tank��y����ת������Ϊ�Ƕ�
		void Yaw(TFloat degree);

		//�����ӵ�����BULLET_BALL,BULLET_ROCKET
		void setBulletType(BulletType type) { mBulletType = type; }
		BulletType getBulletType()const { return mBulletType; }
		TankType getTankType() { return mTankType; }

		//�Ƿ�ɼ�
		TBool IsVisible()const { return mBody->isVisible(); }
		void setVisible(TBool visible);

		//�����ĵ��Ƿ���Tank�İ�Χ����
		TBool Intersect(const TVector3D& point);

		//�����к��Զ�����
		virtual void OnHited();
		//����
		virtual void Fire();
		//����Tank
		virtual void Update();
		//��ȡ�����Ӧ���ƶ�������
		TVector3D getCameraStep()const { return mMoveVec; }

	protected:
		friend class TGameManager;
		friend class TPlayerController;
		TTank(const TString& name, const TString& meshName, TGameManager* gameMgr);

		//������׼�������ӵ����䷽��
		void UpdateFireDir(TFloat degree);
	protected:
		TMesh* mHead;
		TMesh* mBody;

		TVector3D mMoveVec;//�����Ӧ���ƶ�������
		TVector3D mFireDir;//������
		TFloat mSpeed;//�ƶ��ٶ�
		TFloat mFireFrequency;//����Ƶ��

		//TCamera* mCamera;

		TInt mLastFire;//����ʱ���¼
		TFloat mColOffset;//��ײƫ����

		TSceneManager* mScene;
		TGameManager* mGameMgr;

		BulletType mBulletType;
		TankType mTankType;
	};

}