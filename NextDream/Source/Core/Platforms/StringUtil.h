#ifndef __BLADE_CORE_PLATFORMS_STRING_H__
#define __BLADE_CORE_PLATFORMS_STRING_H__

#include <CharDefine.h>
#include <TypeDefine.h>

namespace BladeEngine
{
    #include <cstring>

    class StringUtil
    {
    public:
        static SIZE_T Strlen(const ANSICHAR* inStr)
        {
            return strlen(inStr);
        }

        static SIZE_T Strlen(const WIDECHAR* inStr)
        {
            return wcslen(inStr);
        }

        static SIZE_T Strcpy(ANSICHAR* inDest, SIZE_T inDestLen, const ANSICHAR* inSrc)
        {
            return strcpy_s(inDest, inDestLen, inSrc);
        }

        static SIZE_T Strcpy(WIDECHAR* inDest, SIZE_T inDestLen, const WIDECHAR* inSrc)
        {
            return wcscpy_s(inDest, inDestLen, inSrc);
        }

        static SIZE_T Strcmp(const ANSICHAR* inSrc1, const ANSICHAR* inSrc2, SIZE_T inMaxLen)
        {
            return strncmp(inSrc1, inSrc2, inMaxLen);
        }

        static SIZE_T Strcmp(const WIDECHAR* inSrc1, const WIDECHAR* inSrc2, SIZE_T inMaxLen)
        {
            return wcsncmp(inSrc1, inSrc2, inMaxLen);
        }

        static bool IsNullOrEmpty(const ANSICHAR* inStr)
        {
            return inStr == NULL || Strlen(inStr) == 0;
        }
    };
}

#endif // !__BLADE_CORE_PLATFORMS_STRING_H__