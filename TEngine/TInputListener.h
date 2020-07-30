#pragma once

#include "TCore.h"

namespace T3D
{
	//Êó±ê¼üÅÌ¼àÌýÆ÷
	class TInputListener
	{
	public:
		virtual TBool keyPress(TInt key) = 0;
		virtual TBool keyRelease(TInt key) = 0;

		virtual TBool mouseButtonDown(TInt mouseButton) = 0;
		virtual TBool mouseButtonRelease(TInt mouseButton) = 0;
		virtual TBool mouseMove(TInt x, TInt y) = 0;
		virtual TBool mouseWheel(TInt delta) = 0;

		virtual ~TInputListener() {}
	};
}