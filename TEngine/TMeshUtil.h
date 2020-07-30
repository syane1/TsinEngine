#pragma once

#include "TCommon.h"
#include "TOgreMeshInfo.h"
#include "TCore.h"
#include <fstream>

namespace T3D
{
	//��stream�ж�ȡ�ַ���
	//size��ʾҪ��ȡ�ַ����ĸ��������size==-1�����ȡһ���ַ���
	TString ReadString(std::ifstream& in, TInt size = -1);

	//��ȡOgreMesh�ļ�ʱʹ��
	//��stream�ж�ȡ��һ����ʶ������ӦOGREMESHID
	TUShort ReadChunk(std::ifstream& in);

	//��stream�ж�ȡһ��bool����ֵ
	TBool ReadBool(std::ifstream& in);

	//��stream�ж�ȡһ��unsigned int���͵�ֵ
	TUInt ReadInt(std::ifstream& in);

	//��stream�ж�ȡһ��unsigned short���͵���ֵ
	TUShort ReadShort(std::ifstream& in);

	//��stream�ж�ȡT���͵�һ������
	//size��ʾ����Ĵ�С
	template<typename T>
	std::vector<T> ReadArray(std::ifstream& in, TInt size)
	{
		std::vector<T> v(size, 0);
		in.read((char*)(&v[0]), sizeof(T)*size);
		return v;
	}

	//�����������һ��Ogre.Mesh��ʽ�Ķ������ļ�������ʵ���˶�Ogre.Mesh�ļ���̬ģ�͵���ȷ��ȡ
	//���Ի�ȡģ�͵Ķ��������Ͷ������ݵ�ģ�����ݣ�������һ��TObject�������ģ���ж��submesh�����ͨ��TObject.nextObject����ȡ��һ��submesh��ָ��
	TObject4D* LoadOgreMesh(const TString& meshName, const TVector4D& worldPos = TVector4D::ZERO, const TVector4D& scale = TVector4D(1, 1, 1));

	/****************X�ļ�****************

	xof 0302txt 0064
	//xof:			�̶�
	//0302:			��ʾʹ��DirectX3.2�汾��ģ��
	//txt:			txt��ʽ(X�ļ���Ϊtxt��ʽ�Ͷ����Ƹ�ʽ����)
	//0064:			��ʾ�����͵�λ����64λ

	//ģ��header����:
	template Header{
	<3D82AB43-62DA-11cf-AB39-0020AF71E433>					//GUID��Global Unique Identifier,ȫ��Ψһ��ʶ����
		WORD major;											//X�ļ������汾
		WORD minor;											//X�ļ�����С�汾
		DWORD flags;										//���巽ʽ
	}

	//ģ��Mesh����:
	template Mesh
	{
	<3D82AB43-62DA-11cf-AB39-0020AF71E433>					//GUID��Global Unique Identifier,ȫ��Ψһ��ʶ����
		DWORD nVertices;									//�������
		array Vector vertices[nVertices];					//��������
		DWORD nFaces;										//�����
		array MeshFace faces[nFaces];						//������
		[...]												//��ʾ��ģ���ǿ��ŵ�ģ��,�����������������Ϣ
	}

	//ģ��Vector����:
	template Vector
	{
	<3D82AB43-62DA-11cf-AB39-0020AF71E433>					//GUID��Global Unique Identifier,ȫ��Ψһ��ʶ����
		float x;
		float y;
		float z;
	}

	//ģ��MeshFace����:
	template MeshFace
	{
	<3D82AB43-62DA-11cf-AB39-0020AF71E433>					//GUID��Global Unique Identifier,ȫ��Ψһ��ʶ����
		DWORD nFaceVertexIndices;							//��������(����������3������,����������Ϊ3)
		array DWORD faceVertexIndices[nFaceVertexIndices];	//���������б�
	}
	//��ϸ������:Instance.X
	****************X�ļ�****************/
}