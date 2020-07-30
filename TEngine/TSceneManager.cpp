#include "TSceneManager.h"
#include "TGraphics.h"
#include "TMaterial.h"
#include "TUtil.h"

namespace T3D
{
	TSceneManager::TSceneManager() :mLightEnable(true), mVisibleObjectNumber(0), mVisiblePolygonNumber(0), mTotalPolygonNumber(0), mTerrainMeshs(NULL)
	{
		mCamera = new TCamera();
		mRenderList = new TRenderList4D;
	}

	TSceneManager::~TSceneManager()
	{
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
			SafeDelete(iter->second);
		for (MeshIter iter = mUsedMeshs.begin(); iter != mMeshs.end(); ++iter)
			SafeDelete(iter->second);

		mMeshs.clear();
		DestroyAllLights();
		SafeDelete(mCamera);
		SafeDelete(mRenderList);
	}

	void TSceneManager::drawString(const TString& str, TInt x, TInt y, const TColor& c)
	{
		TGraphics::drawString(str, x, y, c);
	}

	//创建一个模型实体
	TMesh* TSceneManager::CreateMesh(const TString& name, const TString& meshName)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter != mMeshs.end())
		{
			TMesh* mesh = NULL;

			MeshIter meshItr = mUsedMeshs.find(meshName);
			if (meshItr != mUsedMeshs.end())
			{
				mesh = meshItr->second->Clone();
				mesh->mName = name;
			}
			else
			{
				mesh = new TMesh(name, meshName);
				mUsedMeshs.insert(std::make_pair(meshName, mesh->Clone()));
			}
			mMeshs.insert(std::make_pair(name, mesh));
			return mesh;
		}
		return iter->second;
	}

	void TSceneManager::ClearMesh()
	{
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
			SafeDelete(iter->second);
		mMeshs.clear();
	}

	//获取指定的模型实体
	TMesh* TSceneManager::getMesh(const TString& name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter == mMeshs.end())
		{
			return iter->second;
		}
		return NULL;
	}

	//销毁指定的模型实体
	void TSceneManager::DestroyMesh(const TString& name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter != mMeshs.end())
			SafeDelete(iter->second);
		mMeshs.erase(name);
	}

	//指定开启/关闭灯光(全局)
	void TSceneManager::EnableLight(TBool enable)
	{
		mLightEnable = enable;
		for (int i = 0; i < GetLightSize(); i++)
			getLight(i)->lightOn = enable;
	}

	//创建一盏灯光
	TLight* TSceneManager::CreateLight(LIGHT_TYPE lightType)
	{
		TInt light = T3D::CreateLight(lightType);
		return getLight(light);
	}

	//获得一盏灯光，如果灯光不存在，则返回NULL
	TLight* TSceneManager::getLight(TInt id)
	{
		return T3D::GetLight(id);
	}

	//更新
	void TSceneManager::Update()
	{
		//camera->Update();

		mVisibleObjectNumber = 0;
		mTotalPolygonNumber = 0;

		//更新摄像机
		mCamera->Update();

		//将所有模型添加到渲染列表
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
		{
			mTotalPolygonNumber += iter->second->getPolygonNumber();
			//只将视野内的物体插入到渲染列表
			if (iter->second->isVisible() && !Cull_Object4D(iter->second->getObject(), mCamera->getFrustum(), CULL_BOX))
			{
				mVisibleObjectNumber++;
				Insert_Object4D_To_RenderList4D(mRenderList, iter->second->getObject(), TRANSFORM_TRANS, true);
			}
		}

		//渲染列表进行3D流水线处理
		//局部坐标系变换到世界坐标系
		Local_To_World_RenderList4D(mRenderList, TVector4D::ZERO, TRANSFORM_TRANS);
		//世界坐标系直接变换到屏幕坐标系
		//世界坐标系->背面消除->摄像机坐标系变换->
		//裁剪->光照计算->透视变换->屏幕变换
		World_To_Screen_RenderList4D(mRenderList, mCamera->getFrustum());

		//当前可见多边形个数
		mVisiblePolygonNumber = mRenderList->polyData.size();

		//执行渲染
		if (mCamera->getRenderMode() == RENDER_WIRE)
			Draw_RenderList4D_Wire(mRenderList);
		else
			Draw_RenderList4D_Texture_Solid(mRenderList);
	}

	TMesh* TSceneManager::CreateTerrain(const TString& heightMapFileName, const TString& textureFileName, TFloat uTitle, TFloat vTitle, TFloat blockSize)
	{
		if (mTerrainMeshs)
			SafeDelete(mTerrainMeshs);

		Log("Loading Terrain HeightMap : #%s Texture : ...", heightMapFileName.c_str(), textureFileName.c_str());

		TBitmap* heightMap = new TBitmap(heightMapFileName);

		if (!heightMap->isValid() ||
			heightMap->getWidth() % 2 == 0 ||
			heightMap->getHeight() % 2 == 0
			)
		{
			SafeDelete(heightMap);
			return NULL;
		}

		TMaterial* mat = new TMaterial();
		mat->bitmap = new TBitmap(textureFileName);
		SetMaterial("Terrain", mat);

		TInt row = heightMap->getHeight();
		TInt col = heightMap->getWidth();

		//Y = 0.299R+0.587G+0.114B
		mTerrainMeshs = new TMesh();
		TObject4D* obj = new TObject4D();
		mTerrainMeshs->mObject = obj;

		obj->name = "Terrain";
		obj->materialName = "Terrain";
		obj->state = TOBJECT_STATE_ACTIVE | TOBJECT_STATE_VISIBLE;
		obj->scale = TVector4D(1, 1, 1);
		obj->worldPosition = TVector4D::ZERO;
		obj->vertexNumber = row * col;
		obj->polygonNumber = (row - 1)*(col - 1) * 2;

		mTerrainMeshs->mVertexNumber = obj->vertexNumber;
		mTerrainMeshs->mPolygonNumber = obj->polygonNumber;

		//顶点列表
		obj->localList.reserve(obj->vertexNumber);
		obj->transformList.reserve(obj->vertexNumber);
		//多边形列表
		obj->polygonList.reserve(obj->polygonNumber);

		TFloat offsetX = (col - 1)*blockSize / 2;
		TFloat offsetZ = (row - 1)*blockSize / 2;

		TFloat minY = 0, maxY = 0;
		TFloat u = uTitle / (col - 1);
		TFloat v = vTitle / (row - 1);
		for (TInt r = 0; r < row; r++)
		{
			for (TInt c = 0; c < col; c++)
			{
				TVertex4D vex;
				//顶点坐标
				TColor color = heightMap->getPixel(c, r);
				vex.x = c * blockSize - offsetX;
				vex.y = 0.1f*(0.299f*color.r + 0.587f*color.g + 0.114f*color.b);
				vex.z = r * blockSize - offsetZ;

				if (vex.y < minY)
					minY = vex.y;

				if (vex.y > maxY)
					maxY = vex.y;

				//顶点UV
				vex.u = c * u;
				vex.v = r * v;

				obj->localList.push_back(vex);
				obj->transformList.push_back(vex);

				//计算顶点索引
				if (r < row - 1 && c < col - 1)
				{
					TPolygon4D poly;
					poly.state = TPOLY_STATE_ACTIVE;
					poly.attribute = TPOLY_ATTR_VERTEX_POSITION | TPOLY_ATTR_VERTEX_UV;
					poly.verList = &obj->transformList;

					poly.verIndex[0] = r * col + c;
					poly.verIndex[1] = (r + 1)*col + c;
					poly.verIndex[2] = r * col + c + 1;
					obj->polygonList.push_back(poly);

					poly.verIndex[0] = r * col + c+1;
					poly.verIndex[1] = (r + 1)*col + c;
					poly.verIndex[2] = (r + 1) * col + c + 1;
					obj->polygonList.push_back(poly);
				}
			}
		}

		obj->maxBoundingBox = TVector4D(offsetX, maxY, offsetZ);
		obj->minBoundingBox = TVector4D(-offsetX, minY, -offsetZ);
		obj->maxRadius = std::sqrt(offsetX*offsetX + offsetZ * offsetZ);

		SafeDelete(heightMap);
		mMeshs.insert(std::make_pair("Terrain", mTerrainMeshs));
		//设置不执行剔除操作
		mTerrainMeshs->setCullFlag(false);

		Log("Terrain Load Successed!");
		return mTerrainMeshs;
	}

}