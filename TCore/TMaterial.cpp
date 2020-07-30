#pragma warning(disable : 4996)//忽略4996报错(因安全等问题旧函数弃用建议)

#include "TMaterial.h"
#include "TUtil.h"
#include <fstream>

namespace T3D
{
	TMaterial::TMaterial():ambient(0xffffff), diffuse(0xffffff), bitmap(NULL) {}
	
	TMaterial::~TMaterial()
	{
		SafeDelete(bitmap);
	}

	TColor TMaterial::getPixel(TFloat u, TFloat v)
	{
		static TColor defaultColor(255, 255, 255);
		if (bitmap&&bitmap->isValid())
		{
			//这是一个更精确的方法，但是效率第一点
			TInt x = Abs(u - (TInt)u)*bitmap->width;
			TInt y = Abs(v - (TInt)v)*bitmap->height;
			return bitmap->pixels[y*bitmap->pitch + x];
		}
		else
		{
			return defaultColor;
		}
	}

	std::map<TString, TMaterial*>*GMaterials = NULL;
	static void initMaterial()
	{
		static bool init = false;
		if (!init) GMaterials = new std::map<TString, TMaterial*>();
		init = true;
	}
	
	TMaterial* GetMaterial(const TString& name)
	{
		initMaterial();

		MaterialIter iter = GMaterials->find(name);
		if (iter == GMaterials->end())
			return NULL;
		else
			return iter->second;
	}

	bool SetMaterial(const TString& name, TMaterial* material)
	{
		initMaterial();

		GMaterials->insert(std::make_pair(name, material));
		return true;
	}

	bool DestroyMaterial(const TString& name)
	{
		initMaterial();

		MaterialIter iter = GMaterials->find(name);
		if (iter != GMaterials->end())
			SafeDelete(iter->second);
		GMaterials->erase(iter);
		return true;
	}

	void DestroyAllMaterials()
	{
		for (MaterialIter iter = GMaterials->begin(); iter != GMaterials->end(); ++iter)
			SafeDelete(iter->second);

		GMaterials->clear();
	}

	void ParseMaterialScript(const TString& scriptName)
	{
		std::ifstream in;
		//材质脚本，首先获取默认路径
		in.open(GetPath(scriptName).c_str());

		if (in.good())
		{
			TChar line[256];
			//通过while循环读取每一行数据
			while (in.good())
			{
				in.getline(line, 256);
				TString info = Trim(line);

				if (!info.empty())
				{
					//遇到“{”加1，遇到“}”减1，当brackets为0时，跳出当前材质
					TInt brackets = 0;
					std::size_t index = info.find("material");
					//出现新材质
					if (index == 0)
					{
						TMaterial* material = new TMaterial();
						material->name = Trim(info.substr(index + 8));

						//装到全局材质库
						SetMaterial(material->name, material);

						in.getline(line, 256);
						info = Trim(line);
						TChar temp[64];
						if (info == "{")brackets++;
						while (brackets != 0)
						{
							in.getline(line, 256);
							info = Trim(line);

							if (info == "{")brackets++;
							else if (info == "}")brackets--;
							else
							{
								//环境光
								std::size_t ambientIndex = info.find("ambient");
								if (ambientIndex == 0)
								{
									sscanf(info.c_str(), "%s %f %f %f",
										temp,
										&material->ambient.r,
										&material->ambient.g,
										&material->ambient.b
									);
									continue;
								}

								//漫反射
								std::size_t diffuseIndex = info.find("diffuse");
								if (diffuseIndex == 0)
								{
									sscanf(info.c_str(), "%s %f %f %f",
										temp,
										&material->diffuse.r,
										&material->diffuse.g,
										&material->diffuse.b
									);
									continue;
								}

								//镜面反射
								std::size_t specularIndex = info.find("specular");
								if (specularIndex == 0)
								{
									sscanf(info.c_str(), "%s %f %f %f",
										temp,
										&material->specular.r,
										&material->specular.g,
										&material->specular.b
									);
									continue;
								}

								//贴图
								std::size_t textureIndex = info.find("texture ");//这里使用//texture，后面保留一个空格，与texture_unit区别
								if (textureIndex == 0)
								{
									TChar textureName[64];
									sscanf(info.c_str(), "%s %s", temp, textureName);
									material->bitmap = new TBitmap(textureName);
									continue;
								}
							}
						}
					}
				}
			}
		}
		in.close();
	}

}