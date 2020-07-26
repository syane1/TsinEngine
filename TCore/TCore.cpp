#include "TCore.h"
#include "TFrustum.h"
#include "TGraphics.h"
#include "TLight.h"
#include "TMaterial.h"

namespace T3D
{
	TVertex4D::TVertex4D() :x(0.0f), y(0.0f), z(0.0f), w(1.0f), u(0.0f), v(0.0f) {}
	
	TVertex4D::TVertex4D(const TVector4D& v) : x(v.x), y(v.y), z(v.z), w(v.w), u(0.0f), v(0.0f) {}
	
	TVertex4D::TVertex4D(TFloat xf, TFloat yf, TFloat zf) : x(xf), y(yf), z(zf), w(1), u(0.0f), v(0.0f) {}

	TVertex4D TVertex4D::operator*(const TVector4D& right)
	{
		TVertex4D temp = *this;
		temp.x *= right.x, temp.y *= right.y, temp.z *= right.z;
		return temp;
	}

	TVertex4D TVertex4D::operator-(const TVertex4D& right)
	{
		TVertex4D temp = *this;
		temp.x += right.x, temp.y += right.y, temp.z += right.z;
		return temp;
	}

	TVertex4D TVertex4D::operator+(const TVertex4D& right)
	{
		TVertex4D temp = *this;
		temp.x += right.x, temp.y += temp.y, temp.z += right.z;
		return temp;
	}

	TVertex4D TVertex4D::operator/(TFloat factor)
	{
		TVertex4D temp = *this;
		temp.x /= factor, temp.y /= factor, temp.z /= factor;
		return temp;
	}

	TVertex4D& TVertex4D::operator=(const TVertex4D&right)
	{
		x = right.x, y = right.y, z = right.z, w = right.w;
		u = right.u, v = right.v;
		return *this;
	}

	TVector4D TVertex4D::toVector4D()const
	{
		return TVector4D(x, y, z, w);
	}

	TPlane3D::TPlane3D(const TVector4D& p, const TVector4D& nor) :point(p)
	{
		normal = nor;
		normal.Normalize();
	}

	TPlane3D& TPlane3D::operator=(const TPlane3D& right)
	{
		point = right.point;
		normal = right.normal;
		return *this;
	}

	TPolygon4D::TPolygon4D() :state(0), attribute(0), color(0xffffff), verList(NULL) {}

	TPolygonF4D::TPolygonF4D() : state(0), attribute(0), color(0xffffff)/*,pre(NULL),next(NULL)*/ {}

	TObject4D::TObject4D() : nextObject(NULL), materialName(DEFAULT_NAME), scale(1, 1, 1), direction(TVector4D::UNIT_X), needCull(true) {}

	void Transform_Object4D(TObject4D* object, const TMatrix44& mat, OBJ_TRANSFORM_TYPE transformType/*,TBool transformBasis*/)
	{
		TObject4D* obj = object; 
		switch (transformType)
		{
		case TRANSFORM_LOCAL:
			{
				while (obj)
				{
					//只对局部坐标进行变换
					for (TInt i = 0; i < obj->vertexNumber; i++)
					{
						GetVertex4DMulMatrix44(obj->localList[i], mat, obj->localList[i]);
					}
					obj = obj->nextObject;
				}
			}
			break;
		case TRANSFORM_TRANS:
		{
			while (obj)
			{
				//只对局部坐标进行变换
				for (TInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DMulMatrix44(obj->transformList[i], mat, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
		break;
		case TRANSFORM_LOCAL_TO_TRANS:
			{
				while (obj)
				{
					//将局部坐标变换后存储至变换后的坐标顶点列表
					for (TInt i = 0; i < obj->vertexNumber; i++)
					{
						GetVertex4DMulMatrix44(obj->localList[i], mat, obj->transformList[i]);
					}
					obj = obj->nextObject;
				}
			}
			break;
		}
	}

	void Transform_RenderList4D(TRenderList4D* renderList,
		const TMatrix44& mat, OBJ_TRANSFORM_TYPE transformType)
	{
		TPolygonF4D* poly = NULL;
		switch (transformType)
		{
		case T3D::TRANSFORM_LOCAL:
			{
				for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);
					GetVertex4DMulMatrix44(poly->localList[0], mat, poly->localList[0]);
					GetVertex4DMulMatrix44(poly->localList[1], mat, poly->localList[1]);
					GetVertex4DMulMatrix44(poly->localList[2], mat, poly->localList[2]);
				}
			}
			break;
		case T3D::TRANSFORM_TRANS:
			{
				for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);
					GetVertex4DMulMatrix44(poly->transformList[0], mat, poly->transformList[0]);
					GetVertex4DMulMatrix44(poly->transformList[1], mat, poly->transformList[1]);
					GetVertex4DMulMatrix44(poly->transformList[2], mat, poly->transformList[2]);
				}
			}
			break;
		case T3D::TRANSFORM_LOCAL_TO_TRANS:
			{
				for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);
					GetVertex4DMulMatrix44(poly->localList[0], mat, poly->transformList[0]);
					GetVertex4DMulMatrix44(poly->localList[1], mat, poly->transformList[1]);
					GetVertex4DMulMatrix44(poly->localList[2], mat, poly->transformList[2]);
				}
			}
			break;


		}
	}

	void Insert_Object4D_To_RenderList4D(TRenderList4D* renderList, TObject4D* object, OBJ_TRANSFORM_TYPE transformType, bool transformScaleAndPosition)
	{
		if (!object)
			return;
		
		TVector4D scale(1, 1, 1);
		TVector4D pos(0, 0, 0);

		if (transformScaleAndPosition)
		{
			scale = object->scale;
			pos = object->worldPosition;
		}

		//这里备份灯光的数据
		TLight* light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
		{
			light = *itr;
			light->transPosition = light->position;
			light->transDirection = light->direction;
		}

		TObject4D* obj = object;
		TPolygon4D* base;
		TPolygonF4D poly;
		switch (transformType)
		{
		case T3D::TRANSFORM_LOCAL:
			while (obj)
			{
				for (TInt i = 0; i < obj->polygonNumber; i++)
				{
					base = &obj->polygonList[i];
					poly.attribute = base->attribute;
					poly.state = TPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.localList[0] = obj->localList[base->verIndex[0]];
					poly.localList[1] = obj->localList[base->verIndex[1]];
					poly.localList[2] = obj->localList[base->verIndex[2]];

					//设置材质
					poly.material = GetMaterial(obj->materialName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		case T3D::TRANSFORM_TRANS:
			while (obj)
			{
				for (TInt i = 0; i < obj->polygonNumber; i++)
				{
					base = &obj->polygonList[i];
					poly.attribute = base->attribute;
					poly.state = TPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.transformList[0] = obj->localList[base->verIndex[0]] * scale + pos;
					poly.transformList[1] = obj->localList[base->verIndex[1]] * scale + pos;
					poly.transformList[2] = obj->localList[base->verIndex[2]] * scale + pos;

					//设置材质
					poly.material = GetMaterial(obj->materialName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		case T3D::TRANSFORM_LOCAL_TO_TRANS:
			while (obj)
			{
				for (TInt i = 0; i < obj->polygonNumber; i++)
				{
					base = &obj->polygonList[i];
					poly.attribute = base->attribute;
					poly.state = TPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.transformList[0] = obj->transformList[base->verIndex[0]] * scale + pos;
					poly.transformList[1] = obj->transformList[base->verIndex[1]] * scale + pos;
					poly.transformList[2] = obj->transformList[base->verIndex[2]] * scale + pos;

					//设置材质
					poly.material = GetMaterial(obj->materialName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		}
	}

	void Local_To_World_Object(TObject4D* object, OBJ_TRANSFORM_TYPE transformType)
	{
		if (!object)
			return;

		TObject4D* obj = object;
		TVector4D pos = obj->worldPosition;
		if (TRANSFORM_LOCAL_TO_TRANS == transformType)
		{
			while (obj)
			{
				for (TInt i = 0; i < obj->vertexNumber; i++)
				{
					//设置顶点的缩放值，不再保留坐标设置
					GetVertex4DAddVector4D(obj->localList[i] * obj->scale, pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
		else if (TRANSFORM_TRANS == transformType)
		{
			while (obj)
			{
				for (TInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DAddVector4D(obj->transformList[i], pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
	}

	void Local_To_World_RenderList4D(TRenderList4D* renderList, const TVector4D& pos, OBJ_TRANSFORM_TYPE transformType)
	{
		TPolygonF4D* poly = NULL;
		switch (transformType)
		{
		case T3D::TRANSFORM_LOCAL:
			{
				for(TRenderList4D::Itr itr = renderList->polyData.begin();itr!=renderList->polyData.end();++itr)
				{
					poly = &(*itr);

					GetVertex4DAddVector4D(poly->localList[0], pos, poly->localList[0]);
					GetVertex4DAddVector4D(poly->localList[1], pos, poly->localList[1]);
					GetVertex4DAddVector4D(poly->localList[2], pos, poly->localList[2]);
				}
			}
			break;
		case T3D::TRANSFORM_TRANS:
			{
				for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);

					GetVertex4DAddVector4D(poly->localList[0], pos, poly->transformList[0]);
					GetVertex4DAddVector4D(poly->localList[1], pos, poly->transformList[1]);
					GetVertex4DAddVector4D(poly->localList[2], pos, poly->transformList[2]);
				}
			}
			break;
		case T3D::TRANSFORM_LOCAL_TO_TRANS:
			{
				for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);

					GetVertex4DAddVector4D(poly->transformList[0], pos, poly->transformList[0]);
					GetVertex4DAddVector4D(poly->transformList[1], pos, poly->transformList[1]);
					GetVertex4DAddVector4D(poly->transformList[2], pos, poly->transformList[2]);
				}
			}
			break;
		}
	}

	bool Cull_Object4D(TObject4D* object, TFrustum* camera, CULL_TYPE cullType)
	{
		if (!object)
			return true;

		if (!object->needCull)
			return false;

		//先把当前物体的坐标变换成相机坐标
		TVector4D centerPos;
		GetVector4DMulMatrix44(object->worldPosition, camera->mWorldToCamera, centerPos);

		//首先把半径进行缩放
		TFloat scaleRadiusX, scaleRadiusY,scaleRadiusZ;

		if (cullType == CULL_SPHERE)
		{
			scaleRadiusX = object->maxRadius*object->scale.x;
			scaleRadiusY = object->maxRadius*object->scale.y;
			scaleRadiusZ = object->maxRadius*object->scale.z;
		}
		else if (cullType == CULL_BOX)
		{
			//各剔除偏移以模型做大顶点的各值为基准
			scaleRadiusX = object->maxBoundingBox.x*object->scale.x;
			scaleRadiusY = object->maxBoundingBox.y*object->scale.y;
			scaleRadiusZ = object->maxBoundingBox.z*object->scale.z;
		}
		else {
			return true;
		}

		//远近面剔除
		if ((centerPos.z + scaleRadiusZ) < camera->clip_z_near ||
			(centerPos.z - scaleRadiusZ) > camera->clip_z_far)
			return true;

		//左右面剔除
		//这里的测试是基于在xz平面的投影来计算的
		TFloat	x_test = 0.5f* camera->viewplane_width*centerPos.z / camera->view_dist;
		if ((centerPos.x - scaleRadiusX) > x_test ||
			(centerPos.x + scaleRadiusX) < -x_test)\
			return true;

		//上下面剔除
		//这里的测试是基于在zy平面的投影来计算的
		TFloat y_test = 0.5f*camera->viewplane_height*centerPos.z / camera->view_dist;
		if ((centerPos.y - scaleRadiusY) > y_test ||
			(centerPos.y - scaleRadiusY) < -y_test)
			return true;

		return false;
	}

	void World_To_Camera_Object4D(TObject4D* object, TFrustum* camera)
	{
		TObject4D* obj = object;
		while (obj)
		{
			for (TInt i = 0; i < obj->vertexNumber; ++i)
			{
				GetVertex4DMulMatrix44(obj->transformList[i], camera->mWorldToCamera, obj->transformList[i]);
			}
			obj = obj->nextObject;
		}
	}

	void World_To_Camera_RenderList4D(TRenderList4D*renderList, TFrustum*camera)
	{
		//这里对灯光位置进行变换，将其变换到相机坐标系下面
		TMatrix44 mat = camera->mWorldToCamera;
		//这里只保留旋转变换，而不进行平移变换，否则会出现灯光跟随摄像机移动的效果
		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0;

		TLight* light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
		{
			light = *itr;
			GetVector4DMulMatrix44(light->position, mat, light->transPosition);
			GetVector4DMulMatrix44(light->direction, mat, light->transDirection);
		}

		TPolygonF4D* poly = NULL;
		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); itr++)
		{
			poly = &(*itr);
			
			GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, poly->transformList[0]);
			GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, poly->transformList[1]);
			GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, poly->transformList[2]);
		}
	}

	void Sort_RenderList4D(TRenderList4D* renderList)
	{
		//这里对多边形列表进行深度排序（画家算法）
		renderList->polyData.sort(CompTPolygonF4D());
	}

	bool Remove_Backface(TPolygon4D*poly, TFrustum*camera)
	{
		TInt index0 = poly->verIndex[0];
		TInt index1 = poly->verIndex[1];
		TInt index2 = poly->verIndex[2];

		//这里使用变换后的多边形顶点列表，因为背面消除只能在顶点被变换为世界坐标之后才能进行
		//而在这里一般在相机坐标系内对其进行背面消除
		//顶点时按顺时针方向排列的u = p0->p1,v = p0->p2,n = uXv
		TVector4D u = (poly->verList->at(index0) - poly->verList->at(index1)).toVector4D();
		TVector4D v = (poly->verList->at(index0) - poly->verList->at(index2)).toVector4D();

		//多边形法线
		TVector4D n = u.CrossProduct(v);
		TVector4D view = camera->position - poly->verList->at(index0).toVector4D();

		//求得两向量的点积，如果小于0，那么认为这个面背朝摄像机，不予显示
		TFloat dot = n.DotProduct(view);
		if (dot < 0.0f)
		{
			poly->state = TPOLY_STATE_BACKFACE;
			return true;
		}

		return false;
	}

	bool Remove_Backface(TPolygonF4D* poly, TFrustum* camera)
	{
		if (!(poly->state&TPOLY_STATE_ACTIVE) ||
			(poly->state&TPOLY_STATE_BACKFACE) ||
			(poly->state&TPOLY_STATE_CLIPPED))
			return true;
		
		//这里使用变换后的多边形顶点列表，因为背面消除只能在顶点被变换为世界坐标之后才能进行
		//而在这里一般在相机坐标系内对其进行背面消除
		//顶点时按顺时针方向排列的u=p0->p1 v=p0->p2 n=uXv
		TVector4D u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		TVector4D v = (poly->transformList[0] - poly->transformList[2]).toVector4D();
		
		TVector4D n = u.CrossProduct(v);
		TVector4D view = camera->position - poly->transformList[0].toVector4D();

		TFloat dot = n.DotProduct(view);
		if (dot < 0)
		{
			poly->state |= TPOLY_STATE_BACKFACE;
			return true;
		}

		return false;
	}

	void Remove_Backface_Object4D(TObject4D* object, TFrustum* camera)
	{
		TObject4D* obj = object;
		TPolygon4D* poly = NULL;

		while (obj)
		{
			//物体没有被剔除
			if (!(obj->state&TOBJECT_CULLED))
			{
				for (TInt i = 0; i < obj->polygonNumber; i++)
				{
					poly = &obj->polygonList[i];
					Remove_Backface(poly, camera);
				}
			}
			obj = obj->nextObject;
		}
	}

	void Remove_Backface_RenderList4D(TRenderList4D* renderList, TFrustum* camera)
	{
		TPolygonF4D* poly = NULL;

		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end();)
		{
			poly = &(*itr);

			//这里移除不可见的多边形
			//这样在RenderList之后的操作中不需要判断当前多边形是否可见
			if (Remove_Backface(poly, camera))
			{
				renderList->polyData.erase(itr++);
			}
			else {
				++itr;
			}
		}
	}

	void Light_PolygonF4D(TPolygonF4D* poly, TFrustum* camera)
	{
		TBool lightOn = false;
		TColor baseColor(poly->color);
		
		//这里使用了TInt类型而没有使用TColor类型,原因是后面要进行颜色的累加
		//使用unsigned char可能会导致移除,从而导致显示不正常
		TInt sumColorR = 0, sumColorG = 0, sumColorB = 0;//全部颜色总和

		//顶点按照顺时针方向排序，因此u=p0->p1 v=p->p2 n=uXv
		TVector4D u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		TVector4D v = (poly->transformList[0] - poly->transformList[2]).toVector4D();
		TVector4D n = u.CrossProduct(v);

		//法线长度
		TFloat length = n.Length();

		TLight* light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
		{
			light = *itr;
			//灯光关闭则不处理
			if (!light->lightOn)
				continue;

			lightOn = true;
			//环境光
			if (light->lightType == LIGHT_AMBIENT)
			{
				//将每个分量与多边形颜色相乘，并处以256，确保结果在0~255之间
				sumColorR += light->ambient.r*baseColor.r >> 8;
				sumColorG += light->ambient.g*baseColor.g >> 8;
				sumColorB += light->ambient.b*baseColor.b >> 8;
			}
			//平行光
			else if (light->lightType == LIGHT_DIRECTION)
			{
				//无穷远灯光，因此需要知道面法线和光源方向
				//这里使用灯光方向的逆方向作为与面法线夹角的向量
				//这样当夹角小于90°,其点积大于零
				TFloat dot = n.DotProduct(-light->transDirection);


				//平行光光照模型
				//I(d)dir = IOdir*Cldir
				//散射光的计算方式
				//Itotal = Rsdiffuse*Idiffuse*(n*1)
				//将它们两个相乘即可
				if (dot > 0.0f)
				{
					TFloat dirLeng = light->transDirection.Length();

					//接受光照的强度，多边形法线与光照方向的夹角越小，那么其接收强度越大
					//夹角越大，接收强度越小，多边形越暗
					TFloat temp = light->power*dot / (length*dirLeng * 256);
					sumColorR += temp * light->diffuse.r*baseColor.r;
					sumColorG += temp * light->diffuse.g*baseColor.g;
					sumColorB += temp * light->diffuse.b*baseColor.b;
				}
				else
				{
					//这里当多边形是背朝光源时,也进行了一些处理，只是把它的颜色调的很暗，这样才比较协调
					sumColorR += baseColor.r*light->shadowFactor;
					sumColorG += baseColor.g*light->shadowFactor;
					sumColorB += baseColor.b*light->shadowFactor;
				}
			}
			else if (light->lightType == LIGHT_POINT)
			{
				//点光源的光照模型
				//			IOpoint*Clpoint
				//I(d)point=-------------
				//			kc+kl*d+kq*d*d
				//其中d=|p-s|即点光源到多边形的距离
				//多边形到点光源的方向向量
				TVector4D dir = light->transPosition - poly->transformList[0].toVector4D();
				TFloat dot = n.DotProduct(dir);
				if (dot > 0.0f)
				{
					//散射光的计算方式
					//Itotal = Rsdiffuse*Idiffuse*(n*1)
					//将它们两个相乘即可
					//点光源到多边形的距离
					TFloat dist = dir.Length();
					TFloat atten = light->kc + light->kl*dist + light->kq*dist*dist;
					TFloat temp = dot / (length*dist*atten * 256);

					sumColorR += light->diffuse.r*baseColor.r*temp;
					sumColorG += light->diffuse.g*baseColor.g*temp;
					sumColorB += light->diffuse.b*baseColor.b*temp;
				}
				else
				{
					//这里是多边形是背朝光源时,也进行了一些处理,只是把它的颜色调的很暗，这样才比较协调
					sumColorR += baseColor.r*light->shadowFactor;
					sumColorG += baseColor.g*light->shadowFactor;
					sumColorB += baseColor.b*light->shadowFactor;
				}
			}
		}
		if (lightOn)
		{
			//这里再次进行数值检查
			if (sumColorR > 255)sumColorR = 255;
			if (sumColorG > 255)sumColorG = 255;
			if (sumColorB > 255)sumColorB = 255;

			poly->color = TColor_16b(sumColorR, sumColorG, sumColorB);
		}
	}

	void Light_RenderList4D(TRenderList4D* renderList, TFrustum* camera)
	{
		TPolygonF4D* poly = NULL;

		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);
			Light_PolygonF4D(poly, camera);
		}
	}

	void Camera_To_Perspective_Object4D(TObject4D* object, TFrustum* camera)
	{
		if (!object)
			return;

		TObject4D* obj = object;
		TFloat d = camera->view_dist;
		TFloat temp = 0;

		while (obj)
		{
			for (TInt i = 0; i < obj->vertexNumber; i++)
			{
				temp = d / obj->transformList[i].z;

				//变换x、y轴,z轴不变换
				obj->transformList[i].x = temp * obj->transformList[i].x;
				obj->transformList[i].y = temp * obj->transformList[i].y;
			}
			obj = obj->nextObject;
		}
	}

	void Camera_To_Perspective_RenderList4D(TRenderList4D* renderList, TFrustum* camera)
	{
		TFloat d = camera->view_dist;
		TFloat temp = 0;
		TPolygonF4D* poly = NULL;
		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);

			for (TInt i = 0; i < 3; i++)
			{
				temp = d / poly->transformList[i].z;
				poly->transformList[i].x = temp * poly->transformList[i].x;
				poly->transformList[i].y = camera->aspect_ratio*temp*poly->transformList[i].y;
			}
		}
	}

	void Perspective_To_Screen_Object4D(TObject4D* object, TFrustum* camera)
	{
		if (!object)
			return;

		TFloat XOffset = 0.5f*(camera->viewport_width - 1);
		TFloat YOffset = 0.5f*(camera->viewport_height - 1);

		TObject4D* obj = object;
		while (obj)
		{
			for (TInt i = 0; i < obj->vertexNumber; i++)
			{
				//在这里,x、y已经被归一化为[-1,1]的范围之内
				obj->transformList[i].x = XOffset * (1 + obj->transformList[i].x);
				obj->transformList[i].y = XOffset * (1 - obj->transformList[i].y);
			}
			obj = obj->nextObject;
		}
	}

	void Perspective_To_Screen_RenderList4D(TRenderList4D* renderList, TFrustum* camera)
	{
		TFloat XOffset = 0.5f*(camera->viewport_width - 1);
		TFloat YOffset = 0.5f*(camera->viewport_height - 1);

		TPolygonF4D* poly = NULL;
		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);
			for (TInt i = 0; i < 3; i++)
			{
				poly->transformList[i].x = XOffset * (1 + poly->transformList[i].x);
				poly->transformList[i].y = YOffset * (1 + poly->transformList[i].y);
			}
		}
	}

	//判断此多边形是否在摄像机范围之内
	TBool isClipped(TPolygonF4D* poly, const TPlane3D& plane)
	{
		TInt flag = 0;
		for (TInt i = 0; i < 3; i++)
		{
			TFloat temp = plane.normal.DotProduct(poly->transformList[i].toVector4D() - plane.point);
			if (temp < 0)
				flag++;
		}

		if (flag == 3)
			return true;
		else
			return false;
	}

	TBool isClipped(TPolygonF4D* point, TFloat znear, TFloat zfar)
	{
		TInt num = 0;
		for (TInt i = 0; i < 3; i++)
		{
			if (point->transformList[i].z<znear || point->transformList[i].z>zfar)
				num++;
		}

		if (num == 3)
			return true;
		return false;
	}

	//判断此多边形是否在摄像机范围之内
	TBool isClipped(TPolygonF4D* poly, TFrustum* camera)
	{
		TVertex4D v0, v1, v2;
		GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, v0);
		GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, v1);
		GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, v2);

		if (camera->clip_z_near > v0.z&&camera->clip_z_near > v1.z&&camera->clip_z_near > v2.z ||				//近裁剪面
			camera->clip_z_far < v0.z&&camera->clip_z_far < v1.z&&camera->clip_z_far < v2.z ||					//远裁剪面

			camera->clip_plane_L.normal.DotProduct(v0.toVector4D() - camera->clip_plane_L.point) < 0 &&		//左裁剪面
			camera->clip_plane_L.normal.DotProduct(v1.toVector4D() - camera->clip_plane_L.point) < 0 &&		
			camera->clip_plane_L.normal.DotProduct(v2.toVector4D() - camera->clip_plane_L.point) < 0 ||		

			camera->clip_plane_R.normal.DotProduct(v0.toVector4D() - camera->clip_plane_R.point) < 0 &&		//右裁剪面
			camera->clip_plane_R.normal.DotProduct(v1.toVector4D() - camera->clip_plane_R.point) < 0 &&		
			camera->clip_plane_R.normal.DotProduct(v2.toVector4D() - camera->clip_plane_R.point) < 0 ||		

			camera->clip_plane_T.normal.DotProduct(v0.toVector4D() - camera->clip_plane_T.point) < 0 &&		//上裁剪面
			camera->clip_plane_T.normal.DotProduct(v1.toVector4D() - camera->clip_plane_T.point) < 0 &&		
			camera->clip_plane_T.normal.DotProduct(v2.toVector4D() - camera->clip_plane_T.point) < 0 ||		

			camera->clip_plane_B.normal.DotProduct(v0.toVector4D() - camera->clip_plane_B.point) < 0 &&		//下裁剪面
			camera->clip_plane_B.normal.DotProduct(v1.toVector4D() - camera->clip_plane_B.point) < 0 &&		
			camera->clip_plane_B.normal.DotProduct(v2.toVector4D() - camera->clip_plane_B.point) < 0 		
			)
			return true;
		return false;
	}

	void Clip_RenderList4D(TRenderList4D* renderList, TFrustum* camera)
	{
		TPolygonF4D* poly = NULL;
		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end();)
		{
			poly = &(*itr);
			//这里进行多边形裁剪
			if (isClipped(poly, camera->clip_plane_L) ||
				isClipped(poly, camera->clip_plane_R) ||
				isClipped(poly, camera->clip_plane_T) ||
				isClipped(poly, camera->clip_plane_B)
				) 
			{
				renderList->polyData.erase(itr++);
				continue;
			}
			++itr;
		}
	}

	void World_To_Screen_RenderList4D(TRenderList4D* renderList, TFrustum* camera)
	{
		TFloat XOffset = 0.5f*(camera->viewport_width - 1);
		TFloat YOffset = 0.5f*(camera->viewport_height - 1);
		TFloat dist = camera->view_dist;
		TFloat temp = 0;

		//这里对灯光位置进行变换，将其变换到相机坐标系下面
		TMatrix44 mat = camera->mWorldToCamera;
		//这里只保留旋转变换，而不进行平移变换，否则会出现灯光跟随摄像机移动的效果
		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.0f; mat.m[3][3] = 1.0f;

		TLight* light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
		{
			light = *itr;
			GetVector4DMulMatrix44(light->position, mat, light->transPosition);
			GetVector4DMulMatrix44(light->direction, mat, light->transDirection);
		}

		TPolygonF4D* poly = NULL;
		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end();)
		{
			poly = &(*itr);

			//首先进行背面消除
			if (Remove_Backface(poly, camera))
			{
				renderList->polyData.erase(itr++);
				continue;
			}

			//世界坐标变相机坐标
			GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, poly->transformList[0]);
			GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, poly->transformList[1]);
			GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, poly->transformList[2]);

			//进行裁剪
			if (isClipped(poly, camera->clip_z_near, camera->clip_z_far) ||
				isClipped(poly, camera->clip_plane_L) ||
				isClipped(poly, camera->clip_plane_R) ||
				isClipped(poly, camera->clip_plane_T) ||
				isClipped(poly, camera->clip_plane_B) 
				)
			{
				renderList->polyData.erase(itr++);
				continue;
			}

			//如果未被裁剪且不是背面，则进行光照计算
			Light_PolygonF4D(poly, camera);

			//之后执行接下来的变换
			for (TInt i = 0; i < 3; i++)
			{
				//世界坐标变换相机坐标
				//GetVertex4DMulMatrix44(poly->transformList[i],camera->mWorldToCamera,poly->transformList[i]);

				//透视变换
				temp = dist / poly->transformList[i].z;
				poly->transformList[i].x = temp * poly->transformList[i].x;
				poly->transformList[i].y = camera->aspect_ratio*temp*poly->transformList[i].y;

				//屏幕变换
				poly->transformList[i].x = XOffset * (1 + poly->transformList[i].x);
				poly->transformList[i].y = YOffset * (1 - poly->transformList[i].y);
			}

			++itr;
		}

		//在相机坐标系中对渲染列表的多边形进行深度排序
		//这里基于这样一个事实，即虽然x、y坐标发生了变化，但是z坐标依旧是之前摄像机坐标系下的z值
		Sort_RenderList4D(renderList);
	}

	void Draw_Object_Wire(TObject4D* object)
	{
		if (!object) return;

		TObject4D* obj = object;
		while (obj)
		{
			for (TInt i = 0; i < obj->polygonNumber; i++)
			{
				if ((obj->polygonList[i].state&TPOLY_STATE_ACTIVE) &&
					!(obj->polygonList[i].state&TPOLY_STATE_CLIPPED) &&
					!(obj->polygonList[i].state&TPOLY_STATE_BACKFACE)
					)
				{
					TInt index0 = obj->polygonList[i].verIndex[0];
					TInt index1 = obj->polygonList[i].verIndex[1];
					TInt index2 = obj->polygonList[i].verIndex[2];
					TGraphics::drawLine(obj->transformList[index0].x, obj->transformList[index0].y,
						obj->transformList[index1].x, obj->transformList[index1].y,
						TColor(obj->polygonList[i].color)
					);
					TGraphics::drawLine(obj->transformList[index1].x, obj->transformList[index1].y,
						obj->transformList[index2].x, obj->transformList[index2].y,
						TColor(obj->polygonList[i].color)
					);
					TGraphics::drawLine(obj->transformList[index0].x, obj->transformList[index0].y,
						obj->transformList[index2].x, obj->transformList[index2].y,
						TColor(obj->polygonList[i].color)
					);
				}
				//绘制完毕，在这里恢复多边形，清楚背面标志
				obj->polygonList[i].state = TPOLY_STATE_ACTIVE;
			}
			obj->state = TOBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}

	void Draw_RenderList4D_Wire(TRenderList4D* renderList)
	{
		TPolygonF4D* poly = NULL;
		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);

			{
				TGraphics::drawLine(poly->transformList[0].x, poly->transformList[0].y,
					poly->transformList[1].x, poly->transformList[1].y,
					TColor(poly->color)
				);
				TGraphics::drawLine(poly->transformList[1].x, poly->transformList[1].y,
					poly->transformList[2].x, poly->transformList[2].y,
					TColor(poly->color)
				);
				TGraphics::drawLine(poly->transformList[0].x, poly->transformList[0].y,
					poly->transformList[2].x, poly->transformList[2].y,
					TColor(poly->color)
				);
			}
			//绘制完毕，在这里恢复多边形，清楚背面标志
			poly->state = TPOLY_STATE_ACTIVE;
		}
		renderList->polyData.clear();
	}

	void Draw_Object4D_Solid(TObject4D* object)
	{
		if (!object)return;

		TObject4D* obj = object;
		while (obj)
		{
			for (TInt i = 0; i < obj->polygonNumber; i++)
			{
				if ((obj->polygonList[i].state&TPOLY_STATE_ACTIVE) &&
					!(obj->polygonList[i].state&TPOLY_STATE_CLIPPED) &&
					!(obj->polygonList[i].state&TPOLY_STATE_BACKFACE)
					)
				{
					TInt index0 = obj->polygonList[i].verIndex[0];
					TInt index1 = obj->polygonList[i].verIndex[1];
					TInt index2 = obj->polygonList[i].verIndex[2];

					TGraphics::fillTriangle(obj->transformList[index0].x, obj->transformList[index0].y,
						obj->transformList[index1].x, obj->transformList[index1].y,
						obj->transformList[index2].x, obj->transformList[index2].y,
						TColor(obj->polygonList[i].color)
					);
				}
				//绘制完毕，在这里恢复多边形，清除背面标志
				obj->polygonList[i].state = TPOLY_STATE_ACTIVE;
			}
			obj->state = TOBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}

	void Draw_RenderList4D_Solid(TRenderList4D* renderList)
	{
		TPolygonF4D* poly = NULL;
		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);
			
			{
				TGraphics::fillTriangle(poly->transformList[0].x, poly->transformList[0].y,
					poly->transformList[1].x, poly->transformList[1].y,
					poly->transformList[2].x, poly->transformList[2].y,
					TColor(poly->color)
				);
			}
			//绘制完毕，在这里恢复多边形，清除背面标志
			poly->state = TPOLY_STATE_ACTIVE;
		}
		renderList->polyData.clear();
	}

	//绘制平底为下三角的三角形
	void Draw_Top_Trangle(TFloat x0, TFloat y0, TFloat z0, TFloat u0, TFloat v0,
		TFloat x1, TFloat y1, TFloat z1, TFloat u1, TFloat v1,
		TFloat x2, TFloat y2, TFloat z2, TFloat u2, TFloat v2,
		TMaterial* mat, const TColor& color
	)
	{
		if (y2<0 || y0>SCREEN_HEIGHT - 1) return;
		
		TFloat dxdl = (x0 - x1) / (y0 - y1);
		TFloat dxdr = (x0 - x2) / (y0 - y2);

		//--------
		TFloat dzdl = (z0 - z1) / (y0 - y1);
		TFloat dzdr = (z0 - z2) / (y0 - y2);
		
		TFloat startL = x0;
		TFloat startR = x0;


		TFloat begX = 0;
		TFloat endX = 0;

		if (!mat || !mat->useTexture())
		{
			if (y0 < 0)
				y0 = 0;
			if (y2 > SCREEN_HEIGHT - 1)
				y2 = SCREEN_HEIGHT - 1;

			//没有纹理或者没有贴图，那么使用color着色
			for (TFloat y = y0; y < y2; y++)
			{
				//DrawLine(startL,y,startR,y,color);
				begX = startL;
				endX = startR;
				if (endX < begX)std::swap(begX, endX);
				if (begX < 0)begX = 0;
				if (endX > SCREEN_WIDTH - 1)endX = SCREEN_WIDTH - 1;

				for (TFloat x = begX; x < endX; x++)
				{
					if (TGraphics::checkZ(x, y, z0))
						TGraphics::setPixel(x, y,/*z0,*/color);
				}
				startL += dxdl;
				startR += dxdr;
			}
			return;
		}

		TFloat dudl = (u0 - u1) / (y0 - y1);
		TFloat dvdl = (v0 - v1) / (y0 - y1);
		TFloat dudr = (u0 - u2) / (y0 - y2);
		TFloat dvdr = (v0 - v2) / (y0 - y2);

		TFloat startLU = u0;
		TFloat startLV = v0;
		TFloat startRU = u0;
		TFloat startRV = v0;

		TFloat begU = 0;
		TFloat endU = 0;
		TFloat begV = 0;
		TFloat endV = 0;

		TFloat dx = 0;
		TFloat ui = 0;
		TFloat vi = 0;

		TFloat zl = 0;
		TFloat zr = 0;
		TFloat z = 0;

		for (TFloat y = y0; y < y2; y++)
		{
			begX = (TInt)startL;
			endX = (TInt)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;
			endV = startRV;

			//----------
			zl = z0 + (y - y0)*dzdl;
			zr = z0 + (y - y0)*dzdr;

			if (endX < begX)
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);
				std::swap(zl, zr);
			}

			//修正X的范围,并修正贴图的U坐标
			if (begX < 0)
			{
				//修正U
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}
			if (endX > SCREEN_WIDTH - 1)
			{
				//修正U
				endU = endU - (endX - SCREEN_WIDTH)*(startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;

			z = zl;
			TFloat zd = (zl - zr) / (begX - endX);
			for (TFloat x = begX; x <= endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT&&TGraphics::checkZ(x, y, z))
				{
					const TColor tex = mat->getPixel(begU, begV);
					TGraphics::setPixel(x, y,/*z,*/color* tex);
				}

				begU += ui;
				begV += vi;
				z += zd;
			}
			startL += dxdl;
			startR += dxdr;

			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	//绘制下三角
	void Draw_Button_Trangle(TFloat x0, TFloat y0, TFloat z0, TFloat u0, TFloat v0,
		TFloat x1, TFloat y1, TFloat z1, TFloat u1, TFloat v1,
		TFloat x2, TFloat y2, TFloat z2, TFloat u2, TFloat v2,
		TMaterial* mat, const TColor& color
	)
	{
		if (y2<0 || y0>SCREEN_HEIGHT - 1) return;

		TFloat dxdl = (x1 - x2) / (y1 - y2);
		TFloat dxdr = (x0 - x2) / (y0 - y2);
		
		TFloat dzdl = (z1 - z2) / (y1 - y2);
		TFloat dzdr = (z0 - z2) / (y0 - y2);

		TFloat startL = x1;
		TFloat startR = x0;

		TFloat begX = 0;
		TFloat endX = 0;

		if (!mat || !mat->useTexture())
		{
			if (y0 < 0) y0 = 0;
			if (y2 > SCREEN_HEIGHT - 1) y2 = SCREEN_HEIGHT - 1;

			//没有纹理或者没有贴图，那么使用color着色
			for (TFloat y = y0; y < y2; y++)
			{
				begX = startL;
				endX = startR;
				if (endX < endX)std::swap(begX, endX);

				if (begX < 0)begX = 0;
				if (endX > SCREEN_WIDTH - 1) endX = SCREEN_WIDTH - 1;

				for (TFloat x = begX; x < endX; x++)
				{
					if (TGraphics::checkZ(x, y, z0))
						TGraphics::setPixel(x, y,/*z0,*/color);
				}
				startL += dxdl;
				startR += dxdr;
			}
			return;
		}

		TFloat dudl = (u1 - u2) / (y1 - y2);
		TFloat dvdl = (v1 - v2) / (y1 - y2);
		TFloat dudr = (u0 - u2) / (y0 - y2);
		TFloat dvdr = (v0 - v2) / (y0 - y2);

		TFloat startLU = u1;
		TFloat startLV = v1;
		TFloat startRU = u0;
		TFloat startRV = v0;

		TFloat begU = 0;
		TFloat endU = 0;
		TFloat begV = 0;
		TFloat endV = 0;

		TFloat dx = 0;
		TFloat ui = 0;
		TFloat vi = 0;
		
		TFloat zl = 0;
		TFloat zr = 0;
		TFloat z = 0;
		for (TFloat y = y0; y < y2; y++)
		{
			begX = (TInt)startL;
			endX = (TInt)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;
			endV = startRV;
			//计算Z值
			zl = zl + (y - y0)*dzdl;
			zr = z0 + (y - y0)*dzdr;

			if (endX < begX)
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);
				std::swap(zl, zr);
			}

			//修正X的范围，并修正贴图的U坐标
			if (begX < 0)
			{
				//修正U
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}
			if (endX > SCREEN_WIDTH - 1)
			{
				//修正U
				endU = endU - (endX - SCREEN_WIDTH)*(startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;
			z = zl;
			TFloat zd = (zl - zr) / (begX - endX);
			for (TFloat x = begX; x <= endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT&&TGraphics::checkZ(x, y, z))
				{
					const TColor tex = mat->getPixel(begU, begV);
					TGraphics::setPixel(x, y,/*z,*/color*tex);
				}
				begU += ui;
				begV += vi;
				z += zd;
			}
			startL += dxdl;
			startR += dxdr;

			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	void Draw_Trangle(TPolygonF4D* poly)
	{
		TFloat x0 = poly->transformList[0].x, y0 = poly->transformList[0].y, z0 = poly->transformList[0].z, u0 = poly->transformList[0].u, v0 = poly->transformList[0].v;
		TFloat x1 = poly->transformList[1].x, y1 = poly->transformList[1].y, z1 = poly->transformList[1].z, u1 = poly->transformList[1].u, v1 = poly->transformList[1].v;
		TFloat x2 = poly->transformList[2].x, y2 = poly->transformList[2].y, z2 = poly->transformList[2].z, u2 = poly->transformList[2].u, v2 = poly->transformList[2].v;

		//保证 y0<y1<y2
		if (y1 < y0)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(z0, z1);
			std::swap(u0, u1);
			std::swap(v0, v1);
		}

		if (y2 < y0)
		{
			std::swap(x0, x2);
			std::swap(y0, y2);
			std::swap(z0, z2);
			std::swap(u0, u2);
			std::swap(v0, v2);
		}

		if (y2 < y1)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
			std::swap(z1, z2);
			std::swap(u1, u2);
			std::swap(v1, v2);
		}

		TFloat factor = (y1 - y0) / (y2 - y0);
		TFloat newX = x0 + factor * (x2 - x0);
		TFloat newZ = z0 + factor * (u2 - u0);
		TFloat newU = u0 + factor * (u2 - u0);
		//这里不能简单地是以后用v1来设置v
		TFloat newV = v0 + factor * (v2 - v0);
		Draw_Top_Trangle(x0, y0, z0, u0, v0, x1, y1, z1, u1, v1, newX, y1, newZ, newU, newV, poly->material, TColor(poly->color));
		Draw_Button_Trangle(newX, y1, newZ, newU, newV, x1, y2, z1, u1, v1, x2, y2, z2, u2, v2, poly->material, TColor(poly->color));
	}

	void Draw_RenderList4D_Texture_Solid(TRenderList4D* renderList)
	{
		TPolygonF4D* poly = NULL;
		for (TRenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);
			Draw_Trangle(poly);

			//绘制完毕，在这里恢复多边形，清楚背面标识
			poly->state = TPOLY_STATE_ACTIVE;
		}
		renderList->polyData.clear();
	}

}