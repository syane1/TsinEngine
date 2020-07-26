#pragma once

#include "TCommon.h"
#include "TVector.h"

namespace T3D 
{
	enum LIGHT_TYPE
	{
		LIGHT_AMBIENT = 0,		//环境光
		LIGHT_DIRECTION = 1,	//平行光
		LIGHT_POINT = 2,		//点灯光
	};

	struct TLight
	{
		TInt	id;					//登个的分配ID，全局唯一
		TBool	lightOn;			//灯光是否启用
		
		LIGHT_TYPE lightType;		//灯光类型

		TFloat	power;				//灯光强度,默认为1.0f
		TFloat	shadowFactor;		//灯光背面强度，默认为0.05f
		TColor	ambient;			//灯光的环境色（环境光使用）
		TColor	diffuse;			//灯光漫反射颜色
		TColor	specular;			//灯光高光颜色

		TVector4D position;			//灯光世界坐标位置
		TVector4D direction;		//光源朝向，点灯光无效

		TVector4D transPosition;	//灯光摄像机坐标系位置
		TVector4D transDirection;	//摄像机坐标系光源朝向，点灯光无效

		TFloat	kc, kl, kq;			//衰减因子

		TFloat spot_inner;			//聚光灯内锥角
		TFloat spot_outer;			//聚光灯外锥角
		TFloat pf;					//聚光灯指数因子
		
		TLight(TInt lightId, LIGHT_TYPE type);
	};

	//最大灯光数
	#define MAX_LIGHTS 8

	//全局灯光管理
	extern std::vector<TLight*>*GLights;

	typedef std::vector<TLight*>::iterator LightItr;

	//创建一个新的灯光，创建成功返回灯光ID（全局唯一）,创建失败则返回-1
	//场景内最大光源数为MAX_LIGHTS
	extern TInt			CreateLight(LIGHT_TYPE lightType);
	//获得一个灯光，如果灯光不存在，则返回NULL
	extern TLight*		GetLight(TInt id);
	extern TInt			GetLightSize();
	//清除全部光源
	extern void			DestroyAllLights();

}