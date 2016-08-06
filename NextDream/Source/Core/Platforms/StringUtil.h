#ifndef __BLADE_CORE_PLATFORMS_STRING_H__
#define __BLADE_CORE_PLATFORMS_STRING_H__

#include <Stringapiset.h>
#include <CharDefine.h>
#include <TypeDefine.h>
#include <cstring>

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

        static const TCHAR* WideCahrToTChar(const WIDECHAR* inSrc)
        {
#ifdef _UNICODE    
            return inSrc;
#else
            static TArray<TCHAR> tempStr;

            int32 length = WideCharToMultiByte(CP_ACP, 0, inSrc, Strlen(inSrc), NULL, 0, NULL, false);
            if (length == 0)
            {
                return "";
            }
            
            tempStr.Resize(length + 1);
            length = WideCharToMultiByte(CP_ACP, 0, inSrc, Strlen(inSrc), tempStr.TypePtr(), tempStr.GetLength(), NULL, false);
            if (length == 0)
            {
                return "";
            }

            return tempStr.TypePtr();
#endif
        }

        static const TCHAR* AnsiCahrToTChar(const ANSICHAR* inSrc)
        {
#ifdef _UNICODE    
            static TArray<WCHAR> tempStr;

            int32 length = MultiByteToWideChar(CP_ACP, 0, inSrc, Strlen(inSrc), NULL, 0);
            if (length == 0)
            {
                return "";
            }

            tempStr.Resize(length + 1);
            length = MultiByteToWideChar(CP_ACP, 0, inSrc, Strlen(inSrc), tempStr.TypePtr(), tempStr.GetLength());
            if (length == 0)
            {
                return "";
            }

            return tempStr.TypePtr();
#endif
            return inSrc;
        }
    };
}

#endif // !__BLADE_CORE_PLATFORMS_STRING_H__
