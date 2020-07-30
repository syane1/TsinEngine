#pragma once

#include "TCommon.h"
#include "TMesh.h"
#include "TLight.h"
#include "TCore.h"
#include "TCamera.h"

namespace T3D
{
	//������Դ������
	class TSceneManager
	{
		typedef std::map<TString, TMesh*>::iterator MeshIter;

	public:
		TSceneManager();
		~TSceneManager();
		
		//��������
		void drawString(const TString& str, TInt x, TInt y, const TColor& c = TColor(255, 255, 255));

		//����һ��ģ��ʵ��
		TMesh* CreateMesh(const TString& name, const TString& meshName);
		//��ȡָ����ģ��ʵ��
		TMesh* getMesh(const TString& name);
		//����ָ����ģ��ʵ��
		void DestroyMesh(const TString& name);

		void ClearMesh();

		//ָ������/�رյƹ�(ȫ��)
		void EnableLight(TBool enable);
		//��ǰ�Ƿ����ƹ�(ȫ��)
		TBool IsLightEnable()const { return mLightEnable; }
		//����һյ�ƹ�
		TLight* CreateLight(LIGHT_TYPE lightType);
		//���һյ�ƹ⣬����ƹⲻ���ڣ��򷵻�NULL
		TLight* getLight(TInt id);

		//��ȡ�����ָ��
		TCamera* getCamera() { return mCamera; }

		//��ȡ����(�ɼ�/���ɼ�)�������
		TInt getTotalObjectNumber()const { return mMeshs.size(); }
		//��ȡ��ǰ�ɼ��������
		TInt getVisibleObjectNumber()const { return mVisibleObjectNumber; }
		//��ȡ��ǰ�ɼ�����θ���
		TInt getVisiblePolygonNumber()const { return mVisiblePolygonNumber; }
		//��ȡ����(�ɼ�/���ɼ�)����θ���
		TInt getTotalPolygonNumber()const { return mTotalPolygonNumber; }

		//��������
		TMesh* CreateTerrain(const TString& rawFileName, const TString& heightMapFileName, TFloat uTitle = 1, TFloat vTitle = 1, TFloat blockSize = 5.0f);
		TMesh* getTerrain() { return mTerrainMeshs; }

		//����
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