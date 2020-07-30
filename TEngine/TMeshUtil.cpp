#include "TMeshUtil.h"
#include "TUtil.h"
#include "TMaterial.h"

namespace T3D
{
	TString ReadString(std::ifstream& in, TInt size)
	{
		TString ss;
		if (size == -1)//��ȡһ��
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
		//ģ���ļ������Ȼ�ȡĬ��·��
		in.open(GetPath(meshName).c_str(), std::ios_base::binary | std::ios_base::in);
		if (in.bad())
		{
			in.close();
			return NULL;
		}

		TString name = GetNameFromPath(meshName);
		//���ز���
		ParseMaterialScript(name + ".material");

		TObject4D* head = NULL;
		{
			//����ͷ��Ϣ
			TUShort HEADER = ReadShort(in);
			//����汾��
			TString Version = ReadString(in);

			//������
			TUShort MESH = ReadChunk(in);
			//��Ƥ����
			TBool SkeletallyAnimated = ReadBool(in);

			//��������
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
				//��������
				curObject->worldPosition = worldPos;
				curObject->scale = scale;

				//������Ϣ
				TString material = ReadString(in);
				//��ȡ����εĲ���
				curObject->materialName = material;
				
				//�����Ƿ���
				TBool UseSharedVertices = ReadBool(in);
				
				//�޷��Ŷ�������
				TUInt indexCount = ReadInt(in);
				//��ȡģ�Ͷ���θ���
				curObject->polygonNumber = indexCount/3;
				curObject->polygonList.reserve(indexCount / 3);

				//�����Ƿ�Ϊ32λ
				TBool indexis32Bit = ReadBool(in);
				if (indexis32Bit)
				{
					//�洢��Ķ�������
					std::vector<TUInt>faceVertexIndices = ReadArray<TUInt>(in, indexCount);
					//��ȡģ�Ͷ�������
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}
				else 
				{
					//�����涥������
					std::vector<TUShort>faceVertexIndices = ReadArray<TUShort>(in, indexCount);
					//��ȡģ�Ͷ�������
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}

				//���񼸺���Ϣ
				TUShort GEOMETRY = ReadChunk(in);

				//��������
				TUInt vertexCount = ReadInt(in);
				//ģ�Ͷ������
				curObject->vertexNumber = vertexCount;
				curObject->localList.reserve(vertexCount);
				curObject->transformList.reserve(vertexCount);

				//���񼸺ζ�������
				TUShort GEOMETRY_VERTEX_DECLARATION = ReadChunk(in);
				//���񼸺ζ���Ԫ��
				TUShort GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				while (!in.eof() && GEOMETRY_VERTEX_ELEMENT == M_GEOMETRY_VERTEX_ELEMENT)
				{
					//��Դ������
					TUShort source = ReadShort(in);
					//��������
					TUShort type = ReadShort(in);
					//��������
					TUShort semantic = ReadShort(in);
					//�ڻ����е�ƫ��ֵ
					TUShort offset = ReadShort(in);
					//��ɫ���������������ֵ
					TUShort index = ReadShort(in);
					GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				}

				//���㻺��
				TUShort GEOMETRY_VERTEX_BUFFER = GEOMETRY_VERTEX_ELEMENT;
				//�������󶨵��û�����
				TUShort bindIndex = ReadShort(in);

				//ÿ�������С��������������������һ��
				TUShort vertexSize = ReadShort(in);

				//���ζ��㻺������
				TUShort GEOMETRY_VERTEX_BUFFER_DATA = ReadChunk(in);

				TInt vertexNum = vertexSize / sizeof(TFloat);
				//�������ݴ�С
				TInt bufferSize = vertexCount * vertexNum;
				//��ȡ��������
				vertexArray = ReadArray<TFloat>(in, bufferSize);

				for (TInt i = 0; i < (TInt)vertexArray.size(); i += vertexNum)
				{
					TVertex4D vex;
					//��������
					vex.x = vertexArray[i];
					vex.y = vertexArray[i + 1];
					vex.z = vertexArray[i + 2];
					//����UV
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

					//���ｫ�任��Ķ���ָ�뽻��ÿ������Σ���Ϊ�˷�ֹ�任֮�󶥵���������������Ķ�������
					poly.verList = &curObject->transformList;
					curObject->polygonList.push_back(poly);
				}

				//������λ��
				TUShort SUBMESH_OPERATION = ReadChunk(in);
				//��������
				TUShort operationType = ReadShort(in);

				SUBMESH = ReadChunk(in);
			}

			//�����Χ��
			TUShort MESH_BOUNDS = SUBMESH;
			if (MESH_BOUNDS == M_SUBMESH_BOUNDS)
			{
				//��С��Χ�к�����Χ�м�����뾶
				std::vector<TFloat> bounds = ReadArray<TFloat>(in, 7);
				head->minBoundingBox = TVector4D(bounds[0], bounds[1], bounds[2]);
				head->maxBoundingBox = TVector4D(bounds[3], bounds[4], bounds[5]);
				head->avgRadius = (head->maxBoundingBox - head->minBoundingBox).Length() / 2;

				TFloat minR = head->minBoundingBox.Length();
				TFloat maxR = head->maxBoundingBox.Length();
				head->maxRadius = maxR > minR ? maxR : minR;

				//�����������б�
				TUShort SUBMESH_NAME_TABLE = ReadChunk(in);
				if (SUBMESH_NAME_TABLE == M_SUBMESH_NAME_TABLE)
				{
					//�����������б�Ԫ��
					TUShort SUBMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
					while (!in.eof() && SUBMESH_NAME_TABLE_ELEMENT == M_SUBMESH_NAME_TABLE_ELEMENT)
					{
						//���ֺ�����
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