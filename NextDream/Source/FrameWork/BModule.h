#ifndef __BLADE_FRAMEWORK_BMODULE_H__
#define __BLADE_FRAMEWORK_BMODULE_H__

#include <BString.h>

namespace BladeEngine
{
    namespace Framework
    {
        struct IModule
        {
            virtual bool Load(const BString& inFileName) = 0;
            virtual bool StartUp() = 0;
            virtual void ShutDown() = 0;
            virtual void Unload() = 0;
        };
    }
}
#endif // !__BLADE_FRAMEWORK_BMODULE_H__