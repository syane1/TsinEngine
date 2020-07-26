#pragma once

#include "TCommon.h"
#include <map>
#include "TGraphics.h"

namespace T3D
{
	struct TMaterial
	{
		TString name;
		TColor ambient;		//环境色
		TColor diffuse;		//漫反射颜色
		TColor specular;	//镜面颜色

		TBitmap *bitmap;

		TMaterial();
		~TMaterial();
		inline bool useTexture() { return bitmap && bitmap->isValid(); }
		TColor getPixel(TFloat u, TFloat v);
	};

	typedef std::map<TString, TMaterial*>::iterator MaterialIter;
	extern std::map<TString, TMaterial*> *GMaterials;

	extern TMaterial* GetMaterial(const TString& name);
	extern bool SetMaterial(const TString& name, TMaterial* material);
	extern bool DestroyMaterial(const TString& name);
	extern void DestroyAllMaterials();

	//解析材质脚本，读取基本材质和贴图
	//材质脚本必须和模型名城相同，如ogreHead.mesh ogreHead.material
	extern void ParseMaterialScript(const TString& scriptName);
}