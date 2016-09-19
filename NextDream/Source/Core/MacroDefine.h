#ifndef __BLADE_CORE_MACRO_DEFINE_H__
#define __BLADE_CORE_MACRO_DEFINE_H__

namespace BladeEngine
{

#if defined(_WIN32) || defined(_WIN64)
    #define WINDOWS_PLATFORM 1
#endif

#if defined(_UNICODE) || defined(UNICODE)
    #undef UNICODE
    #define UNICODE 1
#else
    #undef ANSI
    #define ANSI 1
#endif

}

#endif // !__BLADE_CORE_MACRO_DEFINE_H__
