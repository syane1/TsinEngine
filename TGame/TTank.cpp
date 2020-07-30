#include "TTank.h"
#include "TMatrix.h"
#include "TBullet.h"
#include "TGameManager.h"

namespace TYas3D
{
	TTank::TTank(const TString& name, const TString& meshName, TGameManager* gameMgr)
		:mGameMgr(gameMgr), mScene(gameMgr->getSceneManager()), mSpeed(0.75f), mFireDir(0, 0, -1), mLastFire(0), mBulletType(BULLET_ROCKET), mMoveVec(0, 0, 0), mColOffset(3.5f), mTankType(TANK_PLAYER)
	{
		mMaxLive = 100;
		mCurrentLive = 0;
		mFireFrequency = 8;

		mName = name;
		mBody = mScene->CreateMesh(name + "_Body", meshName + "_Body.mesh");
		mHead = mScene->CreateMesh(name + "_Head", meshName + "_Head.mesh");
	}

	TTank::~TTank()
	{
		mScene->DestroyMesh(mName + "_Body");
		mScene->DestroyMesh(mName + "_Head");
	}

	void TTank::Fire()
	{
		if (mGameMgr&&mLastFire > mFireFrequency)
		{
			TBullet* bullet = mGameMgr->CrtBullet(this);
			bullet->setPosition(getPosition() + mFireDir * 5.0f + TVector3D(0, 2, 0));
			bullet->setDirection(mFireDir);
			bullet->Yaw(mHead->getRotateVec().y);
			mLastFire = 0;

			mGameMgr->playSound(SOUND_FIRE);
		}
	}

	void TTank::Update()
	{
		mLastFire++;
	}

	void TTank::setVisible(TBool visible)
	{
		mBody->setVisible(visible);
		mHead->setVisible(visible);
	}

	void TTank::Move(const TVector3D& mov)
	{
		mBody->Move(mov);
		mHead->Move(mov);
	}

	TBool TTank::MoveUp()
	{
		//�����߼�����ǰ�ƶ�
		//�Ե�ǰTank�ĳ���Ϊǰ
		TInt degree = mBody->getRotateVec().y;
		TInt dir = degree > 0 ? 1 : -1;
		degree = Abs(degree) % 360;

		const static TInt offset = 3;
		TFloat xOff = 0, zOff = 0;					//Tank�����������ƫ����

		TVector3D pos = getPosition();				//���յķ���
		if (degree < 5)
		{
			mMoveVec = TVector3D(0, 0, -mSpeed);
			pos.z = pos.z - mColOffset;
			xOff = offset;
		}
		else if (degree < 95)
		{
			mMoveVec = TVector3D(-mSpeed*dir, 0, 0);
			pos.x = pos.x + mColOffset*dir;
			zOff = offset;
		}
		else if (degree < 185)
		{
			mMoveVec = TVector3D(0, 0, mSpeed);
			pos.z = pos.z + mColOffset;
			xOff = offset;
		}
		else {
			mMoveVec = TVector3D(mSpeed*dir, 0, 0);
			pos.x = pos.x + mColOffset * dir;
			zOff = offset;
		}
		//���ȼ���Ƿ����ϰ�����ֹ
		//��Tank�������Ҳ෢����������
		if (mGameMgr->canGo(pos.x + mMoveVec.x + xOff, pos.z + mMoveVec.z + zOff) &&
			mGameMgr->canGo(pos.x + mMoveVec.x - xOff, mMoveVec.z - zOff)
			)
		{
			Move(mMoveVec);
			return true;
		}
		else										//�У���ô̹�˺������ƫ������Ϊ0
		{
			mMoveVec = TVector3D::ZERO;
			return false;
		}
	}

	TBool TTank::MoveDown()
	{
		//�����߼�������ƶ�
		//�Ե�ǰTank�ĳ���ķ�����Ϊ��
		TInt degree = mBody->getRotateVec().y;
		TInt dir = degree > 0 ? 1 : -1;
		degree = Abs(degree) % 360;

		const static TInt offset = 3;
		TFloat xOff = 0, zOff = 0;					//Tank�����������ƫ����

		TVector3D pos = getPosition();
		if (degree < 5)
		{
			mMoveVec = TVector3D(0, 0, mSpeed);
			pos.z = pos.z + mColOffset;
			xOff = offset;
		}
		else if (degree < 95)
		{
			mMoveVec = TVector3D(mSpeed*dir, 0, 0);
			pos.x = pos.x + mColOffset * dir;
			zOff = offset;
		}
		else if (degree < 185)
		{
			mMoveVec = TVector3D(0, 0, -mSpeed);
			pos.z = pos.z - mColOffset;
			xOff = offset;
		}
		else
		{
			mMoveVec = TVector3D(-mSpeed * dir, 0, 0);
			pos.x = pos.x - mColOffset * dir;
			zOff = offset;
		}

		//���ȼ���Ƿ����ϰ�����ֹ
		if (mGameMgr->canGo(pos.x + mMoveVec.x + xOff, pos.z + mMoveVec.z + zOff) &&
			mGameMgr->canGo(pos.x + mMoveVec.x - xOff, pos.z + mMoveVec.z - zOff)
			)
		{
			Move(mMoveVec);
			return true;
		}
	}

	void TTank::OnHited()
	{
	}

	TBool TTank::Intersect(const TVector3D& point)
	{
		return mHead->Intersect(point) || mBody->Intersect(point);
	}

	void TTank::UpdateFireDir(TFloat degree)
	{
		TMatrix44 mat;
		GetRotateMatrix44Y(mat, degree);
		GetVector3DMulMatrix44(mFireDir, mat, mFireDir);
	}

	void TTank::FireAimLeft()
	{
		UpdateFireDir(-4.0f);
		mHead->Yaw(mHead->getRotateVec().y - 4.0f);
	}

	void TTank::FireAimRight()
	{
		UpdateFireDir(4.0f);
		mHead->Yaw(mHead->getRotateVec().y + 4.0f);
	}

	void TTank::setPosition(const TVector3D& pos)
	{
		mBody->setPosition(pos);
		mHead->setPosition(pos);
	}

	void TTank::Yaw(TFloat degree)
	{
		mHead->Yaw(degree);
		mBody->Yaw(degree);
	}

	void TTank::TurnLeft()
	{
		mBody->Yaw(mBody->getRotateVec().y - 90.0f);
	}

	void TTank::TurnRight()
	{
		mBody->Yaw(mBody->getRotateVec().y + 90.0f);
	}

}