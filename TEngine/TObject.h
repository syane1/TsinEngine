#pragma once
#include "TCommon.h"

namespace T3D
{
	class TObject
	{
	public:
		virtual ~TObject() {}
		virtual TString getName()const { return mName; }
		virtual void Update() {};
		//��ǰ�����Ƿ񻹻���
		virtual TBool isAlive()const { return mCurrentLive < mMaxLive; }
	protected:
		TObject() :mMaxLive(1), mCurrentLive(0) { }
		TString mName;
		TInt mMaxLive;			//�������ʱ��
		TInt mCurrentLive;		//��ǰ�Ѿ�����ʱ��
	};
}