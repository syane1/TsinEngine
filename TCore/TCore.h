#include "TCommon.h"
#include "TVector.h"
#include <list>

#pragma once

namespace T3D
{
#define TPOLY_ATTR_2SIDE					0x0001		//˫��
#define TPOLY_ATTR_TRANSPARENT				0x0002		//͸��

#define TPOLY_ATTR_SHADE_MODE_PURE			0x0004
#define TPOLY_ATTR_SHADE_MODE_CONSTANT		0x0004
#define TPOLY_ATTR_SHADE_MODE_FLAT			0x0008
#define TPOLY_ATTR_SHADE_MODE_GROUND		0x0010
#define TPOLY_ATTR_SHADE_MODEPHONG			0x0020
#define TPOLY_ATTR_SHADE_MODEFASTPHONE		0x0040

#define TPOLY_ATTR_VERTEX_POSITION			0x1000
#define TPOLY_ATTR_VERTEX_NORMAL			0x2000
#define TPOLY_ATTR_VERTEX_UV				0x400

#define TPOLY_STATE_ACTIVE					0x0100		//��Ч��״̬
#define TPOLY_STATE_CLIPPED					0x0200		//���ü�״̬
#define TPOLY_STATE_BACKFACE				0x0400		//����״̬

#define TOBJECT_STATE_ACTIVE				0x0001
#define TOBJECT_STATE_VISIBLE				0x0002
#define TOBJECT_CULLED						0x0004

	//�����޳�����
	enum CULL_TYPE
	{
		CULL_BOX = 0,
		CULL_SPHERE = 1,
	};

	struct TVertex4D;
	typedef std::vector<TVertex4D>TVertex4DArray;
	typedef std::vector<TVector4D>TVector4DArray;
	typedef std::vector<TPolygon4D>TPolygon4DArray;

	//������󣬰������㡢���ߡ�uv
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

	//��
	struct TPlane3D
	{
		TVector4D			point;			//����һ��
		TVector4D			normal;			//��ķ���

		TPlane3D() {}
		TPlane3D(const TVector4D&p, const TVector4D&nor);
		TPlane3D& operator=(const TPlane3D& right);
	};

	//Poly4D1_TYPE
	//�˶���νṹ�ǻ��ڶ���������ʽ�����ݽṹ
	struct TPolygon4D
	{
		TInt state;					//״̬��Ϣ
		TInt attribute;				//�������������
		TInt color;					//�������ɫ

		TVertex4DArray *verList;	//�����������ָ��transformList
		TInt verIndex[3];			//��������

		TPolygon4D();
	};

	//POLY4DV1_TYP
	//�˶���νṹ���ڶ��㷽ʽ�����ݽṹ
	struct TPolygonF4D
	{
		TInt state;					//״̬��Ϣ
		TInt attribute;				//�������������
		TInt color;					//����ι���ǿ����ɫ

		TVertex4D localList[3];		//����ֲ�����
		TVertex4D transformList[3];	//����ֲ����꾭�任֮�������

		struct TMaterial* material;

		TPolygonF4D();
	};

	struct CompTPolygonF4D
	{
		TBool operator()(const TPolygonF4D& polyA, const TPolygonF4D& polyB)
		{
			TFloat zA = Max(polyA.transformList[0].z, Max(polyA.transformList[1].z, polyA.transformList[2].z));
			TFloat zB = Max(polyB.transformList[0].z, Max(polyB.transformList[1].z, polyB.transformList[2].z));
			//����д����debugģʽ�»ر���
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

	//���ڶ����б�Ͷ�����б������
	struct TObject4D
	{
		TString			name;			//��������
		TInt			state;			//����״̬
		TInt			attribute;		//��������
		TFloat			avgRadius;		//�����ƽ���뾶��������ײ���
		TFloat			maxRadius;		//��������뾶

		TBool			needCull;		//�Ƿ���Ҫִ���޳�����
		
		TString			materialName;	//ģ�Ͳ��ʵ����ƣ�for Ogre Mesh

		TVector4D		minBoundingBox;	//ģ���������Χ����С��
		TVector4D		maxBoundingBox;	//ģ���������Χ������

		//���ﲻ����ƽ�ƻ������ţ����ѵ�һ��ģ�͵Ĳ���Ϊ׼
		TVector4D		worldPosition;	//���嵱ǰ�����������е�λ��
		TVector4D		scale;			//���嵱ǰ����ֵ

		TVector4D		direction;		//����ķ�����������������¼�ظ������ת�Ƕ�
		TInt			vertexNumber;	//�������

		TVertex4DArray	localList;		//����任ǰ������ֲ���������
		TVertex4DArray	transformList;	//����任�������

		TInt			polygonNumber;	//���������������εĸ���
		TPolygon4DArray	polygonList;	//�洢����ε�����

		//���������Ϊ�˴���һ�������mesh�ж��submesh����������ǵģ�
		//����ж��submesh�Ļ�nextObject!=NULL �����ָ���ָ����һ��submesh��ֱ��nextObject==NULL
		TObject4D		*nextObject;

		TObject4D();
	};

	//��Ⱦ�б�
	struct TRenderList4D
	{
		TInt					state;			//����״̬
		TInt					attribute;		//��������

		std::list<TPolygonF4D>	polyData;		//����Ķ�������ݣ�ʹ��˫�˶��У�ɾ������Ч

		typedef std::list<TPolygonF4D>::iterator Itr;
	};

	//����任���Ʊ��
	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,			//ֻ�Ծֲ������б���б任
		TRANSFORM_TRANS = 1,			//ֻ�Ա任��Ķ����б���б任
		TRANSFORM_LOCAL_TO_TRANS = 2,	//�Ծֲ������б���б任����������洢�ڱ任��Ķ����б���
	};

	//��TObject���б任�����Խ���λ�Ʊ任����ת�任
	//transformTypeָ���˶���һ�������б���б任
	//transformBasisָ�����Ƿ�Ҫ�Գ����������б任
	void Transform_Object4D(TObject4D* obj,
		const TMatrix44& mat,
		OBJ_TRANSFORM_TYPE transformType/*,
		TBool transformBasis = true
		*/
	);

	//��TRenderList4D���б任�����Խ���λ�Ʊ任����ת�任
	//transformTypeָ���˶���һ�������б���б任
	void Transform_RenderList4D(TRenderList4D* renderList,
		const TMatrix44& mat,
		OBJ_TRANSFORM_TYPE transformType
	);

	//��Object����RenderList������LocalList�洢����ԭʼ���꣬TransformList�洢���Ǿ���ƽ�ƺ����ŵ�����
	void Insert_Object4D_To_RenderList4D(TRenderList4D* renderList, TObject4D* obj,
		OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS,
		bool transformScaleAndPosition = false
	);

	//�ֲ����궨���б�任���������꣬����ֱ��ʹ�üӷ�����û��ʹ���������˵ķ�������Ϊû�б�Ҫ
	void Local_To_World_RenderList4D(TRenderList4D* renderList,
		const TVector4D& pos,
		OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS
	);

	//����������޳�
	bool Cull_Object4D(TObject4D* object, TFrustum* camera, CULL_TYPE cullType = CULL_SPHERE);

	//�������棬���������㷨��������ӵ��ǻ�����������ģ�������Ҫ�ڶ���任�����������ǰ���б�������
	bool Remove_Backface(TPolygon4D* poly, TFrustum* camera);
	bool Remove_Backface(TPolygonF4D* poly, TFrustum* camera);
	void Remove_Backface_Object4D(TObject4D* object, TFrustum* camera);
	//light�Ƿ�Զ���ν��й��մ���
	void Remove_Backface_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//���ռ���
	//���ռ�����������������ϵ��������֮�󣬻��������������ϵ�н���
	void Light_PolygonF4D(TPolygonF4D* poly, TFrustum* camera);
	void Light_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//�������������������
	void World_To_Camera_Object4D(TObject4D* obj, TFrustum* camera);
	void World_To_Camera_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//��RenderList�Ķ���ν����������ʹ�û����㷨
	//������Ҫ�ڶ���δ������������ϵʱ�ſ��Ե��ã�������������
	void Sort_RenderList4D(TRenderList4D* renderList);

	//�������ת��Ϊ͸������
	void Camera_To_Perspective_Object4D(TObject4D* obj, TFrustum* camera);
	void Camera_To_Screen_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//͸������ת������Ļ����
	void Perspective_To_Screen_Object4D(TObject4D* obj, TFrustum* camera);
	void Perspective_ToRenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//��RenderList4D���вü��������ü��������������Χ�ڵĶ���Σ��˲���������������ռ���ִ��
	void Clip_RenderList4D(TRenderList4D* renderList, TFrustum* camera);

	//��������任����Ļ����
	//�൱�ڵ���
	//World_To_Camera
	//Perspective_To_Screen��Ч��
	//���ڲ��Ѿ�ִ�����޳����������������մ���Ȳ���
	void World_To_Screen_RenderList4D(TRenderList4D* remderList, TFrustum* camera);

	//��Ⱦ�߿�ģ��
	void Draw_Object4D_Wire(TObject4D* obj);
	void Draw_RenderList4D_Wire(TRenderList4D* renerList);

	//���ƴ������renderList
	void Draw_RenderList4D_Texture_Solid(TRenderList4D* renderList);
}