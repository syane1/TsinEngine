#pragma once

namespace T3D
{
	//OgreMesh�ļ������Ϣ

	//Ogre Mesh �������ļ��е�IDö�٣���Դ��Ogre
	enum OGREMESHID
	{
		//�޷�������
		M_HEADER = 0x1000,
			//�汾��Ϣ
		M_MESH = 0x3000,
			//��������Ϣ
			M_SUBMESH = 0x4000,

			//M_GEOMETRY�飨����useSharedVertical = falseʱ�Ż���֣�
			M_GEOMETRY = 0x5000,//������Ǳ�Ƕ���������������
				//��������
				M_GEOMETRY_VERTEX_DECLARATION = 0x5100,
				M_GEOMETRY_VERTEX_ELEMENT = 0x5110,//�ظ�����
				M_GEOMETRY_VERTEX_BUFFER = 0x5200,//�ظ�����
				M_GEOMETRY_VERTEX_BUFFER_DATA = 0x5210,//ԭ��������

			M_SUBMESH_BOUNDS = 0x9000,

			M_SUBMESH_NAME_TABLE = 0xA000,
				//�ӿ������б�ÿ���ӿ�����������ַ���
				M_SUBMESH_NAME_TABLE_ELEMENT = 0xA100,
	};

	//��Ⱦ����ö��
	enum OperationType
	{
		//����б�
		OP_POINT_LIST = 1,
		//���б�һ���߰�����������
		OP_LINE_LIST = 2,
		//��״������
		OP_LINE_STRIP = 3,
		//�������б�
		OP_TRIANGLE_LIST = 4,
		//����������
		OP_TRIANGLE_STRIP= 5,
		//��������
		OP_TRIANGLE_FAN = 6
	};
}