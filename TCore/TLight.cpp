#include "TLight.h"

namespace T3D
{
	TLight::TLight(TInt lightId, LIGHT_TYPE type) :id(lightId), lightOn(true), lightType(type),
													ambient(255, 255, 255), diffuse(255, 255, 255), specular(255, 255, 255),
													position(0, 0, 0), direction(0, 0, 1),
													kc(1.0f), kl(1.0f), kq(1.0f),
													spot_inner(45.0f), spot_outer(60.0f), power(1.0f), shadowFactor(0.05f) {}
	
	std::vector<TLight*>*Glights = NULL;
	//�����ƹ�
	TInt CreateLight(LIGHT_TYPE lightType)
	{
		if (GLights == NULL)
			GLights = new std::vector<TLight*>;

		if ((TInt)GLights->size() > MAX_LIGHTS )
			return -1;

		TInt id = (TInt)GLights->size();
		GLights->push_back(new TLight(id, lightType));

		return id;
	}

	//���һ���ƹ⣬����ƹⲻ���ڣ��򴴽�һ���µƹⲢ����
	TLight* GetLight(TInt id)
	{
		if (Glights == NULL || 
			(TInt)GLights->size() > MAX_LIGHTS || 
			id<0 || id>(TInt)GLights->size() - 1)
			return NULL;

		return GLights->at(id);
	}
	//��ȡ�ƹ��С
	TInt GetLightSize()
	{
		return GLights->size();
	}
	//ɾ�����еƹ�
	void DestroyAllLights()
	{
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
			SafeDelete((*itr));

		GLights->clear();
		SafeDelete(Glights);
	}

}