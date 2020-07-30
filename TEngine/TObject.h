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
		//当前物体是否还活着
		virtual TBool isAlive()const { return mCurrentLive < mMaxLive; }
	protected:
		TObject() :mMaxLive(1), mCurrentLive(0) { }
		TString mName;
		TInt mMaxLive;			//最大生命时间
		TInt mCurrentLive;		//当前已经经过时间
	};
}