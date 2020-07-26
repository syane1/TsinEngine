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
	//ȫ�ֵ�Log�ļ���
	extern std::ofstream *GLogStream;

	//��ʼ��Logϵͳ
	bool InitLog(const TString& fileName);
	//��¼��Ϣ��ʹ��ǰ��Ҫ����InitLog��ʼ��Log����������CLoseLog�ر�Logϵͳ
	void Log(const TChar* string, ...);
	//�ر�Logϵͳ
	void CloseLog();

	//��Χ�ڵ������
	//ʹ��ǰ��Ҫ�ֶ�����
	inline TInt RandomInt(TInt from = 0, TInt to = 10)
	{
		TInt ran = rand() % (to - from + 1) + from;
		return ran;
	}

	//����ת��Ϊ�ַ���
	inline TString IntToString(TFloat num)
	{
		std::stringstream ss;
		ss << num;
		TString re;
		ss >> re;
		return re;
	}

	//�ַ���ת��Ϊ����
	inline TInt StringToInt(const TString &str)
	{
		return atoi(str.c_str());
	}

	//�ַ���ת��Ϊ������
	inline TFloat StringToFloat(const TString &str)
	{
		return (TFloat)atof(str.c_str());
	}

	//ת��Ϊ���ַ���
	inline TWString ToTWString(const TString& str)
	{
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), NULL, 0);
		WCHAR *tString = new WCHAR[wcsLen + 1];

		::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), tString, wcsLen);
		//������'\0'
		tString[wcsLen] = '\0';

		return TWString(tString);
	}

	//��������������Ƿ����
	inline TBool EqualFloat(TFloat l, TFloat r)
	{
		return abs(l - r) <= EPSILON_E6;
	}

	//��·�����ȡ�ļ����ƣ����ֻ���ļ����ƣ���ֱ�ӷ��أ���������׺��
	inline TString GetNameFromPath(const TString &path)
	{
		std::size_t beg = path.find_last_of("\\/");
		std::size_t end = path.find_last_of(".");

		if (beg == TString::npos)
			beg = -1;
		return path.substr(beg + 1, end - beg - 1);
	}

	//���������ַ������˵Ŀհ��ַ�ɾ��
	inline TString Trim(const TString &msg)
	{
		const static TString SPACE_CHAR = "\t\f\v\n\r";
		std::size_t beg = msg.find_first_not_of(SPACE_CHAR);

		//��ֹ���ַ�������
		if (beg > msg.length())
			return TString();
		TString result = msg.substr(beg);
		std::size_t end = result.find_last_not_of(SPACE_CHAR);
		if (end != TString::npos)
			end++;

		return result.substr(0, end);
	}


}