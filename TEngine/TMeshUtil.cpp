#include "TMeshUtil.h"
#include "TUtil.h"
#include "TMaterial.h"

namespace T3D
{
	TString ReadString(std::ifstream& in, TInt size)
	{
		TString ss;
		if (size == -1)//读取一行
		{
			TChar c;
			while (in.read(&c, sizeof(TChar)) && c != '\n'&&c != '\r')
				ss += c;
		}
		else
		{
			TChar c[256];
			in.read(c, sizeof(TChar)* size);
			ss.append(c);
		}

		return ss;
	}

	TUShort ReadChunk(std::ifstream& in)
	{
		TUShort id;
		in.read((TChar*)& id, sizeof(TUShort));

		TUInt length;
		in.read((TChar*)& length, sizeof(TUInt));

		return id;
	}

	TBool ReadBool(std::ifstream& in)
	{
		TBool re = false;
		in.read((TChar*)&re, sizeof(TBool));
		return re;
	}

	TUInt ReadInt(std::ifstream& in)
	{
		TUInt re;
		in.read((TChar*)&re, sizeof(TUInt));
		return re;
	}

	TUShort ReadShort(std::ifstream& in)
	{
		TUShort re;
		in.read((TChar*)&re, sizeof(TUShort));
		return re;
	}

	TObject4D* LoadOgreMesh(const TString& meshName, const TVector4D& worldPos, const TVector4D& scale)
	{
		std::ifstream in;
		//模型文件，首先获取默认路径
		in.open(GetPath(meshName).c_str(), std::ios_base::binary | std::ios_base::in);
		if (in.bad())
		{
			in.close();
			return NULL;
		}

		TString name = GetNameFromPath(meshName);
		//加载材质
		ParseMaterialScript(name + ".material");

		TObject4D* head = NULL;
		{
			//网格头信息
			TUShort HEADER = ReadShort(in);
			//网格版本号
			TString Version = ReadString(in);

			//网格标记
			TUShort MESH = ReadChunk(in);
			//蒙皮骨骼
			TBool SkeletallyAnimated = ReadBool(in);

			//子网格标记
			TUShort SUBMESH = ReadChunk(in);
			TObject4D* curObject = NULL;
			while (!in.eof() && (SUBMESH == M_SUBMESH))
			{
				std::vector<TUInt> indexArray;
				std::vector<TFloat> vertexArray;

				if (head == NULL)
				{
					head = new TObject4D;
					curObject = head;
				}
				else 
				{
					curObject->nextObject = new TObject4D;
					curObject = curObject->nextObject;
				}
				curObject->name = name;
				curObject->state = TOBJECT_STATE_ACTIVE|TOBJECT_STATE_VISIBLE;
				//设置坐标
				curObject->worldPosition = worldPos;
				curObject->scale = scale;

				//材质信息
				TString material = ReadString(in);
				//读取多边形的材质
				curObject->materialName = material;
				
				//顶点是否共享
				TBool UseSharedVertices = ReadBool(in);
				
				//无符号顶点数量
				TUInt indexCount = ReadInt(in);
				//获取模型多边形个数
				curObject->polygonNumber = indexCount/3;
				curObject->polygonList.reserve(indexCount / 3);

				//索引是否为32位
				TBool indexis32Bit = ReadBool(in);
				if (indexis32Bit)
				{
					//存储面的顶点索引
					std::vector<TUInt>faceVertexIndices = ReadArray<TUInt>(in, indexCount);
					//读取模型顶点索引
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}
				else 
				{
					//储存面顶点索引
					std::vector<TUShort>faceVertexIndices = ReadArray<TUShort>(in, indexCount);
					//读取模型顶点索引
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}

				//网格几何信息
				TUShort GEOMETRY = ReadChunk(in);

				//顶点数量
				TUInt vertexCount = ReadInt(in);
				//模型顶点个数
				curObject->vertexNumber = vertexCount;
				curObject->localList.reserve(vertexCount);
				curObject->transformList.reserve(vertexCount);

				//网格几何顶点声明
				TUShort GEOMETRY_VERTEX_DECLARATION = ReadChunk(in);
				//网格几何顶点元素
				TUShort GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				while (!in.eof() && GEOMETRY_VERTEX_ELEMENT == M_GEOMETRY_VERTEX_ELEMENT)
				{
					//绑定源缓冲区
					TUShort source = ReadShort(in);
					//顶点类型
					TUShort type = ReadShort(in);
					//顶点语义
					TUShort semantic = ReadShort(in);
					//在缓存中的偏移值
					TUShort offset = ReadShort(in);
					//颜色和纹理坐标的索引值
					TUShort index = ReadShort(in);
					GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				}

				//顶点缓存
				TUShort GEOMETRY_VERTEX_BUFFER = GEOMETRY_VERTEX_ELEMENT;
				//把索引绑定到该缓存中
				TUShort bindIndex = ReadShort(in);

				//每个顶点大小必须与索引的声明保持一致
				TUShort vertexSize = ReadShort(in);

				//几何顶点缓存数据
				TUShort GEOMETRY_VERTEX_BUFFER_DATA = ReadChunk(in);

				TInt vertexNum = vertexSize / sizeof(TFloat);
				//缓存数据大小
				TInt bufferSize = vertexCount * vertexNum;
				//读取顶点数据
				vertexArray = ReadArray<TFloat>(in, bufferSize);

				for (TInt i = 0; i < (TInt)vertexArray.size(); i += vertexNum)
				{
					TVertex4D vex;
					//顶点坐标
					vex.x = vertexArray[i];
					vex.y = vertexArray[i + 1];
					vex.z = vertexArray[i + 2];
					//顶点UV
					vex.u = vertexArray[i + 6];
					vex.v = vertexArray[i + 7];

					curObject->localList.push_back(vex);
					curObject->transformList.push_back(vex);
				}

				for (TInt i = 0; i < curObject->polygonNumber; i++)
				{
					TInt index = i * 3;
					TPolygon4D poly;
					poly.state = TPOLY_STATE_ACTIVE;
					poly.attribute = TPOLY_ATTR_VERTEX_NORMAL | TPOLY_ATTR_VERTEX_POSITION | TPOLY_ATTR_VERTEX_UV;
					poly.verIndex[0] = indexArray[index];
					poly.verIndex[1] = indexArray[index+1];
					poly.verIndex[2] = indexArray[index+2];

					//这里将变换后的顶点指针交给每个多边形，是为了防止变换之后顶点索引依旧是最初的顶点坐标
					poly.verList = &curObject->transformList;
					curObject->polygonList.push_back(poly);
				}

				//子网格位置
				TUShort SUBMESH_OPERATION = ReadChunk(in);
				//操作类型
				TUShort operationType = ReadShort(in);

				SUBMESH = ReadChunk(in);
			}

			//网格包围盒
			TUShort MESH_BOUNDS = SUBMESH;
			if (MESH_BOUNDS == M_SUBMESH_BOUNDS)
			{
				//最小包围盒和最大包围盒及计算半径
				std::vector<TFloat> bounds = ReadArray<TFloat>(in, 7);
				head->minBoundingBox = TVector4D(bounds[0], bounds[1], bounds[2]);
				head->maxBoundingBox = TVector4D(bounds[3], bounds[4], bounds[5]);
				head->avgRadius = (head->maxBoundingBox - head->minBoundingBox).Length() / 2;

				TFloat minR = head->minBoundingBox.Length();
				TFloat maxR = head->maxBoundingBox.Length();
				head->maxRadius = maxR > minR ? maxR : minR;

				//子网格名字列表
				TUShort SUBMESH_NAME_TABLE = ReadChunk(in);
				if (SUBMESH_NAME_TABLE == M_SUBMESH_NAME_TABLE)
				{
					//子网格名字列表元素
					TUShort SUBMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
					while (!in.eof() && SUBMESH_NAME_TABLE_ELEMENT == M_SUBMESH_NAME_TABLE_ELEMENT)
					{
						//名字和索引
						TShort index = ReadShort(in);
						TString name = ReadString(in);
						SUBMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
					}
				}
			}
		}
		in.close();
		return head;
	}

}