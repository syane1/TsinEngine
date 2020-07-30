#pragma once

#include "TCommon.h"
#include "TMesh.h"
#include "TLight.h"
#include "TCore.h"
#include "TCamera.h"

namespace T3D
{
	//各种资源管理类
	class TSceneManager
	{
		typedef std::map<TString, TMesh*>::iterator MeshIter;

	public:
		TSceneManager();
		~TSceneManager();
		
		//绘制文字
		void drawString(const TString& str, TInt x, TInt y, const TColor& c = TColor(255, 255, 255));

		//创建一个模型实体
		TMesh* CreateMesh(const TString& name, const TString& meshName);
		//获取指定的模型实体
		TMesh* getMesh(const TString& name);
		//销毁指定的模型实体
		void DestroyMesh(const TString& name);

		void ClearMesh();

		//指定开启/关闭灯光(全局)
		void EnableLight(TBool enable);
		//当前是否开启灯光(全局)
		TBool IsLightEnable()const { return mLightEnable; }
		//创建一盏灯光
		TLight* CreateLight(LIGHT_TYPE lightType);
		//获得一盏灯光，如果灯光不存在，则返回NULL
		TLight* getLight(TInt id);

		//获取摄像机指针
		TCamera* getCamera() { return mCamera; }

		//获取所有(可见/不可见)物体个数
		TInt getTotalObjectNumber()const { return mMeshs.size(); }
		//获取当前可见物体个数
		TInt getVisibleObjectNumber()const { return mVisibleObjectNumber; }
		//获取当前可见多边形个数
		TInt getVisiblePolygonNumber()const { return mVisiblePolygonNumber; }
		//获取所有(可见/不可见)多边形个数
		TInt getTotalPolygonNumber()const { return mTotalPolygonNumber; }

		//创建地形
		TMesh* CreateTerrain(const TString& rawFileName, const TString& heightMapFileName, TFloat uTitle = 1, TFloat vTitle = 1, TFloat blockSize = 5.0f);
		TMesh* getTerrain() { return mTerrainMeshs; }

		//更新
		void Update();

	protected:
		std::map<TString, TMesh*>mMeshs;
		std::map<TString, TMesh*>mUsedMeshs;
		TRenderList4D*			mRenderList;

		TMesh*					mTerrainMeshs;

		TCamera*				mCamera;
		TBool					mLightEnable;

		TInt					mVisibleObjectNumber;
		TInt					mVisiblePolygonNumber;
		TInt					mTotalPolygonNumber;
	};
}