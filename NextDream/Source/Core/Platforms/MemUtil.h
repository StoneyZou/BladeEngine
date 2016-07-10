#ifndef __BLADE_CORE_PLATFORM_MEMUTILITY_H__
#define __BLADE_CORE_PLATFORM_MEMUTILITY_H__

#include <TypeDefine.h>

namespace BladeEngine
{
    #include <memory>

    class MemUtil
    {
    public:
        static void* Memset(void* inDest, byte value, SIZE_T inSize)
        {
            return ::memset(inDest, value, inSize);
        }

        static void* Memcopy(void* inDest, SIZE_T inDestSize, const void* inSrc, SIZE_T inSrcSize)
        {
            return ::memcpy_s(inDest, inDestSize, inSrc, inSrcSize);
        }

        static SIZE_T Memcmp(const void* inSrc1, const void* inSrc2, SIZE_T inSrcSize)
        {
            return ::memcmp(inSrc1, inSrc2, inSrcSize);
        }
    };
}

#endif // !__BLADE_CORE_PLATFORM_MEMUTILITY_H__
