#pragma once

#include "TCommon.h"
#include <map>
#include "TGraphics.h"

namespace T3D
{
	struct TMaterial
	{
		TString name;
		TColor ambient;		//����ɫ
		TColor diffuse;		//��������ɫ
		TColor specular;	//������ɫ

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

	//�������ʽű�����ȡ�������ʺ���ͼ
	//���ʽű������ģ��������ͬ����ogreHead.mesh ogreHead.material
	extern void ParseMaterialScript(const TString& scriptName);
}