#pragma once

#include "TCommon.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdarg.h>
#include <Windows.h>
#include <ctime>

namespace T3D
{
	//全局的Log文件流
	extern std::ofstream *GLogStream;

	//初始化Log系统
	bool InitLog(const TString& fileName);
	//记录信息，使用前需要调用InitLog初始化Log，并最后调用CLoseLog关闭Log系统
	void Log(const TChar* string, ...);
	//关闭Log系统
	void CloseLog();

	//范围内的随机数
	//使用前需要手动播种
	inline TInt RandomInt(TInt from = 0, TInt to = 10)
	{
		TInt ran = rand() % (to - from + 1) + from;
		return ran;
	}

	//整数转化为字符串
	inline TString IntToString(TFloat num)
	{
		std::stringstream ss;
		ss << num;
		TString re;
		ss >> re;
		return re;
	}

	//字符串转化为整数
	inline TInt StringToInt(const TString &str)
	{
		return atoi(str.c_str());
	}

	//字符串转化为浮点数
	inline TFloat StringToFloat(const TString &str)
	{
		return (TFloat)atof(str.c_str());
	}

	//转化为宽字符串
	inline TWString ToTWString(const TString& str)
	{
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), NULL, 0);
		WCHAR *tString = new WCHAR[wcsLen + 1];

		::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), tString, wcsLen);
		//最后加上'\0'
		tString[wcsLen] = '\0';

		return TWString(tString);
	}

	//检测两个浮点数是否相等
	inline TBool EqualFloat(TFloat l, TFloat r)
	{
		return abs(l - r) <= EPSILON_E6;
	}

	//从路径里截取文件名称，如果只有文件名称，则直接返回（不包含后缀）
	inline TString GetNameFromPath(const TString &path)
	{
		std::size_t beg = path.find_last_of("\\/");
		std::size_t end = path.find_last_of(".");

		if (beg == TString::npos)
			beg = -1;
		return path.substr(beg + 1, end - beg - 1);
	}

	//将给定的字符串两端的空白字符删除
	inline TString Trim(const TString &msg)
	{
		const static TString SPACE_CHAR = "\t\f\v\n\r";
		std::size_t beg = msg.find_first_not_of(SPACE_CHAR);

		//防止空字符串出现
		if (beg > msg.length())
			return TString();
		TString result = msg.substr(beg);
		std::size_t end = result.find_last_not_of(SPACE_CHAR);
		if (end != TString::npos)
			end++;

		return result.substr(0, end);
	}


}