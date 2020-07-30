#include "TCommon.h"
#include "TVector.h"
#include <list>

#pragma once

namespace T3D
{
#define TPOLY_ATTR_2SIDE					0x0001		//双面
#define TPOLY_ATTR_TRANSPARENT				0x0002		//透明

#define TPOLY_ATTR_SHADE_MODE_PURE			0x0004
#define TPOLY_ATTR_SHADE_MODE_CONSTANT		0x0004
#define TPOLY_ATTR_SHADE_MODE_FLAT			0x0008
#define TPOLY_ATTR_SHADE_MODE_GROUND		0x0010
#define TPOLY_ATTR_SHADE_MODEPHONG			0x0020
#define TPOLY_ATTR_SHADE_MODEFASTPHONE		0x0040

#define TPOLY_ATTR_VERTEX_POSITION			0x1000
#define TPOLY_ATTR_VERTEX_NORMAL			0x2000
#define TPOLY_ATTR_VERTEX_UV				0x400

#define TPOLY_STATE_ACTIVE					0x0100		//有效的状态
#define TPOLY_STATE_CLIPPED					0x0200		//被裁剪状态
#define TPOLY_STATE_BACKFACE				0x0400		//背面状态

#define TOBJECT_STATE_ACTIVE				0x0001
#define TOBJECT_STATE_VISIBLE				0x0002
#define TOBJECT_CULLED						0x0004

	//物体剔除类型
	enum CULL_TYPE
	{
		CULL_BOX = 0,
		CULL_SPHERE = 1,
	};

	struct TVertex4D;
	typedef std::vector<TVertex4D>TVertex4DArray;
	typedef std::vector<TVector4D>TVector4DArray;
	typedef std::vector<TPolygon4D>TPolygon4DArray;

	//定点对象，包含定点、法线、uv
	struct TVertex4D
	{
		TFloat x, y, z, w;

		TFloat u, v;

		TVertex4D();
		TVertex4D(const TVector4D& v);
		TVertex4D(TFloat xf, TFloat yf, TFloat zf);
		TVertex4D operator*(const TVector4D& right);
		TVertex4D operator-(const TVertex4D& right);
		TVertex4D& operator=(const TVertex4D& right);
		TVertex4D operator+(const TVector4D& right);
		TVertex4D operator+(const TVertex4D& right);
		TVertex4D operator/(TFloat factor);

		TVector4D toVector4D()const;
	};

	//面
	struct TPlane3D
	{
		TVector4D			point;			//面上一点
		TVector4D			normal;			//面的法线

		TPlane3D() {}
		TPlane3D(const TVector4D&p, const TVector4D&nor);
		TPlane3D& operator=(const TPlane3D& right);
	};

	//Poly4D1_TYPE
	//此多边形结构是基于定点索引方式的数据结构
	struct TPolygon4D
	{
		TInt state;					//状态信息
		TInt attribute;				//多边形物理属性
		TInt color;					//多边形颜色

		TVertex4DArray *verList;	//这里的引用是指的transformList
		TInt verIndex[3];			//定点索引

		TPolygon4D();
	};

	//POLY4DV1_TYP
	//此多边形结构基于定点方式的数据结构
	struct TPolygonF4D
	{
		TInt state;					//状态信息
		TInt attribute;				//多边形物理属性
		TInt color;					//多边形光照强度颜色

		TVertex4D localList[3];		//物体局部坐标
		TVertex4D transformList[3];	//物体局部坐标经变换之后的坐标

		struct TMaterial* material;

		TPolygonF4D();
	};

	struct CompTPolygonF4D
	{
		TBool operator()(const TPolygonF4D& polyA, const TPolygonF4D& polyB)
		{
			TFloat zA = Max(polyA.transformList[0].z, Max(polyA.transformList[1].z, polyA.transformList[2].z));
			TFloat zB = Max(polyB.transformList[0].z, Max(polyB.transformList[1].z, polyB.transformList[2].z));
			//这里写成在debug模式下回崩溃
			if (Abs(zA - zB) < 0.005f)
			{
				zA = (polyA.transformList[0].z + polyA.transformList[1].z + polyA.transformList[2].z) / 3.0f;
				zB = (polyB.transformList[0].z + polyB.transformList[1].z + polyB.transformList[2].z) / 3.0f;

				if (zA <= zB)
					return false;
				else
					return true;
			}
			else if (zA < zB)
				return false;
			else
				return true;
		}
	};

	//基于定点列表和多边形列表的物体
	struct TObject4D
	{
		TString			name;			//物体名称
		TInt			state;			//物体状态
		TInt			attribute;		//物体属性
		TFloat			avgRadius;		//物体的平均半径，用于碰撞检测
		TFloat			maxRadius;		//物体的最大半径

		TBool			needCull;		//是否需要执行剔除操作
		
		TString			materialName;	//模型材质的名称：for Ogre Mesh

		TVector4D		minBoundingBox;	//模型正方体包围盒最小点
		TVector4D		maxBoundingBox;	//模型正方体包围盒最大点

		//这里不论是平移还是缩放，均已第一个模型的参数为准
		TVector4D		worldPosition;	//物体当前在世界坐标中的位置
		TVector4D		scale;			//物体当前缩放值

		TVector4D		direction;		//物体的方向向量，各分量记录沿各轴的旋转角度
		TInt			vertexNumber;	//顶点个数

		TVertex4DArray	localList;		//物体变换前的物体局部坐标数组
		TVertex4DArray	transformList;	//物体变换后的坐标

		TInt			polygonNumber;	//物体的所包含多边形的个数
		TPolygon4DArray	polygonList;	//存储多边形的数组

		//这个变量是为了处理一个导入的mesh有多个submesh的情况来考虑的，
		//如果有多个submesh的话nextObject!=NULL 否则该指针会指向下一个submesh，直至nextObject==NULL
		TObject4D		*nextObject;

		TObject4D();
	};

	//渲染列表
	struct TRenderList4D
	{
		TInt					state;			//物体状态
		TInt					attribute;		//物体属性

		std::list<TPolygonF4D>	polyData;		//具体的多边形数据，使用双端队列，删除更有效

		typedef std::list<TPolygonF4D>::iterator Itr;
	};

	//定点变换控制标记
	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,			//只对局部顶点列表进行变换
		TRANSFORM_TRANS = 1,			//只对变换后的顶点列表进行变换
		TRANSFORM_LOCAL_TO_TRANS = 2,	//对局部顶点列表进行变换，并将结果存储在变换后的顶点列表中
	};

	//对TObject进行变换，可以进行位移变换和旋转变换
	//transformType指定了对哪一个顶点列表进行变换
	//transformBasis指定了是否要对朝向向量进行变换
	void Transform_Object4D(TObject4D* obj,
		const TMatrix44& mat,
		OBJ_TRANSFORM_TYPE transformType/*,
		TBool transformBasis = true
		*/
	);

	//对TRenderList4D进行变换，可以进行位移变换和旋转变换
	//transformType指定了对哪一个顶点列表进行变换
	void Transform_RenderList4D(TRenderList4D* renderList,
		const TMatrix44& mat,
		OBJ_TRANSFORM_TYPE transformType
	);

	//将Object插入RenderList，这里LocalList存储的是原始坐标，TransformList存储的是经过平移和缩放的坐标
	void Insert_Object4D_To_RenderList4D(TRenderList4D* renderList, TObject4D* obj,
		OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS,
		bool transformScaleAndPosition = false
	);

	//局部坐标定编列表变换至世界坐标，这里直接使用加法，而没有使用与矩阵相乘的方法，因为没有必要
	void Local_To_World_RenderList4D(TRenderList4D* renderList,
		const TVector4D& pos,
		OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS
	);

	//对物体进行剔除
	bool Cull_Object4D(TObject4D* object, TFrustum* camera, CULL_TYPE cullType = CULL_SPHERE);

	//消除背面，背面消除算法的摄像机视点是基于世界坐标的，所以需要在顶点变换成摄像机坐标前进行背面消除
	bool Remove_Backface(TPolygon4D* poly, TFrustum* camera);
	bool Remove_Backface(TPolygonF4D* poly, TFrustum* camera);
	void Remove_Backface_Object4D(TObject4D* object, TFrustum* camera);
	//light是否对多边形进行光照处理
	void Remove_Backface_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//光照计算
	//光照计算智能在世界坐标系消除背面之后，或者在摄像机坐标系中进行
	void Light_PolygonF4D(TPolygonF4D* poly, TFrustum* camera);
	void Light_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//世界坐标变成摄像机坐标
	void World_To_Camera_Object4D(TObject4D* obj, TFrustum* camera);
	void World_To_Camera_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//对RenderList的多边形进行深度排序，使用画家算法
	//这里需要在多边形处于摄像机坐标系时才可以调用，否则会出现问题
	void Sort_RenderList4D(TRenderList4D* renderList);

	//相机坐标转换为透视坐标
	void Camera_To_Perspective_Object4D(TObject4D* obj, TFrustum* camera);
	void Camera_To_Screen_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//透视坐标转换至屏幕坐标
	void Perspective_To_Screen_Object4D(TObject4D* obj, TFrustum* camera);
	void Perspective_ToRenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//对RenderList4D进行裁剪操作，裁剪掉超出摄像机范围内的多边形，此操作必须在摄像机空间内执行
	void Clip_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//世界坐标变换到屏幕坐标
	//相当于调用
	//World_To_Camera
	//Perspective_To_Screen的效用
	//其内部已经执行了剔除，背面消除，光照处理等操作
	void World_To_Screen_RenderList4D(TRenderList4D* remderList, TFrustum* camera);

	//渲染线框模型
	void Draw_Object4D_Wire(TObject4D* obj);
	void Draw_RenderList4D_Wire(TRenderList4D* renerList);

	//绘制带纹理的renderList
	void Draw_RenderList4D_Texture_Solid(TRenderList4D* renderList);
}