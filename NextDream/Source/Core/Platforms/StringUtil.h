#ifndef __BLADE_CORE_PLATFORMS_STRING_H__
#define __BLADE_CORE_PLATFORMS_STRING_H__

#include <string.h>

#include <CharDefine.h>
#include <TypeDefine.h>
#include <MacroDefine.h>

namespace BladeEngine
{
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

        static void Strcpy(ANSICHAR* inDest, SIZE_T inDestSizeInByte, const ANSICHAR* inSrc)
        {
#if DEBUG || _DEBUG
            //assert(inDestSizeInByte >= (strlen(inSrc) + 1));
#endif
            strcpy(inDest, inSrc);
        }

        static void Strncpy(ANSICHAR* inDest, SIZE_T inDestSizeInByte, const ANSICHAR* inSrc, SIZE_T inSrcLen)
        {
#if DEBUG || _DEBUG
            //assert(inDestSizeInByte >= (inSrcLen + 1));
#endif
            strncpy(inDest, inSrc, inSrcLen);
        }

        static void Strcpy(WIDECHAR* inDest, SIZE_T inDestSizeInByte, const WIDECHAR* inSrc)
        {
#if DEBUG || _DEBUG
            //assert(inDestSizeInByte >= (wcslen(inSrc) + 1) * sizeof(WIDECHAR));
#endif
            wcscpy(inDest, inSrc);
        }

        static void Strncpy(WIDECHAR* inDest, SIZE_T inDestSizeInByte, const WIDECHAR* inSrc, SIZE_T inSrcLen)
        {
#if DEBUG || _DEBUG
            //assert(inDestSizeInByte >= (inSrcLen + 1) * sizeof(WIDECHAR));
#endif
            wcsncpy(inDest, inSrc, inSrcLen);
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
