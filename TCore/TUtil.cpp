#include "TUtil.h"

namespace T3D
{
	std::ofstream *GLogStream = NULL;

	bool InitLog(const TString& fileName)
	{
		if (GLogStream == NULL)
		{
			GLogStream = new std::ofstream();
			GLogStream->open(fileName.c_str());
			if (GLogStream->good())
				return true;

			return false;
		}

		return true;
	}

	void Log(const TChar* string, ...)
	{
		if (GLogStream == NULL)
			return;

		TChar buffer[256];

		//��֤���߶���Ϊ��
		if (!string || !GLogStream)
			return;

		va_list arglist;//�ɱ�Ĳ����б�
		//������վ�ϵĿɱ�Ĳ�����ӡ����ַ���
		va_start(arglist, string);
		vsprintf(buffer, string, arglist);
		va_end(arglist);

		TString info(buffer);
		//�������Ϣ
		*GLogStream << info << std::endl;
		GLogStream->flush();
	}

	void CloseLog()
	{
		GLogStream->close();
		SafeDelete(GLogStream);
	}
	
}