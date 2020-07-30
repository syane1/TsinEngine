#pragma once

#include "TCommon.h"
#include "TOgreMeshInfo.h"
#include "TCore.h"
#include <fstream>

namespace T3D
{
	//从stream中读取字符串
	//size表示要读取字符串的个数，如果size==-1，则读取一行字符串
	TString ReadString(std::ifstream& in, TInt size = -1);

	//读取OgreMesh文件时使用
	//从stream中读取下一个标识符，对应OGREMESHID
	TUShort ReadChunk(std::ifstream& in);

	//从stream中读取一个bool型数值
	TBool ReadBool(std::ifstream& in);

	//从stream中读取一个unsigned int类型的值
	TUInt ReadInt(std::ifstream& in);

	//从stream中读取一个unsigned short类型的数值
	TUShort ReadShort(std::ifstream& in);

	//从stream中读取T类型的一组数组
	//size标示数组的大小
	template<typename T>
	std::vector<T> ReadArray(std::ifstream& in, TInt size)
	{
		std::vector<T> v(size, 0);
		in.read((char*)(&v[0]), sizeof(T)*size);
		return v;
	}

	//这个函数加载一个Ogre.Mesh格式的二进制文件，函数实现了对Ogre.Mesh文件静态模型的正确读取
	//可以获取模型的顶点索引和顶点数据等模型数据，并返回一个TObject对象，如果模型有多个submesh，则可通过TObject.nextObject来获取下一个submesh的指针
	TObject4D* LoadOgreMesh(const TString& meshName, const TVector4D& worldPos = TVector4D::ZERO, const TVector4D& scale = TVector4D(1, 1, 1));

	/****************X文件****************

	xof 0302txt 0064
	//xof:			固定
	//0302:			表示使用DirectX3.2版本的模型
	//txt:			txt格式(X文件分为txt格式和二进制格式两种)
	//0064:			表示浮点型的位数是64位

	//模型header定义:
	template Header{
	<3D82AB43-62DA-11cf-AB39-0020AF71E433>					//GUID（Global Unique Identifier,全局唯一标识符）
		WORD major;											//X文件的最大版本
		WORD minor;											//X文件的最小版本
		DWORD flags;										//定义方式
	}

	//模板Mesh定义:
	template Mesh
	{
	<3D82AB43-62DA-11cf-AB39-0020AF71E433>					//GUID（Global Unique Identifier,全局唯一标识符）
		DWORD nVertices;									//顶点个数
		array Vector vertices[nVertices];					//顶点数组
		DWORD nFaces;										//面个数
		array MeshFace faces[nFaces];						//面数组
		[...]												//表示该模型是开放的模板,即还可以添加其他信息
	}

	//模板Vector定义:
	template Vector
	{
	<3D82AB43-62DA-11cf-AB39-0020AF71E433>					//GUID（Global Unique Identifier,全局唯一标识符）
		float x;
		float y;
		float z;
	}

	//模板MeshFace定义:
	template MeshFace
	{
	<3D82AB43-62DA-11cf-AB39-0020AF71E433>					//GUID（Global Unique Identifier,全局唯一标识符）
		DWORD nFaceVertexIndices;							//索引个数(如三角形有3个顶点,则索引个数为3)
		array DWORD faceVertexIndices[nFaceVertexIndices];	//顶点索引列表
	}
	//详细见栗子:Instance.X
	****************X文件****************/
}