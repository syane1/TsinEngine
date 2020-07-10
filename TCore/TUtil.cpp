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

		//保证两者都不为空
		if (!string || !GLogStream)
			return;

		va_list arglist;//可变的参数列表
		//利用在站上的可变的参数打印输出字符串
		va_start(arglist, string);
		vsprintf(buffer, string, arglist);
		va_end(arglist);

		TString info(buffer);
		//输出流信息
		*GLogStream << info << std::endl;
		GLogStream->flush();
	}

	void CloseLog()
	{
		GLogStream->close();
		SafeDelete(GLogStream);
	}
	
}