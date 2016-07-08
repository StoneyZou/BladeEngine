#ifndef __BLADE_CORE_LOGGER_H__
#define __BLADE_CORE_LOGGER_H__

#include <CharDefine.h>
#include <TypeDefine.h>

namespace BladeEngine
{
	#include <cstdio>
	#include <cstdarg>

	enum ELOG_LEVEL
	{
		ELOG_LEVEL_ERROR,
		ELOG_LEVEL_WARMING,
		ELOG_LEVEL_NORMAL,
	};

	class Logger
	{
	private:
		TCHAR m_szBuffer[128];

	public:
		static void Log(const TCHAR* inFormat, const uint32 inCount, ...)
		{
			va_list args;
			va_start(args, inCount);

			LogInternal(ELOG_LEVEL_NORMAL, inFormat, inCount, args);
		}

		static void LogError(const TCHAR* inFormat, const uint32 inCount, ...)
		{
			va_list args;
			va_start(args, inCount);

			LogInternal(ELOG_LEVEL_ERROR, inFormat, inCount, args);
		}

	private:
		static void LogInternal(ELOG_LEVEL inLogLevel, const TCHAR* inFormat, const uint32 inCount, ...)
		{
			//snprintf(m_szBuffer, 128, inFormat, )
		}
	};
}

#endif	// !__BLADE_CORE_LOGGER_H__