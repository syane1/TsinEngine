#pragma once

#include "TCommon.h"
#include "TVector.h"

namespace T3D 
{
	enum LIGHT_TYPE
	{
		LIGHT_AMBIENT = 0,		//������
		LIGHT_DIRECTION = 1,	//ƽ�й�
		LIGHT_POINT = 2,		//��ƹ�
	};

	struct TLight
	{
		TInt	id;					//�Ǹ��ķ���ID��ȫ��Ψһ
		TBool	lightOn;			//�ƹ��Ƿ�����
		
		LIGHT_TYPE lightType;		//�ƹ�����

		TFloat	power;				//�ƹ�ǿ��,Ĭ��Ϊ1.0f
		TFloat	shadowFactor;		//�ƹⱳ��ǿ�ȣ�Ĭ��Ϊ0.05f
		TColor	ambient;			//�ƹ�Ļ���ɫ��������ʹ�ã�
		TColor	diffuse;			//�ƹ���������ɫ
		TColor	specular;			//�ƹ�߹���ɫ

		TVector4D position;			//�ƹ���������λ��
		TVector4D direction;		//��Դ���򣬵�ƹ���Ч

		TVector4D transPosition;	//�ƹ����������ϵλ��
		TVector4D transDirection;	//���������ϵ��Դ���򣬵�ƹ���Ч

		TFloat	kc, kl, kq;			//˥������

		TFloat spot_inner;			//�۹����׶��
		TFloat spot_outer;			//�۹����׶��
		TFloat pf;					//�۹��ָ������
		
		TLight(TInt lightId, LIGHT_TYPE type);
	};

	//���ƹ���
	#define MAX_LIGHTS 8

	//ȫ�ֵƹ����
	extern std::vector<TLight*>*GLights;

	typedef std::vector<TLight*>::iterator LightItr;

	//����һ���µĵƹ⣬�����ɹ����صƹ�ID��ȫ��Ψһ��,����ʧ���򷵻�-1
	//����������Դ��ΪMAX_LIGHTS
	extern TInt			CreateLight(LIGHT_TYPE lightType);
	//���һ���ƹ⣬����ƹⲻ���ڣ��򷵻�NULL
	extern TLight*		GetLight(TInt id);
	extern TInt			GetLightSize();
	//���ȫ����Դ
	extern void			DestroyAllLights();

}